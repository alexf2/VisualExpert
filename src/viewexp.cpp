#include "viewexp.hpp"
#include <afxcmn.h>
#include "mdiexp.hpp"
#include "mfrexp.hpp"

#include "vs.rh"

#include "globlmsg.hpp"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE( TBaseView, CView )

BEGIN_MESSAGE_MAP( TBaseView, CView )	
  ON_WM_CREATE()
  ON_WM_NCCALCSIZE()
  ON_WM_NCPAINT()

  ON_WM_ERASEBKGND()

  ON_WM_HSCROLL()
  ON_WM_VSCROLL()  

  ON_REGISTERED_MESSAGE( globl_iMsgQNewPal, OnGlobalQPal )
  ON_REGISTERED_MESSAGE( globl_iMsgPalChanged, OnGlobalPalCh )  

  ON_WM_SIZE()
  ON_WM_SETFOCUS()

  ON_WM_DESTROY()

END_MESSAGE_MAP()

TBaseView::TBaseView(): CView()
 {      
   m_pBindSubj = NULL;
   m_bFlOwnerCreate = false;
   m_bShadow = false;

   m_iMinW = m_iMinH = 100;

   m_hWndFocusCTL = m_hWndFocus = NULL;

   m_bHScr = m_bVScr = false;
   m_sX_ = m_sY_ = 0;

   m_bFlScrollEnable = true;
 }

TBaseView::~TBaseView()
 {   
   ClearBtnsLists();

   list<TChildItem*>::iterator it( m_lstCh.begin() );
   list<TChildItem*>::iterator itEnd( m_lstCh.end() );

   for( ; it != itEnd; ++it )
	  delete *it;

   m_lstCh.clear();
 }

LONG TBaseView::OnGlobalPalCh( UINT lPtrRes, LONG lPtr )
 {
   if( ShadowIsOn() == false ) return 0;

   if( (CWnd*)lPtr != this ) 
	{
	  OnGlobalQPal( 0, 0 );
	  *(BOOL*)lPtrRes = TRUE;
	}
   return 0;
 }

LONG TBaseView::OnGlobalQPal( UINT, LONG lPtr )
 {
   if( ShadowIsOn() == false ) 
	{
      if( lPtr ) *(BOOL*)lPtr = 0;
	  return 0;
	}

   HPALETTE HpalOld;
   int i;
   HPALETTE hp = TBaseView::m_palShadow;
					  //TPalette
   if( !hp ) i = 0;
   else
	{
	  HDC hdc = ::GetDC( m_hWnd );
	  HpalOld = ::SelectPalette( hdc, hp, FALSE );
	
	  i = ::RealizePalette( hdc );

	  ::SelectPalette( hdc, HpalOld, FALSE );

	  if( i > 0 )
	   InvalidateRect( NULL, TRUE );	

	  ::ReleaseDC( m_hWnd, hdc );
	}

   if( lPtr ) *(BOOL*)lPtr = i;

   return 0;
 }

/*
void TBaseView::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
 {
   if ( pScrollBar->IsKindOf( RUNTIME_CLASS( CScrollBar ) ))
    {
      CView::OnHScroll( nSBCode, nPos, pScrollBar );
    }
    else if ( pScrollBar->IsKindOf( RUNTIME_CLASS( CSliderCtrl ) ))
    {
      CWnd::OnHScroll( nSBCode, nPos, pScrollBar );
    }
    else if ( pScrollBar->IsKindOf( RUNTIME_CLASS( CSpinButtonCtrl ) ))
    {
      CWnd::OnHScroll( nSBCode, nPos, pScrollBar );
    }
 }

void TBaseView::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
 {
   if ( pScrollBar->IsKindOf( RUNTIME_CLASS( CScrollBar ) ))
    {
      CView::OnVScroll( nSBCode, nPos, pScrollBar );
    }
    else if ( pScrollBar->IsKindOf( RUNTIME_CLASS( CSliderCtrl ) ))
    {
      CWnd::OnVScroll( nSBCode, nPos, pScrollBar );
    }
    else if ( pScrollBar->IsKindOf( RUNTIME_CLASS( CSpinButtonCtrl ) ))
    {
      CWnd::OnVScroll( nSBCode, nPos, pScrollBar );
    }
 }*/

BOOL TBaseView::OnEraseBkgnd( CDC *pDC )
 {
   //pDC->SetMapMode( MM_TEXT );
   //pDC->SetViewportOrg( 0, 0 );   

   CRect r; GetClientRect( r ); 
   pDC->FillSolidRect( r, RGB(255,255,255) );

 
   return TRUE;
   //return CView::OnEraseBkgnd( pDC );
 }

void TBaseView::OnUpdate( CView* pSender, LPARAM lHint, CObject* pHint )
 {
   // ...
   // Implement a GetDocSize( ) member function in 
   // your document class; it returns a CSize.
   //SetScrollSizes( MM_TEXT, ((TGrafDoc*)GetDocument())->GetDocSize( ) );
   //ResizeParentToFit( );   // Default bShrinkOnly argument
   // ...
   CView::OnUpdate( pSender, lHint, pHint );
 }

bool TBaseView::ShadowIsOn() const
 {   
   return m_bShadow && 
	 ((TMainFrame*)(AfxGetApp()->m_pMainWnd))->GetCurUser()->m_bUseSpecialDesign;
 }

void TBaseView::OnNcCalcSize( BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp )
 {
   CView::OnNcCalcSize( bCalcValidRects, lpncsp );
   if( ShadowIsOn() == false ) return;

   //CRect r; GetWindowRect( r );
   //GetParent()->ScreenToClient( r );

   lpncsp->rgrc[0].left += 2.5*GetSystemMetrics( SM_CXDLGFRAME );
   lpncsp->rgrc[0].top += 2.5*GetSystemMetrics( SM_CYDLGFRAME );
      
   //lpncsp->rgrc[0].right -= GetSystemMetrics( SM_CXFRAME );
   //lpncsp->rgrc[0].bottom -= GetSystemMetrics( SM_CYFRAME );   	
 }

CPalette TBaseView::m_palShadow;
int TBaseView::m_iCntUsePal = 0;
TAutoMemoryRc_AR<BYTE*> TBaseView::m_arcPal;

void TBaseView::OnNcPaint()
 {
#define COLOR_STA  140
#define COLOR_END  255

   CView::OnNcPaint();            
   if( ShadowIsOn() == false ) return;

   CRect r;  GetClientRect( r ); ClientToScreen( r );
   CRect rW; GetWindowRect( rW );
   int iWWin = rW.Width();
   int iHWin = rW.Height();

   CWindowDC dc( this );      

   int iW = r.left - rW.left + 1;
   int iH = r.top - rW.top + 1;      

   if( TBaseView::m_iCntUsePal < 1 )
	{
      TBaseView::m_iCntUsePal = 1;  

      int iCnt = max( iW, iH );
	  m_arcPal = 
	    new BYTE[ sizeof(LOGPALETTE) + sizeof(PALETTEENTRY)*iCnt ];
      LOGPALETTE *lL = (LOGPALETTE*)m_arcPal();
	  lL->palVersion = 0x300;
	  lL->palNumEntries = iCnt;

	  PALETTEENTRY *pPal = lL->palPalEntry;
	  int iColor = COLOR_STA, iCStep = (COLOR_END - COLOR_STA) / iCnt;
	  for( int i = 0; i < iCnt; i++, iColor += iCStep, pPal++ )
	   {
	     pPal->peRed = pPal->peGreen = pPal->peBlue = iColor;		 
		 pPal->peFlags = 0;
	   }				

	  m_palShadow.CreatePalette( (LPLOGPALETTE)m_arcPal() );

	  CPalette *pPalOld = dc.SelectPalette( &m_palShadow, FALSE );	
	  dc.RealizePalette();	  
	  dc.SelectPalette( pPalOld, FALSE );
	}

   CPalette *pPalOld = dc.SelectPalette( &m_palShadow, FALSE );	

   PALETTEENTRY *pPal = ((LOGPALETTE*)m_arcPal())->palPalEntry;
   CPen *pOldPen = (CPen*)dc.SelectStockObject( BLACK_PEN );
   iHWin -= GetSystemMetrics( SM_CYBORDER  ) + 1;
   for( int i = 0; i < iW; i++, pPal++ )
	{
      CPen pen( PS_SOLID, 0, RGB(pPal->peRed, pPal->peGreen, pPal->peBlue) );
	  dc.SelectObject( &pen );
      dc.MoveTo( i, i );
	  dc.LineTo( i, iHWin );
	  dc.SelectObject( pOldPen );
	}

   pPal = ((LOGPALETTE*)m_arcPal())->palPalEntry;
   iWWin -= GetSystemMetrics( SM_CXBORDER ) + 1;
   for( i = 0; i < iH; i++, pPal++ )
	{
      CPen pen( PS_SOLID, 0, RGB(pPal->peRed, pPal->peGreen, pPal->peBlue) );
	  dc.SelectObject( &pen );
      dc.MoveTo( i, i );
	  dc.LineTo( iWWin, i );
	  dc.SelectObject( pOldPen );
	} 
   
   dc.SelectObject( pOldPen );
   dc.SelectPalette( pPalOld, FALSE );
 }

/*BOOL TBaseView::DestroyWindow()
 {   
   return CView::DestroyWindow();
 }*/

void TBaseView::OnDestroy()
 {
   CView::OnDestroy();
 }

int TBaseView::OnCreate( LPCREATESTRUCT lpCreateStruct )
 {
   if( CView::OnCreate(lpCreateStruct) == -1 )
     return -1;
    
   //arcDC = new CClientDC( this );
   //m_uiTimer = SetTimer( 1, 50, NULL );
	/*CScrollBar *pScrollV = GetScrollBarCtrl( SB_VERT );
	CScrollBar *pScrollH = AfxGetMainWnd()->GetScrollBarCtrl( SB_HORZ );
	m_iIdScrollH = pScrollH->GetDlgCtrlID();
	m_iIdScrollV = pScrollV->GetDlgCtrlID();*/	  	
	  
	//int nMinPos, int nMaxPos, BOOL bRedraw = TRUE );

   CClientDC dc( this );
   m_bShadow = dc.GetDeviceCaps( BITSPIXEL ) > 8;
   
   return 0; 
 }

TO_ObjTypes TBaseView::IsObj() const
 {
   return TO_ExpertSys;
 }

LPCSTR TBaseView::GetShortFileName()
 {
   return "Пусто";
 }
LPCSTR TBaseView::GetName()
 {
   return "Пусто";
 }

void TBaseView::OnInitialUpdate()
 {
   CView::OnInitialUpdate();   
 }

inline double ConvertXY( double dVal, int iBound )
 {
   return  dVal > 1 ? dVal:double(iBound) * dVal;
 }


void TBaseView::MakeLaoyt( bool bFlRedr )
 {
   UINT uiRedr = (bFlRedr == false ? SWP_NOREDRAW:0);
   int iHD = 0, iVD = 0;

   if( m_bFlScrollEnable == true )
	{
      if( m_bHScr == true ) iHD = GetScrollPos( SB_HORZ );
      if( m_bVScr == true ) iVD = GetScrollPos( SB_VERT );
	}

   list<TChildItem*>::iterator it( m_lstCh.begin() );
   list<TChildItem*>::iterator itEnd( m_lstCh.end() );

   CRect r; GetClientRect( r );

   if( r.right < m_iMinW ) r.right = m_iMinW;
   if( r.bottom < m_iMinH ) r.bottom = m_iMinH;


   for( ; it != itEnd; ++it )
	if( (*it)->m_ts == TChildItem::TS_RelationParent )
	   (*it)->m_pCh->SetWindowPos( NULL, 
	     ConvertXY( (*it)->m_left, r.Width() ) - iHD, 
	     ConvertXY( (*it)->m_top, r.Height() ) - iVD,
	     ConvertXY( (*it)->m_W, r.Width() ),
	     ConvertXY( (*it)->m_H, r.Height() ),
		 SWP_NOZORDER|SWP_NOACTIVATE|uiRedr );


   it = m_lstCh.begin();
   itEnd = m_lstCh.end();
   for( ; it != itEnd; ++it )
	if( (*it)->m_ts == TChildItem::TS_RelationFrom )
	  {
        CRect rr; (*it)->m_pFrom->GetWindowRect( rr );
		ScreenToClient( rr );

		CRect rSelf; (*it)->m_pCh->GetWindowRect( rSelf );
		//ScreenToClient( rSelf );

		if( (*it)->m_rel == CH_Top )
		  (*it)->m_pCh->SetWindowPos( NULL, 
		    rr.left, 
			rr.top - rSelf.Height() - 1, 0, 0,
			SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE|uiRedr );
	  }
 }

void TBaseView::OnSize( UINT nType, int cx, int cy )
 {
   if( nType == SIZE_MINIMIZED ) 
	{
      CView::OnSize( nType, cx, cy );
	  return;
	}

   //if( cx < m_iMinW ) cx = m_iMinW;
   //if( cy < m_iMinH ) cy = m_iMinH;	
   CView::OnSize( nType, cx, cy );

   //CRect r; GetClientRect( r );
   //cx = r.Width(), cy = r.Height();

   MakeLaoyt();   

   if( m_bFlScrollEnable == false ) return;

   int iMaxX = 0, iMaxY = 0;
   int iMinX = 65535, iMinY = 65535;
   CWnd* pCh = GetWindow( GW_CHILD );
   if( pCh != NULL )
		pCh = pCh->GetWindow( GW_HWNDLAST );
	while( pCh )
	 {
	   CRect r; pCh->GetWindowRect( r ); ScreenToClient( r );
       iMaxX = max( iMaxX, r.right );
	   iMaxY = max( iMaxY, r.bottom );

	   iMinX = min( iMinX, r.left );
	   iMinY = min( iMinY, r.top );

	   pCh = pCh->GetWindow( GW_HWNDPREV );
	 } 

   iMaxX = abs( iMaxX - iMinX );
   iMaxY = abs( iMaxY - iMinY );


   DWORD dwRem = 0, dwAdd = 0;
   if( iMaxX-2 > cx && m_bHScr == false ) dwAdd |= WS_HSCROLL, m_bHScr = true, m_sX_ = 0;
   if( iMaxX-2 < cx && m_bHScr == true ) dwRem |= WS_HSCROLL, m_bHScr = false;

   if( iMaxY-2 > cy && m_bVScr == false ) dwAdd |= WS_VSCROLL, m_bVScr = true, m_sY_ = 0;
   if( iMaxY-2 < cy && m_bVScr == true ) dwRem |= WS_VSCROLL, m_bVScr = false;

   if( dwRem || dwAdd ) ModifyStyle( dwRem, dwAdd );
     
   if( m_bHScr == true )
	 SetScrollRange( SB_HORZ, 0, iMaxX - cx + 8, FALSE ); 
   if( m_bVScr == true )
	 SetScrollRange( SB_VERT, 0, iMaxY - cy + 8, FALSE ); 	 

   if( nType == SIZE_MAXIMIZED ) 
	{
	  CWnd* pCh = GetWindow( GW_CHILD );
	  if( pCh != NULL ) pCh = pCh->GetWindow( GW_HWNDLAST );
	  while( pCh )
	   {
		 pCh->InvalidateRect( NULL ); 
		 pCh = pCh->GetWindow( GW_HWNDPREV );
	   } 
	  InvalidateRect( NULL ); 
	}
 }

void TBaseView::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
 {
   if( m_bFlScrollEnable == false ) 
	{
	  CView::OnHScroll( nSBCode, nPos, pScrollBar );
	  return;
	}

   CRect r;
   int iDelta = 0;

   switch( nSBCode )
	{
	  case SB_LEFT:
	   m_sX_ -= 1;
       iDelta = -1;
	   break;

      case SB_ENDSCROLL:
	   break;

	  case SB_LINELEFT:
	   m_sX_ -= 10;
	   iDelta = -10;
	   break;

	  case SB_LINERIGHT:
	   m_sX_ += 10;
	   iDelta = 10;
	   break;

	  case SB_PAGELEFT:
	   GetClientRect( r );
	   m_sX_ -= r.right;
	   iDelta = -r.bottom;
	   break;

	  case SB_PAGERIGHT:
	   GetClientRect( r );
	   m_sX_ += r.right;
	   iDelta = r.bottom;
	   break;

      case SB_RIGHT:
	   m_sX_ += 1;
	   iDelta = 1;
	   break;

      case SB_THUMBPOSITION:
	   m_sX_ = nPos;
	   iDelta = -(GetScrollPos( SB_HORZ ) - nPos);
	   break;

      case SB_THUMBTRACK:
	   break;
	}   
   
   //CView::OnHScroll( nSBCode, nPos, pScrollBar );
   SetScrollPos( SB_HORZ, GetScrollPos(SB_HORZ) + iDelta, TRUE );
   MakeLaoyt( true );

   CWnd* pCh = GetWindow( GW_CHILD );
   if( pCh != NULL ) pCh = pCh->GetWindow( GW_HWNDLAST );
   while( pCh )
	{
	  pCh->InvalidateRect( NULL ); 
	  pCh = pCh->GetWindow( GW_HWNDPREV );
	} 
   InvalidateRect( NULL ); 
 }

void TBaseView::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
 {
   if( m_bFlScrollEnable == false ) 
	{
	  CView::OnVScroll( nSBCode, nPos, pScrollBar );
	  return;
	}

   CRect r;
   int iDelta = 0;

   switch( nSBCode )
	{
	case SB_LEFT:
	   iDelta = -1;
	   m_sY_ -= 1;	   
	   break;

      case SB_ENDSCROLL:
	   break;

	  case SB_LINELEFT:
	   m_sY_ -= 10;
	   iDelta = -10;
	   break;

	  case SB_LINERIGHT:
	   m_sY_ += 10;
	   iDelta = 10;
	   break;

	  case SB_PAGELEFT:
	   GetClientRect( r );
	   m_sY_ -= r.bottom;
	   iDelta = -r.bottom;
	   break;

	  case SB_PAGERIGHT:
	   GetClientRect( r );
	   m_sY_ += r.bottom;
       iDelta = r.bottom;
	   break;

      case SB_RIGHT:
	   m_sY_ += 1;
	   iDelta = 1;
	   break;

      case SB_THUMBPOSITION:
	   m_sY_ = nPos;
	   iDelta = -(GetScrollPos( SB_VERT ) - nPos);
	   break;

      case SB_THUMBTRACK:
	   break;
	}
      
   SetScrollPos( SB_VERT, GetScrollPos(SB_VERT) + iDelta, TRUE );
   MakeLaoyt( true );
   
   CWnd* pCh = GetWindow( GW_CHILD );
   if( pCh != NULL ) pCh = pCh->GetWindow( GW_HWNDLAST );
   while( pCh )
	{
	  pCh->InvalidateRect( NULL ); 
	  pCh = pCh->GetWindow( GW_HWNDPREV );
	} 
   InvalidateRect( NULL ); 
 }



BOOL TBaseView::PreCreateWindow( CREATESTRUCT& cs )
 {      
   BOOL bRes = CView::PreCreateWindow( cs );
   if( !bRes ) return 0;
   /*static CString str;
   str =
	AfxRegisterWndClass(CS_OWNDC | CS_HREDRAW | CS_VREDRAW, 
	  LoadCursor(NULL, IDC_ARROW), 
	  (HBRUSH)(COLOR_WINDOW + 1)); */	    

   cs.style |= WS_CLIPCHILDREN|WS_BORDER;

   return TRUE;
 }

TV_Views TBaseView::IsA() const //use as pure virtual
 {
   return (TV_Views)0;
 }
bool TBaseView::CanClose() //use as pure virtual
 {
   return 0;
 }
void TBaseView::SetupHorizBar( CMyDialogBar& ) //use as pure virtual
 {
 }
void TBaseView::SetupVerBar( CMyDialogBar& ) //use as pure virtual
 {
 }
void TBaseView::OnDraw( CDC* pDC ) //use as pure virtual
 {
 }


void CreateBtns( CMyDialogBar& rB, TBtnTool* pDescr, bool bFlTxtAln, 
				 list<CBitmap*>& rLst, list<CWnd*>& rLstBtn )
 {
   TBtnTool *p = pDescr;

   int iMaxBoundX = 0, iMaxBoundY = 0;

   CClientDC dc( (CWnd*)&rB );
   int iTxtMaxW = 0;
   CSize szAdd = dc.GetTextExtent( CString("A") );
   for( ; p->m_typ != TB_EndDescr; p++ )
	 if( p->m_typ == TB_Normal )
	  {
        CSize sz = dc.GetTextExtent( p->m_lpTtl, strlen(p->m_lpTtl) );
		iTxtMaxW = max( iTxtMaxW, sz.cx );
	  }

   iTxtMaxW += 2 * szAdd.cx;
   int iHTxt = 2 * szAdd.cy;

   p = pDescr;
   int iBmpBtnW, iBmpBtnH;
   for( ; p->m_typ != TB_EndDescr; p++ )
	 if( p->m_typ == TB_Bmp )
	  {
        BITMAP bmp;
		CBitmap bm; bm.LoadBitmap( p->m_iIdBmp );
		::GetObject( bm, sizeof(BITMAP), &bmp );
		iBmpBtnW = bmp.bmWidth + 0.2*bmp.bmWidth;
		iBmpBtnH = bmp.bmHeight + 0.2*bmp.bmHeight;
		break;
	  }

   iMaxBoundX = iBmpBtnW;
   iMaxBoundY = iBmpBtnH;

   p = pDescr;
   for( ; p->m_typ != TB_EndDescr; p++ )
	{
      if( p->m_typ == TB_Bmp )
	   {
	     CBitmap *pBmp = new CBitmap();
		 pBmp->LoadBitmap( p->m_iIdBmp );
		 rLst.push_back( pBmp );

		 CButton *pBtn = new CButton();
		 rLstBtn.push_back( pBtn );
		 CRect rr( 0, 0, iBmpBtnW, iBmpBtnH );
		 pBtn->Create( "", 		   
		   WS_CHILD|WS_VISIBLE|BS_BITMAP|BS_CENTER|BS_VCENTER|WS_TABSTOP|p->m_dwExtraStyle,
		   rr, (CWnd*)&rB, p->m_iIDCmd );
		 pBtn->SetBitmap( *pBmp );

		 rB + *new TBtnItem( pBtn, p->m_bShiftMode );
		 rB + new TToolTipData( p->m_iIDCmd, p->m_lpHint );
	   }
	  else
	   {
	     CButton *pBtn = new CButton();
		 rLstBtn.push_back( pBtn );
		 CRect rr( 0, 0, iTxtMaxW, iHTxt );
		 if( bFlTxtAln == false )
		  {
            CSize sz = dc.GetTextExtent( p->m_lpTtl, strlen(p->m_lpTtl) );
			rr.right = sz.cx + 2 * szAdd.cx;
		  }
		 pBtn->Create( p->m_lpTtl, 
		   WS_CHILD|WS_VISIBLE|BS_CENTER|BS_VCENTER|WS_TABSTOP|p->m_dwExtraStyle, 
		   rr, (CWnd*)&rB, p->m_iIDCmd );

		 rB + *new TBtnItem( pBtn, p->m_bShiftMode );
		 rB + new TToolTipData( p->m_iIDCmd, p->m_lpHint );

		 iMaxBoundX = max( iMaxBoundX, rr.Width() );
		 iMaxBoundY = max( iMaxBoundY, rr.Height() );
	   }
	}

   int iM;
   rB.SetMaxBounds( (iM=rB.IsHoris() == true ? iMaxBoundY:iMaxBoundX) );
      

   /*rB.MyLayout( true, rB.IsHoris() == true ?
	  iM + 4*GetSystemMetrics( SM_CYDLGFRAME ): 
	  iM + 4*GetSystemMetrics( SM_CXDLGFRAME )
    );*/
 }


void TBaseView::ClearBtnsLists()
 {
   list<CWnd*>::iterator it( m_lstBtn.begin() );
   list<CWnd*>::iterator itEnd( m_lstBtn.end() );

   for( ; it != itEnd; ++it )
	  delete *it;

   list<CBitmap*>::iterator it2( m_lstBmp.begin() );
   list<CBitmap*>::iterator itEnd2( m_lstBmp.end() );

   for( ; it2 != itEnd2; ++it2 )
	  delete *it2;

   m_lstBmp.clear();
   m_lstBtn.clear();
 }

void TBaseView::OnUpdate_Always( CCmdUI*  pCmdUI )
 {
   pCmdUI->Enable( TRUE );   
 }


void TBaseView::OnActivateView(
	BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
 {
	if (SaveFocusControl())
		return;     // don't call base class when focus is already set

	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
 }

void TBaseView::OnActivateFrame(UINT nState, CFrameWnd* /*pFrameWnd*/)
 {
	if (nState == WA_INACTIVE)
		SaveFocusControl();     // save focus when frame loses activation
 }

BOOL TBaseView::SaveFocusControl()
 {
	// save focus window if focus is on this window's controls
	HWND hWndFocus = ::GetFocus();
	if (hWndFocus != NULL && ::IsChild(m_hWnd, hWndFocus))
	{
		m_hWndFocus = hWndFocus;
		return TRUE;
	}
	return FALSE;
 }

void TBaseView::OnSetFocus(CWnd*)
 {
	if (!::IsWindow(m_hWndFocus) || !::IsChild(m_hWnd, m_hWndFocus))
	{
		// invalid or unknown focus window... let windows handle it
		m_hWndFocus = NULL;
		Default();
		return;
	}
	// otherwise, set focus to the last known focus window
	::SetFocus(m_hWndFocus);
 }

BOOL TBaseView::PreTranslateMessage(MSG* pMsg)
 {
	ASSERT(pMsg != NULL);
	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);

	// allow tooltip messages to be filtered
	if (CView::PreTranslateMessage(pMsg))
		return TRUE;

	// don't translate dialog messages when in Shift+F1 help mode
	CFrameWnd* pFrameWnd = GetTopLevelFrame();
	if (pFrameWnd != NULL && pFrameWnd->m_bHelpMode)
		return FALSE;

	// since 'IsDialogMessage' will eat frame window accelerators,
	//   we call all frame windows' PreTranslateMessage first
	pFrameWnd = GetParentFrame();   // start with first parent frame
	while (pFrameWnd != NULL)
	{
		// allow owner & frames to translate before IsDialogMessage does
		if (pFrameWnd->PreTranslateMessage(pMsg))
			return TRUE;

		// try parent frames until there are no parent frames
		pFrameWnd = pFrameWnd->GetParentFrame();
	}

	// filter both messages to dialog and from children
	return PreTranslateInput(pMsg);
 }

BOOL TBaseView::OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult )
 { 
   NMHDR *ph = (NMHDR*)lParam;
   if( ph->code == NM_SETFOCUS ) 	
	 m_hWndFocus = m_hWndFocusCTL = ph->hwndFrom;	

   return CView::OnNotify( wParam, lParam, pResult );
 }

BOOL TBaseView::OnCommand( WPARAM wParam, LPARAM lParam )
 {
   if( HIWORD(wParam) == LBN_SETFOCUS )
	 m_hWndFocus = m_hWndFocusCTL = (HWND)lParam;

   return CView::OnCommand( wParam, lParam );
 }
