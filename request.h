#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <chrono>

using namespace std;

class Request {
public:
    string ip_address;
    chrono::time_point<chrono::high_resolution_clock> timestamp;

    Request(const string& ip);
};

#endif
