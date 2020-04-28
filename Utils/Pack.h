#pragma once

#include <string>

/**
	*	Clase static para gesti�n de tramas TCP.
	*  Se encarga de la codificaci�n y crc sha256 y descodificaci�n
	*  y comprobaci�n crc sha256.
	*/
class Pack
{
public:

	static bool pack( std::string, char *, int & );
	static bool unpack( char *, int, std::string & );

	/** Estructura cabecera mensajes XML */
	#pragma pack(push,1)
	typedef struct HEAD_XML
	{
		unsigned short size;		/**< Longitud datos comprimidos */
		unsigned char sha256[32];	/**< SHA256 comprobaci�n datos comprimidos */
	};
	#pragma pack(pop)
};
