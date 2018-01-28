// TCalcDialog.cpp : implementation file
//

#include "TCalcDialog.h"
#include "vs.hpp"
#include "var.hpp"
#include "mfrexp.hpp"
#include "ask.hpp"
#include "OwnerDrawBox.h"

#include "..\\RUDAKOW\\SERVIS\\subcode.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TCalcDialog dialog


TCalcDialog::TCalcDialog(CWnd* pParent /*=NULL*/)
	: CDialog(TCalcDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(TCalcDialog)
	//}}AFX_DATA_INIT
  m_hIcCalc = NULL;  

  m_iPrec = 6, m_iShowMode = 0;
  m_kX = 65535, m_kY = 65535;
}


void TCalcDialog::OnDestroy()
 {
   UpdateData( TRUE );

   if( m_hIcCalc )
	{
      DestroyIcon( m_hIcCalc );
	  m_hIcCalc = NULL;
	}

   CDialog::OnDestroy();
 }

void TCalcDialog::DoDataExchange( CDataExchange* pDX )
 {
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TCalcDialog)
	DDX_Control(pDX, IDC_LIST, m_resultsbox);
	DDX_Control(pDX, IDC_EDIT, m_edit);
	//}}AFX_DATA_MAP

	DDX_Control( pDX, IDC_CALC_CLEARSCR, m_btnClear );
	DDX_Control( pDX, IDC_COUNT, m_btnYes );
	DDX_Control( pDX, IDCANCEL, m_btnNo );	
	DDX_Control( pDX, IDC_SLIDER1, m_sld );

	DDX_Radio( pDX, IDC_RAD_NORMAL, m_iShowMode );

	if( pDX->m_bSaveAndValidate )
	 {
       TExpertApp *pApp = (TExpertApp*)AfxGetApp();
       TMainFrame *pFram = (TMainFrame*)pApp->m_pMainWnd;  
	   TUserLocal *pUs = pFram->GetCurUser();

       if( pUs->m_iCalcShowDigitMode != m_iShowMode  ||
		   pUs->m_iCalcPrec != m_iPrec
		 )
        {
	      pUs->m_iCalcShowDigitMode = m_iShowMode;
	      pUs->m_iCalcPrec = m_iPrec;		
	      pUs->ModifyData();
		}
	 }
	 //CDataExchange* pDX, int nIDC, int& value );	
 }

BOOL TCalcDialog::PreTranslateMessage( MSG* pMsg )
 {
   if( (pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP) &&
	    pMsg->wParam == VK_RETURN && ::GetFocus() == m_resultsbox.m_hWnd
     )
	{
      if( pMsg->message == WM_KEYUP ) return TRUE;
	  int iSel = m_resultsbox.GetCurSel();
	  if( iSel == LB_ERR || !m_resultsbox.ArrayOfAttr.GetSize() ) return  TRUE;
      m_edit.SetWindowText( m_resultsbox.ArrayOfAttr[iSel].str );
	  return TRUE;
	}
   else return CDialog::PreTranslateMessage( pMsg );
 }

void TCalcDialog::OnDblClk()
 {
   int iSel = m_resultsbox.GetCurSel();
   if( iSel == LB_ERR || !m_resultsbox.ArrayOfAttr.GetSize() ) return;
   m_edit.SetWindowText( m_resultsbox.ArrayOfAttr[iSel].str );
 }

void TCalcDialog::OnRadio( UINT uiId )
 {
   if( uiId == IDC_RAD_NORMAL ) m_iShowMode = 0;
   else if( uiId == IDC_RAD_FIXED ) m_iShowMode = 1;
   else m_iShowMode = 2;
 }

BEGIN_MESSAGE_MAP(TCalcDialog, CDialog)
	//{{AFX_MSG_MAP(TCalcDialog)
	ON_BN_CLICKED( IDC_COUNT, OnCount )
	ON_BN_CLICKED( IDC_CALC_CLEARSCR, OnClear )
	ON_LBN_DBLCLK( IDC_LIST, OnDblClk )
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE( BN_CLICKED, IDC_RAD_NORMAL, IDC_RAD_SCIENC, OnRadio )
	ON_WM_DESTROY()	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TCalcDialog message handlers


BOOL TCalcDialog::OnInitDialog() 
 {
	CDialog::OnInitDialog();

	m_resultsbox.Align = left_;

	//m_results.InsertColumn( 0, "",  LVCFMT_RIGHT, 257);
	
	//CImageList* il = m_results.GetImageList( LVSIL_NORMAL );
	
	/*il->Add( AfxGetApp()->LoadStandardIcon( IDI_HAND ) );
	il->Add( AfxGetApp()->LoadStandardIcon( IDI_QUESTION ) );
	il->Add( AfxGetApp()->LoadStandardIcon( IDI_EXCLAMATION ) );
	il->Add( AfxGetApp()->LoadStandardIcon( IDI_ASTERISK ) );
	*/

	m_hIcCalc = AfxGetApp()->LoadIcon( IDI_ICON_CALC );
	SetIcon( m_hIcCalc, TRUE );	
	

    m_bmpClear.LoadBitmap( IDB_BMP_CALC_SCR );
	m_bmpYes.LoadBitmap( IDB_BMP_CALC_YES );
	m_bmpNo.LoadBitmap( IDB_BMP_CALC_CANCEL );

	m_btnClear.SetBitmap( (HBITMAP)m_bmpClear );
	m_btnYes.SetBitmap( (HBITMAP)m_bmpYes );
	m_btnNo.SetBitmap( (HBITMAP)m_bmpNo );

	m_sld.SetRange( 1, 17, FALSE );
	m_sld.SetPos( m_iPrec );
	m_sld.SetTicFreq( 1 );

	CenterWindow();	

	return TRUE;
 }

void TCalcDialog::OnClear() 
 {
   m_resultsbox.ResetContent();
   m_resultsbox.ArrayOfAttr.RemoveAll();
 }

int TCalcDialog::Ask( TVariantVar* pVar )
 {
   TAskDlg dlg( this, pVar, m_kX, m_kY );
   return dlg.DoModal();
 }

void TCalcDialog::OnCount() 
{	
	CString exp;
	m_edit.GetWindowText( exp );
	if( exp.IsEmpty() )
	 {
       MessageBox( "Нужно ввести арифметическое выражение", "Предупреждение", 
		 MB_OK|MB_ICONINFORMATION );
	   return;
	 }
	m_resultsbox.AddString( exp );

	TNameSpace nmSpc;
    TExpression expr( exp, nmSpc );          
	TVariantVar var;
	CString result;
	m_iPrec = m_sld.GetPos();

	CString strVals;

	bool bFlSta = true;	
	while( 1 )
	 {
	   try {
		   if( bFlSta == true ) expr.Parse( false ), bFlSta = false;
		   expr.Val( var );
		   var.ValToStr( result, m_iPrec, m_iShowMode );
		}
	   catch( TMyException *pE ) 
		{
		  if( pE->GetSubCode() == TREG_TVarException && 
			  pE->GetExcCode() == VEX_NotInitialized 
	 		)
		   {
             TVarException *pVarExc = (TVarException*)pE;
			 if( Ask(pVarExc->p1) == IDCANCEL ) 
			  {
				m_resultsbox.AddString( "Отмена", RGB( 255, 0, 0 ), TRUE );
				m_resultsbox.SetCurSel( m_resultsbox.GetCount() - 1 );
				return;
			  }

			 CString str2, str3;
			 pVarExc->p1->ValToStr( str3, m_iPrec, m_iShowMode );
			 str2.Format( "%s = %s[%s]", (LPCSTR)pVarExc->p1->m_strName,
			   (LPCSTR)str3, (LPCSTR)pVarExc->p1->GetTypeStr() );
			 if( !strVals.IsEmpty() ) strVals += ", ";
			 strVals += str2;

			 continue;
		   }
		  CString str( pE->GetMsg() );
		  pE->Delete();
		  
		  m_resultsbox.AddString( str, RGB( 255, 0, 0 ), TRUE );	  
		  m_resultsbox.SetCurSel( m_resultsbox.GetCount() - 1 );
		  return;
		}  
	   break;
	 }

	if( !strVals.IsEmpty() )
	  m_resultsbox.AddString( strVals, RGB( 0, 0, 0 ), FALSE );		  
	result += CString(" [" ) + var.GetTypeStr() + CString("]" );
	m_resultsbox.AddString( result,  RGB( 0, 255, 0 ), TRUE );
	m_resultsbox.SetCurSel( m_resultsbox.GetCount() - 1 );
}

void TCalcDialog::OnCancel() 
 {	
	CDialog::OnCancel();	
	DestroyWindow();
 }

void TCalcDialog::PostNcDestroy()
 {
   CDialog::PostNcDestroy();
   delete this;
 }
