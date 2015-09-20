/* 
 * File:   Cliente.cpp
 * Author: Tincho
 * 
 * Created on September 19, 2015, 1:05 PM
 */

#include "Cliente.h"

Cliente::Cliente(){
    
}

Cliente::Cliente(string host, int port, string nick){
    this->host = host;
    this->port = port;
    this->nick = nick;
}

string Cliente::getNick(){
    return this->nick;
}

string Cliente::getHost(){
    return this->host;
}

int Cliente::getPort(){
    return this->port;
}

Cliente::~Cliente(){};