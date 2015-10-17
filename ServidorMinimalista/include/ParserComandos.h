/*
 * File:   ParserComandos.h
 * Author: bruno.amaral
 *
 * Created on September 22, 2015, 8:44 PM
 */


#ifndef PARSERCOMANDOS_H
#define	PARSERCOMANDOS_H


using namespace std;

enum tipoComando { LOGIN , LOGOUT, GET_CONNECTED, MESSAGE , PRIVATE_MESSAGE , ACK};

class Comando {

public:
    Comando(char* sourceHost,unsigned int sourcePort, char* destHost,unsigned int destPort,unsigned int numSeq, bool esAck,tipoComando tipo,char* usuario,char* mensaje,bool mensajePrivado,char*  destinatarioMensajePrivado);
    tipoComando getTipo();
    bool getEsAck();
    char* getusuario();
    char* getMensaje();
    bool getEsMensajePrivado();
    char* getDestinatarioMensajePrivado();
    char* getSourceHost();
    unsigned int getSourcePort();
    char* getDestHost();
    unsigned int getDestPort();
    unsigned int getNumSeq();
    virtual ~Comando();

private:
    tipoComando tipo;
    bool esAck;
    char* usuario;
    char mensaje[512];
    bool mensajePrivado;
    char* destinatarioMensajePrivado;
    char* sourceHost;
    unsigned int sourcePort;
    char* destHost;
    unsigned int destPort;
    unsigned int numSeq;
};

Comando* comandoParsear(char* buffer);



#endif	/* PARSERCOMANDOS_H */

