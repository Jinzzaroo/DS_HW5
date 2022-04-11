#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_STACK_SIZE 10	   // stack 최대 개수
#define MAX_EXPRESSION_SIZE 20 //수식의 최대 길이

/* stack 내에서 우선순위는 내림차순, lparen = 0 가장 낮음 */
typedef enum
{
	lparen = 0, /* ( 왼쪽 괄호 */
	rparen = 9, /* ) 오른쪽 괄호*/
	times = 6,	/* * 곱셈 */
	divide = 6, /* / 나눗셈 */
	plus = 5,	/* + 덧셈 */
	minus = 5,	/* - 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];	  // infix 수식
char postfixExp[MAX_EXPRESSION_SIZE]; // postfix 수식
char postfixStack[MAX_STACK_SIZE];	  // postfix 스택
int evalStack[MAX_STACK_SIZE];		  // 계산식 스택

int postfixStackTop = -1; // postfix 스택 중 가장 최근 스택
int evalStackTop = -1;	  //계산식 스택 중 가장 최근 스택

int evalResult = 0;

void postfixPush(char x);		  // postfix 스택에 저장
char postfixPop();				  // postfix 스택에서 LIFO 형식으로 추출
void evalPush(int x);			  //계산식 스택에 저장
int evalPop();					  //계산식 스택에서 추출
void getInfix();				  // infix 식 입력
precedence getToken(char symbol); // Token 선택 후 분류
precedence getPriority(char x);	  // Token의 우선순위 값 반환
void charCat(char *c);			  // postfix exp로 출력
void toPostfix();				  // infix를 postfix로 변환
void debug();
void reset();
void evaluation();

int main()
{
	char command;

	printf("\n[----- [Annotations by JinYeongKim] [2019038030] -----]\n");

	do
	{
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch (command)
		{
		//infix 입력
		case 'i':
		case 'I':
			getInfix();
			break;
		//infix -> postfix
		case 'p':
		case 'P':
			toPostfix();
			break;
		//postfix에서 연산
		case 'e':
		case 'E':
			evaluation();
			break;
		//디버그
		case 'd':
		case 'D':
			debug();
			break;
		//수식 리셋
		case 'r':
		case 'R':
			reset();
			break;
		//종료
		case 'q':
		case 'Q':
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	} while (command != 'q' && command != 'Q');

	return 1;
}

// postfix에서 스택에 연산자 저장
void postfixPush(char x)
{
	postfixStack[++postfixStackTop] = x; //[0] 부터 연산자 저장
}

// postfix에서 스택에 저장되어 있는 연산자 추출
char postfixPop()
{
	char x;
	// postfixStack에 값이 들어있지 않을 때
	if (postfixStackTop == -1)
		return '\0';
	//postfixStack에서 top 스택에 있는 연산자 추출 후 top 인덱스 -1
	else
	{
		x = postfixStack[postfixStackTop--];
	}
	return x;
}

//계산식에서 스택에 피연산자 저장
void evalPush(int x)
{
	evalStack[++evalStackTop] = x;
}

//계산식의 top 스택에 저장되어 있는 피연산자 추출 후 top 인덱스 -1
int evalPop()
{
	if (evalStackTop == -1)
		return -1;
	else
		return evalStack[evalStackTop--];
}

/*
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */

//중위 표기식 입력
void getInfix()
{
	printf("Type the expression >>> ");
	scanf("%s", infixExp);
}

//토큰 분류
precedence getToken(char symbol)
{
	switch (symbol)
	{
	case '(':
		return lparen;
	case ')':
		return rparen;
	case '+':
			 return plus;
	case '-':
		return minus;
	case '/':
		return divide;
	case '*':
		return times;
	default:
		return operand;
	}
}

//토큰의 우선순위 값 반환
precedence getPriority(char x)
{
	return getToken(x);
}

/*
 * 문자 하나를 전달받아, postfixExp에 차례대로 추가
 */
void charCat(char *c)
{
	if (postfixExp == '\0')
		strncpy(postfixExp, c, 1);
	else
		strncat(postfixExp, c, 1);
}

/*
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix()
{
	/* infixExp의 문자 하나씩을 읽기위한 포인터 */
	char *exp = infixExp;
	char x; /* 문자하나를 임시로 저장하기 위한 변수 */

	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	while (*exp != '\0')
	{
		//만약 피연산자일 경우 바로 출력
		if (getPriority(*exp) == operand)
		{
			x = *exp;
			charCat(&x);
		}
		//좌괄호일 경우 스택에 저장
		else if (getPriority(*exp) == lparen)
		{
			postfixPush(*exp);
		}
		//우괄호일 경우 현재 스택에서 좌괄호가 나올 때까지 스택에 있는 연산자 출력
		else if (getPriority(*exp) == rparen)
		{
			while ((x = postfixPop()) != '(')
			{
				charCat(&x);
			}
		}
		else
		{
			//그 외 비교하고자 하는 token(*exp)의 우선순위가 top에 있는 token 우선순위보다 낮거나 같다면 탑에 있던 token을 pop시켜서 출력
			while (getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp))
			{
				x = postfixPop();
				charCat(&x);
			}
			//*exp 가 top 스택에 저장
			postfixPush(*exp);
		}
		//식의 토큰 인덱스 증가
		exp++;
	}

	//스택에 저장된 마지막까지 남은 연산자들을 LIFO 형식으로 출력
	while (postfixStackTop != -1)
	{
		x = postfixPop();
		charCat(&x);
	}
}

//디버깅
void debug()
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);
	printf("postExp =  %s\n", postfixExp);
	printf("eval result = %d\n", evalResult);
	printf("postfixStack : ");
	for (int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]);
	printf("\n");
}

//수식 초기화
void reset()
{
	for (int i = 0; i < MAX_EXPRESSION_SIZE; i++)
	{
		infixExp[i] = '\0';
		postfixExp[i] = '\0';
	}

	for (int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0';

	postfixStackTop = -1;
	evalStackTop = -1;
	evalResult = 0;
}

//계산
void evaluation()
{
	int opr1, opr2, i;	//피연산자 2개 선언

	int length = strlen(postfixExp);
	char symbol;
	evalStackTop = -1;

	//수식의 길이만큼 수행한다.
	for (i = 0; i < length; i++)
	{
		symbol = postfixExp[i];

		//symbol이 피연산자일 경우, 수로 표현된 '문자'를 실제 계산 가능한 '수'로 변환하기 symbol - '0'을 해준다.
		if (getToken(symbol) == operand)
		{
			evalPush(symbol - '0');	//스택에 삽입
		}
		//스택에 들어온 가장 최근의 피연산자 -> top 스택에 저장
		else
		{
			opr2 = evalPop();	//계산식 top 스택의 피연산자
			opr1 = evalPop();	//계산식 top - 1 스택의 피연산자
			//연산식(+, -, ...)이 들어올 경우 두 피연산자와 연산을 한 후 그 값을 다시 top 스택에 저장
			//즉 연산식이 들어오면 -> 연산식이 들어온 기준으로 직전 두 개의 피연산자끼리 연산 -> 그 결과값을 다시 피연산자로 스택에 저장
			switch (symbol)
			{
			case '+':
				evalPush(opr1 + opr2);
				break;
			case '-':
				evalPush(opr1 - opr2);
				break;
			case '*':
				evalPush(opr1 * opr2);
				break;
			case '/':
				evalPush(opr1 / opr2);
				break;
			default:
				break;
			}
		}
	}
	evalResult = evalPop();
}