#include "CultureInfo.h"
#include "util.h"

namespace BUTIL {

const std::wstring BUTIL::CultureInfo::nombrePais[BUTIL::CultureInfo::N_PAISES] = {
	L"España", 
	L"Norway", 
	L"Mexico", 
	L"UK", 
	L"U.S.A", 
	L"Costa Rica",
};

const std::wstring BUTIL::CultureInfo::nombreIdioma[BUTIL::CultureInfo::N_IDIOMA] = {
	L"Español", 
	L"Noruego", 
	L"Inglés", 
	L"Catalán"
};

BUTIL::CultureInfo::E_PAIS BUTIL::CultureInfo::pais = BUTIL::CultureInfo::SPAIN;
BUTIL::CultureInfo::E_IDIOMA BUTIL::CultureInfo::idioma = BUTIL::CultureInfo::CASTELLANO;

char BUTIL::CultureInfo::sepMiles = '.';
char BUTIL::CultureInfo::sepDecimales = ',';
std::string BUTIL::CultureInfo::sepsMiles = ".";
std::string BUTIL::CultureInfo::sepsDecimales = ",";

std::wstring BUTIL::CultureInfo::sepwMiles = L".";
std::wstring BUTIL::CultureInfo::sepwDecimales = L",";

BUTIL::CultureInfo::E_ALIGN_SIMB BUTIL::CultureInfo::alignSimbMon = BUTIL::CultureInfo::E_ALIGN_DCHA;
BUTIL::CultureInfo::E_ALIGN_SIMB BUTIL::CultureInfo::alignSimbCen = BUTIL::CultureInfo::E_ALIGN_DCHA;

/////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Establece el país y configura todos los parámentros de ese país por defecto.
 * @param[in] _id Identificador del país. 
 */
void BUTIL::CultureInfo::setPais(E_PAIS _pais)
{
	if (_pais < N_PAISES)
	{
		pais = _pais;

		switch (pais)
		{
		case SPAIN:
			setSeparadores('.', ',');
			setIdioma(CASTELLANO);
			setAlignSimbMon(E_ALIGN_DCHA);
			setAlignSimbCen(E_ALIGN_DCHA);
			break;

		case NORUEGA:
			setSeparadores('.', ',');
			setIdioma(NORUEGO);
			setAlignSimbMon(E_ALIGN_DCHA);
			setAlignSimbCen(E_ALIGN_DCHA);
			break;

		case MEXICO:
			setSeparadores('.', ',');
			setIdioma(CASTELLANO);
			setAlignSimbMon(E_ALIGN_DCHA);
			setAlignSimbCen(E_ALIGN_DCHA);
			break;

		case UK:
			setSeparadores(',', '.');
			setIdioma(INGLES);
			setAlignSimbMon(E_ALIGN_IZDA);
			setAlignSimbCen(E_ALIGN_DCHA);
			break;
		
		case USA:
			setSeparadores(',', '.');
			setIdioma(INGLES);
			setAlignSimbMon(E_ALIGN_IZDA);
			setAlignSimbCen(E_ALIGN_DCHA);
			break;
			
		case COSTA_RICA:
			setSeparadores('.', ',');
			setIdioma(CASTELLANO);
			setAlignSimbMon(E_ALIGN_DCHA);
			setAlignSimbCen(E_ALIGN_DCHA);
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Obtiene el código de país configurado
 * @returns Identificador del país. 
 */
BUTIL::CultureInfo::E_PAIS BUTIL::CultureInfo::getPais(void)
{	
	return pais;
}


/////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Establece el idioma 
 * @param[in] _idioma Identificador del idioma.  
 */
void BUTIL::CultureInfo::setIdioma(E_IDIOMA _idioma)
{
	if (_idioma < N_IDIOMA)
		idioma = _idioma;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Obtiene el código del idioma configurado
 * @returns Identificador del idioma. 
 */
BUTIL::CultureInfo::E_IDIOMA BUTIL::CultureInfo::getIdioma(void)
{
	return idioma;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Consulta el nombre textual del país indicado.
 * @param[in] _id Identificador del país.
 * @returns Nombre del país
 */
std::wstring BUTIL::CultureInfo::getNombrePais(void)
{
	return getNombrePais(pais);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Consulta el nombre textual del país indicado.
 * @param[in] _id Identificador del país.
 * @returns Nombre del país
 */
std::wstring BUTIL::CultureInfo::getNombrePais(BUTIL::CultureInfo::E_PAIS _id)
{
	std::wstring result;
	if (_id < N_PAISES)
		return nombrePais[_id];
	else
		return std::wstring(L"no defined");
}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Consulta el nombre del idioma actual
 * @returns Nombre del idioma
 */
std::wstring BUTIL::CultureInfo::getNombreIdioma(void)
{
	return getNombreIdioma(idioma);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Consulta el nombre del idioma indicado.
 * @param[in] _id Identificador del idioma.
 * @returns Nombre del idioma
 */
std::wstring BUTIL::CultureInfo::getNombreIdioma(BUTIL::CultureInfo::E_IDIOMA _id)
{
	if (_id < N_IDIOMA)
		return nombreIdioma[_id];
	else
		return std::wstring(L"no defined");

}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Establece unos nuevos caracteres separadores de decimales y miles.
 */
void CultureInfo::setSeparadores(char _sepMiles, char _sepDecimales)
{
	char cad[2];

	sepMiles = _sepMiles;
	cad[0] = sepMiles;
	cad[1] = '\0';
	sepsMiles = std::string(cad);
	sepwMiles = BUTIL::Util::ToWideString(sepsMiles); 
	
	sepDecimales = _sepDecimales;
	cad[0] = sepDecimales;
	sepsDecimales = std::string(cad);
	sepwDecimales = BUTIL::Util::ToWideString(sepsDecimales);	
}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Obtiene el caracter separador de miles.
 * @returns Caracter separador para los miles.
 */
char CultureInfo::getSepMiles(void)
{
	return sepMiles;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Obtiene el caracter separador de decimales.
 * @returns Caracter separador para los decimales.
 */
char CultureInfo::getSepDecimales(void)
{
	return sepDecimales;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Obtiene el caracter separador de miles.
 * @returns Caracter separador para los miles.
 */
std::string CultureInfo::getsSepMiles(void)
{
	return sepsMiles;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Obtiene el caracter separador de decimales.
 * @returns Caracter separador para los decimales.
 */
std::string CultureInfo::getsSepDecimales(void)
{
	return sepsDecimales;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Obtiene el caracter separador de miles.
 * @returns Caracter separador para los miles.
 */
std::wstring CultureInfo::getwSepMiles(void)
{
	return sepwMiles;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Obtiene el caracter separador de decimales.
 * @returns Caracter separador para los decimales.
 */
std::wstring CultureInfo::getwSepDecimales(void)
{
	return sepwDecimales;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/** 
 * Establece la posición del símbolo monetario en una cantidad monetaria.
 * @param[in] _align Posicionamiento.
 */
void CultureInfo::setAlignSimbMon(E_ALIGN_SIMB _align)
{
	if (_align <= E_ALIGN_DCHA)
		alignSimbMon = _align;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/** 
 * Obtiene el posicionamiento del símbolo monetario establecido.
 */
CultureInfo::E_ALIGN_SIMB CultureInfo::getAlignSimbMon(void)
{
	return alignSimbMon;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/** 
 * Establece la posición del símbolo monetario en una cantidad monetaria cuando son céntimos
 * @param[in] _align Posicionamiento.
 */
void CultureInfo::setAlignSimbCen(E_ALIGN_SIMB _align)
{
	if (_align <= E_ALIGN_DCHA)
		alignSimbCen = _align;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/** 
 * Obtiene el posicionamiento del símbolo monetario establecido cuando son céntimos
 */
CultureInfo::E_ALIGN_SIMB CultureInfo::getAlignSimbCen(void)
{
	return alignSimbCen;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/** 
 * Obtiene el símbolo monetario del país configurado
  *  @param[in] _cent FALSE si deseamos el símbolo de la moneda entera. TRUE si deseamos el símbolo de los céntimos. 
 *	@returns símbolo monetario.
 */
std::wstring CultureInfo::getSimboloMoneda(bool _cent)
{
	return getSimboloMoneda(pais, _cent);
}


/////////////////////////////////////////////////////////////////////////////////////////////
/** 
 * Obtiene el símbolo monetario de un país.
 *	@param[in] _pais País del que queremos conocer el símbolo monetario.
 *  @param[in] _cent FALSE si deseamos el símbolo de la moneda entera. TRUE si deseamos el símbolo de los céntimos. 
 *	@returns símbolo monetario.
 */
std::wstring CultureInfo::getSimboloMoneda(BUTIL::CultureInfo::E_PAIS _pais, bool _cent)
{
	std::wstring result = L"";

	if (!_cent)
	{
		switch(_pais)
		{
		case BUTIL::CultureInfo::SPAIN:
			result = L"\u20AC";
			break;

		case BUTIL::CultureInfo::NORUEGA:
			result = L"Kr";
			break;

		case BUTIL::CultureInfo::MEXICO:
		case BUTIL::CultureInfo::USA:
			result = L"\u0024";
			break;

		case BUTIL::CultureInfo::UK:
			result = L"\u20A4";
			break;

		case BUTIL::CultureInfo::COSTA_RICA:
			result = L"\u20A1";
			break;
		}
	}
	else
	{
		switch(_pais)
		{
		case BUTIL::CultureInfo::SPAIN:
			result = L"\u20AC";
			break;

		case BUTIL::CultureInfo::NORUEGA:
			result = L"Kr";
			break;

		case BUTIL::CultureInfo::MEXICO:
		case BUTIL::CultureInfo::USA:
			result = L"\u00A2";
			break;

		case BUTIL::CultureInfo::UK:
			result = L"p";
			break;

		case BUTIL::CultureInfo::COSTA_RICA:
			result = L"";
			break;
		}
	}
	return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/** 
 * Obtiene el nombre de la moneda del país configurado
 *	@param[in] _centimos TRUE si queremos saber el nombre de los céntimos, FALSE el de la unidad monetaria.
 *	@param[in] _plural TRUE si queremos el nombre en plural, FALSE singular.
 *	@returns nombre de la moneda.
 */
std::wstring CultureInfo::getNombreMoneda(bool _centimos, bool _plural)
{
	return getNombreMoneda(pais, _centimos, _plural);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/** 
 * Obtiene el nombre de la moneda del país.
 *	@param[in] _pais País del que queremos conocer el símbolo monetario. 
 *	@param[in] _centimos TRUE si queremos saber el nombre de los céntimos, FALSE el de la unidad monetaria.
 *	@param[in] _plural TRUE si queremos el nombre en plural, FALSE singular.
 *	@returns nombre de la moneda.
 */
std::wstring CultureInfo::getNombreMoneda(BUTIL::CultureInfo::E_PAIS _pais, bool _centimos, bool _plural)
{
	std::wstring result = L"";
	if (!_centimos)
	{
		switch(_pais)
		{
		case BUTIL::CultureInfo::SPAIN:
			if (_plural)
				result = L"Euro";
			else
				result = L"Euros";
			break;

		case BUTIL::CultureInfo::NORUEGA:
			if (_plural)
				result = L"Kroner";
			else
				result = L"Krone";
			break;

		case BUTIL::CultureInfo::MEXICO:
			if (_plural)
				result = L"Pesos";
			else
				result = L"Peso";
			break;

		case BUTIL::CultureInfo::USA:
			if (_plural)
				result = L"Dollars";
			else
				result = L"Dollar";
			break;

		case BUTIL::CultureInfo::UK:
			if (_plural)
				result = L"Pounds";
			else
				result = L"Pound";
			break;

		case BUTIL::CultureInfo::COSTA_RICA:
			if (_plural)
				result = L"Colones";
			else
				result = L"Colón";
			break;
		}	
	}
	else
	{
		switch(_pais)
		{
		case BUTIL::CultureInfo::SPAIN:
			if (_plural)
				result = L"Céntimos";
			else
				result = L"Céntimo";
			break;

		case BUTIL::CultureInfo::NORUEGA:
			if (_plural)
				result = L"øre";
			else
				result = L"øre";
			break;

		case BUTIL::CultureInfo::MEXICO:
			if (_plural)
				result = L"Centavos";
			else
				result = L"Centavo";
			break;

		case BUTIL::CultureInfo::USA:
			if (_plural)
				result = L"Cents";
			else
				result = L"Cent";
			break;

		case BUTIL::CultureInfo::UK:
			if (_plural)
				result = L"PENCE";
			else
				result = L"PENNY";
			break;

		case BUTIL::CultureInfo::COSTA_RICA:
			result = L"";
			break;
		}	
	}

	return result;
}

///////////////////////////////////////////////////////////////////////////////
/**
 * Formatea una fecha en el formato del país establecido.
 */
std::string BUTIL::CultureInfo::formatFecha(int _dia, int _mes, int _anio)
{	
	char buf[256];
	buf[0] = '\0';

	switch(pais)
	{
	case BUTIL::CultureInfo::SPAIN:
	case BUTIL::CultureInfo::NORUEGA:
	case BUTIL::CultureInfo::MEXICO:
	case BUTIL::CultureInfo::COSTA_RICA:
		sprintf_s( buf, sizeof(buf), "%02d/%02d/%04d", _dia, _mes, _anio);
		break;
	
	case BUTIL::CultureInfo::UK:
	case BUTIL::CultureInfo::USA:
		sprintf_s( buf, sizeof(buf), "%02d/%02d/%04d", _mes, _dia, _anio);
		break;
	}	
	
	return std::string(buf);
}


///////////////////////////////////////////////////////////////////////////////
/**
 * Operador de incremento para el enum E_PAIS
 */
BUTIL::CultureInfo::E_PAIS operator++(BUTIL::CultureInfo::E_PAIS &rs, int)
{
	if (rs >= BUTIL::CultureInfo::N_PAISES)
		rs = BUTIL::CultureInfo::N_PAISES;
	else
		rs = (BUTIL::CultureInfo::E_PAIS)(rs + 1);
	return rs;
};


/////////////////////////////////////////////////////////////////////////////////
/**
 * Operador de incremento para el enum E_IDIOMA
 */
BUTIL::CultureInfo::E_IDIOMA operator++(BUTIL::CultureInfo::E_IDIOMA &rs, int)
{
	if (rs >= BUTIL::CultureInfo::N_IDIOMA)
		rs = BUTIL::CultureInfo::N_IDIOMA;
	else
		rs = (BUTIL::CultureInfo::E_IDIOMA)(rs + 1);
	return rs;
};



} // namespace BUTIL



