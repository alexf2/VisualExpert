#include "ask.hpp"
#include "vs.hpp"
#include "var.hpp"



void TAskDlg::DoDataExchange( CDataExchange* pDX )
 {
   CDialog::DoDataExchange( pDX );         

   DDX_Control( pDX, IDC_EDIT_NAME, m_edVal );
   DDX_Control( pDX, IDC_EDIT_VAL, m_edVar );

   DDX_Text( pDX, IDC_EDIT_NAME, m_varDsk );
   DDX_Text( pDX, IDC_EDIT_VAL, m_varVal );

   if( pDX->m_bSaveAndValidate == TRUE )
	{
	  if( m_varVal.IsEmpty() )
	   {
		 ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( 
			GetParent()->m_hWnd, 
			"Нужно ввести хотябы один символ", 
			"Ошибка", MB_OK | MB_ICONEXCLAMATION );
		 
		 HWND hw1 = ::GetDlgItem( m_hWnd, IDC_EDIT_VAL );
		 PostMessage( WM_NEXTDLGCTL, (WPARAM)hw1, 1 );	        

		 pDX->Fail();	  	  	
	   }

	  TVariantVar var;
      if( !ReadConstant(var, (LPCSTR)m_varVal) )	  
	   {
         MessageBox( 		
			"Недопустимое значение. Нужно ввести лексему:\n"
			"строка: \"СЛОВО\"\n"
			"символ: 'C'\n"
			"действительное значение 1.25",
			"Ошибка", MB_OK | MB_ICONSTOP );
		 
		 HWND hw1 = ::GetDlgItem( m_hWnd, IDC_EDIT_VAL );
		 PostMessage( WM_NEXTDLGCTL, (WPARAM)hw1, 1 );	        

		 pDX->Fail();	  	  	
	   }	  

	  *m_pVar = var;
	}
 }


BOOL TAskDlg::OnInitDialog()
 {
   CDialog::OnInitDialog();
      
   
   HWND hw1 = ::GetDlgItem( m_hWnd, IDC_EDIT_VAL );
   PostMessage( WM_NEXTDLGCTL, (WPARAM)hw1, 1 );	        

   if( m_X != 65535 && m_Y != 65535 )
	 SetWindowPos( NULL, m_X, m_Y, 0, 0, 
	   SWP_NOSIZE|SWP_NOZORDER|SWP_NOREDRAW );
   else CenterWindow( m_mPar );

   return FALSE;
 }

void TAskDlg::OnOK()
 {
   CRect r; GetWindowRect( r );
   m_X = r.left, m_Y = r.top;

   CDialog::OnOK();
 }
