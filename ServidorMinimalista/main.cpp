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
#include <mutex.h>



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

//Número máximo de hijos
#define MAX_HIJOS 10 // TODO para empezar acordarse de poner un numero mayor

//Longitud del buffer
#define BUFFERSIZE 512 // poner el de los datagramas

int comandosConsola(Mutex &mtx){

    string command;
    bool exit = false;
    mtx.lock();
    VariablesGlobales* variablesGlobales = variablesGlobales->getInstance();
    mtx.unlock();

    while (!exit){
        cin >> command;

        if(command.compare("exit") == 0)
            exit = true;
        else if(command.compare("a") == 0)
            cout << "La cantidad clientes es: " << variablesGlobales->getCantConectados() << endl;

        else if(command.compare("a") == 0){
                mtx.lock();
                cout << "La cantidad clientes es: " << variablesGlobales->getCantConectados();
                mtx.unlock();
        }
        else if(command.compare("s") == 0)
            cout << "Cantidad mensajes enviados" << endl;
        else if(command.compare("d") == 0)
            cout << "Cantidad conexiones totales" << endl;
        else if(command.compare("f") == 0){
            mtx.lock();
            time_t activeTime = variablesGlobales->getActiveTime();
            mtx.unlock();

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
void getConnected(int puerto, string host, int serverPort, string serverHost){
    VariablesGlobales* variablesGlobales = variablesGlobales->getInstance();

    set<string> users = variablesGlobales->getConectados();

    string message = "<head>";
    message.append(serverHost);
    message.append("|");

    char* serverPortStr = new char();
    sprintf(serverPortStr,"%d",serverPort);
    message.append(serverPortStr);
    message.append("|");

    message.append(host);
    message.append("|");

    char* portStr = new char();;
    sprintf(portStr,"%d",puerto);
    message.append(portStr);
    message.append("|");

    char* seqStr = new char();;
    sprintf(seqStr,"%d",variablesGlobales->getSeqNumber());
    message.append(seqStr);

    message.append("|");
    message.append("0</head><data>CONNECTED ");

    for (set<string>::iterator it = users.begin(); it != users.end(); ++it){
        if(it != users.begin())
            message.append(" |");

        string actual = *it;
        message.append(actual);
    }
    message.append("<CR></data>");

    /** ENVIAR MENSAJE AL MULTICAST **/
    /** Hay que quedar esperando el ACK de todos **/
    variablesGlobales->changeSeqNumber();
}

//puerto y host corresponden al emisor del mensaje (es para obtener el nick)
void relayedMessage(int puerto, string host, int serverPort, string serverHost, string msg){
    VariablesGlobales* variablesGlobales = variablesGlobales->getInstance();

    Cliente* cliente = variablesGlobales->buscarCliente(host, puerto);

    string message = "<head>";
    message.append(serverHost);
    message.append("|");

    char* serverPortStr = new char();
    sprintf(serverPortStr,"%d",serverPort);
    message.append(serverPortStr);
    message.append("|");
    message.append("0");
    message.append("|");
    message.append("0");
    message.append("|");

    char* seqStr = new char();
    sprintf(seqStr,"%d",variablesGlobales->getSeqNumber());
    message.append(seqStr);

    message.append("|");
    message.append("0</head><data>RELAYED_MESSAGE ");
    message.append(cliente->nick);
    message.append(" ");
    message.append(msg);
    message.append("<CR></data>");

    /** ENVIAR MENSAJE AL MULTICAST **/
    /** Hay que quedar esperando el ACK de todos **/
    variablesGlobales->changeSeqNumber();

}

//puerto y host corresponden al emisor del mensaje (es para obtener el nick)
//nick corresponde al nick del receptor (es para obtener el puerto y host de envio)
void privateMessage(int puerto, string host, int serverPort, string serverHost, string nick, string msg){
    VariablesGlobales* variablesGlobales = variablesGlobales->getInstance();

    Cliente* emisor = variablesGlobales->buscarCliente(host, puerto);
    Cliente* receptor = variablesGlobales->buscarCliente(nick);

    string message = "<head>";
    message.append(serverHost);
    message.append("|");

    char* serverPortStr = new char();
    sprintf(serverPortStr,"%d",serverPort);
    message.append(serverPortStr);
    message.append("|");

    message.append(receptor->host);
    message.append("|");

    char* portStr = new char();
    sprintf(portStr,"%d",receptor->port);
    message.append(portStr);
    message.append("|");

    char* seqStr = new char();;
    sprintf(seqStr,"%d",variablesGlobales->getSeqNumber());
    message.append(seqStr);

    message.append("|");
    message.append("0</head><data>PRIVATE_MESSAGE ");
    message.append(emisor->nick);
    message.append(" ");
    message.append(msg);
    message.append("<CR></data>");

    /** ENVIAR MENSAJE AL MULTICAST **/
    /** Hay que quedar esperando el ACK de todos **/
    variablesGlobales->changeSeqNumber();

}
//puerto y host corresponden al emisor del mensaje (es para borrarlo de los conectados)
void logout(int socket, Comando comand, struct sockaddr_in clienteDireccion, struct sockaddr_in multicastDir){

    VariablesGlobales* variablesGlobales = variablesGlobales->getInstance();

    //Cliente* cliente = variablesGlobales->buscarCliente(comand.getSourceHost(), comand.getSourcePort());
    //variablesGlobales->finConexion(comand.getSourceHost(), comand.getSourcePort());

    //ACK para el que abandona debe ser Bolso
    string message = "<head>";
    message.append(comand.getDestHost());
    message.append("|");

    char* serverPortStr = new char();
    sprintf(serverPortStr,"%d",comand.getDestPort());
    message.append(serverPortStr);
    message.append("|");

    message.append(comand.getSourceHost());
    message.append("|");

    char* portStr = new char();
    sprintf(portStr,"%d",comand.getSourcePort());
    message.append(portStr);
    message.append("|");

    char* seqStr = new char();
    sprintf(seqStr,"%d",comand.getNumSeq());
    message.append(seqStr);

    message.append("|");
    message.append("1</head><data><CR></data>");

    const char *msj = message.c_str();

    sendto(socket, msj , strlen(msj)+1, 0 , (struct sockaddr*)&clienteDireccion , sizeof(clienteDireccion));

    message = "<head>";
    message.append(comand.getSourceHost());
    message.append("|");

    serverPortStr = new char();
    sprintf(serverPortStr,"%d",comand.getSourcePort());
    message.append(serverPortStr);
    message.append("|");

    message.append(comand.getDestHost());
    message.append("|");

    portStr = new char();
    sprintf(portStr,"%d",comand.getDestPort());
    message.append(portStr);
    message.append("|");

    seqStr = new char();
    sprintf(seqStr,"%d",comand.getNumSeq());
    message.append(seqStr);

    message.append("|");
    message.append("0</head><data>GOODBYE<CR></data>");

    msj = message.c_str();

    sendto(socket, msj , strlen(msj)+1, 0 , (struct sockaddr*)&multicastDir , sizeof(multicastDir));


    /** Hay que quedar esperando el ACK de todos **/
    variablesGlobales->changeSeqNumber();
}


void accionesLogin(int socketServidorAtiendeLogin, Comando comand, struct sockaddr_in clienteDireccion){
    //arma y manda ack si no es duplicado crea nuevo usuario
    string message = "<head>";
    message.append(comand.getDestHost());
    message.append("|");

    char* serverPortStr = new char();
    sprintf(serverPortStr,"%d",comand.getDestPort());
    message.append(serverPortStr);
    message.append("|");

    message.append(comand.getSourceHost());
    message.append("|");

    char* portStr = new char();
    sprintf(portStr,"%d",comand.getSourcePort());
    message.append(portStr);
    message.append("|");

    char* seqStr = new char();
    sprintf(seqStr,"%d",comand.getNumSeq());
    message.append(seqStr);

    message.append("|");
    message.append("1</head><data><CR></data>");

    VariablesGlobales* variablesGlobales = variablesGlobales->getInstance();
    cout << comand.getSourceHost() << endl;
    cout << comand.getSourcePort()<< endl;
    cout << comand.getusuario()<< endl;

    //si ya existe solo mandarle ack
    //if (verificarSeq(comando))
        //variablesGlobales->nuevoUsuario(comand.getSourceHost(),comand.getSourcePort(),comand.getusuario());

    const char *msj = message.c_str();
    sendto(socketServidorAtiendeLogin, msj , strlen(msj)+1, 0 , (struct sockaddr*)&clienteDireccion , sizeof(clienteDireccion));

};

int main()
{

    char buffer[BUFFERSIZE]; //donde se guardan msj

    int socketServidorAtiendeLogin;
    struct sockaddr_in myDireccion;
    struct sockaddr_in clienteDireccion;
    struct sockaddr_in multicasDir;

    int socketParaMulticast = socket(PF_INET,SOCK_DGRAM, 0);
    if(socketServidorAtiendeLogin == -1)
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

    cout << "Estoy VIVO" << endl;
    int codigoSalida = 0;
    bool salir = false;
    int hijoPid;
    int pidEstado;
    Mutex mtx = Mutex();


    mtx.lock();
    VariablesGlobales* variablesGlobales = variablesGlobales->getInstance();
    mtx.unlock();

    switch ( hijoPid=fork() ){ //creo hijo con fork

       case -1:  // Error
         cout << "ERROR" << endl;
         salir = true;
         break;

       case 0:   // Este es el proceso hijo
         codigoSalida = comandosConsola(mtx);

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


    recvfrom(socketServidorAtiendeLogin, buffer ,BUFFERSIZE, 0 , (struct sockaddr*)&clienteDireccion, &len);
    cout << "mensaje recibido"<< endl;
    cout << inet_ntoa(clienteDireccion.sin_addr) << endl;
    cout << clienteDireccion.sin_port << endl;
    cout << buffer << endl;

    Comando comando = comandoParsear(buffer);

    switch ( comando.getTipo()) {

        case LOGIN:{
        cout << "llego LOGIN "<< endl;
        accionesLogin(socketServidorAtiendeLogin,comando,clienteDireccion);
        }

        break;

        case LOGOUT:{
        cout << "llego logout"<< endl;
        logout(socketParaMulticast,comando,clienteDireccion,multicasDir);
        }
        break;

        case GET_CONNECTED:{
        cout << "llego get connected"<< endl;
        //accionesGetConnected();
        }
        break;

        case MESSAGE:{
        cout << "llego message"<< endl;
        //accionesMessage();
        }
        break;

        case PRIVATE_MESSAGE:{
        cout << "llego private message"<< endl;
        //accionesPrivateMessage();
        }
        break;

        default:
        cout << "error al parsear"<< endl;
        break;

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




