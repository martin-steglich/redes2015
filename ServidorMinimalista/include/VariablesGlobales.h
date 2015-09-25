#ifndef VARIABLESGLOBALES_H
#define VARIABLESGLOBALES_H


class VariablesGlobales
{
   private:
        static VariablesGlobales* instance;
        VariablesGlobales();
        static int cantConectados;
        static int cantMensajesEnviados;
        static int cantConexionesTotales;


    public:
        static VariablesGlobales* getInstance();
        void nuevoUsuario();
        void nuevoMensaje();
        void nuevaConexion();
        int getCantConectados();
        int getCantMensajesEnviados();
        int getCantConexionesTotales();
        void vaciarMemoria();

};

#endif // VARIABLESGLOBALES_H
