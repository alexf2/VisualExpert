#if !defined(_LOC_RC_)
#define _LOC_RC_

#include "stdafx.h"

class TLocalyzeRC
 {
public:
  TLocalyzeRC( HINSTANCE han );   
  ~TLocalyzeRC();
 
private:
  HINSTANCE m_hinstOldRC;   
 };


#endif
