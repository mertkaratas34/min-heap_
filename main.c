#include "scheduler.h"

int main() {
    PriorityQueue* gateway_queue = createQueue();

    // Örnek Gelen İstekler (Farklı RFC 9213 headerları ile)
    const char* headers[] = { "u=5", "u=0", "u=3, i", "u=7", "u=1" };
    const char* payloads[] = { "Resim Dosyası", "Kritik API Verisi", "Normal Sayfa", "Log Kaydı", "CSS Dosyası" };

    printf("--- API Gateway Istekleri Aliniyor ---\n");
    for (int i = 0; i < 5; i++) {
        HttpRequest* req = (HttpRequest*)malloc(sizeof(HttpRequest));
        req->id = i + 1;
        req->urgency = parse_urgency(headers[i]);
        strcpy(req->data, payloads[i]);
        
        printf("[Gelen] ID: %d, Header: %s, Urgency: %d\n", req->id, headers[i], req->urgency);
        push(gateway_queue, req);
    }

    printf("\n--- Zamanlayici Calisiyor (Priority Processing) ---\n");
    while (gateway_queue->size > 0) {
        HttpRequest* processed = pop(gateway_queue);
        printf("[Islemde] ID: %d, Oncelik: %d, Icerik: %s\n", 
                processed->id, processed->urgency, processed->data);
        free(processed);
    }

    free(gateway_queue);
    return 0;
}