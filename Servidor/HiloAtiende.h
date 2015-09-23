/* 
 * File:   HiloAtiende.h
 * Author: bruno.amaral
 *
 * Created on September 22, 2015, 7:58 PM
 */

#ifndef HILOATIENDE_H
#define	HILOATIENDE_H

int HiloAtiendeCliente(int socketRespuestaLogin, struct sockaddr_in direccionCliente, int puertoCorrespondiente);
int HiloRechazaClientes(int socketRespuestaLogin, struct sockaddr_in direccionCliente);

#endif	/* HILOATIENDE_H */

