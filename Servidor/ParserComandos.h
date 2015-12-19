/*
 * File:   ParserComandos.h
 * Author: bruno.amaral
 *
 * Created on September 22, 2015, 8:44 PM
 */



#ifndef PARSERCOMANDOS_H
#define	PARSERCOMANDOS_H

#include <string>

using namespace std;

enum tipoComando { LOGIN , LOGOUT, GET_CONNECTED, MESSAGE , PRIVATE_MESSAGE , ACK};

class Comando {

public:
    Comando(string sourceHost,unsigned int sourcePort, string destHost,unsigned int destPort,unsigned int numSeq, bool esAck,tipoComando tipo,string usuario,string mensaje,bool mensajePrivado,string  destinatarioMensajePrivado);
    tipoComando getTipo();
    bool getEsAck();
    string getusuario();
    string getMensaje();
    bool getEsMensajePrivado();
    string getDestinatarioMensajePrivado();
    string getSourceHost();
    unsigned int getSourcePort();
    string getDestHost();
    unsigned int getDestPort();
    unsigned int getNumSeq();
    virtual ~Comando();

private:
    tipoComando tipo;
    bool esAck;
    string usuario;
    string mensaje;
    bool mensajePrivado;
    string destinatarioMensajePrivado;
    string sourceHost;
    unsigned int sourcePort;
    string destHost;
    unsigned int destPort;
    unsigned int numSeq;
};

Comando* comandoParsear(char* buffer);



#endif	/* PARSERCOMANDOS_H */

