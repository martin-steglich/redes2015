/*
 * File:   ParserComandos.h
 * Author: bruno.amaral
 *
 * Created on September 22, 2015, 8:44 PM
 */

#include <iostream>
#include <regex>
#include <string>

#ifndef PARSERCOMANDOS_H
#define	PARSERCOMANDOS_H


using namespace std;

enum tipoComando { LOGIN , LOGOUT, GET_CONNECTED, MESSAGE , PRIVATE_MESSAGE};

class Comando {

public:
    Comando();
    tipoComando getTipo();
    string getusuario();
    string getUsuariosConectado();
    string getMensaje();
    bool getEsMensajePrivado();
    string getDestinatarioMensajePrivado();
    virtual ~Comando();

private:
    tipoComando tipo;
    string usuario;
    string usuariosConectado[];
    string mensaje;
    bool mensajePrivado;
    string destinatarioMensajePrivado;
};

Comando comandoParsear(char* buffer){



};


#endif	/* PARSERCOMANDOS_H */

