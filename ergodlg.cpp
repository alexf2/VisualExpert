#include "ergodlg.hpp"
#include "expdlg.hpp"
#include "mfrexp.hpp"

using namespace std;
#include <algorithm>

void TColorCombo::DrawItem( LPDRAWITEMSTRUCT lpD )
 {
   CDC dc; dc.Attach( lpD->hDC );

   CRect r( lpD->rcItem );
   //CRect rC(r); //GetClientRect( rC );
   //r.right = rC.right;
   r.DeflateRect( 4, 2 );

   CBrush br( (COLORREF)GetItemData(lpD->itemID) );   
   dc.FillRect( r, &br );   
   CBrush blackBr( RGB(0,0,0) );
   dc.FrameRect( r, &blackBr );

   if( lpD->itemState & (ODS_FOCUS|ODS_SELECTED) )
	{
	  CBrush rbFr( RGB(0,0,255) );
	  r.InflateRect( 2, 2 );
      dc.FrameRect( r, &rbFr );
	}
   else
	{ 
      CBrush rbFr( RGB(0xFF,0xFF,0xFF) );
	  r.InflateRect( 2, 2 );
      dc.FrameRect( r, &rbFr );
	}

   dc.Detach();
 }

void TColorCombo::MeasureItem( LPMEASUREITEMSTRUCT lpM )
 {
   CWindowDC dc( GetDesktopWindow() );
   CFont *pFOld = (CFont*)dc.SelectStockObject( SYSTEM_FONT );

   CSize sz = dc.GetTextExtent( "ABC", 3 );

   dc.SelectObject( pFOld );

   lpM->itemWidth  = sz.cx;
   lpM->itemHeight = sz.cy;
 }


static void DelIt( TErgoItem *p )
 {
   delete p;
 }

TErgoCategory& TErgoCategory::operator=( TErgoCategory& rC )
 {
   list<TErgoItem*>::iterator itSta( m_lst.begin() );
   list<TErgoItem*>::iterator itEnd( m_lst.end() );
   list<TErgoItem*>::iterator itSrc( rC.m_lstDup.begin() );
   for( ; itSta != itEnd; ++itSta, ++itSrc )
	{
	  (*itSta)->m_clrFore = (*itSrc)->m_clrFore;
	  (*itSta)->m_clrBk = (*itSrc)->m_clrBk;

	  (*itSta)->m_fnt = (*itSrc)->m_fnt;	         
	  if( !(*itSta)->m_pInchertFnt ) (*itSta)->Create();
	}

   return *this;
 }

TErgoCategory::~TErgoCategory()
 {
   for_each( m_lstDup.begin(), m_lstDup.end(), DelIt );
   m_lstDup.clear();
 }

static TErgoItem* FindInch( list<TErgoItem*>& rLst, TErgoItem* pIt )
 {
   list<TErgoItem*>::iterator itSta( rLst.begin() );
   list<TErgoItem*>::iterator itEnd( rLst.end() );
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->m_pPar == pIt ) return *itSta;

   return NULL;
 }

TErgoCategory::TErgoCategory( TErgoCategory &rC )
 {
   list<TErgoItem*>::iterator itSta( rC.m_lst.begin() );
   list<TErgoItem*>::iterator itEnd( rC.m_lst.end() );
   for( ; itSta != itEnd; ++itSta )
	{
      TErgoItem *p = new TErgoItem( *itSta );
	  m_lstDup.push_back( p );
	  p->m_pPar = *itSta;
	  p->m_pInchertFnt = NULL;
	}
   m_strName = rC.m_strName;

   itSta = m_lstDup.begin();
   itEnd = m_lstDup.end();
   list<TErgoItem*>::iterator itSrc( rC.m_lst.begin() );
   for( ; itSta != itEnd; ++itSta, ++itSrc )
	 if( (*itSrc)->m_pInchertFnt )
	   (*itSta)->m_pInchertFnt = FindInch( m_lstDup, (*itSrc)->m_pInchertFnt );
	 else (*itSta)->Create();
 }

BOOL TErgView::OnEraseBkgnd( CDC* )
 {
   return TRUE;
 }
void TErgView::OnPaint()
 {
   CPaintDC dc( this );

   CRect r; GetClientRect( r );
   if( !m_pIt )	
     dc.FillSolidRect( r, RGB(0xFF,0xFF,0xFF) );
   else
	{
      CFont *pFOld = dc.SelectObject( m_pUsl->GetItemFnt( m_pIt ) );

	  dc.FillSolidRect( r, m_pIt->m_clrBk );
	  dc.SetTextColor( m_pIt->m_clrFore );
	  dc.DrawText( "Тест", 4, r, DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_NOPREFIX );

	  dc.SelectObject( pFOld );
	}	
 }

BEGIN_MESSAGE_MAP( TErgView, CStatic )	
  ON_WM_ERASEBKGND()
  ON_WM_PAINT()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP( TErgoDlg, CDialog )	
  ON_LBN_SELCHANGE( IDC_LIST_CAT, OnSelCat )
  ON_LBN_SELCHANGE( IDC_LIST_ITEM, OnSelIt )
  ON_CBN_SELENDOK( IDC_COMBO_BK, OnComboBk )
  ON_CBN_SELENDOK( IDC_COMBO_F, OnComboF )
  ON_BN_CLICKED( IDC_BTN_FNT, OnFont )
END_MESSAGE_MAP()

void TErgoDlg::OnSelCat()
 {
   int iC = m_lstCat.GetCurSel();
   if( iC != LB_ERR ) 
	 FillIt( iC ), m_lstIt.SetCurSel( 0 ), OnSelIt();
 }

void TErgoDlg::OnFont()
 {
   int iC = m_lstIt.GetCurSel();   
   if( iC == LB_ERR ) return;

   TErgoItem *pIt = (TErgoItem*)m_lstIt.GetItemData( iC );
   CFontDialog dlgF( &pIt->m_fnt, CF_EFFECTS|CF_SCREENFONTS, NULL, this );
   if( dlgF.DoModal() == IDOK )
	{
      dlgF.GetCurrentFont( &pIt->m_fnt );
	  pIt->Create();
	  m_eView.InvalidateRect( NULL );

	  m_bChangedFont = true;
	}
 }

void TErgoDlg::OnComboBk()
 {
   int iC = m_cbnBk.GetCurSel();
   if( iC == CB_ERR ) return;

   int iC2 = m_lstIt.GetCurSel();
   if( iC2 == LB_ERR ) return;

   ((TErgoItem*)(m_lstIt.GetItemData( iC2 )))->m_clrBk =
	 (COLORREF)m_cbnBk.GetItemData( iC );

   m_eView.InvalidateRect( NULL );
 }
void TErgoDlg::OnComboF()
 {
   int iC = m_cbnFore.GetCurSel();
   if( iC == CB_ERR ) return;

   int iC2 = m_lstIt.GetCurSel();
   if( iC2 == LB_ERR ) return;

   ((TErgoItem*)(m_lstIt.GetItemData( iC2 )))->m_clrFore =
	 (COLORREF)m_cbnFore.GetItemData( iC );

   m_eView.InvalidateRect( NULL );
 }

static void SelectCombo( TColorCombo& rC, COLORREF& rColor )
 {
   int i = rC.GetCount() - 1;
   for( ; i > -1; --i )
	{
	  COLORREF clr = (COLORREF)rC.GetItemData( i );
	  if( !memcmp(&rColor, &clr, sizeof(COLORREF)) )
	   {
         rC.SetCurSel( i );
		 return;
	   }
	}
 }

void TErgoDlg::OnSelIt()
 {
   int iC = m_lstIt.GetCurSel();
   BOOL bFl;
   if( iC == LB_ERR ) bFl = FALSE; 
   else
	{
      bFl = !((TErgoItem*)(m_lstIt.GetItemData( iC )))->m_pInchertFnt;
	  SelectCombo( m_cbnBk, ((TErgoItem*)m_lstIt.GetItemData(iC))->m_clrBk );
	  SelectCombo( m_cbnFore, ((TErgoItem*)m_lstIt.GetItemData(iC))->m_clrFore );
	}

   m_eView.m_pIt = (TErgoItem*)m_lstIt.GetItemData( iC );
   m_eView.InvalidateRect( NULL );

   m_btnFont.EnableWindow( bFl );
 }

void TErgoDlg::FillCat()
 {
   TWndDrwLock lockR( &m_lstCat );

   list<TErgoCategory*>::iterator itSta( m_lstDup.begin() );
   list<TErgoCategory*>::iterator itEnd( m_lstDup.end() );   
   for( ; itSta != itEnd; ++itSta  )
	 m_lstCat.SetItemData( m_lstCat.AddString((LPCSTR)(*itSta)->m_strName), (DWORD)*itSta );
 }
void TErgoDlg::FillIt( int iCat )
 {
   TWndDrwLock lockR( &m_lstIt );

   
   list<TErgoCategory*>::iterator itSta1( m_lstDup.begin() );
   list<TErgoCategory*>::iterator itEnd1( m_lstDup.end() );  
   int i = 0;
   for( ; itSta1 != itEnd1 && i != iCat; ++itSta1, ++i );

   TErgoCategory *pCat = *itSta1;

   m_lstIt.ResetContent();

   list<TErgoItem*>::iterator itSta( pCat->m_lstDup.begin() );
   list<TErgoItem*>::iterator itEnd( pCat->m_lstDup.end() );  
   for( ; itSta != itEnd; ++itSta  )
	 m_lstIt.SetItemData( m_lstIt.AddString((LPCSTR)(*itSta)->m_strItemName), (DWORD)*itSta );
 }

void TErgoDlg::OnOK()
 {
   list<TErgoCategory*>::iterator itSta( m_lstDup.begin() );
   list<TErgoCategory*>::iterator itEnd( m_lstDup.end() );
   list<TErgoCategory*>::iterator itTarg( m_lst.begin() );
   for( ; itSta != itEnd; ++itSta, ++itTarg )
	 **itTarg = **itSta;

   ((TMainFrame*)(AfxGetApp()->m_pMainWnd))->GetCurUser()->ModifyData();

   //GetParent()->InvalidateRect( NULL );

   CDialog::OnOK();
 }

void TErgoDlg::Dup()
 {
   list<TErgoCategory*>::iterator itSta( m_lst.begin() );
   list<TErgoCategory*>::iterator itEnd( m_lst.end() );
   for( ; itSta != itEnd; ++itSta )
	{
      TErgoCategory *pCat = new TErgoCategory( **itSta );
	  m_lstDup.push_back( pCat );
	}
 }

static void DeleteIt( TErgoCategory *pC )
 {
   delete pC;
 }

TErgoDlg::~TErgoDlg()
 {
   for_each( m_lst.begin(), m_lst.end(), DeleteIt );
   m_lst.clear();

   for_each( m_lstDup.begin(), m_lstDup.end(), DeleteIt );
   m_lstDup.clear();
 }

void TErgoDlg::DoDataExchange( CDataExchange* pDX )
 {
   CDialog::DoDataExchange( pDX );         

   DDX_Control( pDX, IDC_LIST_CAT, m_lstCat );
   DDX_Control( pDX, IDC_LIST_ITEM, m_lstIt );

   DDX_Control( pDX, IDC_COMBO_BK, m_cbnBk );
   DDX_Control( pDX, IDC_COMBO_F, m_cbnFore );   

   DDX_Control( pDX, IDC_BTN_FNT, m_btnFont );      

   DDX_Control( pDX, IDC_STATIC_SAMPL, m_eView );

   /*DDX_Text( pDX, IDC_EDIT_NAME, m_varDsk );
   DDX_Text( pDX, IDC_EDIT_VAL, m_varVal );*/   
 }


BOOL TErgoDlg::OnInitDialog()
 {
   static COLORREF rA[ 16 ] =
	   {
		 RGB(0,0,0),
		 RGB(255,255,255),
		 RGB(0,255,255),
		 RGB(255,0,255),
		 RGB(0,0,255),
		 RGB(255,255,0),
		 RGB(0,255,0),
		 RGB(255,0,0),
		 RGB(128,128,128),
		 RGB(0xC0,0xC0,0xC0),		 
		 RGB(128,0,128),
		 RGB(0,0,128),
		 RGB(128,128,0),
		 RGB(0x80,0x80,0x80),
		 RGB(0,128,0),
		 RGB(128,0,0)
	   };

   CDialog::OnInitDialog();            

   CenterWindow();

   Dup();
   FillCat();
   FillIt( 0 );

   m_eView.m_pUsl = ((TMainFrame*)(AfxGetApp()->m_pMainWnd))->GetCurUser();

   for( int i = 0; i < sizeof(rA)/sizeof(COLORREF); ++i )
     m_cbnBk.SetItemData( m_cbnBk.AddString(""), (DWORD)rA[i] ),
     m_cbnFore.SetItemData( m_cbnFore.AddString(""), (DWORD)rA[i] );

   m_lstCat.SetCurSel( 0 );
   m_lstIt.SetCurSel( 0 );

   OnSelCat();

   HWND hw1 = ::GetDlgItem( m_hWnd, IDC_LIST_CAT );
   PostMessage( WM_NEXTDLGCTL, (WPARAM)hw1, 1 );	        

   return FALSE;
 }
