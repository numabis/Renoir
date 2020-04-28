#pragma once

#include <windows.h>
#include <process.h>

namespace BUTIL
{
	/**
	 * Clase gestión Thread 
	 */ 
	class Thread
	{
	public:
		Thread( );									// Constructor de la clase. No hace nada
		virtual ~Thread();							// Destructor de la clase. No hace nada
		void start (void );							// Lanza la ejecución y llama onInit().
		void stop( void );							// Para la ejecución y llama onFinish(). Se puede volver lanzar start() 
													// pero se llamara de nuevo onInit()
	protected:
		virtual void onInit( void );				// Función virtual cuando se arranca el hilo
		virtual void onFinish( void );				// Funcion virtual cuando se para el hilo
		virtual void onRun( void );					// Función virtual que es llamada cada loop
		bool isFinish( void );

	private:

		static unsigned int __stdcall run( void *);
		int state;									// Estado del hilo. 0=RUN, 1=PARANDO
		
		HANDLE m_thread;
		unsigned m_dwThreadId;
	};
}



