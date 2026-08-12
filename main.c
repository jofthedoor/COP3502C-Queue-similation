/* COP 3502C Assignment 2
   This program is written by: Juan de Lapuerta */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Customer {
    char name[16];
    int tickets;
    int lineNumber;
    int arrivalTime;
} Customer;

typedef struct Node {
    Customer *customer;
    struct Node *next;
} Node;


typedef struct Queue {
    Node *front;
    Node *rear;
} Queue;

void initQueue(Queue *q) {
    q->front = q->rear = NULL;
}

//create a customer 
Customer *createCustomer(int arrivalTime, int lineNumber, char *name, int tickets) {
    Customer *newCustomer = (Customer *)malloc(sizeof(Customer));
    strcpy(newCustomer->name, name);
    newCustomer->tickets = tickets;
    newCustomer->lineNumber = lineNumber;
    newCustomer->arrivalTime = arrivalTime;
    return newCustomer;
}

//enqueue
void enqueue(Queue *q, Customer *customer) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->customer = customer;
    newNode->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

//dequeue
Customer *dequeue(Queue *q) {
    if (q->front == NULL){
        return NULL;
    }
    Node *temp = q->front;
    Customer *customer = temp->customer;
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    free(temp);
    return customer;
}

//check if a queue is empty
int isEmpty(Queue *q) {
    return q->front == NULL;
}

//get front of queue
Customer *peek(Queue *q) {
    if (q->front == NULL) return NULL;
    return q->front->customer;
}
//main dunction
int main() {
    int c, n;
    int max = 12;
    scanf("%d", &c);
    while (c--) {
        scanf("%d", &n);
        Queue lines[max];
        for (int i = 0; i < max; i++) {
            initQueue(&lines[i]);
        }
// initialazing each custemer 
        for (int i = 0; i < n; i++) {
            int time;
            int lineNum;
            int tickets;
            char name[16];
            scanf("%d %d %s %d", &time, &lineNum, name, &tickets);
            Customer *customer = createCustomer(time, lineNum, name, tickets);
            enqueue(&lines[lineNum - 1], customer);
        }

        int currentTime = 0;
        while (1) {//find the custimer with the less tickets
            int minTickets = 101, selectedLine = -1;
            Customer *selcustomer = NULL;
            for (int i = 0; i < max; i++) {
                if (!isEmpty(&lines[i])) {
                    Customer *frontCustomer = peek(&lines[i]);
                    if (frontCustomer->arrivalTime <= currentTime) {
                        if (frontCustomer->tickets < minTickets ||(frontCustomer->tickets == minTickets && i < selectedLine)) {
                            minTickets = frontCustomer->tickets;
                            selectedLine = i;
                            selcustomer = frontCustomer;
                        }
                    }
                }
            }//update time to the next arrival if not found 
            if (selcustomer == NULL) {
                int nextTime = __INT_MAX__;
                for (int i = 0; i < max; i++) {
                    if (!isEmpty(&lines[i])) {
                        Customer *frontCustomer = peek(&lines[i]);
                        if (frontCustomer->arrivalTime < nextTime) {
                            nextTime = frontCustomer->arrivalTime;
                        }
                    }
                }//if no customers left
                if (nextTime == __INT_MAX__){
                    break;
                } 
                currentTime = nextTime;
                continue;
            }//updates the time and prints it
            currentTime += 20 + (selcustomer->tickets * 10);
            printf("%s left the counter at time %d from line %d.\n",
                   selcustomer->name, currentTime, selcustomer->lineNumber);
            dequeue(&lines[selectedLine]);
            free(selcustomer);
        }
        printf("======\n");
    }
    return 0;
}
