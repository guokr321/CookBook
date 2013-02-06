#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>   //tolower
#include <sys/types.h>
#include <sys/stat.h>

#include "dict_idx.h"   //�����򶯴�����

#define DEBUG 1     //���Կ��ذ�ť
#define WORDNum     200     //���е�������
#define WORDLen     20      //ÿ��������󳤶�

#define YES     1
#define NO      0

const char *filename = _IDX_NAME;
const char *dictname = _DICT_NAME;

struct wordFlag
{
    int pos;    //����������λ��
    int flag;   //�Ƿ�ת�� 
};

int explainWord(char *src,WORD_IDX *word);

int main(void)
{
    int c=0;
    char *cpBuffer=NULL;    //�����ַ���,δָ����С���ؼ��㣩
    int num=0;
    FILE *inFile=NULL;      //ԭʼд���ļ�ָ��
    FILE *outFile=NULL;     //�����ļ�ָ��
    FILE *writeFile=NULL;   //���д���ļ�
    long FileSize=0;           //�ļ�����
    char *readBuffer=NULL;  //��ȡ�ļ��ַ���ָ��
    int wordsum;            //Ԥ�ⵥ�ʳ���
    int wordcnt=0;
    int alpha = 0;
    WORD_IDX idx[60];
    
    printf("===============words recongnize=====================\n");
    printf("Step1:-------->end the char:~\n");

    //step1:���ݻ�������С����̬��������
    c=getchar();  
    for( num=0;num<BUFSIZ;num++)
    {
        if(*(stdin->_IO_buf_base+num)=='~')   //������~ʱ��������
        {
            break;
        }
    }
    
    //����ռ�
    if((cpBuffer=(char*)malloc(num*sizeof(char)+1))==NULL)
    {
        perror("CpBuffer malloc address!\n");
        exit(EXIT_FAILURE);
    }
    //�ַ������
    memcpy(cpBuffer,stdin->_IO_buf_base,(num?num:1));   //
    *(cpBuffer+num)='\0';

#if DEBUG
    printf("<------------------------------------------>\n");
    printf("%s\n",cpBuffer);
#endif
    
    //step2:�����ļ�
   if((inFile=fopen("input.txt","a+"))==NULL)
   {
       printf("\n error in:input.txt create\n");
       perror("fopen error!");
       exit(EXIT_FAILURE);
   } 
   fwrite(cpBuffer,strlen(cpBuffer),1,inFile);
   fclose(inFile);

    //stpe3:�ļ��ж�ȡ(��������)
    if((outFile=fopen("input.txt","r"))==NULL)
    {
        printf("\nerror in:input open\n");
        perror("foen error!2");
        exit(EXIT_FAILURE);
    }
    
   // fseek(outFile,0,SEEK_END);      //��ȡ�ļ�����
   // FileSize = ftell(outFile);
   // rewind(outFile);                
    //FileSize --> �ַ�����
  //  wordsum = FileSize;          

    //�˴�FileSizeΪ�ļ����ַ�����
    char *words[WORDNum+1];    //�˴����Ը��������ַ���Ŀ+���ʳ��Ƚ��м��㣬�����˷Ѳ���Ҫ�Ŀռ�
                               //�洢ÿ���Ϸ���Ԫ 
    char *wordsTmp[WORDNum+1]; //�仯֮��Ĵ��δ洢
    for(int i=0;i<=WORDNum;i++)
    {
        if((words[i]=(char *)malloc(sizeof((char*)WORDLen)))==NULL)
        {
            perror("get word!");
            exit(EXIT_FAILURE);
        }
        words[i][0]=0; //��������ĸ����
        if((wordsTmp[i]=(char*)malloc(sizeof((char*)WORDLen)))==NULL)
        {
            perror("get word!2");
            exit(EXIT_FAILURE);
        }
    } 
    
   //step3:�Ϸ�������ȡ
   //���򣺹��ˣ����֣���㣬�ո��»��ߵ�,����'-'������
   //���䣺ʵ�����Ĺ���
   wordcnt = 0;     //�Ϸ����ʼ���,����ΪWORDNum, ��0��ͷ��Ϊ�˷�ֹ��ͷ���ֿո��ַ�
   alpha = 0;       //ÿ������
  
   c = fgetc(outFile);
   do
   {
    //��������
    if(c&0x80)  //��������ASCII�ַ�
    {
        c = fgetc(outFile);     //�൱�ڹ��˵���һ���ַ�
        c = fgetc(outFile);     //++����,����߽��ж�
    }
    //�Ϸ���ĸ�ж�
    if(!(((c>='A')&&(c<='Z'))||   //A~Z
       ((c>='a')&&(c<='z'))||   //a~z
       (c=='-')))                //-
    {
        //�Ƿ�����,��ʱ��������ͳ��
        c = fgetc(outFile);     //�����ж�
        if(c==EOF)
        {
             break; //��ֹ�����ĩβ��Ϊ������һ���Ƿ��ַ��ͽ���
        }
        while(1)
        {
            if(((c>='A')&&(c<='Z'))||   //A~Z
               ((c>='a')&&(c<='z'))||   //a~z
               (c=='-'))                //-    :�˴�����Bug��-�����ڵ��ʿ�ͷλ�ã���Ҫ���Ժ����й���
            {
               //��ʱ���ָ����һ������
               if(0==wordcnt)
               {
                   //�ļ���ͷ�����Ƿ��ַ�
                   wordcnt=1; //��ʼ����
                   break;
               }
               words[wordcnt][++alpha]='\0';    //����߽������ж� 
               wordcnt++;  //������Ŀ
               alpha=0;    
               break; //�¸����ʻ�ȡ
            }
            //��ʱ����Ϊ�Ƿ��ַ�,�������ж�ȡ
            c = fgetc(outFile);
            if(c==EOF)
            {
                break; //��ֹ�����ĩβ��Ϊ�����Ƿ��ַ�
            }
        }//end of while

    }//end of if
    else
    {
        //��ĸ�Ϸ�,���ɵ���
        //bug:-��ͷ����
        while((0==alpha)&&(c=='-'))     //��ֹ��������-��ͷ
        {
            c = fgetc(outFile);
            if(c==EOF)
            {
                break;
            }
        }
        alpha++;                //��¼ÿ���������еĺϷ��ַ���Ŀ
        words[wordcnt][0]++;    //��¼����
        //words[wordcnt][alpha]=c;
        words[wordcnt][alpha]=tolower(c); //ת����Сд��ĸ
        c = fgetc(outFile);
    }//end of else

   }while(c != EOF);

   //��β����
    words[wordcnt][++alpha]='\0';
    printf("\n<------------------------------------------>\n");
    printf("the word numbers are %d\n",wordcnt);
    for(int i=1;i<=wordcnt;i++)
    {
       printf("num:%d\t",words[i][0]);
       printf("%s\n",(words[i]+1));       //��һ���ַ���Ҫ��ӡ��������ֲ��ɼ��ַ���ӡ
    }
    printf("\ni!!!!\n");
    fclose(outFile);
   
    //��ʱ����ַ���ɾ������
    //step4:���Ա仯
    //step4.1 ��������
    struct wordFlag *flagArr;
    if((flagArr=(struct wordFlag*)malloc(sizeof((struct wordFlag *)(wordcnt+1))))==NULL)
    {
        perror("flag Arr");
        exit(EXIT_FAILURE);
    }

    for(int i=1;i<=wordcnt;i++)
    {
        for(int j=0;j<=(words[i][0]+1);j++)  //���ܺ���'\0'
        {
            wordsTmp[i][j]=words[i][j];
        }
        flagArr[i].flag = NO;   //δ����
        flagArr[i].pos = i;     //����λ��
    }
    //step4.2 �����仯
    int len=0;

    DICT_INFO dict;     //�ʵ�ԭ�ṹ��
    dict.wordcnt = WORDSUM;     //��������
    dict.idxFileSize = WORDIDXSIZE;     //idx�ļ���С
    
    //WORD_IDX *idx=(WORD_IDX*)malloc((sizeof(WORD_IDX))*(dict.wordcnt));
    
    WORD_IDX *word=NULL;     //�ʻ�����
    if(NULL==idx)
    {
        perror("idx malloc free");
        printf("!!!!\n");
       //exit(EXIT_FAILURE);
    }
    if(getWords(filename,&dict,idx)==NULL)   //��������
    {
        printf("Error!!! GetWords!\n");
        getchar();
    }

    for(int i=1;i<=wordcnt;i++)     //��ÿ�������ַ�������
    {
        len = wordsTmp[i][0];
        if((len>=4)                     //��̵��ʳ����жϣ���ֹ֮��ĵ��ʳ��ȱ�ɸ�
          &&(wordsTmp[i][len]=='d')
          &&(wordsTmp[i][len-1]=='e'))
        {
            //��ʱ����ʾ�Ѿ�Ϊ���ʣ���ȥʱ����ȥ�ִ�
            flagArr[i].flag = YES;     //�Ѽ��
            if(wordsTmp[i][len-2]=='r')
            {
                //��-r��β�Ĵʣ�˫дr��ĸ+ed
                wordsTmp[i][len-2]='\0';
                wordsTmp[i][0]-=3;
            }
            else if(wordsTmp[i][len-2]=='i')
            {
                //�Ը�����ĸ+y��β�Ĵʣ���yΪi+ed
                wordsTmp[i][len-2]='y';
                wordsTmp[i][len-1]='\0';
                wordsTmp[i][0]-=2;
            }
            else if(wordsTmp[i][len-2]==words[i][len-3])
            {
                //ĩβֻ��һ��������ĸ��β���ضȱ�����,˫д�ø�����ĸ
                wordsTmp[i][len-2]='\0';
                wordsTmp[i][0]-=3;
            }
            else if((wordsTmp[i][len-3]=='a')//||(wordsTmp[i][len-3]=='')  //�˴�����϶�
                    ||(wordsTmp[i][len-3]=='i')||(wordsTmp[i][len-3]=='o')
                    ||(wordsTmp[i][len-3]=='u'))
            {
                //��e��β��+d:�Ե�����������ĸ��Ԫ����ĸ��eɾ��
                wordsTmp[i][len]='\0';
                wordsTmp[i][0]--;
            }
            else if(wordsTmp[i][len-2]=='e')
            {
                //��e��β��+d:˫дee��β��
                wordsTmp[i][len]='\0';
                wordsTmp[i][0]--;
            }
            else
            {
                //������򶯴ʱ仯ʱ���������:looked,hoped�ȴʻ�
                word=getIdx((wordsTmp[i]+1),idx,&dict);
                if(NULL!=word) //�����ͣ�δ���
                {
                    if(0!=explainWord((wordsTmp[i]+1),word))
                    {
                        //�ɹ�����
                        wordsTmp[i][0]=word->length;
                    }
                }
                else
                {
                    //��ԭ����ĸ+y��β�Ĵʣ�ֱ�Ӽ�+ed
                    //һ���ڶ���ԭ��+ed
                    wordsTmp[i][len-1]='\0';
                    wordsTmp[i][0]-=2;
                }

            }
        }//if
        else
        {
            //�˴���չ�����ڷǱ�׼��ת��
            //���ǲ��ú������ݽṹ���ֵ�������
            word = getIdx((wordsTmp[i]+1),idx,&dict);
            if(NULL!=word)
            {
                if(0!=explainWord((wordsTmp[i]+1),word))
                {
                    //�ɹ�����
                    wordsTmp[i][0]=word->length;
                }
            }
        }
    }    
    //step5:���ţ�ͳ�Ʒ�������
    printf("<-------------------------------------->\n");
    for(int i=1;i<=wordcnt;i++)
    {
       printf("num:%d\t",words[i][0]);
       printf("%s\t\t\t-->",(words[i]+1));       //��һ���ַ���Ҫ��ӡ��������ֲ��ɼ��ַ���ӡ
       printf("num:%d\t",wordsTmp[i][0]);
       printf("%s\n",(wordsTmp[i]+1));
    }
    printf("\n");

    //�������
    free(readBuffer);       
    free(cpBuffer);
    //free(idx);
    
    exit(EXIT_SUCCESS);

}
int explainWord(char *src,WORD_IDX *word)
{
    FILE *dictfile = fopen(dictname,"r+");
    char explain[word->length+1];
    memset(explain,'\0',word->length+1);

    if(NULL==dictfile)
    {
        perror("dict open");
        return 0;   //fail
    }
    if(0!=fseek(dictfile,word->offset,SEEK_SET))
    {
        perror("fseek open");
        return 0;
    }
    fread(explain,word->length,1,dictfile);

#if DEBUG
    printf("TEST:---->%s\n",explain);
#endif 
    strncpy(src,explain,word->length+1);
    *(src+word->length+1)='\0';  //����û�б�Ҫ
    
    return 1;   //�ɹ�����   
}

