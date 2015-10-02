#include "VariablesGlobales.h"
#include <stdexcept>
#include<string>

//#define synchronized(M)  for(Lock M##_lock = M; M##_lock; M##_lock.setUnlock())

VariablesGlobales* VariablesGlobales::instance = NULL;
int VariablesGlobales::cantConectados = 0;
int VariablesGlobales::cantMensajesEnviados = 0;
int VariablesGlobales::cantConexionesTotales = 0;
time_t VariablesGlobales::activeTime;


VariablesGlobales::VariablesGlobales(){}
VariablesGlobales* VariablesGlobales::getInstance(){
    if(instance == NULL){
        instance = new VariablesGlobales;
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

void VariablesGlobales::nuevoUsuario(){

    VariablesGlobales::cantConectados++;
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
        string nick = actual->getNick();
        connected.insert(nick);
    }
    return connected;
}

Cliente* VariablesGlobales::buscarCliente(string nick){

    for (set<Cliente*>::iterator it = conectados.begin(); it != conectados.end(); ++it){
        Cliente* actual = *it;
        if(actual->getNick() == nick){
            return actual;
        }
    }
    return NULL;
}
