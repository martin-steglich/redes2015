#ifndef CLIENTE_H
#define CLIENTE_H

#include <string>
#include <time.h>

using namespace std;

class Cliente
{
    public:
        Cliente(string host, unsigned int port, string nick, time_t activeTime,  unsigned int senderSeq);

        string getHost()const;
        unsigned int getPort()const;
        string getNick()const;
        unsigned int getSenderSeq();
        void setSenderSeq(int);
        time_t getActiveTime()const;

        virtual ~Cliente();

    protected:

    private:
        string host;
        unsigned int port;
        string nick;
        unsigned int senderSeq;
        time_t activeTime;


};

#endif // CLIENTE_H
