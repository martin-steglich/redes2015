#include "ParserComandos.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <cstdlib>
#include <exception>
#include <vector>
#include <sstream>

    Comando::Comando(string sourceHost,unsigned int sourcePort,string destHost,unsigned int destPort,unsigned int numSeq, bool esAck, tipoComando tipo,string usuario,string mensaje,bool mensajePrivado,string  destinatarioMensajePrivado){

        this->tipo = tipo;
        this->esAck = esAck;
        if(!usuario.empty()){
            this->usuario =   usuario;
        }
        if(!mensaje.empty()){
            this->mensaje = mensaje;
        }

        this->mensajePrivado = mensajePrivado;

        if(!destinatarioMensajePrivado.empty()){
            this->destinatarioMensajePrivado = destinatarioMensajePrivado;
        }

        if(!sourceHost.empty()){
            this->sourceHost = sourceHost;
        }

        this->sourcePort = sourcePort;

        if(!destHost.empty()){
            this->destHost = destHost;
        }

        this->destPort = destPort;
        this->numSeq = numSeq;


    };

    tipoComando Comando::getTipo(){
        return this->tipo;
    };

    string Comando::getusuario(){
        return this->usuario;
    };

    string Comando::getMensaje(){
        return this->mensaje;
    };

    bool Comando::getEsAck(){
        return this->esAck;
    };

    bool Comando::getEsMensajePrivado(){
        return this->mensajePrivado;
    };

    string Comando::getDestinatarioMensajePrivado(){
        return this->destinatarioMensajePrivado;
    };


    string Comando::getSourceHost(){
    return this->sourceHost;
    };

    unsigned int Comando::getSourcePort(){
    return this->sourcePort;
    };

    string Comando::getDestHost(){
    return this->destHost;
    };

    unsigned int Comando::getDestPort(){
    return this->destPort;
    };

    unsigned int Comando::getNumSeq(){
    return this->numSeq;
    };

    Comando::~Comando(){
        /*cout << "DELETE" << endl;
        if(!this->usuario.empty()){
            cout << "USUARIO"<< endl;
            delete this->usuario;
        }*/
       /* if(this->mensaje != NULL){
            cout << "MENSAJE"<< endl;
            delete[] this->mensaje;
        }*/

       /* if(!this->destinatarioMensajePrivado.empty()){
            cout << "DESTINATARIO"<< endl;
            delete this->destinatarioMensajePrivado;
        }

        if(!this->sourceHost.empty()){
            cout << "SOURCE"<< endl;
            delete this->sourceHost;
        }
        if(!this->destHost.empty()){
            cout << "DEST"<< endl;
            delete this->destHost;
        }
*/
        //delete this;

    };


/*vector<string> split(string str, char delimiter) {
    vector<string> internal;
    stringstream ss(str); // Turn the string into a stream.
    string tok;

    while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
    }

    return internal;
}*/



    Comando* comandoParsear(char* buffer){
        char copia[512];
        memset(copia,0,512);
        strcpy(copia,buffer);
        char * pch = strtok (copia,">");//saco hasta head
        pch = strtok (NULL, "|");//agarro hasta Pipe
        string sourceHost = "";
        sourceHost = sourceHost + pch;
        pch = strtok (NULL, "|");
        unsigned int sourcePort =  atoi(pch);
        pch = strtok (NULL, "|");
        string destHost = "";
        destHost = destHost + pch;
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

            Comando* res = new Comando(sourceHost,sourcePort,destHost,destPort,numSeq,isAck,ACK,"","",false,"");
            return res;
        }

        else if (strcmp(tipo,"LOGIN") == 0){
            string usuario = "";
            usuario = usuario + strtok(NULL,"<");
            Comando* res = new Comando(sourceHost,sourcePort,destHost,destPort,numSeq,isAck,LOGIN,usuario,"",false,"");

            return res;
        }
        else if (strcmp(tipo,"LOGOUT") == 0){

            Comando* res = new Comando(sourceHost,sourcePort,destHost,destPort,numSeq,isAck,LOGOUT,"","",false,"");
            return res;
        }
        else if (strcmp(tipo,"GET_CONNECTED") == 0){
            Comando* res = new Comando(sourceHost,sourcePort,destHost,destPort,numSeq,isAck,GET_CONNECTED,"","",false,"");
            return res;
        }
        else if (strcmp(tipo,"MESSAGE") == 0){
            //char msj[500];// = new char();
            //memset(msj,0,500);
            pch = strtok(NULL,"<");
            string msj = "";
            msj = msj + pch;
            Comando* res = new Comando(sourceHost,sourcePort,destHost,destPort,numSeq,isAck,MESSAGE,"",msj,false,"");
            return res;
        }
        else if (strcmp(tipo,"PRIVATE_MESSAGE") == 0){
            string destinatario = "";
            destinatario = destinatario + strtok(NULL," ");
            string msj = "";
            msj = strtok(NULL,"<");
            Comando* res  = new Comando(sourceHost,sourcePort,destHost,destPort,numSeq,isAck,PRIVATE_MESSAGE,"",msj,true,destinatario);
            return res;
        }
        else
            cout << "error Al parsear "<< endl;

        return NULL;
    };
