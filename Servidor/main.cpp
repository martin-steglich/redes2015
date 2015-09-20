/* 
 * File:   main.cpp
 * Author: Tincho
 *
 * Created on September 14, 2015, 11:04 PM
 */

#include <cstdlib>
#include "Cliente.h"
#include <iostream>
#include <string>


using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    Cliente* cliente = new Cliente("Hola", 12, "chau");
    
    string host = cliente->getHost();
    int port = cliente->getPort();
    string portStr = to_string(port);
    string nick = cliente->getNick();
    cout << "HOST: " + host;
    cout << "PORT: " + portStr;
    cout << "Nick: " + nick;
   
    return 0;
}

