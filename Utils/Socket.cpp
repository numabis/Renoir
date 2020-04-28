#include "Socket.h"
#include "Util.h"

namespace BUTIL
{
	/**
	 * Constructor clase
	 * @param _host Ip host tcp
	 * @param _port Puerto conexión tcp
	 */
	Socket::Socket(void)
	{
		conexion = false; 
		netStrings = false;

		bool status = socket.UnlockComponent("TECNCOSocket_O8fezZmGKO3n");
	}

	/**
	 * Destructor clase.
	 */
	Socket::~Socket(void)
	{
		close();
		CkSettings::cleanupMemory();
	}

	/**
	 * Cierra el socket.
	 */
	void Socket::close(void)
	{
		mx.lock();
		closeUnlock();
		mx.unlock();
	}

	void Socket::closeUnlock(void)
	{
		if (conexion)
		{
			socket.AsyncReceiveAbort();
			while (socket.get_AsyncReceiveFinished() == false)
				socket.SleepMs(1);
				
			socket.Close(1000);

			conexion = false;

			onDisconnect();
		}
	}

	/**
	 * Apertura de la conexión TCP.
	 *
	 * @param	host	Ip servidor
	 * @param	port	Puerto servidor
	 *
	 */
	bool Socket::open(std::string host, unsigned short port, bool ssl, bool _netStrings, std::string newVal)
	{
		mx.lock();
		netStrings = _netStrings;
		//char error[1000];
		//socket.put_VerboseLogging(true);
		if (!newVal.empty())
			setClientIpAddress(newVal.c_str());

		if (socket.Connect(host.c_str(), port, ssl, 10000) == true)
		{
			socket.put_MaxSendIdleMs(2000);
			if (socket.AsyncReceiveBytes() == true)
			{
				conexion = true;
				Bloq::reset();
				onConnect();
				mx.unlock();
				return true;
			}
			else
			{
				closeUnlock();
				mx.unlock();
				return false;
			}
		} else 
		{
			char error[5000];
			;
			sprintf (error,"%d - %s", socket.get_ConnectFailReason(), socket.lastErrorText()) ;
			sprintf (error,"%d - %s", socket.get_ConnectFailReason(), socket.lastErrorText()) ;
		}
		//if (mx.mutex() != NULL)
			mx.unlock();
		return false;
	}

	/**
	 * Método que tiene que ser llamado constantemente para no implementar un hilo individual.
	 * Realiza reconexión cada 5 segundos si la conexión se cae automáticamente.
	 * El sistema no es bloqueante al utilizar CkSocket
	 */
	void Socket::run(void)
	{
		if (!conexion)
			return;

		mx.lock();

		if (!socket.get_AsyncReceiveFinished())
		{
			mx.unlock();
			return;
		}

		if (socket.get_AsyncReceiveSuccess() == false)		// Error
		{
			closeUnlock();
			mx.unlock();
			return;
		}

		CkByteData buf;
		socket.get_AsyncReceivedBytes(buf);

		if (socket.AsyncReceiveBytes() == false || !buf.getSize())
		{
			closeUnlock();
			mx.unlock();
			return;
		}
			
		size_t len = buf.getSize();
		char *pt = (char *)buf.getData();
		Bloq::add(pt, len);

		onReceived();
		mx.unlock();
	}
			

	/**
	 * Método virtual que es llamado cada vez que se conecta al host
	 * @see onDisconnect()
	 */
	void Socket::onConnect(void)
	{

	}

	/**
	 * Método virtual que es llamado cada vez que se desconecta del host
	 * @see onConnect()
	 */
	void Socket::onDisconnect(void)
	{
	}

	/**
	 * Método virtual que es llamado cada vez que se recibe tramas tcp del host.
	 * Hay que utilizar los métodos BLoq:: para acceder a la información recibida.
	 */
	void Socket::onReceived(void)
	{
	}

	/**
	 * Envía datos al host. 
	 * @param _buf Buffer a enviar
	 * @param _size longitud a enviar
	 * @return true si el envío es correcto.
	 */
	bool Socket::send(char *_buf, int _size)
	{
		if (!conexion)
			return false;

		mx.lock();

        if (!socket.CheckWriteable(2000))
		{
			closeUnlock();
			mx.unlock();
            return false;
		}
            
		if (socket.SendBytes(_buf, (unsigned long) _size) == true)
		{
			mx.unlock();
			return true;
		}
		else
		{
			closeUnlock();
			mx.unlock();
			return false;
		}
	}

	/**
	 * Envía datos al host. 
	 * @param _buf Buffer a enviar
	 * @param _size longitud a enviar
	 * @return true si el envío es correcto.
	 */
	bool Socket::sendUnlock(char *_buf, int _size)
	{
		if (!conexion)
			return false;

        if (!socket.CheckWriteable(2000))
		{
			closeUnlock();
            return false;
		}
            
		if (socket.SendBytes(_buf, (unsigned long) _size) == true)
		{
			return true;
		}
		else
		{
			closeUnlock();
			return false;
		}
	}

	bool Socket::send(std::string _msg)
	{
		if (!conexion)
			return false;

		bool status = true;
		mx.lock();

		if (!netStrings)
		{
			status = sendUnlock((char *)_msg.c_str(), static_cast<int>(_msg.length()+1));	// Quiero enviar el NULL FINAL siempre...
			mx.unlock();
			return status;
		}
		else
		{
			Bytef buf[0xffff];
			char buf2[0xffff];
			uLongf lonBuf = sizeof(buf);
			int ret = compress(buf, &lonBuf, (Bytef *)_msg.c_str(), static_cast<uLong>(_msg.length()));
			if (ret==Z_OK)
			{
				sprintf_s(buf2, "%u:", lonBuf);
				size_t lenTmp = strlen(buf2);
				memcpy(buf2+lenTmp, buf, lonBuf);

				status = sendUnlock(buf2, static_cast<int>(lonBuf+lenTmp));
			}
			else
			{
				closeUnlock();
				status = false;
			}
		}

		mx.unlock();
		return status;
	}

	std::string Socket::getIpPort(void)
	{
		char buf[256];
		
		mx.lock();
		sprintf_s(buf, sizeof(buf), "%s:%d", socket.localIpAddress(), socket.get_LocalPort());
		mx.unlock();
		return std::string(buf);	// socket->remoteIpAddress();
	}
	
	std::string Socket::getPort(void)
	{
		char buf[256];
		
		mx.lock();
		sprintf_s(buf, sizeof(buf), "%s", socket.get_LocalPort());
		mx.unlock();
		return std::string(buf);	// socket->remoteIpAddress();
	}
	
	std::string Socket::getIp(void)
	{
		char buf[256];
		
		mx.lock();
		sprintf_s(buf, sizeof(buf), "%s", socket.localIpAddress());
		mx.unlock();
		return std::string(buf);	// socket->remoteIpAddress();
	}
	
	/**
	 * Recupera la MAC ADDRESS del primer adaptador de red encontrado
	 *          
	 * @return string formateado con la MAC ADDRESS
	 */
	std::string Socket::getMacAddress(std::string _mac)
	{
		// Get adapter list
		LANA_ENUM AdapterList;
		NCB Ncb;
		memset(&Ncb, 0, sizeof(NCB));
		Ncb.ncb_command = NCBENUM;
		Ncb.ncb_buffer = (unsigned char *)&AdapterList;
		Ncb.ncb_length = sizeof(AdapterList);
		Netbios(&Ncb);

		// Get all of the local ethernet addresses
		char mac_addr[6];
		for (int i=0; i<AdapterList.length; i++)
		{
			if (getAdapterInfo(AdapterList.lana[i], mac_addr))
			{
				std::string mac = BUTIL::Util::format("%02X-%02X-%02X-%02X-%02X-%02X", (unsigned char) mac_addr[0], (unsigned char) mac_addr[1],
																					   (unsigned char) mac_addr[2], (unsigned char) mac_addr[3],
																					   (unsigned char) mac_addr[4], (unsigned char) mac_addr[5]);
				if (_mac.empty() == true)
					return mac;

				if (BUTIL::Util::toUpper(_mac) == BUTIL::Util::toUpper(mac))
					return mac;
			}
		}
		return "";
	}

	/**
	* Recupera la MAC Address del adaptador indicado.
	*          
	* @param adapter_num Número de Adaptador de RED.
	* @param mac_addr Variable donde almacenar la MAC.
	*          
	* @return true si la operación es correcta 
	*/
	bool Socket::getAdapterInfo(int adapter_num,char *mac_addr)
	{
		// Reset the LAN adapter so that we can begin querying it
		NCB Ncb;
		memset(&Ncb,0,sizeof(Ncb));
		Ncb.ncb_command = NCBRESET;
		Ncb.ncb_lana_num = adapter_num;

		if (Netbios(&Ncb) != NRC_GOODRET) 
		{
			memcpy(mac_addr,"000000",6);  
			//mac_addr = "bad (NCBRESET): ";
			//mac_addr += Ncb.ncb_retcode;
			return false;
		}

		// Prepare to get the adapter status block
		memset(&Ncb,0,sizeof(Ncb));
		Ncb.ncb_command = NCBASTAT;
		Ncb.ncb_lana_num = adapter_num;
		strcpy_s((char *) Ncb.ncb_callname, sizeof(Ncb.ncb_callname), "*");
			
		struct ASTAT
		{
			ADAPTER_STATUS adapt;
			NAME_BUFFER NameBuff[30];
		}Adapter;

		memset(&Adapter,0,sizeof(Adapter));
		Ncb.ncb_buffer = (unsigned char *)&Adapter;
		Ncb.ncb_length = sizeof(Adapter);

		// Get the adapter's info and, if this works, return it in standard, colon-delimited form.
		if (Netbios(&Ncb) == 0)
		{
			memcpy(mac_addr,Adapter.adapt.adapter_address,6);
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Socket::setClientIpAddress(const char *newVal)
	{
		socket.put_ClientIpAddress(newVal);
		return true;
	}

}

