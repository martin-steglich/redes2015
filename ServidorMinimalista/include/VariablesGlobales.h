#ifndef VARIABLESGLOBALES_H
#define VARIABLESGLOBALES_H

#include <set>
#include <time.h>
#include <string>



using namespace std;

struct Cliente{
    string host;
    unsigned int port;
    string nick;
    unsigned int senderSeq;
    time_t activeTime;
};

inline bool operator<(const Cliente& lhs, const Cliente& rhs)
{
  if(lhs.host.compare(rhs.host) == 0)
    return (lhs.port < rhs.port);

  return lhs.host.compare(rhs.host) < 0;
}

class VariablesGlobales
{
   private:
        static VariablesGlobales* instance;
        VariablesGlobales();
        static int cantConectados;
        static int cantMensajesEnviados;
        static int cantConexionesTotales;
        set<Cliente*> conectados;
        static time_t activeTime;
        unsigned int seqNumber;

    public:
        static VariablesGlobales* getInstance();
        void nuevoUsuario(string host, int port, string nick);
        void nuevoMensaje();
        void nuevaConexion();
        void finConexion(string host, unsigned int port);
        int getCantConectados()const;
        int getCantMensajesEnviados()const;
        int getCantConexionesTotales()const;
        void changeSeqNumber();
        unsigned int getSeqNumber();
        time_t getActiveTime()const;
        set<string> getConectados();
        Cliente* buscarCliente(string nick);
        Cliente* buscarCliente(string host, unsigned int port);
        bool existeCliente(string nick);

        void vaciarMemoria();

};

#endif // VARIABLESGLOBALES_H
