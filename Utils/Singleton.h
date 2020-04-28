#pragma once

#include <map>

namespace BUTIL
{                                    
	/** Interface clases Singleton. USO INTERNO. */
	class ISingleton
	{
	public:
		ISingleton( void );
		~ISingleton( void );
		virtual void dispose( void )=0;
	};

	/** 
	 *    Clase para gestión de Singleton automáticos 
	 *    Para su utilización solo es necesario que la clase herede de
	 *    Singleton, de la siguiente forma:
	 *    
	 *     \code     
	 *     class Ejemplo: public Singleton<Ejemplo>
	 *     {
	 *     public:
	 *         Ejemplo();
	 *         ~Ejemplo();                   
	 *     }
	 *     \endcode     
	 *
	 *    Si fuera necesario que el constructor Ejemplo() y destructor
	 *    ~Ejemplo() fueran privados o protegidos, hay que añadir la 
	 *    siguiente linea: friend class Singleton<Ejemplo>;                                       

	 *     \code     
	 *     class Ejemplo: public Singleton<Ejemplo>
	 *     {
	 *     
	 *     protected:
	 *         friend class Singleton<Ejemplo>;  
	 *         Ejemplo();
	 *         ~Ejemplo();                   
	 *     }
	 *     \endcode     
	 *                    
     */
	template <typename T> class Singleton: public ISingleton
	{
	public:
		
		/** 
		 * Recupera la instancia de la clase
		 * 
		 * @return Instancia única de la clase
		 *
		 *    \code
		 *    BUTIL::Log::getInstance().log( "ejemplo" );    
		 *    \endcode         
		 *                                             
		 */                        
		static T & getInstance()
		{
			static T *inst = NULL;
			if( !inst )
				inst = new T();

			return *inst;
		}

		/**
		 * Fuerza la destrucción de la clase en el momento deseado, para poder
		 * ordenar la destrucción de clases cuando es necesario un orden
		 * especifico, normalmente por dependencias.         
		 * 
		 *    \code
		 *    COMUNICACION::Tcp::getInstance().dispose();      // Destruir RED
		 *    BaseDatos::getInstance().dispose(); // Finalizar base de datos.                  
		 *    \endcode                           
		 * 
		 */                   
		void dispose( void )
		{
			delete (&getInstance());
		}
	};

	/**
	 * Clase gestión global. USO INTERNO. 
	 */ 
	class SingletonGlobal
	{
	public:
		void add( ISingleton * );
		void del( ISingleton * );
		static SingletonGlobal &getInstance( void );
		void dispose( void );
		~SingletonGlobal( void );
	private:
		std::map< ISingleton *, ISingleton * >lista;   /**< Lista objetos */
	};
}
