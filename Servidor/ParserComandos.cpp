#include "ParserComandos.h"


    Comando::Comando(){};

    tipoComando Comando::getTipo(){
        return this->tipo;
    };
    
    string Comando::getusuario(){
        return this->usuario;
    };
    
    string* Comando::getUsuariosConectado(){
        return this->usuariosConectado;
    };
    
    string Comando::getMensaje(){
        return this->mensaje;
    };
    
    bool Comando::getEsMensajePrivado(){
        return this->mensajePrivado;
    };
    
    string Comando::getDestinatarioMensajePrivado(){
        return this->destinatarioMensajePrivado;
    };
    
    Comando::~Comando(){};
    

    Comando comandoParsear(char* buffer){};
