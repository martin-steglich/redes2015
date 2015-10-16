#include "ParserComandos.h"
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <string.h>
#include <exception>

    Comando::Comando(char* sourceHost,unsigned int sourcePort,char* destHost,unsigned int destPort,unsigned int numSeq, bool esAck, tipoComando tipo,char* usuario,char* mensaje,bool mensajePrivado,char*  destinatarioMensajePrivado){

        this->tipo = tipo;
        this->esAck = esAck;
        if(usuario != NULL){
            this->usuario = new char();
            strcpy(this->usuario,usuario);
        }
        if(mensaje != NULL){
            memset(this->mensaje,0,512);
            strcpy(this->mensaje,mensaje);
        }

        this->mensajePrivado = mensajePrivado;

        if(destinatarioMensajePrivado != NULL){
            this->destinatarioMensajePrivado = new char();
            strcpy(this->destinatarioMensajePrivado,destinatarioMensajePrivado);
        }

        if(sourceHost != NULL){
            this->sourceHost = new char();
            strcpy(this->sourceHost,sourceHost);
        }

        this->sourcePort = sourcePort;

        if(destHost != NULL){
            this->destHost = new char();
            strcpy(this->destHost,destHost);
        }

        this->destPort = destPort;
        this->numSeq = numSeq;


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

    bool Comando::getEsAck(){
        return this->esAck;
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

    unsigned int Comando::getSourcePort(){
    return this->sourcePort;
    };

    char* Comando::getDestHost(){
    return this->destHost;
    };

    unsigned int Comando::getDestPort(){
    return this->destPort;
    };

    unsigned int Comando::getNumSeq(){
    return this->numSeq;
    };

    Comando::~Comando(){
        if(this->usuario != NULL){
            delete[] this->usuario;
        }
        if(this->mensaje != NULL){
            delete[] this->mensaje;
        }

        if(this->destinatarioMensajePrivado != NULL){
            delete[] this->destinatarioMensajePrivado;
        }

        if(this->sourceHost != NULL){
            delete[] this->sourceHost;
        }
        if(this->destHost != NULL){
            delete[] this->destHost;
        }

    };


    Comando* comandoParsear(char* buffer){
    char copia[512];
    strcpy(copia,buffer);
    char * pch = strtok (copia,">");//saco hasta head
    pch = strtok (NULL, "|");//agarro hasta Pipe
    char* sourceHost =  pch;
    pch = strtok (NULL, "|");
    unsigned int sourcePort =  atoi(pch);
    pch = strtok (NULL, "|");
    char * destHost =  pch;
    pch = strtok (NULL, "|");
    unsigned int destPort =  atoi(pch);
    pch = strtok (NULL, "|");
    unsigned int numSeq =  atoi(pch);
    pch = strtok (NULL, "<");
    bool isAck =  atoi(pch);
    pch = strtok (NULL, ">");// saco /head
    pch = strtok (NULL, ">");//saco data
    pch = strtok (NULL, "<");//agarro todo los datos

    char * tipo = strtok(pch, " <");//agarro el comando

    if (isAck == 1 ){

        Comando* res = new Comando(sourceHost,sourcePort,destHost,destPort,numSeq,isAck,ACK,NULL,NULL,false,NULL);
        return res;
    }

    else if (strcmp(tipo,"LOGIN") == 0){

        char * usuario = strtok(NULL,"<");
        Comando* res = new Comando(sourceHost,sourcePort,destHost,destPort,numSeq,isAck,LOGIN,usuario,NULL,false,NULL);

        return res;
    }
    else if (strcmp(tipo,"LOGOUT") == 0){

        Comando* res = new Comando(sourceHost,sourcePort,destHost,destPort,numSeq,isAck,LOGOUT,NULL,NULL,false,NULL);
        return res;
    }
    else if (strcmp(tipo,"GET_CONNECTED") == 0){
        Comando* res = new Comando(sourceHost,sourcePort,destHost,destPort,numSeq,isAck,GET_CONNECTED,NULL,NULL,false,NULL);
        return res;
    }
    else if (strcmp(tipo,"MESSAGE") == 0){
        char * msj = strtok(NULL,"<");
        Comando* res = new Comando(sourceHost,sourcePort,destHost,destPort,numSeq,isAck,MESSAGE,NULL,msj,false,NULL);
        return res;
    }
    else if (strcmp(tipo,"PRIVATE_MESSAGE") == 0){
        char * destinatario = strtok(NULL," ");
        char * msj = strtok(NULL,"<");
        Comando* res  = new Comando(sourceHost,sourcePort,destHost,destPort,numSeq,isAck,PRIVATE_MESSAGE,NULL,msj,true,destinatario);
        return res;
    }
    else
        cout << "error Al parsear "<< endl;

    return NULL;
    };
