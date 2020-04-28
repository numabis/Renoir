#include "Pack.h"
//#include <zlib.h>
#include <sha256.h>
#include <dependencias.h>

/// ---------------------------------------------------------------------------------------------
/// <summary> Realiza la compresión de un mensaje XML, le añade la cabecera y le pasa un 
/// CRC SHA256 a la trama comprimida</summary>
/// <param name="in"> Cadena XML a comprimir </param>
/// <param name="buf"> Buffer donde se deja la trama comprimida </param>
/// <param name="size"> Longitud usada de Buffer. Pasar primero con la longitud de buffer </param>
/// <returns> true si se ha comprimido correctamente </returns>
/// ---------------------------------------------------------------------------------------------
bool Pack::pack( std::string in, char *buf, int &size )
{
	BUTIL::SHA256 sha256;
	uLong calcDest;			// Variable para ZLIB

	HEAD_XML *head = (HEAD_XML *)buf;			// CABECERA XML -> BUF
	char *dat = buf+sizeof(HEAD_XML);			// DATOS A PARTIR DE LA CABECERA

	if( !buf || size<sizeof(HEAD_XML) )
		return false;

	calcDest = size-sizeof(HEAD_XML);			// Calculamos el máximo reservado...

	int ret = compress( (Bytef *)dat, &calcDest, (Bytef *)in.c_str(), (uLong)in.length()+1 );

	if( ret!=Z_OK )
		return false;

	head->size = (unsigned short)calcDest;

	sha256.update( (BYTE *)dat, head->size );

	sha256.finish( head->sha256 );
	size = head->size + sizeof(HEAD_XML);
	return true;
}

/// ---------------------------------------------------------------------------------------------
/// <summary> Realiza la descompresión de un mensaje XML, analiza la cabecera y el CRC SHA256
/// a la trama comprimida antes</summary>
/// <param name="buf"> Buffer donde esta la trama comprimida </param>
/// <param name="size"> Longitud usada de Buffer </param>
/// <param name="msg"> Cadena XML resultante </param>
/// <returns> true si se ha descomprimido correctamente </returns>
/// ---------------------------------------------------------------------------------------------
bool Pack::unpack( char *buf, int size, std::string &msg )
{
	char dest[524288];
	uLong destSize = sizeof(dest);
	BUTIL::SHA256 sha256;	// Firma SHA256
	BYTE firma[32];

	if( size< sizeof(HEAD_XML ) )
		return false;

	HEAD_XML *head = (HEAD_XML *)buf;			// CABECERA XML -> BUF
	char *dat = buf+sizeof(HEAD_XML);			// DATOS A PARTIR DE LA CABECERA

	msg = "";

	// Ciframos buffer
	sha256.update( (BYTE *)dat, head->size );
	sha256.finish( firma );

	if( memcmp( firma, head->sha256, sizeof(firma) ) )	
		return false;										// Mal la firma SHA256

	ZeroMemory( dest, sizeof(dest) );

	int ret = uncompress( (Bytef *)dest, &destSize, (Bytef *)dat, (uLong) head->size );
	if( ret!=Z_OK )
		return false;										// Mal la descompresión

	msg = std::string( dest );
	return true;
}
