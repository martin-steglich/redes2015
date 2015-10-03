#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED




using namespace std;

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


    public:
        static VariablesGlobales* getInstance();
        void nuevoUsuario();
        void nuevoMensaje();
        void nuevaConexion();
        int getCantConectados()const;
        int getCantMensajesEnviados()const;
        int getCantConexionesTotales()const;
        time_t getActiveTime()const;
        set<string> getConectados();
        Cliente* buscarCliente(string nick);

        void vaciarMemoria();

};



#endif // PARSER_H_INCLUDED
