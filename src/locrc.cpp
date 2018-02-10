#include "locrc.hpp"


TLocalyzeRC::TLocalyzeRC( HINSTANCE han )
 {
   m_hinstOldRC = AfxGetResourceHandle();
   AfxSetResourceHandle( han );
 }
TLocalyzeRC::~TLocalyzeRC()
 {
   AfxSetResourceHandle( m_hinstOldRC );
 }
