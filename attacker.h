#ifndef ATTACKER_H
#define ATTACKER_H

#include <string>
#include "request.h"

class Attacker {
public:
    std::string ip_address;
    int request_rate;

    Attacker(const std::string& ip, int rate);

    Request generate_request();
    std::string get_ip_address();
    int get_request_rate();

};

#endif
