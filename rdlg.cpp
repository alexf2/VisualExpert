#include "rdlg.hpp"
#include "mfrexp.hpp"
#include "vs.hpp"
#include "userdlg.hpp"
#include "userlvl.hpp"
#include "expdlg.hpp"

BEGIN_MESSAGE_MAP( TAccDlg, CDialog )  
  ON_WM_CTLCOLOR()  
  ON_BN_CLICKED( IDC_CHECK_EXP2, OnExp )
  ON_BN_CLICKED( IDC_CHECK_KOG2, OnKog )
  ON_BN_CLICKED( IDC_CHECK_US2, OnUs )

  ON_BN_CLICKED( IDC_BTN_ADD, OnAdd )
  ON_BN_CLICKED( IDC_BTN_REMOVE, OnRemove )
  ON_BN_CLICKED( IDC_BTN_SETOWNER, OnSetOwner )
END_MESSAGE_MAP()

static bool FormatDescrUs( TUser* pUs, CString& str, TMFOptions *pFOpt, bool bCheckOpt, bool bExt );

TAccDlg::~TAccDlg()
 {
   vector<TUserLocal*>::iterator it( m_lstUsers.begin() );
   vector<TUserLocal*>::iterator itEnd( m_lstUsers.end() );

   for( ; it != itEnd; ++it )
	 delete *it;
   
   m_lstUsers.clear();


   list<TUser*>::iterator it2( m_lstUsersExt.begin() );
   list<TUser*>::iterator itEnd2( m_lstUsersExt.end() );

   for( ; it2 != itEnd2; ++it2 )
	 delete *it2;
   
   m_lstUsersExt.clear();   

   m_bDirty = false;
 }

   
   
void TAccDlg::OnExp()
 {
   TMFOptions *pFOpt = ((TMainFrame*)(AfxGetApp()->m_pMainWnd))->GetOptPtr();
   pFOpt->d.m_bExp2   = m_chExp_2.GetCheck();
   FillObjList();
 }

void TAccDlg::OnKog()
 {
   TMFOptions *pFOpt = ((TMainFrame*)(AfxGetApp()->m_pMainWnd))->GetOptPtr();
   pFOpt->d.m_bKogn2  = m_chKog_2.GetCheck();
   FillObjList();
 }

void TAccDlg::OnUs()
 {
   TMFOptions *pFOpt = ((TMainFrame*)(AfxGetApp()->m_pMainWnd))->GetOptPtr();
   pFOpt->d.m_bUs2    = m_chUs_2.GetCheck();
   FillObjList();
 }

void TAccDlg::OnAdd()
 {
   HTREEITEM hCur = m_treeUs.GetSelectedItem();
   if( !hCur )
	{
      MessageBox( "Нужно в левом списке выбрать элемент", "Нельзя добавить", MB_OK|MB_ICONINFORMATION );
	  return;
	}

   TUserLocal *pD = (TUserLocal*)m_treeUs.GetItemData( hCur );
   if( !pD )
	{
      MessageBox( "Это не объект", "Нельзя добавить", MB_OK|MB_ICONEXCLAMATION );
	  return;
	}

   TUser *pUs = FindUs( pD );
   if( pUs )
	{
	  CString str; 
	  str.Format( "%s  [%s %s] уже имеется в списке расширенного доступа", 
	   pUs->GetStrClass(), 
	   pUs->GetSubClass() == TAcessLevel::TUS_Named ? pUs->FName():"Неименованный",
	   pUs->GetSubClass() == TAcessLevel::TUS_Named ? pUs->SName():"" );
      MessageBox( str, "Нельзя добавить", MB_OK|MB_ICONINFORMATION );
	  return;
	}

   pUs = new TUser( *pD );
   m_lstUsersExt.push_back( pUs );

   CString str;
   TMFOptions *pFOpt = ((TMainFrame*)(AfxGetApp()->m_pMainWnd))->GetOptPtr();
   FormatDescrUs( pUs, str, pFOpt, false, false );

   m_lstObjUs.SetItemDataPtr( m_lstObjUs.AddString(str), pUs );

   FillEdOwn();
   //FillObjList();
 }

void TAccDlg::OnRemove()
 {
   int iCur = m_lstObjUs.GetCurSel();
   if( iCur == LB_ERR )
	{
      MessageBox( "Нужно выбрать пользователя в правом списке", "Нельзя удалить", MB_OK|MB_ICONINFORMATION );
	  return;
	}

   TUser *pUs = (TUser*)m_lstObjUs.GetItemData( iCur );

   if( pUs == *m_lstUsersExt.begin() )
	{
      MessageBox( "Нельзя удалить владельца", "Нельзя удалить", MB_OK|MB_ICONEXCLAMATION );
	  return;
	}
   
   delete pUs;
   m_lstUsersExt.remove( pUs );
   m_lstObjUs.DeleteString( iCur );

   iCur = m_lstObjUs.GetCurSel();
   if( iCur == LB_ERR ) m_lstObjUs.SetCurSel( m_lstObjUs.GetCount() - 1 );
 }

void TAccDlg::OnSetOwner()
 {   
   int iCur = m_lstObjUs.GetCurSel();
   if( iCur == LB_ERR )
	{
      MessageBox( "Нужно в правом списке выбрать элемент", "Нельзя назначить", MB_OK|MB_ICONINFORMATION );
	  return;
	}
   
   TUser *pUs = (TUser*)m_lstObjUs.GetItemData( iCur );
   if( pUs == *m_lstUsersExt.begin() )
	{
      MessageBox( "Это уже хозяин объекта", "Нельзя назначить", MB_OK|MB_ICONEXCLAMATION );
	  return;
	}
      
   m_lstUsersExt.remove( pUs );
   m_lstUsersExt.push_front( pUs );	  
   

   FillEdOwn();
   FillObjList();
 }

TUser* TAccDlg::FindUs( TUserLocal* pUsl )
 {
   list<TUser*>::iterator it2( m_lstUsersExt.begin() );
   list<TUser*>::iterator itEnd2( m_lstUsersExt.end() );

   for( ; it2 != itEnd2; ++it2 )
	 if( **it2 == *pUsl && (*it2)->SameRights(*pUsl) ) return *it2;

   return NULL;
 }

BOOL TAccDlg::OnInitDialog()
 {
   CDialog::OnInitDialog();   

   m_bmAdd.LoadBitmap( IDB_BTN_ADDUSER );
   m_bmRemove.LoadBitmap( IDB_BTN_REMOVEUSER );
   m_bmSetOwner.LoadBitmap( IDB_BTN_SETOWNER );

   BITMAP bm;
   ::GetObject( (HBITMAP)m_bmAdd, sizeof(bm), &bm ); 
   BITMAP bm2;
   ::GetObject( (HBITMAP)m_bmSetOwner, sizeof(bm2), &bm2 ); 

   m_bbAddUser.SetBitmap( (HBITMAP)m_bmAdd );
   m_bbRemoveUser.SetBitmap( (HBITMAP)m_bmRemove );
   m_bbSetOwner.SetBitmap( (HBITMAP)m_bmSetOwner );

   bm.bmWidth += 8, bm.bmHeight += 8;
   bm2.bmWidth += 8, bm2.bmHeight += 8;

   //CRect r; GetClientRect( r );
   //int iL = (r.Width() - bm.bmWidth) / 2;

   CRect r2; m_treeUs.GetWindowRect( r2 ); ScreenToClient( r2 );
   CRect r3; m_lstObjUs.GetWindowRect( r3 ); ScreenToClient( r3 );
   int iT = r2.top + (r2.Height() - bm.bmHeight*3) / 2;

   int iL = r2.right + (r3.left - r2.right - bm.bmWidth) / 2;

   m_bbAddUser.SetWindowPos( NULL, iL, iT, bm.bmWidth, bm.bmHeight, 
     SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOREDRAW );
   m_bbRemoveUser.SetWindowPos( NULL, iL, iT+bm.bmHeight*2, bm.bmWidth, bm.bmHeight, 
     SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOREDRAW );  
  
   TMFOptions *pFOpt = ((TMainFrame*)(AfxGetApp()->m_pMainWnd))->GetOptPtr();
   
   m_chExp_2.SetCheck( pFOpt->d.m_bExp2 );
   m_chKog_2.SetCheck( pFOpt->d.m_bKogn2 );
   m_chUs_2.SetCheck( pFOpt->d.m_bUs2 );

   CString strPath( CString(((TExpertApp*)AfxGetApp())->GetStartPath()) + CString(GLOBAL_USERS_FILENAME) );
   LoadUserList_Internal( strPath, m_lstUsers, *pFOpt );
   
   FillTree();
     
   list<TUser*>& rLstObj = m_pObj->GetList();
   list<TUser*>::iterator it2( rLstObj.begin() );
   list<TUser*>::iterator itEnd2( rLstObj.end() );

   for( ; it2 != itEnd2; ++it2 )
	{
      TUser *pUs = new TUser( **it2 );
	  m_lstUsersExt.push_back( pUs );
	}  

   FillObjList();
   FillEdOwn();

   CenterWindow( GetParent() );
   
   SetWindowText( "Доступ к: " + m_strTtl );
   
   return FALSE;
 }

void TAccDlg::FillTree()
 {
   TWndDrwLock lockR( &m_treeUs );

   m_treeUs.DeleteAllItems();

   TV_INSERTSTRUCT tv; memset( &tv, 0, sizeof(TV_INSERTSTRUCT) );
   tv.hInsertAfter = TVI_LAST;
   tv.item.state = TVIS_BOLD;
   tv.item.stateMask = TVIS_BOLD;
   tv.item.mask = TVIF_TEXT | TVIF_STATE;
      
   
   CString str;
   str.Format( "Администратор" );
   tv.item.pszText = (LPSTR)(LPCSTR)str;
   HTREEITEM hRootAdm = m_treeUs.InsertItem( &tv ); 
   //m_treeUs.SetItemData( hRootAdm, NULL );

   str.Format( "Когнитологи" );
   tv.item.pszText = (LPSTR)(LPCSTR)str;
   HTREEITEM hRootKog = m_treeUs.InsertItem( &tv ); 
   m_treeUs.SetItemData( hRootKog, NULL );

   str.Format( "Эксперты" );
   tv.item.pszText = (LPSTR)(LPCSTR)str;
   HTREEITEM hRootExp = m_treeUs.InsertItem( &tv ); 
   m_treeUs.SetItemData( hRootExp, NULL );

   str.Format( "Пользователи" );
   tv.item.pszText = (LPSTR)(LPCSTR)str;
   HTREEITEM hRootUs = m_treeUs.InsertItem( &tv ); 
   m_treeUs.SetItemData( hRootUs, NULL );

   vector<TUserLocal*>::iterator it( m_lstUsers.begin() );
   vector<TUserLocal*>::iterator itEnd( m_lstUsers.end() );

   memset( &tv, 0, sizeof(TV_INSERTSTRUCT) );

   for( ; it != itEnd; ++it )
	{
      HTREEITEM hit;
	  switch( (*it)->GetClass() )
	   {
	     case TAcessLevel::TU_Administrator:		   
		   m_treeUs.SetItemData( hRootAdm, (DWORD)*it );
           continue;
		   break;

	     case TAcessLevel::TU_Kognitolog:
		   hit = hRootKog;
		   break;

	     case TAcessLevel::TU_Expert:
		   hit = hRootExp;
		   break;

	     case TAcessLevel::TU_User:
		   hit = hRootUs;
		   break;
	   };

	  CString str2;
      if( (*it)->GetSubClass() == TAcessLevel::TUS_Named )
	    str2.Format( "%s %s", (*it)->FName(), (*it)->SName() );
      else
	    str2.Format( "--- Неименованный ---" );	     
	  
      tv.hInsertAfter = TVI_LAST;
      tv.item.mask = TVIF_TEXT;   
      tv.hParent = hit;   

      tv.item.pszText = (LPSTR)(LPCSTR)str2;
      
	  m_treeUs.SetItemData( m_treeUs.InsertItem(&tv), (DWORD)*it );
	}      
 }

void TAccDlg::FillEdOwn()
 {   
   CString str;
   TMFOptions *pFOpt = ((TMainFrame*)(AfxGetApp()->m_pMainWnd))->GetOptPtr();

   if( !m_lstUsersExt.size() )
	{
      MessageBox( "Объект вызванный на редактирование доступа имеет ошибочный список доступа", "Ошибка", MB_OK|MB_ICONSTOP );
	  EndDialog( IDCANCEL );
	  return;
	}

   FormatDescrUs( *m_lstUsersExt.begin(), str, pFOpt, false, true );

   m_edOwner.SetWindowText( str );
 }

static bool FormatDescrUs( TUser* pUs, CString& str, TMFOptions *pFOpt, bool bCheckOpt, bool bExt )
 {    
   switch( pUs->GetClass() )
	{
	  case TAcessLevel::TU_Administrator:		   
		str.Format( "[Администратор] " );
		break;

	  case TAcessLevel::TU_Kognitolog:
		if( bCheckOpt == true && pFOpt->d.m_bKogn2 == false ) return false;
		str.Format( bExt == true ? "[Когнитолог] ":"[К] " );
		break;

	  case TAcessLevel::TU_Expert:
		if( bCheckOpt == true && pFOpt->d.m_bExp2 == false ) return false;
		str.Format( bExt == true ? "[Эксперт] ":"[Э] " );
		break;

	  case TAcessLevel::TU_User:
		if( bCheckOpt == true && pFOpt->d.m_bUs2 == false ) return false;
		str.Format( bExt == true ? "[Пользователь] ":"[П] " );
		break;
	};

   CString str2;
   if( pUs->GetSubClass() == TAcessLevel::TUS_Named )
	 str2.Format( "%s %s", pUs->FName(), pUs->SName() );
   else
	 str2.Format( "--- Неименованный ---" );	     

   str += str2;	  

   return true;
 }

void TAccDlg::FillObjList()
 {
   TWndDrwLock lockR( &m_lstObjUs );
   m_lstObjUs.ResetContent();

   TMFOptions *pFOpt = ((TMainFrame*)(AfxGetApp()->m_pMainWnd))->GetOptPtr();

   list<TUser*>::iterator it2( m_lstUsersExt.begin() );
   list<TUser*>::iterator itEnd2( m_lstUsersExt.end() );
      
   for( ; it2 != itEnd2; ++it2 )
	{       
      CString str;
	  if( FormatDescrUs(*it2, str, pFOpt, true, false) == false ) continue;

	  m_lstObjUs.SetItemDataPtr( m_lstObjUs.AddString(str), *it2 );
	}
 }

void TAccDlg::OnOK()
 {
   TMFOptions *pFOpt = ((TMainFrame*)(AfxGetApp()->m_pMainWnd))->GetOptPtr();

   pFOpt->d.m_bExp2   = m_chExp_2.GetCheck();
   pFOpt->d.m_bKogn2  = m_chKog_2.GetCheck();
   pFOpt->d.m_bUs2    = m_chUs_2.GetCheck();


   list<TUser*>& rLstObj = m_pObj->GetList();
   list<TUser*>::iterator it( rLstObj.begin() );
   list<TUser*>::iterator itEnd( rLstObj.end() );
   for( ; it != itEnd; ++it )
	 delete *it;

   rLstObj.clear();
   rLstObj.merge( m_lstUsersExt );

   TUser *pUs = *rLstObj.begin();

   m_pObj->SetRights( pUs->GetClass(), pUs->GetSubClass() );
   
   //m_lstUsersExt.clear();
   
   CDialog::OnOK();
 }
void TAccDlg::OnCancel()
 {
   CDialog::OnCancel();
 }

void TAccDlg::DoDataExchange( CDataExchange* pDX )
 {
   CDialog::DoDataExchange( pDX );

   DDX_Control( pDX, IDC_BTN_ADD, m_bbAddUser );
   DDX_Control( pDX, IDC_BTN_REMOVE, m_bbRemoveUser );
   DDX_Control( pDX, IDC_BTN_SETOWNER, m_bbSetOwner );

   DDX_Control( pDX, IDC_LIST_USERS, m_treeUs );
   DDX_Control( pDX, IDC_LIST_ACC, m_lstObjUs );

   DDX_Control( pDX, IDC_ED_OWNER, m_edOwner );
  
   DDX_Control( pDX, IDC_CHECK_EXP2, m_chExp_2 );
   DDX_Control( pDX, IDC_CHECK_KOG2, m_chKog_2 );
   DDX_Control( pDX, IDC_CHECK_US2, m_chUs_2 );
 }
  
HBRUSH TAccDlg::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
 {
   HBRUSH hbr = CDialog::OnCtlColor( pDC, pWnd, nCtlColor );

   if( nCtlColor == CTLCOLOR_STATIC )
	{
	  int id = pWnd->GetDlgCtrlID();
	  if( id == ID_LST_OWN || id == ID_STAT_TTLLOWN )
	    pDC->SetTextColor( RGB(0,0,255) );
	}

   return hbr;
 }
  
/*  
protected:
  TProtectedObj* m_pObj;
  CBitmap m_bmAdd, m_bmRemove, m_bmSetOwner;
  
  DECLARE_MESSAGE_MAP() 
*/