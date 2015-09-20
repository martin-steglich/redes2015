/* 
 * File:   Cliente.h
 * Author: Tincho
 *
 * Created on September 19, 2015, 1:05 PM
 */

#ifndef CLIENTE_H
#define	CLIENTE_H

#include <iostream>
#include <map>
#include <string>

using namespace std;
class Cliente {
public:
    Cliente(string host, int port, string nick);
    Cliente();
    string getNick();
    string getHost();
    int getPort();
    virtual ~Cliente();
private:
    string nick;
    string host;
    int port;
};

#endif	/* CLIENTE_H */

