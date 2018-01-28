#if !defined(_VIEW__EXPERT_)
#define _VIEW__EXPERT_

#include "stdafx.h"
#include "autorc.hpp"
#include "docexp.hpp"
#include "vessubj.hpp"
#include "viewexp.hpp"

#include "my_chklb.hpp"

#include "vs.rh"

#include <afxcmn.h>
#include <afxcview.h>


class TExpertView: public TBaseView
 {
  
  DECLARE_DYNCREATE( TExpertView )

public:
  TExpertView();
  
  virtual ~TExpertView();  
  
  virtual TV_Views IsA() const;
  virtual bool CanClose();
  
  virtual void SetupHorizBar( CMyDialogBar& );
  virtual void SetupVerBar( CMyDialogBar& );

  //virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
  //virtual void OnDraw( CDC* pDC );  
  //virtual BOOL Create( LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL );
  afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
  virtual void OnUpdate( CView* pSender, LPARAM lHint, CObject* pHint );  
  virtual void OnInitialUpdate();

  afx_msg HBRUSH OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor );


  afx_msg LRESULT OnCommandHelp( WPARAM, LPARAM lParam );
  afx_msg LRESULT OnCommandHelpF1( WPARAM, LPARAM lParam );
  
  void OnUpdate_Acc( CCmdUI* ); 
  void OnUpdate_Remove( CCmdUI* ); 
  void OnUpdate_Edit( CCmdUI* ); 
  void OnUpdate_Clear( CCmdUI* ); 
  void OnUpdate_ClearAll( CCmdUI* ); 
  void OnUpdate_ToRules( CCmdUI* ); 
  void OnUpdate_Compile( CCmdUI* ); 
  void OnUpdate_Consult( CCmdUI* ); 

  afx_msg void OnNewObj();
  afx_msg void OnImport();
  afx_msg void OnRemove(); 

  afx_msg void OnEdit(); 
  afx_msg void OnClear(); 
  afx_msg void OnClearAll(); 

  afx_msg void OnAcc(); 

  afx_msg void OnMakeRules(); 
  afx_msg void OnCompile(); 

  afx_msg void OnConsult(); 
  
  
  //afx_msg void OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );
  //afx_msg void OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar ); 

  //virtual BOOL DestroyWindow();
  afx_msg void OnDestroy();

  virtual void OnDraw( CDC* pDC );

  virtual LPCSTR GetShortFileName();
  virtual LPCSTR GetName();
  
  //afx_msg void OnMouseMove( UINT nFlags, CPoint point );

protected:    
  virtual TO_ObjTypes IsObj() const;

private:
  TMyChLBox m_chkNets, m_chkFrames;
  CTreeCtrl m_treeRules;
  CStatic m_ttlTree, m_ttlNets, m_ttlFrame;
  CString m_strKeepName;

  void ExpandTree( TSubject* );
  void RollList( TMyChLBox&, TSubject* );

  void FillRul( DWORD dwSel = -1 );
  void FillNets();
  void FillFrn();  

  TAutoMemoryRc<CImageList*> m_ilstNormal;

  bool CheckExist( LPCSTR strName );
  afx_msg void TreeSelect( NMHDR *pNotifyStruct, LRESULT *result );

  BOOL CheckForNOItem();
  int GetFName( LPCSTR lpTtl, LPCSTR lpDefExt, LPCSTR lpTypesStr, CString& rName );
  int GetFName2( CString& csTtl, LPCSTR lpDefExt, LPCSTR lpTypesStr, CString& rName );
  TSubject* GetSelectedObj();

  bool ValidateAccess( TSubject* pSubj );

  int AskLevel( TSubject *pSubj );

  void ClearSelectLst( TMyChLBox *pBox );
  void ClearSelectTree();
  void MakeEditSubj( TSubject* pSubj );  

  DECLARE_MESSAGE_MAP()
 };


#endif
