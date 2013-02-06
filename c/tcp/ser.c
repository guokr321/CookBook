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
#define PORT    5000
#define BLOCK   5

void main(int argc,char *argv[])
{
    int server_sockfd,client_sockfd;
    int server_len,client_len;
    struct sockaddr_in server_add;
    struct sockaddr_in client_add;
    int read_cnt;
    char tmp[4];    //������
    char file_name[MAXSIZE];    //�ļ���
    char send[MAXSIZE];         //�����ļ�
    char recv[MAXSIZE];         //�����ļ�
    int opt =1;
   
    memset(file_name,'\0',MAXSIZE);
    //step1:�����׽���
    if(-1==(server_sockfd=socket(AF_INET,SOCK_STREAM,0)))
    {
        perror("socket Err");
        exit(EXIT_FAILURE);
    }

    //step2:ʵ����
    bzero(&server_add,sizeof(struct sockaddr_in));
    server_add.sin_family = AF_INET;
    server_add.sin_addr.s_addr =htonl(INADDR_ANY);
    server_add.sin_port = htons(PORT);
    server_len = sizeof(server_add);

    setsockopt(server_sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));    //��������socket

    if(bind(server_sockfd,(struct sockaddr *)&server_add,server_len)<0)
    {   
        perror("bind Err");
        exit(EXIT_FAILURE);
    }

    //Step3:����listen����
    if(listen(server_sockfd,BLOCK)<0)
    {
        perror("listen Err");
        exit(EXIT_FAILURE);
    }

    //step4:��ȡaccpet+������Ӧ����
    while(1)
    {
        client_len = sizeof(client_add);
        client_sockfd = accept(server_sockfd,(struct sockaddr *)&client_add,&client_len);
        if(-1 == client_sockfd)
        {
            perror("Accept  Err");
            exit(EXIT_FAILURE);
        }
        printf("now Build the accept!\n");
        //�Ծ����������Ӧ

        read(client_sockfd,tmp,4);
       // read(client_sockfd,file_name,MAXSIZE);
        read(client_sockfd,file_name,MAXSIZE);

        printf("Cnt_Flag:%s,file_name:%s\n",tmp,file_name); 

        //FILE *fd=fopen(file_name,"rb");
        FILE *fd=fopen(file_name,"a+b");
        if(fd == NULL)
        {
            perror("Get Fopen Err");
            exit(EXIT_FAILURE);
        }

        if(0==strcmp(tmp,"get"))
        {
            printf("Client get file from local server!\n");
            while(!feof(fd))
            {
                int len=fread(send,1,MAXSIZE,fd);   //���ļ��ж�ȡ�ַ�����������
                write(client_sockfd,send,len);      //�ӻ�������client_sockfdд����Ϣ
                printf("->");
            }
            printf("Send Success!\n");
        }
        else
        {
            printf("client send file to local server!\n");
            while(1)
            {
                read_cnt = read(client_sockfd,recv,MAXSIZE); //�׽����򻺳���д��
                
                if(0==read_cnt)
                {
                    break;
                }
                
                fwrite(recv,1,read_cnt,fd);     //���ļ�д�뻺��������
                printf("->");
            }
            printf("receive success!\n");
        }
        
        close(client_sockfd);
        fclose(fd);
    }
        
    exit(EXIT_SUCCESS);
}
