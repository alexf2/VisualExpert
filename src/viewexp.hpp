#if !defined(_VIEWBASE_)
#define _VIEWBASE_

#include "stdafx.h"
#include "autorc.hpp"
#include "docexp.hpp"
#include "vessubj.hpp"

#include "vs.rh"

#include <list>
using namespace std;

enum CH_Relation { CH_Top };


struct TChildItem
 {
   enum TS_Types { TS_RelationFrom, TS_RelationParent };

   TChildItem() { m_pCh = NULL; }
   TChildItem( CWnd *pCh,  CH_Relation rel, CWnd *pFrom )
	{
      m_pCh = pCh; m_pFrom = pFrom; m_rel = rel;

	  m_ts = TS_RelationFrom;
	}

   
   TChildItem( CWnd *pCh, double left, double top, double W, double H )
	{
      m_pCh = pCh;
	  m_left = left; m_top = top;
	  m_W = W; m_H = H;

	  m_ts = TS_RelationParent;
	}

   TS_Types m_ts;

   CWnd *m_pCh;
   
	struct {      
      double m_left, m_top; //if <= 1 then relative else absolute
      double m_W, m_H;
	 };
	struct {
      CWnd *m_pFrom;
	  CH_Relation m_rel;
	 };
 };

enum TV_Views { TV_Expert, TV_RulesEdit, TV_FactEdit, TV_QueryEdit,
                TV_Consult, TV_NetsEdit, TV_FrameEdit };

enum TO_ObjTypes { TO_ExpertSys, TO_Rules, TO_Facts, TO_Query, 
                   TO_Net, TO_Frames };

class CMyDialogBar;

enum TB_BtnTypes { TB_Bmp, TB_Normal, TB_EndDescr };
struct TBtnTool
 {
   TB_BtnTypes m_typ;
   int m_iIDCmd;      

   LPSTR m_lpTtl;
   int m_iIdBmp;

   bool m_bShiftMode;

   LPSTR m_lpHint;

   DWORD m_dwExtraStyle;
 };

void CreateBtns( CMyDialogBar& rB, TBtnTool* pDescr, bool bFlTxtAln, 
				 list<CBitmap*>& rLst, list<CWnd*>& rLstBtn );


class TBaseView: public CView
 {

friend class TMDIFrame;

protected: // create from serialization only
  TBaseView();
  DECLARE_DYNCREATE( TBaseView )

public:
  
  virtual ~TBaseView();

  TExpertDoc* GetDocument()
   {
     return static_cast<TExpertDoc*>(m_pDocument);
   }

  TBaseView& operator+( TChildItem* pCh )
   {
     m_lstCh.push_back( pCh );
	 return *this;
   }
  
//__________ don't call in subclass __________
  virtual TV_Views IsA() const; //use as pure virtual
  virtual bool CanClose(); //use as pure virtual
  void MakeBindSubj( TProtectedObj *pObj )
   {
     m_pBindSubj = pObj;
   }
  virtual void SetupHorizBar( CMyDialogBar& ); //use as pure virtual
  virtual void SetupVerBar( CMyDialogBar& ); //use as pure virtual

  virtual void OnDraw( CDC* pDC ); //use as pure virtual
//____________________________________________

  //---------------
  virtual void OnActivateView( BOOL bActivate, CView* pActivateView, CView* pDeactiveView );
  virtual void OnActivateFrame( UINT nState, CFrameWnd* pFrameWnd );
  BOOL SaveFocusControl();
  afx_msg void OnSetFocus( CWnd* pOldWnd );
  virtual BOOL PreTranslateMessage( MSG* pMsg );
  //---------------


  virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
  //virtual void OnDraw( CDC* pDC );  
  //virtual BOOL Create( LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL );
  afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
  virtual void OnUpdate( CView* pSender, LPARAM lHint, CObject* pHint );  
  virtual void OnInitialUpdate();

  virtual BOOL OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult );
  virtual BOOL OnCommand( WPARAM wParam, LPARAM lParam );

  afx_msg void OnNcCalcSize( BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp );
  afx_msg void OnNcPaint();

  afx_msg void OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );
  afx_msg void OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );

  afx_msg LONG OnGlobalQPal( UINT, LONG );
  afx_msg LONG OnGlobalPalCh( UINT, LONG );

  afx_msg void OnSize( UINT nType, int cx, int cy );
  void MakeLaoyt( bool bFlRedr = false );

  void OnUpdate_Always( CCmdUI* ); 
  
  //afx_msg void OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );
  //afx_msg void OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar ); 

  //virtual BOOL DestroyWindow();
  afx_msg void OnDestroy();
  
  //afx_msg void OnMouseMove( UINT nFlags, CPoint point );

  afx_msg BOOL OnEraseBkgnd( CDC* );  

  void Notify_OwnerCreate()
   {
     m_bFlOwnerCreate = true;
   }

  virtual bool ShadowIsOn() const;

  virtual LPCSTR GetShortFileName();
  virtual LPCSTR GetName();

  void SetMinWH( int W, int H )
   {
     m_iMinW = W; m_iMinH = H;
   }

  
protected:
  TProtectedObj *m_pBindSubj;
  virtual TO_ObjTypes IsObj() const;
  bool m_bFlOwnerCreate;

  bool m_bShadow;

  list<CBitmap*> m_lstBmp;
  list<CWnd*> m_lstBtn;
  void ClearBtnsLists();

  HWND m_hWndFocus;
  HWND m_hWndFocusCTL;

  CString m_strTmpName;

  void DisableScroll()
   {
     m_bFlScrollEnable = false;
   }
  
private:    
  int m_iMinW, m_iMinH;
  bool m_bFlScrollEnable;

  static CPalette m_palShadow;   
  static int m_iCntUsePal;
  static TAutoMemoryRc_AR<BYTE*> m_arcPal;

  list<TChildItem*> m_lstCh;  
  bool m_bHScr, m_bVScr;
  int m_sX_, m_sY_;

  DECLARE_MESSAGE_MAP()
 };


#endif
