#pragma once
#include <time.h>

class TimeStamp
{

public:

	TimeStamp(void);
	TimeStamp(int, int, int, int, int, int);
	TimeStamp(TimeStamp&);
	static TimeStamp now(void);
	int getYear(void);
	int getMonth(void);
	int getDay(void);
	int getHour(void);
	int getMinute(void);
	int getSecond(void);
	int getSeconds(void);

	void operator += (TimeStamp &fecha1);
	void operator -= (TimeStamp &fecha1);
	TimeStamp& operator = (TimeStamp &);

	void addSecond(int);
	void subtractSecond(int);

	void decodificar(void);

private:

	int hour;
	int minute;
	int second;
	int day;
	int month;
	int year;
	int msecond;

	time_t hora;
	time_t horaExtendida;

	void codificar(void);
};
