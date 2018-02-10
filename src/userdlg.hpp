#if !defined(_USER_DLG_)
#define _USER_DLG_

#include "stdafx.h"
#include "fr_opt.hpp"
#include "userlvl.hpp"

#include "..\\RUDAKOW\\SERVIS\\borctl.hpp"

#include <afxcmn.h>
#include <vector>


void LoadUserList_Internal( CString& strPath, vector<TUserLocal*>& m_lstUsers, TMFOptions& m_Opt );
void SaveUserList_Internal( CString& strPath, vector<TUserLocal*>& m_lstUsers, TMFOptions& m_Opt );

class TPRopPageErgo: public CPropertyPage
 {
   DECLARE_DYNCREATE( TPRopPageErgo )

public:
  TPRopPageErgo(): CPropertyPage() 
   {
     
   }

  TPRopPageErgo( UINT uiRc ): CPropertyPage(uiRc) {};

  virtual ~TPRopPageErgo();

  virtual BOOL OnInitDialog();  
  virtual void DoDataExchange( CDataExchange* pDX );
  
  //afx_msg HBRUSH OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor );

  int m_iSave, m_iRestore;  
  
protected:
  DECLARE_MESSAGE_MAP()
  
 };

class TPRopPageAcess: public CPropertyPage
 {
   DECLARE_DYNCREATE( TPRopPageAcess )

public:
  TPRopPageAcess(): CPropertyPage() 
   {
     
   }

  TPRopPageAcess( UINT uiRc ): CPropertyPage(uiRc) {};

  virtual ~TPRopPageAcess();

  virtual BOOL OnInitDialog();  
  virtual void DoDataExchange( CDataExchange* pDX );
  

  int m_iNumbPass;  
  int m_iUse; //checkbox 
  int m_iAlho;
  CString m_strKey;
  
protected:
  CComboBox m_Combo;

  DECLARE_MESSAGE_MAP()  
 };


class TPRopPageUsers: public CPropPageTooltips
 {
   //DECLARE_DYNCREATE( TPRopPageUsers )

public:
  
  TPRopPageUsers( vector<TUserLocal*>& rUs, UINT uiRc );

  virtual ~TPRopPageUsers();

  virtual BOOL OnInitDialog();  
  virtual void DoDataExchange( CDataExchange* pDX );
  
  bool IsDirty() const { return m_bFlDirty; }
  
protected:
  CComboBox m_ComboClass;
  CEdit m_edName, m_edFam, m_edPass;
  CButton m_bbRemove, m_bbUp, m_bbDown, m_bbAdd, m_bbUpdate;
  CBitmap m_bmRemove, m_bmUp, m_bmDown, m_bmAdd, m_bmUpdate;

  CListCtrl m_lstUsers;

  vector<TUserLocal*>& m_rlstUsers; 
  bool m_bFlDirty;    

  void AssClm( CListCtrl& lst );
  void FillList( CListCtrl& lst );

  afx_msg void ChSelect( NMHDR *pNotifyStruct, LRESULT *result );
  void LoadFromField( int iInd );
  bool IsRemovable( TUserLocal* pUs );
  void AddObj( CListCtrl& lst, TUserLocal* pUs, int iItem, bool bFlModify = false );
  void OnAddInternal( bool bFlUpdate );

  afx_msg void OnAdd();
  afx_msg void OnRemove(); 
  afx_msg void OnUp(); 
  afx_msg void OnDown(); 
  afx_msg void OnUpdate();

  DECLARE_MESSAGE_MAP()  
 };


class TUsersDialog: public TBWCCPropertySheet
 {

   //DECLARE_DYNCREATE( TUsersDialog )

public:
  //TUsersDialog(): TBWCCPropertySheet() {};
  TUsersDialog( TMFOptions& rOpt, LPCTSTR pszCaption, CWnd *pParentWnd = NULL, UINT iSelectPage = 0 );    

  virtual ~TUsersDialog();

  afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
  afx_msg void OnOK();

private:  
  TPRopPageErgo  m_pagErgo;    
  TPRopPageUsers m_pagUser;
  TPRopPageAcess m_pagAcess;
 
  vector<TUserLocal*> m_lstUsers; 

  TMFOptions& m_rOpt;
  TMFOptions  m_Opt;

  bool m_bIbit;

  void LoadUserList( CString& );
  void UpdateOpt();
  void SaveUserList();
  void SaveOpt();
  bool IsCodingChanged();
  BOOL UpdateDataMy( CPropertyPage& rP, BOOL bFl );

  void RecodeAllES();
  void RecodeOneFile( LPSTR );  
  void MsgOpStop( LPSTR lpName );

  DECLARE_MESSAGE_MAP()
 };


#endif
