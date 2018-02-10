#include "dlgacess.hpp"
#include "vs.rh"

CAcessDialog::~CAcessDialog()
 {
 }

BOOL CAcessDialog::OnInitDialog()
 {
   CDialog::OnInitDialog();
   
   SetDlgItemText( IDC_EDIT_CURACESS, (LPCSTR)m_str );
   //::EnableWindow( ::GetDlgItem(m_hWnd, IDC_EDIT_CURACESS), FALSE );

   m_Icon.SubclassDlgItem( 99, this );
   HICON hic = m_Icon.SetIcon( ::LoadIcon(NULL, IDI_ASTERISK ) );
   if( hic ) DestroyIcon( hic );

   return FALSE;
 }
