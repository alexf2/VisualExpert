#include "dlgach.hpp"
#include "vs.hpp"
#include "vs.rh"
#include "my_alho.hpp"

IMPLEMENT_DYNCREATE( TPRopPageUser, CPropertyPage )
IMPLEMENT_DYNCREATE( TPRopPageAdm, CPropertyPage )
IMPLEMENT_DYNCREATE( TAcessChangeDlg, TBWCCPropertySheet )



BEGIN_MESSAGE_MAP( TPRopPageUser, CPropertyPage )
  ON_WM_CTLCOLOR()
  ON_BN_CLICKED( IDC_PROP_RAD_ANONIM, OnAnonime )
  ON_BN_CLICKED( IDC_PROP_RAD_NAMED, OnNamed )
END_MESSAGE_MAP()



void TPRopPageUser::OnAnonime()
 {
   EnableNamed( false );
 }

void TPRopPageUser::OnNamed()
 {
   EnableNamed( true );
 }

void TPRopPageUser::EnableNamed( bool bFl )
 {
   ::EnableWindow( ::GetDlgItem(m_hWnd, IDC_PROP_ED_NAME), bFl );
   ::EnableWindow( ::GetDlgItem(m_hWnd, IDC_PROP_ED_FAM), bFl );   
 }

TPRopPageUser::~TPRopPageUser()
 {
 }


BOOL TPRopPageUser::OnInitDialog()
 {
   CPropertyPage::OnInitDialog();

   HWND hw1 = ::GetDlgItem( m_hWnd, IDC_PROP_ED_PASS1 );
   PostMessage( WM_NEXTDLGCTL, (WPARAM)hw1, 1 );	        

   EnableNamed( false );

   return FALSE;
 }


HBRUSH TPRopPageUser::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
 {
   HBRUSH hbr = CPropertyPage::OnCtlColor( pDC, pWnd, nCtlColor );

   if( nCtlColor == CTLCOLOR_BTN || nCtlColor == CTLCOLOR_EDIT || nCtlColor == CTLCOLOR_STATIC )
	{
	  int id = pWnd->GetDlgCtrlID();
	  if( id == IDC_STATIC_PASS1 || id == IDC_STATIC_PASS2 ||
	      id == IDC_PROP_RAD_ANONIM || id == IDC_PROP_ED_PASS1 ||
		  id == IDC_PROP_ED_PASS2
		)
	  pDC->SetTextColor( RGB(0,0,255) );
	}

   return hbr;
 }


BOOL TPRopPageAdm::OnInitDialog()
 {
   CPropertyPage::OnInitDialog();

   HWND hw1 = ::GetDlgItem( m_hWnd, IDC_PROP_ED_PASS1 );
   PostMessage( WM_NEXTDLGCTL, (WPARAM)hw1, 1 );	        

   return FALSE;
 }

void TPRopPageUser::DoDataExchange( CDataExchange* pDX )
 {
   CPropertyPage::DoDataExchange( pDX );

   //DDX_Control( pDX, IDC_PROP_RAD_ANONIM, m_btnAnonim );
   //DX_Control( pDX, IDC_PROP_RAD_NAMED, m_btnNamed );   
  
   //DDX_Radio( pDX, IDC_PROP_RAD_NAMED, m_iBtnNamed );   
   DDX_Radio( pDX, IDC_PROP_RAD_ANONIM, m_iBtnAnonim );   

   DDX_Text( pDX, IDC_PROP_ED_NAME, m_csFirst );
   DDV_MaxChars( pDX, m_csFirst, PU_MaxFirstName );   

   DDX_Text( pDX, IDC_PROP_ED_FAM, m_csSecond );
   DDV_MaxChars( pDX, m_csSecond, PU_MaxSecondName );   

   DDX_Text( pDX, IDC_PROP_ED_PASS1, m_csPass );
   DDV_MaxChars( pDX, m_csPass, PU_MaxPass );

   DDX_Text( pDX, IDC_PROP_ED_PASS2, m_csPass2 );         
   DDV_MaxChars( pDX, m_csPass2, PU_MaxPass2 );		

   if( m_iBtnAnonim == 1 && (!m_csFirst.GetLength() || !m_csSecond.GetLength()) )
	{
	  ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( 
	     GetParent()->m_hWnd, 
		 "Пустое имя пользователя", 
		 "Ошибка", MB_OK | MB_ICONEXCLAMATION );
	  
	  HWND hw1 = ::GetDlgItem(m_hWnd, IDC_PROP_ED_NAME);
	  PostMessage( WM_NEXTDLGCTL, (WPARAM)hw1, 1 );	        

	  pDX->Fail();	  	  
	}

   if( !m_csPass.GetLength() && !m_csPass2.GetLength() ) return;

   m_csFirst.MakeUpper();
   m_csSecond.MakeUpper();
   m_csPass.MakeUpper();
   m_csPass2.MakeUpper();

   CString strTmp; 
   LPSTR lp = strTmp.GetBuffer( m_csPass.GetLength() + 1 );
   KillSpaces( (LPCSTR)m_csPass, lp );
   m_csPass = lp;

   lp = strTmp.GetBuffer( m_csPass2.GetLength() + 1 );
   KillSpaces( (LPCSTR)m_csPass2, lp );
   m_csPass2 = lp;

   lp = strTmp.GetBuffer( m_csFirst.GetLength() + 1 );
   KillSpaces( (LPCSTR)m_csFirst, lp );
   m_csFirst = lp;

   lp = strTmp.GetBuffer( m_csSecond.GetLength() + 1 );
   KillSpaces( (LPCSTR)m_csSecond, lp );
   m_csSecond = lp;

   if( m_csPass.Compare((LPCSTR)m_csPass2) ) 
	{
	  ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( 
	     GetParent()->m_hWnd, 
		 "Не совпадение повторного ввода пароля. Повторите ввод.", 
		 "Ошибка", MB_OK | MB_ICONEXCLAMATION );
      //AfxMessageBox( "Не совпадение повторного ввода пароля", MB_OK | MB_ICONEXCLAMATION );
	  
	  HWND hw1 = ::GetDlgItem(m_hWnd, IDC_PROP_ED_PASS1),
	       hw2 = ::GetDlgItem(m_hWnd, IDC_PROP_ED_PASS2);
	  ::PostMessage( hw1, WM_CLEAR, 0, 0 );
	  ::PostMessage( hw2, WM_CLEAR, 0, 0 );	  

	  PostMessage( WM_NEXTDLGCTL, (WPARAM)hw1, 1 );	        

	  pDX->Fail();
	}
 }



TPRopPageAdm::~TPRopPageAdm()
 {   
 }

void TPRopPageAdm::DoDataExchange( CDataExchange* pDX )
 {
   CPropertyPage::DoDataExchange( pDX );

   DDX_Text( pDX, IDC_PROP_ED_PASS1, m_csPass );
   DDX_Text( pDX, IDC_PROP_ED_PASS2, m_csPass2 );

   DDV_MaxChars( pDX, m_csPass, PU_MaxPass );
   DDV_MaxChars( pDX, m_csPass2, PU_MaxPass2 );		

   if( !m_csPass.GetLength() && !m_csPass2.GetLength() ) return;

   m_csPass.MakeUpper();
   m_csPass2.MakeUpper();

   CString strTmp; 
   LPSTR lp = strTmp.GetBuffer( m_csPass.GetLength() + 1 );
   KillSpaces( (LPCSTR)m_csPass, lp );
   m_csPass = lp;

   lp = strTmp.GetBuffer( m_csPass2.GetLength() + 1 );
   KillSpaces( (LPCSTR)m_csPass2, lp );
   m_csPass2 = lp;

   if( m_csPass.Compare((LPCSTR)m_csPass2) ) 
	{
	  ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( 
	     GetParent()->m_hWnd, 
		 "Не совпадение повторного ввода пароля. Повторите ввод.", 
		 "Ошибка", MB_OK | MB_ICONEXCLAMATION );
      //AfxMessageBox( "Не совпадение повторного ввода пароля", MB_OK | MB_ICONEXCLAMATION );
	  
	  HWND hw1 = ::GetDlgItem(m_hWnd, IDC_PROP_ED_PASS1),
	       hw2 = ::GetDlgItem(m_hWnd, IDC_PROP_ED_PASS2);
	  ::PostMessage( hw1, WM_CLEAR, 0, 0 );
	  ::PostMessage( hw2, WM_CLEAR, 0, 0 );	  

	  PostMessage( WM_NEXTDLGCTL, (WPARAM)hw1, 1 );	        

	  pDX->Fail();
	}
 }
//CString m_csPass, m_csPass2;


TAcessChangeDlg::~TAcessChangeDlg()
 {

 }



TAcessChangeDlg::TAcessChangeDlg( LPCTSTR pszCaption, CWnd *pParentWnd, UINT iSelectPage ):
  TBWCCPropertySheet( pszCaption, pParentWnd, iSelectPage ),

    m_pagUser( IDD_PROP_USER ),
	m_pagExp( IDD_PROP_EXPERT ),
	m_pagKognit( IDD_PROP_KOGNIT ),
	m_pagAdm( IDD_PROP_ADM )

 {
   AddPage( &m_pagUser ); 
   AddPage( &m_pagExp ); 
   AddPage( &m_pagKognit ); 
   AddPage( &m_pagAdm );       
 }


/*BOOL TAcessChangeDlg::Create( LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext )
 {
   dwStyle |= WS_SYSMENU;
   return CPropertySheet::Create( lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext );
 }*/


