#include "v_rules.hpp"
#include "subj.hpp"
#include "dlgacess.hpp"
#include "docexp.hpp"
#include "mdiexp.hpp"
#include "globlmsg.hpp"

#include "viewexp.hpp"
#include <afxcmn.h>

#include "RDO\\rdocolumns.h"
#include "RDO\\_rdocolumn.h"
#include "RDO\\_rdoresultset.h"
#include "RDO\\_rdoenvironment.h"
#include "rdo\_rdoconnection.h"

#include "..\\RUDAKOW\\SERVIS\\myexc.hpp"

#include "vs.rh"

#include "product.hpp"

#include <afxpriv.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define ID_STATGRP 10

IMPLEMENT_DYNCREATE( TRulesView, TBaseView )

BEGIN_MESSAGE_MAP( TRulesView, TBaseView )	
  ON_WM_CREATE()  
  ON_WM_DESTROY()
  ON_WM_SIZE()
  ON_WM_SHOWWINDOW()

  ON_WM_ERASEBKGND()

  ON_MESSAGE( WM_HELPHITTEST, OnCommandHelp )
  ON_MESSAGE( WM_COMMANDHELP, OnCommandHelpF1 )

  ON_UPDATE_COMMAND_UI( ICM_RUL_ADD, OnUpdate_Always )
  ON_UPDATE_COMMAND_UI( ICM_RUL_DELETE, OnUpdate_Always )
  ON_UPDATE_COMMAND_UI( ICM_RUL_LABEL, OnUpdate_Always )
  ON_UPDATE_COMMAND_UI( ICM_RUL_SEARCH, OnUpdate_Always )
  ON_UPDATE_COMMAND_UI( ICM_RUL_OPT, OnUpdate_Always )
  ON_UPDATE_COMMAND_UI( ICM_RUL_EXIT, OnUpdate_Always )  
  ON_UPDATE_COMMAND_UI( ICM_RUL_ERS, OnUpdate_Always )  


  ON_COMMAND( ICM_RUL_ERS,    OnRulErs )
  ON_COMMAND( ICM_RUL_ADD,    OnRulLabel )
  ON_COMMAND( ICM_RUL_DELETE, OnRulDelete )
  ON_COMMAND( ICM_RUL_LABEL,  OnRulAdd )
  ON_COMMAND( ICM_RUL_SEARCH, OnRulSearch )
  ON_COMMAND( ICM_RUL_OPT,    OnRulOpt )
  ON_COMMAND( ICM_RUL_EXIT,   OnRulExit )

END_MESSAGE_MAP()


BEGIN_MESSAGE_MAP( TRDCOwn, CStatic )	

  ON_WM_CTLCOLOR()

END_MESSAGE_MAP()


LRESULT TRulesView::OnCommandHelp( WPARAM wp, LPARAM lParam )
{
  return 12348;
}

LRESULT TRulesView::OnCommandHelpF1( WPARAM wp, LPARAM lParam )
{
  AfxGetApp()->CWinApp::WinHelp( 3988 );    
  return TRUE;
}



TRulesView::TRulesView(): TBaseView()
 {      
   m_bErrRDC = false;
   m_bRetGrid = false;
   m_bEdit = false;
   m_lCountID = 0;   
   m_bDirty = false;
   m_bTransPending = false;

   m_iSh = 0;
   m_iHighS = 0;

   m_bRedr = false;

   m_bCancelErr = false;
   m_iFirstShow = 0;
 }

TRulesView::~TRulesView()
 {   
 }

void TRulesView::OnShowWindow( BOOL bShow, UINT nStatus )
 {   
   TBaseView::OnShowWindow( bShow, nStatus );   
 }

TO_ObjTypes TRulesView::IsObj() const
 {
   return TO_Rules;
 }

LPCSTR TRulesView::GetShortFileName()
 {
   if( !m_pBindSubj ) return "Пусто";
   //return ((TSubject*)m_pBindSubj)->GetPath();
   return ((TSubject*)m_pBindSubj)->MyName();
 }

LPCSTR TRulesView::GetName()
 {
   if( !m_pBindSubj ) return "Пусто";

   m_strTmpName.Format( "%s%s", 
	  (LPCSTR)((TSubject*)m_pBindSubj)->GetName(),
	  m_bDirty == true ? "*":"" );

   return m_strTmpName;
 }


BOOL TRulesView::OnEraseBkgnd( CDC *pDC )
 {   
   //return Default();

   CRect r; GetClientRect( r ); 
   pDC->FillSolidRect( r, RGB(0xC0,0xC0,0xC0) );
 
   return TRUE;   
 }

void TRulesView::OnUpdate( CView* pSender, LPARAM lHint, CObject* pHint )
 {
   TBaseView::OnUpdate( pSender, lHint, pHint );
 }

void TRulesView::OnDraw( CDC* pDC )
 {
 }

bool TRulesView::MyPreclose()
 {  
   if( m_bDirty == true && m_bTransPending == true )
	{
	  CString str;
	  str.Format( "Данные в [%s \"%s\"] изменены. Сохранить их ?", 
	     (LPCSTR)Subj()->MyName(), (LPCSTR)Subj()->GetName() );
      int iRes = MessageBox( (LPCSTR)str, "Предупреждение", MB_YESNOCANCEL|MB_ICONQUESTION );
	  if( iRes == IDCANCEL ) return false;
	  if( iRes == IDYES ) m_arcRDC()->CommitTrans();   
	  else m_arcRDC()->RollbackTrans();   	  
	}
   else if( m_bTransPending == true ) m_arcRDC()->CommitTrans();  
   m_bTransPending == false;

   try {	 
	 m_bDirty = false;
	 m_arcGrid()->BindProperty( 0x9, NULL );         
	 m_arcGrid()->SetDataSource( NULL );
	 m_arcRDC()->SetDataSourceName( "" );
     m_DSN.Remove();
	}
   catch( TMyException *pE )
	{
      MessageBox( pE->GetMsg(), "Ошибка", MB_OK|MB_ICONSTOP );
	  pE->Delete();
	  
	  //return;
	}
   catch( CException *pE )
	{
	  pE->ReportError();
	  pE->Delete();

	  //return;
	}
            
   return true;
 }

void TRulesView::OnDestroy()
 {         
   TBaseView::OnDestroy();
 }

void TRulesView::OnSize( UINT nType, int cx, int cy )
 {   
   if( m_iFirstShow < 3  ) m_iFirstShow++;   
   else
	{  
      m_iFirstShow = 10;
	  ModifyStyle( 0, WS_CLIPCHILDREN );
	}

   TBaseView::OnSize( nType, cx, cy );
   return; 

   /*
   DWORD dwRedr = SWP_NOREDRAW;
   if( m_bRedr == true )
	 m_bRedr = false, dwRedr = 0;

   if( nType == SIZE_MINIMIZED )
	{
      TBaseView::OnSize( nType, cx, cy );
	  return;
	}

   int cx_ = (m_iMinX <= cx ? cx:m_iMinX),
	   cy_ = (m_iMinY <= cy ? cy:m_iMinY);

   if( m_arcGrid() )
	{
	  if( !m_iSh )
	    m_arcGrid()->SetWindowPos( NULL, 0, 0, cx_, cy_, 
	      SWP_NOZORDER|SWP_NOACTIVATE|dwRedr );
	  else
	    m_arcGrid()->SetWindowPos( NULL, 0, 0, cx_, cy_ - m_iHighS-1, 
	      SWP_NOZORDER|SWP_NOACTIVATE|dwRedr );
	}

   if( m_iSh )
	 m_statGrp.SetWindowPos( NULL, 0, cy - m_iHighS, cx_, m_iHighS-1, 
	   SWP_NOZORDER|SWP_NOACTIVATE|dwRedr );
   

   TBaseView::OnSize( nType, cx, cy );*/
 }

BOOL TRulesView::PreCreateWindow( CREATESTRUCT& cs )
 {
   BOOL bRes = TBaseView::PreCreateWindow( cs );
   if( !bRes ) return bRes;

   cs.style &= ~WS_CLIPCHILDREN;
   return TRUE;
 }

int TRulesView::OnCreate( LPCREATESTRUCT lpCreateStruct )
 {
   if( TBaseView::OnCreate(lpCreateStruct) == -1 )
     return -1;

   m_bShadow = false;

   m_pHorDlgb = ((TMDIFrame*)GetParent())->GetHorDlgBarPtr();

   m_arcRDC = new CRdc();
   m_arcGrid = new CMsDgridCtrl();
   m_arcRDCOwner = new TRDCOwn( this, m_arcGrid() );

   CRect r( 0, 0, 200, 100 );   
	
   if( !m_arcGrid()->Create(NULL, "", WS_CHILD, r, this, 1) )
	{
	  MessageBox( "Невозможно создать DBGrid OCX", "Ошибка", MB_OK|MB_ICONSTOP );
      return -1;
	}
   m_arcGrid()->ModifyStyleEx( 0, WS_EX_CLIENTEDGE );


   CClientDC dc( m_pHorDlgb );
   CSize sz = dc.GetTextExtent( "W", 1 );
   r = CRect( 0, 0, sz.cx * 9, sz.cy * 1.3 );
   m_arcRDCOwner()->Create( "", WS_CHILD, r, m_pHorDlgb );
   if( !m_arcRDC()->Create(NULL, "", WS_VISIBLE|WS_CHILD, r, m_arcRDCOwner(), 2) )
	{
	  MessageBox( "Невозможно создать RDC OCX", "Ошибка", MB_OK|MB_ICONSTOP );
      return -1;
	}

   sz = dc.GetTextExtent( CString("A") );   
   SetMinWH( (m_iMinX = sz.cx * 30), (m_iMinY = sz.cy * 15) );   

   m_iHighS = 3 * sz.cy;		  
   
   m_statGrp.Create( "", WS_BORDER|WS_GROUP|WS_CHILD|WS_CLIPSIBLINGS, r, this, ID_STATGRP );
   m_statGrp.ModifyStyleEx( 0, WS_EX_CLIENTEDGE );                 

   pcitGrid = new TChildItem( m_arcGrid(), 0, 0, 1, 1 );
   pcitGrp  = new TChildItem( &m_statGrp, 0, 0.8, 1, 0.2 );
   *this + pcitGrid + pcitGrp;
   
   

   MakeLaoyt();
   
   return 0; 
 }

void TRulesView::OnInitialUpdate()
 {
   TBaseView::OnInitialUpdate();   

   CClientDC dc( this );
   //CRect rC; GetClientRect( rC );
   //dc.FillSolidRect( rC, RGB(0xC,0xC,0xC) );

   CString strPath;
   ((TExpertDoc*)GetDocument())->GetESystem()->GetFullObjPath( strPath, Subj() );
   try {
	 Subj()->CheckDBFormat( strPath );
     m_DSN.Create( strPath, Subj()->GetName() );
	}
   catch( TMyException *pE )
	{
      MessageBox( pE->GetMsg(), "Ошибка", MB_OK|MB_ICONSTOP );
	  pE->Delete();

	  SwitchBackView();
	  return;
	}
   catch( CException *pE )
	{
	  pE->ReportError();
	  pE->Delete();

	  SwitchBackView();
	  return;
	}   

   m_bErrRDC = false;
   
   m_arcRDC()->SetDataSourceName(  (LPCSTR)Subj()->GetName() );
   CString strSQL;
   strSQL.Format( "SELECT * FROM \"%s\" ORDER BY \"Номер\"", Subj()->MyName() );
   m_arcRDC()->SetSql( (LPCSTR)strSQL );
   CString str;
   Subj()->GetUserID( str );
   m_arcRDC()->SetUserName( (LPCSTR)str );
   m_arcRDC()->SetPassword( Subj()->Password() );   
   m_arcRDC()->SetCursorDriver( 3 );
   m_arcRDC()->SetForeColor( RGB(0,0,255) );   
   //m_arcRDC()->SetUpdateCriteria( 0 );
   //m_arcRDC()->SetUpdateOperation( 1 );

      
   LPUNKNOWN lpCurs = m_arcRDC()->GetDSCCursor();

   C_rdoResultset res = m_arcRDC()->GetResultset();  
   C_rdoColumn clm = res.GetRdoColumns().GetItem( COleVariant(0L) );
   clm.SetKeyColumn( TRUE );

   InitID();

   m_arcRDC()->BeginTrans();
   m_bTransPending = true;

   //BOOL bb = m_arcRDC()->GetConnection().GetTransactions();   

   //m_arcRDC()->Refresh();   

   
   //m_arcRDC()->SetEOFAction( 2 );
   //m_arcRDC()->SetBOFAction( 1 );

   try {
	 if( lpCurs )
	  { 
		C_rdoResultset res = m_arcRDC()->GetResultset();
		CrdoColumns clm = res.GetRdoColumns();

		COleFont fnt = m_arcGrid()->GetHeadFont();
		fnt.SetBold( TRUE );	  
		CY cy = fnt.GetSize(); cy.Lo *= 1.2;
		fnt.SetSize( cy );
		m_arcGrid()->SetHeadFont( fnt );
		  
		long ncol = clm.GetCount();				
		int iMax = 0;
		for( long icol = 0; icol < ncol; icol++ )
		 {
		   CString str = clm.GetItem( COleVariant(icol) ).GetName();
		   CSize sz = dc.GetTextExtent( str );
		   iMax = max( iMax, sz.cx );
		 }
		  
		CSize sz = dc.GetTextExtent( "(ТО)", 4 );
		m_arcGrid()->SetDefColWidth( iMax + sz.cx );	  

		m_arcGrid()->SetDataSource( lpCurs );
		m_arcGrid()->BindProperty( 0x9, m_arcRDC() ); 

		m_arcGrid()->SetAllowAddNew( TRUE );
		m_arcGrid()->SetAllowDelete( TRUE );
		m_arcGrid()->SetAllowRowSizing( TRUE );
		m_arcGrid()->SetAllowUpdate( TRUE );
		m_arcGrid()->SetColumnHeaders( TRUE );	  	  	 
	  }

	 if( m_bErrRDC == true )
	  {
		SwitchBackView();
		return;
	  }  
	}
   catch( CException *pE )
	{
	  pE->ReportError();
	  pE->Delete();

	  SwitchBackView();
	  return;
	}   
   
   m_arcGrid()->ShowWindow( SW_SHOW );
   m_arcRDCOwner()->ShowWindow( SW_SHOW );

   //ModifyStyle( 0, WS_CLIPCHILDREN );
 }


TV_Views TRulesView::IsA() const
 {
   return TV_RulesEdit;
 }

bool TRulesView::CanClose()
 {
   bool bRes = MyPreclose();
   if( bRes == true ) 
	 GetParent()->PostMessage( globl_iMsgSwitchViews, TSWT_Explorer, 
	   (LPARAM)((TExpertDoc*)GetDocument())->GetESystem() );

   return bRes;
 }
  

static TBtnTool btnHoriz[] =
 { 
  { TB_Bmp, ICM_RUL_ERS, NULL, IDB_BMP_RUL_ERS, false, "Удалить текущую запись", BS_PUSHBUTTON },
  { TB_Bmp, ICM_RUL_ADD, NULL, IDB_BMP_RUL_ADD, true, "Сохранить изменения", BS_PUSHBUTTON },
  { TB_Bmp, ICM_RUL_DELETE, NULL, IDB_BMP_EXP_REMOV, true, "Отменить модификацию базы", BS_PUSHBUTTON },
  { TB_Bmp, ICM_RUL_LABEL, NULL, IDB_BMP_RUL_LABEL, true, "Перенумерация записей базы", BS_PUSHBUTTON },

  { TB_Bmp, ICM_RUL_SEARCH, NULL, IDB_BMP_RUL_SEARCH, false, "Найти запись в базе", BS_AUTOCHECKBOX|BS_PUSHLIKE },  

  { TB_Bmp, ICM_RUL_OPT, NULL, IDB_BMP_EXP_OPT, false, "Настройка редактора", BS_PUSHBUTTON },
  { TB_Bmp, ICM_RUL_EXIT, NULL, IDB_BMP_RUL_EXIT, true, "Возврат в эксплорер экспертных систем", BS_PUSHBUTTON },
  { TB_EndDescr, 0, NULL, 0, true, "", 0 }
 };


void TRulesView::SetupHorizBar( CMyDialogBar& rB )
 {     
   switch( ((TSubject*)m_pBindSubj)->IsA() )
	{
	  case TPC_TFac:
	    rB.SetCurVievClass( TV_FactEdit );
		break;

	  case TPC_TQuery:
	    rB.SetCurVievClass( TV_QueryEdit );
		break;

	  case TPC_TPro:
	    rB.SetCurVievClass( TV_RulesEdit );
		break;
	}

   ClearBtnsLists(); //only here ! - first calls horizontal setup
   DWORD dwS = rB.GetBarStyle();
   rB.SetBarStyle( (dwS & ~CBRS_BORDER_BOTTOM)|CBRS_BORDER_LEFT|CBRS_BORDER_RIGHT|CBRS_BORDER_TOP );
   
   rB + *new TBtnItem( m_arcRDCOwner(), true );
   rB + new TToolTipData( 2, "Перемещение по базе данных" );

   CRect r; m_arcRDCOwner()->GetWindowRect( r );

   CreateBtns( rB, btnHoriz, false, m_lstBmp, m_lstBtn );

   rB.SetMaxBounds( max(rB.GetMaxBounds(), r.Height()) );
 }

void TRulesView::SetupVerBar( CMyDialogBar& rB )
 {
   switch( ((TSubject*)m_pBindSubj)->IsA() )
	{
	  case TPC_TFac:
	    rB.SetCurVievClass( TV_FactEdit );
		break;

	  case TPC_TQuery:
	    rB.SetCurVievClass( TV_QueryEdit );
		break;

	  case TPC_TPro:
	    rB.SetCurVievClass( TV_RulesEdit );
		break;
	}

   if( rB.IsWindowVisible() ) rB.ShowWindow( SW_HIDE );
 }


void TRulesView::OnRulAdd()
 {      
   C_rdoResultset res = m_arcRDC()->GetResultset();
   COleVariant varBook = res.GetBookmark();
   long lRow = m_arcGrid()->GetRow();
   res.MoveFirst();
     
   C_rdoColumn clm = res.GetRdoColumns().GetItem( COleVariant(0L) );
   for( int i = 1; !res.GetEof(); i++ )    
     res.Edit(), clm.SetValue( COleVariant(long(i)) ), res.Update(),
	 res.MoveNext();
    
   res.SetBookmark( varBook );   
   m_lCountID = i;   
   m_arcGrid()->SetRow( lRow > -1 ? lRow:0 );

   m_bDirty = true;
 }
void TRulesView::OnRulDelete()
 { 
   if( m_bDirty == false ) return;

   m_bTransPending = false;
   m_arcRDC()->RollbackTrans();      

   m_arcRDC()->Refresh();
   C_rdoResultset res = m_arcRDC()->GetResultset();  
   C_rdoColumn clm = res.GetRdoColumns().GetItem( COleVariant(0L) );
   clm.SetKeyColumn( TRUE );

   m_arcRDC()->BeginTrans();   
   m_bTransPending = true;

   m_bDirty = false;
 }

void TRulesView::OnRulErs()
 {
   C_rdoResultset rs = m_arcRDC()->GetResultset();  
   rs.Delete();         
   m_bDirty = true;
 }

void TRulesView::OnRulLabel()
 {
   if( m_bDirty == false ) return;
 
   m_bTransPending = false;
   m_arcRDC()->CommitTrans();   
   m_arcRDC()->BeginTrans();   
   m_bTransPending = true;

   m_bDirty = false;
 }

void TRulesView::OnRulSearch()
 {   
   if( (m_iSh = ((CButton*)(((TMDIFrame*)GetParent())->GetHorDlgBarPtr()->GetDlgItem(ICM_RUL_SEARCH)))->GetCheck()) )
	{
	  pcitGrid->m_H = 0.8;
	  pcitGrp->m_top = 0.8, pcitGrp->m_H = 0.2;	   
	}
   else
	{
	  pcitGrid->m_H = 1;
	  m_statGrp.ShowWindow( SW_HIDE );
	}
   
   MakeLaoyt();
   if( m_iSh ) m_statGrp.ShowWindow( SW_SHOW );
   
   /*CRect r; GetClientRect( r );
   m_bRedr = true;
   OnSize( 0, r.Width(), r.Height() );
   m_statGrp.InvalidateRect( NULL );   */
 }

void TRulesView::OnRulOpt()
 {
 }
void TRulesView::OnRulExit()
 {
   SwitchBackView();
 }

void TRulesView::SwitchBackView()
 {  
   CWnd *pW = GetParent();
   if( !pW ) return;

   if( MyPreclose() == false ) return;

   pW->PostMessage( globl_iMsgSwitchViews, TSWT_Explorer, 
	 (LPARAM)((TExpertDoc*)GetDocument())->GetESystem() );
 }


BEGIN_EVENTSINK_MAP(TRulesView, TBaseView)
	
	ON_EVENT(TRulesView, 1, 15 /* SelChange */, OnSelChangeDbgrid1, VTS_PI2)
	ON_EVENT(TRulesView, 1, 16 /* Change */, OnChangeDbgrid1, VTS_NONE)

	ON_EVENT(TRulesView, 1, 14 /* Scroll */, OnScrollDbgrid1, VTS_PI2)
	ON_EVENT(TRulesView, 1, 11 /* RowColChange */, OnRowColChangeDbgrid1, VTS_PVARIANT VTS_I2)
    
	ON_EVENT(TRulesView, 1, 3 /* AfterInsert */, OnAfterInsertDbgrid1, VTS_NONE)
	ON_EVENT(TRulesView, 1, 37 /* OnAddNew */, OnOnAddNewDbgrid1, VTS_NONE)
	ON_EVENT(TRulesView, 1, 25 /* AfterColEdit */, OnAfterColEditDbgrid1, VTS_I2)
	
	ON_EVENT(TRulesView, 1, 8 /* BeforeUpdate */, OnBeforeUpdateDbgrid1, VTS_PI2)
	ON_EVENT(TRulesView, 1, 5 /* BeforeColUpdate */, OnBeforeColUpdateDbgrid1, VTS_I2 VTS_PVARIANT VTS_PI2)
	ON_EVENT(TRulesView, 1, 26 /* BeforeColEdit */, OnBeforeColEditDbgrid1, VTS_I2 VTS_I2 VTS_PI2)		
	
	ON_EVENT(TRulesView, 1, 2 /* AfterDelete */, OnAfterDeleteDbgrid1, VTS_NONE)
	ON_EVENT(TRulesView, 1, 4 /* AfterUpdate */, OnAfterUpdateDbgrid1, VTS_NONE)

	ON_EVENT(TRulesView, 1, 6 /* BeforeDelete */, OnBeforeDeleteDbgrid1, VTS_PI2)	

END_EVENTSINK_MAP()

BEGIN_EVENTSINK_MAP( TRDCOwn, CStatic )

	ON_EVENT(TRDCOwn, 2, 0 /* Validate */, OnValidateRemotedatactl1, VTS_PI2 VTS_PI2)
	ON_EVENT(TRDCOwn, 2, 2 /* Error */, OnErrorRemotedatactl1, VTS_I4 VTS_PBSTR VTS_I4 VTS_BSTR VTS_BSTR VTS_I4 VTS_PBOOL)
	ON_EVENT(TRDCOwn, 2, 3 /* QueryCompleted */, OnQueryCompletedRemotedatactl1, VTS_NONE)	

END_EVENTSINK_MAP()


void TRDCOwn::OnValidateRemotedatactl1(short FAR* Action, short FAR* Reserved) 
{
  if( *Action == 12 ) 
     m_p->m_bRetGrid = true;    
  
}

void TRDCOwn::OnErrorRemotedatactl1(long Number, BSTR FAR* Description, long Scode, LPCTSTR Source, LPCTSTR HelpFile, long HelpContext, BOOL FAR* CancelDisplay) 
{
	// TODO: Add your control notification handler code here
  if( m_p->m_bCancelErr == true )
    *CancelDisplay = TRUE, m_p->m_bCancelErr = false;
  else *CancelDisplay = FALSE;

  m_p->m_bErrRDC = true;
}

void TRDCOwn::OnQueryCompletedRemotedatactl1() 
{
	// TODO: Add your control notification handler code here
	
}

void TRulesView::OnSelChangeDbgrid1( short FAR* Cancel ) 
 {
	// TODO: Add your control notification handler code here
  //m_arcRDC()->SetCaption( m_arcGrid()->GetText() );  
 }

void TRulesView::OnChangeDbgrid1() 
 {	
   m_bDirty = true;	
 }


void TRulesView::OnScrollDbgrid1(short FAR* Cancel) 
{
	// TODO: Add your control notification handler code here
	
}

void TRulesView::OnRowColChangeDbgrid1( VARIANT FAR* LastRow, short LastCol) 
 {
   /*COleVariant va = m_arcGrid()->GetBookmark();   
   CString str; 
   
   str.Format( "%d", int(*va.bstrVal) - 1 );
   m_arcRDC()->SetCaption( (LPCSTR)str );*/   
     
   if( m_bRetGrid == true )
	{
      m_bRetGrid = false;
	  long l = m_arcGrid()->GetVisibleRows();
	  m_arcGrid()->SetRow( l > 1 ? l - 2:0 );	  
	}
   else if( m_arcGrid()->GetRow() == -1 )
	{
	  long rv = m_arcGrid()->GetVisibleRows();
	  if( rv > 2 ) m_arcGrid()->SetRow( rv - 2 );
	  else m_arcGrid()->SetRow( 0 );
	}

   CString str;
   if( m_arcGrid()->GetAddNewMode() == 2 )
	 str.Format( "Новый" );
   else if( m_arcGrid()->GetRow() == m_arcGrid()->GetVisibleRows() - 1 )
	 str.Format( "***" );
   else
	{	   
      COleVariant var = 
	    m_arcRDC()->GetResultset().GetRdoColumns().GetItem(COleVariant(0L)).GetValue();
      str.Format( "%d", int(var.lVal) );
      
	}

   m_arcRDC()->SetCaption( (LPCSTR)str );
 }

HBRUSH TRDCOwn::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
 {   
   HBRUSH hbr = CStatic::OnCtlColor( pDC, pWnd, nCtlColor );
   
   pDC->SetTextColor( RGB(0,0,255) );

   if( nCtlColor == CTLCOLOR_STATIC )
	{
	  HWND hw = ::GetDlgItem( m_hWnd, ID_STATGRP );
	  if( hw == pWnd->m_hWnd )
        hbr = (HBRUSH)GetStockObject( LTGRAY_BRUSH );
	}
   
   return hbr;
 }


void TRulesView::OnAfterInsertDbgrid1() 
 {	
   m_bDirty = true;
 }

void TRulesView::OnOnAddNewDbgrid1() 
{  
  C_rdoResultset res = m_arcRDC()->GetResultset();    
  res.GetRdoColumns().GetItem( COleVariant(0L) ).SetValue( COleVariant(m_lCountID++) );  

  m_bDirty = true;
   
}

void TRulesView::OnAfterColEditDbgrid1(short ColIndex) 
{
	// TODO: Add your control notification handler code here
	long l = m_arcGrid()->GetAddNewMode();//!!!!!!
		
}


void TRulesView::OnBeforeUpdateDbgrid1( short FAR* Cancel ) 
 {
	// TODO: Add your control notification handler code here
  //*Cancel = 1;

  *Cancel = 0;

  try {
    CheckRule();	
   }
  catch( TMyException *pE )
   {
     MessageBox( pE->GetMsg(), "Ошибка", MB_OK|MB_ICONSTOP );
	 pE->Delete();
	 //*Cancel = 1;
	 m_bCancelErr = true;
   }
  catch( CException *pE )
   {
     pE->ReportError();
	 pE->Delete();
	 //*Cancel = 1;
	 m_bCancelErr = true;
   }
 }
 
void TRulesView::OnBeforeColUpdateDbgrid1(short ColIndex, VARIANT FAR* OldValue, short FAR* Cancel) 
{
	// TODO: Add your control notification handler code here
  //*Cancel = 1;		   
   //CrdoColumns clms = m_arcRDC()->GetResultset().GetRdoColumns();
   //int iC = clms.GetCount();   

   if( ColIndex == 1 )		  
     strC1 = (LPCTSTR)m_arcGrid()->GetText();	
   else if( ColIndex == 2 )
	 strC2 = (LPCTSTR)m_arcGrid()->GetText();		
   else if( ColIndex == 3 )
	 strC3 = (LPCTSTR)m_arcGrid()->GetText();		
   else if( ColIndex == 4 )
	 strC4 = (LPCTSTR)m_arcGrid()->GetText();			      
}
 
void TRulesView::OnBeforeColEditDbgrid1( short ColIndex, short KeyAscii, short FAR* Cancel ) 
{
	// TODO: Add your control notification handler code here
   if( !ColIndex ) 
	{ 
	  *Cancel = 1;
      //MessageBox( "Нельзя редактировать счётчик", "Предупреждение", MB_OK|MB_ICONASTERISK );
	}   

   m_bDirty = true;   
}


void TRulesView::InitID()
 {
   C_rdoResultset res = m_arcRDC()->GetResultset();
   res.MoveFirst();
  
   m_lCountID = 0; 
   C_rdoColumn clm = res.GetRdoColumns().GetItem( COleVariant(0L) );
   for( int i = 1; !res.GetEof(); i++ )
    {
      COleVariant var = clm.GetValue();
	  m_lCountID = max( m_lCountID, var.lVal );
	  res.MoveNext();
    }

   res.MoveFirst();   
   m_lCountID++;
 }

void TRulesView::OnAfterDeleteDbgrid1() 
{
  m_arcGrid()->Refresh();     
  m_arcRDC()->UpdateControls();
  m_arcGrid()->SetRow( m_iKeyPos );
}


BOOL TRulesView::PreTranslateMessage( MSG* pMsg )
 {
   if( (pMsg->message == WM_KEYUP||pMsg->message == WM_KEYDOWN) &&
	    pMsg->wParam == VK_ESCAPE )
	 return TRUE;

   return TBaseView::PreTranslateMessage( pMsg );
 }

void TRulesView::OnAfterUpdateDbgrid1() 
{
	// TODO: Add your control notification handler code here
//////////////


}

void TRulesView::OnBeforeDeleteDbgrid1( short FAR* Cancel ) 
{
	// TODO: Add your control notification handler code here
  m_iKeyPos = m_arcGrid()->GetRow();	
  m_bDirty = true;

  /*C_rdoResultset res = m_arcRDC()->GetResultset();
  if( res.GetRowCount() == 1 )
   {
     *Cancel = TRUE;
   }*/
/*  C_rdoResultset res = m_arcRDC()->GetResultset();
  int k;
  if( m_arcGrid()->GetVisibleRows() < 3 )
   {
     int i = res.GetRdoColumns().GetCount();
	 res.Edit();

	 for( ; i > 1; i-- )
       res.GetRdoColumns().GetItem( COleVariant(long(i)) ).SetValue( COleVariant(" ", VT_BSTR) );

     res.Update();

	 *Cancel = TRUE;
   }*/
 }

void TRulesView::CheckRule()
 {
   switch( ((TSubject*)m_pBindSubj)->IsA() )
	{
	  case TPC_TFac:
	    CheckFac();
		break;

	  case TPC_TQuery:
	    CheckQ();
		break;

	  case TPC_TPro:
	    CheckPro();
		break;
	}
 }

void TRulesView::CheckFac()
 {
   TFactWork facts;
   TNameSpace nmSpc;

   facts.Create( strC1, strC2, strC3, &nmSpc  );	
 }

void TRulesView::CheckQ()
 {
   TQWork query;
   TNameSpace nmSpc;
   
   query.Create( strC1, strC2, &nmSpc );
 }

void TRulesView::CheckPro()
 {
   TNameSpace nmSpc;
   TSimpleProduct prod( &nmSpc, 1 );
   
   prod.Create( strC1, strC2, strC3, &nmSpc );	
 }
