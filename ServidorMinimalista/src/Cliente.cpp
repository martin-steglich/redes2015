#include "Cliente.h"

Cliente::Cliente(string host, unsigned int port, string nick, time_t activeTime){
    this->host = host;
    this->port = port;
    this-> nick = nick;
    this->activeTime = activeTime;
}

string Cliente::getHost()const{
    return host;
}

unsigned int Cliente::getPort()const{
    return port;
}

string Cliente::getNick()const{
    return nick;
}

time_t Cliente::getActiveTime()const{
    return activeTime;
}




Cliente::~Cliente()
{
    //dtor
}
