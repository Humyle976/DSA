#ifndef LINKEDLISTQUEUE_H
#define LINKEDLISTQUEUE_H

#include "request.h"

struct Node {
    Request request;
    chrono::time_point<chrono::high_resolution_clock> requestSentTime;
    Node* next;
    Node(const Request& req) : request(req), next(nullptr) {
        requestSentTime = chrono::high_resolution_clock::now();
    }
};

class LinkedListQueue {
private:
    Node* front;
    Node* rear;


public:
    int size;
    LinkedListQueue();

    ~LinkedListQueue();

    void push(const Request& req);

    void pop();

    Request getNodeRequest() const;

    chrono::time_point<chrono::high_resolution_clock> getNodeTime();
    bool is_empty() const;

    int get_size();
};
#endif
