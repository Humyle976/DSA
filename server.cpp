#include "server.h"
#include "simulationdialog.h"
#include "request.h"
#include <QString>
#include <chrono>
#include <QMutex>
#include <thread>
#include <iostream>
#include <QDebug>
#include <unordered_map>
#include <atomic>

using namespace std;

Server::Server(int cap, int queue_limit, int rate_limit, bool implementBlackListIPs)
    : capacity(cap), queue_size_limit(queue_limit), rate_limit(rate_limit), implementBlackListIPs(implementBlackListIPs), is_down(false)
    {
}

bool Server::add_request(const Request& req,simulationDialog *Dialog) {

    if (is_host_down()) {
        emit Dialog->appendTerminal("Server Is Down!!!");
        cout << "Server is down. Cannot accept request." << endl;
        return false;
    }

    if (implementBlackListIPs) {
        auto current_time = chrono::high_resolution_clock::now();

        if (blocked_ips.find(req.ip_address) != blocked_ips.end()) {
            auto block_duration = chrono::duration_cast<chrono::seconds>(current_time - blocked_ips[req.ip_address]).count();
            if (block_duration <  60) {
                emit Dialog->appendTerminal(QString::fromStdString(req.ip_address) + " Is Blocked! Cant Make Request");
                cout << "IP " << req.ip_address << " is blocked for 5 seconds." << endl;
                return false;
            } else {
                blocked_ips.erase(req.ip_address);
                consecutive_requests[req.ip_address] = 0;
            }
        }

        if (++consecutive_requests[req.ip_address] > rate_limit) {
            blocked_ips[req.ip_address] = current_time;
            consecutive_requests[req.ip_address] = 0;

            cout << "IP " << req.ip_address << " exceeded rate limit and is now blocked." << endl;
            return false;
        }
    }

    if (!is_host_down()) {

        cout << "Request added to the queue. Current queue size: " << request_queue.get_size() << endl;
        request_queue.push(req);
        emit Dialog->appendTerminal("Incoming request from " + QString::fromStdString(req.ip_address));
        return true;
    } else {
        emit Dialog->appendTerminal("Server Is Down!!!");
        cout << "SERVER DOWN: Request queue full." << endl;
        if (request_queue.get_size() >= queue_size_limit) {
            is_down = true;
        }
        return false;
    }
}

int Server::process_requests(simulationDialog *Dialog) {
    cout << "Processing requests. Queue size: " << request_queue.get_size() << endl;

    if (request_queue.is_empty()) {
        return 0;
    }

    if(is_host_down())
    {
        return -1;
    }
    int processed = 0;

    while (!request_queue.is_empty() && processed < get_capacity()) {
        Request req = request_queue.getNodeRequest();
        auto responseTime = chrono::duration<double>(
                                chrono::high_resolution_clock::now() - request_queue.getNodeTime()
                                ).count();
        request_queue.pop();
        emit Dialog->appendTerminal("Processed Request Of IP: " + QString::fromStdString(req.ip_address) + "   " + QString::number(responseTime) + " Seconds");
        processed++;
        this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    cout << "Processed " << processed << " requests." << endl;
    return processed;
}

bool Server::is_host_down() {

    return ((get_queue_size() - capacity) > get_queue_size_limit());
}


int Server::get_queue_size(){
    return request_queue.get_size();
}

int Server::get_queue_size_limit() const {
    return queue_size_limit;
}

int Server::get_capacity() {
    return capacity;
}

