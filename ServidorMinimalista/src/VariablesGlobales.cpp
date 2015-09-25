#include "VariablesGlobales.h"
#include <stdexcept>
#include<string>

//#define synchronized(M)  for(Lock M##_lock = M; M##_lock; M##_lock.setUnlock())

VariablesGlobales* VariablesGlobales::instance = NULL;
VariablesGlobales::VariablesGlobales(){}
VariablesGlobales* VariablesGlobales::getInstance(){
    if(instance == NULL){
        instance = new VariablesGlobales;

    }
    return instance;
}

int VariablesGlobales::getCantConectados(){
    return cantConectados;
}
int VariablesGlobales::getCantMensajesEnviados(){
    return cantMensajesEnviados;
}

int VariablesGlobales::getCantConexionesTotales(){
    return cantConexionesTotales;
}

void VariablesGlobales::nuevoUsuario(){

    cantConectados++;
}

void VariablesGlobales::nuevoMensaje(){
    cantMensajesEnviados++;
}

void VariablesGlobales::nuevaConexion(){
    cantConexionesTotales++;
}
