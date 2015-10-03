#include "DateTime.h"
#include <time.h>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <sstream>
#include <string>



DateTime::DateTime(){
    time_t fecha;
    struct tm * infofecha;

    time (&fecha);
    infofecha = localtime (&fecha);

    anio = infofecha->tm_year + 1900;
    mes = infofecha->tm_mon + 1;
    if (infofecha->tm_mday == 31){
        dia = 1;
        if (mes == 12){
            mes = 1;
            anio++;
        }
        else mes++;
    }
    else dia = infofecha->tm_mday;

    hora = infofecha->tm_hour;
    minutos = infofecha->tm_min;
};


DateTime::DateTime(int anio, int mes,int dia,int hrs,int mins){

     if (((anio >= 0) && (anio <= 9999)) && ((mes >= 1) && (mes <= 12)) && ((dia >= 1) && (dia <= 30)) && ((hrs >= 0) && (hrs <=23)) && ((mins >= 0) && (mins <= 59))){
        this->anio = anio;
        this->mes = mes;
        this->dia = dia;
        this->hora = hrs;
        this->minutos = mins;
    }
    else
        throw std::invalid_argument("Algun parametro invalido.");


};


string DateTime::get_fecha() const{

    stringstream stream;

    string dt;


    if (anio < 10)
        stream << "000" << anio << "/";
    else if (anio < 100)
        stream << "00" << anio << "/";
    else if (anio < 1000)
        stream << "0" << anio << "/";
    else
        stream << anio << "/";

    if (mes < 10)
        stream << "0" << mes << "/";
    else
        stream << mes << "/";

    if (dia < 10)
        stream << "0" << dia << " ";
    else
        stream << dia << " ";

    if (hora < 10)
        stream << "0" << hora << ":";
    else
        stream << hora << ":";

    if (minutos < 10)
        stream << "0" << minutos;
    else
        stream << minutos;

    dt = stream.str();

    return dt;
}

unsigned int DateTime::get_anio() const{

    return anio;
}




unsigned int DateTime::get_mes() const{

    return mes;
}

unsigned int DateTime::get_dia() const{

    return dia;
}

unsigned int DateTime::get_hora() const{

    return hora;
}

unsigned int DateTime::get_minutos() const{

    return minutos;
}


DateTime& DateTime::operator=(const DateTime& dt){

    anio = dt.anio;
    mes = dt.mes;
    dia = dt.dia;
    hora = dt.hora;
    minutos = dt.minutos;

    return *this;
};


bool DateTime::operator==(const DateTime& fecha) const{
    return ((anio == fecha.anio)&&(mes == fecha.mes)&&(dia == fecha.dia)&&(hora == fecha.hora)&&(minutos==fecha.minutos));
};


bool DateTime::operator!=(const DateTime& fecha) const{

	return ((anio != fecha.anio)||(mes != fecha.mes)||(dia != fecha.dia)||(hora != fecha.hora)||(minutos!=fecha.minutos));
};


bool DateTime::operator<(const DateTime& fecha) const{
    return ((anio < fecha.anio)||//el anio es menor o
           ((anio == fecha.anio)&&(mes < fecha.mes))|| //mismo anio, menor mes o
           ((anio == fecha.anio)&&(mes == fecha.mes)&&(dia < fecha.dia))|| //mismo anio y mes, menor dia o
           ((anio == fecha.anio)&&(mes == fecha.mes)&&(dia == fecha.dia)&&(hora < fecha.hora))||//mismo anio, mes y dia, menor hora o
           ((anio == fecha.anio)&&(mes == fecha.mes)&&(dia == fecha.dia)&&(hora == fecha.hora)&&(minutos < fecha.minutos)));//mismo anio,mes,dia y hora, menor minutos

};


bool DateTime::operator>(const DateTime& fecha) const{
     return ((anio > fecha.anio)||//el anio es mayor o
           ((anio == fecha.anio)&&(mes > fecha.mes))|| //mismo anio, mayor mes o
           ((anio == fecha.anio)&&(mes == fecha.mes)&&(dia > fecha.dia))|| //mismo anio y mes, mayor dia o
           ((anio == fecha.anio)&&(mes == fecha.mes)&&(dia == fecha.dia)&&(hora > fecha.hora))||//mismo anio, mes y dia, mayor hora o
           ((anio == fecha.anio)&&(mes == fecha.mes)&&(dia == fecha.dia)&&(hora == fecha.hora)&&(minutos > fecha.minutos)));//mismo anio,mes,dia y hora, mayor minutos
}


bool DateTime::operator<=(const DateTime& fecha) const{
    return ((anio < fecha.anio)||//el anio es menor o
           ((anio == fecha.anio)&&(mes < fecha.mes))|| //mismo anio, menor mes o
           ((anio == fecha.anio)&&(mes == fecha.mes)&&(dia < fecha.dia))|| //mismo anio y mes, menor dia o
           ((anio == fecha.anio)&&(mes == fecha.mes)&&(dia == fecha.dia)&&(hora < fecha.hora))||//mismo anio, mes y dia, menor hora o
           ((anio == fecha.anio)&&(mes == fecha.mes)&&(dia == fecha.dia)&&(hora == fecha.hora)&&(minutos < fecha.minutos))||
           ((anio == fecha.anio)&&(mes == fecha.mes)&&(dia == fecha.dia)&&(hora == fecha.hora)&&(minutos == fecha.minutos)));
}


bool DateTime::operator>=(const DateTime& fecha) const{
	return ((anio > fecha.anio)||//el anio es menor o
           ((anio == fecha.anio)&&(mes > fecha.mes))|| //mismo anio, menor mes o
           ((anio == fecha.anio)&&(mes == fecha.mes)&&(dia > fecha.dia))|| //mismo anio y mes, menor dia o
           ((anio == fecha.anio)&&(mes == fecha.mes)&&(dia == fecha.dia)&&(hora > fecha.hora))||//mismo anio, mes y dia, menor hora o
           ((anio == fecha.anio)&&(mes == fecha.mes)&&(dia == fecha.dia)&&(hora == fecha.hora)&&(minutos > fecha.minutos))||
           ((anio == fecha.anio)&&(mes == fecha.mes)&&(dia == fecha.dia)&&(hora == fecha.hora)&&(minutos == fecha.minutos)));

}


DateTime DateTime::operator+ (double dias) const{

    if (dias >= 0){

    DateTime dt;

    int parteEnteraDias = (int) dias;              //representa la parte entera de los dias dados como parametro
    int aniosASumar  = parteEnteraDias / 360;
    int mesesASumar = (parteEnteraDias % 360)/ 30;
    int diasASumar  = (parteEnteraDias % 360)% 30;
    double parteDecimalDias = dias - parteEnteraDias; //representa la parte decimal de los dias dados como parametro
    double EnHoras = parteDecimalDias * 24;
    int horasASumar = (int) EnHoras;          //representa la cantidad de horas a sumar.
    double parteDecimalHoras = EnHoras - horasASumar; //representa la fraccion de horas a sumar.
    double EnMinutos = parteDecimalHoras * 60;
    int minutosASumar = (int) EnMinutos;

    bool sumarAnio = false;
    bool sumarMes = false;
    bool sumarDia = false;
    bool sumarHora = false;

    dt.minutos = minutos + minutosASumar;
    if (dt.minutos > 59){
        sumarHora = true;
        dt.minutos = dt.minutos - 60; //ajusto los minutos
    }

    dt.hora = hora + horasASumar;
    if (sumarHora)
        dt.hora++;
    if (dt.hora > 23){
        sumarDia = true;
        dt.hora = dt.hora - 24; //ajusto las horas
    }

    dt.dia = dia + diasASumar;
    if (sumarDia)
        dt.dia++;
    if (dt.dia > 30){
        sumarMes = true;
        dt.dia = dt.dia - 30; //ajusto los dias
    }

    dt.mes = mes + mesesASumar;
    if (sumarMes)
        dt.mes++;
    if (dt.mes > 12){
        sumarAnio = true;
        dt.mes = dt.mes - 12; //ajusto los meses
    }



    int anioN = anio + aniosASumar;
    if (sumarAnio)
        anioN++;
    if (anioN <= 9999)
        dt.anio = anioN;
    else
        throw MiException(2);

    return dt;
    }
    else
        throw std::invalid_argument("Los dias a sumar deben ser positivos.");

};


DateTime DateTime::operator- (double dias) const{

    if (dias >= 0){

        //DateTime dt; //donde almaceno resultado

        //En esta parte separo los dias pasados como parametro en a�os, meses, dias, horas y minutos.

        int parteEnteraDias = (int) dias;              //representa la parte entera de los dias dados como parametro
        int aniosARestar  = parteEnteraDias / 360;
        int mesesARestar = (parteEnteraDias % 360)/ 30;
        int diasARestar  = (parteEnteraDias % 360)% 30;
        double parteDecimalDias = dias - parteEnteraDias; //representa la parte decimal de los dias dados como parametro
        double EnHoras = parteDecimalDias * 24;
        int parteEnteraHoras = (int) EnHoras;          //representa la cantidad de horas a restar.
        double parteDecimalHoras = EnHoras - parteEnteraHoras; //representa la fraccion de horas a restar.
        double EnMinutos = parteDecimalHoras * 60;
        int minutosARestar = (int) EnMinutos;


        //variables auxilares
        bool restarAnio  = false;
        bool restarMes  = false;
        bool restarDia  = false;
        bool restarHora = false;

        //resto minutos
        int minutosN = minutos - minutosARestar;
        if (minutosN < 0){
            minutosN = minutosN + 60; //ajusto minutos
            restarHora = true;
        };

        //resto horas
       int horaN = hora - parteEnteraHoras;
        if (restarHora)
            horaN--;
        if (horaN < 0){
            horaN = horaN + 24; //ajusto hora.
            restarDia = true;
        };
        //resto dias
        int diaN = dia - diasARestar;
        if (restarDia)
            diaN--;
        if (diaN < 1){
            diaN = diaN + 30; //ajusto dia
            restarMes = true;
        };
        //resto meses
        int mesN = mes - mesesARestar;
        if (restarMes)
            mesN--;
        if (mesN < 1){
            mesN = mesN + 12; //ajusto mes
            restarAnio = true;
        };
        //resto anio
        int anioN = anio - aniosARestar;
        if (restarAnio)
            anioN--;

		if (anioN < 0){
		throw MiException(1);
		}

        DateTime dt(anioN, mesN, diaN, horaN, minutosN);

            return dt;

    }
    else
       throw std::invalid_argument("Los dias a restar deben ser positivos");



};


double DateTime::operator- (DateTime& fecha2) const{
//Calculamos el valor absoluto de la diferencia.

    double difAnios, difMeses, difDias, difHoras, difMinutos;

    int difAniosI = anio - fecha2.anio;
    difAnios = abs(difAniosI);

    int difMesesI = mes - fecha2.mes;
    difMeses = abs(difMesesI);

    int difDiasI = dia - fecha2.dia;
    difDias = abs(difDiasI);

    int difHorasI = hora - fecha2.hora;
    difHoras = abs(difHorasI);

    int difMinutosI = minutos - fecha2.minutos;
    difMinutos = abs(difMinutosI);


    difAnios = difAnios * 360;
	difMeses = difMeses * 30;
	difMinutos = difMinutos / 60;
	difHoras = (difHoras + difMinutos) / 24;


	return difAnios + difMeses + difDias + difHoras;
}


DateTime operator+ (double dias, DateTime& fecha){

    if (dias >= 0){

        int parteEnteraDias = (int) dias;              //representa la parte entera de los dias dados como parametro
        int aniosASumar  = parteEnteraDias / 360;
        int mesesASumar = (parteEnteraDias % 360)/ 30;
        int diasASumar  = (parteEnteraDias % 360)% 30;
        double parteDecimalDias = dias - parteEnteraDias; //representa la parte decimal de los dias dados como parametro
        double EnHoras = parteDecimalDias * 24;
        int horasASumar = (int) EnHoras;          //representa la cantidad de horas a sumar.
        double parteDecimalHoras = EnHoras - horasASumar; //representa la fraccion de horas a sumar.
        double EnMinutos = parteDecimalHoras * 60;
        int minutosASumar = (int) EnMinutos;

        bool sumarAnio = false;
        bool sumarMes = false;
        bool sumarDia = false;
        bool sumarHora = false;

        int minutosN = fecha.get_minutos() + minutosASumar;
        if (minutosN > 59){
            sumarHora = true;
            minutosN = minutosN - 60; //ajusto los minutos
        }

        int horaN = fecha.get_hora() + horasASumar;
        if (sumarHora)
            horaN++;
        if (horaN > 23){
            sumarDia = true;
            horaN = horaN - 24; //ajusto las horas
        }

        int diaN = fecha.get_dia() + diasASumar;
        if (sumarDia)
            diaN++;
        if (diaN > 30){
            sumarMes = true;
            diaN = diaN - 30; //ajusto los dias
        }

        int mesN = fecha.get_mes() + mesesASumar;
        if (sumarMes)
            mesN++;
        if (mesN > 12){
            sumarAnio = true;
            mesN = mesN - 12; //ajusto los meses
        }

        int anioN = fecha.get_anio() + aniosASumar;
        if (sumarAnio)
            anioN++;
        if (anioN <= 9999){
            DateTime dt(anioN, mesN, diaN, horaN, minutosN);
            return dt;
        }
        else
        throw MiException(2);




    }
    else
        throw std::invalid_argument("Los dias a sumar deben ser positivos.");
};


ostream & operator<< (ostream & salida,DateTime& fecha){

    salida << fecha.get_fecha();

    return salida;

}
