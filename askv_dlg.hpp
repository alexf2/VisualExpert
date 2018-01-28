#if !defined(_ASKVAR_DLGW_)
#define _ASKVAR_DLGW_

#include "stdafx.h"
#include "warea.hpp"


class TAskVarDlg: public CDialog
 {
public:
  TAskVarDlg( CWnd *pPar, int id, TVariantVar* pVar ):     
    CDialog( id, pPar )	
   {          
     m_pVar = pVar;     
	 m_varDsk.Format( "%s [не инициализирована]", (LPCSTR)pVar->m_strName );
	 m_dKD = 1;
   }

  virtual BOOL OnInitDialog();
  virtual void DoDataExchange( CDataExchange* pDX );

  double m_dKD;

private:        
  CEdit m_edVal, m_edVar;
  CString m_varDsk;
  CString m_varVal;

  TVariantVar* m_pVar;
 };


#endif
