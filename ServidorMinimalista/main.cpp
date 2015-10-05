#include <iostream>
#include <cstdlib>
//#include "Cliente.h"
//#include "HiloAtiende.h"
//#include "Errores.h"
#include "ParserComandos.h"
#include <iostream>
#include <string>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <netinet/in.h>
#include <resolv.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <stdexcept>
#include "VariablesGlobales.h"

/**
* El cliente envía el login y
* guarda el puerto por el que lo envio (va a ser el puerto que siempre va a utilizar para enviar)
* El servidor cada paquete que le llega tiene que le envía un ACK
* hacia el mismo (host, puerto) que envió el mensaje
*
* Los mensajes desde el servidor se mandan siempre al multicast (desde el lado del cliente
* se verifica si es para él). Los clientes, por más que no sean los destinatarios del mensaje,
* siempre mandan un ACK.
*
* La idea para obtener el host y puerto origen de un mensaje es parsear el header y obtener los
* campos que corresponden a eso. Siempre enviar a ese par (host, puerto).
*
* Cuando llega un login (es un nuevo Cliente), se parsea el mensaje y se obtiene:
* host, puerto (del header) y nick (del data), se crea una nueva instancia de
* Cliente pasándole host, puerto, nick, y un time_t (mirar el comando f en la función comandosConsola)
*
**/
using namespace std;

#define HELLO_PORT 5000
#define HELLO_GROUP "225.5.4.29"
//Puerto
#define PORT 54321
#define PORTACK 51234

//Número máximo de hijos
#define MAX_HIJOS 10 // TODO para empezar acordarse de poner un numero mayor

//Longitud del buffer
#define BUFFERSIZE 512 // poner el de los datagramas

int comandosConsola(){

    string command;
    bool exit = false;
    VariablesGlobales* variablesGlobales = variablesGlobales->getInstance();

    while (!exit){
        cin >> command;

        if(command.compare("exit") == 0)
            exit = true;
        else if(command.compare("a") == 0)
            cout << "La cantidad de clientes es: " << variablesGlobales->getCantConectados() << endl;
        else if(command.compare("s") == 0)
            cout << "Cantidad mensajes enviados" << variablesGlobales->getCantMensajesEnviados() << endl;
        else if(command.compare("d") == 0)
            cout << "Cantidad conexiones totales" << variablesGlobales->getCantConexionesTotales() << endl;
        else if(command.compare("f") == 0){
                time_t activeTime = variablesGlobales->getActiveTime();
                time_t serverTime;
                time(&serverTime);
                double seconds = difftime(serverTime, activeTime);
                printf ("El servidor se encuentra activo hace %.f segundos\n", seconds);
        }
        cout<< endl;
    }

    return 1;
}

//puerto y host corresponden al receptor (el que pidio los conectados)
string getConnected(Comando* comand){

    VariablesGlobales* variablesGlobales = variablesGlobales->getInstance();

    set<string> users = variablesGlobales->getConectados();

    string message = "<head>";
    message.append(comand->getDestHost());
    message.append("|");

    char* serverPortStr = new char();
    sprintf(serverPortStr,"%d",comand->getDestPort());
    message.append(serverPortStr);
    message.append("|");

    message.append(comand->getSourceHost());
    message.append("|");

    char* portStr = new char();
    sprintf(portStr,"%d",comand->getSourcePort());
    message.append(portStr);
    message.append("|");

    char* serverSeqStr = new char();
    sprintf(serverSeqStr,"%d",variablesGlobales->getSeqNumber());
    message.append(serverSeqStr);

    message.append("|0|");

    message.append(serverSeqStr);
    message.append("</head><data>CONNECTED ");


    for (set<string>::iterator it = users.begin(); it != users.end(); ++it){
        if(it != users.begin())
            message.append(" |");

        string actual = *it;
        message.append(actual);
    }
    message.append("<CR></data>");

    variablesGlobales->changeSeqNumber();

    return message;

    //const char *msj = message.c_str();

    //sendto(socket, msj , strlen(msj)+1, 0 , (struct sockaddr*)&multicastDir , sizeof(multicastDir));
    /** Hay que quedar esperando el ACK de todos **/


    //delete[] serverPortStr;
    //delete[] portStr;
    //delete[] seqStr;
    //delete[] serverSeqStr;


}

//puerto y host corresponden al emisor del mensaje (es para obtener el nick)
string relayedMessage(Comando* comand){

    VariablesGlobales* variablesGlobales = variablesGlobales->getInstance();

    Cliente* cliente = variablesGlobales->buscarCliente(comand->getSourceHost(), comand->getSourcePort());

    string message = "<head>";
    message.append(comand->getDestHost());
    message.append("|");

    char* serverPortStr = new char();
    sprintf(serverPortStr,"%d",comand->getDestPort());
    message.append(serverPortStr);
    message.append("|");
    message.append("0");
    message.append("|");
    message.append("0");
    message.append("|");

    char* serverSeqStr = new char();
    sprintf(serverSeqStr,"%d",variablesGlobales->getSeqNumber());
    message.append(serverSeqStr);

    message.append("|0|");

    message.append(serverSeqStr);
    message.append("</head><data>RELAYED_MESSAGE ");

    message.append(cliente->nick);
    message.append(" ");
    message.append(comand->getMensaje());
    message.append("<CR></data>");

    variablesGlobales->changeSeqNumber();

    return message;


    //delete[] serverPortStr;
    //delete[] serverSeqStr;

}

//puerto y host corresponden al emisor del mensaje (es para obtener el nick)
//nick corresponde al nick del receptor (es para obtener el puerto y host de envio)
string privateMessage(Comando* comand){//(int puerto, string host, int serverPort, string serverHost, string nick, string msg){

    VariablesGlobales* variablesGlobales = variablesGlobales->getInstance();

    Cliente* emisor = variablesGlobales->buscarCliente(comand->getSourceHost(),comand->getSourcePort());
    Cliente* receptor = variablesGlobales->buscarCliente(comand->getDestinatarioMensajePrivado());

    string message = "<head>";
    message.append(comand->getDestHost());
    message.append("|");

    char* serverPortStr = new char();
    sprintf(serverPortStr,"%d",comand->getDestPort());
    message.append(serverPortStr);
    message.append("|");

    message.append(receptor->host);
    message.append("|");


    char* receptorPortStr = new char();
    sprintf(receptorPortStr,"%d",receptor->port);
    message.append(receptorPortStr);
    message.append("|");

    char* serverSeqStr = new char();
    sprintf(serverSeqStr,"%d",variablesGlobales->getSeqNumber());
    message.append(serverSeqStr);

    message.append("|0|");

    message.append(serverSeqStr);
    message.append("</head><data>PRIVATE_MESSAGE ");


    message.append(emisor->nick);
    message.append(" ");
    message.append(comand->getMensaje());
    message.append("<CR></data>");

    //const char *msj = message.c_str();



    //sendto(socket, msj , strlen(msj)+1, 0 , (struct sockaddr*)&multicastDir , sizeof(multicastDir));

    /** Hay que quedar esperando el ACK de todos **/
    variablesGlobales->changeSeqNumber();

    return message;

    /*delete[] serverPortStr;
    delete[] portStr;
    delete[] seqStr;
    delete[] serverSeqStr;
    delete[] receptorPortStr;*/

}

//puerto y host corresponden al emisor del mensaje (es para borrarlo de los conectados)
void logout(char* serverHost, char* host, int socket, Comando* comand, struct sockaddr_in clienteDireccion, struct sockaddr_in multicastDir){

    VariablesGlobales* variablesGlobales = variablesGlobales->getInstance();

    //Cliente* cliente = variablesGlobales->buscarCliente(comand.getSourceHost(), comand.getSourcePort());
    //variablesGlobales->finConexion(comand.getSourceHost(), comand.getSourcePort());

    //ACK para el que abandona
    char message[256];
    strcpy(message,"<head>");
    strcat(message,serverHost);
    strcat(message,"|");

    char* serverPortStr = new char();
    sprintf(serverPortStr,"%d",comand->getDestPort());
    strcat(message,serverPortStr);
    strcat(message,"|");

    strcat(message,host);
    strcat(message,"|");

    char* portStr = new char();
    sprintf(portStr,"%d",comand->getSourcePort());
    strcat(message,portStr);
    strcat(message,"|");

    char* seqStr = new char();
    sprintf(seqStr,"%d",comand->getNumSeq());
    strcat(message,seqStr);


    strcat(message,"|");
    strcat(message,"1|");

    char* serverSeqStr = new char();
    sprintf(serverSeqStr,"%d",variablesGlobales->getSeqNumber());
    strcat(message,serverSeqStr);
    strcat(message,"</head><data><CR></data>");

    cout << "MESSAGE:" << message << endl;
    //const char *msj = message.c_str();

    sendto(socket, message , strlen(message)+1, 0 , (struct sockaddr*)&clienteDireccion , sizeof(clienteDireccion));

    strcpy(message,"<head>");
    strcat(message,serverHost);
    strcat(message,"|");

    strcat(message,serverPortStr);
    strcat(message,"|");

    strcat(message,host);
    strcat(message,"|");

    strcat(message,portStr);
    strcat(message,"|");


    strcat(message,serverSeqStr);
    strcat(message,"|0|");
    strcat(message,serverSeqStr);
    strcat(message,"</head><data>GOODBYE<CR></data>");

    cout << "MESSAGE2:" << message << endl;
    //msj = message.c_str();

    sendto(socket, message , strlen(message)+1, 0 , (struct sockaddr*)&multicastDir , sizeof(multicastDir));


    /** Hay que quedar esperando el ACK de todos **/
    variablesGlobales->changeSeqNumber();

    delete[] serverPortStr;
    delete[] portStr;
    delete[] seqStr;
    delete[] serverSeqStr;
}


void accionesLogin(char* host, char* nick, int socketServidorAtiendeLogin, Comando* comand, struct sockaddr_in clienteDireccion){

    VariablesGlobales* variablesGlobales = variablesGlobales->getInstance();


    //arma y manda ack si no es duplicado crea nuevo usuario
    string message = "<head>";
    message.append(comand->getDestHost());
    message.append("|");

    char* serverPortStr = new char();
    sprintf(serverPortStr,"%d",comand->getDestPort());
    message.append(serverPortStr);
    message.append("|");

    message.append(comand->getSourceHost());
    message.append("|");

    char* portStr = new char();
    sprintf(portStr,"%d",comand->getSourcePort());
    message.append(portStr);
    message.append("|");

    char* seqStr = new char();
    sprintf(seqStr,"%d",comand->getNumSeq());
    message.append(seqStr);

    message.append("|");
    message.append("1|");

    char* serverSeqStr = new char();
    sprintf(serverSeqStr,"%d",variablesGlobales->getSeqNumber());
    message.append(serverSeqStr);
    message.append("</head><data><CR></data>");



    cout << host << endl;
    cout << comand->getSourcePort()<< endl;
    cout << nick<< endl;
    unsigned int port = comand->getSourcePort();

    //si ya existe solo mandarle ack
    //if (verificarSeq(comand))
    cout << "La cantidad clientes antes de nuevo usuario es: " << variablesGlobales->getCantConectados() << endl;
    variablesGlobales->nuevoUsuario(host,port,nick);
    cout << "La cantidad clientes despues es: " << variablesGlobales->getCantConectados() << endl;


    const char *msj = message.c_str();
    sendto(socketServidorAtiendeLogin, msj , strlen(msj)+1, 0 , (struct sockaddr*)&clienteDireccion , sizeof(clienteDireccion));

};


void MandoMultEsperoAck(int socketAcks,int socketMulti, Comando* comando,struct sockaddr_in multicastDir){

    string message;

    switch ( comando->getTipo()){

        case MESSAGE:{
            message =  relayedMessage(comando);
        }break;

        case PRIVATE_MESSAGE:{
            message = privateMessage(comando);
        }break;

        case GET_CONNECTED:{
            message = getConnected(comando);
        }break;

    }

    const char *msj = message.c_str();
    set<string> conectados;
    bool respondieronTodos = false;
    int intentos = 1;

    char buffer[BUFFERSIZE];

    struct sockaddr_in clienteDireccion;
    socklen_t len = sizeof clienteDireccion;

    VariablesGlobales* variablesGlobales = variablesGlobales->getInstance();

    while ( !respondieronTodos && intentos < 3){

        conectados = variablesGlobales->getConectados();

        sendto(socketMulti, msj , strlen(msj)+1, 0 , (struct sockaddr*)&multicastDir , sizeof(multicastDir));

        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 100000;
        if (setsockopt(socketAcks, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
            perror("Error");
        }

        int cant = 0;

        /**ver como meter un timeout total y no para cada recive **/
        while ( !respondieronTodos || (cant < 2) ){

            recvfrom(socketAcks, buffer ,BUFFERSIZE, 0 , (struct sockaddr*)&clienteDireccion, &len );// no le llega nada porque clientes no mandan a ese puerto

            Comando* com = comandoParsear(buffer);

            cout << com->getNumSeq() << endl;
            cout << variablesGlobales->getSeqNumber() << endl;

            if ((com->getEsAck()) && (com->getNumSeq() == variablesGlobales->getSeqNumber())) {
                Cliente* cli = variablesGlobales->buscarCliente(com->getSourceHost(), com->getSourcePort());
                conectados.erase(cli->nick);
                if (conectados.empty()){
                    respondieronTodos = true;
                    }
            }
            cant++;
        }

        intentos++;


    }

    /**TODO salta error de compilacion**/
    /**si alguno no contesto, se borran los que no respondieron**/
    /*if (intentos == 3){

        for (set<Cliente*>::iterator it = conectados.begin(); it != conectados.end(); ++it){
            Cliente* actual = *it;
            string host = actual->host;
            unsigned int port = actual->port;
            variablesGlobales->finConexion(host,port);
        }
    }*/


    close(socketAcks);

};


void mandarAck(char* serverHost, char* host, int socket, Comando* comand, struct sockaddr_in clienteDireccion){

    VariablesGlobales* variablesGlobales = variablesGlobales->getInstance();

    char message[256];
    strcpy(message,"<head>");
    strcat(message,serverHost);
    strcat(message,"|");

    char* serverPortStr = new char();
    sprintf(serverPortStr,"%d",comand->getDestPort());
    strcat(message,serverPortStr);
    strcat(message,"|");

    strcat(message,host);
    strcat(message,"|");

    char* portStr = new char();
    sprintf(portStr,"%d",comand->getSourcePort());
    strcat(message,portStr);
    strcat(message,"|");

    char* seqStr = new char();
    sprintf(seqStr,"%d",comand->getNumSeq());
    strcat(message,seqStr);


    strcat(message,"|");
    strcat(message,"1|");

    char* serverSeqStr = new char();
    sprintf(serverSeqStr,"%d",variablesGlobales->getSeqNumber());
    strcat(message,serverSeqStr);

    sendto(socket, message , strlen(message)+1, 0 , (struct sockaddr*)&clienteDireccion , sizeof(clienteDireccion));

}


int main()
{

    char buffer[BUFFERSIZE]; //donde se guardan msj

    int socketServidorAtiendeLogin;
    struct sockaddr_in myDireccion;
    struct sockaddr_in clienteDireccion;
    struct sockaddr_in multicasDir;


    int socketParaMulticast = socket(PF_INET,SOCK_DGRAM, 0);
    if(socketParaMulticast == -1)
      cout << "No puedo inicializar el socket";


     multicasDir.sin_family=PF_INET;
     inet_aton(HELLO_GROUP, &multicasDir.sin_addr);
     multicasDir.sin_port=htons(HELLO_PORT);

    socketServidorAtiendeLogin = socket(PF_INET,SOCK_DGRAM, 0);
    if(socketServidorAtiendeLogin == -1)
      cout << "No puedo inicializar el socket";

    myDireccion.sin_family = PF_INET ; //indica el dominio de comunicacion a utilizar
    myDireccion.sin_port = htons(PORT); //puerto htons transforma el int a una ip
    myDireccion.sin_addr.s_addr = htonl(INADDR_ANY); //ip en la que escucha INADDR_ANY significa cualquiera

    if( bind( socketServidorAtiendeLogin, (struct sockaddr*)&myDireccion, sizeof(myDireccion)) == -1 )
      cout << "No Puede Hacer Bind" << endl;

    int socketAcks;
    struct sockaddr_in acksDir;


    socketAcks = socket(PF_INET,SOCK_DGRAM, 0);
    if(socketAcks == -1)
      cout << "No puedo inicializar el socket";

    acksDir.sin_family = PF_INET ; //indica el dominio de comunicacion a utilizar
    acksDir.sin_port = htons(PORTACK); //puerto htons transforma el int a una ip
    acksDir.sin_addr.s_addr = htonl(INADDR_ANY); //ip en la que escucha INADDR_ANY significa cualquiera

    if( bind( socketAcks, (struct sockaddr*)&acksDir, sizeof(acksDir)) == -1 )
    cout << "No Puede Hacer Bind" << endl;


    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 100000;

    if (setsockopt(socketServidorAtiendeLogin, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
        perror("Error");
    }

    cout << "Estoy VIVO" << endl;
    int codigoSalida = 0;
    bool salir = false;
    int hijoPid;
    int pidEstado;
    VariablesGlobales* variablesGlobales = variablesGlobales->getInstance();

    switch ( hijoPid=fork() ){ //creo hijo con fork

       case -1:  // Error
         cout << "ERROR" << endl;
         salir = true;
         break;

       case 0:   // Este es el proceso hijo
         codigoSalida = comandosConsola();

         if (codigoSalida == 1)
            exit(EXIT_SUCCESS);
         else
            exit(EXIT_FAILURE);// Código de salida

       default:  //Es el proceso padre

         break;
    }

    socklen_t len = sizeof clienteDireccion;

     while(!salir){

    //FD_ZERO(&socketsActuales);
    //FD_SET(socketServidorAtiendeLogin, &socketsActuales);

    memset (buffer,NULL,BUFFERSIZE);//vacio buffer


    if(recvfrom(socketServidorAtiendeLogin, buffer ,BUFFERSIZE, 0 , (struct sockaddr*)&clienteDireccion, &len) >= 0){
        cout << "mensaje recibido"<< endl;
        cout << inet_ntoa(clienteDireccion.sin_addr) << endl;
        cout << clienteDireccion.sin_port << endl;
        cout << buffer << endl;

        Comando* comando = comandoParsear(buffer);

        //cout << "**********USUARIO: " << comando->getusuario() << "*********" << endl;
        char* nick = new char[150];
        char* serverHost = new char[60];
        char* host = new char[60];

        switch ( comando->getTipo()) {

            case LOGIN:{

            strcpy(nick,comando->getusuario());
            strcpy(serverHost,comando->getDestHost());
            strcpy(host,comando->getSourceHost());
            cout << "llego LOGIN "<< endl;
            accionesLogin(host, nick, socketServidorAtiendeLogin,comando,clienteDireccion);

            }

            break;

            case LOGOUT:{
            strcpy(serverHost,comando->getDestHost());
            strcpy(host,comando->getSourceHost());
            cout << "llego logout"<< endl;
            logout(serverHost,host, socketParaMulticast,comando,clienteDireccion,multicasDir);
            }
            break;

            default:{
                    strcpy(serverHost,comando->getDestHost());
                    strcpy(host,comando->getSourceHost());
                    mandarAck(serverHost,host, socketParaMulticast,comando,clienteDireccion);
                    /** verificar secuencia sino no existe stop and wait **/
                    switch ( hijoPid=fork() ){ //creo hijo que espera multicast

                       case -1:  // Error
                         cout << "ERROR" << endl;
                         salir = true;
                         break;

                       case 0:   // Este es el proceso hijo
                         MandoMultEsperoAck(socketAcks, socketParaMulticast,comando,multicasDir);

                       default:  //Es el proceso padre

                        break;
                    }
            }
            break;

            }

        delete[] serverHost;
        delete[] host;
        delete[] nick;


    }


        hijoPid=waitpid(-1, &pidEstado, WNOHANG);
        if (hijoPid>0)
          {
            if(WIFEXITED(pidEstado) == 1)
                salir = true;
          }
      }

    close(socketServidorAtiendeLogin);

    return 0;
}




