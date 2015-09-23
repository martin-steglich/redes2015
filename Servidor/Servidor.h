/* 
 * File:   Servidor.h
 * Author: bruno.amaral
 *
 * Created on September 22, 2015, 8:10 PM
 */

#ifndef SERVIDOR_H
#define	SERVIDOR_H

#ifdef	__cplusplus
extern "C" {
#endif

//Longitud del buffer 
#define BUFFERSIZE 512 // poner el de los datagramas 
    
    
class Servidor {

private:
    
    int socketServidorAtiendeLogin;
    int socketRespuestaLogin;
    struct sockaddr_in clienteDireccion;
    struct sockaddr_in myDireccion;
    fd_set socketsActuales;        // Conjunto de descriptores a controlar
    int contadorHijos;
    
public:
    
    Servidor();
    int getSocketServidorAtiendeLogin();
    int getSocketRespuestaLogin();
    struct sockaddr_in getClienteDireccion();
    struct sockaddr_in getMyDireccion();
    fd_set getSocketsActuales();        
    int getContadorHijos();
    virtual ~Servidor();    
    
};


#ifdef	__cplusplus
}
#endif

#endif	/* SERVIDOR_H */

