#if !defined(_EXPDLG_)
#define _EXPDLG_

#include "stdafx.h"
#include "autorc.hpp"
#include "vs.rh"
#include "fr_opt.hpp"

#include <afxcmn.h>

#include <vector>
using namespace std;


class TWndDrwLock
 {
public:
  TWndDrwLock( CWnd* pW )
   {
     m_pW = pW;
	 m_pW->SetRedraw( FALSE );
   }

  ~TWndDrwLock()
   {
     m_pW->SetRedraw( TRUE );
   }

private:
  CWnd *m_pW;
 };


class TDrive
 {
public:
  TDrive() { m_cLetter = 0; m_uiType = 0; }
  TDrive( char cLetter, UINT uiType )
   {
     m_cLetter = cLetter;
	 m_uiType = uiType;
   }

  char m_cLetter;
  UINT m_uiType;
 };

class TFolderDlg: public CDialog
 {
public:
  TFolderDlg( CWnd* pFram, CString& rstrFolder, bool bFlCurDir, TMFOptions& rOpt ):
    CDialog( IDD_DLG_FOLDER, pFram ),
    m_rFolder( rstrFolder ),
	m_rOpt( rOpt )
	 {
	   m_Folder = rstrFolder;
	   m_mPar = pFram;
	   m_bInitPath = bFlCurDir;
	 }

  virtual ~TFolderDlg();

  virtual BOOL OnInitDialog();
  virtual void DoDataExchange( CDataExchange* pDX );
  virtual void OnOK();

  afx_msg void OnUp();
  afx_msg void OnList();
  afx_msg void OnIco();
  afx_msg void OnDblClk_List( NMHDR * pNotifyStruct, LRESULT * result );   
  afx_msg void OnCBNSelectOK();
  afx_msg HBRUSH OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor );
  

private:
  CString& m_rFolder;
  TMFOptions& m_rOpt;
  CString m_Folder;
  CWnd* m_mPar;
  bool m_bInitPath;

  CEdit m_edPath;
  CListCtrl m_lstFold;
  CButton m_btnUp, m_radList, m_radIco;
  CBitmap m_bmDirUp;
  CComboBox m_cbnDrv;

  int m_iSmallBmpX;

  bool m_bFlEmpty;

  vector<TDrive*> m_lstDrv;

  TAutoMemoryRc<CImageList*> m_ilstNormal, m_ilstSmall;

  void FillList();  
  void FillDrive();
  void ClearDriveList();
  bool CheckPath( bool bFlGetDefault = true );
  void EnterList();
  void EnterEd();

  DECLARE_MESSAGE_MAP()
 };


class TDlgName: public CDialog
 {
public:
  TDlgName( CWnd *pPar, CString& rStr ):     
    CDialog( IDD_DLG_EXPERTNAME, pPar ),
    m_rStr( rStr ) 
   {
     m_mPar = pPar;
	 m_str = rStr;
   }

  virtual BOOL OnInitDialog();
  virtual void DoDataExchange( CDataExchange* pDX );
  virtual void OnOK();
  
private:  
  CString& m_rStr;
  CString m_str;
  CWnd* m_mPar;
 };

#endif
