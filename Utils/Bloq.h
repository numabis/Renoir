#pragma once

#include <sstream>

namespace BUTIL
{
	/**
	 * Encapsula un sistema de stream básico para buffer de entrada
	 * de dispositivos series, tcp, etc.     
	 */
	class Bloq: public std::stringstream 
	{
	public:
		Bloq(void);
		~Bloq(void);

		void add( char *,size_t );
		size_t size( void );
		void reset( void );
		size_t move( char *,size_t );
		size_t copy( char *,size_t );
		void erase( size_t );

		std::string getString();

	private:
		size_t sizeTotal;                 /**< Longitud ocupada */
	};
}
