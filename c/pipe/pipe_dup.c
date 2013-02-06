#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    int data_processed = 0;
    int file_pipes[2];
    const char some_data[] = "123456";
    char buf[BUFSIZ+1];
    pid_t pid_rst = 0;

    memset(buf,'\0',sizeof(buf));

    if(pipe(file_pipes) == 0)
    {
        pid_rst = fork();
        if(pid_rst == -1)
        {
            perror("fork error");
            exit(EXIT_FAILURE);
        }
        else if(pid_rst == 0)
        {
            close(0);           //step1:�رձ�׼����
            dup(file_pipes[0]); //step2:��ͬһ���ܵ����ļ�
            close(file_pipes[0]);
            close(file_pipes[1]);
            //���ӽ����д��������̲����ݲ�����ʹ��exec
            printf("Pid:%d,in the sub Process\n",getpid());
            (void)execlp("od","od","-c",(char*)0);
            exit(EXIT_SUCCESS);    
        }
        else
        {
            close(file_pipes[0]);
            printf("Pid:%d,in the father Process\n",getpid());
            data_processed = write(file_pipes[1],some_data,sizeof(some_data)); //��ʹ��file_pipes[0],�������Ϊ-1
            printf("wrote %d bytes\n",data_processed);
        }

        exit(EXIT_SUCCESS);
    }
    exit(EXIT_FAILURE);
}
