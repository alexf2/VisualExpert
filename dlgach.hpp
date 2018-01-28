#if !defined(_DLGACH_)
#define _DLGACH_

#include "stdafx.h"

#include "..\\RUDAKOW\\SERVIS\\borctl.hpp"


enum TPU_StrLimits { PU_MaxFirstName = 50, 
                     PU_MaxSecondName = 50, 
					 PU_MaxPass = 15,
                     PU_MaxPass2 = 15 };

class TPRopPageUser: public CPropertyPage
 {
   DECLARE_DYNCREATE( TPRopPageUser )

public:
  TPRopPageUser(): CPropertyPage() 
   {
     m_iBtnAnonim = 0;     
   }

  TPRopPageUser( UINT uiRc ): CPropertyPage(uiRc), m_iBtnAnonim(0) {};

  virtual ~TPRopPageUser();

  virtual BOOL OnInitDialog();  
  virtual void DoDataExchange( CDataExchange* pDX );
  
  afx_msg HBRUSH OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor );
  afx_msg void OnAnonime();
  afx_msg void OnNamed();
  void EnableNamed( bool );

  int m_iBtnAnonim;  
  CString m_csFirst, m_csSecond, m_csPass, m_csPass2;
  
protected:
  DECLARE_MESSAGE_MAP()
  //CButton m_btnAnonim, m_btnNamed;
  //CEdit m_edFirst, m_edSecond, m_edPass, m_edPass2;
 };

class TPRopPageAdm: public CPropertyPage
 {
   DECLARE_DYNCREATE( TPRopPageAdm )

public:
  TPRopPageAdm(): CPropertyPage() {};
  TPRopPageAdm( UINT uiRc ): CPropertyPage(uiRc) {};

  virtual ~TPRopPageAdm();

  virtual BOOL OnInitDialog();  
  virtual void DoDataExchange( CDataExchange* pDX );
  
  CString m_csPass, m_csPass2;
  
protected:  
  //CEdit m_edPass, m_edPass2;
 };


class TAcessChangeDlg: public TBWCCPropertySheet
 {

   DECLARE_DYNCREATE( TAcessChangeDlg )

public:
  TAcessChangeDlg(): TBWCCPropertySheet() {};
  TAcessChangeDlg( LPCTSTR pszCaption, CWnd *pParentWnd = NULL, UINT iSelectPage = 0 );
    

  virtual ~TAcessChangeDlg();

  
  TPRopPageUser m_pagUser, m_pagExp, m_pagKognit;
  TPRopPageAdm  m_pagAdm; 
 };


#endif
