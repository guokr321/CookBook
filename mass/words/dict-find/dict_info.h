#ifndef _DICT_INFO_H
#define _DICT_INFO_H

#define DICT_DEBUG   1   //���Կ���
#define BUFSIZE 5000    //�л�����
#define IFO_EXT ".ifo"  //ifo�ļ���׺

typedef struct 
{
    char ver[100];      //�汾��Ϣ
    int wordcnt;        //��������
    int idxFileSize;    //�����ļ���С
    char bookname[100]; //�ʵ�����
    char sameTypeSequence[10];  //
    char other[1000];   //������Ϣ

}DICT_INFO;

//�ʵ����������һ��DICT_INFO�ṹ��ָ��
DICT_INFO *getDictInfo(char *file);
//����ÿ��
static void parseLine(char *line,DICT_INFO *dict);

#endif  //end the _DICT_INFO_H
