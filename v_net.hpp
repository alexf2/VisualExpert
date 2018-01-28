#if !defined(_VIEW__NET_)
#define _VIEW__NET_

#include "stdafx.h"
#include "autorc.hpp"

#include "docexp.hpp"
#include "vessubj.hpp"
#include "viewexp.hpp"

#include <list>
#include "Net.h"	// Added by ClassView
using namespace std;

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

enum NetCursorState {
    zoomin,
    zoomout,
    move,
    del,
    addnode,
    addline
};

struct spathinfo {
    __int32 p[2];
};

class TNetView: public TBaseView
 {
  
  DECLARE_DYNCREATE( TNetView )

public:
     CArray<spathinfo, spathinfo&> path;
     void AddChilds(__int32 n);
	 void SolveQuestion(__int32 l, __int32 n);
	 void RedrawLine( __int32 i );
	 CRect GetLineNameRect( __int32 iLine );
	void CreateRectFromNode(__int32 iNode, CRect* rect );
	 LPRECT oldnodepos;
	 void FillNodeRgn( __int32 iNode, CRgn* rgn );
	 __int32 iLineSelected;
	 __int32 lineend;
	 bool linedrawing;
	 CPoint MenuMousePos;
	 CMenu* popupmenu;
	 CMenu* popupmenu1;
	 __int32 iNodeSelected;
	 NetCursorState cursorstate;
	 CRect DRtoLR( CRect r );
	 CPoint DPtoLP( CPoint p );
	 CRect LRtoDR( CRect r );
	 CPoint LPtoDP( CPoint p );
	 CPoint CaptureStartPoint;
	 bool nodemoving;
  CNet* net;
  TNetView();
  
  virtual ~TNetView();  
  
  virtual TV_Views IsA() const;
  virtual bool CanClose();
  
  virtual void SetupHorizBar( CMyDialogBar& );
  virtual void SetupVerBar( CMyDialogBar& );

  virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
  //virtual void OnDraw( CDC* pDC );  
  //virtual BOOL Create( LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL );
  afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
  virtual void OnUpdate( CView* pSender, LPARAM lHint, CObject* pHint );  
  virtual void OnInitialUpdate();
      
  afx_msg void OnDestroy();  
  afx_msg BOOL OnEraseBkgnd( CDC* );  

  afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
  afx_msg void OnLButtonUp( UINT, CPoint );
  afx_msg void OnRButtonDown( UINT nFlags, CPoint point );
  afx_msg void OnMouseMove( UINT nFlags, CPoint point );
  afx_msg void OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );
  afx_msg void OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );


  afx_msg void OnRulOpt(); 
  afx_msg void OnNetSave(); 
  afx_msg void OnNetZoomPl(); 
  afx_msg void OnNetZoomMi(); 
  afx_msg void OnNetNewNode(); 
  afx_msg void OnNetNewAdge(); 
  afx_msg void OnNetErase(); 
  afx_msg void OnNetMove(); 
  afx_msg void OnNetSelect();
  
  afx_msg void OnMenuNew(); 
  afx_msg void OnMenuNewLine(); 
  afx_msg void OnMenuName();
  afx_msg void OnMenuDelete();
  afx_msg void OnMenuProp();
  afx_msg void OnMenuInh();
  afx_msg void OnMenuTrans();
  afx_msg void OnMenuQuestion();

  afx_msg void OnRulExit();  

  afx_msg LRESULT OnCommandHelp( WPARAM, LPARAM lParam );
  afx_msg LRESULT OnCommandHelpF1( WPARAM, LPARAM lParam );

  void SetupMapMode( CDC* pDC );

  virtual void OnDraw( CDC* pDC );

  virtual LPCSTR GetShortFileName();
  virtual LPCSTR GetName(); 

protected:    
  virtual TO_ObjTypes IsObj() const;

  CString filename;

private:      
	 CRect n( CRect r );
  int m_shiftX, m_shiftY;
  double m_dScale;

  TAutoMemoryRc<CClientDC*> m_arcDC;

  //list<CPoint> m_lst;

  DECLARE_MESSAGE_MAP()
 };


#endif
