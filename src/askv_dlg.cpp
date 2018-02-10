#include "askv_dlg.hpp"
#include "vs.rh"
#include "vs.hpp"


void TAskVarDlg::DoDataExchange( CDataExchange* pDX )
 {
   CDialog::DoDataExchange( pDX );         

   DDX_Control( pDX, IDC_EDIT_NAME, m_edVal );
   DDX_Control( pDX, IDC_EDIT_VAL, m_edVar );

   DDX_Text( pDX, IDC_EDIT_NAME, m_varDsk );
   DDX_Text( pDX, IDC_EDIT_VAL, m_varVal );

   DDX_Text( pDX, IDC_EDIT_KD, m_dKD );
   DDV_MinMaxDouble( pDX, m_dKD, 0, 1 );	

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

	  try {
	    *m_pVar = var;
	   }
	  catch( TMyException *pE )
	   {
		 MessageBox( pE->GetMsg(), "Ошибка", 
			  MB_OK|MB_ICONSTOP );
		 pE->Delete();
		 pDX->Fail();
	   }
	}
 }


BOOL TAskVarDlg::OnInitDialog()
 {
   CDialog::OnInitDialog();
      
   
   HWND hw1 = ::GetDlgItem( m_hWnd, IDC_EDIT_VAL );
   PostMessage( WM_NEXTDLGCTL, (WPARAM)hw1, 1 );	        

   CenterWindow();

   return FALSE;
 }
