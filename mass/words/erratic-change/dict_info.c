#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "dict_info.h"

//�ʵ����������һ��DICT_INFO�ṹ��ָ��
//��ȷ������Ϣ�ļ��ṹ�壬���󷵻�NULL
DICT_INFO *getDictInfo(char *file)
{
    FILE *ifo;
    char *line;
    char buffer[BUFSIZE];
    DICT_INFO *dict =(DICT_INFO *)malloc(sizeof(DICT_INFO));
    if(dict==NULL)
    {
        perror("error Dict");
        exit(EXIT_FAILURE);
    }
    if((ifo = fopen(file,"r+"))==NULL)
    {
        perror("fopen ifo!");
        exit(EXIT_FAILURE);
    }
    
    while((line=fgets(buffer,BUFSIZE,ifo))!=NULL)
    {
        parseLine(line,dict);
    }
    fclose(ifo);
    
    return dict;    
}
//����ÿ��,����Ϣװ���ض��ֶ�
static void parseLine(char *line,DICT_INFO *dict)
{
    char *idx;
    if((idx=strchr(line,'='))!=NULL) //����������ʼ���ض��ַ���
    {
        if(strstr(line,"version")!=NULL)
        {
            strcpy(dict->ver,idx+1);
        }
        else if(strstr(line,"wordcount")!=NULL)
        {
            dict->wordcnt = atoi(idx+1);
        }
        else if(strstr(line,"idxfilesize")!=NULL)
        {
            dict->idxFileSize=atoi(idx+1);
        }
        else if(strstr(line,"bookname")!=NULL)
        {
            strcpy(dict->bookname,idx+1);
        }
        else if(strstr(line,"sametypesequence")!=NULL)
        {
            strcpy(dict->sameTypeSequence,idx+1);
        }
        else
        {
            strcat(dict->other,line);
        }
    }
}

#if  DICT_DEBUG
int main(int argc,char **argv)
{
    DICT_INFO *tmp=getDictInfo("powerword2007_pwdnnjsj.ifo");
    if(tmp==NULL)
    {
        perror("get info err(main)");
        exit(EXIT_FAILURE);
    }
    
    
    printf("version:%s",tmp->ver);
    printf("bookname:%s",tmp->bookname);
    printf("wordcount:%d\n",tmp->wordcnt);
    printf("idxfilesize:%d\n",tmp->idxFileSize);
    printf("sts:%s\n",tmp->sameTypeSequence);
    printf("%s",tmp->other);
    free(tmp);

    exit(EXIT_SUCCESS);

}

#endif



