 /*=============================================================================
#
# Author: tianweidut - liutianweidlut@gmail.com
#
# QQ : 416774905
#
# Last modified:	2010-11-25 21:29
#
# Filename:		client.c
#
# Description: 
#
=============================================================================*/ 

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>  //AF_INETʹ��
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#define MAXSIZE 100
#define GET  0
#define PUT  1

int main(int argc,char *argv[]) //0:Ӧ�ó��� 1:get/put 2:���� 3:port 4:Զ���ļ��� 5:�����ļ���
{
    int sockfd,len;
    char recvs[MAXSIZE];
    char send[MAXSIZE];
    struct sockaddr_in add;
    struct hostent *host;
    int read_cnt=0;
    char *get_ch = "get";
    char *put_ch = "put";

    if(argc != 6)
    {
        printf("client ip port filename\n");
        printf("Tips:0:Ӧ�ó��� 1:get/put 2:���� 3:port 4:Զ���ļ��� 5:�����ļ���\n");
        exit(EXIT_FAILURE); 
    }
    host = gethostbyname(argv[2]);      //serve��ַת��
    if(-1==(sockfd=socket(AF_INET,SOCK_STREAM,0)))
    {
        perror("socked Err!");
        exit(EXIT_FAILURE);
    }
    bzero(&add,sizeof(add));    //sockʵ��
    add.sin_family = AF_INET;
    add.sin_addr = *((struct in_addr *)host->h_addr);
    add.sin_port = htons(atoi(argv[3])); 
    len = sizeof(add);
    if(-1==connect(sockfd,(struct sockaddr *)&add,len))
    {
            perror("connect Err!");
            exit(EXIT_FAILURE);
    }

    FILE *fd = fopen(argv[5],"a+b");     //create an empty file binary::�����ļ��洢
    if(fd == NULL)
    {
        perror("file create failure!");
        exit(EXIT_FAILURE);
    }

    if(strcmp(argv[1],"get")==0) //���
    {
        //��ȡ�ļ�
        printf("\n Get the File:%s From Ip:%s:%s,Copy the local file:%s\n",argv[4],argv[2],argv[3],argv[5]);
        
        //д�������Ϣ
        write(sockfd,get_ch,sizeof(get_ch));        //��������

      //  write(sockfd,argv[4],MAXSIZE);      //�����ļ���
        write(sockfd,argv[4],strlen(argv[4]));      //�����ļ���
        
        printf("file len:%d\n",strlen(argv[4]));
        //�����ļ�
        printf("\nNow:reve the file\n");
        while(1)
        {
            read_cnt = read(sockfd,recvs,MAXSIZE);
            if(0 == read_cnt)
            {
                break;
            }
            printf("->");
            fwrite(recvs,1,read_cnt,fd);
        }
        printf("\nSuccess!\n");

    }
    else
    {
        //�ϴ��ļ�
        printf("\n Put the File:%s From Ip:%s:%s,Copy the local file:%s\n",argv[4],argv[2],argv[3],argv[5]);
        
        //д�������Ϣ
        write(sockfd,put_ch,sizeof(get_ch));        //�ϴ�����
        write(sockfd,argv[4],strlen(argv[4]));      //�洢�ļ���
        
        //�����ļ�
        printf("\nNow:Send the file\n");
        while(!feof(fd))
        {
            int len=fread(send,1,MAXSIZE,fd);
            write(sockfd,send,len);
            printf("->");
        }         
        printf("\nsuccess!\n");
    }

    close(sockfd);
    fclose(fd);
    exit(EXIT_SUCCESS);
}
