#include "attacker.h"
#include <string>

using namespace std;

Attacker::Attacker(const std::string& ip, int rate)
    : ip_address(ip), request_rate(rate) {}


Request Attacker::generate_request()
{
    return Request(ip_address);
}



int Attacker::get_request_rate()
{
    return request_rate;
}


std::string Attacker::get_ip_address()
{
    return ip_address;
}
