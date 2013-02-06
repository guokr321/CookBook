 /*=============================================================================
#
# Author: tianweidut - liutianweidlut@gmail.com
#
# QQ : 416774905
#
# Last modified: 2011-04-16 21:29
#
# Filename: pthread_attr.c
#
# Description: 
#
=============================================================================*/ 
//�߳�����
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void * thread_function(void *arg);
char msg[]="hell world!"; 
int thread_finished = 0;
int test = 1;
int main()
{
    int res;
    pthread_t a_thread;
    pthread_attr_t thread_attr;


    //����31-44��ע�͵������Ա��뵫������ʱ���ִ��� Resource temporarily unavailable����ʱ�������ϲ�join
    res = pthread_attr_init(&thread_attr);
    if(res != 0)
    {
        perror("pthread attr init error!");
        exit(EXIT_FAILURE);
    }

    res = pthread_attr_setdetachstate(&thread_attr,PTHREAD_CREATE_DETACHED);
    if(res != 0)
    {
        perror("pthread attr set error!");
        exit(EXIT_FAILURE);
    }
    
    res = pthread_create(&a_thread,&thread_attr,thread_function,(void*)msg);
    if(res != 0)
    {
        perror("pthread create error!");
        exit(EXIT_FAILURE);
    }

    //pthread_attr_destroy(&thread_attr);
    while(!thread_finished)
    {
        printf("waiting for the thread to say!...\n\n");
        sleep(1);
    }
    printf("Other thread is over!%d\n",test);
    exit(EXIT_SUCCESS);
}

void * thread_function(void *arg)
{
    printf("thread_function is running ...Argument was %s\n",(char*)arg);
    sleep(2);
    printf("test....\n");
    thread_finished = 1;
    //sleep(1); �˴�������Դ��룬��ʱ���ʱtest����Ϊ2��֤�������̶߳������У�û�кϲ�
    //��sleep(1)����ʱ���ϳ�ʱ�䲻�ܽ����߳��еġ�test��ʾ���޸ľͽ�����
    test = 2;
    pthread_exit(NULL);
}
