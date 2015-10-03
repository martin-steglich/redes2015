#include "ParserComandos.h"
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <string.h>
#include <exception>

    Comando::Comando(char* sourceHost, char* sourcePort,char* destHost, char* destPort, char* numSeq, tipoComando tipo,char* usuario,char* mensaje,bool mensajePrivado,char*  destinatarioMensajePrivado){

        this->tipo = tipo;
        this->usuario = usuario;
        this->mensaje = mensaje;
        this->mensajePrivado = mensajePrivado;
        this->destinatarioMensajePrivado = destinatarioMensajePrivado;
        this->sourceHost;
        this->sourcePort;
        this->destHost;
        this->destPort;
        this->numSeq;


    };

    tipoComando Comando::getTipo(){
        return this->tipo;
    };

    char* Comando::getusuario(){
        return this->usuario;
    };

    char* Comando::getMensaje(){
        return this->mensaje;
    };

    bool Comando::getEsMensajePrivado(){
        return this->mensajePrivado;
    };

    char* Comando::getDestinatarioMensajePrivado(){
        return this->destinatarioMensajePrivado;
    };


    char* Comando::getSourceHost(){
    return this->sourceHost;
    };
    char* Comando::getSourcePort(){
    return this->sourcePort;
    };
    char* Comando::getDestHost(){
    return this->destHost;
    };
    char* Comando::getDestPort(){
    return this->destPort;
    };
    char* Comando::getNumSeq(){
    return this->numSeq;
    };

    Comando::~Comando(){};


    Comando comandoParsear(char* buffer){
    char copia[2000];
    strcpy(copia,buffer);
    char * pch = strtok (copia,">");//saco hasta head
    pch = strtok (NULL, "|");//agarro hasta Pipe
    char * sourceHost =  pch;
    pch = strtok (NULL, "|");
    char* sourcePort =  pch;
    pch = strtok (NULL, "|");
    char * destHost =  pch;
    pch = strtok (NULL, "|");
    char* destPort =  pch;
    pch = strtok (NULL, "|");
    char* numSeq =  pch;
    pch = strtok (NULL, "<");
    bool isAck =  pch;
    pch = strtok (NULL, ">");// saco /head
    pch = strtok (NULL, ">");//saco data
    pch = strtok (NULL, "<");//agarro todo los datos

    char * tipo = strtok(pch, " <");//agarro el comando

    if (strcmp(tipo,"LOGIN") == 0){
    cout << "llego login 1"<< endl;
        char * usuario = strtok(NULL,"<");
        return Comando(sourceHost,sourcePort,destHost,destPort,numSeq,LOGIN,usuario,NULL,false,NULL);
    }
    else if (strcmp(tipo,"LOGOUT" == 0)){
        cout << "whattt"<< endl;
        return Comando(sourceHost,sourcePort,destHost,destPort,numSeq,LOGOUT,NULL,NULL,false,NULL);
    }
    else if (strcmp(tipo,"GET_CONNECTED") == 0){
        return Comando(sourceHost,sourcePort,destHost,destPort,numSeq,GET_CONNECTED,NULL,NULL,false,NULL);
    }
    else if (strcmp(tipo,"MESSAGE" == 0)){
        char * msj = strtok(NULL,"<");
        return Comando(sourceHost,sourcePort,destHost,destPort,numSeq,MESSAGE,NULL,msj,false,NULL);
    }
    else if (strcmp(tipo,"PRIVATE_MESSAGE" == 0)){
        char * destinatario = strtok(NULL," ");
        char * msj = strtok(NULL,"<");
        return Comando(sourceHost,sourcePort,destHost,destPort,numSeq,PRIVATE_MESSAGE,NULL,msj,true,destinatario);
    }
    else
        cout << "error Al parsear "<< endl;

    };
