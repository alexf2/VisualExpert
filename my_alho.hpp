#if !defined(_MY_ALHO_)
#define _MY_ALHO_

//#include <algorithm>
//using namespace std;

#include "stdafx.h"

void KillSpaces( LPCSTR lpSource, LPSTR lpDst );

template<class T, class C> T MyFind( C::iterator itSta, 
									 C::iterator itEnd, 
									 T pWhat ) 
 {
   for( ; itSta != itEnd; ++itSta )
	 if( **itSta == *pWhat ) return *itSta;

   return NULL;
 }

typedef bool (*TCmpFuncPtr)( const void* p1, const void* p2 );

template<class T, class C> T MyFind2( C::iterator itSta, 
									  C::iterator itEnd, 
									  T pWhat, TCmpFuncPtr pCmp ) 
 {
   for( ; itSta != itEnd; ++itSta )
	 if( pCmp((void*)*itSta, (void*)pWhat) == true ) return *itSta;

   return NULL;
 }

#endif
