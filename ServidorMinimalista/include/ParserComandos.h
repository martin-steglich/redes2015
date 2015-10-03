/*
 * File:   ParserComandos.h
 * Author: bruno.amaral
 *
 * Created on September 22, 2015, 8:44 PM
 */


#ifndef PARSERCOMANDOS_H
#define	PARSERCOMANDOS_H


using namespace std;

enum tipoComando { LOGIN , LOGOUT, GET_CONNECTED, MESSAGE , PRIVATE_MESSAGE};

class Comando {

public:
    Comando(char sourceHost[80], char* sourcePort, char* destHost, char* destPort, char* numSeq,tipoComando tipo,char* usuario,char* mensaje,bool mensajePrivado,char*  destinatarioMensajePrivado);
    tipoComando getTipo();
    char* getusuario();
    char* getMensaje();
    bool getEsMensajePrivado();
    char* getDestinatarioMensajePrivado();
    char* getSourceHost();
    char* getSourcePort();
    char* getDestHost();
    char* getDestPort();
    char* getNumSeq();
    virtual ~Comando();

private:
    tipoComando tipo;
    char* usuario;
    char* mensaje;
    bool mensajePrivado;
    char* destinatarioMensajePrivado;
    char* sourceHost;
    char* sourcePort;
    char* destHost;
    char* destPort;
    char* numSeq;
};

Comando comandoParsear(char* buffer);



#endif	/* PARSERCOMANDOS_H */

