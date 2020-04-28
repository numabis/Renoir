#include "Thread.h"

namespace BUTIL
{
	/// <summary>Constructor. </summary>
	Thread::Thread() 
	{
		m_thread = NULL;
	}

	/// <summary>Destructor. </summary>
	Thread::~Thread()
	{
	}

	/// <summary>Crea y lanza el hilo</summary>
	void Thread::start( void )
	{
		state = 0;
		m_thread = (HANDLE)_beginthreadex( NULL, 0 , this->run, this, 0, &m_dwThreadId );
	}



	/// <summary>Para la ejecución y destruye el hilo</summary>
	void Thread::stop( void )
	{
		if( !m_thread )
			return;

		if( !state )
		{
			state=1;
			WaitForSingleObject( m_thread, INFINITE );
		}
		CloseHandle( m_thread );
		m_thread = NULL;
	}

	/// <summary> Comprueba si se ha iniciado un estado de muerte del hilo
	/// </summary>
	/// <returns> true si esta en estado finalizando operación </returns>
	bool Thread::isFinish( void )
	{
		return state?true:false;

	}

    /**
     *  Función virtual que es llamada después del último onRun() para
     *  que se pueda finalizar variables, etc.
     *  
     *  Destruir los recursos reservados en onInit()
     *  
     *  @see onInit          
     *           
     */               
	void Thread::onFinish( void )
	{
	}

    /**
     *  Función virtual que es llamada antes del primer onRun() para que
     *  se pueda inicializar variables, etc.
     *  
     *  Crear los recurso propios del hilo.          
     *  
     *  @see    onFinish
     *  @see    onRun          
     *          
     */       
	void Thread::onInit( void )
	{
	}

    /**
     *  Función virtual que es ejecutada desde el bucle del hilo
     *  constantemente.
     *  
     *  Es necesario en muchas ocasiones, realizar un Sleep desde este
     *  método para reducir el consumo de la CPU.
     *  
     */               
	void Thread::onRun( void )
	{
	}

	/// <summary>Ayuda a lanzar la clase como hilo. Es de uso INTERNO.</summary>
	/// <param name="lpt">Puntero de la instancia de la clase </param>
	/// <return>Error finalización del hilo</return>
	unsigned int  Thread::run( void *lpt )
	{
		Thread *my = static_cast <Thread *>( lpt );
		
		my->onInit();

		while( !my->state )
		{//			Sleep(1);
			my->onRun();
		}

		my->onFinish();
		_endthreadex( 0 );
		return NULL;
	}
}
