#include "ask_q.hpp"
#include "expdlg.hpp"


void TStaDlg::DoDataExchange( CDataExchange* pDX )
 {
   CDialog::DoDataExchange( pDX );         

   DDX_Control( pDX, IDC_LIST_Q, m_lbLst );
   DDX_Control( pDX, IDC_CHECK_REM, m_btnChk );
 }


BOOL TStaDlg::OnInitDialog()
 {
   CDialog::OnInitDialog();            
   
   CenterWindow();
   
   FillList();

   m_lbLst.SetCurSel( 
	m_iGoal > 0 && m_iGoal < m_lbLst.GetCount() ? m_iGoal:0 );

   m_btnChk.SetCheck( m_bFlRemove );

   HWND hw1 = ::GetDlgItem( m_hWnd, IDC_LIST_Q );
   PostMessage( WM_NEXTDLGCTL, (WPARAM)hw1, 1 );	        

   return FALSE;
 }

void TStaDlg::FillList()
 {
   TWndDrwLock lockR( &m_lbLst );

   vector<TQWork*>::iterator itSta( m_pW->m_vecQ.begin() ); 
   vector<TQWork*>::iterator itEnd( m_pW->m_vecQ.end() ); 
 
   CString str;
   for( ; itSta != itEnd; ++itSta )
	{
      str.Format( "%s [%s]", (LPCSTR)(*itSta)->m_strComment, (LPCSTR)(*itSta)->m_strQ );
	  m_lbLst.AddString( str );
	}
 }

void TStaDlg::OnOK()
 {
   int iRes = m_lbLst.GetCurSel();

   if( iRes == LB_ERR )
	{
      MessageBox( "Нужно выбрать вопрос из списка", "Предупреждение", MB_OK|MB_ICONEXCLAMATION );
	  return;
	}

   m_iGoal = iRes;
   m_bFlRemove = m_btnChk.GetCheck();

   CDialog::OnOK();
 }
