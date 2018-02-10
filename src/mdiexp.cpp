#include "mdiexp.hpp"
#include "vs.hpp"
#include "mfrexp.hpp"
#include "userlvl.hpp"
#include "vs.rh"

#include "v_expert.hpp"
#include "v_rules.hpp"
#include "v_net.hpp"
#include "v_work.hpp"


#include "rdlg.hpp"
#include "docexp.hpp"
#include "expdlg.hpp"

#include <afxpriv.h>

#include "globlmsg.hpp"

IMPLEMENT_DYNCREATE( TMDIFrame, CMDIChildWnd )

BEGIN_MESSAGE_MAP( TMDIFrame, CMDIChildWnd )
  ON_WM_CREATE()
  ON_WM_SYSCOMMAND()
  ON_WM_ERASEBKGND()

  
  ON_REGISTERED_MESSAGE( globl_iMsgQNewPal, OnGlobalQPal )
  ON_REGISTERED_MESSAGE( globl_iMsgPalChanged, OnGlobalPalCh )  

  ON_REGISTERED_MESSAGE( globl_iMsgSwitchViews, OnSwtVievs )
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP( CMyDialogBar, CDialogBar )
  ON_WM_ERASEBKGND()
  ON_WM_SIZE()
  ON_WM_CREATE()

  ON_MESSAGE( WM_HELPHITTEST, OnCommandHelp )

  ON_WM_DESTROY()
END_MESSAGE_MAP()


LRESULT CMyDialogBar::OnCommandHelp( WPARAM wp, LPARAM lParam )
{
  switch( m_vievsCurrent )
  {
     case TV_Expert:
       return 12355;

     case TV_RulesEdit: case TV_FactEdit: case TV_QueryEdit:
       return 12358;

     case TV_Consult:
       return 12357;

     case TV_NetsEdit:
       return 12356;
  }  

  return 12355;
}


TMDIFrame::TMDIFrame():
 m_dbarHor( true ),
 m_dbarVer( false )
 {   
 }

TMDIFrame::~TMDIFrame()
 {
 }



BOOL TMDIFrame::OnEraseBkgnd( CDC* )
 {   
   return TRUE;
 }

BOOL TMDIFrame::PreCreateWindow( CREATESTRUCT& cs )
 {
   BOOL bRes = CMDIChildWnd::PreCreateWindow( cs );
   if( !bRes ) return bRes;   

   //cs.style |= WS_HSCROLL|WS_VSCROLL;
   return TRUE;
 }

BOOL TMDIFrame::DestroyWindow()
 {
   m_dbarHor.DestroyWindow();
   m_dbarVer.DestroyWindow();

   return CMDIChildWnd::DestroyWindow();
 }

void TMDIFrame::OnSysCommand( UINT nID, LPARAM lParam )
 {
   if( (nID & 0xFFF0) == IDM_ACESS_TO_ES )
	{
      TExpertDoc *pDoc = (TExpertDoc*)GetActiveDocument();	  
	  if( !pDoc )
	   {
         MessageBox( "Невозможно получить документ", "Ошибка", MB_OK|MB_ICONEXCLAMATION );
		 return;
	   }
	  if( !pDoc->GetESystem() )
	   {
         MessageBox( "Незагружена экспертная система", "Ошибка", MB_OK|MB_ICONEXCLAMATION );
		 return;
	   }

	  CString str; 
	  str.Format( "Экспертной системе \"%s\"", (LPCSTR)pDoc->GetESystem()->GetName() ); 
	  TAccDlg dlg( AfxGetApp()->m_pMainWnd, pDoc->GetESystem(), str );

	  if( dlg.DoModal() == IDOK ) pDoc->SetModifiedFlag();
	}
   else CMDIChildWnd::OnSysCommand( nID, lParam );	
 }

int TMDIFrame::OnCreate( LPCREATESTRUCT lpC )
 {
   if( CMDIChildWnd::OnCreate(lpC) == -1 )
	  return -1;	 
      

   if( ((TMainFrame*)(AfxGetApp()->m_pMainWnd))->GetProtectLvl()->GetClass() == TAcessLevel::TU_Administrator )
	{
      ASSERT((IDM_ACESS_TO_ES & 0xFFF0) == IDM_ACESS_TO_ES);
      ASSERT(IDM_ACESS_TO_ES < 0xF000);

	  CMenu* pSysMenu = GetSystemMenu( FALSE );
	  if( pSysMenu != NULL )
	   {	  		
	     pSysMenu->AppendMenu( MF_SEPARATOR );		 
	     pSysMenu->AppendMenu( MF_STRING, IDM_ACESS_TO_ES, "Доступ..." );	
	   }
	}


   if( !m_dbarHor.Create(this, IDD_DLG_DLGBARH, CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY,
	                     IDD_DLG_DLGBARH) )
	{
	  MessageBox( "Не удалось создать горизонтальный DialogBar", "Ошибка", MB_OK|MB_ICONEXCLAMATION );
      return -1;
	}

   DWORD dwS = m_dbarHor.GetBarStyle();
   m_dbarHor.SetBarStyle( (dwS & ~CBRS_BORDER_BOTTOM)|CBRS_BORDER_LEFT|CBRS_BORDER_RIGHT );
   

   if( !m_dbarVer.Create(this, IDD_DLG_DLGBARV, CBRS_LEFT|CBRS_TOOLTIPS|CBRS_FLYBY,
	                     IDD_DLG_DLGBARV) )
	{
	  MessageBox( "Не удалось создать вертикальный DialogBar", "Ошибка", MB_OK|MB_ICONEXCLAMATION );
      return -1;
	}

   dwS = m_dbarVer.GetBarStyle();
   m_dbarVer.SetBarStyle( (dwS & ~CBRS_BORDER_TOP)|CBRS_BORDER_LEFT|CBRS_BORDER_BOTTOM );

   /*CRect rr( 0, 10, 60, 30 );
   CButton *pb1 = new CButton();
   pb1->Create( "Button1", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, rr, &m_dbarHor, 110 );
   CButton *pb2 = new CButton();
   pb2->Create( "Button2", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, rr, &m_dbarHor, 111 );
   CButton *pb3 = new CButton();
   pb3->Create( "Button3", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, rr, &m_dbarHor, 112 );

   m_dbarHor + *new TBtnItem( pb1, true ) + *new TBtnItem( pb2, true ) + *new TBtnItem( pb3, true );

   CButton *pb4 = new CButton();
   pb4->Create( "Button4", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, rr, &m_dbarHor, 113 );

   CButton *pb5 = new CButton();
   pb5->Create( "Button5", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, rr, &m_dbarHor, 114 );

   m_dbarHor + *new TBtnItem( pb4, false ) + *new TBtnItem( pb5, false );


   rr = CRect( 0, 10, 60, 30 );
   pb1 = new CButton();
   pb1->Create( "Button1", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, rr, &m_dbarVer, 210 );
   pb2 = new CButton();
   pb2->Create( "Button2", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, rr, &m_dbarVer, 211 );
   pb3 = new CButton();
   pb3->Create( "Button3", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, rr, &m_dbarVer, 212 );

   m_dbarVer + *new TBtnItem( pb1, true ) + *new TBtnItem( pb2, true ) + *new TBtnItem( pb3, true );

   pb4 = new CButton();
   pb4->Create( "Button4", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, rr, &m_dbarVer, 213 );

   pb5 = new CButton();
   pb5->Create( "Button5", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, rr, &m_dbarVer, 214 );

   m_dbarVer + *new TBtnItem( pb4, false ) + *new TBtnItem( pb5, false );
   */

   BOOL bFlag;
   OnGlobalQPal( 0, (LONG)&bFlag );

   ChangeTitle();

   return 0;
 }


/*void CMyDialogBar::OnShowWindow( BOOL bShow, UINT nStatus )
 {
   if( bShow && !nStatus ) MyLayout();

   CDialogBar::OnShowWindow( bShow, nStatus );
 }*/

BOOL CMyDialogBar::PreCreateWindow( CREATESTRUCT& cs )
 {
   BOOL bRes = CDialogBar::PreCreateWindow( cs );
   if( !bRes ) return bRes;

   cs.style |= WS_CLIPCHILDREN;
   return TRUE;
 }

void CMyDialogBar::OnDestroy()
 {
   if( m_bFlHoriz == true )
	{
      if( CMyDialogBar::m_iUseCnt > 1 ) CMyDialogBar::m_iUseCnt--;
      else
	   {
         CMyDialogBar::m_iUseCnt = 0;
	     ::GlobalFree( CMyDialogBar::m_hrcBmp );
	     CMyDialogBar::m_hrcBmp = NULL;
	     CMyDialogBar::m_palPal.DeleteObject();
	   }
	}
   else
	{
      if( CMyDialogBar::m_iUseCntv > 1 ) CMyDialogBar::m_iUseCntv--;
      else
	   {
         CMyDialogBar::m_iUseCntv = 0;
	     ::GlobalFree( CMyDialogBar::m_hrcBmpv );
	     CMyDialogBar::m_hrcBmpv = NULL;
	     CMyDialogBar::m_palPalv.DeleteObject();
	   }
	}   

   CDialogBar::OnDestroy();
 }

int CMyDialogBar::OnToolHitTest( CPoint point, TOOLINFO* pTI ) const
 {
   list<TToolTipData *>::iterator itSta( m_vecToolTips.begin() );
   list<TToolTipData *>::iterator itEnd( m_vecToolTips.end() );

   TToolTipData *pDta = NULL;
   CRect r;
   HWND hw;
   for( ; itEnd != itSta; ++itSta )
	{
	  hw = ::GetDlgItem( m_hWnd, (*itSta)->m_uiId );
	  if( !hw ) continue;
	  ::GetWindowRect( hw, &r );
	  ScreenToClient( &r );
	  if( r.PtInRect(point) == TRUE ) 
	   {
         pDta = *itSta;
		 break;
	   }
	}   

   if( !pDta ) return -1;

   //pTI->uFlags = TTF_IDISHWND | TTF_SUBCLASS;
   if( pTI )
	{
      pTI->uFlags = TTF_IDISHWND;
      pTI->hwnd = m_hWnd;
      pTI->uId = (UINT)hw;
      pTI->rect = r;
      //pTI->hinst = AfxGetResourceHandle();
      pTI->hinst = 0;
      //pTI->lpszText = MAKEINTRESOURCE( m_uiCommand );
      pTI->lpszText = _strdup( (LPCSTR)(pDta->m_csTxt) );
	}
   else return ::GetDlgCtrlID( hw );

   return 1;
 }

CMyDialogBar::~CMyDialogBar()
 {
   ClearBtns();   
 }

void CMyDialogBar::ClearBtns()
 {
   m_imaxBounds = 50;

   list<TToolTipData *>::iterator itSta( m_vecToolTips.begin() );
   list<TToolTipData *>::iterator itEnd( m_vecToolTips.end() );

   for( ; itEnd != itSta; ++itSta )
     if( *itSta ) delete *itSta;
	  
   m_vecToolTips.clear();


   list<TBtnItem*>::iterator itSta2( m_lstBtn.begin() );
   list<TBtnItem*>::iterator itEnd2( m_lstBtn.end() );

   for( ; itEnd2 != itSta2; ++itSta2 )
     if( *itSta2 ) 
	  {
	    //delete (*itSta2)->m_pBtn; - not owned button
	    delete *itSta2;
	  }
	  
   m_lstBtn.clear();   
 }


CRcFile::CRcFile( HINSTANCE hinst, UINT uiIdRc, LPCSTR lpName ): CFile()
   {
     m_hHan = hinst;
	 m_hrc = NULL;
	 m_hrc = FindResource( hinst, MAKEINTRESOURCE(uiIdRc), lpName );
     if( !m_hrc ) AfxThrowResourceException();

	 m_hg = NULL;
	 m_hg =  LoadResource( m_hHan, m_hrc );	
     if( !m_hg ) AfxThrowResourceException();

	 m_lpCuros = m_lpRc = (LPSTR)LockResource( m_hg );
     m_dwSizeRc = SizeofResource( m_hHan, m_hrc );
	 
	 m_dwCnt = 0;
	 m_bEOF = false;
   }


UINT CRcFile::Read( void* lpBuf, UINT nCount )
 {
   if( m_bEOF ) return 0;   

   DWORD dwRead = (m_dwCnt + nCount >= m_dwSizeRc) ? 
	 m_dwSizeRc-m_dwCnt:nCount;

   if( dwRead < nCount ) m_bEOF = true;

   memcpy( lpBuf, m_lpCuros, dwRead );
   m_lpCuros += dwRead;
   m_dwCnt += dwRead;

   return dwRead;
 }

DWORD CRcFile::GetLength() const
 {
   return m_dwSizeRc;
 }

CRcFile::~CRcFile()
 {
   ::FreeResource( m_hrc );
 }

int CMyDialogBar::OnCreate( LPCREATESTRUCT lpC )
 {
   if( CDialogBar::OnCreate(lpC) == -1 ) return -1;

   if( m_bFlHoriz == true && !CMyDialogBar::m_iUseCnt )
	{ 
      CRcFile frc( AfxGetResourceHandle(), IDB_DIB_BKGNDH, "BITMAP256" );
      CMyDialogBar::m_hrcBmp = ReadDIBFile( frc );
	  CreateDIBPalette( (HDIB)CMyDialogBar::m_hrcBmp, &m_palPal );

	  LPVOID lpDIB = ::GlobalLock( CMyDialogBar::m_hrcBmp );
	  m_iW = DIBWidth( (LPSTR)lpDIB );
      m_iH = DIBHeight( (LPSTR)lpDIB );
      ::GlobalUnlock( CMyDialogBar::m_hrcBmp );	  

	  m_rS = CRect( 0, 0, CMyDialogBar::m_iW, CMyDialogBar::m_iH );
	}

   if( m_bFlHoriz == false && !CMyDialogBar::m_iUseCntv )
	{ 
      CRcFile frc( AfxGetResourceHandle(), IDB_DIB_BKGNDV, "BITMAP256" );
      CMyDialogBar::m_hrcBmpv = ReadDIBFile( frc );
	  CreateDIBPalette( (HDIB)CMyDialogBar::m_hrcBmpv, &m_palPalv );

	  LPVOID lpDIB = ::GlobalLock( CMyDialogBar::m_hrcBmpv );
	  m_iWv = DIBWidth( (LPSTR)lpDIB );
      m_iHv = DIBHeight( (LPSTR)lpDIB );
      ::GlobalUnlock( CMyDialogBar::m_hrcBmpv );	  

	  m_rSv = CRect( 0, 0, CMyDialogBar::m_iWv, CMyDialogBar::m_iHv );
	}
	
   if( m_bFlHoriz == true ) CMyDialogBar::m_iUseCnt++;	   
   else CMyDialogBar::m_iUseCntv++;	   

   EnableToolTips( TRUE );
   
   //BOOL b = m_brBkGnd.CreateDIBPatternBrush( CMyDialogBar::m_hrcBmp, DIB_RGB_COLORS );

   return 0;
 }

bool CMyDialogBar::TextureIsOn() const
 {
   return ((TMainFrame*)(AfxGetApp()->m_pMainWnd))->GetCurUser()->m_bUseSpecialDesign;
 }

BOOL CMyDialogBar::OnEraseBkgnd( CDC *pDC )
 {      
   if( TextureIsOn() == false ) return CDialogBar::OnEraseBkgnd( pDC );

   CRect r; GetClientRect( r );     

   CPalette *pOld;

   if( m_bFlHoriz == true )
	{
      pOld = pDC->SelectPalette( &CMyDialogBar::m_palPal, FALSE );

      int iY = 0;
      for( ; iY < r.bottom; iY += CMyDialogBar::m_iH )
	   {
         int iX = 0;
         for( ; iX < r.right; iX += CMyDialogBar::m_iW )
		  {
	        CRect r2( iX, iY, iX + CMyDialogBar::m_iW, iY + CMyDialogBar::m_iH );
	        PaintDIB( pDC->m_hDC, r2, CMyDialogBar::m_hrcBmp, CMyDialogBar::m_rS, 
		              &CMyDialogBar::m_palPal );
		  }
	   }
	}
   else
	{
      pOld = pDC->SelectPalette( &CMyDialogBar::m_palPalv, FALSE );

	  int iY = 0;
      for( ; iY < r.bottom; iY += CMyDialogBar::m_iHv )
	   {
         int iX = 0;
         for( ; iX < r.right; iX += CMyDialogBar::m_iWv )
		  {
	        CRect r2( iX, iY, iX + CMyDialogBar::m_iWv, iY + CMyDialogBar::m_iHv );
	        PaintDIB( pDC->m_hDC, r2, CMyDialogBar::m_hrcBmpv, CMyDialogBar::m_rSv, 
		              &CMyDialogBar::m_palPalv );
		  }
	   }
	}
   //pDC->FillRect( r, &m_brBkGnd );

   pDC->SelectPalette( pOld, FALSE );

   return TRUE;
 }



CMyDialogBar& CMyDialogBar::operator+( TBtnItem *pBtnIt )
 {
   if( m_lstBtn.size() )
	 pBtnIt->m_pPrivBtn = (*--m_lstBtn.end())->m_pBtn;

   m_lstBtn.push_back( pBtnIt );

   pBtnIt->m_pBtn->GetWindowRect( pBtnIt->r );
   ScreenToClient( pBtnIt->r );      

   pBtnIt->m_iW = pBtnIt->r.Width();
   pBtnIt->m_iH = pBtnIt->r.Height();
   
   return *this;
 }

void CMyDialogBar::InsertHead( TBtnItem &rBtnIt )
 {
   if( m_lstBtn.size() ) (*m_lstBtn.begin())->m_pPrivBtn = rBtnIt.m_pBtn;

   m_lstBtn.push_front( &rBtnIt );

   rBtnIt.m_pBtn->GetWindowRect( rBtnIt.r );
   ScreenToClient( rBtnIt.r );      

   rBtnIt.m_iW = rBtnIt.r.Width();
   rBtnIt.m_iH = rBtnIt.r.Height();
 }

CSize CMyDialogBar::CalcFixedLayout( BOOL bStretch, BOOL bHorz )
 {
   CSize sz = CDialogBar::CalcFixedLayout( bStretch, bHorz );

   if( bHorz )	
     sz.cy = m_imaxBounds + 4*GetSystemMetrics( SM_CYDLGFRAME );
   else	
	 sz.cx = m_imaxBounds + 4*GetSystemMetrics( SM_CXDLGFRAME );

   return sz;
 }
//m_imaxBounds = 50;

void CMyDialogBar::MyLayout( bool bRedr, bool bFlCallDirectly, int iBnd )
 {
   int iDst = 
	 MUL_DSTBUTTONS * GetSystemMetrics( m_bFlHoriz == true ? SM_CXDLGFRAME:SM_CYDLGFRAME );
   int iCurX = 2 * iDst; 
   CRect r; GetClientRect( r );
 
   if( bFlCallDirectly == true )
	{
      if( IsHoris() == true ) r.bottom = iBnd;
	  else r.right = iBnd;
	}

   int iFixedSize = 2 * iCurX;
   list<TBtnItem*>::iterator itSta2( m_lstBtn.begin() );
   list<TBtnItem*>::iterator itEnd2( m_lstBtn.end() );

   int iCntVar = 0;
   bool bFlSta = true;
   for( ; itEnd2 != itSta2; ++itSta2 )	 
	{
      (*itSta2)->m_pBtn->GetWindowRect( (*itSta2)->r );
	  ScreenToClient( (*itSta2)->r );

      iFixedSize += m_bFlHoriz == true ? (*itSta2)->m_iW:(*itSta2)->m_iH;

	  if( bFlSta == true )
	   {
         bFlSta = false; continue;
	   }
	  if( (*itSta2)->m_bFixedDst == true ) iFixedSize += iDst;
	  else iCntVar++;
	}

   int iVarDst;
   if( m_bFlHoriz == true )
	  iVarDst = iCntVar ? (r.Width() - iFixedSize) / iCntVar:0;
   else
	  iVarDst = iCntVar ? (r.Height() - iFixedSize) / iCntVar:0;

   if( iVarDst < iCurX ) iVarDst = iCurX;

   itSta2 = m_lstBtn.begin();
   itEnd2 = m_lstBtn.end();
   bFlSta = true;
   CRect rP; GetClientRect( rP ); 
   int iPH = m_bFlHoriz == true  ? rP.Height():rP.Width();
   for( ; itEnd2 != itSta2; ++itSta2 )	 
	{
      if( bFlSta == false ) 
	    iCurX += (*itSta2)->m_bFixedDst == true ? iDst:iVarDst;
	  else bFlSta = false;

	  CRect rKey( (*itSta2)->r );
	  if( m_bFlHoriz == true )
	   {
         (*itSta2)->r.left = iCurX;
	     (*itSta2)->r.right = (*itSta2)->r.left + (*itSta2)->m_iW;

		 if( rKey != (*itSta2)->r )
	       (*itSta2)->m_pBtn->SetWindowPos( NULL, (*itSta2)->r.left,
	       (iPH - (*itSta2)->m_iH + 1) / 2, 0, 0, 
		   SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE|(bRedr ? 0:SWP_NOREDRAW) );
		 

	     iCurX += (*itSta2)->m_iW;
	   }
	  else
	   {
         (*itSta2)->r.top = iCurX;
	     (*itSta2)->r.bottom = (*itSta2)->r.top + (*itSta2)->m_iH;

		 if( rKey != (*itSta2)->r )
	       (*itSta2)->m_pBtn->SetWindowPos( NULL, (iPH - (*itSta2)->m_iW + 1) / 2,
	       (*itSta2)->r.top, 0, 0, 
		   SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE|(bRedr ? 0:SWP_NOREDRAW) );

	      iCurX += (*itSta2)->m_iH;
	   }	  
	}
 }

void CMyDialogBar::OnSize( UINT nType, int cx, int cy )
 {
   CDialogBar::OnSize( nType, cx, cy );

   if( nType == SIZE_MINIMIZED ) return;

   MyLayout( nType == SIZE_RESTORED );   
 }

void TMDIFrame::SwitchOnVerBar( bool bFl )
 {
   if( bFl == true )
	{
      if( m_dbarVer.GetSafeHwnd() ) return;

	  if( !m_dbarVer.Create(this, IDD_DLG_DLGBARV, CBRS_LEFT|CBRS_TOOLTIPS|CBRS_FLYBY,
	                     IDD_DLG_DLGBARV) )
	   {
	     MessageBox( "Не удалось создать вертикальный DialogBar", "Ошибка", MB_OK|MB_ICONEXCLAMATION );
         return;
	   }
	}
   else
	{
	  if( !m_dbarVer.GetSafeHwnd() ) return;

      m_dbarVer.DestroyWindow();
	}
 }

LONG TMDIFrame::OnSwtVievs( UINT uiViewTo, LONG pObj )
 {
//enum TSWT_Codes { TSWT_Explorer, TSWT_Rules, TSWT_Facts, TSWT_Q, 
  //                TSWT_Run, TSWT_Net, TSWT_Frm };

   //TBaseView* TMDIFrame::ChangeView( TV_Views tvToActivate, TProtectedObj *pObj )

   int iSwt = 0;
   switch( (TSWT_Codes)uiViewTo )
	{
	  case TSWT_Explorer:
        iSwt = TV_Expert;
		break; 
	  case TSWT_Rules:
	    iSwt = TV_RulesEdit;
		break;
	  case TSWT_Facts:
	    iSwt = TV_FactEdit;
		break;
	  case TSWT_Q:
	    iSwt = TV_QueryEdit;
		break;
	  case TSWT_Run:
	    iSwt = TV_Consult;
		break;
	  case TSWT_Net:
	    iSwt = TV_NetsEdit;
		break;
	  case TSWT_Frm:
	    iSwt = TV_FrameEdit;
		break;
	};

   ChangeView( (TV_Views)iSwt, (TProtectedObj*)pObj );
	
   return 0;
 }

LONG TMDIFrame::OnGlobalPalCh( UINT lPtrRes, LONG lPtr )
 {
   if( (CWnd*)lPtr != this ) 
	{
	  OnGlobalQPal( 0, 0 );
	  *(BOOL*)lPtrRes = TRUE;
	}
   return 0;
 }

LONG TMDIFrame::OnGlobalQPal( UINT, LONG lPtr )
 {
   LONG r1 = m_dbarHor.QPalette( 0 );
   LONG r2;
   if( m_dbarVer.GetSafeHwnd() ) r2 = m_dbarVer.QPalette( r1 );
   else r2 = 0;
    
   //if( GetMDIFrame()->GetTopWindow()->GetSafeHwnd() == m_hWnd )

   if( lPtr )
     *(BOOL*)lPtr = max( *(BOOL*)lPtr, max(r1, r2) );

   return 0;
   //m_dbarVer.QPalette();
 }

void TMDIFrame::ChangeTitle()
 {
   TBaseView *pView = (TBaseView*)GetActiveView();
   if( !pView ) return;
   CString str;      
   str.Format( "%s - \"%s\"", pView->GetShortFileName(), pView->GetName() );

   if( pView->GetDocument() )
     pView->GetDocument()->SetTitle( str );
   else
     SetWindowText( str );
 }

TBaseView* TMDIFrame::ChangeView( TV_Views tvToActivate, TProtectedObj *pObj )
 {
   CWaitCursor wCurs;   

   TBaseView *pView = (TBaseView*)GetActiveView();
   if( pView )
	{
	  if( pView->IsA() == tvToActivate ) return NULL;
	  if( pView->CanClose() == false ) return NULL;	
	}
   else
	{
      ::MessageBox( ((TExpertApp*)AfxGetApp())->m_pMainWnd->m_hWnd,
	     "Нарушение структуры дочернего MDI-окна", "Ошибка", MB_OK|MB_ICONSTOP );
	  return NULL;
	}

   TBaseView *pNewView = NULL;
   switch( tvToActivate )
	{
	  case TV_Expert:
	    pNewView = new TExpertView();
		break;
	  case TV_RulesEdit:
	    pNewView = new TRulesView();
		break;
	  case TV_FactEdit:
	    pNewView = new TRulesView();
		break;
	  case TV_QueryEdit:
	    pNewView = new TRulesView();
		break;
	  case TV_Consult:
	    pNewView = new TWorkView();
		break;
	  case TV_NetsEdit:
	    pNewView = new TNetView();
		break;
	  case TV_FrameEdit:
	    //pNewView = new TFrameView();
		break;
	};

   if( !pNewView )
	{
      ::MessageBox( ((TExpertApp*)AfxGetApp())->m_pMainWnd->m_hWnd,
	     "Невозможно создать новый TBaseView", "Ошибка", MB_OK|MB_ICONSTOP );
	  return NULL;
	}

   CCreateContext ccCtx;
   CDocument *pDocKey;
   pDocKey = ccCtx.m_pCurrentDoc = pView->GetDocument();
   

   if( pObj ) pNewView->MakeBindSubj( pObj );
   else if( tvToActivate == TV_Expert )
	 pNewView->MakeBindSubj( ((TExpertDoc*)pDocKey)->GetESystem() );   

   pNewView->Notify_OwnerCreate();
   if( !pNewView->Create( NULL, NULL, 0L, CFrameWnd::rectDefault,
	                      this, 200 + int(tvToActivate), &ccCtx )
	 )
	{
      delete pNewView;
	  ::MessageBox( ((TExpertApp*)AfxGetApp())->m_pMainWnd->m_hWnd,
	     "Не прошла инициализация TBaseView", "Ошибка", MB_OK|MB_ICONSTOP );
	}

   pView->ShowWindow( SW_HIDE );      
   //CRect rC; pView->GetWindowRect( rC ); ScreenToClient( rC );
   //CClientDC dc( this );
   //dc.FillSolidRect( rC, RGB(0xC0,0xC0,0xC0) );
   pNewView->OnInitialUpdate();

   SetActiveView( pNewView );   
   m_dbarHor.ClearBtns();
   m_dbarVer.ClearBtns();      
   pNewView->ShowWindow( SW_SHOW );   
   
   
   pView->GetDocument()->RemoveView( pView );
   pView->DestroyWindow();
   //delete pView;

   pNewView->SetDlgCtrlID( AFX_IDW_PANE_FIRST );      

   RecalcLayout();

   pNewView->SetupHorizBar( m_dbarHor );
   pNewView->SetupVerBar( m_dbarVer );

   RecalcLayout();

   ChangeTitle();

   return pNewView;
 } 


LONG CMyDialogBar::QPalette( LONG )
 {
	HPALETTE HpalOld;
	int i;
	HPALETTE hp = m_bFlHoriz == true ? CMyDialogBar::m_palPal:CMyDialogBar::m_palPalv;
					  //TPalette
	if( !hp ) return 0;

	HDC hdc = ::GetDC( m_hWnd );
	HpalOld = ::SelectPalette( hdc, hp, FALSE );
	
	i = ::RealizePalette( hdc );

	::SelectPalette( hdc, HpalOld, FALSE );

	if( i > 0 )
	  InvalidateRect( NULL, TRUE );	

	::ReleaseDC( m_hWnd, hdc );

	return i;
 }

int CMyDialogBar::m_iW = 0,  CMyDialogBar::m_iH = 0;
CPalette CMyDialogBar::m_palPal;
HDIB CMyDialogBar::m_hrcBmp = NULL;
int CMyDialogBar::m_iUseCnt = 0;
CRect CMyDialogBar::m_rS;

int CMyDialogBar::m_iWv = 0,  CMyDialogBar::m_iHv = 0;
CPalette CMyDialogBar::m_palPalv;
HDIB CMyDialogBar::m_hrcBmpv = NULL;
int CMyDialogBar::m_iUseCntv = 0;
CRect CMyDialogBar::m_rSv;
