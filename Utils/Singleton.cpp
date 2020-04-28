#include "Singleton.h"

namespace BUTIL
{
	SingletonGlobal singletonGlobal;      /**< Instancia para un destructor automatico */

	/**
	 * Recupera la instancia SIngletonGlobal para no tener que utilizar extern
	 * 
	 * @return Instancia única de SingletonGlobal
	 * 
	 */    
	SingletonGlobal & SingletonGlobal::getInstance( void )
	{
		return singletonGlobal;
	}

	/**
	 * Añade una instancia Singleton a la lista
	 * 
	 * @param   inst    Instancia única 
	 * 
	 */    
	void SingletonGlobal::add( ISingleton *inst )
	{
		lista[inst]=inst;
	}

	/**
	 * Elimina una instancia Singleton de la lista 
	 * 
	 * @param   inst    Instancia única a eliminar 
	 * 
	 */    
	void SingletonGlobal::del( ISingleton *inst )
	{
		std::map<ISingleton *, ISingleton *>::iterator it = lista.find( inst );
		if( it!=lista.end() )
			lista.erase( it );
	}

	/**
	 * Elimina todas las instancias y las destruye con esta llamada manual
	 * por parte del programador, permite controlar la destrucción de los 
	 * objetos.
	 * 
	 */    
	void SingletonGlobal::dispose( void )
	{
		std::map<ISingleton *, ISingleton *>::iterator it = lista.begin();
		while( it != lista.end() )
		{
			it->second->dispose();
			it = lista.begin();
		}

	}

	/**
	 * Destructor para llamar a dispose() en caso de quedar alguna pendiente
	 * de destrucción.
	 */  
	SingletonGlobal::~SingletonGlobal( void )      
	{
		dispose();
	}

	/**
	 * Constructor del Interface, se auto añade a la lista
	 */ 
	ISingleton::ISingleton( void )
	{
		SingletonGlobal::getInstance().add( this );
	}

	/** 
	 * Destructor del Interface, se elimina de la lista
	 */ 
	ISingleton::~ISingleton( void )
	{
		SingletonGlobal::getInstance().del( this );
	}
}
