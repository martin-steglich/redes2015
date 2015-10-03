#ifndef DateTime_HH
#define DateTime_HH

#include <iostream>
#include <exception>

using namespace std;



class DateTime {

    private:
        unsigned int anio;
        unsigned int dia;
        unsigned int mes;
        unsigned int hora;
        unsigned int minutos;
    public:
        DateTime();
        DateTime(int anio, int mes, int dia, int horas=0, int minutos=0);
        string get_fecha() const;
        unsigned int get_anio() const;
        unsigned int get_mes() const;
        unsigned int get_dia() const;
        unsigned int get_hora() const;
        unsigned int get_minutos() const;
        DateTime& operator=(const DateTime&);
        double operator- (DateTime& fecha2) const;

};

#endif // DATETIME_H
