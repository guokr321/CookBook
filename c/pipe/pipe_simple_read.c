#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc,char **argv)
{
    int data_processed;
    char buf[BUFSIZ+1];
    char file_desc = 0;
    memset(buf,'\0',sizeof(buf));

    //1:���sccanf�ǽ�������������д������ʽ���ַ���
    //2:���������ַ�����
    sscanf(argv[1],"%d",&file_desc);    
    data_processed = read(file_desc,buf,BUFSIZ);
    printf("%d - read %d bytes:%s\n",getpid(),data_processed,buf);

    exit(EXIT_SUCCESS);
}

