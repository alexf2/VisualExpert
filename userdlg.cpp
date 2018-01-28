#include "userdlg.hpp"
#include "vs.hpp"
#include "vs.rh"
#include "dlgach.hpp"
#include "my_alho.hpp"
#include "vessubj.hpp"

#include "..\\RUDAKOW\\SERVIS\\ctxhold.hpp"

#include <algorithm>
#include <io.h>
#include <direct.h>
#include <errno.h>

IMPLEMENT_DYNCREATE( TPRopPageErgo, CPropertyPage )
IMPLEMENT_DYNCREATE( TPRopPageAcess, CPropertyPage )
//IMPLEMENT_DYNCREATE( TPRopPageUsers, CPropertyPage )

//IMPLEMENT_DYNCREATE( TUsersDialog, TBWCCPropertySheet )



BEGIN_MESSAGE_MAP( TPRopPageErgo, CPropertyPage )  
END_MESSAGE_MAP()


BEGIN_MESSAGE_MAP( TPRopPageAcess, CPropertyPage )  
END_MESSAGE_MAP()


BEGIN_MESSAGE_MAP( TPRopPageUsers, CPropPageTooltips )  
  ON_NOTIFY( LVN_ITEMCHANGED, IDC_LIST_USERS, ChSelect )
  ON_COMMAND( IDC_BTN_ADD,OnAdd )
  ON_COMMAND( IDC_BTN_REMOVE,OnRemove )
  ON_COMMAND( IDC_BTN_UP,OnUp )
  ON_COMMAND( IDC_BTN_DOWN,OnDown )
  ON_COMMAND( IDC_BTN_UPDATE,OnUpdate )
END_MESSAGE_MAP()


BEGIN_MESSAGE_MAP( TUsersDialog, TBWCCPropertySheet )  
  ON_WM_CREATE()

  ON_COMMAND( IDOK, OnOK )
END_MESSAGE_MAP()



TPRopPageErgo::~TPRopPageErgo()
 {
 }

BOOL TPRopPageErgo::OnInitDialog()
 {
   CPropertyPage::OnInitDialog();

   HWND hw1 = ::GetDlgItem( m_hWnd, IDC_CHECK_OPTSAVE );
   PostMessage( WM_NEXTDLGCTL, (WPARAM)hw1, 1 );	        

   return FALSE;
 }

void TPRopPageErgo::DoDataExchange( CDataExchange* pDX )
 {
   CPropertyPage::DoDataExchange( pDX );

   DDX_Check( pDX, IDC_CHECK_OPTSAVE, m_iSave );   
   DDX_Check( pDX, IDC_CHECK_OPTLOAD, m_iRestore );      
 }



TPRopPageAcess::~TPRopPageAcess()
 {
 }

BOOL TPRopPageAcess::OnInitDialog()
 {
   CPropertyPage::OnInitDialog();

   UpdateData( FALSE );
   m_Combo.AddString( "XOR с ключом до 8 байт" );
   m_Combo.AddString( "DES с ключом 8 байт" );  
   UpdateData( FALSE );

   HWND hw1 = ::GetDlgItem( m_hWnd, IDC_EDIT_PASSNUMB );
   PostMessage( WM_NEXTDLGCTL, (WPARAM)hw1, 1 );	        

   return FALSE;
 }

void TPRopPageAcess::DoDataExchange( CDataExchange* pDX )
 {
   CPropertyPage::DoDataExchange( pDX );

   DDX_Check( pDX, IDC_CHECK_USE_CODING, m_iUse );   
   DDX_Text( pDX, IDC_EDIT_PASSNUMB, m_iNumbPass );     
   DDV_MinMaxInt( pDX, m_iNumbPass, 1, 100 );

   DDX_Control( pDX, IDC_COMBO_ALHO, m_Combo );
   DDX_CBIndex( pDX, IDC_COMBO_ALHO, m_iAlho );

   DDX_Text( pDX, IDC_EDIT_KEY, m_strKey );
   DDV_MaxChars( pDX, m_strKey, 8 );		
 }



TPRopPageUsers::TPRopPageUsers( vector<TUserLocal*>& rUs, UINT uiRc ): 
  CPropPageTooltips( uiRc ),
  m_rlstUsers( rUs ),
  m_bFlDirty( false )
 {    
   
   *this + new TToolTipData( IDC_BTN_UPDATE, "Модификация выделенного пользователя" ) +

	       new TToolTipData( IDC_BTN_ADD, "Добавление пользователя" ) +
		   new TToolTipData( IDC_BTN_REMOVE, "Удаление выделенного пользователя" ) +
		   new TToolTipData( IDC_BTN_UP, "Шаг по списку вверх" ) +
		   new TToolTipData( IDC_BTN_DOWN, "Шаг по списку вниз" );
	
 }

TPRopPageUsers::~TPRopPageUsers()
 {
 }

void TPRopPageUsers::AssClm( CListCtrl& lst )
 {
   CRect rName;
   m_edName.GetWindowRect( rName );
   CRect rFam;
   m_edFam.GetWindowRect( rFam );
   CRect rPass;
   m_edPass.GetWindowRect( rPass );
   CRect rAlho;
   m_ComboClass.GetWindowRect( rAlho );

   CRect r;
   GetClientRect( r );
   int iDelta = r.Width() - rName.Width() - rFam.Width() -
	 rFam.Width() - rAlho.Width() - 8;
   iDelta = iDelta > 0 ? iDelta:0;

   
   LV_COLUMN lv; 

   //LVCF_SUBITEM
   lv.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
   lv.fmt = LVCFMT_LEFT;
   lv.pszText = "Имя";
   lv.iSubItem = 0;
   lv.cx = rName.Width() + 2;
   lv.cchTextMax = strlen( lv.pszText );
   lst.InsertColumn( 0, &lv );
   
   lv.pszText = "Фамилия";
   lv.iSubItem = 1;
   lv.cx = rFam.Width() + 2;
   lv.cchTextMax = strlen( lv.pszText );
   lst.InsertColumn( 1, &lv );

   lv.pszText = "Класс";
   lv.iSubItem = 2;
   lv.cx = rAlho.Width() + 2;
   lv.cchTextMax = strlen( lv.pszText );
   lst.InsertColumn( 2, &lv );

   lv.pszText = "Пароль";
   lv.iSubItem = 3;
   lv.cx = rPass.Width() + 2 + iDelta;
   lv.cchTextMax = strlen( lv.pszText );
   lst.InsertColumn( 3, &lv );
 }


void TPRopPageUsers::OnAddInternal( bool bFlUpdate )
 {
   int iInd = m_lstUsers.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED );
   if( iInd == -1 && bFlUpdate == true ) 
	{
      ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( m_hWnd, 
 	    "В списке нет выделенных пользователей", 
		"Ошибка", MB_OK|MB_ICONSTOP );
	  return;
	}

   TAutoMemoryRc<TUserLocal*> arcUs( new TUserLocal() );

   CString name, fname, passw;
   CString csTmp;
   LPSTR lp = csTmp.GetBuffer( 1024 );

   m_edName.GetWindowText( lp, 1000 );
   KillSpaces( lp, lp );
   name = lp;
   name.MakeUpper();

   m_edFam.GetWindowText( lp, 1000 );
   KillSpaces( lp, lp );
   fname = lp;
   fname.MakeUpper();

   m_edPass.GetWindowText( lp, 1000 );
   KillSpaces( lp, lp );
   passw = lp;   
   passw.MakeUpper();

   csTmp.ReleaseBuffer();

   if( name.GetLength() > PU_MaxFirstName ||
	   fname.GetLength() > PU_MaxFirstName 
	 )
	{
	  char cBuf[100];
	  sprintf( cBuf, "Длина имени не более %u знаков", (int)PU_MaxFirstName );
      ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( m_hWnd, 
 	    cBuf, 
		"Ошибка", MB_OK|MB_ICONSTOP );
	  return;
	}

   if( passw.GetLength() > PU_MaxPass )
	{
      char cBuf[100];
	  sprintf( cBuf, "Длина пароля не более %u знаков", (int)PU_MaxPass );
      ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( m_hWnd, 
 	    cBuf, 
		"Ошибка", MB_OK|MB_ICONSTOP );
	  return;
	}

	   

   TAcessLevel::TUserClass uClass;
   TAcessLevel::TUserSubclass usSubClass;

   if( name.IsEmpty() && fname.IsEmpty() )
	 usSubClass = TAcessLevel::TUS_Anonime;
   else usSubClass = TAcessLevel::TUS_Named;

   switch( m_ComboClass.GetCurSel() )
	{
	  case 0:
	    uClass = TAcessLevel::TU_Administrator;		 
		usSubClass = TAcessLevel::TUS_Administrator;
		if( !name.IsEmpty() || !fname.IsEmpty() )
		 {
            ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( m_hWnd, 
 	          "Администратор не имеет имени", 
		      "Ошибка", MB_OK|MB_ICONSTOP );
	       return;
		 }
        break;

	  case 1:
	    uClass = TAcessLevel::TU_Kognitolog;
	    break;

	  case 2:
	    uClass = TAcessLevel::TU_Expert;
	    break;

	  case 3:
	    uClass = TAcessLevel::TU_User;
	    break;

	  default:
	    return;
	};

   if( usSubClass != TAcessLevel::TUS_Named && bFlUpdate == false )
	{
      ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( m_hWnd, 
 	    "Анонимные пользователи уже существуют", 
		"Ошибка", MB_OK|MB_ICONSTOP );
	  return;
	}
   if( uClass == TAcessLevel::TU_Administrator && bFlUpdate == false )
	{
       ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( m_hWnd, 
 	     "Администратор уже есть", 
		 "Ошибка", MB_OK|MB_ICONSTOP );
	   return;
	}

   TAcessLevel::TUserClass uClass_;
   TAcessLevel::TUserSubclass usSubClass_;
   m_rlstUsers[ iInd ]->GetRights( uClass_, usSubClass_ );

   if( bFlUpdate == true && 
	   (uClass == TAcessLevel::TU_Administrator || usSubClass != TAcessLevel::TUS_Named)
     )
	{
	  if( uClass != uClass_ || usSubClass != usSubClass_ )
	   {
         ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( m_hWnd, 
 	      "Менять атрибуты доступа анонимных пользователей и администратора запрещено", 
		  "Ошибка", MB_OK|MB_ICONSTOP );
	     return;
	   }
	}
   if( bFlUpdate == true && 
	   (usSubClass == TAcessLevel::TUS_Named) &&
       usSubClass_ !=  TAcessLevel::TUS_Named
	 )
	{
	  ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( m_hWnd, 
 	      "У анонимного пользователя можно менять только пароль", 
		  "Ошибка", MB_OK|MB_ICONSTOP );
	     return;    
	}
   
   arcUs()->SetRights( uClass, usSubClass );
   arcUs()->SetFName( (LPCSTR)name );
   arcUs()->SetSName( (LPCSTR)fname );
   arcUs()->SetPassword( (LPCSTR)passw );
   
   typedef vector<TUserLocal*> TMyVec;
   TUserLocal* pUs = MyFind<TUserLocal*, TMyVec>( m_rlstUsers.begin(), m_rlstUsers.end(), arcUs() );
   if( pUs && bFlUpdate == false )
	{
      ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( m_hWnd, 
 	    "Такой пользователь уже есть. Используйте модификацию", 
		 "Ошибка", MB_OK|MB_ICONEXCLAMATION );
	}
   else
	{
	  if( bFlUpdate == true )
	   {
         *m_rlstUsers[ iInd ] = *arcUs();
		 //m_lstUsers.InvalidateRect( NULL );
         AddObj( m_lstUsers, arcUs(), iInd, true );

	     m_bFlDirty = true;
		 return;
	   }

      m_rlstUsers.push_back( arcUs() );	  
	  AddObj( m_lstUsers, arcUs(), m_lstUsers.GetItemCount() );
	  arcUs.ClearRc();

	  int iInd = m_lstUsers.GetItemCount() - 1;
	  if( iInd < 0 ) iInd = 0;	  
	   
	  LV_ITEM lvIt; memset( &lvIt, 0, sizeof(LV_ITEM) );
	  lvIt.mask = LVIF_STATE;
	  lvIt.iItem = iInd;
	  lvIt.stateMask = LVIS_SELECTED;
	  lvIt.state = LVIS_SELECTED;
	  m_lstUsers.SetItemState( iInd, &lvIt );		   

	  m_bFlDirty = true;
	}
	//TAcessLevel::TUserClass& uClass, TAcessLevel::TUserSubclass& usSubClass ) const
 }

void TPRopPageUsers::OnAdd()
 {
   OnAddInternal( false );
 }

bool TPRopPageUsers::IsRemovable( TUserLocal* pUs )
 {
   TAcessLevel::TUserClass uClass;
   TAcessLevel::TUserSubclass usSubClass;
   pUs->GetRights( uClass, usSubClass );

   return !(uClass == TAcessLevel::TU_Administrator ||
          usSubClass != TAcessLevel::TUS_Named);
 }

void TPRopPageUsers::OnRemove()
 {
   int iInd = m_lstUsers.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED );
   if( iInd == -1 ) return;

   TUserLocal* pUs = m_rlstUsers[ iInd ];

   if( IsRemovable(pUs) == false )
	{
      ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( m_hWnd, 
 	     "Это нельзя удалять", 
		 "Предупреждение", MB_OK|MB_ICONEXCLAMATION );
	}
   else
	{ 
	  m_rlstUsers.erase( find(m_rlstUsers.begin(), m_rlstUsers.end(), pUs) );
	  //remove( m_rlstUsers.begin(), m_rlstUsers.end(), m_rlstUsers[iInd] );
	  //m_rlstUsers.resize( m_rlstUsers.size() - 1 );
	  delete pUs; 
	  m_lstUsers.DeleteItem( iInd );

	  m_bFlDirty = true;

	  
	  if( iInd == m_lstUsers.GetItemCount() ) iInd--;
	  if( iInd < 0 ) iInd = 0;
	   
	  LV_ITEM lvIt; memset( &lvIt, 0, sizeof(LV_ITEM) );
	  lvIt.mask = LVIF_STATE;
	  lvIt.iItem = iInd;
	  lvIt.stateMask = LVIS_SELECTED;
	  lvIt.state = LVIS_SELECTED;
	  m_lstUsers.SetItemState( iInd, &lvIt );		   
	}
 }

void TPRopPageUsers::OnUp()
 {   
   int iInd = m_lstUsers.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED );
   if( iInd == -1 ) iInd = 0;
   else if( iInd > 0  ) iInd--;

   LV_ITEM lvIt; memset( &lvIt, 0, sizeof(LV_ITEM) );
   lvIt.mask = LVIF_STATE;
   lvIt.iItem = iInd;
   lvIt.stateMask = LVIS_SELECTED;
   lvIt.state = LVIS_SELECTED;
   m_lstUsers.SetItemState( iInd, &lvIt );	
 }

void TPRopPageUsers::OnDown()
 {
   int iInd = m_lstUsers.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED );
   if( iInd == -1 ) iInd = 0;
   else if( iInd < m_lstUsers.GetItemCount() - 1 ) iInd++;

   LV_ITEM lvIt; memset( &lvIt, 0, sizeof(LV_ITEM) );
   lvIt.mask = LVIF_STATE;
   lvIt.iItem = iInd;
   lvIt.stateMask = LVIS_SELECTED;
   lvIt.state = LVIS_SELECTED;
   m_lstUsers.SetItemState( iInd, &lvIt );	
 }

void TPRopPageUsers::OnUpdate()
 {
   OnAddInternal( true );
 }


void TPRopPageUsers::AddObj( CListCtrl& lst, TUserLocal* pUs, int iItem, bool bFlModify )
 {
   LV_ITEM lvIt;
   lvIt.mask = LVIF_TEXT;   
   
   CString str;
   LPSTR lp = str.GetBuffer( 1024 * 2 );
   lvIt.pszText = lp;   

   lvIt.iItem = iItem;	  

   TAcessLevel::TUserClass uClass;
   TAcessLevel::TUserSubclass usSubClass;
   pUs->GetRights( uClass, usSubClass );

   lvIt.iSubItem = 0; 
   if( usSubClass !=  TAcessLevel::TUS_Named )
     sprintf( lp, "---" );
   else
     sprintf( lp, "%s", pUs->FName() );
   if( bFlModify == false ) lst.InsertItem( &lvIt );
   else lst.SetItem( &lvIt );

   lvIt.iSubItem = 1;   
   if( usSubClass !=  TAcessLevel::TUS_Named )
     sprintf( lp, "---" );
   else
     sprintf( lp, "%s", pUs->SName() );
   lst.SetItem( &lvIt );

   lvIt.iSubItem = 2; 	  
   sprintf( lp, "%s", pUs->GetStrClass() );
   lst.SetItem( &lvIt );	 

   lvIt.iSubItem = 3;   
   sprintf( lp, "%s", pUs->Password() );
   lst.SetItem( &lvIt );	 

   str.ReleaseBuffer();
 }

void TPRopPageUsers::FillList( CListCtrl& lst )
 {     
   vector<TUserLocal*>::iterator it( m_rlstUsers.begin() );
   vector<TUserLocal*>::iterator itEnd( m_rlstUsers.end() );

   for( int i = 0; it != itEnd; i++, ++it )
     AddObj( lst, (*it), i ) ;
 }

BOOL TPRopPageUsers::OnInitDialog()
 {
   CPropPageTooltips::OnInitDialog();

   m_ComboClass.SubclassDlgItem( IDC_COMBO_CLASS, this );
   m_edName.SubclassDlgItem( IDC_EDIT_NAME, this );
   m_edFam.SubclassDlgItem( IDC_EDIT_FAM, this );
   m_edPass.SubclassDlgItem( IDC_EDIT_PASS, this );

   m_lstUsers.SubclassDlgItem( IDC_LIST_USERS, this );   
	//int nItem, UINT nState, UINT nMask );
   m_lstUsers.SetBkColor( GetSysColor(COLOR_BTNFACE) );
   m_lstUsers.SetTextBkColor( GetSysColor(COLOR_BTNFACE) );

   m_bbRemove.SubclassDlgItem( IDC_BTN_REMOVE, this );
   m_bbUp.SubclassDlgItem( IDC_BTN_UP, this );
   m_bbDown.SubclassDlgItem( IDC_BTN_DOWN, this );
   m_bbAdd.SubclassDlgItem( IDC_BTN_ADD, this );
   m_bbUpdate.SubclassDlgItem( IDC_BTN_UPDATE, this );

   AssClm( m_lstUsers );

   /*m_bbRemove.LoadBitmaps( IDB_BTN_REMOVE, IDB_BTN_REMOVE, IDB_BTN_REMOVE, IDB_BTN_REMOVE_D );
   m_bbUp.LoadBitmaps( IDB_BTN_UP, IDB_BTN_UP, IDB_BTN_UP, IDB_BTN_UP_D );
   m_bbDown.LoadBitmaps( IDB_BTN_DOWN, IDB_BTN_DOWN, IDB_BTN_DOWN, IDB_BTN_DOWN_D );
   m_bbAdd.LoadBitmaps( IDB_BTN_ADD, IDB_BTN_ADD, IDB_BTN_ADD, IDB_BTN_ADD_D );
   m_bbUpdate.LoadBitmaps( IDB_BTN_UPDATE, IDB_BTN_UPDATE, IDB_BTN_UPDATE, IDB_BTN_UPDATE_D );
   */

   m_bmRemove.LoadBitmap( IDB_BTN_REMOVE );
   m_bmUp.LoadBitmap( IDB_BTN_UP );
   m_bmDown.LoadBitmap( IDB_BTN_DOWN );
   m_bmAdd.LoadBitmap( IDB_BTN_ADD );
   m_bmUpdate.LoadBitmap( IDB_BTN_UPDATE );

   HBITMAP hbmOld;
   hbmOld = m_bbRemove.SetBitmap( (HBITMAP)m_bmRemove );
   m_bbUp.SetBitmap( (HBITMAP)m_bmUp );
   m_bbDown.SetBitmap( (HBITMAP)m_bmDown );
   m_bbAdd.SetBitmap( (HBITMAP)m_bmAdd );
   m_bbUpdate.SetBitmap( (HBITMAP)m_bmUpdate );

   //::SetWindowPos( ::GetDlgItem(m_hWnd, IDC_STATIC_GRP), HWND_BOTTOM , 0,0,0,0, 
	 // SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE );

   /*m_bbRemove.SizeToContent();
   m_bbUp.SizeToContent();
   m_bbDown.SizeToContent();
   m_bbAdd.SizeToContent();
   m_bbUpdate.SizeToContent();*/

   m_ComboClass.AddString( "Администратор" );
   m_ComboClass.AddString( "Когнитолог" );
   m_ComboClass.AddString( "Експерт" );
   m_ComboClass.AddString( "Пользователь (консультация)" );

   FillList( m_lstUsers );
   //m_lstUsers.SetItemState( 1, LVIS_SELECTED, LVIF_STATE );

   LV_ITEM lvIt; memset( &lvIt, 0, sizeof(LV_ITEM) );
   lvIt.mask = LVIF_STATE;
   lvIt.iItem = 0;
   lvIt.stateMask = LVIS_SELECTED;
   lvIt.state = LVIS_SELECTED;
   m_lstUsers.SetItemState( 0, &lvIt );	

   
   
   

   CWnd *pWnd = GetDlgItem( IDC_STATIC_GRP );
   pWnd->ModifyStyle( 0, WS_CLIPSIBLINGS, 0 );
	//DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0 );
   //InvalidateRect( NULL );

   HWND hw1 = ::GetDlgItem( m_hWnd, IDC_LIST_USERS );
   PostMessage( WM_NEXTDLGCTL, (WPARAM)hw1, 1 );	        

   return FALSE;
 }


void TPRopPageUsers::ChSelect( NMHDR *pNotifyStruct, LRESULT *result )
 {
   NM_LISTVIEW *pNM = (NM_LISTVIEW *)pNotifyStruct;

   if( pNM->iItem != -1 && (pNM->uChanged & LVIF_STATE) && 
	   (pNM->uNewState & LVIS_SELECTED)
	 )  LoadFromField( pNM->iItem );
  }

void TPRopPageUsers::LoadFromField( int iInd )
 {
/*
oBox m_ComboClass;
  CEdit m_edName, m_edFam, m_edPass;

  FName() const { return m_csFirstName; }
  LPCSTR SName() const { return m_csSecondName; }
  LPCSTR Password()
*/
   TUserLocal& rUs = *m_rlstUsers[ iInd ];

   m_edName.SetWindowText( rUs.FName() );
   m_edFam.SetWindowText( rUs.SName() );
   m_edPass.SetWindowText( rUs.Password() );

   TAcessLevel::TUserClass uClass;
   TAcessLevel::TUserSubclass usSubClass;
   rUs.GetRights( uClass, usSubClass );
   int iIt = 0;
   switch( uClass )
	{
	  case TAcessLevel::TU_Administrator:
	    iIt = 0;
		break;

	  case TAcessLevel::TU_Kognitolog:
	    iIt = 1;
		break;

	  case TAcessLevel::TU_Expert:
	    iIt = 2;
		break;

	  case TAcessLevel::TU_User:
	    iIt = 3;
		break;	   
	}

   m_ComboClass.SetCurSel( iIt );   
 }

/*
CComboBox m_ComboClass;
  CEdit m_edName, m_edFam, m_edPass;
  CBitmapButton m_bbRemove, m_bbUp, m_bbDown, m_bbAdd, m_bbUpdate;
  CListCtrl m_lstUsers;

  list<TUserLocal*>& m_rlstUsers; 
  bool m_bFlDirty;
  */

void TPRopPageUsers::DoDataExchange( CDataExchange* pDX )
 {
   CPropPageTooltips::DoDataExchange( pDX );
 }

/*
TUsersDialog(): TBWCCPropertySheet() {};
  TUsersDialog( TOptData& rOpt, LPCTSTR pszCaption, CWnd *pParentWnd = NULL, UINT iSelectPage = 0 );
    

  virtual ~TUsersDialog();
*/


TUsersDialog::~TUsersDialog()
 {
   vector<TUserLocal*>::iterator it( m_lstUsers.begin() );
   vector<TUserLocal*>::iterator itEnd( m_lstUsers.end() );

   for( ; it != itEnd; ++it )
	if( *it )
	 {
       delete *it;
	   *it = NULL;
	 }	
 }

void TUsersDialog::LoadUserList( CString& strPath )
 {
   LoadUserList_Internal( strPath, m_lstUsers, m_rOpt );
 }
 
void LoadUserList_Internal( CString& strPath, vector<TUserLocal*>& m_lstUsers, TMFOptions& m_rOpt )
 {
   TContextStatusMsgHold msgHold( "Загрузка списка пользователей..." );
   CWaitCursor wCursor;

    TCodingBAKMirrorFile f(
	   m_rOpt.d.m_bUseCoding, 
	   m_rOpt.d.codUserFile,
	   m_rOpt.d.m_cKey
	 );
        	
	CFileException except;

    if( !f.Open((LPCSTR)strPath, CFile::shareExclusive | CFile::modeRead, &except) )
	  AfxThrowFileException( except.m_cause, except.m_lOsError, (LPCSTR)strPath );	  	  
	   
	CArchive arch( &f, CArchive::load );

	TUserLocal  usrCfg;

	/*TAcessLevel::TUserClass uClass;
    TAcessLevel::TUserSubclass usSubClass;
    rUsr.GetRights( uClass, usSubClass );*/
	
	while( true )
	 {
	   TAutoMemoryRc<TUserLocal*> arcUs( new TUserLocal() );
	   try {
         arch >> *arcUs();      
		}
	   catch( CArchiveException* pE )
		{	  
		  if( pE->m_cause == CArchiveException::endOfFile )
		   {
             pE->Delete();  
			 break;
		   }
		  else throw pE;		   
		}	   	   

	   if( arcUs()->CheckCRC() == false ) throw TExceptCfgBadCRC();

	   m_lstUsers.push_back( arcUs() );
	   arcUs.ClearRc();
	 }	
 }

TUsersDialog::TUsersDialog( TMFOptions& rOpt, LPCTSTR pszCaption, CWnd *pParentWnd, UINT iSelectPage  ):
    TBWCCPropertySheet( pszCaption, pParentWnd, iSelectPage ),

    m_pagAcess( IDD_USER_ACESS ),
	m_pagErgo( IDD_USER_ERGO ),
	m_pagUser( m_lstUsers, IDD_USER_USER ),

	m_rOpt( rOpt ),
	m_Opt( rOpt )

 {   
   m_pagErgo.m_iSave = rOpt.d.m_bSaveMainFramePosOnExit;
   m_pagErgo.m_iRestore = rOpt.d.m_bRestoreMainFrame;

   m_pagAcess.m_iNumbPass = rOpt.d.m_iMaxRepass;
   m_pagAcess.m_iUse = rOpt.d.m_bUseCoding;
   m_pagAcess.m_iAlho = rOpt.d.codUserFile;

   LPSTR lps = m_pagAcess.m_strKey.GetBuffer( 9 );
   strncpy( lps, rOpt.d.m_cKey, 8 );
   lps[8] = 0;
   m_pagAcess.m_strKey.ReleaseBuffer();

   //m_pagAcess.

   AddPage( &m_pagAcess ); 
   AddPage( &m_pagErgo ); 
   AddPage( &m_pagUser );    

   CString strPath( CString(((TExpertApp*)AfxGetApp())->GetStartPath()) + CString(GLOBAL_USERS_FILENAME) );

   m_bIbit = false;
   try {
     LoadUserList( strPath );
	 m_bIbit = true;
	}
   catch( TExceptCfgBadCRC )
	{
      ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( m_hWnd, 
	     "Файл пользователей испорчен. Его можно стереть и перегрузить Visyal Expert.", 
		 "Ошибка", MB_OK|MB_ICONSTOP );
	  return;
	}
   catch( CArchiveException* pE )
	{	  	
	  if( pE->m_strFileName.IsEmpty() ) pE->m_strFileName = strPath;
      pE->ReportError( MB_OK | MB_ICONSTOP );
	  pE->Delete();  
	  return;
	}
   catch( CFileException* pE )
	{
      if( pE->m_strFileName.IsEmpty() ) pE->m_strFileName = strPath;
      pE->ReportError( MB_OK | MB_ICONSTOP );
	  pE->Delete();  
	  return;
	}
   catch( CException* pE )
	{	  
      pE->ReportError( MB_OK | MB_ICONSTOP );
	  pE->Delete();  
	  return;
	} 
 }
  
int TUsersDialog::OnCreate( LPCREATESTRUCT lpCreateStruct )
 {
   if( m_bIbit == false ) return -1;

   return TBWCCPropertySheet::OnCreate( lpCreateStruct );
 }

void TUsersDialog::UpdateOpt()
 {
   m_Opt.d.m_iMaxRepass = m_pagAcess.m_iNumbPass; 
   m_Opt.d.m_bUseCoding = m_pagAcess.m_iUse ? true:false;
   switch( m_pagAcess.m_iAlho )
	{
	  case 0:
	    m_Opt.d.codUserFile = TCodingBAKMirrorFile::ALHO_XOR;
	    break;

	  case 1:
	    m_Opt.d.codUserFile = TCodingBAKMirrorFile::ALHO_DES8;
	    break;
	}

   memset( m_Opt.d.m_cKey, 0, sizeof(m_Opt.d.m_cKey) );
   if( m_pagAcess.m_strKey.GetLength() ) 
	 strncpy( m_Opt.d.m_cKey, (LPCSTR)m_pagAcess.m_strKey, 8 );

   m_Opt.d.m_bSaveMainFramePosOnExit = m_pagErgo.m_iSave ? true:false;
   m_Opt.d.m_bRestoreMainFrame = m_pagErgo.m_iRestore ? true:false;   
 }

void TUsersDialog::SaveUserList()
 {
   CString strPath( CString(((TExpertApp*)AfxGetApp())->GetStartPath()) + CString(GLOBAL_USERS_FILENAME) );
   SaveUserList_Internal( strPath, m_lstUsers, m_Opt );
 }

void SaveUserList_Internal( CString& strPath, vector<TUserLocal*>& m_lstUsers, TMFOptions& m_Opt )
 {
   TCodingBAKMirrorFile f(
	   m_Opt.d.m_bUseCoding, 
	   m_Opt.d.codUserFile,
	   m_Opt.d.m_cKey
	   );   

   CFileException except;
   if( !f.Open((LPCSTR)strPath, CFile::shareExclusive | CFile::modeCreate | CFile::modeWrite, &except) )	      
	  AfxThrowFileException( except.m_cause, except.m_lOsError, (LPCSTR)strPath );	  
	   
   CArchive arch( &f, CArchive::store );

   vector<TUserLocal*>::iterator it( m_lstUsers.begin() );
   vector<TUserLocal*>::iterator itEnd( m_lstUsers.end() );

   for( int i = 0; it != itEnd; i++, ++it )
     arch << *(*it);	
 }

void TUsersDialog::SaveOpt()
 {
   TCodingBAKMirrorFile f;	  

   CString strPath( CString(((TExpertApp*)AfxGetApp())->GetStartPath()) + CString(GLOBAL_CONFIG_FILENAME) );

   CFileException except;
   if( !f.Open((LPCSTR)strPath, CFile::shareExclusive | CFile::modeCreate | CFile::modeWrite, &except) )	      
	  AfxThrowFileException( except.m_cause, except.m_lOsError, (LPCSTR)strPath );	  
	   
   CArchive arch( &f, CArchive::store );

   arch << m_Opt;	 
 }

bool TUsersDialog::IsCodingChanged()
 {
   return (m_Opt.d.m_bUseCoding != m_rOpt.d.m_bUseCoding) ||
    (m_Opt.d.m_bUseCoding && 
	 m_Opt.d.codUserFile != m_rOpt.d.codUserFile) ||
	 strncmp( m_Opt.d.m_cKey, m_rOpt.d.m_cKey, 8 );
 }

BOOL TUsersDialog::UpdateDataMy( CPropertyPage& rP, BOOL bFl )
 {
   if( !rP.m_hWnd ) return TRUE;
   return rP.UpdateData( bFl );
 }

void TUsersDialog::RecodeOneFile( LPSTR lpName )
 {   
   TAutoMemoryRc<TExpertSystem*> arcES( new TExpertSystem() );
   
   TAutoMemoryRc<TCodingBAKMirrorFile*> arcF = 
	    new TCodingBAKMirrorFile( m_rOpt.d.m_bUseCoding, 
	                              m_rOpt.d.codUserFile, 
							      m_rOpt.d.m_cKey 
							    );      
   
   arcF()->Open( lpName, CFile::modeRead|CFile::shareDenyWrite );
   CArchive arch( arcF(), CArchive::load );
   arch >> *arcES();

   arch.Close();
   arcF()->Close();

   TAutoMemoryRc<TCodingBAKMirrorFile*> arcF2 = 
	    new TCodingBAKMirrorFile( m_Opt.d.m_bUseCoding, 
	                              m_Opt.d.codUserFile, 
							      m_Opt.d.m_cKey 
							    );      

   arcF2()->Open( lpName, CFile::modeWrite|CFile::shareDenyWrite );
   CArchive arch2( arcF2(), CArchive::store );
   arch2 << *arcES();
 }

void TUsersDialog::MsgOpStop( LPSTR lpName )
 {
   CString str;
   str.Format( "Перекодирование дескрипторов ЕС прекращено на файле [%s]."
	           " Переименуйте BAK-файлы дескрипторов обратно.", lpName 
			 );
   MessageBox( str, "Ошибка", MB_OK|MB_ICONSTOP );
 }

void TUsersDialog::RecodeAllES()
 {   
   TContextStatusMsgHold msgHold( "Идёт перекодирование дескрипторов ЭС..." );
   CWaitCursor wCursor;

   CString strPath( ((TExpertApp*)AfxGetApp())->GetDescrPath() ); 
   strPath += "*.ves";

   _finddata_t fnd;
   memset( &fnd, 0, sizeof(_finddata_t) );
   fnd.attrib = _A_ARCH | _A_SUBDIR | _A_SYSTEM | _A_HIDDEN | _A_RDONLY | _A_NORMAL;

   long lHandl = _findfirst( (LPSTR)(LPCSTR)strPath, &fnd );
   if( lHandl == -1 ) return;	 
     
   int iRes;
   do {
	try {
       CString strPath0( ((TExpertApp*)AfxGetApp())->GetDescrPath() );  	   
	   strPath0 += fnd.name;
       RecodeOneFile( (LPSTR)(LPCSTR)strPath0 );  
	 }
	catch( CArchiveException* pE )
	{	  	
	  if( pE->m_strFileName.IsEmpty() ) pE->m_strFileName = fnd.name;
      //pE->ReportError( MB_OK | MB_ICONSTOP );
	  //pE->Delete();  	  
	  _findclose( lHandl );
	  MsgOpStop( fnd.name );
	  throw pE;
	}
    catch( CFileException* pE )
	 {
       if( pE->m_strFileName.IsEmpty() ) pE->m_strFileName = fnd.name;
       //pE->ReportError( MB_OK | MB_ICONSTOP );
	   //pE->Delete();  
	   _findclose( lHandl );
	   MsgOpStop( fnd.name );
	   throw pE;
	 }
    catch( CException* pE )
	 {	  
       //pE->ReportError( MB_OK | MB_ICONSTOP );
	   //pE->Delete();  	
	   _findclose( lHandl );
	   MsgOpStop( fnd.name );
	   throw pE;
	 }

      iRes = _findnext( lHandl, &fnd );	  

	} while( !iRes );

   DWORD dw = ::GetLastError();
   _findclose( lHandl );
   if( errno != ENOENT ) CFileException::ThrowOsError( dw, (LPSTR)(LPCSTR)strPath );
 }

void TUsersDialog::OnOK()
 { 
   if( !UpdateDataMy(m_pagAcess, TRUE) || !UpdateDataMy(m_pagErgo, TRUE) || !UpdateDataMy(m_pagUser, TRUE) )
	 return;

   UpdateOpt();

   TContextStatusMsgHold msgHold( "Сохранение настроек..." );
   CWaitCursor wCursor;

   bool bFlUsSave = false;
   try {
     if( IsCodingChanged() == true || m_pagUser.IsDirty() == true ) 
	   SaveUserList();
	 bFlUsSave = true;
	 if( IsCodingChanged() == true ) RecodeAllES();
	}
   catch( CArchiveException* pE )
	{	  	
	  if( pE->m_strFileName.IsEmpty() ) pE->m_strFileName = GLOBAL_USERS_FILENAME;
      pE->ReportError( MB_OK | MB_ICONSTOP );
	  pE->Delete();  	  
	}
   catch( CFileException* pE )
	{
      if( pE->m_strFileName.IsEmpty() ) pE->m_strFileName = GLOBAL_USERS_FILENAME;
      pE->ReportError( MB_OK | MB_ICONSTOP );
	  pE->Delete();  
	  //return;
	}
   catch( CException* pE )
	{	  
      pE->ReportError( MB_OK | MB_ICONSTOP );
	  pE->Delete();  	  
	}
   
   if( bFlUsSave == false )
	{
      m_rOpt.d.m_bSaveMainFramePosOnExit = false;
       ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( m_hWnd, 
	     "Изменения отменены. Для восстановления файлов конфигурации завершите пакет и переименуйте BAK-файлы", 
		 "Возможно испорчен файл описания пользователей", MB_OK | MB_ICONSTOP );
	  Default();
	  return;
	}

   bool bFl = false;
   try {
     SaveOpt();
	 bFl = true;
	}
   catch( CArchiveException* pE )
	{	  	
	  if( pE->m_strFileName.IsEmpty() ) pE->m_strFileName = GLOBAL_CONFIG_FILENAME;
      pE->ReportError( MB_OK | MB_ICONSTOP );
	  pE->Delete();  	  
	}
   catch( CFileException* pE )
	{
      if( pE->m_strFileName.IsEmpty() ) pE->m_strFileName = GLOBAL_CONFIG_FILENAME;
      pE->ReportError( MB_OK | MB_ICONSTOP );
	  pE->Delete();  
	  //return;
	}
   catch( CException* pE )
	{	  
      pE->ReportError( MB_OK | MB_ICONSTOP );
	  pE->Delete();  	  
	} 

   if( bFl == true ) m_rOpt = m_Opt;
   else
	{
	  m_rOpt.d.m_bSaveMainFramePosOnExit = false;
       ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( m_hWnd, 
	     "Изменения отменены. Для восстановления файлов конфигурации завершите пакет и переименуйте BAK-файлы", 
		 "Возможно испорчен файл конфигурации", MB_OK | MB_ICONSTOP );
	}

   Default();
 }
