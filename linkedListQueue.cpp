#include "linkedListQueue.h"

LinkedListQueue::LinkedListQueue() : front(nullptr), rear(nullptr) , size(0) {}

LinkedListQueue::~LinkedListQueue() {
    while (!is_empty()) {
        pop();
    }
}

void LinkedListQueue::push(const Request& req) {
    Node* newNode = new Node(req);
    if (rear) {
        rear->next = newNode;
    }
    rear = newNode;
    size++;
    if (!front) {
        front = rear;
    }
}


void LinkedListQueue::pop() {
    if (front) {
        Node* temp = front;
        front = front->next;
        delete temp;
        size--;
        if (!front) {
            rear = nullptr;
        }
    }
}


Request LinkedListQueue::getNodeRequest() const {
    return front->request;
}

chrono::time_point<chrono::high_resolution_clock> LinkedListQueue::getNodeTime()
{
    return front->requestSentTime;
}


bool LinkedListQueue::is_empty() const {
        return front == nullptr;
}


int LinkedListQueue::get_size()
{
    return size;
}
