# -*- coding: UTF-8 -*-
'''
Created on 2012-12-24

@author: tianwei
'''
from math import e

import numpy as np
import dumbo

from common import debug, load_w_matrix, get_sep

__author__ = "tianwei"
__date__ = "December 24 2012"
__description__ = "el-svm for the whole splited block data"


class Mapper():
    def __init__(self):
        """
        """
        self.w_matrix = load_w_matrix(self.params["w_matrix_filename"])
        self.SEP = None

    def calculate(self, point):
        """
        Calculate Program

        Inputs:
            point, a single-original line data

        Outputs:
            localH: a(T) * a
            localD: a * y
        """
        y = int(point[-1])
        point[-1] = 1
        point = np.dot(point, self.w_matrix)
        point_list = map(lambda x: 1 / (1 + pow(e, -x)), point.tolist())
        point = np.array(point_list + [-1], dtype=np.float64)
        point.shape = (1, int(np.shape(point)[0]))

        return (np.dot(point.T, point), y * point.T)

    def __call__(self, data):
        """
        Mapper Program:

        Inputs:
            data, which is the whole split block data

        Outputs:
            key: untified id
            value: resultD,resultH
        """

        # SETP1: read data matrix and do some transpose
        resultH = None
        resultD = None

        for docID, doc in data:
            for term in doc.split("\n"):
                self.SEP = self.SEP if self.SEP is not None else get_sep(term)
                point = np.fromstring(term, dtype=np.float64, sep=self.SEP)
                (localH, localD) = self.calculate(point)
                if resultH is not None:
                    resultH = resultH + localH
                    resultD = resultD + localD
                else:
                    resultH = localH
                    resultD = localD

        debug(np.shape(resultH))
        debug(np.shape(resultD))

        yield "nonused", (resultD.tolist(), resultH.tolist())


class Reducer():
    def __init__(self):
        """
        """
        self.w_matrix = load_w_matrix(self.params["w_matrix_filename"])

    def __call__(self, key, values):
        """
        Reducer Program: generate the model arguments

        Inputs:
            key: untified id
            values: resultD and resultH

        Outputs:
            model arguments
        """
        globalD = None
        globalH = None

        for value in values:
            debug(np.shape(value[1]))
            debug(np.shape(value[0]))
            globalD = globalD + np.array(value[0]) if globalD is not None else np.array(value[0])
            globalH = globalH + np.array(value[1]) if globalH is not None else np.array(value[1])

        debug("global")
        debug(np.shape(globalH))
        debug(np.shape(globalD))

        yield "nonused", (globalD.tolist(), globalH.tolist())


if __name__ == "__main__":
    dumbo.run(Mapper, Reducer, Reducer)
