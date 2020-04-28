#pragma once

#include <windows.h>

namespace BUTIL
{
	/**
	 * Clase que encapsula sistema temporizador de 64bits.
	 */
	class Timers
	{
	public:
		Timers();
		Timers( int  );
		void set( int  );
		virtual bool query( bool=true );
		int queryEnd( void );


	private:
		LARGE_INTEGER frec;					/**< Pulso ciclos 1 segundo CPU */
		LARGE_INTEGER tiempo;				/**< Control tiempo */
		bool unaVez;								/**< Indica que ya ha saltado el final de un reloj */
	};

}

