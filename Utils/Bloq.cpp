#include "Bloq.h"
#include <Windows.h>
#include <dependencias.h>

namespace BUTIL
{
	/**
	 * Constructor.
	 * Inicializa el stream vacio.
	 */
	Bloq::Bloq(void): std::stringstream(  std::ios::in|std::ios::out|std::ios::binary )
	{
		sizeTotal = 0;
	}

	/**
	 * Destructor.
	 * Libera el buffer reservado previamente. 
	 */
	Bloq::~Bloq(void)
	{
	}

	/**
	 * Limpia el stream acumulado hasta el momento.
	 */
	void Bloq::reset( void )
	{
		std::stringstream::str("");
		sizeTotal = 0;
	}

	/**
	 * Devuelve el número de bytes ocupado.
	 *      
	 * @return Longitud en bytes ocupado. 0 si esta vacío.
	 */
	size_t Bloq::size( void )
	{
		return sizeTotal;
	}

	/**
	 * Añade un bloque de datos al sistema stream
	 *      
	 * @param  pt      Puntero buffer a guardar
	 * @param  size    Longitud del buffer a guardar
	 *      
	 * @see reset()
	 * @see size()
	 */
	void Bloq::add( char *pt,size_t size )
	{
		std::stringstream::write( pt, static_cast<std::streamsize>(size) );
		sizeTotal += size;
	}

	/**
	 * Copia una porción del stream y esta es eliminada del sistema
	 * 
	 *      
	 * @param pt Buffer receptor. Tiene que estar reservado previamente.
	 * @param size Longitud que se desea mover o tamaño máximo buffer.
	 *       
	 * @return Retorna la longitud que ha sido movida. Un valor 0 indica
	 * que no hay datos.     
	 *      
	 * @see copy()
	 */
	size_t Bloq::move( char *pt,size_t size )
	{
        if( size<=0 || sizeTotal<=0 )
            return 0;
    
		if( sizeTotal < size )
			size = sizeTotal;
		
		std::stringstream::read( pt, static_cast<std::streamsize>(size) );
		sizeTotal -= size;
		
		if( sizeTotal==0 )
			reset();
		

		return size;
	}

	/**
	 * Copia una porción del stream sin eliminarla del sistema. Se debe
	 * utilizar este mecanismo cuando los datos a los que se quieren acceder
	 * son de longitud variable y están precedidas de una estructura que hace 
	 * la función de cabecera. De esta forma se puede recuperar la cabecera y
	 * comprobar si se puede recuperar el resto, en caso afirmativo es cuando 
	 * se utilizara el move() en vez del copy() para recuperar de nuevo la 
	 * cabecera y los datos.                              
	 *      
	 * @param pt Buffer receptor. Tiene que estar reservado previamente. 
	 * @param size Longitud que se desea copiar o tamaño máximo buffer.
	 *       
	 * @return Retorna la longitud que ha sido copiada. Un valor 0 indica
	 * que no hay datos.     
	 *      
	 * @see move()
	 */
	size_t Bloq::copy( char *pt,size_t size )
	{
        if( size<=0 || sizeTotal<=0 )
            return 0;

		if( sizeTotal < size )
			size = sizeTotal;

		std::streamoff pos = std::stringstream::tellg();
		std::stringstream::read( pt, static_cast<std::streamsize>(size) );
		std::stringstream::seekg( pos, std::ios_base::beg );
		return size;
	}

	/**
	 * Elimina una porción de stream directamente de la memoria.
	 *      
	 * @param size Tamaño a eliminar
	 */
	void Bloq::erase( size_t size )
	{
		if( sizeTotal< size || size<=0 )
			return;

		std::stringstream::seekg( static_cast<long>(size), std::ios_base::cur );
		sizeTotal -= size;

		if( !sizeTotal )
			str("");
	}

	/**
	 *	Recupera un mensaje completo utf8 siempre separado con un NULL.
	 *
	 *	@return "" cadena vacia no hay mensaje.
	 *
	 */
	std::string Bloq::getString()
	{
		std::string ret = "";
		if( size() )
		{
			size_t found = str().find( ':' );
			if( found!=std::string::npos ) //&& found<(pos+sizeTotal) )
			{
				unsigned int len = atol( str().substr(0,found).c_str() );
				found++;
				if( size()>=(found+len) )
				{
					char buf[0xffff];
					uLongf lonBuf = sizeof(buf);
					memset( buf, 0, sizeof(buf) );

					int err = uncompress( (Bytef * )buf, &lonBuf, (Bytef * )str().c_str()+found, len );
					if( err==Z_OK )
						ret = std::string(buf);
				}

				erase( found+len );
			}
		}
		return ret;
	}
}
