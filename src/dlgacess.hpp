#if !defined(_DLGACESS_)
#define _DLGACESS_

#include "stdafx.h"

class CAcessDialog: public CDialog
 {
public:
  CAcessDialog( UINT nIDTemplate, CWnd* pParentWnd = NULL, LPCSTR lp = NULL ):
    m_str( lp ),
    CDialog( nIDTemplate, pParentWnd )
	 {};

  virtual ~CAcessDialog();

  virtual BOOL OnInitDialog();

private:
  CString m_str;  
  CStatic m_Icon;
 };


#endif
