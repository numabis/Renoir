#pragma once

#include <string>

/**
	*	Clase static para gestión de tramas TCP.
	*  Se encarga de la codificación y crc sha256 y descodificación
	*  y comprobación crc sha256.
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
		unsigned char sha256[32];	/**< SHA256 comprobación datos comprimidos */
	};
	#pragma pack(pop)
};
