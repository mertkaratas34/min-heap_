#include "scheduler.h"

PriorityQueue* createQueue() {
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    pq->size = 0;
    return pq;
}

// RFC 9213 Header Parse: "u=2, i=?0" gibi bir string içinden 'u' değerini çeker.
int parse_urgency(const char* header) {
    if (header == NULL) return DEFAULT_URGENCY;
    
    char* u_ptr = strstr(header, "u=");
    if (u_ptr) {
        int val = atoi(u_ptr + 2);
        if (val >= 0 && val <= 7) return val;
    }
    return DEFAULT_URGENCY;
}

void swap(HttpRequest** a, HttpRequest** b) {
    HttpRequest* temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(PriorityQueue* pq, int index) {
    while (index > 0 && pq->heap[index]->urgency < pq->heap[(index - 1) / 2]->urgency) {
        swap(&pq->heap[index], &pq->heap[(index - 1) / 2]);
        index = (index - 1) / 2;
    }
}

void push(PriorityQueue* pq, HttpRequest* req) {
    if (pq->size >= MAX_HEAP_SIZE) return;
    pq->heap[pq->size] = req;
    heapify_up(pq, pq->size);
    pq->size++;
}

void heapify_down(PriorityQueue* pq, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < pq->size && pq->heap[left]->urgency < pq->heap[smallest]->urgency)
        smallest = left;
    if (right < pq->size && pq->heap[right]->urgency < pq->heap[smallest]->urgency)
        smallest = right;

    if (smallest != index) {
        swap(&pq->heap[index], &pq->heap[smallest]);
        heapify_down(pq, smallest);
    }
}

HttpRequest* pop(PriorityQueue* pq) {
    if (pq->size == 0) return NULL;
    HttpRequest* root = pq->heap[0];
    pq->heap[0] = pq->heap[pq->size - 1];
    pq->size--;
    heapify_down(pq, 0);
    return root;
}