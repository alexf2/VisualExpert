#if !defined(_TLSF_FILEDLG_)
#define _TLSF_FILEDLG_

#include "stdafx.h"


class TLSFileDlg: public CFileDialog
 {
public:
  TLSFileDlg( BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName, DWORD dwFlags, LPCTSTR lpszFilter, 
              CWnd* pParentWnd, bool bFlShowTtl, bool bFlWarnPath );

  virtual void OnFolderChange();
  virtual void OnFileNameChange();
  virtual void OnInitDone();

  virtual BOOL OnInitDialog();

  afx_msg HBRUSH OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor );

private:
  bool m_bFirstCall;
  BOOL m_bOpen;  

  CString m_csPrivPath;
  bool CheckChanged();

  bool m_bRedTtl;
  bool m_bFlShowTtl;

  bool m_bFlWarnPath;

  DECLARE_MESSAGE_MAP()
 };


#endif
