/* 
 * File:   main.cpp
 * Author: Tincho
 *
 * Created on September 14, 2015, 11:04 PM
 */

#include <cstdlib>
#include "Cliente.h"
#include "HiloAtiende.h"
#include "Errores.h"
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
#include <string.h>


using namespace std;

//Puerto 
#define PORT 54321

//Número máximo de hijos
#define MAX_HIJOS 10 // TODO para empezar acordarse de poner un numero mayor

//Longitud del buffer 
#define BUFFERSIZE 512 // poner el de los datagramas 

/*int main(int argc, char** argv) {
    Cliente* cliente = new Cliente("Hola", 12, "chau");
    
    string host = cliente->getHost();
    int port = cliente->getPort();
    string portStr = string(port);
    string nick = cliente->getNick();
    cout << "HOST: " + host;
    cout << "PORT: " + portStr;
    cout << "Nick: " + nick;
   
    return 0;
}*/

int main(int argv, char** argc){
    
    int socketServidorAtiendeLogin;
    int socketRespuestaLogin;
    struct sockaddr_in clienteDireccion;
    struct sockaddr_in myDireccion;
    fd_set socketsActuales;        // Conjunto de descriptores a controlar
    int contadorHijos=0;
    int codigoSalida;
    char buffer[BUFFERSIZE];
    int hijoPid;
    int pidEstado;
    ssize_t tamañoMsjRec;
    bool servidorArriba= true;
    int puertoCorrespondiente = PORT +1 ;
    Comando comando; 
    
    //crea Socket que atiende logins --socket(dominio de comunicacion ,tipo Comunicacion , protocolo)
    socketServidorAtiendeLogin = socket(PF_INET,SOCK_DGRAM, 0);
    if(socketServidorAtiendeLogin == -1)
      error(1, "No puedo inicializar el socket");
    
    socketRespuestaLogin = socket(PF_INET,SOCK_DGRAM, 0);
    if(socketRespuestaLogin == -1)
      error(1, "No puedo inicializar el socket");
    
    //Definde Estructura que mantiene info de direccion de comunicaion
    myDireccion.sin_family = PF_INET ; //indica el dominio de comunicacion a utilizar 
    myDireccion.sin_port = htons(PORT); //puerto htons transforma el int a una ip
    myDireccion.sin_addr.s_addr = htonl(INADDR_ANY); //ip en la que escucha INADDR_ANY significa cualquiera
    socklen_t tamañoDireccion = sizeof(sockaddr_in);

    //asocia socket con puerto --bind(socket ya creado, puntero a estructura casteado, tamaño estructura)
    if( bind( socketServidorAtiendeLogin, (struct sockaddr*)&myDireccion, sizeof(myDireccion)) == -1 )
      error(2, "El puerto no está Disponible"); // Error al hacer el bind() 

   
    while(servidorArriba)
    {
    //se inicializa y añade el socket creado a la estructura que controlamos  
    FD_ZERO(&socketsActuales);
    FD_SET(socketServidorAtiendeLogin, &socketsActuales);

    //select() bloquea el proceso hasta que: un cliente se conecte, desconecte o mande un msj para UDP solo corre lo primero
    if (select(socketServidorAtiendeLogin+1, &socketsActuales, NULL, NULL, NULL)) //controlar error con -1
      {
        tamañoMsjRec = recvfrom(socketServidorAtiendeLogin, *buffer , BUFFERSIZE , 0, (struct sockaddr*)&clienteDireccion , tamañoDireccion ); //recibimos msj
        comando = comandoParsear(*buffer);// TODO parsear entrada y devolver estructura con los datos obtenidos para reconocer accion deberia llegar solo login
        if ( comando.getTipo() == LOGIN ){
           switch ( hijoPid=fork() ){ //creo hijo con fork
         
               case -1:  // Error
                 error(4, "No se puede crear el proceso hijo");
                 break;
                 
               case 0:   // Este es el proceso hijo
                 if ( contadorHijos < MAX_HIJOS ){
                   codigoSalida=HiloAtiendeCliente(socketRespuestaLogin, clienteDireccion, puertoCorrespondiente);
                   puertoCorrespondiente++; }
                 else
                   codigoSalida=HiloRechazaClientes(socketRespuestaLogin, clienteDireccion);

                 exit(codigoSalida); // Código de salida
                
               default:  //Es el proceso padre
                 contadorHijos++;
                 break;
            }
        }
    }    

    // Miramos si murio algún hijo  waitpid( espera por el proceso que llama, estado, opcion volver si no murio nadie)
    hijoPid=waitpid(0, &pidEstado, WNOHANG);
    if (hijoPid>0)
      {
        contadorHijos--;   // un hijo menos 
        // TODO podemos usar los codigos de salida para matar al servidor o los comandos
        //bajar la bandera servidorArriba, etc
      }
    }

    close(socketServidorAtiendeLogin);

    return 0;
}
