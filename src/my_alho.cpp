#include "my_alho.hpp"

void KillSpaces( LPCSTR lpSource, LPSTR lpDst )
 {
   for( int i = strlen(lpSource); i; i--, lpSource++ )
	 if( *lpSource != ' ' ) *lpDst++ = *lpSource;

   *lpDst = 0;
 }

