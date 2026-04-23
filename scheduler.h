#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// RFC 9213: Urgency aralığı 0-7 arasıdır. 
// Varsayılan değer (default) 3'tür.
#define DEFAULT_URGENCY 3
#define MAX_HEAP_SIZE 100

typedef struct {
    int id;
    int urgency; // 0 (High) - 7 (Low)
    char data[256];
} HttpRequest;

typedef struct {
    HttpRequest* heap[MAX_HEAP_SIZE];
    int size;
} PriorityQueue;

// Fonksiyon Prototipleri
PriorityQueue* createQueue();
void push(PriorityQueue* pq, HttpRequest* req);
HttpRequest* pop(PriorityQueue* pq);
int parse_urgency(const char* priority_header);
void heapify_up(PriorityQueue* pq, int index);
void heapify_down(PriorityQueue* pq, int index);

#endif