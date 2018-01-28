#if !defined(_RIGHTS_DLG_)
#define _RIGHTS_DLG_

#include "stdafx.h"
#include "userlvl.hpp"
#include "userdlg.hpp"
#include "vs.rh"

#include "..\\RUDAKOW\\SERVIS\\borctl.hpp"

#include <list>
#include <vector>
using namespace std;

class TAccDlg: public CDialog
 {
   
public:
  TAccDlg( CWnd* pPar, TProtectedObj* pObj, LPCSTR lpTtl ): 
	CDialog( IDD_DLG_ACC, pPar ),
    m_strTtl( lpTtl )
   {
     m_pObj = pObj; 
   }

  virtual ~TAccDlg();

  virtual BOOL OnInitDialog();  
  virtual void DoDataExchange( CDataExchange* pDX );
  
  afx_msg HBRUSH OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor );
  afx_msg void OnExp();
  afx_msg void OnKog(); 
  afx_msg void OnUs();

  afx_msg void OnAdd(); 
  afx_msg void OnRemove();  
  afx_msg void OnSetOwner(); 

  virtual void OnOK();
  virtual void OnCancel();
  
protected:
  CString m_strTtl;

  TProtectedObj* m_pObj;
  CBitmap m_bmAdd, m_bmRemove, m_bmSetOwner;

  CButton m_bbAddUser, m_bbRemoveUser, m_bbSetOwner;
  CListBox  m_lstObjUs;
  CTreeCtrl m_treeUs; 
  CEdit m_edOwner;
  
  CButton m_chExp_2, m_chKog_2, m_chUs_2;

  vector<TUserLocal*> m_lstUsers; 
  list<TUser*> m_lstUsersExt; 
  void FillObjList();
  void FillTree();
  void FillEdOwn();

  TUser* FindUs( TUserLocal* );

  bool m_bDirty;
  
  DECLARE_MESSAGE_MAP() 
 };

#endif
