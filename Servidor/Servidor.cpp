#include "Servidor.h"

Servidor::Servidor(){

    
    socketServidorAtiendeLogin = 
    socketRespuestaLogin ;
    clienteDireccion ;
    myDireccion ;
    socketsActuales ;        // Conjunto de descriptores a controlar
    contadorHijos = 0;


};
    


int Servidor::getSocketServidorAtiendeLogin(){};


int Servidor::getSocketRespuestaLogin(){};
    
struct sockaddr_in Servidor::getClienteDireccion(){};
    
struct sockaddr_in Servidor::getMyDireccion(){};
    
    
fd_set Servidor::getSocketsActuales(){};        
    
int Servidor::getContadorHijos(){
    return contadorHijos;
};     

Servidor::~Servidor(){};