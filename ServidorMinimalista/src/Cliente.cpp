#include "Cliente.h"

Cliente::Cliente(string host, unsigned int port, string nick, time_t activeTime,unsigned int senderSeq){
    this->host = host;
    this->port = port;
    this-> nick = nick;
    this->activeTime = activeTime;
    //this->senderSeq=sequenceNumber;
    this-> senderSeq=senderSeq;
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


unsigned int Cliente::getSenderSeq(){
    return senderSeq;
}


void Cliente::setSenderSeq(int ss){
    this->senderSeq = ss;
}

//unsigned int Cliente::getSequenceNumber(){
//    return sequenceNumber;
//}


Cliente::~Cliente()
{
    //dtor
}
