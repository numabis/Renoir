#include "Mutex.h"

namespace BUTIL
{
	/**
	* Constructor.
	*     
	* Inicializa el Mutex 
	*/
	Mutex::Mutex(void)
	{
		m_mutex = CreateMutex( NULL, FALSE, NULL );
	}

	/**
	* Constructor.
	*     
	* Inicializa el Mutex con nombre 
	*/
	Mutex::Mutex(const wchar_t * mtxName)
	{
		m_mutex = CreateMutex( NULL, FALSE, mtxName );
	}

	/**
	* Destructor
	*/
	Mutex::~Mutex()
	{
		CloseHandle( m_mutex );
	}

	/**
	* Bloquea el Mutex
	*     
	* @see unlock()
	*/
	void Mutex::lock(void) 
	{
		DWORD d = WaitForSingleObject( m_mutex, INFINITE );
	}

	/**
	* Desbloque al Mutex
	*     
	* @see lock()
	*/
	void Mutex::unlock(void) 
	{
		ReleaseMutex( m_mutex );
	}

//	HANDLE Mutex::mutex() 
//	{
//		return m_mutex	;
//	}
}