#pragma once

#include <windows.h>
#include <string>

namespace BUTIL
{

	typedef struct
	{
		DWORD total[2];
		DWORD state[8];
		BYTE buffer[64];
	}
	sha256_context;

	void sha256_starts( sha256_context *ctx );
	void sha256_update( sha256_context *ctx, BYTE *input, DWORD length );
	void sha256_finish( sha256_context *ctx, BYTE digest[32] );



	/**
	 * Clase encapsula SHA256 llamadas originales
	 */
	class SHA256
	{
	public:
		SHA256();
		void update( BYTE *, size_t size );
		void finish( BYTE * );
		std::string finish();

	private:
		sha256_context sha256;
	};
}

