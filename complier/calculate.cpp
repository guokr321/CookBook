// Calculator.cpp : Defines the entry point for the console application.
//


///////////////////////////////////////////////////////////////////////////////
//		������������ʽ���ķ�����
//		E-->AE'
//		E'-->+AE'|e
//		A-->BA'
//		A'-->-BA'|e
//		B-->TB'
//		B-->/TB'|e
//		T-->FT'
//		T'-->*FT'|e
//		F-->(E)|num
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <stdlib.h>
#include <string>
#include <stdio.h>
using namespace std;

// ��һ���ǼǺŵĶ���
#define ADD 0
#define MUL 1
#define LBRACE 2
#define RBRACE 3
#define NUM 4
#define END 5
#define OTHER 6
//�����
#define SUB 7
#define DIV 8

//char input[200];	// ���봮��
string input;

int lookahead;
int pCur;
int yylval;

// ��������
int yylex();
void Match(int t);
long T();
long E_();
long E();
long A();
long A_();
long B();
long B_();
long T_();
long F();

// �ʷ�������������һ���Ǻ�
int yylex()
{
	char num[20];
	int temp = 0;

	// ���˵��հ�
	while ( input[pCur]==' ' ) pCur++;

	// ��������֣���ô������Ǻŵ���ֵ����������� yylval ��
	while (input[pCur] >= '0' && input[pCur] <= '9'){
		num[temp++] = input[pCur++];
	}
	if (temp >0) 
	{
		sscanf(num, "%d", &yylval);
		return NUM;
	}

	// �����ǺŵĴ���
	switch (input[pCur++])	// ע�⣺����ָ����ǰ����һλ
	{
	case '+': return ADD;
	case '*':return MUL;
	case '(':return LBRACE;
	case ')':return RBRACE;
	case '\0': return END;
	case '-': return SUB;
	case '/': return DIV;
	default: return OTHER;
	}
}

// ƥ�亯��������ǰ�Ǻ��������ͬ���������һ���Ǻ�
void Match(int t)
{
	if (lookahead == t) lookahead = yylex();
	else 
	{
		printf("\n Error\n");

		exit(0);
	}
}

 
long E()
{
	switch (lookahead)
	{
	case LBRACE:	// FIRST(TE')={(,num}
	case NUM:
		return A() + E_();
	default:
		printf("\n Error\n");
		exit(0);
	}		
}

// ���� E'-->+AE'|e
long E_()
{
	switch (lookahead)
	{
	case ADD:	// E'-->+TE' ������� FIRST(E')={+,e}
		Match(ADD); return A() + E_();

	case RBRACE:// E'-->e ����������ʱ����Ҫ���� FOLLOW���ϣ� FOLLOW(E')={), $}
	case SUB:
	case DIV:
	case END:
		return 0;
	default:
		printf("\n Error\n");

		exit(0);
	}	
}
long A()
{
	switch (lookahead)
	{
	case LBRACE:	// FIRST(TE')={(,num}
	case NUM:
		return B() - A_();
	default:
		printf("\n Error\n");

		exit(0);
	}		
}
long A_()
{
	switch (lookahead)
	{
	case SUB:	 
		Match(SUB); return B() - A_();

	case RBRACE: 
	case ADD:
	case DIV:
	case END:
		return 0;
	default:
		printf("\n Error\n");

		exit(0);
	}	
}
long B()
{
	switch (lookahead)
	{
	case LBRACE:	// FIRST(TE')={(,num}
	case NUM:
		return T() / B_();
 
	default:
		printf("\n Error\n");

		exit(0);
	}		
}
long B_()
{
	switch (lookahead)
	{
	case DIV:	// FIRST(*FT')={*}
		Match(DIV);
		return T() /B_();
	case ADD:	// T'-->e ����������ʱ����Ҫ���� FOLLOW���ϣ� FOLLOW(T')={+,),$}
	case SUB:
	case MUL:
	case RBRACE:
	case END:
		return 1;
	default:
		printf("\n Error\n");

		exit(0);
	}	
}
long T()
{
	switch (lookahead)
	{
	case LBRACE:	// FIRST(FT')={(,num}
	case NUM:
		return F()*T_();
	default:
		printf("\n Error\n");

		exit(0);
	}
}
// ���� T'-->*FT'|e
long T_()
{
	switch (lookahead)
	{
	case MUL:	// FIRST(*FT')={*}
		Match(MUL);
		return F() * T_();
	case ADD:	// T'-->e ����������ʱ����Ҫ���� FOLLOW���ϣ� FOLLOW(T')={+,),$}
	case SUB:
	case DIV:
	case RBRACE:
	case END:
		return 1; 
	default:
		printf("\n Error\n");

		exit(0);
	}		
}

// ���� F-->(E)|num
long F()
{
	int temp;

	switch(lookahead)
	{
	case LBRACE:	// FIRST((E))={(}
		Match(LBRACE);
		temp = E();
		Match(RBRACE);
		return temp;
	case NUM:		// FIRST(num) = {num}
		temp = yylval;
		Match(NUM);
		return temp;
	default:
		printf("\n Error\n");

		exit(0);		
	}
}


int main(int argc, char* argv[])
{
	bool again_ask=true;
	char a;

	while (again_ask)
	{
			pCur = 0;

			// �������봮
			cout<<"please input your expression:";
			cin>>input;

			// lookahead ����ֵ
			lookahead = yylex();

			// ���� ��ʼ����E ��Ӧ�Ĵ���������������봮
			cout<<"the answer is "<<E()<<endl;

			cout<<"Y/N:";
			cin>>a;
			
			again_ask=((a=='Y'||a=='y')?1:0);

	}
	cout<<"welcome to the next!\n";
	return 0;
}



