#include <stdio.h>
#include<windows.h>
#define MAXIMUM_QUEUE_SIZE 100


typedef struct Processes {
	int Process_No;
	int arrival_time;
	int burst_time;
} Process;


Process * queue[MAXIMUM_QUEUE_SIZE];
int front = 0, rear = -1, processed = 0, curr_time = 0, tq = 7;
int last_front = 0, last_rear = -1;

void swap(Process * a, Process * b) {
	Process temp = * a;
	* a = * b;
	* b = temp;
}


void sort(Process p[], int n) {
	int i, j;
	short swapped;
	
	for (i = 0; i < n; ++i) {
		swapped = 0;
		
		for (j = 0; j < n-i-1; ++j)
		{
			if (p[j].arrival_time > p[j+1].arrival_time)
			{
				swap(&p[j], &p[j+1]);
				swapped = 1;
			}
		}
		
		if (swapped == 0)
			break;
	}
}


void enqueue(Process p[], int n) {
	int i, j, can_insert;
	for (i = 0; i < n; ++i)
	{
		can_insert = 1;
		
		if (p[i].arrival_time <= curr_time && p[i].burst_time > 0)
		{
			if (front == 0) {
				queue[++rear] = &p[i];
			}
			else
			{
				for (j = last_front; j <= last_rear; ++j) {
					if (queue[j]->Process_No == p[i].Process_No)
						can_insert = 0;
				}
				if (can_insert == 1)
					queue[++rear] = &p[i];
			}
		}
	}
	for (i = last_front; i <= last_rear; ++i)
	{
		if (queue[i]->burst_time > 0)
			queue[++rear] = queue[i];
	}
}


void execute() {
	int i;
	
	if (front-1 == rear) {
		printf("CPU idle for 1 second.\n");
		curr_time += 1;
	}
	else {
		last_front = front;
		last_rear = rear;
		
		for (i = front; i <= rear; ++i, ++front)
		{
			if (queue[i]->burst_time > tq)
			{
				queue[i]->burst_time -= tq;
				curr_time += tq;
				printf("Process number %d Excuted till %d (Seconds).\n", queue[i]->Process_No, curr_time);
			}
			else if (queue[i]->burst_time > 0)
			{
				curr_time += queue[i]->burst_time;
				queue[i]->burst_time = 0;
				printf("Process number %d excuted till %d (Seconds).\n", queue[i]->Process_No, curr_time);
				
				++processed;
			}
		}
	}
}


int main() {
	int n, i;
	short err_flag = 0;
	
	do {
		if (err_flag == 1)
			fprintf(stderr, "\n NO. of processes Should_be_greater than 1.\n");

		printf("Enter the number of processes: ");
		scanf("%d", &n);
		
		err_flag = 1;
	} while (n < 1);
	err_flag = 0;
	
	Process p[n];
	
	for (i = 0; i < n; ++i) {
		printf("\n");
		printf("Enter arrival_time of process %d: ", i+1);
		scanf("%d", &p[i].arrival_time);
		
		printf("Enter burst_time of process %d: ", i+1);
		scanf("%d", &p[i].burst_time);
		
		p[i].Process_No = i+1;
	}
	
	sort(&p[0], n); 
	
	while (1) {
		enqueue(p, n);
		
		printf("\nIn queue: ");
		for (i = 0; i <= rear; ++i) {
			printf("%d ", queue[i]->Process_No);
		}
		printf("\nFront = %d, Rear = %d.\n\n", front, rear);
		
		execute();
		Sleep(2);
		
		if (processed == n)
			break;
	}
	
	return 0;
}
