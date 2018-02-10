#include "v_expert.hpp"
#include "viewexp.hpp"
#include "mdiexp.hpp"
#include "mfrexp.hpp"
#include "expdlg.hpp"
#include "docexp.hpp"
#include "subj.hpp"
#include "tlsf_dlg.hpp"
#include "vs.hpp"
#include "dlgacess.hpp"

#include "warea.hpp"

#include "creator.hpp"
#include "cre_dlg.hpp"

#include "rdlg.hpp"
#include "dlgacess.hpp"

#include "globlmsg.hpp"
#include "netfsubj.hpp"

#include <afxcmn.h>
#include <io.h>
#include <afxpriv.h>

#include "vs.rh"

#include "..\\RUDAKOW\\SERVIS\\myexc.hpp"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE( TExpertView, TBaseView )

//static UINT uiCodeMsgHlp = WM_COMMANDHELP;

BEGIN_MESSAGE_MAP( TExpertView, TBaseView )	
  ON_WM_CREATE()
  ON_WM_CTLCOLOR()

  ON_WM_DESTROY()

  ON_MESSAGE(WM_HELPHITTEST , OnCommandHelp)
  ON_MESSAGE( WM_COMMANDHELP, OnCommandHelpF1 )

  ON_NOTIFY( NM_RCLICK, ID_EXP_RULES_TREE, TreeSelect )

  ON_UPDATE_COMMAND_UI( ICM_EXP_NEWOBJ, OnUpdate_Always )
  ON_UPDATE_COMMAND_UI( ICM_EXP_IMPORT, OnUpdate_Always )
  ON_UPDATE_COMMAND_UI( ICM_EXP_REMOVE, OnUpdate_Remove )

  ON_UPDATE_COMMAND_UI( ICM_EXP_EDIT, OnUpdate_Edit )
  ON_UPDATE_COMMAND_UI( ICM_EXP_CLEAR, OnUpdate_Clear )
  ON_UPDATE_COMMAND_UI( ICM_EXP_CLEARALL, OnUpdate_ClearAll )

  ON_UPDATE_COMMAND_UI( ICM_EXP_OPT, OnUpdate_Always )
  ON_UPDATE_COMMAND_UI( ICM_EXP_ACC, OnUpdate_Acc )

  ON_UPDATE_COMMAND_UI( ICM_EXP_MAKERULES, OnUpdate_ToRules )
  ON_UPDATE_COMMAND_UI( ICM_EXP_COMPILE, OnUpdate_Compile )

  ON_UPDATE_COMMAND_UI( ICM_EXP_CONSULT, OnUpdate_Consult )

  ON_COMMAND( ICM_EXP_NEWOBJ, OnNewObj )
  ON_COMMAND( ICM_EXP_IMPORT, OnImport )
  ON_COMMAND( ICM_EXP_REMOVE, OnRemove )

  ON_COMMAND( ICM_EXP_EDIT, OnEdit )
  ON_COMMAND( ICM_EXP_CLEAR, OnClear )
  ON_COMMAND( ICM_EXP_CLEARALL, OnClearAll )

  ON_COMMAND( ICM_EXP_ACC, OnAcc )

  ON_COMMAND( ICM_EXP_MAKERULES, OnMakeRules )
  ON_COMMAND( ICM_EXP_COMPILE, OnCompile )

  ON_COMMAND( ICM_EXP_CONSULT, OnConsult )  
END_MESSAGE_MAP()


static void Quant()
 {
   int res;
   do {
     MSG msg;
     if( (res = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) == TRUE )      
       DispatchMessage( &msg );
    } while( res == TRUE );
 }


LRESULT TExpertView::OnCommandHelp( WPARAM wp, LPARAM lParam )
{
  return 12345;
}

LRESULT TExpertView::OnCommandHelpF1( WPARAM wp, LPARAM lParam )
{
  AfxGetApp()->CWinApp::WinHelp( 2989 );    
  return TRUE;
}


TExpertView::TExpertView(): TBaseView()
 {      
   m_chkNets.SetBkColor( RGB(0xC0, 0xC0, 0xC0) ); 
   m_chkFrames.SetBkColor( RGB(0xC0, 0xC0, 0xC0) );    
 }

void TExpertView::OnUpdate_Acc( CCmdUI* pCmdUI )
 {  
   pCmdUI->Enable( 
	 ((TMainFrame*)(AfxGetApp()->m_pMainWnd))->GetProtectLvl()->GetClass() == TAcessLevel::TU_Administrator
	             );
 }

BOOL TExpertView::CheckForNOItem()
 {
   BOOL bFl = TRUE;

   HWND hw = m_hWndFocus;
   if( hw == m_treeRules.m_hWnd )
	{
      HTREEITEM hCur = m_treeRules.GetSelectedItem();	  
	  if( !hCur || !m_treeRules.GetItemData(hCur) ) bFl = FALSE;
	}
   else if( hw == m_chkNets.m_hWnd ) 
	{
      int ic = m_chkNets.GetCurSel();
	  if( ic == LB_ERR || m_chkNets.GetItemData(ic) == -1 ) bFl = FALSE;
	}
   else if( hw == m_chkFrames.m_hWnd ) 
	{
      int ic = m_chkFrames.GetCurSel();
	  if( ic == LB_ERR || m_chkFrames.GetItemData(ic) == -1 ) bFl = FALSE;
	}	

   return bFl;
 }

void TExpertView::OnUpdate_Remove( CCmdUI* pCmdUI )
 {      
   pCmdUI->Enable( TRUE );
 }
void TExpertView::OnUpdate_Edit( CCmdUI* pCmdUI )
 {
   pCmdUI->Enable( TRUE );
 }
void TExpertView::OnUpdate_Clear( CCmdUI* pCmdUI )
 {
   pCmdUI->Enable( TRUE );
 }
void TExpertView::OnUpdate_ClearAll( CCmdUI* pCmdUI )
 {
   pCmdUI->Enable( TRUE );
 }
void TExpertView::OnUpdate_ToRules( CCmdUI* pCmdUI )
 {
   pCmdUI->Enable( TRUE );
 }
void TExpertView::OnUpdate_Compile( CCmdUI* pCmdUI )
 {
   pCmdUI->Enable( TRUE );
 }
void TExpertView::OnUpdate_Consult( CCmdUI* pCmdUI )
 {
   pCmdUI->Enable( TRUE );
 }

TExpertView::~TExpertView()
 {   
 }

TO_ObjTypes TExpertView::IsObj() const
 {
   return TO_ExpertSys;
 }

LPCSTR TExpertView::GetShortFileName()
 {
   if( !m_pBindSubj ) return "Пусто";
   if( !GetDocument() ) return "";   

   char drive[ _MAX_DRIVE ];
   char dir[ _MAX_DIR ];
   char fname[ _MAX_FNAME ];
   char ext[ _MAX_EXT ];
   char cBuf[ _MAX_PATH ];

   _splitpath( GetDocument()->GetPathName(), drive, dir, fname, ext );
   _makepath( cBuf, "", "", fname, ext );	  	 

   m_strKeepName = cBuf;

   return m_strKeepName;
 }

LPCSTR TExpertView::GetName()
 {
   if( !GetDocument() || !((TExpertDoc*)GetDocument())->GetESystem() ) return "Пусто";

   m_strTmpName.Format( "%s%s", 
	  (LPCSTR)(((TExpertDoc*)GetDocument())->GetESystem())->m_csName,
	  GetDocument()->IsModified() ? "*":"" );

   return m_strTmpName;
 }

void TExpertView::OnDraw( CDC* pDC )
 {
 }

void TExpertView::OnUpdate( CView* pSender, LPARAM lHint, CObject* pHint )
 {
   // ...
   // Implement a GetDocSize( ) member function in 
   // your document class; it returns a CSize.
   //SetScrollSizes( MM_TEXT, ((TGrafDoc*)GetDocument())->GetDocSize( ) );
   //ResizeParentToFit( );   // Default bShrinkOnly argument
   // ...
   TBaseView::OnUpdate( pSender, lHint, pHint );
 }

void TExpertView::OnDestroy()
 {
   TBaseView::OnDestroy();
 }


/*BOOL TExpertView::DestroyWindow()
 {
   return TBaseView::DestroyWindow();
 }*/

#define INTER_SPACE 0.025
#define WIDTH_ 0.3
#define INTER_TOP 0.09
#define HEIGHT_ 0.9

int TExpertView::OnCreate( LPCREATESTRUCT lpCreateStruct )
 {
   static char *cTtlRul = "Продукционные базы знаний";
   static char *cTtlNet = "Сетевые базы знаний";
   static char *cTtlFac = "Фреймовые базы знаний";

   if( TBaseView::OnCreate(lpCreateStruct) == -1 )
     return -1;
    
  //CCheckListBox  m_chkNets, m_chkFrames;
  //CTreeView m_treeRules;
  //CStatic m_ttlTree, m_ttlNets, m_ttlFrame

   m_ilstNormal = new CImageList();

   CBitmap bmRul, bmRulC, bmFac, bmFacC, bmQ, bmQC, bmRoot;
   bmRul.LoadBitmap( IDB_TREE_RUL );
   bmRulC.LoadBitmap( IDB_TREE_RULC );
   bmFac .LoadBitmap( IDB_TREE_FAC );
   bmFacC.LoadBitmap( IDB_TREE_FACC );
   bmQ.LoadBitmap( IDB_TREE_Q );
   bmQC.LoadBitmap( IDB_TREE_QC );         
   bmRoot.LoadBitmap( IDB_TREE_ROOT );         
	  
   BITMAP bm;
   ::GetObject( (HBITMAP)bmRul, sizeof(bm), &bm );

   m_ilstNormal()->Create( bm.bmWidth, bm.bmHeight, 
	                       ILC_MASK | ILC_COLOR4, 7, 1 );
   m_ilstNormal()->Add( &bmRoot, RGB(0,0,128) );	  
   m_ilstNormal()->Add( &bmRul, RGB(0,0,128) );	  
   m_ilstNormal()->Add( &bmFac, RGB(0,0,128) );	  
   m_ilstNormal()->Add( &bmQ, RGB(0,0,128) );	  
     
   m_ilstNormal()->Add( &bmRulC, RGB(0,0,128) );	  
   m_ilstNormal()->Add( &bmFacC, RGB(0,0,128) );
   m_ilstNormal()->Add( &bmQC, RGB(0,0,128) );	    
   m_ilstNormal()->SetBkColor( CLR_NONE );
      
   
   CRect r( 0, 0, 100, 100 );
   m_treeRules.Create( 
	  WS_CHILD|WS_VISIBLE|WS_BORDER|WS_TABSTOP | TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS|TVS_SHOWSELALWAYS,
	  r, this, ID_EXP_RULES_TREE );	   

   m_treeRules.SetImageList( m_ilstNormal(), TVSIL_NORMAL );
   

   m_chkNets.Create( 
	 WS_CHILD|WS_VISIBLE|WS_BORDER|WS_VSCROLL|WS_HSCROLL|WS_TABSTOP | LBS_NOINTEGRALHEIGHT|LBS_HASSTRINGS|LBS_OWNERDRAWFIXED|LBS_SORT|LBS_NOTIFY, 
	 r, this, ID_EXP_NETS_LIST );
   m_chkFrames.Create( 
	 WS_CHILD|WS_VISIBLE|WS_BORDER|WS_VSCROLL|WS_HSCROLL|WS_TABSTOP | LBS_NOINTEGRALHEIGHT|LBS_HASSTRINGS|LBS_OWNERDRAWFIXED|LBS_SORT|LBS_NOTIFY, 
	 r, this, ID_EXP_FRAME_LIST );

   m_treeRules.ModifyStyleEx( 0, WS_EX_CLIENTEDGE );
   m_chkNets.ModifyStyleEx( 0, WS_EX_CLIENTEDGE );
   m_chkFrames.ModifyStyleEx( 0, WS_EX_CLIENTEDGE );


   CFont fnt; fnt.Attach( (HFONT)::GetStockObject(DEFAULT_GUI_FONT) );
   SetFont( &fnt, FALSE );

   m_treeRules.SetFont( &fnt, FALSE );
   m_chkNets.SetFont( &fnt, FALSE );
   m_chkFrames.SetFont( &fnt, FALSE );


   CClientDC dc( this );
   CFont *pFOld = dc.SelectObject( &fnt );
   CSize sz = dc.GetTextExtent( CString(cTtlRul) );

   int iMaxWS = 0, iMaxHS = 0;

   r = CRect( 0, 0, sz.cx, sz.cy );
   m_ttlTree.Create( cTtlRul, 
	  WS_CHILD|WS_VISIBLE, r, this, ID_EXP_TTL_RUL );	
   m_ttlTree.SetFont( &fnt, FALSE );
   iMaxWS += sz.cx;
   iMaxHS = sz.cy;

   sz = dc.GetTextExtent( CString(cTtlNet) );
   r = CRect( 0, 0, sz.cx, sz.cy );
   m_ttlNets.Create( cTtlNet, 
	  WS_CHILD|WS_VISIBLE, r, this, ID_EXP_TTL_NET );	
   m_ttlNets.SetFont( &fnt, FALSE );
   iMaxWS += sz.cx;
   

   sz = dc.GetTextExtent( CString(cTtlFac) );
   r = CRect( 0, 0, sz.cx, sz.cy );
   m_ttlFrame.Create( cTtlFac, 
	  WS_CHILD|WS_VISIBLE, r, this, ID_EXP_TTL_FRAM );
   m_ttlFrame.SetFont( &fnt, FALSE );
   iMaxWS += 1.5*sz.cx;   
   
   sz = dc.GetTextExtent( CString("AAAA") );
   iMaxWS += sz.cx;
   iMaxHS *= 10;

   dc.SelectObject( pFOld );
   fnt.Detach();

   SetMinWH( iMaxWS, iMaxHS );
	

  *this + new TChildItem( &m_treeRules, INTER_SPACE, 1.5*sz.cy, WIDTH_, HEIGHT_ ) +
          new TChildItem( &m_chkNets, 2*INTER_SPACE+WIDTH_, 1.5*sz.cy, WIDTH_, HEIGHT_ ) +
		  new TChildItem( &m_chkFrames, 3*INTER_SPACE+2*WIDTH_, 1.5*sz.cy, WIDTH_, HEIGHT_ ) +

		  new TChildItem( &m_ttlTree,  CH_Top, &m_treeRules ) +
          new TChildItem( &m_ttlNets,  CH_Top, &m_chkNets ) +
		  new TChildItem( &m_ttlFrame, CH_Top, &m_chkFrames );

   MakeLaoyt();
   		  
   return 0; 
 }

HBRUSH TExpertView::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
 {   
   int id = pWnd->GetDlgCtrlID();

   if( id == ID_EXP_RULES_TREE )
      return TBaseView::OnCtlColor( pDC, pWnd, nCtlColor );

   if( nCtlColor == CTLCOLOR_STATIC )
	{	  
	  pDC->SetTextColor( RGB(0,0,255) );
	  pDC->SetBkColor( RGB(255, 255, 255) );
	  return (HBRUSH)GetStockObject( WHITE_BRUSH );
	}
   
   pDC->SetBkColor( RGB(0xC0,0xC0, 0xC0) );
   pDC->SetTextColor( RGB(0,0,0) );
   return (HBRUSH)GetStockObject( LTGRAY_BRUSH );
 }

void TExpertView::OnInitialUpdate()
 {
   TBaseView::OnInitialUpdate();   

   if( m_bFlOwnerCreate == false ) 
	{
      ((TMDIFrame*)GetParent())->Setup_DirectlyView( this );
      m_pBindSubj = (TProtectedObj*)((TExpertDoc*)GetDocument())->GetESystem();
	}

   FillRul();
   FillNets();
   FillFrn();

   m_treeRules.SetItemState( m_treeRules.GetRootItem(), 
	  TVIS_EXPANDED|TVIS_SELECTED, TVIF_STATE );   

   m_treeRules.Expand( m_treeRules.GetRootItem(), TVE_EXPAND );
   
   m_treeRules.SetFocus();

   if( ((TExpertDoc*)GetDocument())->m_pSubjKey )
	{
	  if( ((TExpertDoc*)GetDocument())->m_pSubjKey->IsA() == TPC_TPro ||
	      ((TExpertDoc*)GetDocument())->m_pSubjKey->IsA() == TPC_TQuery ||
		  ((TExpertDoc*)GetDocument())->m_pSubjKey->IsA() == TPC_TFac
		)
		 ExpandTree( ((TExpertDoc*)GetDocument())->m_pSubjKey );
      else if( ((TExpertDoc*)GetDocument())->m_pSubjKey->IsA() == TPC_TNet )
	    RollList( m_chkNets, ((TExpertDoc*)GetDocument())->m_pSubjKey );
	  else if( ((TExpertDoc*)GetDocument())->m_pSubjKey->IsA() == TPC_TFrm )
	    RollList( m_chkFrames, ((TExpertDoc*)GetDocument())->m_pSubjKey );

	  ((TExpertDoc*)GetDocument())->m_pSubjKey = NULL;
	}
   //m_treeRules.SetWindowPos( NULL, 0,0,0,0, 
	 //SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE );
 }




TV_Views TExpertView::IsA() const
 {
   return TV_Expert;
 }

bool TExpertView::CanClose()
 {
   return true;
 }
  


static TBtnTool btnHoriz[] =
 {
  { TB_Bmp, ICM_EXP_NEWOBJ, NULL, IDB_BMP_EXP_NEW, true, "Создать новый объект ЭС", BS_PUSHBUTTON },
  { TB_Bmp, ICM_EXP_IMPORT, NULL, IDB_BMP_EXP_EXP, true, "Подключить к ЭС готовый объект", BS_PUSHBUTTON },
  { TB_Bmp, ICM_EXP_REMOVE, NULL, IDB_BMP_EXP_REMOV, true, "Удалить объект ЭС", BS_PUSHBUTTON },

  { TB_Bmp, ICM_EXP_EDIT, NULL, IDB_BMP_EXP_EDIT, false, "Редактировать выбранный объект", BS_PUSHBUTTON },
  { TB_Bmp, ICM_EXP_CLEAR, NULL, IDB_BMP_EXP_CLEAR, true, "Очистить выделение в текущем списке", BS_PUSHBUTTON },
  { TB_Bmp, ICM_EXP_CLEARALL, NULL, IDB_BMP_EXP_CLEARALL, true, "Очистить всё выделение", BS_PUSHBUTTON },

  { TB_Bmp, ICM_EXP_OPT, NULL, IDB_BMP_EXP_OPT, false, "Настройка эксплорера ЭС", BS_PUSHBUTTON },
  { TB_Bmp, ICM_EXP_ACC, NULL, IDB_BMP_EXP_ACC, true, "Доступ к объекту ЭС", BS_PUSHBUTTON },
  { TB_EndDescr, 0, NULL, 0, true, "", 0 }
 };

static TBtnTool btnVert[] =
 {
  { TB_Bmp, ICM_EXP_MAKERULES, NULL, IDB_BMP_EXP_MAKERULES, true, "Преобразовать помеченные сети и фреймы", BS_PUSHBUTTON },
  { TB_Bmp, ICM_EXP_COMPILE, NULL, IDB_BMP_EXP_COMPILE, true, "Собрать из помеченных баз правил ЭС", BS_PUSHBUTTON },

  { TB_Bmp, ICM_EXP_CONSULT, NULL, IDB_BMP_EXP_CONSULT, false, "Задать вопрос ЭС", BS_PUSHBUTTON },
  { TB_EndDescr, 0, NULL, 0, false, "", 0 }
 };

void TExpertView::SetupHorizBar( CMyDialogBar& rB )
 {     
   rB.SetCurVievClass( TV_Expert );

   ClearBtnsLists(); //only here ! - first calls horizontal setup
   DWORD dwS = rB.GetBarStyle();
   rB.SetBarStyle( (dwS & ~CBRS_BORDER_BOTTOM)|CBRS_BORDER_LEFT|CBRS_BORDER_RIGHT|CBRS_BORDER_TOP );

   CreateBtns( rB, btnHoriz, false, m_lstBmp, m_lstBtn );
 }

void TExpertView::SetupVerBar( CMyDialogBar& rB )
 {
   rB.SetCurVievClass( TV_Expert );

   DWORD dwS = rB.GetBarStyle();
   rB.SetBarStyle( (dwS & ~CBRS_BORDER_RIGHT & ~CBRS_BORDER_TOP)|CBRS_BORDER_LEFT|CBRS_BORDER_BOTTOM );
  
   CreateBtns( rB, btnVert, false, m_lstBmp, m_lstBtn );

   if( rB.GetParent() && rB.GetParent()->IsWindowVisible() &&
       !rB.IsWindowVisible()
	 )
     rB.ShowWindow( SW_SHOW );   
 }

/*CCheckListBox m_chkNets, m_chkFrames;
  CTreeCtrl m_treeRules;
  CStatic m_ttlTree, m_ttlNets, m_ttlFrame;*/

static int GetImageInd( TSubject* pS )
 {
   switch( pS->IsA() )
	{
	  case TPC_TFac:
        return 2;
      case TPC_TQuery:
	    return 3;
	  case TPC_TPro:
	    return 1;
	  default:
	    return 0;
	}
 }

void TExpertView::TreeSelect( NMHDR *pNotifyStruct, LRESULT *result )
 {
   /*NM_TREEVIEW *pNM = (NM_TREEVIEW *)pNotifyStruct;

   if( pNM->iItem != -1 && (pNM->uChanged & LVIF_STATE) && 
	   (pNM->uNewState & LVIS_SELECTED)
	 )  LoadFromField( pNM->iItem );*/

    HTREEITEM hCur = m_treeRules.GetSelectedItem();	  
	DWORD dw;
	if( !hCur || !(dw = m_treeRules.GetItemData(hCur)) ) return;

	((TSubject*)dw)->SetSel( !((TSubject*)dw)->GetSel() );
	int img = GetImageInd( (TSubject*)dw ) + 
	   (((TSubject*)dw)->GetSel() == true ? 3:0);
	m_treeRules.SetItemImage( hCur, img, img );	   
 }

static HTREEITEM FindIt( CTreeCtrl *pTree, HTREEITEM hIt, TSubject* pSubj )
 {   
   while( hIt )
	{
      if( (TSubject*)(pTree->GetItemData(hIt)) == pSubj ) return hIt;

	  HTREEITEM ht = pTree->GetNextItem( hIt, TVGN_CHILD );
	  if( ht ) ht = FindIt( pTree, ht, pSubj );
	  if( ht ) return ht;

	  hIt = pTree->GetNextItem( hIt, TVGN_NEXT );
	}
   return NULL;
 }

void TExpertView::RollList( TMyChLBox& rChb, TSubject* pSubj )
 {
   int iCnt = rChb.GetCount();

   for( int i = 0; i < iCnt; ++i )
	 if( rChb.GetItemData(i) == (DWORD)pSubj )
	  {
        rChb.SetCurSel( i );
		rChb.SetFocus();
		return;
	  }
 }

void TExpertView::ExpandTree( TSubject* pSubj )
 {      
   HTREEITEM hFnd = FindIt( &m_treeRules, m_treeRules.GetRootItem(), pSubj );
   if( hFnd )	 
	 m_treeRules.Expand( hFnd, TVE_EXPAND ), m_treeRules.Select( hFnd, TVGN_CARET );
 }

void TExpertView::FillRul( DWORD dwSel )
 {
   TWndDrwLock lockR( &m_treeRules );

   HTREEITEM hKey = NULL;

   m_treeRules.DeleteAllItems();

   TV_INSERTSTRUCT tv; memset( &tv, 0, sizeof(TV_INSERTSTRUCT) );
   tv.hInsertAfter = TVI_LAST;
   tv.item.state = TVIS_BOLD;
   tv.item.stateMask = TVIS_BOLD;
   tv.item.mask = TVIF_TEXT | TVIF_STATE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
   tv.item.iImage = 0;
   tv.item.iSelectedImage = tv.item.iImage;
      
   
   CString str;
   str.Format( "Экспертная система" );
   tv.item.pszText = (LPSTR)(LPCSTR)str;
   HTREEITEM hRootES = m_treeRules.InsertItem( &tv ); 
   m_treeRules.SetItemData( hRootES, NULL );

   if( !dwSel ) hKey = hRootES;

   TExpertSystem *pE = (TExpertSystem*)m_pBindSubj;
   TUserLocal* pUs = ((TMainFrame*)AfxGetApp()->m_pMainWnd)->GetCurUser();

   list<TPro*>::iterator it( pE->m_lstPro.begin() );
   list<TPro*>::iterator itEnd( pE->m_lstPro.end() );

   for( ; it != itEnd; ++it )
	{
      CString str;

	  if( pUs->m_bShowSource == true )
	    str.Format( "%s (%s)", (LPCSTR)(*it)->GetName(), 
	      (LPCSTR)(*it)->GetPath() );
	  else
	   str.Format( "%s", (LPCSTR)(*it)->GetName() );

	  tv.hInsertAfter = TVI_LAST;
      tv.item.mask = TVIF_TEXT|TVIF_IMAGE | TVIF_SELECTEDIMAGE;   
      tv.hParent = hRootES;   
	  tv.item.iImage = (*it)->GetSel() == true ? 4:1;
	  tv.item.iSelectedImage = tv.item.iImage;

      tv.item.pszText = (LPSTR)(LPCSTR)str;
      
	  HTREEITEM hit2;
	  m_treeRules.SetItemData( (hit2=m_treeRules.InsertItem(&tv)), (DWORD)*it );

	  if( (DWORD)*it == dwSel ) hKey = hit2;

	  if( (*it)->m_arcQ() )
	   {
         if( pUs->m_bShowSource == true )
	       str.Format( "%s (%s)", (LPCSTR)(*it)->m_arcQ()->GetName(), 
	         (LPCSTR)(*it)->m_arcQ()->GetPath() );
	     else
	       str.Format( "%s", (LPCSTR)(*it)->m_arcQ()->GetName() );

	     tv.hInsertAfter = TVI_LAST;
         tv.item.mask = TVIF_TEXT|TVIF_IMAGE | TVIF_SELECTEDIMAGE;   
         tv.hParent = hit2;   
		 tv.item.iImage = (*it)->m_arcQ()->GetSel() == true ? 6:3;
		 tv.item.iSelectedImage = tv.item.iImage;

         tv.item.pszText = (LPSTR)(LPCSTR)str;
      
		 HTREEITEM hit3;
	     m_treeRules.SetItemData( (hit3=m_treeRules.InsertItem(&tv)), (DWORD)((*it)->m_arcQ()) );

		 if( (DWORD)((*it)->m_arcQ()) == dwSel ) hKey = hit3;
	   }

	  if( (*it)->m_arcF() )
	   {
         if( pUs->m_bShowSource == true )
	       str.Format( "%s (%s)", (LPCSTR)(*it)->m_arcF()->GetName(), 
	         (LPCSTR)(*it)->m_arcF()->GetPath() );
	     else
	       str.Format( "%s", (LPCSTR)(*it)->m_arcF()->GetName() );

	     tv.hInsertAfter = TVI_LAST;
         tv.item.mask = TVIF_TEXT|TVIF_IMAGE | TVIF_SELECTEDIMAGE;   
         tv.hParent = hit2;   
		 tv.item.iImage = (*it)->m_arcF()->GetSel() == true ? 5:2;
		 tv.item.iSelectedImage = tv.item.iImage;

         tv.item.pszText = (LPSTR)(LPCSTR)str;
      
		 HTREEITEM hit4;
	     m_treeRules.SetItemData( (hit4=m_treeRules.InsertItem(&tv)), (DWORD)((*it)->m_arcF()) );
		 if( (DWORD)((*it)->m_arcF()) == dwSel ) hKey = hit4;
	   }
	}   

   if( dwSel != -1 )
	{
      if( hKey )
	    m_treeRules.Expand( hKey, TVE_EXPAND ), m_treeRules.Select( hKey, TVGN_CARET );
	  else
	    m_treeRules.Expand( m_treeRules.GetRootItem(), TVE_EXPAND ), 
		m_treeRules.Select( m_treeRules.GetRootItem(), TVGN_CARET );
	}

 }

void TExpertView::FillNets()
 {
   TWndDrwLock lockR( &m_chkNets );

   m_chkNets.ResetContent();

   TExpertSystem *pE = (TExpertSystem*)m_pBindSubj;

   if( !pE || !pE->m_lstNet.size() )
	{      
	  m_chkNets.SetItemData( m_chkNets.AddString("<Пусто>"), -1 );
	  m_chkNets.SetHorizontalExtent( 0 );
	  return;
	}

   list<TNet*>::iterator it( pE->m_lstNet.begin() );
   list<TNet*>::iterator itEnd( pE->m_lstNet.end() );

   TUserLocal* pUs = ((TMainFrame*)AfxGetApp()->m_pMainWnd)->GetCurUser();
	//)->GetCurUser()->m_bUseSpecialDesign;

   CClientDC dc( &m_chkNets );
   CFont *pFntOld = dc.SelectObject( m_chkNets.GetFont() );
   int iMaxW = 0;

   for( ; it != itEnd; ++it )
	{
	  CString str;

	  if( pUs->m_bShowSource == true )
	    str.Format( "%s (%s)", (LPCSTR)(*it)->GetName(), 
	      (LPCSTR)(*it)->GetPath() );
	  else
	   str.Format( "%s", (LPCSTR)(*it)->GetName() );
	      
      m_chkNets.SetItemData( m_chkNets.AddString(str), (DWORD)*it );

	  CSize sz = dc.GetTextExtent( str );
	  iMaxW = max( iMaxW, sz.cx );
	}

   m_chkNets.SetHorizontalExtent( 1.2*iMaxW  );
   dc.SelectObject( pFntOld );
 }

void TExpertView::FillFrn()
 {
   TWndDrwLock lockR( &m_chkFrames );

   m_chkFrames.ResetContent();

   TExpertSystem *pE = (TExpertSystem*)m_pBindSubj;

   if( !pE || !pE->m_lstFrm.size() )
	{      
	  m_chkFrames.SetItemData( m_chkFrames.AddString("<Пусто>"), -1 );
	  m_chkFrames.SetHorizontalExtent( 0 );
	  return;
	}

   list<TFrm*>::iterator it( pE->m_lstFrm.begin() );
   list<TFrm*>::iterator itEnd( pE->m_lstFrm.end() );

   TUserLocal* pUs = ((TMainFrame*)AfxGetApp()->m_pMainWnd)->GetCurUser();
	//)->GetCurUser()->m_bUseSpecialDesign;

   CClientDC dc( &m_chkFrames );
   CFont *pFntOld = dc.SelectObject( m_chkFrames.GetFont() );
   int iMaxW = 0;

   for( ; it != itEnd; ++it )
	{
	  CString str;	  

	  if( pUs->m_bShowSource == true )
	    str.Format( "%s (%s)", (LPCSTR)(*it)->GetName(), 
	      (LPCSTR)(*it)->GetPath() );
	  else
	   str.Format( "%s", (LPCSTR)(*it)->GetName() );

      m_chkFrames.SetItemData( m_chkFrames.AddString(str), (DWORD)*it );

	  CSize sz = dc.GetTextExtent( str );
	  iMaxW = max( iMaxW, sz.cx );
	}

   m_chkFrames.SetHorizontalExtent( 1.2*iMaxW  );
   dc.SelectObject( pFntOld );
 }

int TExpertView::GetFName( LPCSTR lpTtl, LPCSTR lpDefExt, LPCSTR lpTypesStr, CString& rName )
 {
   rName += CString( "*." ) + lpDefExt;
   TLSFileDlg dlgFile( TRUE, lpDefExt, (LPCSTR)rName,
	  OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, lpTypesStr, AfxGetApp()->m_pMainWnd, false, true );   	

   dlgFile.m_ofn.lpstrTitle = lpTtl;  

   if( dlgFile.DoModal() == IDCANCEL ) return IDCANCEL;
   
   rName = dlgFile.GetFileName();

   return IDOK;
 }

int TExpertView::GetFName2( CString& csTtl, LPCSTR lpDefExt, LPCSTR lpTypesStr, CString& rName )
 {
   rName += CString( "*." ) + lpDefExt;
   TLSFileDlg dlgFile( TRUE, lpDefExt, (LPCSTR)rName,
	  OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, lpTypesStr, AfxGetApp()->m_pMainWnd, false, false );   	

   dlgFile.m_ofn.lpstrTitle = (LPCSTR)csTtl;  

   if( dlgFile.DoModal() == IDCANCEL ) return IDCANCEL;
   
   rName = dlgFile.GetFileName();
   csTtl = dlgFile.GetPathName();
   csTtl = csTtl.Left( csTtl.GetLength() - rName.GetLength() );

   return IDOK;
 }


bool TExpertView::CheckExist( LPCSTR strName )
 {
   if( ::CheckExist(strName) == true )
	{
	  CString str;
	  str.Format( "Файл \"%s\" уже существует", (LPCSTR)strName );
      MessageBox( str, "Нельзя создать", MB_OK|MB_ICONSTOP );
	  return true;
	}
	   
   return false;
 }

void TExpertView::OnNewObj()
 {
   CString strName( ((TExpertSystem*)m_pBindSubj)->m_csPath );
   CString strComment;

   TSubject* pSubj_ = NULL;

   try {

     if( m_hWndFocusCTL == m_treeRules.m_hWnd )
	  {
        HTREEITEM hCur = m_treeRules.GetSelectedItem();	  
	    int iItem = 0;//0-rules, 1-faq, 2-quest
	  

	    TSubject *pSubj;
	    if( hCur && (pSubj = (TSubject*)m_treeRules.GetItemData(hCur)) ) 
		 {
           if( pSubj->IsA() != TPC_TPro )
			{
              MessageBox( "Чтобы создать объект нужно выделить узел (корень или базу правил)", "Предупреждение", MB_OK|MB_ICONEXCLAMATION );
		      return;
			}		 

		   TPro *p = (TPro *)pSubj;
		   if( p->m_arcQ() && p->m_arcF() )
			{
              MessageBox( "Эта база правил уже имеет базы фактов и вопросов", "Предупреждение", MB_OK|MB_ICONEXCLAMATION );
		      return; 
			}

		   TDlgType dlg( this, "Создать", false, !bool(p->m_arcF()), !bool(p->m_arcQ()) );
		   dlg.m_iRes = 0;
		   if( (iItem = dlg.DoModal()) == IDCANCEL ) return;
		   iItem = dlg.m_iRes;
		   strComment = dlg.m_str;
		 }    	  	  
	    else 
		 {
           TDlgType dlg( this, "Создать", true, false, false );
		   dlg.m_iRes = 0;
		   if( dlg.DoModal() == IDCANCEL ) return;
		   iItem = dlg.m_iRes;
		   strComment = dlg.m_str;
		 }

	    CString str; 
	    str.Format( "Создать базу %s", iItem==0 ? "правил":(iItem==1 ? "фактов":"вопросов") );	  

        if( GetFName(str, "mdb", "Базы (*.mdb)|*.mdb|Все файлы (*.*)|*.*||", strName) == IDCANCEL ) return;	  
	    if( CheckExist(strName) ) return;

	    pSubj_ = CreateMgr( iItem == 0 ? TC_Rules:(iItem == 1 ? TC_Facts:TC_Quest), 
	       strName, ((TExpertSystem*)m_pBindSubj)->m_csPath, strComment );	   	  
	    ((TExpertSystem*)m_pBindSubj)->AddVSubj( pSubj_, pSubj );
		FillRul( hCur ? m_treeRules.GetItemData(hCur):0 );

		ExpandTree( pSubj_ );
		
		
		//m_treeRules.SetFocus();
		//m_hWndFocus = m_hWndFocusCTL = m_treeRules.m_hWnd;
	  }
     else if( m_hWndFocusCTL == m_chkNets.m_hWnd ) 
	  {
        //m_chkNets
	    TDlgType dlg( this, "Создать семантическую сеть", false, false, false );
	    dlg.m_iRes = 0;
	    if( dlg.DoModal() == IDCANCEL ) return;	  
	    strComment = dlg.m_str;	  
	    if( GetFName("Создать семантическую сеть", "mdb", "Базы (*.mdb)|*.mdb|Все файлы (*.*)|*.*||", strName) == IDCANCEL ) return;
	    if( CheckExist(strName) ) return;

		int iSel = m_chkNets.GetCurSel();

	    pSubj_ = CreateMgr( TC_Net, 
	       strName, ((TExpertSystem*)m_pBindSubj)->m_csPath, strComment );	   	  
	    ((TExpertSystem*)m_pBindSubj)->AddVSubj( pSubj_ );
	    FillNets();

		//m_chkNets.SetFocus();
		m_chkNets.SetCurSel( iSel != LB_ERR ? iSel:0 );
		//m_hWndFocus = m_hWndFocusCTL = m_chkNets.m_hWnd;
	  }
     else if( m_hWndFocusCTL == m_chkFrames.m_hWnd ) 
	  {
        //m_chkFrames
	    TDlgType dlg( this, "Создать фреймовую базу знаний", false, false, false );
	    dlg.m_iRes = 0;
	    if( dlg.DoModal() == IDCANCEL ) return;	  
	    strComment = dlg.m_str;	  
	    if( GetFName("Создать фреймовую базу знаний", "mdb", "Базы (*.mdb)|*.mdb|Все файлы (*.*)|*.*||", strName) == IDCANCEL ) return;	  
	    if( CheckExist(strName) ) return;

		int iSel = m_chkFrames.GetCurSel();

	    pSubj_ = CreateMgr( TC_Frames, 
	       strName, ((TExpertSystem*)m_pBindSubj)->m_csPath, strComment );	   	  
	    ((TExpertSystem*)m_pBindSubj)->AddVSubj( pSubj_ );
	    FillFrn();

		//m_chkFrames.SetFocus();
		m_chkFrames.SetCurSel( iSel != LB_ERR ? iSel:0 );
		//m_hWndFocus = m_hWndFocusCTL = m_chkFrames.m_hWnd;
	  }	
	}//try
   catch( CException *pE )
	{
	  pE->ReportError();
	  pE->Delete();
	  return;
	}   
   catch( TMyException *pE )
	{
      MessageBox( pE->GetMsg(), "Ошибка", 
	     MB_OK|MB_ICONSTOP );
	  pE->Delete();
	  return;
	}

   ((TExpertSystem*)m_pBindSubj)->SetModify( true );
   GetDocument()->SetModifiedFlag();

   Quant();

   MakeEditSubj( pSubj_ );
 }



void TExpertView::OnImport()
 {
   CString strName( ((TExpertSystem*)m_pBindSubj)->m_csPath );
   CString strComment;

   TSubject* pSubj_ = NULL;

   try {

     if( m_hWndFocusCTL == m_treeRules.m_hWnd )
	  {
        HTREEITEM hCur = m_treeRules.GetSelectedItem();	  
	    int iItem = 0;//0-rules, 1-faq, 2-quest
	  

	    TSubject *pSubj;
	    if( hCur && (pSubj = (TSubject*)m_treeRules.GetItemData(hCur)) ) 
		 {
           if( pSubj->IsA() != TPC_TPro )
			{
              MessageBox( "Чтобы добавить  объект нужно выделить узел (корень или базу правил)", "Предупреждение", MB_OK|MB_ICONEXCLAMATION );
		      return;
			}		 

		   TPro *p = (TPro *)pSubj;
		   if( p->m_arcQ() && p->m_arcF() )
			{
              MessageBox( "Эта база правил уже имеет базы фактов и вопросов", "Предупреждение", MB_OK|MB_ICONEXCLAMATION );
		      return; 
			}

		   TDlgType dlg( this, "Добавить извне", false, !bool(p->m_arcF()), !bool(p->m_arcQ()) );
		   dlg.m_iRes = 0;
		   if( (iItem = dlg.DoModal()) == IDCANCEL ) return;
		   iItem = dlg.m_iRes;
		   strComment = dlg.m_str;
		 }    	  	  
	    else 
		 {
           TDlgType dlg( this, "Добавить извне", true, false, false );
		   dlg.m_iRes = 0;
		   if( dlg.DoModal() == IDCANCEL ) return;
		   iItem = dlg.m_iRes;
		   strComment = dlg.m_str;
		 }

	    CString str; 
	    str.Format( "Добавить базу %s", iItem==0 ? "правил":(iItem==1 ? "фактов":"вопросов") );	  

        if( GetFName2(str, "mdb", "Базы (*.mdb)|*.mdb|Все файлы (*.*)|*.*||", strName) == IDCANCEL ) return;	  

	    pSubj_ = ImportMgr( iItem == 0 ? TC_Rules:(iItem == 1 ? TC_Facts:TC_Quest), 
	       strName, ((TExpertSystem*)m_pBindSubj)->m_csPath, strComment, (LPSTR)(LPCSTR)str );	   	  
	    ((TExpertSystem*)m_pBindSubj)->AddVSubj( pSubj_, pSubj );
		FillRul( hCur ? m_treeRules.GetItemData(hCur):0 );
		
		ExpandTree( pSubj_ );
		
		//m_treeRules.SetFocus();
		//m_hWndFocus = m_hWndFocusCTL = m_treeRules.m_hWnd;
	  }
     else if( m_hWndFocusCTL == m_chkNets.m_hWnd ) 
	  {
        //m_chkNets
        CString str; str.Format( "Добавить внешнюю семантическую сеть" );

	    TDlgType dlg( this, str, false, false, false );
	    dlg.m_iRes = 0;
	    if( dlg.DoModal() == IDCANCEL ) return;	  
	    strComment = dlg.m_str;	  		
	    if( GetFName2(str, "mdb", "Базы (*.mdb)|*.mdb|Все файлы (*.*)|*.*||", strName) == IDCANCEL ) return;
	    

		int iSel = m_chkNets.GetCurSel();

	    pSubj_ = ImportMgr( TC_Net, 
	       strName, ((TExpertSystem*)m_pBindSubj)->m_csPath, strComment, (LPSTR)(LPCSTR)str );	   	  
	    ((TExpertSystem*)m_pBindSubj)->AddVSubj( pSubj_ );
	    FillNets();

		//m_chkNets.SetFocus();
		m_chkNets.SetCurSel( iSel != LB_ERR ? iSel:0 );
		//m_hWndFocus = m_hWndFocusCTL = m_chkNets.m_hWnd;
	  }
     else if( m_hWndFocusCTL == m_chkFrames.m_hWnd ) 
	  {
        //m_chkFrames
	    CString str; str.Format( "Добавить внешнюю фреймовую базу знаний" );
	    TDlgType dlg( this, str, false, false, false );
	    dlg.m_iRes = 0;
	    if( dlg.DoModal() == IDCANCEL ) return;	  
	    strComment = dlg.m_str;	  
	    if( GetFName2(str, "mdb", "Базы (*.mdb)|*.mdb|Все файлы (*.*)|*.*||", strName) == IDCANCEL ) return;
	    

		int iSel = m_chkFrames.GetCurSel();

	    pSubj_ = ImportMgr( TC_Frames, 
	       strName, ((TExpertSystem*)m_pBindSubj)->m_csPath, strComment, (LPSTR)(LPCSTR)str );	   	  
	    ((TExpertSystem*)m_pBindSubj)->AddVSubj( pSubj_ );
	    FillFrn();

		//m_chkFrames.SetFocus();
		m_chkFrames.SetCurSel( iSel != LB_ERR ? iSel:0 );
		//m_hWndFocus = m_hWndFocusCTL = m_chkFrames.m_hWnd;
	  }	
	}//try
   catch( CException *pE )
	{
	  pE->ReportError();
	  pE->Delete();
	  return;
	}       
   catch( TMyException *pE )
	{
      MessageBox( pE->GetMsg(), "Ошибка", 
	     MB_OK|MB_ICONSTOP );
	  pE->Delete();
	  return;
	}

   ((TExpertSystem*)m_pBindSubj)->SetModify( true );
   GetDocument()->SetModifiedFlag();
 }

int TExpertView::AskLevel( TSubject *pSubj )
 {
   CString str; 
   str.Format( "Удалить объект [%s \"%s\"] вместе с источниками ?", 
	 pSubj->MyName(), pSubj->GetName() );

   return MessageBox( (LPCSTR)str, "Вопрос", MB_ICONQUESTION|MB_YESNOCANCEL );
 }

void TExpertView::OnRemove()
 {
   if( ValidateAccess(GetSelectedObj()) == false ) return;   

   try {

     if( m_hWndFocusCTL == m_treeRules.m_hWnd )
	  {
        HTREEITEM hCur = m_treeRules.GetSelectedItem();	  	  	  

	    TSubject *pSubj;
	    if( hCur && (pSubj = (TSubject*)m_treeRules.GetItemData(hCur)) ) 
		 {
	       bool bRes = 
		     ((TExpertSystem*)m_pBindSubj)->RemoveObj( pSubj, AskLevel(pSubj) );
           HTREEITEM hNxt = m_treeRules.GetNextVisibleItem( hCur );	  	  	    
           if( bRes ) 
			 m_treeRules.DeleteItem( hCur );
			 //m_treeRules.Select( hNxt, TVGN_CARET );		   
		 }
	    else	   
	      MessageBox( "Нужно выделить в любом списке удаляемый объект", 
		               "Предупреждение", MB_OK|MB_ICONEXCLAMATION );	
	  }
     else if( m_hWndFocusCTL == m_chkNets.m_hWnd ) 
	  {
	    int iSel = m_chkNets.GetCurSel();
	    TSubject *pSubj = 0;
	    if( iSel == LB_ERR || !(pSubj = (TSubject*)m_chkNets.GetItemData(iSel)) ||
		    pSubj == (void*)-1 )
	      MessageBox( "Нужно выделить в любом списке удаляемый объект", 
		               "Предупреждение", MB_OK|MB_ICONEXCLAMATION );	
	    else
		 {
           bool bRes = 		   
		     ((TExpertSystem*)m_pBindSubj)->RemoveObj( pSubj, AskLevel(pSubj) );
           if( bRes ) 
			 m_chkNets.DeleteString( iSel ),
             m_chkNets.SetCurSel( iSel );		   
		 }
	  }
     else if( m_hWndFocusCTL == m_chkFrames.m_hWnd ) 
	  {
	    int iSel = m_chkFrames.GetCurSel();
	    TSubject *pSubj = 0;
	    if( iSel == LB_ERR || !(pSubj = (TSubject*)m_chkFrames.GetItemData(iSel)) ||
		    pSubj == (void*)-1 )
	      MessageBox( "Нужно выделить в любом списке удаляемый объект", 
		               "Предупреждение", MB_OK|MB_ICONEXCLAMATION );	
	    else
		 {
           bool bRes = 
		     ((TExpertSystem*)m_pBindSubj)->RemoveObj( pSubj, AskLevel(pSubj) );		   
           if( bRes ) 
			 m_chkFrames.DeleteString( iSel ),
			 m_chkFrames.SetCurSel( iSel );
		 }
	  }
   }
  catch( CException *pE )
   {
     pE->ReportError();
	 pE->Delete();
	 return;
   }
  catch( TMyException *pE )
	{
      MessageBox( pE->GetMsg(), "Ошибка", 
	     MB_OK|MB_ICONSTOP );
	  pE->Delete();
	  return;
	}

  ((TExpertSystem*)m_pBindSubj)->SetModify( true );
   GetDocument()->SetModifiedFlag();
 }

void TExpertView::OnEdit()
 {
   if( ValidateAccess(GetSelectedObj()) == false ) return;   
   TSubject* pSubj = GetSelectedObj();
   if( !pSubj ) return;

   MakeEditSubj( pSubj );
 }

void TExpertView::MakeEditSubj( TSubject* pSubj )
 {
   ((TExpertDoc*)GetDocument())->m_pSubjKey = pSubj;

   CWnd *pW = GetParent();
   if( !pW ) return;

   int iSwt = 0;

   switch( pSubj->IsA() )
	{
	  case TPC_TNet:
	    iSwt = TSWT_Net;
		break;
	  case TPC_TFrm:
	    iSwt = TSWT_Frm;
		break;
	  case TPC_TFac:
	    iSwt = TSWT_Facts;
		break;
	  case TPC_TQuery:
	    iSwt = TSWT_Q;
		break;
	  case TPC_TPro:
	    iSwt = TSWT_Rules;
		break;
	};

   pW->PostMessage( globl_iMsgSwitchViews, iSwt, (LPARAM)pSubj );
 }


void TExpertView::ClearSelectTree()
 {
   //TWndDrwLock lockR( &m_treeRules );

   list<TPro*>::iterator itSta2( ((TExpertSystem*)m_pBindSubj)->m_lstPro.begin() ); 
   list<TPro*>::iterator itEnd2( ((TExpertSystem*)m_pBindSubj)->m_lstPro.end() ); 

   HTREEITEM hCur = m_treeRules.GetSelectedItem();	  

   for( ;itSta2 != itEnd2; itSta2++ )	
	{
      (*itSta2)->SetSel( false );
	  if( (*itSta2)->m_arcQ() ) (*itSta2)->m_arcQ()->SetSel( false );
	  if( (*itSta2)->m_arcF() ) (*itSta2)->m_arcF()->SetSel( false );
	}

   FillRul( m_treeRules.GetItemData(hCur) );   
 }

void TExpertView::ClearSelectLst( TMyChLBox *pBox )
 {
   TWndDrwLock lockR( pBox );
   
   int ic = pBox->GetCount();
   for( int i = 0; i < ic; ++i )
     pBox->SetCheck( i, 0  );
 }

void TExpertView::OnClear()
 {
   if( m_hWndFocusCTL == m_treeRules.m_hWnd )
	 ClearSelectTree();

   else if( m_hWndFocusCTL == m_chkNets.m_hWnd ) 
	 ClearSelectLst( &m_chkNets );

   else if( m_hWndFocusCTL == m_chkFrames.m_hWnd ) 
	 ClearSelectLst( &m_chkFrames );	
 }

void TExpertView::OnClearAll()
 {   
   ClearSelectTree();
   ClearSelectLst( &m_chkNets );
   ClearSelectLst( &m_chkFrames );	
 }

TSubject* TExpertView::GetSelectedObj()
 {
   if( m_hWndFocusCTL == m_treeRules.m_hWnd )
	{
      HTREEITEM hCur = m_treeRules.GetSelectedItem();	  
	  if( !hCur ) return NULL;

	  TSubject *p = (TSubject*)m_treeRules.GetItemData( hCur );
	  return long(p) == - 1 ? NULL:p;
	}
   else if( m_hWndFocusCTL == m_chkNets.m_hWnd ) 
	{
      int iSel = m_chkNets.GetCurSel();
	  if( iSel == LB_ERR ) return NULL;

	  TSubject *p = (TSubject*)m_chkNets.GetItemData( iSel );
	  return long(p) == - 1 ? NULL:p;
	}
   else if( m_hWndFocusCTL == m_chkFrames.m_hWnd ) 
	{
      int iSel = m_chkFrames.GetCurSel();
	  if( iSel == LB_ERR ) return NULL;

	  TSubject *p = (TSubject*)m_chkFrames.GetItemData( iSel );
	  return long(p) == - 1 ? NULL:p;
	}     

   return NULL;
 }

void TExpertView::OnAcc()
 {
   TSubject* pObj = GetSelectedObj();
   if( !pObj || pObj == (void*)-1 )
	{
      MessageBox( "Нужно выделить объект экспертной системы", "Предупреждение", MB_OK|MB_ICONEXCLAMATION );
	  return; 
	}
   	
   LPCSTR lpForm = "";
   switch( pObj->IsA() )
	{  
	  case TPC_TNet:
	    lpForm = "Семантической сети \"%s\"";
		break;
	  case TPC_TFrm:
	    lpForm = "Фреймовой базе знаний \"%s\"";
		break;
	  case TPC_TFac:
	    lpForm = "Базе фактов \"%s\"";
		break;
	  case TPC_TQuery:
	    lpForm = "Базе вопросов \"%s\"";
		break;
	  case TPC_TPro:
	    lpForm = "Продукционной базе правил \"%s\"";
		break;
	};

   CString str; 
   str.Format( lpForm, pObj->GetName() ); 
   TAccDlg dlg( AfxGetApp()->m_pMainWnd, pObj, str );

   if( dlg.DoModal() == IDOK ) GetDocument()->SetModifiedFlag();
 } 

void TExpertView::OnCompile()
 {
 }

static void DeleteGarbFile( LPCSTR lpPath )
 {
   ::DeleteFile( lpPath );
 }

void TExpertView::OnMakeRules()
 {
   TProtectedObj *pProtObj = ((TMainFrame*)AfxGetApp()->m_pMainWnd)->GetProtectLvl();

   char drive[ _MAX_DRIVE ];
   char dir[ _MAX_DIR ];
   char fname[ _MAX_FNAME ];
   char ext[ _MAX_EXT ];
   char cBuf[ _MAX_PATH ];
   

   TAutoMemoryRc<TSubject*> arcSubj;
   CString strPathPro;

   int iCnt = m_chkNets.GetCount();
   bool bFlFound = false;
   for( int i = 0; i < iCnt; i++ )
	if( m_chkNets.GetCheck(i) )
	 {              
	   try {
		  TNet *pNet = (TNet*)m_chkNets.GetItemData( i );
		  if( long(pNet) == -1 || !pNet ) continue;
		  bFlFound = true;

		  if( ValidateAccess(pNet) == false ) continue;   
 
		  _splitpath( pNet->GetPath(), drive, dir, fname, ext );
		  _makepath( cBuf, drive, dir, fname, "" );	  	 

		  CString strFNam;
		  strFNam.Format( "%s_Prod.mdb", cBuf );
		  CString strNam;
		  strNam.Format( "Из сети %s", (LPCSTR)pNet->GetName() );	    
		  
		  arcSubj =
			CreateMgr( TC_Rules, strFNam,
			  ((TExpertSystem*)m_pBindSubj)->m_csPath, strNam );

		  strPathPro = ((TExpertSystem*)m_pBindSubj)->m_csPath + strFNam;
		  CString strPathNet = ((TExpertSystem*)m_pBindSubj)->m_csPath + pNet->GetPath();
		  
		  CompileNet( pNet, (TPro*)(arcSubj()), strPathPro, strPathNet );
		   
		  ((TExpertSystem*)m_pBindSubj)->AddVSubj( arcSubj(), NULL );
		  TSubject *pS = arcSubj(); arcSubj.ClearRc();
		  FillRul();
		  ExpandTree( pS );

		  m_chkNets.SetCheck( i, 0 );
		}
	  catch( CException *pE )
	   {
		 pE->ReportError();
		 pE->Delete();

		 if( arcSubj() )  DeleteGarbFile( strPathPro );
		 arcSubj.FreeRc();

		 continue;
	   }   
	  catch( TMyException *pE )
	   {
		 MessageBox( pE->GetMsg(), "Ошибка", 
			MB_OK|MB_ICONSTOP );
		 pE->Delete();

		 if( arcSubj() )  DeleteGarbFile( strPathPro );
		 arcSubj.FreeRc();

		 continue;
	   }
	  ((TExpertSystem*)m_pBindSubj)->SetModify( true );
	  GetDocument()->SetModifiedFlag();
	 }

	if( bFlFound == false )
	  MessageBox( "Для компиляции в списах сетевых и фреймовых баз нужно отметить хотябы одну базу", "Предупреждение", MB_OK|MB_ICONEXCLAMATION );
 }

void TExpertView::OnConsult()
 {
   if( ValidateAccess(GetSelectedObj()) == false ) return;   
   TSubject* pSubj = GetSelectedObj();   

   if( !pSubj || pSubj->IsA() != TPC_TPro )
	{
      MessageBox( "Для консультации в левом списке требуестся выбрать базу знаний", "Ошибка", MB_OK | MB_ICONEXCLAMATION );
	  return;
	}

   ((TExpertDoc*)GetDocument())->m_pSubjKey = pSubj;

   CWnd *pW = GetParent();
   if( !pW ) return;

   try {
	  ((TExpertDoc*)GetDocument())->arcWA = 
	     new TWorkArea( (TPro*)pSubj, ((TExpertDoc*)GetDocument())->GetESystem() );
	  ((TExpertDoc*)GetDocument())->arcWA()->Create();
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
   
   pW->PostMessage( globl_iMsgSwitchViews, TSWT_Run, (LPARAM)pSubj );
 }

bool TExpertView::ValidateAccess( TSubject* pSubj )
 {
   if( !pSubj || pSubj == (void*)-1 ) return true;

   if( pSubj->IsValidated() == true ) return true;   
   
   TMainFrame *pFram = (TMainFrame*)((TExpertApp*)AfxGetApp())->m_pMainWnd;  

   if( pSubj->KonfirmToObj(*pFram->GetProtectLvl()) == false )
	{
	  CString str;
	  str.Format( "Нарушение прав доступа к [%s \"%s\"]", 
	   pSubj->MyName(), pSubj->GetName() );
      MessageBox( (LPSTR)(LPCSTR)str, "Ошибка", MB_OK | MB_ICONSTOP );
	  
	  return false;
	}			

   pSubj->SetValidate( true );
   return true;
 }
