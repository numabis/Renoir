#include "Timers.h"

namespace BUTIL
{
	/**
	 * Constructor. Inicializa el tiempo a 0
	 */
	Timers::Timers()
	{
		QueryPerformanceFrequency( &frec );
		set(0);
	}

	/**
	 * Constructor.
	 * 
	 * @param  _value  Inicializa el contador al tiempo en ms indicado.   
	 */
	Timers::Timers( int _value )
	{
		QueryPerformanceFrequency( &frec );
		set(_value);
	}

	/**
	 * Carga un temporizador en milisegundos.
	 *      
	 * @param value Tiempo en milisegundos a controlar.
	 *      
	 * @see Timers()
	 */
	void Timers::set( int value )
	{
		QueryPerformanceCounter( &tiempo );
		tiempo.QuadPart += (value*frec.QuadPart/1000);
		unaVez = true;
	}

	/**
	 * Consulta si un temporizador ha caducado.
	 *  
	 * @see Timers()
	 * @see set()
	 *      
	 * @return true si ha caducado el temporizador.
	 */
	bool Timers::query( bool _soloUnaVez )
	{		
		LARGE_INTEGER now;
		QueryPerformanceCounter( &now );
		if( now.QuadPart>tiempo.QuadPart && ( unaVez || !_soloUnaVez ) ) {
			unaVez = false;
			return true;
		}

		return false;
	}

	/**
	 * Pendiente de los segundos que faltan para finalizar.
	 *      
	 * @return Segundos pendientes para finalizar.
	 */
	int Timers::queryEnd( void )
	{
		LARGE_INTEGER now;
		QueryPerformanceCounter( &now );
		if( now.QuadPart>tiempo.QuadPart || !frec.QuadPart )
			return 0;

		return static_cast<int>(( tiempo.QuadPart-now.QuadPart)/frec.QuadPart  );
	}
}
