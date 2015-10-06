#include "VariablesGlobales.h"
#include <stdexcept>
#include <string>
#include <iostream>
#include <cstdlib>
#include <mutex.h>


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
    Mutex mtx = Mutex();
    mtx.lock();
    if(instance == NULL){
        instance = new VariablesGlobales();
        VariablesGlobales::cantConectados = 0;
        VariablesGlobales::cantMensajesEnviados = 0;
        VariablesGlobales::cantConexionesTotales = 0;
        time(&VariablesGlobales::activeTime);

    }
    mtx.unlock();
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
    Mutex mtx = Mutex();
    mtx.lock();
    if(seqNumber == 1)
        seqNumber = 0;
    else
        seqNumber = 1;
    mtx.unlock();
}

unsigned int VariablesGlobales::getSeqNumber(){

    return seqNumber;
}

void VariablesGlobales::nuevoUsuario(string host, int port, string nick){
    Mutex mtx = Mutex();
    mtx.lock();
    if(!(existeCliente(nick))){
        Cliente* cliente = new Cliente;

        cliente->host = host;
        cliente->port = port;
        cliente->nick = nick;
        time(&cliente->activeTime);
        cliente->senderSeq = 0;
        conectados.insert(cliente);

    }
    mtx.unlock();
}

void VariablesGlobales::finConexion(string host, unsigned int port){
    Mutex mtx = Mutex();
    mtx.lock();
    for (set<Cliente*>::iterator it = conectados.begin(); it != conectados.end(); ++it){
        Cliente* actual = *it;
        if((actual->host == host) && (actual->port == port)){
            conectados.erase(it);
        }
    }
    mtx.unlock();
}


void VariablesGlobales::nuevoMensaje(){
    Mutex mtx = Mutex();
    mtx.lock();
    VariablesGlobales::cantMensajesEnviados++;
    mtx.unlock();
}

void VariablesGlobales::nuevaConexion(){
    Mutex mtx = Mutex();
    mtx.lock();
    VariablesGlobales::cantConexionesTotales++;
    mtx.unlock();
}

time_t VariablesGlobales::getActiveTime()const{

    return activeTime;
}

set<string> VariablesGlobales::getConectados(){
    Mutex mtx = Mutex();
    mtx.lock();
    set<string> connected;
    for (set<Cliente*>::iterator it = conectados.begin(); it != conectados.end(); ++it){
        Cliente* actual = *it;
        string nick = actual->nick;

        connected.insert(nick);
    }
    mtx.unlock();
    return connected;
}

Cliente* VariablesGlobales::buscarCliente(string nick){
    Mutex mtx = Mutex();
    mtx.lock();
    for (set<Cliente*>::iterator it = conectados.begin(); it != conectados.end(); ++it){
        Cliente* actual = *it;
        if(actual->nick == nick){
            mtx.unlock();
            return actual;
        }
    }
    mtx.unlock();
    return NULL;
}

Cliente* VariablesGlobales::buscarCliente(string host, unsigned int port){
    Mutex mtx = Mutex();
    mtx.lock();
    for (set<Cliente*>::iterator it = conectados.begin(); it != conectados.end(); ++it){
        Cliente* actual = *it;
        if((actual->host == host) && (actual->port == port)){
            mtx.unlock();
            return actual;
        }
    }

    mtx.unlock();
    return NULL;

}

bool VariablesGlobales::existeCliente(string nick){
    Mutex mtx = Mutex();
    mtx.lock();
    for (set<Cliente*>::iterator it = conectados.begin(); it != conectados.end(); ++it){
        Cliente* actual = *it;
        if(actual->nick == nick){
            mtx.unlock();
            return true;
        }
    }
    mtx.unlock();
    return false;
}

bool VariablesGlobales::existeCliente(string host, unsigned int port){
    Mutex mtx = Mutex();
    mtx.lock();
    for (set<Cliente*>::iterator it = conectados.begin(); it != conectados.end(); ++it){
        Cliente* actual = *it;
        if((actual->host == host) && (actual->port == port)){
            mtx.unlock();
            return true;
        }
    }
    mtx.unlock();
    return false;
}

void VariablesGlobales::numeroSecuenciaCliente(string host, unsigned int port){
    Mutex mtx = Mutex();
    mtx.lock();
    Cliente* cliente = buscarCliente(host, port);
    if(cliente->senderSeq == 1)
        cliente->senderSeq = 0;
    else
        cliente->senderSeq = 1;

    mtx.unlock();

}

void VariablesGlobales::vaciarMemoria(){
    for (set<Cliente*>::iterator it = conectados.begin(); it != conectados.end(); ++it){
        Cliente* actual = *it;
        delete actual;
    }

    conectados.clear();
}
