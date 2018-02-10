#include "expdlg.hpp"
#include "vs.hpp"

#include <errno.h>
#include <direct.h>
#include <io.h>

BEGIN_MESSAGE_MAP( TFolderDlg, CDialog )
  ON_BN_CLICKED( ID_BTN_UP, OnUp )
  ON_BN_CLICKED( IDC_RAD_LIST, OnList )
  ON_BN_CLICKED( IDC_RAD_ICO, OnIco )
  ON_NOTIFY( NM_DBLCLK, IDC_LIST_DIR, OnDblClk_List )
  ON_CBN_SELENDOK( IDC_COMBO_DRIVE, OnCBNSelectOK )
  ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


void TDlgName::DoDataExchange( CDataExchange* pDX )
 {
   CDialog::DoDataExchange( pDX );

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

   if( pDX->m_bSaveAndValidate == TRUE ) m_rStr = m_str;  
 }

BOOL TDlgName::OnInitDialog()
 {
   CDialog::OnInitDialog();
   
   CenterWindow( m_mPar );

   return FALSE;
 }

void TDlgName::OnOK()
 {   
   CDialog::OnOK();
 }



void TFolderDlg::EnterList()
 {   
   if( m_bFlEmpty == true ) return;

   int iIt = m_lstFold.GetNextItem( -1, LVNI_SELECTED );
   if( iIt == -1 ) return;
      
   CString str( m_lstFold.GetItemText(iIt, 0) ); 
   
   if( m_Folder[m_Folder.GetLength()-1] != '\\' ) m_Folder += "\\";
   m_Folder += str + "\\";
   m_edPath.SetWindowText( m_Folder );

   try{
     FillList();
	}
   catch( CException *pE )
	{
      pE->ReportError();
	  pE->Delete();
	}
 }

void TFolderDlg::EnterEd()
 {
   CString str = m_Folder;
   m_edPath.GetWindowText( m_Folder );
   m_Folder.TrimLeft(); m_Folder.TrimRight();
   if( CheckPath(false) == false )
	{
	  ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( 
 	     GetParent()->m_hWnd, 
		 "Путь не существует", 
		 "Ошибка", MB_OK | MB_ICONSTOP );
      m_Folder = str;
	  return;
	}

   try{
     FillList();
	}
   catch( CException *pE )
	{
      pE->ReportError();
	  pE->Delete();
	}
 }

HBRUSH TFolderDlg::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
 {
   HBRUSH hbr = CDialog::OnCtlColor( pDC, pWnd, nCtlColor );

   if( nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_BTN )
	{
	  int id = pWnd->GetDlgCtrlID();
	  if( id == ID_TXT_FOLDER || id == IDOK )
	    pDC->SetTextColor( RGB(0,0,255) );
	}

   return hbr;
 }


void TFolderDlg::OnDblClk_List( NMHDR * pNotifyStruct, LRESULT * result )
 {
   EnterList();
   *result = TRUE;
 }

void TFolderDlg::OnCBNSelectOK()
 {
   int iSel = m_cbnDrv.GetCurSel();
   if( iSel == CB_ERR ) return;

   if( m_lstDrv[iSel]->m_cLetter + 'A' - 1 == toupper(m_Folder[0]) )
	 return;

   CString str;
   LPSTR lp = str.GetBuffer( 1024 );
   *lp = 0;

   if( _getdcwd(m_lstDrv[iSel]->m_cLetter, lp, 1023) )
	{
	  m_Folder = lp;
	  m_edPath.SetWindowText( m_Folder );
	  try{
        FillList();
	   }
      catch( CException *pE )
	   {
         pE->ReportError();
	     pE->Delete();
	   }
	}
   else
	{
	  CFileException exc( CFileException::OsErrorToException(::GetLastError()) );      
      exc.m_strFileName = CString(m_lstDrv[iSel]->m_cLetter+'A'-1) + ':';

	  exc.ReportError();
	}
   str.ReleaseBuffer();
 }

void TFolderDlg::OnUp()
 {
   char path_buffer[_MAX_PATH];
   char drive[_MAX_DRIVE];
   char dir[_MAX_DIR];
   char fname[_MAX_FNAME];
   char ext[_MAX_EXT];

   if( m_Folder[m_Folder.GetLength()-1] == '\\' &&
	   m_Folder[m_Folder.GetLength()-2] != ':'
	 )
     m_Folder = m_Folder.Left( m_Folder.GetLength() - 1 );

   _splitpath( m_Folder, drive, dir, fname, ext );      

   if( !strlen(fname) && !strlen(ext) && 
	   (!strlen(dir)||!strcmp(dir,"\\")) ) return;

   if( !strlen(fname) && !strlen(ext) )
     _makepath( path_buffer, drive, NULL, NULL, NULL );
   else
	 _makepath( path_buffer, drive, dir, NULL, NULL );
   int iL = strlen( path_buffer );   
   if( path_buffer[iL-1] != '\\' ) 
	 strcat( path_buffer, "\\" );

   m_Folder = path_buffer;
   m_edPath.SetWindowText( m_Folder );

   try{
     FillList();
	}
   catch( CException *pE )
	{
      pE->ReportError();
	  pE->Delete();
	}

   LV_ITEM lvIt;
   memset( &lvIt, 0, sizeof(LV_ITEM) );
   
   lvIt.mask = LVIF_STATE;
   lvIt.iItem = 0;
   lvIt.stateMask = LVIS_SELECTED;
   lvIt.state = LVIS_SELECTED;
   m_lstFold.SetItemState( 0, &lvIt );	
 }
void TFolderDlg::OnList()
 {
   //if( !m_radList.GetCheck() ) { m_radIco.SetCheck(1); OnIco(); }

   LV_ITEM lvIt;   
   memset( &lvIt, 0, sizeof(LV_ITEM) );
   lvIt.iItem = m_lstFold.GetNextItem( -1, LVNI_SELECTED );

   m_lstFold.ModifyStyle( LVS_ICON, LVS_SMALLICON );
   m_lstFold.InvalidateRect( NULL );
      
   lvIt.mask = LVIF_STATE;   
   lvIt.stateMask = LVIS_SELECTED;
   lvIt.state = LVIS_SELECTED;
   m_lstFold.SetItemState( lvIt.iItem, &lvIt );	
 }
void TFolderDlg::OnIco()
 {
   //if( !m_radIco.GetCheck() ) { m_radList.SetCheck(1); OnList(); }
	//m_radList, m_radIco;

   LV_ITEM lvIt;   
   memset( &lvIt, 0, sizeof(LV_ITEM) );
   lvIt.iItem = m_lstFold.GetNextItem( -1, LVNI_SELECTED );

   m_lstFold.ModifyStyle( LVS_SMALLICON, LVS_ICON );
   m_lstFold.InvalidateRect( NULL );

   lvIt.mask = LVIF_STATE;   
   lvIt.stateMask = LVIS_SELECTED;
   lvIt.state = LVIS_SELECTED;
   m_lstFold.SetItemState( lvIt.iItem, &lvIt );	
 }

bool TFolderDlg::CheckPath( bool bFlGetDefault )
 {  
   _finddata_t fnd;
   memset( &fnd, 0, sizeof(_finddata_t) );
   fnd.attrib = _A_ARCH | _A_SYSTEM | _A_HIDDEN | _A_RDONLY | _A_NORMAL;
   
   CString str( m_Folder );
   long lHandl;
   if( !m_Folder.IsEmpty() )
	{
      if( str[str.GetLength()-1] != '\\' ) str += "\\*.*";
      else str += "*.*";
	  lHandl = _findfirst( (LPSTR)(LPCSTR)str, &fnd );
	}
   else lHandl = -1;
   
   if( lHandl != -1 )  _findclose( lHandl );
   else 
	{
	  if( bFlGetDefault == false ) return false;

      LPSTR lp = m_Folder.GetBuffer( 1024 );
      _getcwd( lp, 1023 );
	  m_Folder.ReleaseBuffer();
	  return false;
	}   

   return true;
 }

BOOL TFolderDlg::OnInitDialog()
 {
   CDialog::OnInitDialog();
   CenterWindow( m_mPar );

   m_edPath.SubclassDlgItem( IDC_EDIT_PATH, this );
   m_lstFold.SubclassDlgItem( IDC_LIST_DIR, this );
   m_btnUp.SubclassDlgItem( ID_BTN_UP, this );
   m_radList.SubclassDlgItem( IDC_RAD_LIST, this );
   m_radIco.SubclassDlgItem( IDC_RAD_ICO, this );
   m_cbnDrv.SubclassDlgItem( IDC_COMBO_DRIVE, this );

   if( m_rOpt.d.m_bMode == true ) 
	{
	  m_radIco.SetCheck( 1 );
	  m_radList.SetCheck( 0 );
	}
   else 
	{
	  m_radList.SetCheck( 1 );
	  m_radIco.SetCheck( 0 );
	}

   if( m_bInitPath == true ) 
	{
	  LPSTR lp = m_Folder.GetBuffer( 1024 );
      _getcwd( lp, 1023 );
	  m_Folder.ReleaseBuffer();
	}

   CheckPath();
   m_edPath.SetWindowText( m_Folder );

   m_ilstNormal = new CImageList();
   m_ilstSmall = new CImageList();

   CBitmap bmNormal, bmSmall, bmNormalD, bmSmallD;
   bmNormal.LoadBitmap( IDB_FOLD_NORMAL );
   bmSmall.LoadBitmap( IDB_FOLD_SMALL );
   bmNormalD.LoadBitmap( IDB_FOLD_NORMAL_D );
   bmSmallD.LoadBitmap( IDB_FOLD_SMALL_D );
   

   m_bmDirUp.LoadBitmap( IDB_BTN_DIRUP );   
   m_btnUp.SetBitmap( (HBITMAP)m_bmDirUp );
	  
   BITMAP bm;
   ::GetObject( (HBITMAP)bmNormal, sizeof(bm), &bm );

   m_ilstNormal()->Create( bm.bmWidth, bm.bmHeight, 
	                ILC_MASK | ILC_COLOR4, 2, 1 );
   m_ilstNormal()->Add( &bmNormal, RGB(0,0,128) );	  
   m_ilstNormal()->Add( &bmNormalD, RGB(0,0,128) );	  
   m_ilstNormal()->SetBkColor( CLR_NONE );

   ::GetObject( (HBITMAP)bmSmall, sizeof(bm), &bm );
   m_iSmallBmpX = bm.bmWidth;
   m_ilstSmall()->Create( bm.bmWidth, bm.bmHeight, 
	                ILC_MASK | ILC_COLOR4, 2, 1 );
   m_ilstSmall()->Add( &bmSmall, RGB(0,0,128) );	  
   m_ilstSmall()->Add( &bmSmallD, RGB(0,0,128) );	  
   m_ilstSmall()->SetBkColor( CLR_NONE );


   m_lstFold.SetImageList( m_ilstNormal(), LVSIL_NORMAL );
   m_lstFold.SetImageList( m_ilstSmall(), LVSIL_SMALL );  
   m_lstFold.ModifyStyle( 0, m_rOpt.d.m_bMode == true ? LVS_ICON:LVS_SMALLICON );

   /*LV_COLUMN lv; 
   
   lv.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
   lv.fmt = LVCFMT_LEFT;
   lv.pszText = "Имя";
   lv.iSubItem = 0;
   lv.cx = 100;
   lv.cchTextMax = strlen( lv.pszText );
   m_lstFold.InsertColumn( 0, &lv );*/

	//DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0 );

   FillDrive();

   try{
     FillList();
	}
   catch( CException *pE )
	{
      pE->ReportError();
	  pE->Delete();
	}

   return FALSE;
 }

static LPSTR DrvType( UINT ui )
 {
   switch( ui )
	{
	  case DRIVE_UNKNOWN:
	    return "Unknown";
	  case DRIVE_REMOVABLE:
	    return "Removable";
	  case DRIVE_FIXED:
	    return "Fixed";
	  case DRIVE_REMOTE:
	    return "Remote";
	  case DRIVE_CDROM:
	    return "CDROM";
	  case DRIVE_RAMDISK:
	    return "Ramdisk";
	  default:
	    return "Unknown";
	};
 }

void TFolderDlg::FillDrive()
 {
   ClearDriveList();
   m_cbnDrv.ResetContent();   

   char cDr[] = "A:\\";
   for( int i = 'A'; i <= 'Z'; ++i, ++(*cDr) )
	{
      UINT ui = GetDriveType( cDr );
	  if( ui == DRIVE_UNKNOWN || ui == DRIVE_NO_ROOT_DIR ) continue;

	  TDrive *pDrv = new TDrive( i-'A'+1, ui );	  
	  m_lstDrv.push_back( pDrv );

	  char cBuf[100];
	  sprintf( cBuf, " %c:  - %s", char(i), DrvType(ui) );
	  m_cbnDrv.InsertString( -1, cBuf );	   
	}      
 }


TFolderDlg::~TFolderDlg()
 {
   ClearDriveList();
 }

void TFolderDlg::ClearDriveList()
 {
   vector<TDrive*>::iterator itSta( m_lstDrv.begin() );
   vector<TDrive*>::iterator itEnd( m_lstDrv.end() );

   for( ; itSta != itEnd; itSta++ )
	 delete *itSta;

   m_lstDrv.clear();
 }


void TFolderDlg::FillList()
 {
   m_bFlEmpty = true;
   m_lstFold.DeleteAllItems();

   TWndDrwLock lockLst( &m_lstFold );
   
   CString str( m_Folder );
   if( str[str.GetLength()-1] != '\\' ) str += "\\*.*";
   else str += "*.*";

   _finddata_t fnd;
   memset( &fnd, 0, sizeof(_finddata_t) );
   fnd.attrib = _A_ARCH | _A_SYSTEM | _A_HIDDEN | _A_RDONLY | _A_NORMAL;

   long lHandl = _findfirst( (LPSTR)(LPCSTR)str, &fnd );
   if( lHandl == -1 )
	 CFileException::ThrowOsError( ::GetLastError(), (LPSTR)(LPCSTR)str );	
     
   LV_ITEM lvIt;
   memset( &lvIt, 0, sizeof(LV_ITEM) );
   lvIt.mask = LVIF_TEXT | LVIF_IMAGE;   
   //lvIt.mask = LVIF_TEXT;
   lvIt.iImage = 0;
   //lvIt.iSubItem = 0;          
   lvIt.iItem = 0;	        

   //CWindowDC dc( &m_lstFold );
   //int iCx = m_iSmallBmpX;

   int iRes;
   do {
      if( (fnd.attrib & _A_SUBDIR) && 
	      strcmp(fnd.name,".") && strcmp(fnd.name,"..") ) 
	   {
         lvIt.pszText = fnd.name;         
	     m_lstFold.InsertItem( &lvIt );
		 //CSize sz; dc.GetTextExtent( fnd.name, strlen(fnd.name) );
		 //iCx = max( iCx, sz.cx );
	     //lvIt.iItem++;
	   }

      iRes = _findnext( lHandl, &fnd );	  	  
	} while( !iRes );

   DWORD dw = ::GetLastError();
   _findclose( lHandl );
   if( errno != ENOENT ) CFileException::ThrowOsError( dw, (LPSTR)(LPCSTR)str );   

   if( !m_lstFold.GetItemCount() )
	{
      m_bFlEmpty = true;
	  lvIt.pszText = "<Пусто>"; 
	  lvIt.iImage = 1;
	  lvIt.iItem = 0;
	  m_lstFold.InsertItem( &lvIt );	  
	}
   else
	{
	  m_bFlEmpty = false;
      lvIt.mask = LVIF_STATE;
      lvIt.iItem = 0;
      lvIt.stateMask = LVIS_SELECTED;
      lvIt.state = LVIS_SELECTED;
      m_lstFold.SetItemState( 0, &lvIt );	
	}

   if( !m_Folder.IsEmpty() )
	{
	  CString csPref( " " + CString(toupper(m_Folder[0])) + ":" );
      int iFnd = m_cbnDrv.FindString( -1, (LPSTR)(LPCSTR)csPref );
	  if( iFnd != CB_ERR ) m_cbnDrv.SetCurSel( iFnd );
	}
   
   if( m_bFlEmpty == false && m_radList.GetCheck() )
	{
      m_lstFold.ModifyStyle( LVS_SMALLICON, LVS_ICON );
      //m_lstFold.InvalidateRect( NULL );
	  m_lstFold.ModifyStyle( LVS_ICON, LVS_SMALLICON );
	  //m_lstFold.InvalidateRect( NULL );
	  //m_lstFold.SetColumnWidth( -1, iCx + 2 );
	}
 }

void TFolderDlg::DoDataExchange( CDataExchange* pDX )
 {
   CDialog::DoDataExchange( pDX );

   DDX_Text( pDX, IDC_EDIT_PATH, m_Folder );
   DDV_MaxChars( pDX, m_Folder, 260 );   

   if( pDX->m_bSaveAndValidate == TRUE && m_Folder.IsEmpty() )
	{
	  ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( 
 	     GetParent()->m_hWnd, 
		 "Нужно ввести хотябы один символ", 
		 "Ошибка", MB_OK | MB_ICONEXCLAMATION );
	  
	  HWND hw1 = ::GetDlgItem( m_hWnd, IDC_EDIT_PATH );
	  PostMessage( WM_NEXTDLGCTL, (WPARAM)hw1, 1 );	        

	  pDX->Fail();	  	  	
	}
   
   if( pDX->m_bSaveAndValidate == TRUE ) 
	{	  
	  _finddata_t fnd;
      memset( &fnd, 0, sizeof(_finddata_t) );
      fnd.attrib = _A_ARCH | _A_SYSTEM | _A_HIDDEN | _A_RDONLY | _A_NORMAL;
   
	  CString str( m_Folder );
	  if( str[str.GetLength()-1] != '\\' ) str += "\\*.*";
	  else str += "*.*";
      long lHandl = _findfirst( (LPSTR)(LPCSTR)str, &fnd );
      if( lHandl != -1 )  _findclose( lHandl );
	  else pDX->Fail();		

	  m_rFolder = m_Folder;  
	  if( m_rFolder[m_rFolder.GetLength()-1] != '\\' ) m_rFolder += "\\";

	  m_rOpt.d.m_bMode = m_radIco.GetCheck() ? true:false;      
	}
 }

void TFolderDlg::OnOK()
 {   
   HWND hw = ::GetFocus();
   if( hw == m_lstFold.m_hWnd ) EnterList();
   else if( hw == m_edPath.m_hWnd ) EnterEd();
   else
     CDialog::OnOK(); 
 }

