#include <iostream>
#include <cstdlib>
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
#include "mutex.h"
#include <stdexcept>
#include <set>




//#include "VariablesGlobales.h"

using namespace std;

#define HELLO_PORT 5000
#define HELLO_GROUP "225.5.4.29"

#define PORT 54321
#define PORTACK 51234

#define BUFFERSIZE 512

struct Cliente{
    string host;
    unsigned int port;
    string nick;
    unsigned int senderSeq;
    time_t activeTime;
};

//-----------------------------------------
//static int cantConectados;
int cantMensajesEnviados;
int cantConexionesTotales;
unsigned int seqNumber;
time_t activeTime;
Mutex mtx;
set<Cliente*> conectados;
bool salir;
Comando* command;
//-----------------------------------------


set<string> getNickConectados(){
    set<string> connected;
    for (set<Cliente*>::iterator it = conectados.begin(); it != conectados.end(); ++it){
        Cliente* actual = *it;
        string nick = actual->nick;

        connected.insert(nick);
    }

    return connected;
}

Cliente* buscarCliente(string nick){
    cout << "ENTRE2" << endl;
    for (set<Cliente*>::iterator it = conectados.begin(); it != conectados.end(); ++it){
        Cliente* actual = *it;
        if(actual->nick == nick){
            return actual;
        }
    }
    cout << "SALI2" << endl;
    return NULL;
}

Cliente* buscarCliente(string host, unsigned int port){

    mtx.lock();
    for (set<Cliente*>::iterator it = conectados.begin(); it != conectados.end(); ++it){
        Cliente* actual = *it;
        if((actual->host == host) && (actual->port == port)){
            mtx.unlock();
            return actual;
        }
    }

    mtx.unlock();
    return NULL;

}

void *comandosConsola(void* arg){

    string command;
    bool exit = false;

    while (!exit){
        cin >> command;
        if(command.compare("exit") == 0){
            exit = true;
            mtx.lock();
            salir = true;
            mtx.unlock();
        }
        else if(command.compare("a") == 0){
                set<string> users = getNickConectados();
                cout << "La cantidad de clientes conectados es: " << users.size() << endl;
        }
        else if(command.compare("s") == 0){
            cout << "La cantidad de mensajes enviados es: " << cantMensajesEnviados << endl;
        }else if(command.compare("d") == 0){
            cout << "La cantidad de conexiones totales es: " << cantConexionesTotales << endl;
        }else if(command.compare("f") == 0){
            time_t serverTime;
            time(&serverTime);
            double seconds = difftime(serverTime, activeTime);
            printf ("El servidor se encuentra activo hace %.f segundos\n", seconds);
        }
        cout<< endl;
    }

    //pthread_exit(NULL);
}

void changeSeqNumber(){

    if(seqNumber == 1)
        seqNumber = 0;
    else
        seqNumber = 1;
}

bool existeCliente(string nick){

    mtx.lock();
    for (set<Cliente*>::iterator it = conectados.begin(); it != conectados.end(); ++it){
        Cliente* actual = *it;
        if(actual->nick == nick){
            mtx.unlock();
            return true;
        }
    }
    mtx.unlock();
    return false;
}

void nuevoUsuario(string host, int port, string nick){
     if(!(existeCliente(nick))){
        Cliente* cliente = new Cliente;
        cliente->host = host;
        cliente->port = port;
        cliente->nick = nick;
        time(&cliente->activeTime);
        cliente->senderSeq = 0;
        conectados.insert(cliente);

        //cantConectados++;
        cantConexionesTotales++;
        cout << "#clientes desde nuevoUsuario() es: " << conectados.size() << endl;

    }

}

void finConexion(string host, unsigned int port){

    for (set<Cliente*>::iterator it = conectados.begin(); it != conectados.end(); ++it){
        Cliente* actual = *it;
        if((actual->host == host) && (actual->port == port)){
            conectados.erase(it);
        }
    }


}

void nuevoMensaje(){
    cantMensajesEnviados++;
}

bool existeCliente(string host, unsigned int port){

    mtx.lock();
    for (set<Cliente*>::iterator it = conectados.begin(); it != conectados.end(); ++it){
        Cliente* actual = *it;
        if((actual->host == host) && (actual->port == port)){
            mtx.unlock();
            return true;
        }
    }
    mtx.unlock();
    return false;
}

void numeroSecuenciaCliente(string host, unsigned int port){
    Cliente* cliente = buscarCliente(host, port);
    if(cliente->senderSeq == 1)
        cliente->senderSeq = 0;
    else
        cliente->senderSeq = 1;

}

void vaciarMemoria(){
    for (set<Cliente*>::iterator it = conectados.begin(); it != conectados.end(); ++it){
        Cliente* actual = *it;
        delete actual;
    }
    conectados.clear();
}

char* getConnectedMessage(Comando* command){
    set<string> users = getNickConectados();

    char* message = new char();
    strcpy(message, "<head>");
    cout << "******-2: " << message << endl;
    strcat(message, command->getDestHost());
    cout << "command->getDestHost(): " << command->getDestHost() << endl;
    strcat(message, "|");
    cout << "******-1: " << message << endl;
    char* serverPortStr = new char();
    sprintf(serverPortStr,"%d",command->getDestPort());
    strcat(message,serverPortStr);
    strcat(message,"|");
    cout << "******: " << message << endl;
    strcat(message, command->getSourceHost());
    strcat(message, "|");
    cout << "******0: " << message << endl;
    char* portStr = new char();
    sprintf(portStr,"%d",command->getSourcePort());
    strcat(message,portStr);
    strcat(message,"|");
    cout << "******1: " << message << endl;
    char* serverSeqStr = new char();
    sprintf(serverSeqStr,"%d",seqNumber);
    strcat(message,serverSeqStr);
    strcat(message, "|");

    strcat(message, "0|");
    cout << "******2: " << message << endl;
    strcat(message,serverSeqStr);
    strcat(message, "</head><data>CONNECTED ");
    cout << "******3: " << message << endl;
    for (set<string>::iterator it = users.begin(); it != users.end(); ++it){
        if(it != users.begin())
            strcat(message," |");

        string actual = *it;
        strcat(message,actual.c_str());
    }
    strcat(message,"<CR></data>");

    cout << "******GETCONNECTED: " << message << endl;

    return message;
}

char* getRelayedMessage(Comando* command){

    Cliente* cliente = buscarCliente(command->getSourceHost(), command->getSourcePort());

    char* message = new char();
    strcpy(message, "<head>");
    strcat(message, command->getDestHost());
    strcat(message, "|");

    char* serverPortStr = new char();
    sprintf(serverPortStr,"%d",command->getDestPort());
    strcat(message,serverPortStr);
    strcat(message,"|");

    strcat(message, "0");
    strcat(message, "|");
    strcat(message, "0");
    strcat(message, "|");

    char* serverSeqStr = new char();
    sprintf(serverSeqStr,"%d",seqNumber);
    strcat(message,serverSeqStr);
    strcat(message, "|");

    strcat(message, "0|");

    strcat(message,serverSeqStr);
    strcat(message, "</head><data>RELAYED_MESSAGE ");

    strcat(message, (cliente->nick).c_str());
    strcat(message, " ");
    strcat(message, command->getMensaje());
    strcat(message, "<CR></data>");



    return message;
}

char* getPrivateMessage(Comando* command){

    Cliente* cliente = buscarCliente(command->getSourceHost(), command->getSourcePort());
    Cliente* receptor =buscarCliente(command->getDestinatarioMensajePrivado());

    char* message = new char();
    strcpy(message, "<head>");
    strcat(message, command->getDestHost());
    strcat(message, "|");

    char* serverPortStr = new char();
    sprintf(serverPortStr,"%d",command->getDestPort());
    strcat(message,serverPortStr);
    strcat(message,"|");

    strcat(message, (receptor->host).c_str());
    strcat(message, "|");

    char* portStr = new char();
    sprintf(portStr,"%d",receptor->port);
    strcat(message,portStr);
    strcat(message,"|");

    char* serverSeqStr = new char();
    sprintf(serverSeqStr,"%d",seqNumber);
    strcat(message,serverSeqStr);
    strcat(message, "|");

    strcat(message, "0|");

    strcat(message,serverSeqStr);
    strcat(message, "</head><data>PRIVATE_MESSAGE ");

    strcat(message, (cliente->nick).c_str());
    strcat(message, " ");
    strcat(message, command->getMensaje());
    strcat(message, "<CR></data>");



    return message;
}

char* getLogoutMessage(Comando* command){

    char* message = new char();
    strcpy(message, "<head>");
    strcat(message, command->getDestHost());
    strcat(message, "|");

    char* serverPortStr = new char();
    sprintf(serverPortStr,"%d",command->getDestPort());
    strcat(message,serverPortStr);
    strcat(message,"|");

    strcat(message, command->getSourceHost());
    strcat(message, "|");

    char* portStr = new char();
    sprintf(portStr,"%d",command->getSourcePort());
    strcat(message,portStr);
    strcat(message,"|");

    char* serverSeqStr = new char();
    sprintf(serverSeqStr,"%d",seqNumber);
    strcat(message,serverSeqStr);
    strcat(message, "|");

    strcat(message, "0|");

    strcat(message,serverSeqStr);
    strcat(message, "</head><data>GOODBYE");
    strcat(message,"<CR></data>");



    return message;
}

void sendACK(Comando* command, struct sockaddr_in clienteDireccion){

    char* message = new char();
    strcpy(message,"<head>");
    strcat(message,command->getDestHost());
    strcat(message,"|");

    char* serverPortStr = new char();
    sprintf(serverPortStr,"%d",command->getDestPort());
    strcat(message,serverPortStr);
    strcat(message,"|");

    strcat(message,command->getSourceHost());
    strcat(message,"|");

    char* portStr = new char();
    sprintf(portStr,"%d",command->getSourcePort());
    strcat(message,portStr);
    strcat(message,"|");

    char* seqStr = new char();
    sprintf(seqStr,"%d",command->getNumSeq());
    strcat(message,seqStr);


    strcat(message,"|");
    strcat(message,"1|");

    char* serverSeqStr = new char();
    sprintf(serverSeqStr,"%d",seqNumber);
    strcat(message,serverSeqStr);
    strcat(message,"</head><data></data>");

    int socketAck = socket(PF_INET,SOCK_DGRAM, 0);
    if(socketAck == -1)
      cout << "No puedo inicializar el socket para enviar el ack" << endl;

    sendto(socketAck, message , strlen(message)+1, 0 , (struct sockaddr*)&clienteDireccion , sizeof(clienteDireccion));
}

void* sendMessage(void* arg){

    char* message;
    switch (command->getTipo()){
        case MESSAGE:{
            message = getRelayedMessage(command);
        }break;

        case PRIVATE_MESSAGE:{
            message = getPrivateMessage(command);
        }break;

        case GET_CONNECTED:{
            message = getConnectedMessage(command);
        }break;

        case LOGOUT:{
            message = getLogoutMessage(command);
        }
    }
    int senderSocket = socket(PF_INET,SOCK_DGRAM, 0);
    int receiverSocket = socket(PF_INET,SOCK_DGRAM, 0);

    if(senderSocket != -1 && receiverSocket != -1){
        struct sockaddr_in multicast;
        multicast.sin_family=PF_INET;
        inet_aton(HELLO_GROUP, &multicast.sin_addr);
        multicast.sin_port=htons(HELLO_PORT);

        struct sockaddr_in ack;
        ack.sin_family = PF_INET;
        ack.sin_port = htons(PORTACK);
        ack.sin_addr.s_addr = htonl(INADDR_ANY);

        struct sockaddr_in ackReceiver;
        socklen_t len = sizeof ackReceiver;

        if ((bind(receiverSocket, (struct sockaddr*)&ack, sizeof(ack)) != -1)){
            sendto(senderSocket, message , strlen(message)+1, 0 , (struct sockaddr*)&multicast , sizeof(multicast));
            set<string> conectados = getNickConectados();
            int attempts = 0;

            while((!conectados.empty() && (attempts < 4))){
                struct timeval tv;
                tv.tv_sec = 0;
                tv.tv_usec = 100000;
                if (setsockopt(receiverSocket, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
                    perror("Error");
                    break;
                }

                char buffer[BUFFERSIZE];
                if(recvfrom(receiverSocket, buffer ,BUFFERSIZE, 0 , (struct sockaddr*)&ackReceiver, &len) >= 0){
                    cout << "ALGO RECIBIDO" << endl;
                    Comando* ackCommand = comandoParsear(buffer);
                    if((ackCommand->getEsAck()) && (ackCommand->getNumSeq() == seqNumber)){
                        Cliente* cli = buscarCliente(ackCommand->getSourceHost(), ackCommand->getSourcePort());
                        conectados.erase(cli->nick);
                        cout << "ACK BIEN RECIBIDO" << endl;
                    }

                }else{
                    sendto(senderSocket, message , strlen(message)+1, 0 , (struct sockaddr*)&multicast , sizeof(multicast));
                    conectados = getNickConectados();
                    attempts++;
                }

            }

            if(attempts == 4){
                for (set<string>::iterator it = conectados.begin(); it != conectados.end(); ++it){
                    string actual = *it;
                    Cliente* noAck = buscarCliente(actual);
                    finConexion(noAck->host,noAck->port);
                }
            }
            switch (command->getTipo()){
                case MESSAGE:{
                    nuevoMensaje();
                }break;

                case PRIVATE_MESSAGE:{
                    nuevoMensaje();
                }break;

                case LOGOUT:{
                    Cliente* loggedOut = buscarCliente(command->getSourceHost(), command->getSourcePort());
                    finConexion(loggedOut->host,loggedOut->port);

                }
            }


        }else{
            cout << "No puede hacer bind" << endl;
        }


    }else{
        cout << "No puedo inicializar el socket" << endl;
    }



    close(receiverSocket);
   //return 1;
}


int main(){

    //cantConectados = 0;
    cantMensajesEnviados = 0;
    cantConexionesTotales = 0;
    seqNumber = 0;
    activeTime = time(&activeTime);
    mtx = Mutex();

    cout << "Estoy VIVO" << endl;

    salir = false;
    char buffer[BUFFERSIZE];

    int receiverSocket = socket(PF_INET,SOCK_DGRAM, 0);
    if(receiverSocket == -1){
        cout << "No es posible inicializar el socket" << endl;
        return -1;
    }

    struct sockaddr_in myAddress;
    myAddress.sin_family = PF_INET ; //indica el dominio de comunicacion a utilizar
    myAddress.sin_port = htons(PORT); //puerto htons transforma el int a una ip
    myAddress.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(receiverSocket, (struct sockaddr*)&myAddress, sizeof(myAddress)) == -1){
            cout << "No Puede Hacer Bind" << endl;
            return -1;
    }

    struct sockaddr_in senderAddress;
    socklen_t len = sizeof senderAddress;

    pthread_t senderThread;

    //int pidComandosConsola = fork();//Creo el hilo para los comandos de consola
    /*switch(pidComandosConsola){
        case -1:{
            cout << "Error al crear el hilo para los comandos de consola";
            return -1;
        }break;

        case 0: {//Proceso hijo
            if(comandosConsola() == 1){
                exit(EXIT_SUCCESS);
            }else
                exit(EXIT_FAILURE);
        }break;
    }*/

    pthread_t consoleCommandThread;
    pthread_create(&consoleCommandThread, NULL, &comandosConsola, NULL);

    mtx.lock();
    while(!salir){
        mtx.unlock();
        memset (buffer,NULL,BUFFERSIZE);//vacio buffer
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 100000;

        if (setsockopt(receiverSocket, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
            cout << "No se puede setear el timeout" << endl;
            return -1;
        }

        if(recvfrom(receiverSocket, buffer ,BUFFERSIZE, 0 , (struct sockaddr*)&senderAddress, &len) >= 0){
            cout << "Mensaje recibido: "<< buffer << endl;
            cout << "IP origen: " << inet_ntoa(senderAddress.sin_addr) << endl;
            cout << "Puerto origen: " << senderAddress.sin_port << endl;

            command = comandoParsear(buffer);
            sendACK(command, senderAddress);
            Cliente* sender = buscarCliente(command->getSourceHost(), command->getSourcePort());
            if(((sender != NULL)&&(sender->senderSeq == command->getNumSeq())) || ((sender == NULL) && (command->getNumSeq() == 0))){
                switch(command->getTipo()){

                    case LOGIN:{
                        nuevoUsuario(command->getSourceHost(),command->getSourcePort(), command->getusuario());
                        numeroSecuenciaCliente(command->getSourceHost(),command->getSourcePort());

                    }break;
                    default:{
                        pthread_create(&senderThread, NULL, &sendMessage, NULL);
                        int childState;
                        int ret = pthread_join(senderThread, (void **)&childState);
                        if(ret != 0){
                            cout << "Ocurrio un error en el envio del mensaje" << endl;
                            return -1;
                        }
                        if(command->getTipo() != LOGOUT)
                            numeroSecuenciaCliente(command->getSourceHost(),command->getSourcePort());
                        
                           /* case -1:{
                                cout << "Error al crear el hilo para enviar la contestacion del mensaje" << endl;
                                return -1;
                            }break;

                            case 0:{//Proceso hijo
                                if(sendMessage(command) == 1){
                                    exit(EXIT_SUCCESS);
                                }else{
                                   exit(EXIT_FAILURE);
                                }
                            }break;

                            default:{//Proceso padre
                                int childState;
                                int endedChild = waitpid(senderThread,&childState, 0);
                                if(WIFEXITED(childState) != 1){
                                    cout << "Ocurrio un error en el envio del mensaje" << endl;
                                    return -1;
                                }
                                numeroSecuenciaCliente(command->getSourceHost(),command->getSourcePort());
                            }break;*/

                        }

                    }

                }

            

            //command->~Comando();
            
        }
        mtx.lock();
        /*int exitedPid = waitpid(pidComandosConsola, &consolaState, WNOHANG);
        if(exitedPid >0){
            if(WIFEXITED(consolaState) == 1)
                salir = true;
            else{
                cout << "Ocurrio un error en el hilo de los comandos de consola" << endl;
                return -1;
            }

        }*/
    }
    mtx.unlock();
    close(receiverSocket);
    vaciarMemoria();
    return 0;
}

