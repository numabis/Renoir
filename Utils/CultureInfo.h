#pragma once

#include <string>

namespace BUTIL
{
/**
\brief	Almacena información referente a la cultura que queremos usar.
*
* Básicamente almacena información sobre los caracteres separadores de decimales y miles.
* Más adelante puede que almacene formatos de fecha u otros parámetros dependientes de la cultura.
*/
class CultureInfo
{
public:
	/**
	 * Definición de paises soportados en el juego.
	 */
	enum E_PAIS 
	{ 		
		E_PAIS_INI,
		SPAIN = E_PAIS_INI, 
		NORUEGA, 
		MEXICO, 
		UK, 
		USA, 
		COSTA_RICA,
		E_PAIS_FIN = COSTA_RICA,
		N_PAISES 
	};


	/// Idiomas definidos en el juego
	enum E_IDIOMA {
		E_IDIOMA_INI,
		CASTELLANO = E_IDIOMA_INI, 
		NORUEGO, 
		INGLES, 
		CATALAN, 
		E_IDIOMA_FIN = CATALAN,
		N_IDIOMA,
	};	

	static void setPais(E_PAIS _pais);
	static E_PAIS getPais(void);
	static std::wstring getNombrePais(void);
	static std::wstring getNombrePais(E_PAIS _id);	
	static std::wstring getNombreIdioma(void);
	static std::wstring getNombreIdioma(E_IDIOMA _id);

	static void setIdioma(E_IDIOMA _idioma);
	static E_IDIOMA getIdioma(void);

	static void setSeparadores(char _sepMiles, char _sepDecimales);		
	static char getSepMiles(void);
	static char getSepDecimales(void);

	static std::string getsSepMiles(void);
	static std::string getsSepDecimales(void);

	static std::wstring getwSepMiles(void);
	static std::wstring getwSepDecimales(void); 

	/** 
	 * Alineación del símbolo monetario
	 */
	enum E_ALIGN_SIMB {
		E_ALIGN_NONE,		/**< sin símbolo monetario */
		E_ALIGN_IZDA,		/**< Símbolo a la izda de la parte entera */
		E_ALIGN_DCHA,		/**< Símbolo a la derecha de la parte decimal. */
	};		
	static void setAlignSimbMon(E_ALIGN_SIMB  _align);
	static E_ALIGN_SIMB  getAlignSimbMon(void);
	static void setAlignSimbCen(E_ALIGN_SIMB  _align);
	static E_ALIGN_SIMB  getAlignSimbCen(void);

	
	static std::wstring getSimboloMoneda(bool _cent=false);
	static std::wstring getSimboloMoneda(BUTIL::CultureInfo::E_PAIS _pais, bool _cent=false);		
	static std::wstring getNombreMoneda(bool _centimos=false, bool _plural=true);
	static std::wstring getNombreMoneda(BUTIL::CultureInfo::E_PAIS _pais, bool _centimos=false, bool _plural=true);

	static std::string formatFecha(int _dia, int _mes, int _anio);

private:
	static E_PAIS pais;				/**< País configurado. */
	static E_IDIOMA idioma;			/**< Idioma configurado. */

	static const std::wstring nombrePais[N_PAISES];		/**< Nombre textual de los países. */		
	static const std::wstring nombreIdioma[N_IDIOMA];	/**< Nombre textual del idioma. */

	static char sepMiles;		/**< almacena el caracter separador de miles. */
	static char sepDecimales;	/**< almacena el caracter separador de decimales. */		

	static std::string sepsMiles;		/**< almacena un string  separador de miles. */
	static std::string sepsDecimales;	/**< almacena un string separador de decimales. */		

	static std::wstring sepwMiles;		/**< almacena un wstring separador de miles. */
	static std::wstring sepwDecimales;	/**< almacena un wstring separador de decimales. */		
	
	static E_ALIGN_SIMB  alignSimbMon;		/**< Posición del símbolo Monetario en una cantidad monetaria. */
	static E_ALIGN_SIMB  alignSimbCen;		/**< Posición del símbolo Monetario en una cantidad monetaria cuando son céntimos. */

private: 
	/** 
	* Constructor. No se permiten instancias de esta clase.
	*/
	CultureInfo(void) {};

	/** 
	* Destructor     
	*/
	~CultureInfo(void) {};
};

/**
 * Defino operadores para los enum de Pais
 */
BUTIL::CultureInfo::E_PAIS operator++(CultureInfo::E_PAIS &rs, int);


/**
 * Defino operadores para los enum de Idioma
 */
BUTIL::CultureInfo::E_IDIOMA operator++(BUTIL::CultureInfo::E_IDIOMA &rs, int);

}

