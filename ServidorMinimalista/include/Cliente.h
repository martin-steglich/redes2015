#ifndef CLIENTE_H
#define CLIENTE_H

#include <string>
#include <time.h>

using namespace std;

class Cliente
{
    public:
        Cliente(string host, unsigned int port, string nick, time_t activeTime);

        string getHost()const;
        unsigned int getPort()const;
        string getNick()const;
        time_t getActiveTime()const;

        virtual ~Cliente();
    protected:
    private:
        string host;
        unsigned int port;
        string nick;
        time_t activeTime;


};

#endif // CLIENTE_H
