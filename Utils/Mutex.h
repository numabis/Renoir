#pragma once

#include <windows.h>

namespace BUTIL
{                                
	/**
	 * Clase encapsulado Mutex para protección de recursos compartidos
	 */
	class Mutex
	{
	public:

		Mutex(void);
		Mutex(const wchar_t *);
		~Mutex();
		void lock(void);
		void unlock(void);
//		HANDLE mutex();

	private:
		HANDLE m_mutex;
	};
}
