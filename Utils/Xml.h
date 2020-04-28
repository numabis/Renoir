#pragma once

#include <windows.h>
#include <string>
//#include "dependencias.h"
#include "tinyxml.h"
//#include "zlib.h"

namespace BUTIL
{
	/** Clase que contiene funciones estaticas para ayuda en la gestión de nodos XML 
	    Tiene una dependencia directa a la clase TiXmlElement */
	class Xml
	{
	public:
		static std::string XML_string (TiXmlElement *xMainNode , std::string key );
		static std::wstring XML_wstring (TiXmlElement *xMainNode , std::string key );

		static unsigned int XML_binary( TiXmlElement * );

		static double XML_double(TiXmlElement *xMainNode , std::string key, double=0.00 );
		static float XML_float(TiXmlElement *xMainNode , std::string key, float=0.00f );

		static double XML_dinero(TiXmlElement *xMainNode , std::string key, double=0.00 );
		static int XML_int(TiXmlElement *xMainNode , std::string key, int=0 );
		static __int64 XML_int64 (TiXmlElement *xMainNode , std::string key, __int64=0 );
		static unsigned __int64 XML_uint64 (TiXmlElement *xMainNode , std::string key, unsigned __int64=0 );
		static unsigned int XML_uint(TiXmlElement *xMainNode , std::string key, unsigned int=0 );
		static bool XML_bool(TiXmlElement *xMainNode , std::string key );
		
		static __int64 XML_int64Cent (TiXmlElement *xMainNode , std::string key, __int64=0 );

		static int XML_intCent (TiXmlElement *xMainNode , std::string key, int=0 );
		

		static std::string XML_string ( TiXmlElement *xMainNode );
		static std::wstring XML_wstring ( TiXmlElement *xMainNode );

		static double XML_double( TiXmlElement *xMainNode, double=0.00 );
		static float XML_float( TiXmlElement *xMainNode, float=0.00f );

		static double XML_dinero( TiXmlElement *xMainNode, double=0.00 );
		static int XML_int( TiXmlElement *xMainNode, int=0 );
		static __int64 XML_int64 ( TiXmlElement *xMainNode, __int64=0 );
		static unsigned __int64 XML_uint64 ( TiXmlElement *xMainNode, unsigned __int64=0 );
		static int XML_intCent( TiXmlElement *xMainNode, int=0 );
		static __int64 XML_int64Cent ( TiXmlElement *xMainNode, __int64=0 );
		static unsigned int XML_uint( TiXmlElement *xMainNode, unsigned int=0 );
		static bool XML_bool(TiXmlElement *xMainNode );

		static std::string XML_ATTR_string (TiXmlElement *, std::string);
		static std::wstring XML_ATTR_wstring (TiXmlElement *, std::string);

		static int XML_ATTR_int (TiXmlElement *, std::string, int=0 );
		static __int64 XML_ATTR_int64 (TiXmlElement *, std::string, __int64=0 );
		static unsigned __int64 XML_ATTR_uint64 (TiXmlElement *, std::string, unsigned __int64=0 );

		static int XML_ATTR_intCent (TiXmlElement *, std::string, int=0 );
		static __int64 XML_ATTR_int64Cent (TiXmlElement *, std::string, __int64=0 );
		static unsigned int XML_ATTR_uint (TiXmlElement *, std::string, unsigned int=0 );
		static double XML_ATTR_double (TiXmlElement *, std::string, double=0 );
		static bool XML_ATTR_bool (TiXmlElement *, std::string);


		

		//static std::string create( std::string , std::string );
        static std::string createRoot(std::string msg, bool);
        static std::wstring createRoot(std::wstring msg, bool);
        static std::string createConfig(std::string msg);
        static std::string XML_nodo( std::string, std::string, bool=false );
		static std::string XML_nodo( std::string, std::wstring, bool=false );

		static std::string XML_nodo( std::string, int );
		static std::string XML_nodo( std::string, unsigned int );
		static std::string XML_nodo( std::string, __int64 );
		static std::string XML_nodo( std::string, unsigned __int64, bool=false );
		static std::string XML_nodo( std::string, char *, bool=false );
		static std::string XML_nodo( std::string, double );
		static std::string XML_nodo( std::string, float );
		static std::string XML_nodo( std::string, bool );

        static std::string XML_attr(const char *, int);
        static std::string XML_attr(const char *, bool);
        static std::wstring XML_attr(const wchar_t *, bool);
        static std::string XML_attr(const char *, char *);
        static std::string XML_attr(std::string, std::string);
#ifdef _WSTRING
        static std::wstring XML_attr(std::string, std::wstring);
#endif

		static TiXmlElement * parse( TiXmlDocument &, std::string msg );
		static TiXmlElement * load( TiXmlDocument &, std::string msg );
		static TiXmlElement * loadZ( TiXmlDocument &, std::string msg );
        static bool saveZ(std::string, std::string);
        static bool save(std::string, std::string);
        static bool saveZ(std::string, std::wstring);
        static bool save(std::string, std::wstring);
        static std::wstring utf8_wstring( std::string );
		static std::string wstring_utf8( std::wstring );

		// Comprueba si existe un nodo 
		static bool existsNode(TiXmlElement *, std::string);

	private:
		static bool XML_Cent( std::string );


	};
}


