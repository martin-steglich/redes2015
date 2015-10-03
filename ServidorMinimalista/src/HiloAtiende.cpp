
#include <iostream>
#include <cstdlib>
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
#include <stdio.h>
#include <stdlib.h>
#include "HiloAtiende.h"
#include "ParserComandos.h"

#define BUFFERSIZE 512

int HiloAtiendeCliente(Comando comando, struct sockaddr_in direccionCliente)
{

    char buffer[BUFFERSIZE];
    int code=0;         // Código de salida por defecto
    bool logout;

    //creo Socket de Hijo
    struct sockaddr_in myDireccion;

    int socketAtiende = socket(PF_INET,SOCK_DGRAM, 0);

    if(socketAtiende == -1)
      cout << "No puedo inicializar el socket" << endl;

    myDireccion.sin_family = PF_INET ;
    myDireccion.sin_port = htons(puertoCorrespondiente);
    myDireccion.sin_addr.s_addr = htonl(INADDR_ANY);

    if( bind( socketAtiende, (struct sockaddr*)&myDireccion, sizeof(myDireccion)) == -1 )
      cout << "El puerto no está Disponible" << endl;

    memset(buffer, 0, BUFFERSIZE);
    sprintf(buffer, sizeof(buffer), "%s%d", "Te corresponde el puerto: ", puertoCorrespondiente);

    //envio puerto que le toca y controlo exito
    if(sendto(socketAtiende, buffer, strlen(buffer), direccionCliente ,sizeof(struct sockaddr_in)) == -1)
      error(6, "No puedo enviar información");

    while (!logout){

      memset(buffer, 0, BUFFERSIZE);

      if((recvfrom(socketAtiende, *buffer , BUFFERSIZE , 0 , (struct sockaddr*)&direccionCliente , sizeof(struct direccionCliente) )) == -1)
        error(5, "No puedo recibir información");


     Comando comando =  comandoParsear(*buffer);
     // Evaluamos los comandos
     switch ( comando.getTipo()){

        case LOGIN:
        //decirle que ya esta logeado o error
             break;

        case LOGOUT:
        //mandar goodbye
        logout = true;
            break;

        case GET_CONNECTED:
            break;

        case MESSAGE:
            break;

        case PRIVATE_MESSAGE:
            break;

        default:
           break;
      }
    }

    close(socketAtiende);
    return code;
}

int HiloRechazaClientes(int socketRespuestaLogin, struct sockaddr_in direccionCliente)
{
    char buffer[BUFFERSIZE];
    //lleno con 0's el mensaje
    memset(buffer, 0, BUFFERSIZE);
    //genere el mensaje en buffer
    sprintf(buffer, "Demasiados clientes conectados. Por favor, espere unos minutos\n");
    //envio y controlo exito
    if(sendto(socketRespuestaLogin, buffer, strlen(buffer), direccionCliente ,sizeof(struct sockaddr_in)) == -1)
      error(6, "No puedo enviar información");

    close(socket);

    return 0;
}
