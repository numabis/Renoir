#pragma once

#include <windows.h>
#include "Bloq.h"
#ifdef _SIMULADOR_CLIENTES_
#include <Timers.h>
#endif
#include "dependencias.h"
#include "Mutex.h"

namespace BUTIL
{
	/**
	 * Encapsula comunicación TCP como cliente utilizando la libreria CkSocket.
	 * Esta clase solo se encarga de enviar y recibir datos en formato RAW.
	 */
	class Socket : public BUTIL::Bloq
	{

	public:

		Socket(void);
		~Socket(void);
		
		bool open(std::string, unsigned short, bool, bool, std::string newVal="");
		void close(void);

		bool send(char *, int);
		bool send(std::string);
		
		std::string getIpPort(void);
		std::string getPort(void);
		std::string getIp(void);
		
		static std::string getMacAddress(std::string _mac);



	protected:

		virtual void onConnect(void);
		virtual void onDisconnect(void);
		virtual void onReceived(void);

		void run(void);
			
		bool conexion;

	private:

		CkSocket socket;
		bool netStrings;
		BUTIL::Mutex mx;

		void closeUnlock(void);
		bool sendUnlock(char *, int);

		static bool getAdapterInfo(int , char *);

		bool Socket::setClientIpAddress(const char *newVal);
	};
}