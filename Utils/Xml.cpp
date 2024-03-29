#include "Xml.h"
#include "Util.h"
#include <stdlib.h>
#include "tinyxml.h"
#include "zlib.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "Convert.h"


namespace BUTIL
{
	/**
	 * Convierte una cadena de UTF8 a WSTRING
	 * @param src Cadena en formato UTF8
	 * @return Cadena en formato WSTRING (UNICODE)
	 */
	std::wstring Xml::utf8_wstring( std::string src )
	{

		std::wstring dest;
		wchar_t w = 0;
		int bytes = 0;
		wchar_t err = L'�';
		for (size_t i = 0; i < src.size(); i++)
		{
			unsigned char c = (unsigned char)src[i];
			if (c <= 0x7f)
			{//first byte
				if (bytes)
				{
					dest.push_back(err);
					bytes = 0;
				}
				dest.push_back((wchar_t)c);
			}
			else if (c <= 0xbf)
			{//second/third/etc byte
				if (bytes)
				{
					w = ((w << 6)|(c & 0x3f));
					bytes--;
					if (bytes == 0)
						dest.push_back(w);
				}
				else
					dest.push_back(err);
			}
			else if (c <= 0xdf)
			{//2byte sequence start
				bytes = 1;
				w = c & 0x1f;
			}
			else if (c <= 0xef)
			{//3byte sequence start
				bytes = 2;
				w = c & 0x0f;
			}
			else if (c <= 0xf7)
			{//3byte sequence start
				bytes = 3;
				w = c & 0x07;
			}
			else
			{
				dest.push_back(err);
				bytes = 0;
			}
		}
		if (bytes)
			dest.push_back(err);
		
		return dest;
	}

	/**
	 * Convierte una cadena de WSTRING a UTF8
	 * @param src Cadena en formato WSTRING (UNICODE)
	 * @return Cadena en formato UTF8
	 */

	std::string Xml::wstring_utf8( std::wstring src )
	{
		std::string dest;

		for (size_t i = 0; i < src.size(); i++)
		{
			wchar_t w = src[i];
			if (w <= 0x7f)
				dest.push_back((char)w);
			else if (w <= 0x7ff)
			{
				dest.push_back(0xc0 | ((w >> 6)& 0x1f));
				dest.push_back(0x80| (w & 0x3f));
			}
			else if (w <= 0xffff)
			{
				dest.push_back(0xe0 | ((w >> 12)& 0x0f));
				dest.push_back(0x80| ((w >> 6) & 0x3f));
				dest.push_back(0x80| (w & 0x3f));
			}
			else if (w <= 0x10ffff)
			{
				dest.push_back(0xf0 | ((w >> 18)& 0x07));
				dest.push_back(0x80| ((w >> 12) & 0x3f));
				dest.push_back(0x80| ((w >> 6) & 0x3f));
				dest.push_back(0x80| (w & 0x3f));
			}
			else
				dest.push_back('?');
		}
		return dest;
	}




	/**
	 * Recupera un nodo string
	 * @param xMainNode Nodo a analizar
	 * @param key Nombre del nodo 
	 * @return valor Nodo. Si no existe retorna ""
	 */
	std::string Xml::XML_string (TiXmlElement *xMainNode , std::string key )
	{
		try
		{
			if( !xMainNode )
				return "";

			TiXmlElement *elem = xMainNode->FirstChildElement( key.c_str() );
			if( !elem )
				elem = xMainNode->FirstChildElement( Util::toLower( key ).c_str() );

			if( elem )
			{
				const char *pt = elem->GetText();
				if( pt )
					return std::string( pt );
			}
		}
		catch( ... )
		{
		}
		return "";
	}

	/**
	 * Recupera un nodo wstring (UNICODE)
	 * @param xMainNode Nodo a analizar
	 * @param key Nombre del nodo 
	 * @return valor Nodo. Si no existe retorna ""
	 */
	std::wstring Xml::XML_wstring (TiXmlElement *xMainNode , std::string key )
	{
		try
		{
			if( !xMainNode )
				return L"";

			TiXmlElement *elem = xMainNode->FirstChildElement( key.c_str() );
			if( !elem )
				elem = xMainNode->FirstChildElement( Util::toLower( key ).c_str() );

			if( elem )
			{
				const char *pt = elem->GetText();
				if( pt )
				{
					return utf8_wstring( std::string(pt) );
				}
			}
		}
		catch( ... )
		{
		}
		return L"";
	}

	/*
	* Comprueba si existe un nodo 
	*/
	bool Xml::existsNode(TiXmlElement *xMainNode, std::string key)
	{
		TiXmlElement *elem = xMainNode->FirstChildElement( key.c_str() );

		if(!elem)
			elem = xMainNode->FirstChildElement(Util::toLower(key).c_str());

		if(elem)
			return true;

		return false;
	}


	/**
	 * Recupera un nodo double
	 * @param xMainNode Nodo a analizar
	 * @param key Nombre del nodo 
	 * @param _default Valor por defecto a aplicar si no existe
	 * @return valor Nodo. Si no existe retorna valor parametro defecto o 0.00
	 */
	double Xml::XML_double(TiXmlElement *xMainNode , std::string key, double _default )
	{
		std::string val = XML_string( xMainNode, key );
		if( val.length() )
		{
			return atof ( val.c_str() );
		}
		return _default;
	}

	float Xml::XML_float(TiXmlElement *xMainNode , std::string key, float _default )
	{
		std::string val = XML_string( xMainNode, key );
		if( val.length() )
		{
			return (float)atof ( val.c_str() );
		}
		return _default;
	}

	/**
	 * Recupera un nodo double. Convierte un valor de centimos a euros.
	 * @param xMainNode Nodo a analizar
	 * @param key Nombre del nodo 
	 * @param _default Valor por defecto a aplicar si no existe
	 * @return valor Nodo. Si no existe retorna valor parametro defecto o 0.00
	 */
	double Xml::XML_dinero(TiXmlElement *xMainNode , std::string key, double _default )
	{
		std::string val = XML_string( xMainNode, key );
		if( val.length() )
		{
			__int64 ret = _atoi64( val.c_str() );
			if( ret==0 )
				return 0;

			return ((double)ret/100.00)+0.00001;
		}
		return _default;
	}

	/**
	 * Recupera un nodo int
	 * @param xMainNode Nodo a analizar
	 * @param key Nombre del nodo 
	 * @param _default Valor por defecto a aplicar si no existe
	 * @return valor Nodo. Si no existe retorna valor parametro defecto o 0
	 */
	int Xml::XML_int(TiXmlElement *xMainNode , std::string key, int _default )
	{
		std::string val = XML_string( xMainNode, key );
		if( val.length() )
		{
			return atoi ( val.c_str() );
		}
		return _default;
	}

	/**
	 * Recupera un nodo int64
	 * @param xMainNode Nodo a analizar
	 * @param key Nombre del nodo 
	 * @param _default Valor por defecto a aplicar si no existe
	 * @return valor Nodo. Si no existe retorna valor parametro defecto o 0
	 */
	__int64 Xml::XML_int64(TiXmlElement *xMainNode , std::string key, __int64 _default)
	{
		std::string val = XML_string( xMainNode, key );
		if( val.length() )
		{
			return _atoi64 ( val.c_str() );
		}
		return _default;
	}

	/**
	 * Recupera un nodo int64
	 * @param xMainNode Nodo a analizar
	 * @param key Nombre del nodo 
	 * @param _default Valor por defecto a aplicar si no existe
	 * @return valor Nodo. Si no existe retorna valor parametro defecto o 0
	 */
	unsigned __int64 Xml::XML_uint64(TiXmlElement *xMainNode , std::string key, unsigned __int64 _default)
	{
		unsigned __int64 ret = 0;
		unsigned __int64 mul = 1;
		std::string val = XML_string( xMainNode, key );
		if( val.length() )
		{
			for( size_t i=val.length()-1; i>=0; i-- )
			{
				char ascii= val[i];
				if( ascii>='0' && ascii<='9' )
				{
					ascii-='0';
					ret += (ascii*mul);
					mul*=10;
				}
			}
			return ret;
		}
		return _default;
	}

	/**
	 * Recupera un nodo int. Convierte de Euros a centimos.
	 * @param xMainNode Nodo a analizar
	 * @param key Nombre del nodo 
	 * @param _default Valor por defecto a aplicar si no existe
	 * @return valor Nodo. Si no existe retorna valor parametro defecto o 0
	 */
	int Xml::XML_intCent(TiXmlElement *xMainNode , std::string key, int _default )
	{
		std::string val = XML_string( xMainNode, key );
		if( val.length() )
		{
			if( XML_Cent( val ) )
			{
				val = Util::replace( val, ".", "" );
				return atoi ( val.c_str() );
			}

			double valDbl = atof( val.c_str() );
			valDbl += 0.001;
			valDbl *= 100.00;
			return static_cast<int>( valDbl );
		}
		return _default;
	}

	/**
	 * Recupera un nodo int64. Convierte de euros a centimos.
	 * @param xMainNode Nodo a analizar
	 * @param key Nombre del nodo 
	 * @param _default Valor por defecto a aplicar si no existe
	 * @return valor Nodo. Si no existe retorna valor parametro defecto o 0
	 */
	__int64 Xml::XML_int64Cent(TiXmlElement *xMainNode , std::string key, __int64 _default)
	{
		std::string val = XML_string( xMainNode, key );
		if( val.length() )
		{
			if( XML_Cent( val ) )
			{
				val = Util::replace( val, ".", "" );
				return _atoi64 ( val.c_str() );
			}
			long double valDbl = atof( val.c_str() );
			valDbl += 0.001;
			valDbl *= 100.00;
			return static_cast<__int64>( valDbl );
		}
		return _default;
	}

	/**
	 * Recupera un nodo uint
	 * @param xMainNode Nodo a analizar
	 * @param key Nombre del nodo 
	 * @param _default Valor por defecto a aplicar si no existe
	 * @return valor Nodo. Si no existe retorna valor parametro defecto o 0
	 */
	unsigned int Xml::XML_uint(TiXmlElement *xMainNode , std::string key, unsigned int _default )
	{
		std::string val = XML_string( xMainNode, key );
		if( val.length() )
		{
			long long lLong = _atoi64( val.c_str() );
			return (unsigned int)lLong;
		}
		return _default;
	}

	/**
	 * Recupera un nodo bool. Convierte 1, TRUE, YES, SI a true.
	 * @param xMainNode Nodo a analizar
	 * @param key Nombre del nodo 
	 * @return true o false.
	 */
	bool Xml::XML_bool(TiXmlElement *xMainNode , std::string key )
	{
		std::string val = XML_string( xMainNode, key );
		if( val.length() )
		{
			val = Util::toLower( val );
			if( val[0]=='1' || val[0]=='t' || val[0]=='y' || val[0]=='s')
			   return true;
		}
		return false;
	}


	/**
	 * Recupera un nodo string de un Atributo XML
	 * @param xMainNode Nodo a analizar
	 * @param key Nombre del nodo 
	 * @return valor Nodo. Si no existe retorna ""
	 */
	std::string Xml::XML_ATTR_string (TiXmlElement *xMainNode , std::string key)
	{
		try
		{
			if( !xMainNode )
				return "";

			const char *attr = xMainNode->Attribute( key.c_str() );
			if( !attr )
				attr = xMainNode->Attribute( Util::toLower( key ).c_str() );

			if( attr )
				return std::string( attr );
		}
		catch( ... )
		{
		}
		return "";
	}

	/**
	 * Recupera un nodo string de un Atributo XML
	 * @param xMainNode Nodo a analizar
	 * @param key Nombre del nodo 
	 * @return valor Nodo. Si no existe retorna ""
	 */
	std::wstring Xml::XML_ATTR_wstring (TiXmlElement *xMainNode , std::string key)
	{
		try
		{
			if( !xMainNode )
				return L"";

			const char *attr = xMainNode->Attribute( key.c_str() );
			if( !attr )
				attr = xMainNode->Attribute( Util::toLower( key ).c_str() );

			if( attr )
				return utf8_wstring( std::string( attr ) );
		}
		catch( ... )
		{
		}
		return L"";
	}

	/**
	 * Recupera un nodo int de un Atributo XML
	 * @param xMainNode Nodo a analizar
	 * @param key Nombre del nodo 
	 * @param _default Valor defecto si no existe el nodo
	 * @return valor Nodo. 
	 */
	int Xml::XML_ATTR_int (TiXmlElement *xMainNode , std::string key, int _default )
	{
		std::string val = XML_ATTR_string( xMainNode, key );

		if( val.length() )
			return atoi( val.c_str() );

		return _default;
	}

	/**
	 * Recupera un nodo int de un Atributo XML. Convierte Centimos a Euros
	 * @param xMainNode Nodo a analizar
	 * @param key Nombre del nodo 
	 * @param _default Valor defecto si no existe el nodo
	 * @return valor Nodo. 
	 */
	int Xml::XML_ATTR_intCent (TiXmlElement *xMainNode , std::string key, int _default )
	{
		std::string val = XML_ATTR_string( xMainNode, key );

		if( val.length() )
		{
			if( XML_Cent( val ) )
			{
				val = Util::replace( val, ".", "" );
				return atoi( val.c_str() );
			}

			double valDbl = atof( val.c_str() );
			valDbl += 0.001;
			valDbl *= 100.00;
			return static_cast<int>( valDbl );
		}

		return _default;
	}

	/**
	 * Recupera un nodo int64 de un Atributo XML. Convierte Centimos a Euros
	 * @param xMainNode Nodo a analizar
	 * @param key Nombre del nodo 
	 * @param _default Valor defecto si no existe el nodo
	 * @return valor Nodo. 
	 */
	__int64 Xml::XML_ATTR_int64Cent (TiXmlElement *xMainNode , std::string key, __int64 _default )
	{
		std::string val = XML_ATTR_string( xMainNode, key );

		if( val.length() )
		{
			if( XML_Cent( val ) )
			{
				val = Util::replace( val, ".", "" );
				return _atoi64( val.c_str() );
			}

			double valDbl = atof( val.c_str() );
			valDbl += 0.001;
			valDbl *= 100.00;
			return static_cast<__int64>( valDbl );
		}

		return _default;
	}


	/**
	 * Recupera un nodo double de un Atributo XML. 
	 * @param xMainNode Nodo a analizar
	 * @param key Nombre del nodo 
	 * @param _default Valor defecto si no existe el nodo
	 * @return valor Nodo. 
	 */
	double Xml::XML_ATTR_double (TiXmlElement *xMainNode , std::string key, double _default )
	{
		std::string val = XML_ATTR_string( xMainNode, key );

		if( val.length() )
			return atof( val.c_str() );

		return _default;
	}

	/**
	 * Recupera un nodo int64 de un Atributo XML. 
	 * @param xMainNode Nodo a analizar
	 * @param key Nombre del nodo 
	 * @param _default Valor defecto si no existe el nodo
	 * @return valor Nodo. 
	 */
	__int64 Xml::XML_ATTR_int64 (TiXmlElement *xMainNode , std::string key, __int64 _default )
	{
		std::string val = XML_ATTR_string( xMainNode, key );

		if( val.length() )
			return _atoi64( val.c_str() );

		return _default;
	}

	/**
	 * Recupera un nodo unsigned int64 de un Atributo XML. 
	 * @param xMainNode Nodo a analizar
	 * @param key Nombre del nodo 
	 * @param _default Valor defecto si no existe el nodo
	 * @return valor Nodo. 
	 */
	unsigned __int64 Xml::XML_ATTR_uint64 (TiXmlElement *xMainNode , std::string key, unsigned __int64 _default )
	{
		std::string val = XML_ATTR_string( xMainNode, key );

		if( val.length() )
			return static_cast<unsigned __int64>(_atoi64( val.c_str() ));

		return _default;
	}


	/**
	 * Recupera un nodo unsigned de un Atributo XML. 
	 * @param xMainNode Nodo a analizar
	 * @param key Nombre del nodo 
	 * @param _default Valor defecto si no existe el nodo
	 * @return valor Nodo. 
	 */
	unsigned int Xml::XML_ATTR_uint (TiXmlElement *xMainNode , std::string key, unsigned int _default )
	{
		std::string val = XML_ATTR_string( xMainNode, key );

		if( val.length() )
		{
			long long lLong = _atoi64( val.c_str() );
			return (unsigned int)lLong;
		}
		return _default;
	}

	/**
	 * Recupera un nodo bool de un Atributo XML. Convierte Yes, 1, True o Si a true 
	 * @param xMainNode Nodo a analizar
	 * @param key Nombre del nodo 
	 * @return valor Nodo. 
	 */
	bool Xml::XML_ATTR_bool (TiXmlElement *xMainNode , std::string key)
	{
		std::string val = XML_ATTR_string( xMainNode, key );
		if( val.length() )
		{
			val = Util::toLower( val );
			if( val[0]=='1' || val[0]=='t' || val[0]=='y' || val[0]=='s')
			   return true;
		}
		return false;
	}



	/**
	 * Prepara un mensaje XML completo.
	 * @param id Mensaje a formar
	 * @param msg cuerpo xml
	 */
	//std::string Xml::create( std::string id, std::string msg )
	//{
	//	std::stringstream ss;
	//	ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	//	ss << "<msg id=\""<< id << "\">";
	//	ss << msg;
	//	ss << "</msg>";
    //
	//	return ss.str();
	//}

    std::string Xml::createRoot(std::string msg, bool _response)
    {
        std::stringstream ss;
        ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
        ss << "<root";
        ss << XML_attr("response", _response);
        ss << ">";
        ss << msg;
        ss << "</root>";

        return ss.str();
    }
    std::wstring Xml::createRoot(std::wstring msg, bool _response)
    {
        std::wstringstream ss;
        ss << L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
        ss << L"<root";
        ss << XML_attr(L"response", _response);
        ss << L">";
        ss << msg;
        ss << L"</root>";

        return ss.str();
    }
    std::string Xml::createConfig(std::string msg)
    {
        std::stringstream ss;
        ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        ss << "<config>\n";
        ss << msg;
        ss << "</config>\n";

        return ss.str();
    }

    std::string Xml::XML_node(std::string _key, std::string _val)
    {
        return "<" + _key + ">" + _val + "</" + _key + ">";
    }


	/**
	 * Prepara un nodo para generar XML.
	 * @param _key Clave nodo
	 * @param _val Valor Nodo
	 * @return valor Nodo. <_key>_val</_key>
	 */
	std::string Xml::XML_nodo( std::string _key, std::string _val, bool _hex )
	{
        if( _hex )
            _val = BUTIL::Util::toHex( _val );
		return "<"+_key+">"+_val+"</"+_key+">";
	}

	/**
	 * Prepara un nodo para generar XML.
	 * @param _key Clave nodo
	 * @param _val Valor Nodo
	 * @return valor Nodo. <_key>_val</_key>
	 */
	std::string Xml::XML_nodo( std::string _key, std::wstring _val, bool _hex )
	{
		std::string utf8 = wstring_utf8( _val );

        if( _hex )
            utf8 = BUTIL::Util::toHex( utf8 );

		return "<"+_key+">"+utf8+"</"+_key+">";
	}

	/**
	 * Prepara un nodo para generar XML.
	 * @param _key Clave nodo
	 * @param _val Valor Nodo
	 * @return valor Nodo. <_key>_val</_key>
	 */
	std::string Xml::XML_nodo( std::string _key, int _val )
	{
		char buf[256];
		sprintf_s( buf, sizeof(buf), "<%s>%d</%s>", _key.c_str(), _val, _key.c_str() );
		return std::string( buf );
	}

	/**
	 * Prepara un nodo para generar XML.
	 * @param _key Clave nodo
	 * @param _val Valor Nodo
	 * @return valor Nodo. <_key>_val</_key>
	 */
	std::string Xml::XML_nodo( std::string _key, unsigned int _val )
	{
		char buf[256];
		sprintf_s( buf, sizeof(buf), "<%s>%u</%s>", _key.c_str(), _val, _key.c_str() );
		return std::string( buf );
	}

	/**
	 * Prepara un nodo para generar XML.
	 * @param _key Clave nodo
	 * @param _val Valor Nodo
	 * @return valor Nodo. <_key>_val</_key>
	 */
	std::string Xml::XML_nodo( std::string _key, __int64 _val )
	{
		char buf[256];
		sprintf_s( buf, sizeof(buf), "<%s>%I64d</%s>", _key.c_str(), _val, _key.c_str() );
		return std::string( buf );
	}

	/**
	 * Prepara un nodo para generar XML.
	 * @param _key Clave nodo
	 * @param _val Valor Nodo
	 * @return valor Nodo. <_key>_val</_key>
	 */
	std::string Xml::XML_nodo( std::string _key, unsigned __int64 _val, bool _hex )
	{
		char buf[256];
		std::string valAux;

		sprintf_s( buf, sizeof(buf), "%I64u", _val );
		valAux = std::string(buf);
		return XML_nodo( _key, valAux, _hex );
	}

	/**
	 * Prepara un nodo para generar XML.
	 * @param _key Clave nodo
	 * @param _val Valor Nodo
	 * @return valor Nodo. <_key>_val</_key>
	 */
	std::string Xml::XML_nodo( std::string _key, char *_val, bool _hex )
	{
		std::string aux = std::string(_val);
		if( _hex )
            aux = BUTIL::Util::toHex( aux );

		char buf[256];
		sprintf_s( buf, sizeof(buf), "<%s>%s</%s>", _key.c_str(), aux.c_str(), _key.c_str() );
		return std::string( buf );
	}

	/**
	 * Prepara un nodo para generar XML.
	 * @param _key Clave nodo
	 * @param _val Valor Nodo
	 * @return valor Nodo. <_key>_val</_key>
	 */
	std::string Xml::XML_nodo( std::string _key, double _val )
	{
		char buf[256];
		sprintf_s( buf, sizeof(buf), "<%s>%.2f</%s>", _key.c_str(), _val, _key.c_str() );
		return std::string( buf );
	}

	/**
	 * Prepara un nodo para generar XML.
	 * @param _key Clave nodo
	 * @param _val Valor Nodo
	 * @return valor Nodo. <_key>_val</_key>
	 */
	std::string Xml::XML_nodo( std::string _key, float _val )
	{
		char buf[256];
		sprintf_s( buf, sizeof(buf), "<%s>%.2f</%s>", _key.c_str(), _val, _key.c_str() );
		return std::string( buf );
	}


	/**
	 * Prepara un nodo para generar XML.
	 * @param _key Clave nodo
	 * @param _val Valor Nodo
	 * @return valor Nodo. <_key>_val</_key>
	 */
	std::string Xml::XML_nodo( std::string _key, bool _val )
	{
		char buf[256];
		sprintf_s( buf, sizeof(buf), "<%s>%s</%s>", _key.c_str(), (_val?"true":"false"), _key.c_str() );
		return std::string( buf );
	}

	/** 
	 * Genera un atributo con un valor int
	 * @param	_key	Nombre del atributo
	 * @param	_val	Valor
	 * @return	Atributo en formato ' _key="_val" '
	 */
    std::string Xml::XML_attr(const char * _key, int _val)
    {
        char buf[256];
        sprintf_s(buf, sizeof(buf), " %s=\"%d\" ", _key, _val);
        return std::string(buf);
    }

    std::string Xml::XML_attr(const char * _key, bool _val)
    {
        char buf[256];
        sprintf_s(buf, sizeof(buf), " %s=\"%s\" ", _key, (_val ? "true" : "false"));
        return std::string(buf);
    }
    std::wstring Xml::XML_attr(const wchar_t * _key, bool _val)
    {
        wchar_t buf[256];
        _swprintf(buf, L" %s=\"%s\" ", _key, (_val ? L"true" : L"false"));
        return std::wstring(buf);
    }

    /**
	 * Genera un atributo con un valor texto
	 * @param	_key	Nombre del atributo
	 * @param	_val	Valor
	 * @return	Atributo en formato ' _key="_val" '
	 */
    std::string Xml::XML_attr(std::string _key, std::string _val)
    {
        return _key + "=\"" + _val + "\" ";
    }
#ifdef _WSTRING
    std::wstring Xml::XML_attr(std::string _key, std::wstring _val)
    {
        return S2WS(_key) + L"=\"" + _val + L"\" ";
    }
#endif

    std::string Xml::XML_attr(const char * _key, char *_val)
    {
        char buf[256];
        sprintf_s(buf, sizeof(buf), " %s=\"%s\" ", _key, _val);
        return std::string(buf);
    }


	/**
	 * Leer una cadena diretamente de un nodo
	 * @param xMainNode Nodo a procesar
	 * @return Contenido del nodo
	 */
	std::string Xml::XML_string ( TiXmlElement *xMainNode )
	{ 
		try
		{
			if( xMainNode )
			{
				const char *pt = xMainNode->GetText();
				if( pt )
					return std::string( pt );
			}
		}
		catch( ... )
		{
		}
		return "";
	}

	/**
	 * Recupera un valor de 32bits en formato binario. Se complementan automaticamente los bit menos significativos
	 * que falten hasta completar los 32 bits. "<...>1</...>" = "10000000000000000000000000000000"
	 * @param	xMainNode Nodo a procesar
	 * @return	Valor convertido
	 */
	unsigned int Xml::XML_binary( TiXmlElement *xMainNode )
	{
		std::string ret = XML_string( xMainNode );
		unsigned int bin = 0;
		for( size_t i=0; i<ret.length(); i++ )
		{
			if( ret[i]=='1' )
				bin |= 1<<(31-i);

		}
		return bin;
	}

	/**
	 * Leer una cadena diretamente de un nodo
	 * @param xMainNode Nodo a procesar
	 * @return Contenido del nodo UNICODE
	 */
	std::wstring Xml::XML_wstring ( TiXmlElement *xMainNode )
	{ 
		try
		{
			if( xMainNode )
			{
				const char *pt = xMainNode->GetText();
				if( pt )
					return utf8_wstring( std::string( pt ) );
			}
		}
		catch( ... )
		{
		}
		return L"";
	}

	/**
	 * Leer un double diretamente de un nodo
	 * @param xMainNode Nodo a procesar
	 * @param _default Valor por defecto si no existe el nodo
	 * @return Contenido del nodo
	 */
	double Xml::XML_double( TiXmlElement *xMainNode, double _default )
	{
		std::string val = XML_string( xMainNode );
		if( val.length() )
			return atof ( val.c_str() );

		return _default;
	}
	
	/**
	 * Leer un float directamente de un nodo
	 * @param xMainNode Nodo a procesar
	 * @param _default Valor por defecto si no existe el nodo
	 * @return Contenido del nodo
	 */
	float Xml::XML_float( TiXmlElement *xMainNode, float  _default )
	{
		std::string val = XML_string( xMainNode );
		if( val.length() )
			return (float)atof ( val.c_str() );

		return _default;
	}

	/**
	 * Leer un double diretamente de un nodo. Centimos a Euros
	 * @param xMainNode Nodo a procesar
	 * @param _default Valor por defecto si no existe el nodo
	 * @return Contenido del nodo
	 */
	double Xml::XML_dinero( TiXmlElement *xMainNode, double _default )
	{
		std::string val = XML_string( xMainNode );
		if( val.length() )
		{
			__int64 ret = _atoi64( val.c_str() );
			if( ret==0 )
				return 0;

			return ((double)ret/100.00)+0.00001;
		}
		return _default;
	}

	/**
	 * Leer un int diretamente de un nodo
	 * @param xMainNode Nodo a procesar
	 * @param _default Valor por defecto si no existe el nodo
	 * @return Contenido del nodo
	 */
	int Xml::XML_int( TiXmlElement *xMainNode, int _default )
	{
		std::string val = XML_string( xMainNode );
		if( val.length() )
			return atoi ( val.c_str() );

		return _default;
	}

	/**
	 * Leer un int64 diretamente de un nodo
	 * @param xMainNode Nodo a procesar
	 * @param _default Valor por defecto si no existe el nodo
	 * @return Contenido del nodo
	 */
	__int64 Xml::XML_int64( TiXmlElement *xMainNode, __int64 _default )
	{
		std::string val = XML_string( xMainNode );
		if( val.length() )
			return _atoi64 ( val.c_str() );

		return _default;
	}

	/**
	 * Leer un unsigned int64 diretamente de un nodo
	 * @param xMainNode Nodo a procesar
	 * @param _default Valor por defecto si no existe el nodo
	 * @return Contenido del nodo
	 */
	unsigned __int64 Xml::XML_uint64( TiXmlElement *xMainNode, unsigned __int64 _default )
	{
		std::string val = XML_string( xMainNode );
		if( val.length() )
			return static_cast<unsigned __int64>(_atoi64 ( val.c_str() ));

		return _default;
	}

	/**
	 * Leer un int diretamente de un nodo. Pasa de Euros a Centimos
	 * @param xMainNode Nodo a procesar
	 * @param _default Valor por defecto si no existe el nodo
	 * @return Contenido del nodo
	 */
	int Xml::XML_intCent( TiXmlElement *xMainNode, int _default )
	{
		std::string val = XML_string( xMainNode );
		if( val.length() )
		{
			val = Util::replace( val, ".", "" );
			return atoi ( val.c_str() );
		}

		return _default;
	}

	/**
	 * Leer un int64 diretamente de un nodo. Pasa de Euros a centimos
	 * @param xMainNode Nodo a procesar
	 * @param _default Valor por defecto si no existe el nodo
	 * @return Contenido del nodo
	 */
	__int64 Xml::XML_int64Cent( TiXmlElement *xMainNode, __int64 _default )
	{
		std::string val = XML_string( xMainNode );
		if( val.length() )
		{
			val = Util::replace( val, ".", "" );
			return _atoi64 ( val.c_str() );
		}

		return _default;
	}

	/**
	 * Leer un bool diretamente de un nodo
	 * @param xMainNode Nodo a procesar
	 * @return Contenido del nodo
	 */
	bool Xml::XML_bool( TiXmlElement *xMainNode )
	{
		std::string val = XML_string( xMainNode );
		if( val.length() )
		{
			val = Util::toLower( val );
			if( val[0]=='1' || val[0]=='t' || val[0]=='y' || val[0]=='s')
			   return true;
		}
		return false;
	}

	/**
	 * Leer un unsigned int diretamente de un nodo
	 * @param xMainNode Nodo a procesar
	 * @param _default Valor por defecto si no existe el nodo
	 * @return Contenido del nodo
	 */
	unsigned int Xml::XML_uint( TiXmlElement *xMainNode, unsigned int _default )
	{
		std::string val = XML_string( xMainNode );
		if( val.length() )
		{
			long long lLong = _atoi64( val.c_str() );
			return (unsigned int)lLong;
		}
		return _default;
	}


	/**
	 * Carga un XMLDOCUMENT desde una cadena std::string
	 * @param doc Documento XMl cargar.
	 * @param msg Mensaje a cargar.
	 * @return Primero elemento o NULL si hay error.
	 */
	TiXmlElement * Xml::parse( TiXmlDocument &doc, std::string msg )
	{
        exLOGDEBUG("");
        doc.Clear();
		doc.Parse( msg.c_str() );
		if( doc.Error() )
			return NULL;

		return doc.FirstChildElement();

	}

	/**
	 * Carga un XMLDOCUMENT desde un archivo XML.
	 * @param doc Documento XMl cargar.
	 * @param msg Mensaje a cargar.
	 * @return Primero elemento o NULL si hay error.
	 */
	TiXmlElement * Xml::load( TiXmlDocument &doc, std::string _msg )
	{
        exLOGDEBUG("");
		doc.LoadFile(_msg.c_str() );
        if (doc.Error())
        {
            int row = doc.ErrorRow();
            int col = doc.ErrorCol();
            const char *desc = doc.ErrorDesc();
            
            std::ostringstream  msg;
            if (row!=0 || col!=0)
                msg << "Error reading " << _msg << " : " << desc << "@(" << row << "," << col << ")";
            else
                msg << "Error reading " << _msg << " : " << desc;
            exLOGERROR(msg.str().c_str());
            MBOX(msg.str(), "Info", MB_ICONERROR | MB_OK);
            return NULL;
        }

		return doc.FirstChildElement();
	}
	
	
    bool Xml::save(std::string _file, std::string _xml)
    {
        exLOGDEBUG("");
        bool status = false;
#ifdef _WSTRING
        std::wstring wxml = S2WS(_xml);
        uLong destSize = static_cast<uLong>(wxml.length());
        std::wofstream wfil(_file.c_str(), std::ios_base::out | std::ios::trunc | std::ios_base::binary);
        if (!wfil.fail()) {
            wfil.write(wxml.c_str(), destSize);
            wfil.close();
#else
        //std::string wxml = _xml;
        uLong destSize = static_cast<uLong>(_xml.length());
        std::ofstream wfil(_file.c_str(), std::ios_base::out | std::ios::trunc | std::ios_base::binary);
        if (!wfil.fail()) {
            wfil.write(_xml.c_str(), destSize);
            wfil.close();
            status = true;
#endif

        }
        //std::ofstream fil(_file.c_str(), std::ios_base::out | std::ios::trunc | std::ios_base::binary);
        //if (!fil.fail()) {
        //    fil.write(_xml.c_str(), destSize);
        //    fil.close();
        //}
        return status;
    }
    bool Xml::save(std::string _file, std::wstring _wxml)
    {
        exLOGDEBUG("");
        bool status = false;
        uLong destSize = static_cast<uLong>(_wxml.length());

        std::wofstream wfil(_file.c_str(), std::ios_base::out | std::ios::trunc | std::ios_base::binary);
        if (!wfil.fail()) {
            wfil.write(_wxml.c_str(), destSize);
            wfil.close();
        }
        return status;
    }

	/**
	 *	Almacena un texto en un archivo en formato ZXML
	 *
	 *	@param	_file	Nombre del fichero
	 *	@param	_xml	Contenido a guardar
	 *
	 *	@return	true si es correcto la escritura
	 */
    bool Xml::saveZ(std::string _file, std::string _xml)
    {
        exLOGDEBUG("");
        bool status = false;
        uLong destSize = static_cast<uLong>(_xml.length() * 4);
        char *dest = new char[destSize];

        int ret = compress((Bytef *)dest, &destSize, (Bytef *)_xml.c_str(), static_cast<uLong>(_xml.length() + 1));
        if (ret == Z_OK) 
        {
            std::ofstream fil(_file.c_str(), std::ios_base::out | std::ios::trunc | std::ios_base::binary);
            if (!fil.fail()) {
                fil.write(dest, destSize);
                fil.close();
            }
        }
        delete[]dest;
        return status;
    }
    bool Xml::saveZ(std::string _file, std::wstring _wxml)
    {
        exLOGDEBUG("");
        bool status = false;
        uLong destSize = static_cast<uLong>(_wxml.length() * 4);
        wchar_t *dest = new wchar_t[destSize];

        int ret = compress((Bytef *)dest, &destSize, (Bytef *)_wxml.c_str(), static_cast<uLong>(_wxml.length() + 1));
        if (ret == Z_OK) {
            std::wofstream fil(_file.c_str(), std::ios_base::out | std::ios::trunc | std::ios_base::binary);
            if (!fil.fail()) {
                fil.write(dest, destSize);
                fil.close();
            }
        }
        delete[]dest;
        return status;
    }

	/**
	 * Carga un archivo XML en formato comprimido.
	 * @param      doc Documento XML carga.
	 * @param      msg Nombre del archivo .zxml
	 * @return     Primer elemento XML o NULL si hay un error.
	 */
	TiXmlElement * Xml::loadZ( TiXmlDocument &doc, std::string msg )
	{
        exLOGDEBUG("");
		std::ifstream  fil( msg.c_str(), std::ios_base::in|std::ios_base::binary );
		if( fil.fail() )
			return NULL;

		fil.seekg( 0, std::ios_base::end );
		std::streamoff lon = fil.tellg();
		fil.seekg( 0, std::ios_base::beg );

		char *buf = new char [static_cast<unsigned int>(lon)];
		if( !buf )
		{
			fil.close();
			return NULL;
		}

		fil.read( buf, lon );
		fil.close();

		// Ahora reservamos al menos 32 veces la longitud original.

		uLong destSize = static_cast<uLong>(lon*32);
		char *dest = new char [ destSize ];
		if( !dest )
		{
			delete []buf;
			return false;
		}

		int ret = uncompress( (Bytef *)dest, &destSize, (Bytef *)buf, (uLong)lon );
		if( ret!=Z_OK )
		{
			delete []buf;
			delete []dest;
			return NULL;
		}

		std::string xml = std::string( dest );
		delete []buf;
		delete []dest;

		return parse( doc, xml );
	}


	/**
	 * Comprueba si la cadena introducida tiene 2 decimales para conversiones de euros.
	 * @param val Valor entrada
	 * @return true si es correcto.
	 */
	bool Xml::XML_Cent( std::string val)
	{
		std::string::size_type len,pos;

		len = val.length();
		if( len<3 )	// .00
			return false;

		pos = val.find( "." );
		if( pos==std::string::npos )
			return false;

		if( (len-pos) != 3 )
			return false;		// No tiene los decimales permitidos

		return true;
	}

}
