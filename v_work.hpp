#if !defined(_VIEW__WORK_)
#define _VIEW__WORK_

#include "stdafx.h"
#include "autorc.hpp"

#include "docexp.hpp"
#include "vessubj.hpp"
#include "viewexp.hpp"
#include "userlvl.hpp"

#include "credb.hpp"

#include <afxcmn.h>
#include <afxmt.h>

enum TIT_ItemStateProd { IT_Normal, IT_Used, IT_CurrentStep, IT_PrivStep };
enum TVA_ItemStateVar { VA_Normal, VA_NotNeed, VA_NotInit, VA_Canceled };


class TProdList: public CListBox
 {
public:
  TProdList();

  virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
  virtual void MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct );  
  void DrawLbl( CDC& rDC, CRect& r, TIT_ItemStateProd typ );

  afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
  afx_msg BOOL OnEraseBkgnd( CDC *pDC );

  TUserLocal *m_pUsl;

  void UpdateExtent( CString& );
  int m_iExtent;
  int m_iLeft;

private:    
  CBrush m_br;
  CBitmap bmpBr;    

  CPen m_pRed, m_pGreen, m_pRed2;
  TErgoItem* GetItemClr( int iR, CString& rID );

  DECLARE_MESSAGE_MAP()
 };

class TFacList: public CListBox
 {
public:
  TFacList();

  virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
  virtual void MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct );    

  afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
  afx_msg BOOL OnEraseBkgnd( CDC *pDC );

  TUserLocal *m_pUsl;


private:        

  DECLARE_MESSAGE_MAP()
 };

class TVarList: public CListBox
 {
public:
  TVarList();

  virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
  virtual void MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct );    

  afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
  afx_msg BOOL OnEraseBkgnd( CDC *pDC );

  TUserLocal *m_pUsl;
  void DrawLbl( CDC& rDC, CRect& r, TVA_ItemStateVar typ );

  TVA_ItemStateVar GetItemFlag( TVariantVar* pV );

  void SelectVar( TVariantVar* );

private:        
  int m_iLeft;
  CBrush m_br;
  CBitmap bmpBr;
  CPen m_pRed, m_pGreen, m_pRed2;

  DECLARE_MESSAGE_MAP()
 };

class TTreeParam: public CTreeCtrl
 {
public:
  TTreeParam(): CTreeCtrl()
   {
   }

  afx_msg HBRUSH OnCtlColor( CDC*, CWnd*, UINT );

  DECLARE_MESSAGE_MAP()
 };

enum TR_Type { TR_Res_High, TR_Res_Med, TR_Res_Low,
               TR_Fac_High, TR_Fac_Med, TR_Fac_Low,
			   TR_Rul_High, TR_Rul_Med, TR_Rul_Low
 };

class TResult
 {
public:
   TResult( TTreeVarItem* pVt );

   CString m_str;
   TVariantVar m_Val;

   TR_Type m_trLvl;
 };

class TResults
 {
public:
   TResults() {};
   ~TResults();

   void Clear();

   list<TResult*> m_lstRes;
 };


class TKeepVars
 {
public:
  TKeepVars( TNameSpace *pSpc );
  ~TKeepVars();

  void Restore();

private:
  list<TVariantVar*> m_lstVar;
  TNameSpace *m_pSpc; 
 };


class TStopException: public TMyException
 {

  enum { TSC_Subcode = TREG_TStopException }; //! - у каждого субкласса должен быть свой

public:
  TStopException( BOOL bFlDel, int iExcCode );    

  virtual int GetSubCode() const;
 };

class TTreeCont
 {
public:
   TTreeCont() {};
   TTreeCont( list<TTreeVar*>&, TVariantVar* );
   ~TTreeCont();

   list<TTreeVar*> m_lstTrees;
 };

class TWorkView: public TBaseView
 {

friend UINT ThreadProc( LPVOID pObj ); 

  DECLARE_DYNCREATE( TWorkView )
 
public:
  TWorkView();
  
  virtual ~TWorkView();  
  
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

  virtual BOOL PreTranslateMessage( MSG* pMsg );
      
  afx_msg void OnDestroy();  
  
  afx_msg void OnRulOpt();  
  afx_msg void OnRulExit();  

  void Upd_RulExit( CCmdUI* );  	
  void Upd_WAsk( CCmdUI* );  	
  void Upd_WStop( CCmdUI* );  	
  void Upd_WStep( CCmdUI* );  	  
  void Upd_Print( CCmdUI* );

  afx_msg void OnWAsk();
  afx_msg void OnWStop(); 
  afx_msg void OnWStepMode(); 
  afx_msg void OnWAnimate();   
  afx_msg void OnWStep();
  afx_msg void OnWPrint();

  afx_msg LRESULT OnCommandHelp( WPARAM, LPARAM lParam );
  afx_msg LRESULT OnCommandHelpF1( WPARAM, LPARAM lParam );

  afx_msg void OnVarModify();

  afx_msg void OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags );
  afx_msg HBRUSH OnCtlColor( CDC*, CWnd*, UINT );

  virtual void OnDraw( CDC* pDC );
  afx_msg BOOL OnEraseBkgnd( CDC* );  
  
  virtual void OnPrint( CDC* pDC, CPrintInfo* pInfo );
  virtual BOOL OnPreparePrinting( CPrintInfo* pInfo );

  virtual LPCSTR GetShortFileName();
  virtual LPCSTR GetName(); 

  TPro* GetPro() { return (TPro*)m_pBindSubj; }  

  TSubject* Subj() { return (TSubject*)m_pBindSubj; }  

protected:    
  virtual TO_ObjTypes IsObj() const;             

  int m_iDeepRec, m_iDeepRecMax, m_iUsedRules;
  void RefreshUsedRules();
  void RefreshDeep();

  bool m_bFlRemoveUsedRules;
  vector<TResults*> m_lstRes;
  void ClearRes();
  void AddResult( TVariantVar *pV, list<TTreeVar*>& rL );
  TWorkArea *m_pWA;

  int m_iGoal;
  bool m_bFlHaveResult;

  void LetsGo();
  void Go2( list<TSimpleProduct*>&, int&, list<TTreeVar*>& );

  TSimpleProduct *FindVarInPos( TWorkArea *pWA, TVariantVar *pV );
  bool AddAllImplTo( TSimpleProduct *pProd, list<TVariantVar*>& rLst );
  void DetectNeedVars( list<TVariantVar*>& rLstNeed );
  void ResetNeedFlag( TNameSpace* );
  bool FindInNeed( list<TVariantVar*>& rLst, TVariantVar* pV );
  void BuildC( list<TVariantVar*>& rLstNeed );
  bool ScasForCannotUsedVar( TSimpleProduct *pP, list<TTreeVar*>& rVarsTree );
  bool TestVars( TExpression *pE, TVariantVar *pVarNot, list<TTreeVar*>& );

  void CopyUsed( list<TSimpleProduct*>& rDst, list<TSimpleProduct*>& rSrc, 
			     bool bFlRemove, TVariantVar *pV );
    
  void MarkProduct( TSimpleProduct* pP, list<TSimpleProduct*>* pLstUsed );

  void ImplTree( TSimpleProduct* pP, list<TTreeVar*>& rLst );   
  void AddTreeVar( list<TTreeVarItem>& rTo, list<TTreeVarItem>& rFrom );

  void MakeStartTree( list<TTreeVar*>& rL );
  TFactWork *FindInFacts( TVariantVar *pV );

  void ShowPos( int, bool, bool, bool );

  TProdList m_lstProd;
  TFacList m_lstFac;
  TVarList m_lstVar;

  TTreeParam m_tree;

  void FillListFac( TFacList& rLstFac, vector<TFactWork*>& );
  void FillListProd( TProdList& rLstProd, vector<TSimpleProduct*>& );
  void FillListVar( TVarList& rLstVar, TNameSpace* );
  void FillTree( TTreeParam&, vector<TResults*>* pRes = NULL );  

  void ClearLstProStat( list<TSimpleProduct*>* pLst );

  bool m_bFlStepMode;
  bool m_bAnimMode;

  __int64 m_64Wait;

  TAutoMemoryRc<CWinThread*> m_arcTh;
  CMutex m_mtxObjLock;
  CEvent m_evStop, m_evStep;

  TSimpleProduct *m_pPrevProd;

private:        
  CMyDialogBar *m_pHorDlgb;  

  bool m_bRestart;

  void SwitchBackView();    
  int m_iFirstShow;
  void DetectNeedVars();

  CBrush m_brBlue;
  int m_iWBox;

  HTREEITEM m_hCurRecurs, m_hMaxRecurs, m_hUsedRul, m_hGoal;

  TChildItem *pcitLstProd, *m_pchFac, *m_pchVar, *m_pchTtlFac, *m_pchTtlVar;
  TChildItem *m_pchTree;

  TAutoMemoryRc<CImageList*> m_ilstNormal;

  DECLARE_MESSAGE_MAP()
 };


#endif
