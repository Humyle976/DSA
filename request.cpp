#include "request.h"

Request::Request(const string& ip) : ip_address(ip) {
    timestamp = chrono::high_resolution_clock::now();
}
