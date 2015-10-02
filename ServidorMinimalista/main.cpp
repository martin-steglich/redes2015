#include <iostream>
#include <cstdlib>
//#include "Cliente.h"
//#include "HiloAtiende.h"
//#include "Errores.h"
//#include "ParserComandos.h"
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
#include <time.h>


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



//Puerto
#define PORT 54321

//Número máximo de hijos
#define MAX_HIJOS 10 // TODO para empezar acordarse de poner un numero mayor

//Longitud del buffer
#define BUFFERSIZE 512 // poner el de los datagramas

int comandosConsola(){
    string command;
    bool exit = exit;

    VariablesGlobales* variablesGlobales = variablesGlobales->getInstance();

    //Mutex mutex;
    while (!exit){
        cin >> command;

        if(command.compare("exit") == 0)
            exit = true;
        else if(command.compare("a") == 0)
            cout << "La cantidad clientes es: " << variablesGlobales->getCantConectados();

        else if(command.compare("s") == 0)
            cout << "Cantidad mensajes enviados" << endl;
        else if(command.compare("d") == 0)
            cout << "Cantidad conexiones totales" << endl;
        else if(command.compare("f") == 0){
            time_t activeTime = variablesGlobales->getActiveTime();

            time_t serverTime;
            time(&serverTime);
            double seconds = difftime(serverTime, activeTime);
            printf ("El servidor se encuentra activo hace %.f segundos", seconds);
        }
    }

    return 1;
}

int main()
{
    cout << "Hello world!" << endl;
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

    /*char buffer[BUFFERSIZE];
    fd_set socketsActuales;

    int socketServidorAtiendeLogin;
    struct sockaddr_in myDireccion;
    struct sockaddr_in clienteDireccion;

    socketServidorAtiendeLogin = socket(PF_INET,SOCK_DGRAM, 0);
    if(socketServidorAtiendeLogin == -1)
      cout << "No puedo inicializar el socket";

    myDireccion.sin_family = PF_INET ; //indica el dominio de comunicacion a utilizar
    myDireccion.sin_port = htons(PORT); //puerto htons transforma el int a una ip
    myDireccion.sin_addr.s_addr = htonl(INADDR_ANY); //ip en la que escucha INADDR_ANY significa cualquiera

    int contador = 0;


    if( bind( socketServidorAtiendeLogin, (struct sockaddr*)&myDireccion, sizeof(myDireccion)) == -1 )
      cout << "No Puede Hacer Bind" << endl;

    while ( contador < 2 ){

    socklen_t len = sizeof clienteDireccion;


    FD_ZERO(&socketsActuales);
    FD_SET(socketServidorAtiendeLogin, &socketsActuales);

    recvfrom(socketServidorAtiendeLogin, buffer ,BUFFERSIZE, 0 , (struct sockaddr*)&clienteDireccion, &len);
    cout << "mensaje recibido"<< endl;
    cout << inet_ntoa(clienteDireccion.sin_addr) << endl;
    cout << clienteDireccion.sin_port << endl;
    cout << buffer << endl;

    clienteDireccion.sin_port = htons(40291);
    cout << clienteDireccion.sin_port << endl;
     if(sendto(socketServidorAtiendeLogin, buffer, strlen(buffer)+1, 0 , (struct sockaddr*)&clienteDireccion , sizeof(clienteDireccion) ) == -1)
        cout << "No Puede mandar msj";

   cout << clienteDireccion.sin_port << endl;


       contador++;
    }

    close(socketServidorAtiendeLogin);*/
    while(!salir){

        variablesGlobales->nuevoUsuario();
        hijoPid=waitpid(-1, &pidEstado, WNOHANG);
        if (hijoPid>0)
          {
            if(WIFEXITED(pidEstado) == 1)
                salir = true;// un hijo menos
            // TODO podemos usar los codigos de salida para matar al servidor o los comandos
            //bajar la bandera servidorArriba, etc
          }
      }
    return 0;
}




