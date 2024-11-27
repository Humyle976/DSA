#ifndef SERVER_H
#define SERVER_H

#include "request.h"
#include "simulationdialog.h"
#include <queue>
#include <string>
#include <unordered_map>
#include <chrono>
#include <string>
#include "linkedListQueue.h"

class Server {
private:
    int capacity;
    int rate_limit;
    bool implementBlackListIPs;
    unordered_map<string, int> consecutive_requests;
    unordered_map<string, chrono::time_point<chrono::high_resolution_clock>> blocked_ips;
    bool is_down;
    LinkedListQueue request_queue;


public:

    int queue_size_limit;
    Server(int cap, int queue_limit, int rate_limit, bool implementBlackListIPs);

    bool add_request(const Request& req,simulationDialog *Dialog);

    int process_requests(simulationDialog *Dialog);

    bool is_host_down();

    void reset_host();

    void QueueMethods(const Request& req,const string& method);

    int get_queue_size_limit() const;

    int get_queue_size();

    int get_capacity();
};

#endif
