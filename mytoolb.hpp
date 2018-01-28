#if !defined(_MYTOOLB_)
#define _MYTOOLB_

#include "stdafx.h"
#include "autorc.hpp"
#include "vs.rh"

class TToolBar: public CToolBar
 {
public:
  DECLARE_DYNCREATE( TToolBar )

  TToolBar();
  virtual ~TToolBar() {};
  
  void AsnText( char* const* lpStr, int iNumb, UINT *pBtn, int iNumbBtn, bool );
  void CompressBtns();

  //virtual CSize CalcFixedLayout( BOOL bStretch, BOOL bHorz );  
  //virtual CSize CalcDynamicLayout( int nLength, DWORD dwMode );
  

private:
  //CFont m_cfFnt;

  DECLARE_MESSAGE_MAP()
 };


#endif
