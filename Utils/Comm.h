#pragma once
#include <windows.h>
#include <string>

namespace BUTIL
{
	/**
	 * Clase que encapsula la comunicación SERIE de un dispositivo
	 */
	class Comm
	{
	public:

		Comm(void);
		~Comm(void);

		bool open( LPCWSTR port, DWORD bps, BYTE bit, BYTE bitStop, BYTE paridad );
		void close( void );

		void dtr( bool );
		void rts( bool );


		size_t recv( BYTE *buffer, size_t size );	
		
		bool send( BYTE *buffer, size_t size );
		bool send( char * );

		size_t queryRecv( void );

		void reset( void );

        /** Enumerador velocidades soportadas */
		enum 
		{
			COMM_300	= CBR_300,		/**< 300 bps */
			COMM_2400   = CBR_2400,		/**< 2400 bps */
			COMM_9600	= CBR_9600,		/**< 9600 bps */ 
			COMM_19200  = CBR_19200,	/**< 19200 bps */
		};

        /** Enumerador longitud datos soportados */
		enum
		{
			COMM_BIT7	= 7,			/**< 7 bit datos */
			COMM_BIT8	= 8,			/**< 8 bit datos */
		};

        /** Enumerador longitud del bit de stop */
		enum
		{
			COMM_STOP1 = ONESTOPBIT,	/**< 1 bit stop */
			COMM_STOP2 = TWOSTOPBITS,	/**< 2 bit stop */
		};

        /** Enumerador del bit de paridad */
		enum
		{
			COMM_NOPARITY = NOPARITY,	/**< Sin paridad */
			COMM_EVEN = EVENPARITY,		/**< Paridad PAR */
			COMM_ODD = ODDPARITY,		/**< Paridad IMPAR */
		};


	protected:

	private:
			HANDLE hCom;                 /**< Handle sistema */
			DCB dcb;                     /**< Estructura parámetros */

	};
}
