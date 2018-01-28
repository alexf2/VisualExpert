#include "cre_dlg.hpp"
#include "vs.hpp"


void TDlgType::DoDataExchange( CDataExchange* pDX )
 {
   CDialog::DoDataExchange( pDX );         

   DDX_Control( pDX, IDC_RAD_RULES, m_bRul );
   DDX_Control( pDX, IDC_RAD_FAC, m_bFac );
   DDX_Control( pDX, IDC_RAD_Q, m_bQ );   

   DDX_Radio( pDX, IDC_RAD_RULES, m_iRes );	

   DDX_Text( pDX, IDC_ED_NAME, m_str );
   DDV_MaxChars( pDX, m_str, 256 );   

   if( pDX->m_bSaveAndValidate == TRUE && m_str.IsEmpty() )
	{
	  ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( 
	     GetParent()->m_hWnd, 
		 "Нужно ввести хотябы один символ", 
		 "Ошибка", MB_OK | MB_ICONEXCLAMATION );
	  
	  HWND hw1 = ::GetDlgItem( m_hWnd, IDC_ED_NAME );
	  PostMessage( WM_NEXTDLGCTL, (WPARAM)hw1, 1 );	        

	  pDX->Fail();	  	  	
	}
 }


BOOL TDlgType::OnInitDialog()
 {
   CDialog::OnInitDialog();
   
   CenterWindow( m_mPar );

   m_bRul.SetCheck( 0 ), m_bFac.SetCheck( 0 ), 
   m_bQ.SetCheck( 0 );   
      

   m_bRul.EnableWindow( m_bFlRul );
   m_bFac.EnableWindow( m_bFlFac ); 
   m_bQ.EnableWindow( m_bFlQ  );

   if( m_bFlRul ) m_bRul.SetCheck( 1 );
   else if( m_bFlFac ) m_bFac.SetCheck( 1 );
   else if( m_bFlQ ) m_bQ.SetCheck( 1 );
   

   SetWindowText( m_ttl );

   HWND hw1 = ::GetDlgItem( m_hWnd, IDC_ED_NAME );
   PostMessage( WM_NEXTDLGCTL, (WPARAM)hw1, 1 );	        

   return FALSE;
 }
