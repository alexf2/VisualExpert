#include "v_work.hpp"

#include "subj.hpp"
#include "dlgacess.hpp"
#include "docexp.hpp"
#include "mdiexp.hpp"
#include "globlmsg.hpp"
#include "warea.hpp"
#include "var.hpp"
#include "mfrexp.hpp"
#include "expdlg.hpp"
#include "ask_q.hpp"
#include "vs.hpp"
#include "askv_dlg.hpp"

#include "ergodlg.hpp"

#include "viewexp.hpp"
#include <afxcmn.h>
#include <algorithm>

#include <afxdlgs.h>

#include "..\\RUDAKOW\\SERVIS\\myexc.hpp"

#include "vs.rh"

#include "product.hpp"

#include <afxpriv.h>


IMPLEMENT_DYNCREATE( TWorkView, TBaseView )

BEGIN_MESSAGE_MAP( TWorkView, TBaseView )	
  ON_WM_CREATE()  
  ON_WM_DESTROY()
  ON_WM_SIZE()
  ON_WM_ERASEBKGND()

  ON_WM_CTLCOLOR()

  ON_MESSAGE( WM_HELPHITTEST , OnCommandHelp )
  ON_MESSAGE( WM_COMMANDHELP, OnCommandHelpF1 )

  ON_WM_KEYUP()
  
  ON_UPDATE_COMMAND_UI( ICM_RUL_OPT, OnUpdate_Always )
  ON_UPDATE_COMMAND_UI( ICM_RUL_EXIT, Upd_RulExit )  

  ON_UPDATE_COMMAND_UI( ICM_WRK_ASK, Upd_WAsk )  
  ON_UPDATE_COMMAND_UI( ICM_WRK_STOP, Upd_WStop )  
  ON_UPDATE_COMMAND_UI( ICM_WRK_STEPMODE, OnUpdate_Always )  
  ON_UPDATE_COMMAND_UI( ICM_WRK_ANIMATE, OnUpdate_Always )  
  ON_UPDATE_COMMAND_UI( ICM_WRK_STEP, Upd_WStep )      
  ON_UPDATE_COMMAND_UI( ICM_WRK_PRINT, Upd_Print )      

  ON_COMMAND( ICM_RUL_OPT,    OnRulOpt )
  ON_COMMAND( ICM_RUL_EXIT,   OnRulExit )

  ON_COMMAND( ICM_WRK_ASK,       OnWAsk )
  ON_COMMAND( ICM_WRK_STOP,      OnWStop )
  ON_COMMAND( ICM_WRK_STEPMODE,  OnWStepMode )
  ON_COMMAND( ICM_WRK_ANIMATE,  OnWAnimate )  
  ON_COMMAND( ICM_WRK_STEP,      OnWStep )
  ON_COMMAND( ICM_WRK_PRINT,     OnWPrint )

  ON_LBN_DBLCLK( 13, OnVarModify )

END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP( TProdList, CListBox )	
  ON_WM_CREATE()    
  ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP( TFacList, CListBox )	
  ON_WM_CREATE()    
  ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP( TVarList, CListBox )	
  ON_WM_CREATE()    
  ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


BEGIN_MESSAGE_MAP( TTreeParam, CTreeCtrl )	  
  ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

LRESULT TWorkView::OnCommandHelp( WPARAM wp, LPARAM lParam )
{
  return 12347;
}

LRESULT TWorkView::OnCommandHelpF1( WPARAM wp, LPARAM lParam )
{
  AfxGetApp()->CWinApp::WinHelp( 3986 );    
  return TRUE;
}


HBRUSH TTreeParam::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
 {
   HBRUSH hbr = CTreeCtrl::OnCtlColor( pDC, pWnd, nCtlColor );
   
  //pDC->SetBkColor( RGB(0,0,255) );
  //pDC->SetTextColor( RGB(255, 255, 0) );

   return hbr;
 }

void TWorkView::OnVarModify()
 {
   int iC = m_lstVar.GetCurSel();
   if( iC == LB_ERR ) return;

   TVariantVar *pV = (TVariantVar*)m_lstVar.GetItemData( iC );
   if( !pV ) return;

   bool bKeyC = pV->m_bCanceled;

   TAskVarDlg dlg( this, IDD_DLG_ASKWORK, pV );
   dlg.DoModal();
   
   pV->m_bCanceled = bKeyC;
   m_lstVar.SelectVar( pV );
 }

TWorkView::TWorkView(): 
  TBaseView(),
  m_mtxObjLock( FALSE ),
  m_evStop( FALSE, TRUE ),
  m_evStep( TRUE, FALSE )
 {         
   m_iFirstShow = 0;
   pcitLstProd = NULL;

   m_pWA  = NULL;   

   m_iGoal = -1;

   m_bFlStepMode = false;
   m_bAnimMode = true;
   m_bFlRemoveUsedRules = true;

   m_iDeepRec = m_iDeepRecMax = m_iUsedRules = 0;

   m_bRestart = false;
   m_bFlHaveResult = false;

   m_64Wait = 700;
 }

TWorkView::~TWorkView()
 {   
   if( (TExpertDoc*)GetDocument() )
	 ((TExpertDoc*)GetDocument())->arcWA.FreeRc();

   ClearRes();   
 }

void TWorkView::ClearRes()
 {
   vector<TResults*>::iterator itSta( m_lstRes.begin() );
   vector<TResults*>::iterator itEnd( m_lstRes.end() );

   for( ; itSta != itEnd; ++itSta )
	  delete *itSta;

   m_lstRes.clear();
 }

void TWorkView::OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags )
 {
   /*if( nChar == VK_F10 )
	 PostMessage( WM_COMMAND, ICM_WRK_STEP, 0 );
   else if ( nChar == VK_F5 && GetKeyState(VK_SHIFT) )
	 PostMessage( WM_COMMAND, ICM_WRK_STOP, 0 );*/

   TBaseView::OnKeyUp( nChar, nRepCnt, nFlags );
 }

BOOL TWorkView::PreTranslateMessage( MSG* pMsg )
 {
   if( pMsg->message == WM_KEYDOWN )
	{
	  if( pMsg->wParam == VK_F11 )
		PostMessage( WM_COMMAND, ICM_WRK_STEP, 0 );
	  else if ( pMsg->wParam == VK_F5 && GetKeyState(VK_SHIFT) )
		PostMessage( WM_COMMAND, ICM_WRK_STOP, 0 );
	}

   return TBaseView::PreTranslateMessage( pMsg );
 }

HBRUSH TWorkView::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
 {
   HBRUSH hbr = TBaseView::OnCtlColor( pDC, pWnd, nCtlColor );
   
   /*int id = pWnd->GetDlgCtrlID();
   if( id == 14 )
	{	  
	  pDC->SetBkColor( RGB(0,0,255) );
	  pDC->SetTextColor( RGB(255, 255, 0) );
	  return (HBRUSH)m_brBlue;
	}*/

   return hbr;
 }

TO_ObjTypes TWorkView::IsObj() const
 {
   return TO_Rules;
 }

LPCSTR TWorkView::GetShortFileName()
 {
   if( !m_pBindSubj ) return "Пусто";
   //return ((TSubject*)m_pBindSubj)->GetPath();
   return ((TSubject*)m_pBindSubj)->MyName();
 }

LPCSTR TWorkView::GetName()
 {
   if( !m_pBindSubj ) return "Пусто";

   m_strTmpName.Format( "%s", 
	  (LPCSTR)((TSubject*)m_pBindSubj)->GetName() );
	  
   return m_strTmpName;
 }


BOOL TWorkView::OnEraseBkgnd( CDC *pDC )
 {      

   return Default();

   /*CRect r; GetClientRect( r ); 
   pDC->FillSolidRect( r, RGB(0xC0,0xC0,0xC0) );
 
   return TRUE;   */
 }

void TWorkView::OnUpdate( CView* pSender, LPARAM lHint, CObject* pHint )
 {
   TBaseView::OnUpdate( pSender, lHint, pHint );
 }

void TWorkView::OnDraw( CDC* pDC )
 {
 }


void TWorkView::OnDestroy()
 {         
   TBaseView::OnDestroy();
 }

void TWorkView::OnSize( UINT nType, int cx, int cy )
 {      
   if( m_iFirstShow < 3  ) m_iFirstShow++;   
   else
	{  
      m_iFirstShow = 10;
	  ModifyStyle( 0, WS_CLIPCHILDREN );
	}

   TBaseView::OnSize( nType, cx, cy );      
 }

BOOL TWorkView::PreCreateWindow( CREATESTRUCT& cs )
 {
   BOOL bRes = TBaseView::PreCreateWindow( cs );
   if( !bRes ) return bRes;

   cs.style &= ~WS_CLIPCHILDREN;
   return TRUE;
 }

void TWorkView::RefreshUsedRules()
 {
   CString str;

   str.Format( "Число использованных правил: %d", m_iUsedRules );
   m_tree.SetItemText( m_hUsedRul, str );
 }

void TWorkView::RefreshDeep()
 {   
   CString str;

   str.Format( "Текущая глубина рекурсии: %d", m_iDeepRec );    
   m_tree.SetItemText( m_hCurRecurs, str );
  
   str.Format( "Максимальная глубина рекурсии: %d", m_iDeepRecMax );
   m_tree.SetItemText( m_hMaxRecurs, str );
 }

int GetImgIndex( TR_Type typ )
 {
   switch( typ )
	{
	  case TR_Fac_High:
        return 0; 	   
	  case TR_Fac_Med:
	    return 1;
	  case TR_Fac_Low:
	    return 2;
	   
	  case TR_Rul_High:
	    return 3;
	  case TR_Rul_Med:
	    return 4;
	  case TR_Rul_Low:
	    return 5;

	  case TR_Res_High:
	    return 6;
	  case TR_Res_Med:
	    return 7;
	  case TR_Res_Low:
	    return 8;

	  default:
	    return 0;
	}
 }

int GetImgIndex2( list<TResult*>& rL )
 {
   if( !rL.size() ) return 8;
   
   TR_Type trLvl = (*rL.rbegin())->m_trLvl;
   switch( (*rL.rbegin())->m_trLvl )
	{
	  case TR_Fac_High: case TR_Rul_High:
	    return 6;

	  case TR_Fac_Med: case TR_Rul_Med:
	    return 7;

	  case TR_Fac_Low: case TR_Rul_Low:
	    return 8;
	}

   return 8;
 }

void TWorkView::FillTree( TTreeParam& rT, vector<TResults*>* pRes )
 {
   TWndDrwLock lockR( &rT );

   HTREEITEM hKey = NULL;

   rT.DeleteAllItems();

   TV_INSERTSTRUCT tv; memset( &tv, 0, sizeof(TV_INSERTSTRUCT) );
   tv.hInsertAfter = TVI_LAST;
   //tv.item.state = TVIS_BOLD;
   tv.item.stateMask = TVIS_BOLD;
   tv.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;   
   tv.item.iSelectedImage = tv.item.iImage = 9;
      
   
   CString str;

   str.Format( "Текущая глубина рекурсии: %d", m_iDeepRec );
   tv.item.pszText = (LPSTR)(LPCSTR)str;
   m_hCurRecurs = rT.InsertItem( &tv ); 
   rT.SetItemData( m_hCurRecurs, NULL );

   str.Format( "Максимальная глубина рекурсии: %d", m_iDeepRecMax );
   tv.item.pszText = (LPSTR)(LPCSTR)str;
   m_hMaxRecurs = rT.InsertItem( &tv ); 
   rT.SetItemData( m_hMaxRecurs, NULL );

   str.Format( "Число использованных правил: %d", m_iUsedRules );
   tv.item.pszText = (LPSTR)(LPCSTR)str;
   m_hUsedRul = rT.InsertItem( &tv ); 
   rT.SetItemData( m_hUsedRul, NULL );

   tv.item.state = TVIS_BOLD;   
   tv.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_STATE; 
   tv.item.iSelectedImage = tv.item.iImage = 9;

   if( m_iGoal == -1 )
     str.Format( "Цель: нет" );
   else
	 str.Format( "Цель: [%s] \"%s\"", 
	   (LPCSTR)m_pWA->m_vecQ[m_iGoal]->m_strQ, 
	   (LPCSTR)m_pWA->m_vecQ[m_iGoal]->m_strComment );

   tv.item.pszText = (LPSTR)(LPCSTR)str;
   m_hGoal = rT.InsertItem( &tv ); 
   rT.SetItemData( m_hGoal, NULL );

   //rT.Expand( m_hGoal, TVE_EXPAND ), rT.Select( m_hGoal, TVGN_CARET );
   rT.EnsureVisible( m_hGoal ), rT.Select( m_hGoal, TVGN_CARET ), rT.Expand( m_hGoal, TVE_EXPAND );
   

   if( pRes )
	{
	  if( !pRes->size() ) 
	   {
		 tv.item.pszText = "Решение не найдено";
		 tv.hParent = m_hGoal;
		 HTREEITEM htIt = rT.InsertItem( &tv ); 

		 rT.EnsureVisible( htIt ), rT.Select( htIt, TVGN_CARET ), rT.Expand( htIt, TVE_EXPAND );
	   }

	  vector<TResults*>::iterator itSta( pRes->begin() );
	  vector<TResults*>::iterator itEnd( pRes->end() );
        	  
	  int i = 1;
	  for( ; itSta != itEnd; ++itSta, ++i )
	   {	     
         str.Format( "Результат%d", i ); 
		 tv.item.state = TVIS_BOLD;
         tv.item.mask = TVIF_TEXT | TVIF_STATE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		 tv.item.pszText = (LPSTR)(LPCSTR)str;
		 tv.hParent = m_hGoal;
		 tv.item.iSelectedImage = tv.item.iImage = GetImgIndex2( (*itSta)->m_lstRes );		 
         HTREEITEM m_hRes = rT.InsertItem( &tv ); 

		 list<TResult*>::iterator itSta2( (*itSta)->m_lstRes.begin() );
		 list<TResult*>::iterator itEnd2( (*itSta)->m_lstRes.end() );
		 
         tv.item.mask = TVIF_TEXT;
		 tv.hParent = m_hRes;

		 int iSz = (*itSta)->m_lstRes.size(), k = 1;
		 //CString str;
		 HTREEITEM htIt = NULL;
		 for( ; itSta2 != itEnd2; ++itSta2, ++k )
		  {
		    
		    if( iSz == k ) 
			 {			   
			   tv.item.mask = TVIF_TEXT | TVIF_STATE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			   str = "Тогда " + (*itSta2)->m_str;
		       tv.item.pszText = (LPSTR)(LPCSTR)str;
			 }
			else 
			 {
			   tv.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			   tv.item.pszText = (LPSTR)(LPCSTR)((*itSta2)->m_str);
			 }

			tv.item.iSelectedImage = tv.item.iImage = GetImgIndex( (*itSta2)->m_trLvl  );            

		    htIt = rT.InsertItem( &tv ); 
		  }

		 //rT.Expand( m_hRes, TVE_EXPAND ), rT.Select( m_hRes, TVGN_CARET ),
		 if( htIt )
           rT.Select( htIt, TVGN_CARET ), rT.EnsureVisible( htIt );
		 tv.hParent = NULL;
	   }
	}

   //rT.Expand( m_hGoal, TVE_EXPAND ), rT.Select( m_hGoal, TVGN_CARET );
   

   //m_hCurRecurs, m_hMaxRecurs, m_hUsedRul, m_hGoal;
 }


void TWorkView::FillListVar( TVarList& rLstVar, TNameSpace* pSpc )
 {
   TWndDrwLock lockR( &rLstVar );
   rLstVar.ResetContent();

   list<TVariantVar*>& rLst = pSpc->GetLstRef();

   list<TVariantVar*>::iterator itSta( rLst.begin() );
   list<TVariantVar*>::iterator itEnd( rLst.end() );
  
   CString str, str2;
   for( ; itSta != itEnd; ++itSta )	
	if( (*itSta)->m_bConst == false )
	 {
	   (*itSta)->ValToStr( str2 );
	   if( (*itSta)->m_bFIni == true )
	     str.Format( "%s = %s :%1.2f", (LPCSTR)(*itSta)->m_strName, 
		   (LPCSTR)str2, (*itSta)->m_dKD );	   	   
	   else
		 str.Format( "%s = %s", (LPCSTR)(*itSta)->m_strName, 
		   (LPCSTR)str2 );	   	   

	   int iIt = rLstVar.AddString( str );	   

	   rLstVar.SetItemData( iIt, (DWORD)*itSta );	  	   
	 }
 }
/*
double m_dKD;
  bool m_bCanceled;
  bool m_bNeed;
  bool m_bConst;
*/

void TWorkView::FillListFac( TFacList& rLstFac, vector<TFactWork*>& rFac )
 {
   TWndDrwLock lockR( &rLstFac );
   rLstFac.ResetContent();

   vector<TFactWork*>::iterator itSta( rFac.begin() );
   vector<TFactWork*>::iterator itEnd( rFac.end() );

   CWindowDC dc( GetDesktopWindow() );
   CFont *pFOld = dc.SelectObject( rLstFac.m_pUsl->GetItemFnt(rLstFac.m_pUsl->m_ergo_LPFAC_Normal()) );
   int iMaxW = 0;
   for( ; itSta != itEnd; ++itSta )
	{
	  //CString str; str.Format( "%s :%1.1f", (LPCSTR)(*itSta)->m_strFac, (*itSta)->m_dKD );
	  int it = rLstFac.AddString( (LPCSTR)(*itSta)->m_strFac );
	  rLstFac.SetItemData( it, IT_Normal );	  

	  CSize sz = dc.GetTextExtent( (*itSta)->m_strFac );
	  iMaxW = max( iMaxW, sz.cx );
	}

   rLstFac.SetHorizontalExtent( iMaxW + 25 );
   m_iWBox = iMaxW + 25;

   dc.SelectObject( pFOld );
 }

void TWorkView::FillListProd( TProdList& rLstProd, vector<TSimpleProduct*>& rLst )
 {
   TWndDrwLock lockR( &rLstProd );
   rLstProd.ResetContent();

   vector<TSimpleProduct*>::iterator itSta( rLst.begin() );
   vector<TSimpleProduct*>::iterator itEnd( rLst.end() );

   CWindowDC dc( GetDesktopWindow() );
   CFont *pFOld = dc.SelectObject( rLstProd.m_pUsl->GetItemFnt(rLstProd.m_pUsl->m_ergo_LSPROD_DigConst()) );
   int iMaxW = 0;
   for( ; itSta != itEnd; ++itSta )
	{
	  int it = rLstProd.AddString( (*itSta)->m_strProd );
	  rLstProd.SetItemData( it, IT_Normal );
	  //enum TIT_ItemStateProd { IT_Normal, IT_Used, IT_CurrentStep, IT_PrivStep };

	  CSize sz = dc.GetTextExtent( (*itSta)->m_strProd );
	  iMaxW = max( iMaxW, sz.cx );
	}

   CSize sz2 = dc.GetTextExtent( "A", 1 );
   rLstProd.SetHorizontalExtent( iMaxW + rLstProd.m_iLeft );
   rLstProd.m_iExtent = m_iWBox = iMaxW + sz2.cx + rLstProd.m_iLeft;

   dc.SelectObject( pFOld );
 }


int TWorkView::OnCreate( LPCREATESTRUCT lpCreateStruct )
 {
   if( TBaseView::OnCreate(lpCreateStruct) == -1 )
     return -1;      

   m_bShadow = false;

   m_brBlue.CreateSolidBrush( RGB(0,0,128) );
   m_pWA = ((TExpertDoc*)GetDocument())->arcWA();

   CRect r( 0, 0, 100, 100 );
   m_lstProd.Create( WS_TABSTOP|WS_CHILD|WS_VISIBLE|LBS_OWNERDRAWFIXED|LBS_NOTIFY|LBS_NOINTEGRALHEIGHT|LBS_HASSTRINGS|WS_VSCROLL|WS_HSCROLL,
	 r, this, 11 );	
   m_lstProd.ModifyStyleEx( 0, WS_EX_CLIENTEDGE );

   m_lstFac.Create( WS_TABSTOP|WS_CHILD|WS_VISIBLE|LBS_OWNERDRAWFIXED|LBS_NOTIFY|LBS_NOINTEGRALHEIGHT|LBS_HASSTRINGS|WS_VSCROLL|WS_HSCROLL,
	 r, this, 12 );	
   m_lstFac.ModifyStyleEx( 0, WS_EX_CLIENTEDGE );

   m_lstVar.Create( WS_TABSTOP|WS_CHILD|WS_VISIBLE|LBS_OWNERDRAWFIXED|LBS_NOTIFY|LBS_NOINTEGRALHEIGHT|LBS_HASSTRINGS|WS_VSCROLL|WS_HSCROLL,
	 r, this, 13 );	
   m_lstVar.ModifyStyleEx( 0, WS_EX_CLIENTEDGE );

   m_tree.Create( WS_TABSTOP|WS_CHILD|WS_VISIBLE|WS_BORDER|WS_TABSTOP | TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS|TVS_SHOWSELALWAYS,
	 r, this, 14 );
   m_tree.ModifyStyleEx( 0, WS_EX_CLIENTEDGE );
	 

   CClientDC dc( this );


   pcitLstProd = new TChildItem( &m_lstProd, 0, 0, 1, 0.7 );   
   m_pchFac = new TChildItem( &m_lstFac, 0, 0.7, 1.0/3.0, 0.3 );      
   m_pchVar = new TChildItem( &m_lstVar, 2.0/3.0, 0.7, 1.0/3.0, 0.3 );   
   m_pchTree = new TChildItem( &m_tree, 1.0/3.0, 0.7, 1.0/3.0, 0.3 );   
   *this + pcitLstProd + m_pchFac + m_pchVar + m_pchTree;


   FillListProd( m_lstProd, m_pWA->m_vecProd );   
   FillListFac( m_lstFac, m_pWA->m_vecWork );
   FillListVar( m_lstVar, m_pWA->m_arcNam() );	
   FillTree( m_tree );
   
   CSize sz = dc.GetTextExtent( CString("A") );   
   SetMinWH( sz.cx * 30, sz.cy * 15 );
   
   MakeLaoyt();         

   m_ilstNormal = new CImageList();

   CBitmap bmFacH, bmFacM, bmFacL, bmRulH, bmRulM, bmRulL;
   CBitmap bmResH, bmResM, bmResL;
   CBitmap bmEmpt;

   bmEmpt.LoadBitmap( IDB_BMP_WRK_EMPTY );
   
   bmFacH.LoadBitmap( IDB_WRK_FACH );
   bmFacM.LoadBitmap( IDB_WRK_FACM );
   bmFacL.LoadBitmap( IDB_WRK_FACL );

   bmRulH.LoadBitmap( IDB_WRK_RULH );
   bmRulM.LoadBitmap( IDB_WRK_RULM );
   bmRulL.LoadBitmap( IDB_WRK_RULL );            

   bmResH.LoadBitmap( IDB_RES_RULH );
   bmResM.LoadBitmap( IDB_RES_RULM );
   bmResL.LoadBitmap( IDB_RES_RULL );            
	  
   BITMAP bm;
   ::GetObject( (HBITMAP)bmFacH, sizeof(bm), &bm );

   m_ilstNormal()->Create( bm.bmWidth, bm.bmHeight, 
	                       ILC_MASK | ILC_COLOR4, 10, 1 );
   m_ilstNormal()->Add( &bmFacH, RGB(0,0,128) );	  
   m_ilstNormal()->Add( &bmFacM, RGB(0,0,128) );	  
   m_ilstNormal()->Add( &bmFacL, RGB(0,0,128) );	     
     
   m_ilstNormal()->Add( &bmRulH, RGB(0,0,128) );	  
   m_ilstNormal()->Add( &bmRulM, RGB(0,0,128) );	  
   m_ilstNormal()->Add( &bmRulL, RGB(0,0,128) );	     

   m_ilstNormal()->Add( &bmResH, RGB(0,0,128) );	  
   m_ilstNormal()->Add( &bmResM, RGB(0,0,128) );	  
   m_ilstNormal()->Add( &bmResL, RGB(0,0,128) );	
   
   m_ilstNormal()->Add( &bmEmpt, RGB(0,0,128) );	   
   
   m_ilstNormal()->SetBkColor( CLR_NONE );

   m_tree.SetImageList( m_ilstNormal(), TVSIL_NORMAL );

   return 0; 
 }

TProdList::TProdList(): CListBox()
 {
   m_pUsl = ((TMainFrame*)(AfxGetApp()->m_pMainWnd))->GetCurUser();
   m_iExtent = 0;
 }

void TProdList::UpdateExtent( CString& cs )
 {
   CClientDC dc( this );
   CFont *pFOld = dc.SelectObject( m_pUsl->GetItemFnt(m_pUsl->m_ergo_LSPROD_DigConst()) );
   CSize sz = dc.GetTextExtent( cs );
   
   if( m_iExtent < sz.cx )
	{
	  CSize sz2 = dc.GetTextExtent( "A", 1 );
      m_iExtent = sz.cx + sz2.cx + m_iLeft;
	  SetHorizontalExtent( m_iExtent );
	}  

   dc.SelectObject( pFOld );
 }

int TProdList::OnCreate( LPCREATESTRUCT lpCreateStruct )
 {
   if( CListBox::OnCreate(lpCreateStruct) == -1 )
     return -1;

   bmpBr.LoadBitmap( IDB_BMP_LSPATTERN );
   m_br.CreatePatternBrush( &bmpBr );

   m_pRed.CreatePen( PS_SOLID, 2, RGB(128,0,0) );
   m_pGreen.CreatePen( PS_SOLID, 2, RGB(0,128,0) );   
   m_pRed2.CreatePen( PS_SOLID, 2, RGB(255,0,0) );
   
   return 0; 
 }

BOOL TProdList::OnEraseBkgnd( CDC *pDC )
 {      
   //return Default();

   CRect r;
   int iCnt = GetCount();
   if( iCnt == LB_ERR || iCnt < 1 ) GetClientRect( r );
   else
	{
	  CRect rr;
      GetItemRect( iCnt - 1, rr );
	  GetClientRect( r );
	  r.top = rr.bottom;
	}

   if( r.top < r.bottom )
     pDC->FillSolidRect( r, RGB(0,0,128) );
 
   return TRUE;   
 }

void TProdList::DrawLbl( CDC& rDC, CRect& r, TIT_ItemStateProd typ )
 {
   rDC.FillRect( r, &m_br );
   CRect rr( r ); 
   switch( typ )
	{
	  case IT_Used:
	   {         
	     int iW = r.Width(), iH = r.Height();
		 if( iW < iH )		  
           rr.DeflateRect( 3, 2 + iH - iW );
		 else
		   rr.DeflateRect( 2 + iW - iH, 3 );
		  
		 CPen *pOld = rDC.SelectObject( &m_pRed );

		 rDC.MoveTo( rr.left, rr.top );
		 rDC.LineTo( rr.right, rr.bottom );
		 rDC.MoveTo( rr.left, rr.bottom );
		 rDC.LineTo( rr.right, rr.top );

		 rDC.SelectObject( pOld );
	   }
	  break;

	  case IT_CurrentStep: case IT_PrivStep:
	   {
	     int iW = r.Width(), iH = r.Height();		 
         rr.DeflateRect( 4, 0 );
		 
		 CPen *pOld = rDC.SelectObject( typ == IT_CurrentStep ? &m_pRed2:&m_pGreen );

		 int iMY = rr.Height() / 2;
		 int iXA = rr.Width() / 2;
		 int iDelta = 0.6 * iMY;
		 rDC.MoveTo( rr.left, rr.top + iMY );
		 rDC.LineTo( rr.right, rr.top + iMY );		 
		 rDC.LineTo( rr.right - iXA, rr.top + iMY - iDelta );
		 rDC.MoveTo( rr.right, rr.top + iMY );
		 rDC.LineTo( rr.right - iXA, rr.top + iMY + iDelta );

		 rDC.SelectObject( pOld );
	   }
	  break;	  
	}
 }
//TIT_ItemStateProd { IT_Normal, IT_Used, IT_CurrentStep, IT_PrivStep };

TErgoItem *TProdList::GetItemClr( int iR, CString& rID )
 {
   switch( iR )
	{
	  case STRING_CONSTANT:
	    return m_pUsl->m_ergo_LSPROD_StrConst();

	  case CHARACTER_CONSTANT:
	    return m_pUsl->m_ergo_LSPROD_StrConst();

	  case BOOL_CONSTANT:
	    return m_pUsl->m_ergo_LSPROD_KeyWord();

	  case IDENTIFIER:
	   {
	     if( !rID.CompareNoCase("ЕСЛИ") || !rID.CompareNoCase("ТО") )
		    return m_pUsl->m_ergo_LSPROD_KeyWord();

	     TFuncInfo* pFI = TNodeFunction::FindFuncPtr( rID );
		 if( !pFI )
		   return m_pUsl->m_ergo_LSPROD_Var();
		 return m_pUsl->m_ergo_LSPROD_Func();
	   }

	  case FLOAT_CONSTANT: case INTEGER_CONSTANT:
	    return m_pUsl->m_ergo_LSPROD_DigConst();

	  case OROR: case ANDAND: case EQU: case NEQ: case LEQ: 
	  case GEQ: case '+': case '-': case '*': case '/': case '%':
	  case ',':
	    return m_pUsl->m_ergo_LSPROD_Oper();

	  default:
	    return m_pUsl->m_ergo_LSPROD_Other();
	};
 }


void TProdList::DrawItem( LPDRAWITEMSTRUCT lpDr )
 {
   if( !(lpDr->itemAction & ODA_DRAWENTIRE) ) return;
 
   CString str; GetText( lpDr->itemID, str );

   CRect r( lpDr->rcItem );
   int iXMax = lpDr->rcItem.right;
   r.right = r.left + m_iLeft;

   CDC dc; dc.Attach( lpDr->hDC );
   DrawLbl( dc, r, (TIT_ItemStateProd)GetItemData(lpDr->itemID) );

   CFont *pFOld = dc.SelectObject( m_pUsl->GetItemFnt(m_pUsl->m_ergo_LSPROD_DigConst()) );
   
   TNameSpace nsp;
   TExpression exp( (LPCSTR)str, nsp );
   exp.InitParse();

   YYSTYPE yys;
   YYLTYPE yyl;
   memset( &yys, 0 , sizeof(YYSTYPE) );
   memset( &yyl, 0 , sizeof(YYLTYPE) );
   
   dc.SetBkMode( OPAQUE );
   /*dc.SetTextAlign( TA_LEFT|TA_TOP|TA_NOUPDATECP );
   CString strNull( " " );
   dc.TextOut( r.right, r.top, strNull );
   dc.SetTextAlign( TA_LEFT|TA_TOP|TA_UPDATECP );*/
   CString strTmp;
   //CString str2;
   int iRes;   
   CRect rWL; GetClientRect( rWL );
   int xMax = rWL.Width();
   while( iRes = exp.Lexer(&yys, &yyl) )
	{	  
	  if( iRes == IDENTIFIER )
	   {
		 int iLen = yyl.m_iEndClm - yyl.m_iStaClm + 1;
		 LPSTR lpTmp = strTmp.GetBuffer( iLen + 2 );
		 strncpy( lpTmp, &(((LPSTR)(LPCSTR)str)[ yyl.m_iStaClm ]), iLen );
		 strTmp.ReleaseBuffer( iLen );
	   }

	  TErgoItem *pIt = GetItemClr( iRes, strTmp );

	  dc.SetTextColor( pIt->m_clrFore );
	  dc.SetBkColor( pIt->m_clrBk );
	  int iCnt = 0;
	  if( yyl.m_iStaClm && str[yyl.m_iStaClm - 1] == ' ' )
	   { 
	     LPSTR lp = &(((LPSTR)(LPCSTR)str)[ yyl.m_iStaClm - 1 ]);
		 LPSTR lpSta = &(((LPSTR)(LPCSTR)str)[ 0 ]);
		 LPSTR lp2 = lp; 
		 for( ; *lp2 == ' ' && lp2 >= lpSta; lp2-- ) iCnt++;		 
		 /*LPSTR lpp = str2.GetBuffer( iCnt + 1);
		 memset( lpp, ' ', iCnt );
		 str2.ReleaseBuffer( iCnt );*/
         //dc.TextOut( 0, 0, str2 );
	   }
	  
	  //dc.TextOut( 0, 0, &(((LPSTR)(LPCSTR)str)[ yyl.m_iStaClm ]),
	   //yyl.m_iEndClm - yyl.m_iStaClm + 1 );	  
	  
	  r.left = r.right;	  
	  int iSzS;
	  LPSTR lpS;
	  CSize szz = dc.GetTextExtent( (lpS=&(((LPSTR)(LPCSTR)str)[ yyl.m_iStaClm - iCnt ])), 
	     (iSzS=yyl.m_iEndClm - yyl.m_iStaClm + 1 + iCnt) );

      r.right = r.left + szz.cx;
	  dc.DrawText( lpS, iSzS, r, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX );

	  //if( r.right > xMax ) break;
	}

   int iHor = GetScrollPos( SB_HORZ );
   xMax += iHor;
   
   if( r.right < xMax )
	{
      r.left = r.right, r.right += (xMax - r.right);
	  dc.FillSolidRect( r, RGB(0,0,128) );
	}

   dc.SelectObject( pFOld );

   dc.Detach();   
 }

void TProdList::MeasureItem( LPMEASUREITEMSTRUCT lpM )
 {
   CWindowDC dc( GetDesktopWindow() );

   CFont *pFOld = dc.SelectObject( m_pUsl->GetItemFnt(m_pUsl->m_ergo_LSPROD_DigConst()) );
   //CString str; GetText( lpM->itemID, str );
   //if( str.IsEmpty() ) str = " ";
   CString str( "ABC" );
   CSize sz = dc.GetTextExtent( str );   

   lpM->itemWidth = sz.cx;
   lpM->itemHeight = sz.cy;

   dc.SelectStockObject( DEFAULT_GUI_FONT );
   sz = dc.GetTextExtent( str );   
   m_iLeft = sz.cx;

   dc.SelectObject( pFOld );
 }

void TWorkView::OnInitialUpdate()
 {
   TBaseView::OnInitialUpdate();      
 }


TV_Views TWorkView::IsA() const
 {
   return TV_Consult;
 }

bool TWorkView::CanClose()
 {   
   DWORD dwExitCode;
   bool bFl = m_arcTh() && 
     GetExitCodeThread(m_arcTh()->m_hThread, &dwExitCode) && dwExitCode == STILL_ACTIVE;
   
   if( bFl == true )
	{
	  CString str;
	  str.Format( "Прервать вывод ?" );	     
      int iRes = MessageBox( (LPCSTR)str, "Предупреждение", MB_YESNO|MB_ICONQUESTION );
	  if( iRes == IDNO ) return false;
	  
	  m_evStop.SetEvent();	  
	  return false; 
	}

   return true; 
 }
  

static TBtnTool btnHoriz[] =
 {
  { TB_Bmp, ICM_WRK_PRINT, NULL, IDB_BMP_WRK_PRINT, true, "Печать результата", BS_PUSHBUTTON },
  { TB_Bmp, ICM_WRK_ASK, NULL, IDB_BMP_WRK_ASK, false, "Задать вопрос", BS_PUSHBUTTON },
  { TB_Bmp, ICM_WRK_STOP, NULL, IDB_BMP_WRK_STOP, true, "Остановить вывод", BS_PUSHBUTTON },

  { TB_Bmp, ICM_WRK_STEPMODE, NULL, IDB_BMP_WRK_STEPMODE, false, "Вывод по шагам", BS_AUTOCHECKBOX|BS_PUSHLIKE|WS_GROUP },
  { TB_Bmp, ICM_WRK_ANIMATE, NULL, IDB_BMP_WRK_ANIMATE, true, "Режим анимации вывода", BS_AUTOCHECKBOX|BS_PUSHLIKE },
  { TB_Bmp, ICM_WRK_STEP, NULL, IDB_BMP_WRK_STEP, true, "Шаг вывода", BS_PUSHBUTTON|WS_GROUP },

  { TB_Bmp, ICM_RUL_OPT, NULL, IDB_BMP_EXP_OPT, false, "Настройка", BS_PUSHBUTTON },
  { TB_Bmp, ICM_RUL_EXIT, NULL, IDB_BMP_RUL_EXIT, true, "Возврат в эксплорер экспертных систем", BS_PUSHBUTTON },
  { TB_EndDescr, 0, NULL, 0, true, "", 0 }
 };


void TWorkView::SetupHorizBar( CMyDialogBar& rB )
 {     
   rB.SetCurVievClass( TV_Consult );

   m_pHorDlgb = &rB;

   ClearBtnsLists(); //only here ! - first calls horizontal setup
   DWORD dwS = rB.GetBarStyle();
   rB.SetBarStyle( (dwS & ~CBRS_BORDER_BOTTOM)|CBRS_BORDER_LEFT|CBRS_BORDER_RIGHT|CBRS_BORDER_TOP );         

   CreateBtns( rB, btnHoriz, false, m_lstBmp, m_lstBtn );      
   
   ((CButton*)m_pHorDlgb->GetDlgItem( ICM_WRK_STEPMODE ))->SetCheck( m_bFlStepMode );
   ((CButton*)m_pHorDlgb->GetDlgItem( ICM_WRK_ANIMATE ))->SetCheck( m_bAnimMode );
 }

void TWorkView::SetupVerBar( CMyDialogBar& rB )
 {
   rB.SetCurVievClass( TV_Consult );
   if( rB.IsWindowVisible() ) rB.ShowWindow( SW_HIDE );
 }

void TWorkView::Upd_RulExit( CCmdUI* pCmdUI )
 { 
   DWORD dwExitCode;
   bool bFl = m_arcTh() && 
     GetExitCodeThread(m_arcTh()->m_hThread, &dwExitCode) && dwExitCode == STILL_ACTIVE;
   pCmdUI->Enable( !bFl );
 }
void TWorkView::Upd_WAsk( CCmdUI* pCmdUI )
 {
   DWORD dwExitCode;
   bool bFl = m_arcTh() && 
     GetExitCodeThread(m_arcTh()->m_hThread, &dwExitCode) && dwExitCode == STILL_ACTIVE;
   pCmdUI->Enable( !bFl );
 }
void TWorkView::Upd_WStop( CCmdUI* pCmdUI )
 {
   DWORD dwExitCode;
   bool bFl = m_arcTh() && 
     GetExitCodeThread(m_arcTh()->m_hThread, &dwExitCode) && dwExitCode == STILL_ACTIVE;
   pCmdUI->Enable( bFl );
 }

void TWorkView::Upd_Print( CCmdUI* pCmdUI )
 {
   pCmdUI->Enable( m_bFlHaveResult );
 }

void TWorkView::Upd_WStep( CCmdUI* pCmdUI )
 {
   DWORD dwExitCode;
   bool bFl = m_arcTh() && 
     GetExitCodeThread(m_arcTh()->m_hThread, &dwExitCode) && dwExitCode == STILL_ACTIVE;
   pCmdUI->Enable( bFl );
 }

UINT ThreadProc( LPVOID pObj )
 {
   ((TWorkView*)pObj)->LetsGo();
   return 0;
 }

void TWorkView::OnWAsk()
 {
   m_bFlHaveResult = false;

   if( m_bRestart == true )
	{
	  try {
         TWndDrwLock lock1( &m_lstProd ),
		             lock2( &m_lstFac ),
					 lock3( &m_lstVar ),
					 lock4( &m_tree );
	    
         m_lstProd.ResetContent();
		 m_lstFac.ResetContent();
		 m_lstVar.ResetContent();
		 m_tree.DeleteAllItems();

         CWaitCursor wc;

		 ((TExpertDoc*)GetDocument())->arcWA = 
			new TWorkArea( (TPro*)m_pBindSubj, ((TExpertDoc*)GetDocument())->GetESystem() );
		 ((TExpertDoc*)GetDocument())->arcWA()->Create();

		 m_pWA = ((TExpertDoc*)GetDocument())->arcWA();

		 FillListProd( m_lstProd, m_pWA->m_vecProd );   
		 FillListFac( m_lstFac, m_pWA->m_vecWork );
		 FillListVar( m_lstVar, m_pWA->m_arcNam() );	
		 FillTree( m_tree );
	   }
	  catch( CException *pE )
	   {
		 pE->ReportError();
		 pE->Delete();
		 ((TExpertDoc*)GetDocument())->arcWA.FreeRc();

		 return;
	   }   
	  catch( TMyException *pE )
	   {
		 MessageBox( pE->GetMsg(), "Ошибка", 
			MB_OK|MB_ICONSTOP );
		 pE->Delete();
		 ((TExpertDoc*)GetDocument())->arcWA.FreeRc();

		 return;
	   }
	}

   TStaDlg dlg( this, IDD_DLG_STA, m_pWA );
   dlg.m_bFlRemove = m_bFlRemoveUsedRules;
   dlg.m_iGoal = m_iGoal;
   if( dlg.DoModal() != IDCANCEL )
	{
	  m_bFlRemoveUsedRules = dlg.m_bFlRemove;
      m_iGoal = dlg.m_iGoal;
	  FillTree( m_tree );
	  //LetsGo();
	  m_arcTh = new CWinThread( ThreadProc, (LPVOID)this );   
	  m_arcTh()->m_bAutoDelete = FALSE;
	  m_arcTh()->CreateThread();
	}

   m_bRestart = true;
 }

void TWorkView::OnWStop()
 {
   m_evStop.SetEvent(); 
 }

void TWorkView::OnWAnimate()
 {
   m_bAnimMode = ((CButton*)m_pHorDlgb->GetDlgItem( ICM_WRK_ANIMATE ))->GetCheck();

   ((CButton*)m_pHorDlgb->GetDlgItem( ICM_WRK_STEPMODE ))->SetCheck( 0 );
   m_bFlStepMode = false;
 }

static int CmpKDRes( TResults *p1, TResults *p2 )
 {
   if( !p1->m_lstRes.size() || !p2->m_lstRes.size() ) return 0;

   return (*p1->m_lstRes.rbegin())->m_Val.m_dKD > (*p2->m_lstRes.rbegin())->m_Val.m_dKD;
 }

void TWorkView::OnPrint( CDC* pDC, CPrintInfo* pInfo )
 {
   pDC->SetMapMode( MM_ISOTROPIC );
   int iCx = 800,//GetSystemMetrics( SM_CXSCREEN ), 
	   iCy = 600;//GetSystemMetrics( SM_CYSCREEN );
   int iLShft = iCx * 0.02, 
	   iUpShft = iCx * 0.02; 
   pDC->SetWindowExt( iCx, iCy );
   pDC->SetViewportExt( pInfo->m_rectDraw.Width(), pInfo->m_rectDraw.Height() );
   pDC->SetViewportOrg( 0, 0 );
   /*CRect r; m_tree.GetClientRect( r );
   double dMul = pInfo->m_rectDraw.Width() / r.Width();
   m_tree.Print( pDC, PRF_CLIENT );*/

   CImageList imgLst;
   CBitmap bmMark;
   bmMark.LoadBitmap( IDB_VE );
   BITMAP bm;
   ::GetObject( (HBITMAP)bmMark, sizeof(bm), &bm );

   
   imgLst.Create( bm.bmWidth, bm.bmHeight, 
	              ILC_MASK | ILC_COLOR4, 1, 1 );
   imgLst.Add( &bmMark, RGB(192,192,192) );	        
   imgLst.SetBkColor( CLR_NONE );

   imgLst.Draw( pDC, 0, CPoint(iLShft, iUpShft), ILD_NORMAL );
   
   CFont fntTtl;
   static LOGFONT lfTtl = { 28, 0, 0, 0, FW_BOLD, TRUE, 0, 0, DEFAULT_CHARSET,
      OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
	  VARIABLE_PITCH | FF_DONTCARE, "Times New Roman Cyr" };
   fntTtl.CreateFontIndirect( &lfTtl );

   CString str; str.Format( "Экспертная система \"%s\"", 
	  (LPCSTR)((TExpertDoc*)GetDocument())->GetESystem()->GetName() );
   CFont *pFOld = pDC->SelectObject( &fntTtl );
   CSize sz = pDC->GetTextExtent( str );

   CRect rT( 0, 0, iCx - (2*iLShft + bm.bmWidth + bm.bmWidth*0.5), 0 );
   //pDC->DrawText( str, &rT, DT_CALCRECT );
   rT.left = iLShft + bm.bmWidth + bm.bmWidth*0.5;
   rT.right = iCx - rT.left;
   //int iH = rT.Height();
   //rT.top = (bm.bmHeight - iH)/2 + iUpShft;
   rT.top = iUpShft;
   rT.bottom = iUpShft + bm.bmHeight;
   pDC->DrawText( str, &rT, DT_CENTER|DT_VCENTER|DT_WORDBREAK|DT_NOPREFIX );
	//iLShft + bm.bmWidth + bm.bmWidth*0.5, 
	 //(bm.bmHeight - sz.cy)/2 + iUpShft, str );
   pDC->SelectObject( pFOld );

   CPen penBlu( PS_SOLID, 3, RGB(0,0,255) );
   CPen *pOldPen = pDC->SelectObject( &penBlu );
   pDC->MoveTo( iLShft, iUpShft + bm.bmHeight+2 );
   pDC->LineTo( iCx - iLShft, iUpShft + bm.bmHeight+2 );

   pDC->MoveTo( iLShft+4, iUpShft+4 + bm.bmHeight+2 );
   pDC->LineTo( iCx - iLShft+4, iUpShft+4 + bm.bmHeight+2 );
   pDC->SelectObject( pOldPen );

   CFont fntGoal;
   static LOGFONT lfGl = { 18, 0, 0, 0, FW_BOLD, TRUE, 0, 0, DEFAULT_CHARSET,
      OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
	  VARIABLE_PITCH | FF_DONTCARE, "Arial Cyr" };
   fntGoal.CreateFontIndirect( &lfGl );

   CFont fntPro;
   static LOGFONT lfPro = { 14, 0, 0, 0, FW_NORMAL, FALSE, 0, 0, DEFAULT_CHARSET,
      OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
	  VARIABLE_PITCH | FF_DONTCARE, "Arial Cyr" };
   fntPro.CreateFontIndirect( &lfPro );

   CString strTst( "Ab" );
   pFOld = pDC->SelectObject( &fntPro );
   CSize szPro = pDC->GetTextExtent( strTst );
   pDC->SelectObject( &fntGoal );
   CSize szGl = pDC->GetTextExtent( strTst );
   pDC->SelectObject( pFOld );

   
   str.Format( "Цель: [%s] \"%s\"", 
	   (LPCSTR)m_pWA->m_vecQ[m_iGoal]->m_strQ, 
	   (LPCSTR)m_pWA->m_vecQ[m_iGoal]->m_strComment );

   int yY = iUpShft+4 + bm.bmHeight+2 + 4;
   int xX = iLShft + 4;
   pFOld = pDC->SelectObject( &fntGoal );

   rT = CRect( xX, yY, iCx - xX, yY + iCy - iUpShft );
   sz = pDC->GetTextExtent( str );
   pDC->DrawText( str, &rT, DT_LEFT|DT_TOP|DT_WORDBREAK|DT_NOPREFIX );

   pDC->SelectObject( pFOld );


   vector<TResults*> lstTmpR( m_lstRes.begin(), m_lstRes.end() );
   sort( lstTmpR.begin(), lstTmpR.end(), CmpKDRes );

   vector<TResults*>::iterator itSta( lstTmpR.begin() );
   vector<TResults*>::iterator itEnd( lstTmpR.end() );

   CPen penBluT( PS_SOLID, 2, RGB(0,0,255) );
   pOldPen = pDC->SelectObject( &penBluT );
           
   int i = 1;
   int iXPt = xX + szPro.cx,
	   iYPt = yY + sz.cy;
   int iYDelta = szGl.cy*1.5;
   for( ; itSta != itEnd; ++itSta, ++i )
	{
	  int iYPt2 = iYPt + iYDelta;
	  pDC->MoveTo( iXPt, iYPt );
	  pDC->LineTo( iXPt, iYPt2 );
	  pDC->LineTo( iXPt + szGl.cx, iYPt2 );

	  rT = CRect( iXPt + szGl.cx+2, iYPt2-szGl.cy*0.5, 
	              iCx - xX, iYPt2+szGl.cy*0.5+1 );

      str.Format( "Результат%d", i ); 

	  int iInd2 = GetImgIndex2( (*itSta)->m_lstRes );	

	  IMAGEINFO inf;
	  m_ilstNormal()->GetImageInfo( iInd2, &inf ); 
	  m_ilstNormal()->Draw( pDC, iInd2, 
	     CPoint(rT.left, iYPt2-abs(inf.rcImage.bottom-inf.rcImage.top)*0.5), ILD_NORMAL );

	  rT.left += abs( inf.rcImage.right - inf.rcImage.left );	   
	  pFOld = pDC->SelectObject( &fntGoal );
	  pDC->DrawText( str, &rT, DT_LEFT|DT_TOP|DT_WORDBREAK|DT_NOPREFIX );
	  pDC->SelectObject( pFOld );
	  iYPt = iYPt2;
	  

	  list<TResult*>::iterator itSta2( (*itSta)->m_lstRes.begin() );
	  list<TResult*>::iterator itEnd2( (*itSta)->m_lstRes.end() );

	  pFOld = pDC->SelectObject( &fntPro );
	  int iXPt_ = rT.left,
	      iYPt_ = rT.bottom;

	  int iSz = (*itSta)->m_lstRes.size(), k = 1;	  
	  int iKeyY = iYPt2;
	  for( ; itSta2 != itEnd2; ++itSta2, ++k )
	   {	 
         int iYPt2_ = iYPt_ + szPro.cy*1.5;

		 pDC->MoveTo( iXPt_, iYPt_ );
		 pDC->LineTo( iXPt_, iYPt2_ );
		 pDC->LineTo( iXPt_ + szPro.cx, iYPt2_ );

		 rT = CRect( iXPt_ + szPro.cx+2, iYPt2_-szPro.cy*0.5, 
					 iCx - xX, iYPt2_+szPro.cy*0.5+1 );

		 if( iSz == k ) 
		  {			   			
			str = "Тогда " + (*itSta2)->m_str;		    
			pDC->SetTextColor( RGB(255,0,0) );
		  }
		 else 
		  {			
			str = (*itSta2)->m_str;
		  }

		 int iInd = GetImgIndex( (*itSta2)->m_trLvl  );            
		 
		 m_ilstNormal()->GetImageInfo( iInd, &inf ); 
		 m_ilstNormal()->Draw( pDC, iInd, 
			CPoint(rT.left, iYPt2_-abs(inf.rcImage.bottom-inf.rcImage.top)*0.5), ILD_NORMAL );

		 rT.left += abs( inf.rcImage.right - inf.rcImage.left );	   		 
		 pDC->DrawText( str, &rT, DT_LEFT|DT_TOP|DT_WORDBREAK|DT_NOPREFIX );
		 iYPt_ = iYPt2_;		 

		 pDC->SetTextColor( RGB(0,0,0) );
	   }

	  iYDelta = iYPt_ - iKeyY + szGl.cy*1.0;
	  //iYPt = iYPt_ + szGl.cy*1.5;

	  pDC->SelectObject( pFOld );
	}

   pDC->SelectObject( pOldPen );
 }


BOOL TWorkView::OnPreparePrinting( CPrintInfo* pInfo )
 {
   return DoPreparePrinting( pInfo );
 }

void TWorkView::OnWPrint()
 {
   /*CPrintDialog dlg( TRUE, 
	 PD_ALLPAGES | PD_USEDEVMODECOPIES | PD_NOPAGENUMS | PD_HIDEPRINTTOFILE | PD_NOSELECTION,
	 this );
 
   if( dlg.DoModal() != IDCANCEL )
	{
	  CDC dc; dc.Attach( dlg.CreatePrinterDC() );
      m_tree.Print( &dc, dlg.m_pd.Flags );
	  //::DeleteDC( dc.Detach() );
	}*/
	/*CWinApp *pApp = AfxGetApp();
	CDC dc;
	pApp->CreatePrinterDC( dc );*/

    //AfxGetApp()->OnFilePrintSetup();
	//CDC dc;
    //AfxGetApp()->CreatePrinterDC( dc );  
	//m_tree.Print( &dc, 0 );

	/*CPrintDialog dlg( TRUE );
    if( AfxGetApp()->DoPrintDialog(&dlg) == IDCANCEL ) return;

	CDC dc; 
	AfxGetApp()->CreatePrinterDC( dc );  
	m_tree.Print( &dc, PRF_CLIENT );
    dc.DeleteDC();*/

	OnFilePrint();
 }

void TWorkView::OnWStepMode()
 {   
   m_bFlStepMode = ((CButton*)m_pHorDlgb->GetDlgItem( ICM_WRK_STEPMODE ))->GetCheck();

   ((CButton*)m_pHorDlgb->GetDlgItem( ICM_WRK_ANIMATE ))->SetCheck( 0 );
   m_bAnimMode = false;
 }

void TWorkView::OnWStep()
 {
   m_evStep.SetEvent();
 }



void TWorkView::OnRulOpt()
 {
   TAutoMemoryRc<TErgoCategory*> arc1 = new TErgoCategory( "Окно базы правил" );
   TAutoMemoryRc<TErgoCategory*> arc2 = new TErgoCategory( "Список переменных" );
   TAutoMemoryRc<TErgoCategory*> arc3 = new TErgoCategory( "Список фактов" );

   TUserLocal *pUsl = ((TMainFrame*)(AfxGetApp()->m_pMainWnd))->GetCurUser();

   *arc1() + 
	 pUsl->m_ergo_LSPROD_DigConst() + pUsl->m_ergo_LSPROD_StrConst() +
	 pUsl->m_ergo_LSPROD_KeyWord() + pUsl->m_ergo_LSPROD_Var() + 
	 pUsl->m_ergo_LSPROD_Func() +
	 pUsl->m_ergo_LSPROD_Oper() +
	 pUsl->m_ergo_LSPROD_Other();

   *arc2() + 
	 pUsl->m_ergo_LPVAR_Normal() +
     pUsl->m_ergo_LPVAR_NotNeed() +
	 pUsl->m_ergo_LPVAR_NotInit() +
	 pUsl->m_ergo_LPVAR_Canceled() +
	 pUsl->m_ergo_LPVAR_Cursor();

   *arc3() + 
	 pUsl->m_ergo_LPFAC_Normal() + pUsl->m_ergo_LPFAC_Cursor();

   TErgoDlg dlg( this );

   TErgoCategory *p1 = arc1(), *p2 = arc2(), *p3 = arc3();
   arc1.ClearRc(), arc2.ClearRc(), arc3.ClearRc();
   dlg + p1 + p2 + p3;

   if( dlg.DoModal() == IDOK ) 
	{
	  if( dlg.m_bChangedFont == false )	   
		m_lstProd.InvalidateRect( NULL ),
		m_lstFac.InvalidateRect( NULL ),
		m_lstVar.InvalidateRect( NULL );   
	  else
	   {
	     m_lstProd.InvalidateRect( NULL );
		 m_lstFac.InvalidateRect( NULL );
		 m_lstVar.InvalidateRect( NULL );   
	      /*TWndDrwLock lockR( this );
	    
	      m_lstProd.DestroyWindow(); m_lstProd.Detach();
		  m_lstFac.DestroyWindow(); m_lstFac.Detach();
		  m_lstVar.DestroyWindow(); m_lstVar.Detach();

	      CRect r( 0, 0, 100, 100 );
		  m_lstProd.Create( WS_TABSTOP|WS_CHILD|WS_VISIBLE|LBS_OWNERDRAWFIXED|LBS_NOTIFY|LBS_NOINTEGRALHEIGHT|LBS_HASSTRINGS|WS_VSCROLL|WS_HSCROLL,
			r, this, 11 );	
		  m_lstProd.ModifyStyleEx( 0, WS_EX_CLIENTEDGE );

		  m_lstFac.Create( WS_TABSTOP|WS_CHILD|WS_VISIBLE|LBS_OWNERDRAWFIXED|LBS_NOTIFY|LBS_NOINTEGRALHEIGHT|LBS_HASSTRINGS|WS_VSCROLL|WS_HSCROLL,
			r, this, 12 );	
		  m_lstFac.ModifyStyleEx( 0, WS_EX_CLIENTEDGE );

		  m_lstVar.Create( WS_TABSTOP|WS_CHILD|WS_VISIBLE|LBS_OWNERDRAWFIXED|LBS_NOTIFY|LBS_NOINTEGRALHEIGHT|LBS_HASSTRINGS|WS_VSCROLL|WS_HSCROLL,
			r, this, 13 );	
		  m_lstVar.ModifyStyleEx( 0, WS_EX_CLIENTEDGE );   

		  FillListProd( m_lstProd, m_pWA->m_vecProd );   
		  FillListFac( m_lstFac, m_pWA->m_vecWork );
		  FillListVar( m_lstVar, m_pWA->m_arcNam() );	*/
	   }
	}
 }

void TWorkView::OnRulExit()
 {
   SwitchBackView();
 }

void TWorkView::SwitchBackView()
 {  
   CWnd *pW = GetParent();
   if( !pW ) return;

   //if( MyPreclose() == false ) return;

   pW->PostMessage( globl_iMsgSwitchViews, TSWT_Explorer, 
	 (LPARAM)((TExpertDoc*)GetDocument())->GetESystem() );
 }



TFacList::TFacList(): CListBox()
 {
   m_pUsl = ((TMainFrame*)(AfxGetApp()->m_pMainWnd))->GetCurUser();
 }

int TFacList::OnCreate( LPCREATESTRUCT lpCreateStruct )
 {
   if( CListBox::OnCreate(lpCreateStruct) == -1 )
     return -1;
   
   return 0; 
 }

BOOL TFacList::OnEraseBkgnd( CDC *pDC )
 {      
   //return Default();

   CRect r;
   int iCnt = GetCount();
   if( iCnt == LB_ERR || iCnt < 1 ) GetClientRect( r );
   else
	{
	  CRect rr;
      GetItemRect( iCnt - 1, rr );
	  GetClientRect( r );
	  r.top = rr.bottom;
	}

   if( r.top < r.bottom )
     pDC->FillSolidRect( r, RGB(0,0,128) );
 
   return TRUE;   
 }


void TFacList::DrawItem( LPDRAWITEMSTRUCT lpDr )
 {    
   CString str; GetText( lpDr->itemID, str );

   CRect r( lpDr->rcItem );
   int xMax = lpDr->rcItem.right;
   
   CDC dc; dc.Attach( lpDr->hDC );  

   CFont *pFOld = dc.SelectObject( m_pUsl->GetItemFnt(m_pUsl->m_ergo_LPFAC_Normal()) );
         
   dc.SetBkMode( OPAQUE );
   TErgoItem *pIt = (lpDr->itemState & ODS_FOCUS) ? m_pUsl->m_ergo_LPFAC_Cursor():
     m_pUsl->m_ergo_LPFAC_Normal();

   dc.SetTextColor( pIt->m_clrFore );
   dc.SetBkColor( pIt->m_clrBk );

	  
   dc.DrawText( str, r, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX );
   CSize sz = dc.GetTextExtent( str );


   int iHor = GetScrollPos( SB_HORZ );
   xMax += iHor;
   
   r.right = r.left + sz.cx;
   if( r.right < xMax )
	{
      r.left = r.right, r.right += (xMax - r.right);
	  dc.FillSolidRect( r, pIt->m_clrBk );
	}

   dc.SelectObject( pFOld );

   dc.Detach();   
 }

void TFacList::MeasureItem( LPMEASUREITEMSTRUCT lpM )
 {
   CWindowDC dc( GetDesktopWindow() );

   CFont *pFOld = dc.SelectObject( m_pUsl->GetItemFnt(m_pUsl->m_ergo_LPFAC_Normal()) );
   CString str( "ABC" );
   CSize sz = dc.GetTextExtent( str );   

   lpM->itemWidth = sz.cx;
   lpM->itemHeight = sz.cy;   

   dc.SelectObject( pFOld );
 }


void TVarList::SelectVar( TVariantVar* pV )
 {
   for( int i = GetCount() - 1; i > -1; --i )
	 if( GetItemData(i) == (DWORD)pV )
	  {
	    SetCurSel( i ); 
		CRect r;
		GetItemRect( i, r );
		InvalidateRect( r ); return;
	  }
 }

TVarList::TVarList(): CListBox()
 {
   m_pUsl = ((TMainFrame*)(AfxGetApp()->m_pMainWnd))->GetCurUser();
 }

int TVarList::OnCreate( LPCREATESTRUCT lpCreateStruct )
 {
   if( CListBox::OnCreate(lpCreateStruct) == -1 )
     return -1;

   bmpBr.LoadBitmap( IDB_BMP_LSPATTERN );
   m_br.CreatePatternBrush( &bmpBr );

   m_pRed.CreatePen( PS_SOLID, 2, RGB(128,0,0) );
   m_pGreen.CreatePen( PS_SOLID, 2, RGB(0,128,0) );   
   m_pRed2.CreatePen( PS_SOLID, 2, RGB(255,0,0) );
   
   return 0; 
 }

BOOL TVarList::OnEraseBkgnd( CDC *pDC )
 {      
   //return Default();

   CRect r;
   int iCnt = GetCount();
   if( iCnt == LB_ERR || iCnt < 1 ) GetClientRect( r );
   else
	{
	  CRect rr;
      GetItemRect( iCnt - 1, rr );
	  GetClientRect( r );
	  r.top = rr.bottom;
	}

   if( r.top < r.bottom )
     pDC->FillSolidRect( r, RGB(0,0,128) );
 
   return TRUE;   
 }

TVA_ItemStateVar TVarList::GetItemFlag( TVariantVar* pV )
 {
   int iFlag = VA_Normal;
   if( pV->m_bNeed == false ) iFlag = VA_NotNeed;
   else if( pV->m_bCanceled == true ) iFlag = VA_Canceled;
   else if( pV->m_bFIni == false ) iFlag = VA_NotInit;

   return (TVA_ItemStateVar)iFlag;
 }

void TVarList::DrawItem( LPDRAWITEMSTRUCT lpDr )
 {    
   CString str; //GetText( lpDr->itemID, str );

   TVariantVar *pVar = (TVariantVar*)GetItemData( lpDr->itemID );

   CString str2;
   pVar->ValToStr( str2 );

   if( pVar->m_bFIni == true )
	 str.Format( "%s = %s :%1.2f", (LPCSTR)pVar->m_strName, 
	   (LPCSTR)str2, pVar->m_dKD );	   	   
   else
	 str.Format( "%s = %s", (LPCSTR)pVar->m_strName, 
	   (LPCSTR)str2 );	   	   

   //CRect r( lpDr->rcItem );
   //int xMax = lpDr->rcItem.right;

   CRect r( lpDr->rcItem );
   int xMax = lpDr->rcItem.right;
   r.right = r.left + m_iLeft;
   
   CDC dc; dc.Attach( lpDr->hDC );  
   DrawLbl( dc, r, (TVA_ItemStateVar)GetItemFlag(pVar) );
   r.left = r.right; r.right = xMax;

   CFont *pFOld = dc.SelectObject( m_pUsl->GetItemFnt(m_pUsl->m_ergo_LPVAR_Normal()) );
         
   dc.SetBkMode( OPAQUE );
   TErgoItem *pIt =NULL;
   if( lpDr->itemState & ODS_FOCUS )
	 pIt = m_pUsl->m_ergo_LPVAR_Cursor();
   else
	{
      switch( GetItemFlag(pVar) )
	   {
	     case VA_Normal:
		   pIt = m_pUsl->m_ergo_LPVAR_Normal();
		   break;

		 case VA_NotNeed:
		   pIt = m_pUsl->m_ergo_LPVAR_NotNeed();
		   break;

		 case VA_NotInit:
		   pIt = m_pUsl->m_ergo_LPVAR_NotInit();
		   break;

		 case VA_Canceled:
		   pIt = m_pUsl->m_ergo_LPVAR_Canceled();
		   break;
	   }
	}
     

   dc.SetTextColor( pIt->m_clrFore );
   dc.SetBkColor( pIt->m_clrBk );

	  
   dc.DrawText( str, r, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX );
   CSize sz = dc.GetTextExtent( str );


   int iHor = GetScrollPos( SB_HORZ );
   xMax += iHor;
   
   r.right = r.left + sz.cx;
   if( r.right < xMax )
	{
      r.left = r.right, r.right += (xMax - r.right);
	  dc.FillSolidRect( r, pIt->m_clrBk );
	}

   dc.SelectObject( pFOld );

   dc.Detach();   
 }

void TVarList::MeasureItem( LPMEASUREITEMSTRUCT lpM )
 {
   CWindowDC dc( GetDesktopWindow() );

   CFont *pFOld = dc.SelectObject( m_pUsl->GetItemFnt(m_pUsl->m_ergo_LPVAR_Normal()) );
   CString str( "ABC" );
   CSize sz = dc.GetTextExtent( str );   

   lpM->itemWidth = sz.cx;
   lpM->itemHeight = sz.cy;   

   dc.SelectStockObject( DEFAULT_GUI_FONT );
   sz = dc.GetTextExtent( str );   
   m_iLeft = sz.cx;

   dc.SelectObject( pFOld );
 }

//enum TVA_ItemStateVar { VA_Normal, VA_NotNeed, VA_NotInit, VA_Canceled };
void TVarList::DrawLbl( CDC& rDC, CRect& r, TVA_ItemStateVar typ )
 {
   rDC.FillRect( r, &m_br );
   CRect rr( r ); 
   switch( typ )
	{
	  case VA_Canceled:
	   {         
	     int iW = r.Width(), iH = r.Height();
		 if( iW < iH )		  
           rr.DeflateRect( 3, 2 + iH - iW );
		 else
		   rr.DeflateRect( 2 + iW - iH, 3 );
		  
		 CPen *pOld = rDC.SelectObject( &m_pRed );

		 rDC.MoveTo( rr.left, rr.top );
		 rDC.LineTo( rr.right, rr.bottom );
		 rDC.MoveTo( rr.left, rr.bottom );
		 rDC.LineTo( rr.right, rr.top );

		 rDC.SelectObject( pOld );
	   }
	  break;

	  case VA_NotNeed:
	   {
	     int iW = r.Width(), iH = r.Height();		 
         rr.DeflateRect( 4, 0 );
		 
		 CPen *pOld = rDC.SelectObject( &m_pRed2 );
		 
		 rDC.MoveTo( rr.left, rr.top + rr.Height()/2 );
		 rDC.LineTo( rr.right, rr.top + rr.Height()/2 );		 

		 rDC.SelectObject( pOld );
	   }
	   break;

	  case VA_Normal:
	   {
         int iW = r.Width(), iH = r.Height();
		 if( iW < iH )		  
           rr.DeflateRect( 3, 2 + iH - iW );
		 else
		   rr.DeflateRect( 2 + iW - iH, 3 );
		  
		 CPen *pOld = rDC.SelectObject( &m_pGreen );

		 rDC.MoveTo( rr.left, rr.top + rr.Height()/1.5 );
		 rDC.LineTo( rr.left + rr.Width()/2, rr.bottom );		 
		 rDC.LineTo( rr.right, rr.top );

		 rDC.SelectObject( pOld );
	   }
	  break;	  
	}
 }

void TWorkView::LetsGo()
 {
   m_evStep.ResetEvent(), m_evStop.ResetEvent();

   TVariantVar *pvGoal = m_pWA->m_vecQ[m_iGoal]->m_vecVarQ[0]->m_pVar;

   if( !pvGoal )
	{
      MessageBox( "Нет цели - ошибочное имя переменной", "Ошибка",
	      MB_OK|MB_ICONINFORMATION );
	  return;
	}

   m_iDeepRec = m_iDeepRecMax = m_iUsedRules = 0;

   list<TVariantVar*> lstNeed;
   DetectNeedVars( lstNeed );

   ResetNeedFlag( m_pWA->m_arcNam() );

   list<TVariantVar*>::iterator itSta( lstNeed.begin() );
   list<TVariantVar*>::iterator itEnd( lstNeed.end() );
   for( ; itSta != itEnd; ++itSta )
	 (*itSta)->m_bNeed = true;

   pvGoal->m_bNeed = true;

   m_lstVar.InvalidateRect( NULL );

   //m_pWA->ClearVarTree();
   TTreeCont treeCont;
   MakeStartTree( treeCont.m_lstTrees );
   
   list<TSimpleProduct*> m_lstUsedProd;
   ClearRes();   
   TFactWork *pFFnd = FindInFacts( pvGoal );
   if( pFFnd ) AddResult( pFFnd->m_pVar, treeCont.m_lstTrees );

   m_pPrevProd = NULL;

   int iUsRuls = 0;
   try {
     Go2( m_lstUsedProd, iUsRuls, treeCont.m_lstTrees );
	 FillTree( m_tree, &m_lstRes );  
	 m_bFlHaveResult = true;
	}
   catch( CException *pE )
	{
	  pE->ReportError();
	  pE->Delete();	 
	}   
   catch( TMyException *pE )
	{
	  if( pE->GetSubCode() == TREG_TStopException )
	    MessageBox( "Пользователь прервал вывод", "Прекращение вывода", 
	      MB_OK|MB_ICONINFORMATION );
	  else
        MessageBox( pE->GetMsg(), "Ошибка", 
	       MB_OK|MB_ICONSTOP );
	  pE->Delete();	  
	}   
  
   ClearLstProStat( NULL );
   FillListProd( m_lstProd, m_pWA->m_vecProd );   
 }

void TWorkView::ClearLstProStat( list<TSimpleProduct*>* pLst )
 {
   //TWndDrwLock lockR( &m_lstProd );

   if( !pLst )
	{
	  int iCnt = m_lstProd.GetCount();

	  for( int i = 0; i < iCnt; ++i )	
		if( m_lstProd.GetItemData(i) != IT_Normal ) 
		 {
		   m_lstProd.SetItemData( i, IT_Normal );
		   CRect r; m_lstProd.GetItemRect( i, r );
		   m_lstProd.InvalidateRect( r );
		 }	  	
	}
   else
	{
	  vector<TSimpleProduct*>::iterator itSta( m_pWA->m_vecProd.begin() );
	  vector<TSimpleProduct*>::iterator itEnd( m_pWA->m_vecProd.end() );

	  int k = 0;
	  for( ; itSta != itEnd; ++itSta, ++k )
	   {
         list<TSimpleProduct*>::iterator it = 
		   find( pLst->begin(), pLst->end(), *itSta );		 

		 m_lstProd.SetItemData( k, it != pLst->end() ? IT_Used:IT_Normal );
		 CRect r; m_lstProd.GetItemRect( k, r );
		 m_lstProd.InvalidateRect( r );
	   }		
	}
 }




bool TWorkView::TestVars( TExpression *pE, TVariantVar *pVarNot, list<TTreeVar*>& rVarsTree )
 {   
   list<TNode*>& rL = pE->GetRefList();
   list<TNode*>::iterator itSta( rL.begin() );
   list<TNode*>::iterator itEnd( rL.end() );

   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->IsA() == TN_Var )
	  {
	    TVariantVar *pV = &(((TNodeVar*)(*itSta))->m_rVariable);
		if( pV->m_bFIni == false && pV->m_bConst == false && 
		    (DWORD)pV != (DWORD)pVarNot 
	 	  )
		 {
		   if( pV->m_bCanceled == true || 
			   pV->m_bNeed == false ||
			   pV->m_bCanImplicat == true
			 ) return false;

           bool bCanceled = pV->m_bCanceled;
		   bool bNeed = pV->m_bNeed;
		   bool bConst = pV->m_bConst;
		   bool bCanImplicat = pV->m_bCanImplicat;

		   TAskVarDlg dlg( this, IDD_DLG_ASKWORK, pV );
           if( dlg.DoModal() == IDCANCEL )
			{
              pV->m_bCanceled = true;
			  m_lstVar.SelectVar( pV );
			  return false;
			}

		   pV->m_bCanceled = bCanceled;
		   pV->m_bNeed = bNeed;
		   pV->m_bConst = bConst;
		   pV->m_bCanImplicat = bCanImplicat;

		   pV->m_dKD = dlg.m_dKD;

		   TFactWork *pF = new TFactWork();
		   pF->m_strComment = "Введено пользователем";
		   pF->m_dKD = dlg.m_dKD;
		   pF->m_strVar = pV->m_strName;
		   pF->m_pVar = pV;
		   CString str5; pV->ValToStr( str5 );
		   pF->m_strFac.Format( "Введено: %s = %s :%1.2f", (LPCSTR)pV->m_strName, (LPCSTR)str5, dlg.m_dKD );
		   m_pWA->m_vecWork.push_back( pF );		   

		   TTreeVar *pp = new TTreeVar( pV );
		   TTreeVarItem varIt( pF, false, pV );
	       pp->m_lst.push_back( varIt );
           rVarsTree.push_back( pp );


		   FillListFac( m_lstFac, m_pWA->m_vecWork );
		   m_lstFac.SetCurSel( m_lstFac.GetCount() - 1 );

		   m_lstVar.SelectVar( pV );
		 }
	  }

	return true;
 }

bool TWorkView::ScasForCannotUsedVar( TSimpleProduct *pP, list<TTreeVar*>& rVarsTree )
 {
   vector<TExpressionFram*>::iterator itSta( pP->m_vecPos.begin() );
   vector<TExpressionFram*>::iterator itEnd( pP->m_vecPos.end() );

   for( ; itSta != itEnd; ++itSta )
	 if( TestVars((*itSta)->m_arcExpr(), NULL, rVarsTree) == false ) return false;

   return TestVars( pP->m_arcImpl(), pP->m_pImplVar, rVarsTree );
 }

void TWorkView::CopyUsed( list<TSimpleProduct*>& rDst, list<TSimpleProduct*>& rSrc, 
						  bool bFlRemove, TVariantVar *pV )
 {
   list<TSimpleProduct*>::iterator itSta( rSrc.begin() );
   list<TSimpleProduct*>::iterator itEnd( rSrc.end() );

   TExpression* pE;
   for( ; itSta != itEnd; ++itSta )
	 if( !(bFlRemove == true && ((*itSta)->FindVarInPos(pV, pE) || pE)) )
	   rDst.push_back( *itSta );
 }

int TStopException::GetSubCode() const
 {
   return TSC_Subcode;
 }

TStopException::TStopException( BOOL bFlDel, int iExcCode ):
   TMyException( iExcCode, bFlDel )
 {    
   m_strMsg.Format( "Прерывание вывода пользователем" ); 
 }


void ThrowStop()
 {
   TStopException *pE = new TStopException( TRUE, 0 );
   throw pE;      
 }

TTreeCont::~TTreeCont()
 {
   list<TTreeVar*>::iterator itSta( m_lstTrees.begin() ); 
   list<TTreeVar*>::iterator itEnd( m_lstTrees.end() ); 

   for( ; itSta != itEnd; itSta++ )
	 delete *itSta;

   m_lstTrees.clear();
 }

TTreeCont::TTreeCont( list<TTreeVar*>& rLst, TVariantVar *pV )
 {
   list<TTreeVar*>::iterator itSta( rLst.begin() ); 
   list<TTreeVar*>::iterator itEnd( rLst.end() ); 

   for( ; itSta != itEnd; itSta++ )
	 if( (DWORD)((*itSta)->m_pVar) != (DWORD)pV )
	  {
		TTreeVar *p = new TTreeVar( **itSta );
		m_lstTrees.push_back( p );
	  }
 }
   
TTreeVar::TTreeVar( TTreeVar& rV )
 {
   m_pVar = rV.m_pVar;

   list<TTreeVarItem>::iterator itSta( rV.m_lst.begin() ); 
   list<TTreeVarItem>::iterator itEnd( rV.m_lst.end() ); 

   for( ; itSta != itEnd; itSta++ )
	 m_lst.push_back( *itSta );   
 }

void TWorkView::ImplTree( TSimpleProduct* pP, list<TTreeVar*>& rLst )
 {
   TTreeVar *p = new TTreeVar( pP->m_pImplVar );

   pP->ClearUsedFlag();

   vector<TExpressionFram*>::iterator itSta( pP->m_vecPos.begin() );
   vector<TExpressionFram*>::iterator itEnd( pP->m_vecPos.end() );

   for( ; itEnd != itSta; ++itSta )
	{
	  list<TNode*>& rL = (*itSta)->m_arcExpr()->GetRefList();

	  list<TNode*>::iterator itSta2( rL.begin() );
	  list<TNode*>::iterator itEnd2( rL.end() );
	  
	  for( ; itSta2 != itEnd2; ++itSta2 )
		if( (*itSta2)->IsA() == TN_Var )
		 {
           TNodeVar *pNV = (TNodeVar*)*itSta2;
		   if( pNV->m_rVariable.m_bConst == true ||
			   pNV->m_rVariable.m_bFIni == false
			 ) continue;

		   list<TTreeVar*>::iterator itSta3( rLst.begin() );
	       list<TTreeVar*>::iterator itEnd3( rLst.end() );
		   bool bFlFnd = false;
		   for( ; itSta3 != itEnd3; ++itSta3 )
			 if( (DWORD)((*itSta3)->m_pVar) == (DWORD)&(pNV->m_rVariable) )
			  {
			    bFlFnd = true; break;
			  }

		   if( bFlFnd == false ) 
			{
			  /*TTreeVar *pp = new TTreeVar( pNV->m_rVariable );
			  pp->m_lst.push_back( TTreeVarItem(*itSta, false, pNV->m_rVariable) );
			  p->m_lst.push_back( pp );*/
			  continue;
			}

		   if( (*itSta3)->m_pVar->m_bUsed == true ) continue;
		   (*itSta3)->m_pVar->m_bUsed = true;
		   AddTreeVar( p->m_lst, (*itSta3)->m_lst );
		 }
	}

   list<TNode*>& rL = pP->m_arcImpl()->GetRefList();

   list<TNode*>::iterator itSta2( rL.begin() );
   list<TNode*>::iterator itEnd2( rL.end() );
   
   for( ; itSta2 != itEnd2; ++itSta2 )
	 if( (*itSta2)->IsA() == TN_Var )
	  {
        TNodeVar *pNV = (TNodeVar*)*itSta2;
		if( pNV->m_rVariable.m_bConst == true ||
			pNV->m_rVariable.m_bFIni == false ||
			(DWORD)&pNV->m_rVariable == (DWORD)pP->m_pImplVar
		  ) continue;

		list<TTreeVar*>::iterator itSta3( rLst.begin() );
	    list<TTreeVar*>::iterator itEnd3( rLst.end() );
		bool bFlFnd = false;
		for( ; itSta3 != itEnd3; ++itSta3 )
		  if( (DWORD)((*itSta3)->m_pVar) == (DWORD)&(pNV->m_rVariable) )
		   {
			 bFlFnd = true; break;
		   }

		if( bFlFnd == false ) 
		 {
		   /*TTreeVar *pp = new TTreeVar( pNV->m_rVariable );
		   pp->m_lst.push_back( TTreeVarItem(*itSta, false, pNV->m_rVariable) );
		   p->m_lst.push_back( pp );*/
		   continue;
		 }

		if( (*itSta3)->m_pVar->m_bUsed == true ) continue;
		(*itSta3)->m_pVar->m_bUsed = true;
		AddTreeVar( p->m_lst, (*itSta3)->m_lst );
	  }

   TTreeVarItem trVar( pP, true, pP->m_pImplVar );
   p->m_lst.push_back( trVar );
   rLst.push_back( p );

   TVariantVar *pvGoal = m_pWA->m_vecQ[m_iGoal]->m_vecVarQ[0]->m_pVar;
   if( (DWORD)(pP->m_pImplVar) == (DWORD)pvGoal )
	{
      AddResult( pP->m_pImplVar, rLst );
	  FillTree( m_tree, &m_lstRes );  
	}
 }

void TWorkView::AddTreeVar( list<TTreeVarItem>& rTo, list<TTreeVarItem>& rFrom )
 {
   list<TTreeVarItem>::iterator itSta3( rFrom.begin() );
   list<TTreeVarItem>::iterator itEnd3( rFrom.end() );
   
   for( ; itSta3 != itEnd3; ++itSta3 )
	 rTo.push_back( *itSta3 );
 }

void TWorkView::MarkProduct( TSimpleProduct* pP, list<TSimpleProduct*>* pLstUsed )
 {
   int iCnt = m_lstProd.GetCount();

   for( int i = 0; i < iCnt; ++i )
	{
      if( m_lstProd.GetItemData(i) == IT_PrivStep ) 
	   {
	     if( pLstUsed )
		  {
	        list<TSimpleProduct*>::iterator it = find( pLstUsed->begin(), 
			   pLstUsed->end(), m_pWA->m_vecProd[i] );
		    if( it != pLstUsed->end() ) 
	          m_lstProd.SetItemData( i, IT_Used );
			else m_lstProd.SetItemData( i, IT_Normal );
		  }
		 else m_lstProd.SetItemData( i, IT_Normal );

		 CRect r; m_lstProd.GetItemRect( i, r );
		 m_lstProd.InvalidateRect( r );
	   }
	  else if( m_lstProd.GetItemData(i) == IT_CurrentStep ) 
	   { 
	     if( pLstUsed )
		  {
	        list<TSimpleProduct*>::iterator it = find( pLstUsed->begin(), 
			   pLstUsed->end(), m_pWA->m_vecProd[i] );
		    if( it != pLstUsed->end() ) 
	          m_lstProd.SetItemData( i, IT_Used );
			else m_lstProd.SetItemData( i, IT_PrivStep );
		  }
		 else m_lstProd.SetItemData( i, IT_PrivStep );
	     
		 CRect r; m_lstProd.GetItemRect( i, r );
		 m_lstProd.InvalidateRect( r );
	   }
	}

   vector<TSimpleProduct*>::iterator itSta( m_pWA->m_vecProd.begin() );
   vector<TSimpleProduct*>::iterator itEnd( m_pWA->m_vecProd.end() );

   int k = 0;
   for( ; itSta != itEnd; ++itSta, ++k )
	 if( (DWORD)*itSta == (DWORD)pP )
	  {
        m_lstProd.SetItemData( k, (DWORD)IT_CurrentStep  );
		m_lstProd.SetCurSel( k );
		CRect r; m_lstProd.GetItemRect( k, r );
		m_lstProd.InvalidateRect( r );
		break;
	  }		
 }

void TWorkView::ShowPos( int iCnt, bool bStat, bool bAction, bool bRecurse )
 {   
   DWORD dw = m_lstProd.GetItemData( iCnt );   
   if( dw == LB_ERR ) return;
   int iCntIt = m_lstProd.GetCount() - 1;

   TWndDrwLock lockR( &m_lstProd );

   CString str;
   TSimpleProduct *pSP = m_pWA->m_vecProd[ iCnt ];
   if( bAction == true )
	{
      str.Format( bStat == true ? "ЕСЛИ %s ТО %s :%1.2f (срабатывает)":"ЕСЛИ %s ТО %s :%1.2f (не срабатывает)", 
	     (LPCSTR)(pSP->m_strPos), (LPCSTR)(pSP->m_strImpl), double(pSP->m_dKDImpl) );
	  if( bStat == true && bRecurse ) str += " + рекурсия";
	}
   else
	 str.Format( "ЕСЛИ %s ТО %s :%1.2f",
	    (LPCSTR)(pSP->m_strPos), (LPCSTR)(pSP->m_strImpl), double(pSP->m_dKDImpl) );

   m_lstProd.DeleteString( iCnt );
   if( iCntIt == iCnt )
     m_lstProd.SetItemData( m_lstProd.AddString(str), dw );
   else
     m_lstProd.SetItemData( m_lstProd.InsertString(iCnt, str), dw );

   m_lstProd.UpdateExtent( str );
 }

void TWorkView::Go2( list<TSimpleProduct*>& rLstUsed, int& rUsedRules, list<TTreeVar*>& rLstTrees  )
 {  
    if( m_bFlStepMode || m_bAnimMode )
      ClearLstProStat( &rLstUsed );		

    m_iUsedRules = rUsedRules = rLstUsed.size();   
	if( m_bFlStepMode || m_bAnimMode )
	  RefreshUsedRules();

    __int64 dwTick = 0;

	bool bFlUsed = true;
	while( bFlUsed == true )
	 {
       bFlUsed = false;

	   vector<TSimpleProduct*>::iterator itSta( m_pWA->m_vecProd.begin() );
	   vector<TSimpleProduct*>::iterator itEnd( m_pWA->m_vecProd.end() );

	   int iCntPro = 0;
	   for( ; itSta != itEnd; ++itSta, ++iCntPro )
		{		                      
		  if( m_bAnimMode ) dwTick = ::GetTickCount();

		  list<TSimpleProduct*>::iterator it = find( rLstUsed.begin(), rLstUsed.end(), *itSta );
		  if( it != rLstUsed.end() ) continue;

		  if( m_bFlStepMode || m_bAnimMode )
		    MarkProduct( *itSta, &rLstUsed );		  

		  if( ScasForCannotUsedVar(*itSta, rLstTrees) == false ) continue;

		  bool bFlTryPos =  (*itSta)->CalcPos();
		  if( m_bFlStepMode || m_bAnimMode )
		    ShowPos( iCntPro, bFlTryPos, true, (*itSta)->m_pImplVar->m_bFIni );

		  if( m_bFlStepMode == true )
		   {
		     CSyncObject* sync[2] = { &m_evStop, &m_evStep };
		     CMultiLock lock( sync, 2 );

			 DWORD dwRes = lock.Lock( INFINITE, FALSE );

			 if( dwRes >= WAIT_OBJECT_0 && dwRes <= WAIT_OBJECT_0 + 1 )
			   dwRes -= WAIT_OBJECT_0;
			 else if( dwRes >= WAIT_ABANDONED_0 && dwRes <= WAIT_ABANDONED_0 + 1 )
			  dwRes = -1;

             if( m_bFlStepMode || m_bAnimMode )
			   ShowPos( iCntPro, bFlTryPos, false, (*itSta)->m_pImplVar->m_bFIni );
			 if( dwRes == 0 ) ThrowStop();
		   }
		  else 
		   {
		     __int64 dwDelta = (__int64)::GetTickCount() - dwTick;
			 if( !(m_bAnimMode && dwDelta > 0 && dwDelta < m_64Wait) ) dwDelta = m_64Wait;
			 if( ::WaitForSingleObject(m_evStop, m_64Wait-dwDelta) != WAIT_TIMEOUT )
			  {
				if( m_bFlStepMode || m_bAnimMode )
				  ShowPos( iCntPro, bFlTryPos, false, (*itSta)->m_pImplVar->m_bFIni );
				ThrowStop();
			  }		  
		   }		  
          
		  if( m_bFlStepMode || m_bAnimMode )
		    ShowPos( iCntPro, bFlTryPos, false, (*itSta)->m_pImplVar->m_bFIni );
		  
		  
		  if( bFlTryPos == false ) continue;
		  
		  bFlUsed = true;
		  rLstUsed.push_back( *itSta );		  
		  
		  m_iUsedRules = ++rUsedRules;
		  if( m_bFlStepMode || m_bAnimMode )
		    RefreshUsedRules();

          if( (*itSta)->m_pImplVar->m_bFIni == true )
		   {
		     TKeepVars varKeep( m_pWA->m_arcNam() );             
			 TTreeCont cont( rLstTrees, (*itSta)->m_pImplVar );

             (*itSta)->CalcImpl();
			 m_lstVar.SelectVar( (*itSta)->m_pImplVar );

			 ImplTree( *itSta, cont.m_lstTrees );
			 list<TSimpleProduct*> lstUsed;
			 CopyUsed( lstUsed, rLstUsed, m_bFlRemoveUsedRules, (*itSta)->m_pImplVar );			 
			 			 
			 int iUsRuls = 0;
			 m_iDeepRec++; m_iDeepRecMax = max( m_iDeepRecMax, m_iDeepRec );
			 RefreshDeep();
			 Go2( lstUsed, iUsRuls, cont.m_lstTrees );
			 m_iDeepRec--;
			 RefreshDeep();
			 m_iUsedRules = rUsedRules;
			 if( m_bFlStepMode || m_bAnimMode )
		       RefreshUsedRules();

			 varKeep.Restore();
			 m_lstVar.InvalidateRect( NULL );
		   }
		  else
		   {
		     (*itSta)->CalcImpl();
			 m_lstVar.SelectVar( (*itSta)->m_pImplVar );
			 ImplTree( *itSta, rLstTrees );

			 /*list<TTreeVar*>::iterator itt( rLstTrees.begin() );
			 list<TTreeVar*>::iterator ittE( rLstTrees.end() );
			 for( ; itt != ittE; ++itt )
			  {
				TTreeVar *pvvv = *itt;
			  }*/
		   }
		  
		}
	 }
 }

TKeepVars::TKeepVars( TNameSpace *pSpc )
 {
   m_pSpc = pSpc;

   list<TVariantVar*>::iterator itSta( pSpc->GetLstRef().begin() );
   list<TVariantVar*>::iterator itEnd( pSpc->GetLstRef().end() );
   for( ; itSta != itEnd; ++itSta )
	{
      TVariantVar *p = new TVariantVar();
	  if( (*itSta)->m_bFIni == false || (*itSta)->m_bConst == true )
	    p->m_bFIni = (*itSta)->m_bFIni, p->m_bConst = (*itSta)->m_bConst;
	  else
	    *p = **itSta;
	  m_lstVar.push_back( p );
	}
 }

TKeepVars::~TKeepVars()
 {
   list<TVariantVar*>::iterator itSta( m_lstVar.begin() );
   list<TVariantVar*>::iterator itEnd( m_lstVar.end() );
   for( ; itSta != itEnd; ++itSta )
	 delete *itSta;

   m_lstVar.clear();
 }

void TKeepVars::Restore()
 {
   list<TVariantVar*>::iterator itSta( m_lstVar.begin() );
   list<TVariantVar*>::iterator itEnd( m_lstVar.end() );

   list<TVariantVar*>::iterator itSta2( m_pSpc->GetLstRef().begin() );
   list<TVariantVar*>::iterator itEnd2( m_pSpc->GetLstRef().end() );
   for( ; itSta != itEnd; ++itSta, ++itSta2 )
	 if( (*itSta)->m_bFIni == true && (*itSta)->m_bConst == false )
	   **itSta2 = **itSta;	
	 else if( (*itSta)->m_bFIni == false )
	   (*itSta2)->UnInit();
 }

//private:
  //list<TVariantVar*> m_lstVar;

void TWorkView::AddResult( TVariantVar *pV, list<TTreeVar*>& rL )
 {
   list<TTreeVar*>::reverse_iterator itSta( rL.rbegin() );
   list<TTreeVar*>::reverse_iterator itEnd( rL.rend() );

   for( ; itSta != itEnd; ++itSta )
	 if( (DWORD)(*itSta)->m_pVar == (DWORD)pV )
	  {
	    list<TTreeVarItem>::iterator itSta2( (*itSta)->m_lst.begin() );
		list<TTreeVarItem>::iterator itEnd2( (*itSta)->m_lst.end() );
		
        TAutoMemoryRc<TResults*> arcRes = new TResults();

		for( ; itSta2 != itEnd2; ++itSta2 )
		  arcRes()->m_lstRes.push_back( new TResult(&*itSta2) );

		TResults* p = arcRes(); arcRes.ClearRc();
		m_lstRes.push_back( p );
        return;
	  }
 }

TResults::~TResults()
 {
   Clear();
 }

void TResults::Clear()
 {
   list<TResult*>::iterator itSta( m_lstRes.begin() );
   list<TResult*>::iterator itEnd( m_lstRes.end() );

   for( ; itSta != itEnd; ++itSta )
	 delete *itSta;

   m_lstRes.clear();
 }
   

TResult::TResult( TTreeVarItem* pVt )
 {
   m_Val = pVt->m_Var;
   m_Val.m_strName = pVt->m_Var.m_strName;

   CString str2; pVt->m_Var.ValToStr( str2 );
   m_str.Format( "[%s = %s]:%1.2f - %s", 
	 (LPCSTR)(pVt->m_Var.m_strName), 
	 (LPCSTR)str2, pVt->m_Var.m_dKD,
	 (LPCSTR)(pVt->m_bTyp == true ? pVt->m_pProd->m_strComment:pVt->m_pFac->m_strComment) );

   double dKD = pVt->m_Var.m_dKD;
   if( pVt->m_bTyp == false )
	{
      if( dKD >= 0.67 ) m_trLvl = TR_Fac_High;
	  else if( dKD < 0.67 && dKD >= 0.33 ) m_trLvl = TR_Fac_Med;
	  else m_trLvl = TR_Fac_Low;
	}
   else
	{
      if( dKD >= 0.67 ) m_trLvl = TR_Rul_High;
	  else if( dKD < 0.67 && dKD >= 0.33 ) m_trLvl = TR_Rul_Med;
	  else m_trLvl = TR_Rul_Low;
	}
 }

TFactWork *TWorkView::FindInFacts( TVariantVar *pV )
 {
   vector<TFactWork*>::iterator itSta( m_pWA->m_vecWork.begin() );
   vector<TFactWork*>::iterator itEnd( m_pWA->m_vecWork.end() );

   for( ; itSta != itEnd; ++itSta )
	 if( (DWORD)(*itSta)->m_pVar == (DWORD)pV ) return *itSta;

   return NULL;
 }

void TWorkView::MakeStartTree( list<TTreeVar*>& rL )
 {
   list<TTreeVar*>::iterator itSta( rL.begin() );
   list<TTreeVar*>::iterator itEnd( rL.end() );
 
   for( ; itSta != itEnd; ++itSta )
	{
	  TTreeVar *p = new TTreeVar( (*itSta)->m_pVar );
	  p->m_lst.push_back( TTreeVarItem(*itSta, false, (*itSta)->m_pVar) );
      rL.push_back( p );
	}
 }

void TWorkView::ResetNeedFlag( TNameSpace* pSpc )
 {
   list<TVariantVar*>& rLst = pSpc->GetLstRef();

   list<TVariantVar*>::iterator itSta( rLst.begin() );
   list<TVariantVar*>::iterator itEnd( rLst.end() );
  
   for( ; itSta != itEnd; ++itSta )	
	 (*itSta)->m_bNeed = false;
 }

void TWorkView::DetectNeedVars( list<TVariantVar*>& rLstNeed )
 {   
   list<TVariantVar*>& rLst = m_pWA->m_arcNam()->GetLstRef();
   TVariantVar *pvGoal = m_pWA->m_vecQ[m_iGoal]->m_vecVarQ[0]->m_pVar;

   list<TVariantVar*>::iterator itSta( rLst.begin() );
   list<TVariantVar*>::iterator itEnd( rLst.end() );
     
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->m_bConst == false && (DWORD)*itSta != (DWORD)pvGoal )
	  {	   
		list<TVariantVar*> lstC;
		lstC.push_back( *itSta );
		BuildC( lstC );
		if( FindInNeed(lstC, pvGoal) ) 
		  rLstNeed.push_back( *itSta );
	  }    
 }

void TWorkView::BuildC( list<TVariantVar*>& rLstNeed )
 {  
   bool bFlUpdated = true;
   while( bFlUpdated == true )
	{
      bFlUpdated = false;

	  list<TVariantVar*>::iterator itSta( rLstNeed.begin() );
	  list<TVariantVar*>::iterator itEnd( rLstNeed.end() );

	  for( ; itSta != itEnd; ++itSta )
	   {
	     TSimpleProduct *pProdFnd =
           FindVarInPos( m_pWA, *itSta );
		 if( pProdFnd && AddAllImplTo(pProdFnd, rLstNeed) ) 
		  {
		    bFlUpdated = true;
		    break;
		  }
	   }
	}
 }

bool TWorkView::FindInNeed( list<TVariantVar*>& rLst, TVariantVar* pV )
 {
   list<TVariantVar*>::iterator itSta( rLst.begin() );
   list<TVariantVar*>::iterator itEnd( rLst.end() );
   
   for( ; itSta != itEnd; ++itSta )
	 if( (DWORD)*itSta  == (DWORD)pV ) return true;

   return false;
 }

bool TWorkView::AddAllImplTo( TSimpleProduct *pProd, list<TVariantVar*>& rLst )
 {
   list<TNode*>& rL = pProd->m_arcImpl()->GetRefList();

   list<TNode*>::iterator itSta( rL.begin() );
   list<TNode*>::iterator itEnd( rL.end() );

   bool bFlAdded = false;
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->IsA() == TN_Var && 
	     ((TNodeVar*)(*itSta))->m_rVariable.m_bConst == false &&
		 FindInNeed( rLst, &(((TNodeVar*)(*itSta))->m_rVariable) ) == false
	   )
	  {
        rLst.push_back( &(((TNodeVar*)(*itSta))->m_rVariable) );
		bFlAdded = true;
	  }

   return bFlAdded;
 }

TSimpleProduct *TWorkView::FindVarInPos( TWorkArea *pWA, TVariantVar *pV )
 {
   vector<TSimpleProduct*>::iterator itSta( pWA->m_vecProd.begin() );
   vector<TSimpleProduct*>::iterator itEnd( pWA->m_vecProd.end() );

   for( ; itSta != itEnd; ++itSta )
	{
	  TExpression *pExpr;
      TExpressionFram *pPos = (*itSta)->FindVarInPos( pV, pExpr );
	  if( pPos || pExpr ) return *itSta;
	}

   return NULL;
 }

