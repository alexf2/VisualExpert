#if !defined(_VIEW__RULES_)
#define _VIEW__RULES_

#include "stdafx.h"
#include "autorc.hpp"

#include "docexp.hpp"
#include "vessubj.hpp"
#include "viewexp.hpp"

#include "credb.hpp"

#include <afxdisp.h>

#include "RDO\\font.h"
#include "RDO\\picture.h"
#include "RDO\\msdgridctrl.h"
#include "RDO\\rdc.h"

/*
#include "vs.rh"

#include "stdafx.h"
#include "autorc.hpp"
#include "docexp.hpp"
#include "vessubj.hpp"
#include "viewexp.hpp"

#include "my_chklb.hpp"

#include "vs.rh"

#include <afxcmn.h>
#include <afxcview.h>*/
class TRulesView;
class TRDCOwn: public CStatic
 {
public:
   TRDCOwn( TRulesView* p, CMsDgridCtrl* pGrid ): CStatic() 
	{
      m_p = p;
	  m_pGrid = pGrid;
	}

   void OnValidateRemotedatactl1( short FAR* Action, short FAR* Reserved );
   void OnErrorRemotedatactl1( long Number, BSTR FAR* Description, long Scode, LPCTSTR Source, LPCTSTR HelpFile, long HelpContext, BOOL FAR* CancelDisplay );
   void OnQueryCompletedRemotedatactl1();

   afx_msg HBRUSH OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor );

   DECLARE_EVENTSINK_MAP()
   DECLARE_MESSAGE_MAP()

   TRulesView *m_p;
   CMsDgridCtrl* m_pGrid;
 };

class TRulesView: public TBaseView
 {
  
  DECLARE_DYNCREATE( TRulesView )
friend class TRDCOwn;

public:
  TRulesView();
  
  virtual ~TRulesView();  
  
  virtual TV_Views IsA() const;
  virtual bool CanClose();
  
  virtual void SetupHorizBar( CMyDialogBar& );
  virtual void SetupVerBar( CMyDialogBar& );

  virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
  //virtual void OnDraw( CDC* pDC );  
  //virtual BOOL Create( LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL );
  afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
  afx_msg void OnSize( UINT nType, int cx, int cy );
  virtual void OnUpdate( CView* pSender, LPARAM lHint, CObject* pHint );  
  virtual void OnInitialUpdate();
      
  afx_msg void OnDestroy();  

  afx_msg void OnRulAdd();  
  afx_msg void OnRulDelete();  
  afx_msg void OnRulLabel();  
  afx_msg void OnRulErs();
  afx_msg void OnRulSearch();  
  afx_msg void OnRulOpt();  
  afx_msg void OnRulExit();  

  afx_msg LRESULT OnCommandHelp( WPARAM, LPARAM lParam );
  afx_msg LRESULT OnCommandHelpF1( WPARAM, LPARAM lParam );

  afx_msg void OnShowWindow( BOOL bShow, UINT nStatus );
  int m_iFirstShow;

  virtual void OnDraw( CDC* pDC );
  afx_msg BOOL OnEraseBkgnd( CDC* );  

  virtual LPCSTR GetShortFileName();
  virtual LPCSTR GetName(); 

  TPro* GetPro() { return (TPro*)m_pBindSubj; }
  TFac* GetFac() { return (TFac*)m_pBindSubj; }
  TQuery* GetQuery() { return (TQuery*)m_pBindSubj; }

  TSubject* Subj() { return (TSubject*)m_pBindSubj; }

  bool m_bRetGrid;

  bool m_bCancelErr;

protected:    
  virtual TO_ObjTypes IsObj() const;

  TAutoMemoryRc<CRdc*> m_arcRDC;
  TAutoMemoryRc<CMsDgridCtrl*> m_arcGrid;
  TAutoMemoryRc<TRDCOwn*> m_arcRDCOwner;

  TTempDSN m_DSN;

  
  void OnSelChangeDbgrid1( short FAR* Cancel ); 
  void OnChangeDbgrid1();
  void OnScrollDbgrid1( short FAR* Cancel );
  void OnRowColChangeDbgrid1( VARIANT FAR* LastRow, short LastCol );

  void OnAfterInsertDbgrid1();
  void OnOnAddNewDbgrid1();
  void OnAfterColEditDbgrid1(short ColIndex);
  
  void OnBeforeUpdateDbgrid1(short FAR* Cancel);
  void OnBeforeColUpdateDbgrid1(short ColIndex, VARIANT FAR* OldValue, short FAR* Cancel);
  void OnBeforeColEditDbgrid1(short ColIndex, short KeyAscii, short FAR* Cancel);
  bool m_bErrRDC;

  void OnAfterUpdateDbgrid1();
  void OnBeforeDeleteDbgrid1( short FAR* Cancel );
  int m_iKeyPos;
  CString m_strKey;
  long m_ColIndex;
  bool m_bEdit;  

  bool m_bDirty;

  long m_lCountID;
  void InitID();
  

  void OnAfterDeleteDbgrid1();  

  virtual BOOL PreTranslateMessage( MSG* pMsg );

  DECLARE_EVENTSINK_MAP()

private:      
  CMyDialogBar *m_pHorDlgb;  

  void SwitchBackView();
  bool MyPreclose();

  CStatic m_statGrp;
  TChildItem *pcitGrid, *pcitGrp;

  bool m_bTransPending;
  int m_iSh;
  int m_iHighS;
  int m_iMinX, m_iMinY;
  bool m_bRedr;

  void CheckRule();

  void CheckFac();		
  void CheckQ();		
  void CheckPro();

  CString strC1, strC2, strC3, strC4;  

  DECLARE_MESSAGE_MAP()
 };


#endif
