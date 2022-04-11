#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4 // 큐의 최대 크기 4

typedef char element; // char -> element 로 바꾸어 데이터를 element로 인식
typedef struct
{
	element queue[MAX_QUEUE_SIZE]; // 큐에 들어오는 데이터는 element이므로 데이터형 element // MAX_QUEUE_SIZE만큼의 큐 리스트 선언
	int front, rear;			   // 새로운 element가 들어오는 끝 -> rear // element가 나가는 끝 -> front
} QueueType;

QueueType *createQueue();
int freeQueue(QueueType *cQ);
int isEmpty(QueueType *cQ);
int isFull(QueueType *cQ);
void enQueue(QueueType *cQ, element item);
void deQueue(QueueType *cQ, element *item);
void printQ(QueueType *cQ);
void debugQ(QueueType *cQ);
element getElement();

int main(void)
{
	QueueType *cQ = createQueue(); // cQ 큐 리스트 선언
	//큐 값 초기화
	for (int i = 0; i < MAX_QUEUE_SIZE; i++)
	{
		cQ->queue[i] = '\0';
	}
	element data; // data 원소 선언

	char command;

	printf("\n[----- [Annotations by JinYeongKim] [2019038030] -----]\n");

	do
	{
		printf("-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch (command)
		{

		// 원소 삽입
		case 'i':
		case 'I':
			data = getElement();
			enQueue(cQ, data);
			break;

		// 원소 삭제
		case 'd':
		case 'D':
			deQueue(cQ, &data);
			break;

		//큐 리스트 출력
		case 'p':
		case 'P':
			printQ(cQ);
			break;

		// 디버깅
		case 'b':
		case 'B':
			debugQ(cQ);
			break;

		// free and quit
		case 'q':
		case 'Q':
			printf("Free queue memory...\n");
			freeQueue(cQ);
			break;

		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	} while (command != 'q' && command != 'Q');

	return 1;
}

// 큐 리스트 생성
QueueType *createQueue()
{
	QueueType *cQ;
	cQ = (QueueType *)malloc(sizeof(QueueType)); // 동적 할당 배열을 이용하는 원형 큐

	// 초기 큐 리스트의 front와 rear를  index 0 으로 동일하게 설정
	cQ->front = 0;
	cQ->rear = 0;
	return cQ; // 생성한 큐 리스트의 주소 반환
}

// 큐 리스트의 할당된 메모리 반환
int freeQueue(QueueType *cQ)
{
	if (cQ == NULL)
		return 1; // cQ에 메모리 할당이 안 되어있다면 굳이 free 할 필요 x
	free(cQ);	  // 메모리 반환
	return 1;
}

// 원소 선택
element getElement()
{
	element item; // item 원소 (실은 char형)
	printf("Input element = ");
	scanf(" %c", &item);
	return item; // 삽입한 item 원소값 반환
}

// 큐 리스트가 공백인지 검사
int isEmpty(QueueType *cQ)
{
	// cQ의 front와 rear가 같으면 공백으로 판단하고 1 반환 (True)
	if (cQ->front == cQ->rear)
	{
		printf("Circular Queue is empty!");
		return 1;
	}
	else
		return 0; // front != rear -> 0 반환 (False)
}

// 큐 리스트가 만원인지 검사
int isFull(QueueType *cQ)
{ // front 바로 앞까지 rear가 도달했을 때 만원으로 판단 (rear + 1 의 인덱스가 front 인덱스가 될 때)
	if (((cQ->rear + 1) % MAX_QUEUE_SIZE) == cQ->front)
	{
		printf(" Circular Queue is full!");
		return 1;
	}
	else
		return 0;
}

// 원소 삽입
void enQueue(QueueType *cQ, element item)
{
	// 큐 리스트가 만원이 아닐 때 실행
	if (isFull(cQ))
		return;
	else
	{
		// 현재 rear 인덱스에서 +1 하여 다음 큐가 rear가 되게끔 하고 이동한 큐에 원소 삽입
		cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE;
		cQ->queue[cQ->rear] = item;
	}
}

// 원소 삭제
void deQueue(QueueType *cQ, element *item)
{
	// 큐 리스트가 공백이 아닐 때 실행
	if (isEmpty(cQ))
		return;
	else
	{
		cQ->front = (cQ->front + 1) % MAX_QUEUE_SIZE; // 현재 front 인덱스에서 +1 하여 다음 큐가 front가 되게끔 한다.
		*item = cQ->queue[cQ->front];				  // 현재 front에 있는 원소값으로 item 원소 초기화
		cQ->queue[cQ->front] = '\0';
		return;
	}
}

// 큐 리스트 출력
void printQ(QueueType *cQ)
{
	int i, first, last;

	first = (cQ->front + 1) % MAX_QUEUE_SIZE; // front + 1 부터가 유효한 원소이므로 그 index를 first로 설정
	last = (cQ->rear + 1) % MAX_QUEUE_SIZE;	  // rear + 1 부터는 유효한 원소가 없으므로 그 index를 last로 설정

	printf("Circular Queue : [");

	// first부터 last까지 큐에 들어있는 원소 출력
	i = first;
	while (i != last)
	{
		printf("%3c", cQ->queue[i]);
		i = (i + 1) % MAX_QUEUE_SIZE;
	}
	printf(" ]\n");
}

//실제로 각각의 큐에 어떤 값이 들어있는지 디버깅
void debugQ(QueueType *cQ)
{

	printf("\n---DEBUG\n");
	for (int i = 0; i < MAX_QUEUE_SIZE; i++)
	{
		if (i == cQ->front)
		{
			printf("  [%d] = front\n", i);
			continue;
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]);
	}
	// printQ(cQ);
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear);
}
