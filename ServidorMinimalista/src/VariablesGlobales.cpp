#include "VariablesGlobales.h"
#include <stdexcept>
#include<string>
#include <iostream>
#include <cstdlib>
using namespace std;
//#define synchronized(M)  for(Lock M##_lock = M; M##_lock; M##_lock.setUnlock())

VariablesGlobales* VariablesGlobales::instance = NULL;
int VariablesGlobales::cantConectados = 0;
int VariablesGlobales::cantMensajesEnviados = 0;
int VariablesGlobales::cantConexionesTotales = 0;
unsigned int seqNumber = 0;
time_t VariablesGlobales::activeTime;


VariablesGlobales::VariablesGlobales(){}
VariablesGlobales* VariablesGlobales::getInstance(){
    if(instance == NULL){
        instance = new VariablesGlobales();
        VariablesGlobales::cantConectados = 0;
        VariablesGlobales::cantMensajesEnviados = 0;
        VariablesGlobales::cantConexionesTotales = 0;
        time(&VariablesGlobales::activeTime);

    }
    return instance;
}

int VariablesGlobales::getCantConectados()const{
    return VariablesGlobales::cantConectados;
}
int VariablesGlobales::getCantMensajesEnviados()const{
    return VariablesGlobales::cantMensajesEnviados;
}

int VariablesGlobales::getCantConexionesTotales()const{
    return VariablesGlobales::cantConexionesTotales;
}

void VariablesGlobales::changeSeqNumber(){
    if(seqNumber == 1)
        seqNumber = 0;
    else
        seqNumber = 1;

}

unsigned int VariablesGlobales::getSeqNumber(){
    return seqNumber;
}

void VariablesGlobales::nuevoUsuario(string host, int port, string nick){
    if(!(existeCliente(nick))){
        Cliente* cliente = new Cliente;

        cliente->host = host;
        cliente->port = port;
        cliente->nick = nick;
        time(&cliente->activeTime);
        cliente->senderSeq = 0;
        conectados.insert(cliente);


    }
}

void VariablesGlobales::finConexion(string host, unsigned int port){
    for (set<Cliente*>::iterator it = conectados.begin(); it != conectados.end(); ++it){
        Cliente* actual = *it;
        if((actual->host == host) && (actual->port == port)){
            conectados.erase(it);
        }
    }
}

void VariablesGlobales::nuevoMensaje(){
    VariablesGlobales::cantMensajesEnviados++;
}

void VariablesGlobales::nuevaConexion(){
    VariablesGlobales::cantConexionesTotales++;
}

time_t VariablesGlobales::getActiveTime()const{
    return activeTime;
}

set<string> VariablesGlobales::getConectados(){

    set<string> connected;
    for (set<Cliente*>::iterator it = conectados.begin(); it != conectados.end(); ++it){
        Cliente* actual = *it;
        string nick = actual->nick;

        connected.insert(nick);
    }

    return connected;
}

Cliente* VariablesGlobales::buscarCliente(string nick){

    for (set<Cliente*>::iterator it = conectados.begin(); it != conectados.end(); ++it){
        Cliente* actual = *it;
        if(actual->nick == nick){
            return actual;
        }
    }

    return NULL;
}

Cliente* VariablesGlobales::buscarCliente(string host, unsigned int port){
    for (set<Cliente*>::iterator it = conectados.begin(); it != conectados.end(); ++it){
        Cliente* actual = *it;
        if((actual->host == host) && (actual->port == port)){
            return actual;
        }
    }
    return NULL;

}

bool VariablesGlobales::existeCliente(string nick){
    for (set<Cliente*>::iterator it = conectados.begin(); it != conectados.end(); ++it){
        Cliente* actual = *it;
        if(actual->nick == nick){
            return true;
        }
    }

    return false;
}
