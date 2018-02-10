#if !defined(_ASK_DLG_)
#define _ASK_DLG_

#include "stdafx.h"
#include "vs.rh"
#include "var.hpp"
//#include "fr_opt.hpp"
//#include "userlvl.hpp"

class TAskDlg: public CDialog
 {
public:
  TAskDlg( CWnd *pPar, TVariantVar* pVar, int& rX, int& rY ):     
    CDialog( IDD_DLG_ASK, pPar ),
	m_X( rX ),
	m_Y( rY )
   {          
     m_pVar = pVar;
     m_mPar = pPar;	 
	 m_varDsk.Format( "%s [не инициализирована]", (LPCSTR)pVar->m_strName );
   }

  virtual BOOL OnInitDialog();
  virtual void DoDataExchange( CDataExchange* pDX );
  virtual void OnOK();

private:    
  CWnd* m_mPar;  
  int &m_X, &m_Y;
  
  CEdit m_edVal, m_edVar;
  CString m_varDsk;
  CString m_varVal;

  TVariantVar* m_pVar;
 };


#endif
