#if !defined(_CREATEOBJ_DLG_)
#define _CREATEOBJ_DLG_

#include "stdafx.h"
#include "vs.rh"
//#include "fr_opt.hpp"
//#include "userlvl.hpp"

class TDlgType: public CDialog
 {
public:
  TDlgType( CWnd *pPar, LPCSTR lpTtl, bool bFlRul, bool bFlFac, bool bFlQ ):     
    CDialog( IDD_DLG_WHATCREATE, pPar )
   {
     m_ttl = lpTtl;
     m_mPar = pPar;	 
	 m_iRes = 0;
	 m_bFlRul = bFlRul;
	 m_bFlFac = bFlFac;
	 m_bFlQ   = bFlQ;
   }

  virtual BOOL OnInitDialog();
  virtual void DoDataExchange( CDataExchange* pDX );
  //virtual void OnOK();
  
  int m_iRes;
  CString m_str;

private:    
  CWnd* m_mPar;  
  CString m_ttl;
  
  bool m_bFlRul, m_bFlFac, m_bFlQ;
  CButton m_bRul, m_bFac, m_bQ;  
 };


#endif
