#include "Comm.h"

namespace BUTIL
{
	/**
	 * Constructor. Solamente inicializa las variables del sistema.
	 * Una clase puede heredar directamente de Comm.     
	 *      
	 */
	Comm::Comm(void)
	{
		hCom = NULL;
	}

	/**
	 * Destructor.
	 * Cierra y libera el dispositivo serie si no se ha realizado manualmente.
	 *      
	 * @see close()
	 */
	Comm::~Comm(void)
	{
		close();
	}

	/**
	 * Cierra el dispositivo serie.
	 *      
	 * @see open()
	 */
	void Comm::close( void )
	{
		if( hCom )
		{
			CloseHandle( hCom );
			hCom = NULL;
		}
	}

	/**
	 * Abre el dispositivo serie indicado
	 *      
	 * @param port Puerto serie, "COM1", "COM2" ...
	 * @param bps Velocidad 
	 * @param bit Bit de datos 7 o 8
	 * @param bitStop Bit de stop
	 * @param paridad Paridad
	 *      
	 * @return true si se ha reservado correctamente el dispositivo
	 */
	bool Comm::open(LPCWSTR port, DWORD bps, BYTE bit, BYTE bitStop, BYTE paridad )
	{
		hCom = CreateFile( port, 
							GENERIC_READ|GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							0,
							NULL );

		if( hCom==INVALID_HANDLE_VALUE )
		{
			hCom=NULL;
			return false;
		}

		if( !GetCommState( hCom, &dcb ) )
		{
			close();
			return false;
		}

		dcb.BaudRate = bps;
		dcb.ByteSize = bit;
		dcb.Parity = paridad;
		dcb.StopBits = bitStop;
		dcb.fRtsControl = RTS_CONTROL_DISABLE;
		dcb.fDtrControl = DTR_CONTROL_DISABLE;

		if( !SetCommState( hCom, &dcb ) )
		{
			close();
			return false;
		}

		return true;
	}

	/**
	 * Cambia el estado de DTR (DATA TERMINAL READY)
	 *      
	 * @param status true para activar
	 */
	void Comm::dtr( bool status )
	{
		if( GetCommState( hCom, &dcb ) )
		{
			dcb.fDtrControl = status?DTR_CONTROL_ENABLE:DTR_CONTROL_DISABLE;
			SetCommState( hCom, &dcb );
		}
	}

	/**
	 * Cambia el estado de RTS (REQUEST TO SEND)
	 *      
	 * @param status true para activar
	 */
	void Comm::rts( bool status )
	{
		if( GetCommState( hCom, &dcb ) )
		{
			dcb.fRtsControl = status?RTS_CONTROL_ENABLE:RTS_CONTROL_DISABLE;
			SetCommState( hCom, &dcb );
		}
	}


	/** 
	 * Comprueba si hay lectura pendiente.
	 *      
	 * return número de bytes en memoria pendientes de leer o 0 si no
	 * hay lecturas.     
	 */
	size_t Comm::queryRecv( void )
	{
		DWORD nBytes = 0;
		_COMSTAT cs;
		memset(&cs, 0, sizeof(_COMSTAT));

		if( !hCom )
			return 0;

		ClearCommError( hCom, &nBytes, &cs );
		return cs.cbInQue;
	}

	/**
	 * Lectura del buffer.
	 *      
	 * @param buffer buffer donde depositar datos
	 * @param size longitud del buffer
	 * @see queryRecv()
	 *      
	 * @return longitud leída. 0 si no hay datos.
	 */
	size_t Comm::recv( BYTE *buffer, size_t size )
	{
		DWORD nBytes = static_cast<DWORD>(queryRecv());
		if( !nBytes )
			return 0;

		if( nBytes>size )
			nBytes = static_cast<DWORD>(size);

		if( ReadFile( hCom, buffer, nBytes, &nBytes, NULL ) )
			return nBytes;

		return 0;
	}

	/**
	 * Envío de datos
	 *      
	 * @param buffer Puntero buffer a enviar
	 * @param size longitud del buffer a enviar
	 *      
	 * @return true si es correcto el envío. No garantizada la entrega.
	 */
	bool Comm::send( BYTE *buffer, size_t size )
	{
		DWORD nBytes = 0;

		if( !hCom )
			return false;

		if( WriteFile( hCom, buffer, static_cast<DWORD>(size), &nBytes, NULL ) )
			return true;

		return false;

	}

	/**
	 * Envío de una cadena de datos, no se envía el NULL final de la cadena.
	 *      
	 * @pt Buffer a enviar
	 *      
	 * @return true si es correcto el envío. No garantizada la entrega.
	 */
	bool Comm::send( char *pt )
	{
	    if( pt==NULL )
	    	return false;
	    	
		DWORD nBytes = strlen(pt);
		if( nBytes<1 )
			return false;
			
		return send( (BYTE *)pt, nBytes );
	}

	/**
	 * Vacía el buffer de recepción. 
	 */
	void Comm::reset( void )
	{
		BYTE buf[16];
		size_t len = 0;

		while( true )
		{
			len = queryRecv();
			if( !len )
				break;

			if( len>sizeof(buf) )
				len = sizeof(buf);

			recv( buf, len );
		}
	}

}
