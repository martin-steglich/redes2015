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
    mtx.lock();
    set<string> connected;
    for (set<Cliente*>::iterator it = conectados.begin(); it != conectados.end(); ++it){
        Cliente* actual = *it;
        string nick = actual->nick;

        connected.insert(nick);
    }
    mtx.unlock();
    return connected;
}

Cliente* buscarCliente(string nick){
    mtx.lock();
    for (set<Cliente*>::iterator it = conectados.begin(); it != conectados.end(); ++it){
        Cliente* actual = *it;
        if(actual->nick == nick){
            mtx.unlock();
            return actual;
        }
    }
    mtx.unlock();
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
            mtx.lock();
            cout << "La cantidad de mensajes enviados es: " << cantMensajesEnviados << endl;
            mtx.unlock();
        }else if(command.compare("d") == 0){
            mtx.lock();
            cout << "La cantidad de conexiones totales es: " << cantConexionesTotales << endl;
            mtx.unlock();
        }else if(command.compare("f") == 0){
            time_t serverTime;
            time(&serverTime);
            mtx.lock();
            double seconds = difftime(serverTime, activeTime);
            mtx.unlock();
            printf ("El servidor se encuentra activo hace %.f segundos\n", seconds);
        }
        cout<< endl;
    }

    //pthread_exit(NULL);
}

void changeSeqNumber(){
    mtx.lock();
    if(seqNumber == 1)
        seqNumber = 0;
    else
        seqNumber = 1;
    mtx.unlock();
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
        mtx.lock();
        Cliente* cliente = new Cliente;
        cliente->host = host;
        cliente->port = port;
        cliente->nick = nick;
        time(&cliente->activeTime);
        cliente->senderSeq = 0;
        conectados.insert(cliente);

        //cantConectados++;
        cantConexionesTotales++;
        //cout << "#clientes desde nuevoUsuario() es: " << conectados.size() << endl;
        mtx.unlock();
    }


}

void finConexion(string host, unsigned int port){
    mtx.lock();
    for (set<Cliente*>::iterator it = conectados.begin(); it != conectados.end(); ++it){
        Cliente* actual = *it;
        if((actual->host == host) && (actual->port == port)){
            conectados.erase(it);
        }
    }
    mtx.unlock();


}

void nuevoMensaje(){
    mtx.lock();
    cantMensajesEnviados++;
    mtx.unlock();
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
    mtx.lock();
    if(cliente->senderSeq == 1)
        cliente->senderSeq = 0;
    else
        cliente->senderSeq = 1;

    mtx.unlock();

}

void vaciarMemoria(){
    for (set<Cliente*>::iterator it = conectados.begin(); it != conectados.end(); ++it){
        Cliente* actual = *it;
        delete actual;
    }
    conectados.clear();
}

string getConnectedMessage(Comando* command){
    set<string> users = getNickConectados();

    string message = "<head>";


    message = message + command->getDestHost();
    message = message + "|";
    
    char* serverPortStr = new char();
    sprintf(serverPortStr,"%d",command->getDestPort());
    char* portStr = new char();
    sprintf(portStr,"%d",command->getSourcePort());
    char* serverSeqStr = new char();
    sprintf(serverSeqStr,"%d",seqNumber);

    message = message + serverPortStr;
    message = message + "|";
    message = message + command->getSourceHost();
    message = message + "|";
    message = message + portStr;
    message = message + "|";
    message = message + serverSeqStr;
    message = message + "|0|";
    message = message + serverSeqStr;
    message = message + "</head><data>CONNECTED ";

    for (set<string>::iterator it = users.begin(); it != users.end(); ++it){
        if(it != users.begin())
            message = message + " |";

        string actual = *it;
        message = message + actual;
    }

    message = message + "<CR></data>";

//    strcpy(message, "<head>");


    /*strcat(message, command->getDestHost());
    cout << "command->getDestHost(): " << command->getDestHost() << endl;
    strcat(message, "|");*/

   /* char* serverPortStr = new char();
    sprintf(serverPortStr,"%d",command->getDestPort());
    strcat(message,serverPortStr);
    strcat(message,"|");*/

    /*strcat(message, command->getSourceHost());
    strcat(message, "|");*/

    /*char* portStr = new char();
    sprintf(portStr,"%d",command->getSourcePort());
    strcat(message,portStr);
    strcat(message,"|");*/

    /*char* serverSeqStr = new char();
    sprintf(serverSeqStr,"%d",seqNumber);
    strcat(message,serverSeqStr);
    strcat(message, "|");*/

    /*strcat(message, "0|");

    strcat(message,serverSeqStr);
    strcat(message, "</head><data>CONNECTED ");*/

    /*for (set<string>::iterator it = users.begin(); it != users.end(); ++it){
        if(it != users.begin())
            strcat(message," |");

        string actual = *it;
        strcat(message,actual.c_str());
    }*/
    //strcat(message,"<CR></data>");



    return message;
}

string getRelayedMessage(Comando* command){

    Cliente* cliente = buscarCliente(command->getSourceHost(), command->getSourcePort());


    char* serverPortStr = new char();
    sprintf(serverPortStr,"%d",command->getDestPort());
    char* serverSeqStr = new char();
    sprintf(serverSeqStr,"%d",seqNumber);

    string message = "<head>";
    message = message + command->getDestHost();
    message = message + "|";
    message = message + serverPortStr;
    message = message + "|";
    message = message + "0.0.0.0";
    message = message + "|";
    message = message + "0";
    message = message + "|";
    message = message + serverSeqStr;
    message = message + "|";
    message = message + "0|";
    message = message + serverSeqStr;
    message = message + "</head><data>RELAYED_MESSAGE ";
    message = message + cliente->nick;
    message = message + " ";
    message = message + command->getMensaje();
    message = message + "<CR></data>";



    return message;
}

string getPrivateMessage(Comando* command){

    Cliente* cliente = buscarCliente(command->getSourceHost(), command->getSourcePort());
    Cliente* receptor =buscarCliente(command->getDestinatarioMensajePrivado());

    if (receptor != NULL ) {

        char* serverPortStr = new char();
        sprintf(serverPortStr,"%d",command->getDestPort());
        char* portStr = new char();
        sprintf(portStr,"%d",receptor->port);
        char* serverSeqStr = new char();
        sprintf(serverSeqStr,"%d",seqNumber);


        string message = "<head>";
        message = message + command->getDestHost();
        message = message + "|";
        message = message + serverPortStr;
        message = message + "|";
        message = message + receptor->host;
        message = message + "|";
        message = message + portStr;
        message = message + "|";
        message = message + serverSeqStr;
        message = message + "|";
        message = message + "0|";
        message = message + serverSeqStr;
        message = message + "</head><data>PRIVATE_MESSAGE ";
        message = message + cliente->nick;
        message = message + " ";
        message = message + command->getMensaje();
        message = message + "<CR></data>";
        return message;
    }

    return NULL;

}

string getLogoutMessage(Comando* command){

    char* serverPortStr = new char();
    sprintf(serverPortStr,"%d",command->getDestPort());
    char* portStr = new char();
    sprintf(portStr,"%d",command->getSourcePort());
    char* serverSeqStr = new char();
    sprintf(serverSeqStr,"%d",seqNumber);

    string message = "<head>";
    message = message + command->getDestHost();
    message = message + "|";
    message = message + serverPortStr;
    message = message + "|";
    message = message + command->getSourceHost();
    message = message + "|";
    message = message + portStr;
    message = message + "|";
    message = message + serverSeqStr;
    message = message + "|";
    message = message + "0|";
    message = message + serverSeqStr;
    message = message + "</head><data>GOODBYE";
    message = message + "<CR></data>";

    return message;
}

void sendACK(Comando* command, struct sockaddr_in clienteDireccion){

    char* serverPortStr = new char();
    sprintf(serverPortStr,"%d",command->getDestPort());
    char* portStr = new char();
    sprintf(portStr,"%d",command->getSourcePort());
    char* serverSeqStr = new char();
    sprintf(serverSeqStr,"%d",seqNumber);
    char* seqStr = new char();
    sprintf(seqStr,"%d",command->getNumSeq());

    string message = "<head>";
    message = message + command->getDestHost();
    message = message + "|";
    message = message + serverPortStr;
    message = message + "|";
    message = message + command->getSourceHost();
    message = message + "|";
    message = message + portStr;
    message = message + "|";
    message = message + seqStr;
    message = message + "|";
    message = message + "1|";
    message = message + serverSeqStr;
    message = message + "</head><data></data>";


    int socketAck = socket(PF_INET,SOCK_DGRAM, 0);
    if(socketAck == -1)
      cout << "No puedo inicializar el socket para enviar el ack" << endl;

    const char * msg = message.c_str();
    sendto(socketAck, msg , strlen(msg)+1, 0 , (struct sockaddr*)&clienteDireccion , sizeof(clienteDireccion));
}

void* sendMessage(void* arg){

    string message = "";
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
    if (!message.empty()){
        const char * msg = message.c_str();
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
                sendto(senderSocket, msg , strlen(msg)+1, 0 , (struct sockaddr*)&multicast , sizeof(multicast));
                set<string> conectados = getNickConectados();
                int attempts = 0;

                while((!conectados.empty() && (attempts < 4))){
                    struct timeval tv;
                    tv.tv_sec = 0;
                    tv.tv_usec = 900000;
                    if (setsockopt(receiverSocket, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
                        perror("Error");
                        break;
                    }

                    char buffer[BUFFERSIZE];
                    memset (buffer,0,BUFFERSIZE);
                    if(recvfrom(receiverSocket, buffer ,BUFFERSIZE, 0 , (struct sockaddr*)&ackReceiver, &len) >= 0){
                        Comando* ackCommand = comandoParsear(buffer);
                        if((ackCommand->getEsAck()) && (ackCommand->getNumSeq() == seqNumber)){
                            Cliente* cli = buscarCliente(ackCommand->getSourceHost(), ackCommand->getSourcePort());
                            if (cli != NULL)
                                conectados.erase(cli->nick);
                        }

                    }else{
                        sendto(senderSocket, msg , strlen(msg)+1, 0 , (struct sockaddr*)&multicast , sizeof(multicast));

                        attempts++;
                        if (attempts < 4 )
                            conectados = getNickConectados();

                    }

                }

                if(attempts == 4){
                    for (set<string>::iterator it = conectados.begin(); it != conectados.end(); ++it){
                        string actual = *it;
                        Cliente* noAck = buscarCliente(actual);
    		    if (noAck != NULL)
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
                        if (loggedOut != NULL)
                            finConexion(loggedOut->host,loggedOut->port);

                    }
                }


            }else{
                cout << "No puede hacer bind" << endl;
            }


        }else{
            cout << "No puedo inicializar el socket" << endl;
        }


        close(senderSocket);
        close(receiverSocket);

    }

}

bool isSalir(){
    mtx.lock();
    bool exit = salir;
    mtx.unlock();
    return exit;
}


int main(){

    cantMensajesEnviados = 0;
    cantConexionesTotales = 0;
    seqNumber = 0;
    activeTime = time(&activeTime);
    salir = false;
    mtx = Mutex();

    cout << "Servidor de Chat - Redes 2015" << endl;


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

    pthread_t consoleCommandThread;
    pthread_create(&consoleCommandThread, NULL, &comandosConsola, NULL);

    while(!isSalir()){
        memset (buffer,0,BUFFERSIZE); //vacio buffer

        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 200000;

        if (setsockopt(receiverSocket, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
            cout << "No se puede setear el timeout" << endl;
            return -1;
        }

        if(recvfrom(receiverSocket, buffer ,BUFFERSIZE, 0 , (struct sockaddr*)&senderAddress, &len) >= 0){

            command = comandoParsear(buffer);
            if(command->getTipo() == MESSAGE){
                cout << "Mensaje recibido: MESSAGE "<< command->getMensaje() << endl;

            }else if(command->getTipo() == PRIVATE_MESSAGE){
                cout << "Mensaje recibido: PRIVATE_MESSAGE "<< command->getMensaje() << endl;
            }else if(command->getTipo() == LOGIN){
                cout << "Mensaje recibido: LOGIN "<< command->getusuario() << endl;
            }else if(command->getTipo() == LOGOUT){
                cout << "Mensaje recibido: LOGOUT" << endl;
            }else if(command->getTipo() == GET_CONNECTED){
                cout << "Mensaje recibido: GET_CONNECTED"<< endl;
            }
            cout << "IP origen: " << command->getSourceHost() << endl;
            if ( existeCliente(command->getSourceHost(), command->getSourcePort()) || ( command->getTipo() == LOGIN && !existeCliente(command->getSourceHost(), command->getSourcePort()) && !existeCliente(command->getusuario()))){


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
                        if(existeCliente(command->getSourceHost(),command->getSourcePort()))
                            numeroSecuenciaCliente(command->getSourceHost(),command->getSourcePort());

                       }
                      changeSeqNumber();


                    }

                }

            }
            if(command != NULL){
                //command->~Comando();
                delete command;
            }
        }


    }

    close(receiverSocket);
    vaciarMemoria();
    return 0;
}
