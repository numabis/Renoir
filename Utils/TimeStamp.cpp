#include "timestamp.h"
#include <memory.h>

TimeStamp::TimeStamp(void)
{
	hour = minute = second = 0;
	day = month = year = 0;
	msecond = 0;
}

TimeStamp::TimeStamp(TimeStamp &_fecha)
{
	year = _fecha.year;
	month = _fecha.month;
	day = _fecha.day;
	hour = _fecha.hour;
	minute = _fecha.minute;
	second = _fecha.second;
	msecond = _fecha.msecond;
	hora = _fecha.hora;
	horaExtendida = _fecha.horaExtendida;
}

TimeStamp::TimeStamp(int _year, int _month, int _day, int _hour, int _minute, int _second)
{
	year = _year;
	month = _month;
	day = _day;
	hour = _hour;
	minute = _minute;
	second = _second;
	msecond = 0;
	codificar();
	horaExtendida = hora;
}

TimeStamp TimeStamp::now(void)
{
	TimeStamp nuevo;
	nuevo.hora = time(NULL);
	nuevo.horaExtendida = nuevo.hora;
	nuevo.decodificar();
	return nuevo;
}

void TimeStamp::codificar(void)
{
	struct tm conv;
	memset(&conv, 0, sizeof(tm));
	
	conv.tm_year = year-1900;
	conv.tm_mon = month-1;
	conv.tm_mday = day;
	conv.tm_hour = hour;
	conv.tm_min = minute;
	conv.tm_sec = second;
	conv.tm_isdst = -1;

	hora = mktime(&conv);
}

void TimeStamp::decodificar(void)
{
	struct tm *conv= localtime(&hora);
	year = conv->tm_year+1900;
	month = conv->tm_mon+1;
	day = conv->tm_mday;
	hour = conv->tm_hour;
	minute = conv->tm_min;
	second = conv->tm_sec;
	msecond = 500;
}

int TimeStamp::getYear(void)
{
	return year;
}

int TimeStamp::getMonth(void)
{
	return month;
}

int TimeStamp::getDay(void)
{
	return day;
}

int TimeStamp::getHour(void)
{
	return hour;
}

int TimeStamp::getMinute(void)
{
	return minute;
}

int TimeStamp::getSecond(void)
{
	return second;
}

TimeStamp& TimeStamp::operator = (TimeStamp &_entrada)
{
	year = _entrada.year;
	month = _entrada.month;
	day = _entrada.day;
	hour = _entrada.hour;
	minute = _entrada.minute;
	second = _entrada.second;
	msecond = _entrada.msecond;
	hora = _entrada.hora;
	horaExtendida = _entrada.horaExtendida;

	return *this;
}

void TimeStamp::operator += (TimeStamp &fecha1)
{
	hora += fecha1.hora;
	horaExtendida += fecha1.hora;
	decodificar();
}

void TimeStamp::operator -= (TimeStamp &fecha1)
{
	hora -= fecha1.hora;
	horaExtendida = hora;
//	decodificar();
}

void TimeStamp::addSecond(int _segundos)
{
	hora += _segundos;
	horaExtendida = hora;
//	decodificar();
}

void TimeStamp::subtractSecond(int _segundos)
{
	hora -= _segundos;
	horaExtendida = hora;
//	decodificar();
}

int TimeStamp::getSeconds(void)
{
	return (int)horaExtendida;
}