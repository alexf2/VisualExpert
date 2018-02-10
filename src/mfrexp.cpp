#include "mfrexp.hpp"

#include "vs.rh"
#include "vs.hpp"
#include "autorc.hpp"

#include <afxcmn.h>
#include <afxwin.h>
#include <typeinfo.h>

#include "rdlg.hpp"
#include "dlgacess.hpp"
#include "userdlg.hpp"

#include "viewexp.hpp"

#include "globlmsg.hpp"

#include "about.hpp"

#include <strstream>
#include <iomanip>

#include "TCalcDialog.h"
#include "Timer.h"


#include <vector>
using namespace std;


IMPLEMENT_DYNCREATE( TMainFrame, CMDIFrameWnd )


BEGIN_MESSAGE_MAP( TMainFrame, CMDIFrameWnd )
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()

	ON_UPDATE_COMMAND_UI( AFX_IDW_TOOLBAR, CMDIFrameWnd::OnUpdateControlBarMenu )
	ON_COMMAND_EX( AFX_IDW_TOOLBAR, CMDIFrameWnd::OnBarCheck )

	ON_COMMAND( ID_HELP_FINDER, CMDIFrameWnd::OnHelpFinder )
	ON_COMMAND( ID_HELP, CMDIFrameWnd::OnHelp )
	ON_COMMAND( ID_CONTEXT_HELP, CMDIFrameWnd::OnContextHelp )
	ON_COMMAND( ID_DEFAULT_HELP, CMDIFrameWnd::OnHelpFinder )
	
	ON_UPDATE_COMMAND_UI( ID_USERS, OnUpdateUsers )
	ON_UPDATE_COMMAND_UI( ID_ACESS, OnUpdateAcess )
	//ON_UPDATE_COMMAND_UI( ID_GLOBAL_OPT, OnUpdateUsers )
	ON_UPDATE_COMMAND_UI( ID_CLOSE_ALL, OnUserWnd )
	//ON_UPDATE_COMMAND_UI( ID_APP_WINDOW, OnUserWnd )
	ON_UPDATE_COMMAND_UI( ID_WINDOW_NEXT, OnUserWnd )
	ON_UPDATE_COMMAND_UI( ID_WINDOW_PRIV, OnUserWnd )
	ON_UPDATE_COMMAND_UI( ID_APP_WINDOW, OnUserWnd )	
	

	ON_UPDATE_COMMAND_UI( ID_ACESS_CURRENT, OnAlways )	
	

	ON_UPDATE_COMMAND_UI( AFX_IDM_FIRST_MDICHILD, OnUserWnd )	

	ON_COMMAND( ID_CM_ABOUT, OnAbout )
	ON_COMMAND( ID_ACESS, OnAcess )
	ON_COMMAND( ID_USERS, OnUsers )
	ON_COMMAND( ID_ACESS_CURRENT, OnAcessCurrent )
	ON_COMMAND( ID_APP_WINDOW, OnAppWindow )
	ON_COMMAND( ID_CLOSE_ALL, OnCloseAll )
	ON_COMMAND( ID_WINDOW_NEXT, OnNext )
	ON_COMMAND( ID_WINDOW_PRIV, OnPriv )

		
	ON_REGISTERED_MESSAGE( TMainFrame::m_iMsgCalc, OnCalc )
	ON_REGISTERED_MESSAGE( TMainFrame::m_iMsgClock, OnClock )

	ON_WM_QUERYNEWPALETTE()
    ON_WM_PALETTECHANGED()

	//ON_COMMAND( ID_GLOBAL_OPT, OnGlobalOpt )	
    	
	//ID_VIEW_TOOLBAR

	//ON_UPDATE_COMMAND_UI(ID_INDICATOR_LINE, OnUpdateLineNumber)
	//ON_UPDATE_COMMAND_UI(ID_INDICATOR_CLOCK, OnUpdateClock)
END_MESSAGE_MAP()

void TMainFrame::OnPaletteChanged( CWnd* pFocusWnd )
 {   
   /*BOOL bRes = 0;   
   SendMessageToDescendants( globl_iMsgPalChanged, (WPARAM)&bRes, (LPARAM)pFocusWnd, TRUE, FALSE );
   if( !bRes )
	  CMDIFrameWnd::OnPaletteChanged( pFocusWnd );*/

   if( pFocusWnd->GetSafeHwnd() == GetSafeHwnd() || IsChild(pFocusWnd) )
     CMDIFrameWnd::OnPaletteChanged( pFocusWnd );
   else
     SendMessageToDescendants( globl_iMsgQNewPal, 0, 0, TRUE, FALSE );
 }

BOOL TMainFrame::OnQueryNewPalette()
 {
   BOOL bRes = 0;
   SendMessageToDescendants( globl_iMsgQNewPal, 0, (LPARAM)&bRes, TRUE, FALSE );

   return bRes;
 }


//m_arcTimeAlert - содержит TClickBmpWithClock
void ExecAlert( TAlertLebel* pAlertLbl, void* pExtraData )
 {
   //TAlertLebel::TA_Types ta = pAlertLbl->GetType();
   ::MessageBox( ((CWnd*)pExtraData)->m_hWnd, pAlertLbl->GetMsg(), 
	 "Событие", MB_OK|MB_ICONASTERISK );
 }


UINT TMainFrame::m_iMsgCalc = 0, TMainFrame::m_iMsgClock = 0;

CArchive& AFXAPI operator>>( CArchive& rA, TMFOptions& rOpt )
 {   
   rA.Read( &rOpt.d, sizeof(TOptData) );
   rA >> rOpt.dwCRC;
   rA.Read( &rOpt.s, sizeof(TScreenData) );
   	     
   return rA;
 }

DWORD TMFOptions::CalcCRC()
 {
   DWORD dwKeep = dwCRC; dwCRC = 0;

   DWORD dwRes = 0;
   unsigned char *lp = (unsigned char *)&d;
   for( int i = sizeof(TOptData); i; --i, lp++ )
	 dwRes += *lp;

   dwCRC = dwKeep;
   return dwRes;
 }


void TMainFrame::OnUpdateUsers( CCmdUI* pCmdUI )
 {
   TAcessLevel::TUserClass uClass;
   TAcessLevel::TUserSubclass usSubClass;
   m_protAcessLvl.GetRights( uClass, usSubClass );

   pCmdUI->Enable( uClass == TAcessLevel::TU_Administrator );   
 }

void TMainFrame::OnUserWnd( CCmdUI* pCmdUI )
 {
   pCmdUI->Enable( MDIGetActive() != NULL );
 }

void TMainFrame::OnAlways( CCmdUI* pCmdUI )
 {
   pCmdUI->Enable( TRUE );
 }

TMainFrame::TMainFrame()
 {
   m_parrMenuBmp = NULL;
   m_iCountRePass = 0;
   m_bFlLoadingFrame = false;
   m_bFlShow = false;
   m_iCurAcessSheetIndex = 0;

   m_iMsgCalc = RegisterWindowMessage( "WM_VE_CALC" );
   m_iMsgClock = RegisterWindowMessage( "WM_VE_CLOCK" );
   globl_iMsgQNewPal = RegisterWindowMessage( "WM_GL_QUERYNEWPAL" );
   globl_iMsgPalChanged = RegisterWindowMessage( "WM_GL_PALCHANGED" );
   globl_iMsgSwitchViews = RegisterWindowMessage( "WM_VB_SWITCHVIEWS" );
 }

TMainFrame::~TMainFrame()
 {
   if( m_parrMenuBmp )
	{
      delete[] m_parrMenuBmp;
	  m_parrMenuBmp = NULL;
	}
 }

void TMainFrame::SetPasswordFrom( TPRopPageUser& rPage, TProtectedObj& rProtObj )
 {
   TAutoMemoryRc<TUser*> arcUser( rProtObj.GetFirstUser() );
   if( !arcUser ) 
	{
	  arcUser = new TUser( rPage.m_csFirst, rPage.m_csSecond );
	  rProtObj + *arcUser();
	}
   arcUser()->SetPassword( rPage.m_csPass );

   arcUser.ClearRc();
 }

void TMainFrame::SetPasswordFrom( TPRopPageAdm& rPage, TProtectedObj& rProtObj )
 {
   TAutoMemoryRc<TUser*> arcUser( rProtObj.GetFirstUser() );
   if( !arcUser ) 
	{
	  arcUser = new TUser( NULL, NULL );
	  rProtObj + *arcUser();
	}
   arcUser()->SetPassword( rPage.m_csPass );

   arcUser.ClearRc();
 }


void TMainFrame::SetNameFrom( TPRopPageUser& rPage, TProtectedObj& rProtObj )
 {
   TAutoMemoryRc<TUser*> arcUser( new TUser(rPage.m_csFirst, rPage.m_csSecond) );
   arcUser()->SetPassword( rPage.m_csPass );

   rProtObj + *arcUser();

   arcUser.ClearRc();
 }


void TMainFrame::OnUsers()
 {
   TAutoMemoryRc<TUsersDialog*> arc( new TUsersDialog(m_optOptions, "Администрирование", this) );
   arc()->DoModal();   
 }

void TMainFrame::OnAcess()
 {
   //const int iMaxPass = 3;
   if( m_iCountRePass > m_optOptions.d.m_iMaxRepass )
	{
      ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( m_hWnd, 
	     "Изменение уровня привилегий отвергуто - превышено число попыток", 
		 "Уровент доступа", MB_OK | MB_ICONSTOP );
	  return;
	}

   TAcessChangeDlg dlgAcessCh( "Изменение уровня доступа", this, m_iCurAcessSheetIndex );
   //TAcessChangeDlg( LPCTSTR pszCaption, CWnd *pParentWnd = NULL, UINT iSelectPage = 0 );

   if( dlgAcessCh.DoModal() != IDOK ) return;
   m_iCurAcessSheetIndex = dlgAcessCh.m_iActiveIndex;
	
   

   TAcessLevel::TUserClass uClass = TAcessLevel::TU_User;
   TAcessLevel::TUserSubclass usSubClass = TAcessLevel::TUS_Anonime;   

   TProtectedObj protObj( uClass, usSubClass );
   //void TMainFrame::SetNameFrom( TPRopPageUser& rPage, TProtectedObj& rProtObj )
   //TProtectedObj( TUserClass uClass, TUserSubclass usSubClass, TProtectedObj *pPar = NULL, bool bInchert = true ):

   switch( dlgAcessCh.m_iActiveIndex )
	{
      case 0:
	    uClass = TAcessLevel::TU_User;
		if( dlgAcessCh.m_pagUser.m_iBtnAnonim == 0 ) 
		 {
		   usSubClass = TAcessLevel::TUS_Anonime;
		   SetPasswordFrom( dlgAcessCh.m_pagUser, protObj );
		 }
		else
		 {
           SetNameFrom( dlgAcessCh.m_pagUser, protObj );
		   usSubClass = TAcessLevel::TUS_Named;
		 }		
		 
	    break;

	  case 1:
	    uClass = TAcessLevel::TU_Expert;
		if( dlgAcessCh.m_pagExp.m_iBtnAnonim == 0 ) 
		 {
		   usSubClass = TAcessLevel::TUS_Anonime;
		   SetPasswordFrom( dlgAcessCh.m_pagExp, protObj );
		 }
		else
		 {
           SetNameFrom( dlgAcessCh.m_pagExp, protObj );
		   usSubClass = TAcessLevel::TUS_Named;
		 }
		

	    break;

	  case 2:
	    uClass = TAcessLevel::TU_Kognitolog;
		if( dlgAcessCh.m_pagKognit.m_iBtnAnonim == 0 ) 
		 {
		   usSubClass = TAcessLevel::TUS_Anonime;
		   SetPasswordFrom( dlgAcessCh.m_pagKognit, protObj );
		 }
		else
		 {
           SetNameFrom( dlgAcessCh.m_pagKognit, protObj );
		   usSubClass = TAcessLevel::TUS_Named;
		 }
		
	    break;

	  case 3:
	    uClass = TAcessLevel::TU_Administrator;
		usSubClass = TAcessLevel::TUS_Administrator;
		SetPasswordFrom( dlgAcessCh.m_pagAdm, protObj );
	    break;
	};

   

/*enum TUserClass { TU_Administrator=0, TU_Kognitolog=1, TU_Expert=2, TU_User=3 };
  enum TUserSubclass { TUS_Administrator=0, TUS_Named=1, TUS_Anonime=2 };
*/
   TUser usr;

   protObj.SetRights( uClass, usSubClass );	
   if( protObj.GetFirstUser() ) 
	{
	  protObj.GetFirstUser()->SetRights( uClass, usSubClass );	   
	  usr = *protObj.GetFirstUser();
	}
   else usr.SetRights( uClass, usSubClass );
   
   TUserLocal *pFoudedUs;
   int iRes = FindUser( usr, &pFoudedUs );
   TAutoMemoryRc<TUserLocal*> arcFoudedUs( pFoudedUs );

   if( !iRes ) { m_iCountRePass = 0; return; } //отказ 
   if( iRes == 1 ) //повторить ввод пароля
	{
	  m_iCountRePass++;
      PostMessage( WM_COMMAND, MAKELONG(ID_ACESS,0), 0 );
	  return;
	}
   
   m_iCountRePass = 0;

   SaveUserCfg();

   m_protAcessLvl.ClearNamed();
   m_protAcessLvl.SetRights( uClass, usSubClass );	

   m_protAcessLvl + *arcFoudedUs();
   arcFoudedUs.ClearRc();

   /*if( usSubClass == TAcessLevel::TUS_Named && protObj.GetFirstUser() ) 
	{
	  TAutoMemoryRc<TUser*> usr = new TUser( *protObj.GetFirstUser() );
	  m_protAcessLvl + *usr();
	  usr.ClearRc();
	}*/
 }


void TMainFrame::OnAcessCurrent()
 {
   static const char cStrF[] = 
	"Класс пользователя: [%s]\r\n"
	"Тип пользователя: [%s]\r\n"
	"Имя: [%s]\r\n"
	"Фамилия: [%s]";

   TExpertApp *pApp = (TExpertApp *)AfxGetApp();

   TAcessLevel::TUserClass uClass;
   TAcessLevel::TUserSubclass usSubClass;
   m_protAcessLvl.GetRights( uClass, usSubClass );

   CString str;   
   LPSTR lp = str.GetBuffer( 1024 * 2 );
   if( uClass == TAcessLevel::TU_Administrator )	
     sprintf( lp, cStrF, m_protAcessLvl.GetStrClass(),
	                    "--", "--", "--" );	   	
   else 
	{
	  if( usSubClass == TAcessLevel::TUS_Named )	   
	    sprintf( lp, cStrF, m_protAcessLvl.GetStrClass(),	                    
		  				   m_protAcessLvl.GetStrSubClass(), 
						   m_protAcessLvl.GetFirstUser()->FName(),
						   m_protAcessLvl.GetFirstUser()->SName()
						 );	   	
	  else
	    sprintf( lp, cStrF, m_protAcessLvl.GetStrClass(),	                    
		  				   m_protAcessLvl.GetStrSubClass(), 
						   "--",
						   "--"
						 );	   	
	}

   str.ReleaseBuffer();

   CAcessDialog dlg( IDD_DIALOG_CUR_ACESS, this, (LPCSTR)str );
   dlg.DoModal();
   //MessageBox( (LPCSTR)str, "Текущий уровень доступа", MB_OK | MB_ICONINFORMATION );
	//enum TUserClass { TU_Administrator=0, TU_Kognitolog=1, TU_Expert=2, TU_User=3 };
 }

void TMainFrame::OnUpdateAcess(  CCmdUI* pCmdUI  )
 {
   pCmdUI->Enable( MDIGetActive() == NULL );
 }

void TMainFrame::OnAbout()
 {
   try {
     AboutShow( this, IDB_DIB_ABOUT256 );
	}
   catch( CException *pE )
	{	  
	  pE->ReportError( MB_OK | MB_ICONSTOP );
	  pE->Delete();  	
	  return;
	}   
 }

void TMainFrame::OnAppWindow()
 {
   WindowProc( WM_COMMAND, 0xFF09, 0 );
 }

void TMainFrame::OnNext()
 {
   MDINext();
 }

LONG TMainFrame::OnCalc( UINT, LONG )
 {
    TCalcDialog* dlg = new TCalcDialog( this ); //self destructable dialog

    dlg->m_iPrec = GetCurUser()->m_iCalcPrec;
	dlg->m_iShowMode = GetCurUser()->m_iCalcShowDigitMode;
	
	dlg->Create( IDD_CALC );		
	
	return 0;   
 }

//m_arcTimeAlert - содержит TClickBmpWithClock
//Здесь редактировать список событий объекта TClickBmpWithClock
LONG TMainFrame::OnClock( UINT, LONG )
 {
   CTimerDialog dlg( this, this ); 

   m_arcTimeAlert()->Lock( true );
   m_arcTimeAlert()->EnableAlert( false );
   dlg.DoModal();
   m_arcTimeAlert()->EnableAlert( true );
   m_arcTimeAlert()->Lock( false );

   //dlg->Create( IDD_TIMER );	   

   return 0;
 }

void TMainFrame::OnPriv()
 {
   CMDIChildWnd* pCh = MDIGetActive();
   if( !pCh ) return;

   //pCh->MDIDestroy(); return;

   HWND hwPriv = ::GetWindow( pCh->m_hWnd, GW_HWNDLAST );
	//pCh->m_hWnd;
   HWND hw = ::GetWindow( hwPriv, GW_HWNDNEXT );
   while( hw != pCh->m_hWnd && hw )
	{
      hwPriv = hw;
	  hw = ::GetWindow( hwPriv, GW_HWNDNEXT );
	}

   if( !hwPriv || hwPriv == pCh->m_hWnd ) return;

   CWnd *pWnd = CWnd::FromHandle( hwPriv );
   MDIActivate( pWnd );
 }

void TMainFrame::OnCloseAll()
 {
   CWinApp *pApp = AfxGetApp();

   POSITION ps = pApp->GetFirstDocTemplatePosition();
   CDocTemplate *pDocTmpl;
   while( ps )
	{
      pDocTmpl = pApp->GetNextDocTemplate( ps );

      POSITION pos = pDocTmpl->GetFirstDocPosition();
	  CDocument *pDoc;
	  while( pos ) 
	   {
         pDoc = pDocTmpl->GetNextDoc( pos );
	     if( pDoc->CanCloseFrame(this) == FALSE ) return;
	   }
	}

   ps = pApp->GetFirstDocTemplatePosition();   
   while( ps )
	{
      pDocTmpl = pApp->GetNextDocTemplate( ps );

      POSITION pos = pDocTmpl->GetFirstDocPosition();
	  CDocument *pDoc;
	  while( pos ) 
	   {
         pDoc = pDocTmpl->GetNextDoc( pos );
	     pDoc->OnCloseDocument();
	   }
	}   
 }

static char* const lpButtonsText[] =
 {
    "Доступ",

	 "",
	 
	"Новая ЭС/~N~",
	"Открыть/~O~",
	"Сохранить/~S~",
	"Закрыть/~C~",

	"",
	
    "Выход/~E~",
	"",
	"Реквизиты",	

    "Окна ЭС/~W~",
	"Справка/~H~",	
 };

struct TBtnDescr
 {
   UINT uCmd;
   bool bRight;
 };

static TBtnDescr buttons[] =
 {	
  { ID_ACESS, false },
	    
		{ ID_SEPARATOR, false },

  { ID_FILE_NEW, false },
  { ID_FILE_OPEN, false },
  { ID_FILE_SAVE_AS, false },
  { ID_FILE_CLOSE, false },

	{ ID_SEPARATOR,	false },

  { ID_APP_EXIT, false },  

	{ ID_SEPARATOR,	false },

  { ID_CM_ABOUT, false },

  { ID_APP_WINDOW, true },  	    	
  { ID_CONTEXT_HELP, true }
 };

static UINT buttonsRC[] =
 {
   IDB_ACESS,
	0,
   IDB_NEW,
   IDB_OPEN,
   IDB_SAVE,
   IDB_CLOSE,
    0,
   IDB_EXIT,
    0,
   IDB_ABOUT,

   IDB_WINDOW,
   IDB_HELP
 };

struct TMenuBmp
 {
   UINT uiCmd;
   UINT uiBmpU, uiBmpC;
 };

static TMenuBmp menuBitmaps[] =
 {  
  { ID_FILE_NEW, IDB_M_NEW, IDB_M_NEW },
  { ID_FILE_OPEN, IDB_M_OPEN, IDB_M_OPEN },
  { ID_FILE_SAVE_AS, IDB_M_SAVE, IDB_M_SAVE },

  { 57652, IDB_M_VERT, IDB_M_VERT },
  { 57651, IDB_M_HORIZ, IDB_M_HORIZ },

  { ID_WINDOW_NEXT, IDB_M_NEXT, IDB_M_NEXT },
  { ID_WINDOW_PRIV, IDB_M_PRIV, IDB_M_PRIV }
 };


static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator	    
	ID_CALC,	
	ID_ALERT
};


BOOL TMainFrame::PreCreateWindow( CREATESTRUCT& cs )
 {   
   cs.style |= WS_HSCROLL | WS_HSCROLL;
   //cs.dwExStyle |= 

    //if( !cs.hMenu ) return CMDIFrameWnd::PreCreateWindow( cs );

    BOOL bRes = CMDIFrameWnd::PreCreateWindow( cs );
    if( !bRes ) return bRes;
    
   return bRes;
 }


void TMainFrame::OnClose()
 {    
	CMDIChildWnd* pMDIChild = MDIGetActive();
	if( pMDIChild != NULL )
		pMDIChild = (CMDIChildWnd*)pMDIChild->GetWindow( GW_HWNDLAST );
	while( pMDIChild != NULL )
	 {
	    if( pMDIChild->GetActiveView() &&
	       ((TBaseView*)(pMDIChild->GetActiveView()))->CanClose() == false
	      ) return;	 
	
		pMDIChild = (CMDIChildWnd*)pMDIChild->GetWindow( GW_HWNDPREV );
	 } 
	

	if( m_optOptions.d.m_bSaveMainFramePosOnExit == true )
	 {  
	   try{
		 KeepScreenConfig();
		 SaveGlobalConfig();
		}
	   catch( CArchiveException* pE )
		{	  		  
		  if( pE->m_strFileName.IsEmpty() ) pE->m_strFileName = GLOBAL_CONFIG_FILENAME;
		  pE->ReportError( MB_OK | MB_ICONEXCLAMATION );
		  pE->Delete();  
		}
	   catch( CException* pE )
		{	  
		  pE->ReportError( MB_OK | MB_ICONEXCLAMATION );
		  pE->Delete();  
		} 	   	   
	 }
 
	CMDIFrameWnd::OnClose();
 }


void TMainFrame::OnDestroy()
 {
   //m_arcTool()->Detach();

   SaveUserCfg();
   
   if( m_protAcessLvl.m_lstUsers.size() ) 
	 GetCurUser()->DestroyGDI();

   CMDIFrameWnd::OnDestroy();
 }

class TUsHold
 {
public:
  TUsHold( vector<TUserLocal*>& rV ):
    m_rV( rV )
	 {
	   m_iNoEr = -1;
	 }

  ~TUsHold();

  void NoErase( int i ) { m_iNoEr = i; }

private:
  vector<TUserLocal*>& m_rV;
  int m_iNoEr;
 };

TUsHold::~TUsHold()
 {
   vector<TUserLocal*>::iterator itSta( m_rV.begin() );
   vector<TUserLocal*>::iterator itEnd( m_rV.end() );
   int i = 0;
   for( ; itSta != itEnd; itSta++, i++ )
	 if( i!= m_iNoEr ) delete *itSta;

   m_rV.clear();
 }

void TMainFrame::SaveUserCfg()
 {
   if( !m_protAcessLvl.m_lstUsers.size() ) return;

   TUserLocal* pUs = GetCurUser();
   if( pUs->m_bModifiedData == false ) return;

   pUs->m_bModifiedData = false;

   vector<TUserLocal*> m_vecUs; 
   TUsHold hld( m_vecUs );
   

   CString strPath( CString(((TExpertApp*)AfxGetApp())->GetStartPath()) + CString(GLOBAL_USERS_FILENAME) );
   
   try {
     LoadUserList_Internal( strPath, m_vecUs, m_optOptions );
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

   vector<TUserLocal*>::iterator itSta( m_vecUs.begin() );
   vector<TUserLocal*>::iterator itEnd( m_vecUs.end() );
   int i = 0;
   bool bFl = false;
   for( ; itSta != itEnd; itSta++, i++ )
	 if( pUs->Cmp( **itSta ) == true )
      {
	    bFl = true;
        delete *itSta; 
		*itSta = pUs; 
		//m_protAcessLvl.m_lstUsers.erase( m_protAcessLvl.m_lstUsers.begin() );
		hld.NoErase( i );

		break;
	  }
	   

   try {     
	  SaveUserList_Internal( strPath, m_vecUs, m_optOptions );
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
 }

void TMainFrame::SetupMenu( HMENU hMenu )
 {   	
   /*DWORD dw = GetMenuCheckMarkDimensions();
   UINT lo = LOWORD(dw);
   UINT hi = HIWORD(dw);*/

   if( !hMenu ) return;
   CMenu mnu; mnu.Attach( hMenu );

   if( !m_parrMenuBmp )	
      m_parrMenuBmp = new CBitmap[ sizeof(menuBitmaps)/sizeof(TMenuBmp) ];	

   for( int i = 0; i < sizeof(menuBitmaps)/sizeof(TMenuBmp); i++ )
	{	 
	  if( !(HBITMAP)m_parrMenuBmp[i] ) 
	    m_parrMenuBmp[i].LoadBitmap( menuBitmaps[i].uiBmpU );
	 
	  BOOL bFl = mnu.SetMenuItemBitmaps( menuBitmaps[i].uiCmd, MF_BYCOMMAND, &(m_parrMenuBmp[i]), &(m_parrMenuBmp[i]) );       
	}	

   mnu.Detach();	   

 }


int TMainFrame::OnCreate( LPCREATESTRUCT lpCreateStruct )
 {
    if( !LoadConfigOnStart() ) return -1;	   

    SetupMenu( lpCreateStruct->hMenu );		

	CWinApp *pApp = AfxGetApp();
	POSITION ps = pApp->GetFirstDocTemplatePosition();
    CMultiDocTemplate *pDocTmpl;
    while( ps )
	 {
       pDocTmpl = (CMultiDocTemplate *)pApp->GetNextDocTemplate( ps );
       SetupMenu( pDocTmpl->m_hMenuShared );
	 }

	if( CMDIFrameWnd::OnCreate(lpCreateStruct) == -1 )
	  return -1;	


	/*if( !m_wndToolBar.Create(this, WS_CHILD | WS_VISIBLE |
			CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY,  AFX_IDW_TOOLBAR ) ||
		!m_wndToolBar.LoadBitmap(IDR_MAINFRAME) 		
	  )
	{
		//AfxMessageBox( "Failed to create toolbar\n", MB_OK | MB_ICONSTOP );
	    ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( m_hWnd, "Ошибка при создании тулбара", "Ошибка", MB_OK | MB_ICONSTOP );
		return -1;      
	}	

	//m_wndToolBar.AsnText( lpButtonsText, sizeof(lpButtonsText) / sizeof(char**), buttons, sizeof(buttons) / sizeof(UINT)-1, FALSE );
	if( !m_wndToolBar.SetButtons( buttons, sizeof(buttons) / sizeof(UINT)) )
	 {
	   ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( m_hWnd, "Ошибка при создании тулбара", "Ошибка", MB_OK | MB_ICONSTOP );
  	   return -1;      
	 }*/


	m_arcTool = new TMSStyleTooolBar( this );
		
	for( int i = 0; i < sizeof(buttons) / sizeof(TBtnDescr); i++ )
	 {                      
	   TMSItem *pMsIt;
	   pMsIt = new TMSItem(); 
	   pMsIt->Init( lpButtonsText[i], buttonsRC[i], buttons[i].uCmd, buttons[i].bRight );
	  
	   *m_arcTool(), pMsIt;
	   //void Init( LPSTR lpStr, UINT uiIdRcBmp, UINT m_uiCommand );
	 }
	
    LOGFONT lf = { 8, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,
      OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
	  VARIABLE_PITCH | FF_DONTCARE, "MS Sans Serif"
	};

	CFont *pFnt = new CFont();
	pFnt->CreateFontIndirect( &lf );
    m_arcTool()->SetFont( pFnt );
	
    m_arcTool()->Init( false ); 

    if( !m_arcTool()->Create(this, WS_CHILD | WS_VISIBLE |
			CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY,  AFX_IDW_TOOLBAR ) 
		//!m_arcTool()->LoadBitmap(IDR_MAINFRAME) 		
	  )
	{
		//AfxMessageBox( "Failed to create toolbar\n", MB_OK | MB_ICONSTOP );
	    ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( m_hWnd, "Ошибка при создании тулбара", "Ошибка", MB_OK | MB_ICONSTOP );
		return -1;      
	}

		
	/*
	if( !m_wndStatusBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_BOTTOM , AFX_IDW_STATUS_BAR) ||
	    !m_wndStatusBar.SetIndicators(indicators,
	    sizeof(indicators)/sizeof(UINT))
	  )
	{
		//TRACE0("Failed to create status bar\n");
	    ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( m_hWnd, "Ошибка при создании строки статуса", "Ошибка", MB_OK | MB_ICONSTOP );
		return -1;      
	}

	UINT nID, nStyle;
	int cxWidth;
	m_wndStatusBar.GetPaneInfo( 0, nID, nStyle, cxWidth );
	m_wndStatusBar.SetPaneInfo( 0, nID, SBPS_STRETCH | SBPS_NORMAL, cxWidth );	
    */

	if( !m_wndStatusBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_BOTTOM , AFX_IDW_STATUS_BAR) ||
	    !m_wndStatusBar.SetIndicators(indicators,
	    sizeof(indicators)/sizeof(UINT))
	  )
	{
		//TRACE0("Failed to create status bar\n");
	    ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( m_hWnd, "Ошибка при создании строки статуса", "Ошибка", MB_OK | MB_ICONSTOP );
		return -1;      
	}

    //m_indOnLine, m_indIn, m_indOut;    	

	m_arcCalc = new TClickBmp( IDB_BMP_CALC, 0, m_iMsgCalc );
	m_arcTimeAlert = new TClickBmpWithClock( IDB_BMP_CLOCK, 0, m_iMsgClock, IDB_BMP_CLOCKL );
	 //UINT uiRcBmp, int iIdCmd, int iMsgToCastOnClick, UINT uiRcBmp2 );	

	CRect rBmpPane( 0, 0, m_arcCalc()->GetW()+1, m_arcCalc()->GetH()+1 );
	m_arcCalc()->Create( NULL, SS_BITMAP|SS_USERITEM|WS_CHILD|WS_VISIBLE, rBmpPane, &m_wndStatusBar );	
	m_arcTimeAlert()->Create( NULL, SS_BITMAP|SS_USERITEM|WS_CHILD|WS_VISIBLE, rBmpPane, &m_wndStatusBar );	

	TAutoMemoryRc<TConnectToItem*> arcIt, arcIt2;
	arcIt = new TConnectToItem( 1 );
	arcIt2 = new TConnectToItem( 2 );
	*(arcIt()) + m_arcCalc();
	*(arcIt2()) + m_arcTimeAlert();
	TConnectToItem* pItem = arcIt();
	TConnectToItem* pItem2 = arcIt2();
	arcIt.ClearRc(), arcIt2.ClearRc();
	m_wndStatusBar + pItem + pItem2;

	int iLampW = m_arcCalc()->GetW();	             
	
	UINT nID, nStyle;
	int cxWidth;
	m_wndStatusBar.GetPaneInfo( 0, nID, nStyle, cxWidth );
	m_wndStatusBar.SetPaneInfo( 0, nID, SBPS_STRETCH | SBPS_NORMAL, cxWidth );	

	m_wndStatusBar.SetPaneText( ID_CALC, "" );
	m_wndStatusBar.GetPaneInfo( ID_CALC, nID, nStyle, cxWidth );
	m_wndStatusBar.SetPaneInfo( ID_CALC, nID, 
	                            SBPS_NOBORDERS|SBPS_POPOUT, iLampW );	

	iLampW = m_arcTimeAlert()->GetW();	             

	m_wndStatusBar.SetPaneText( ID_ALERT, "" );
	m_wndStatusBar.GetPaneInfo( ID_ALERT, nID, nStyle, cxWidth );
	m_wndStatusBar.SetPaneInfo( ID_ALERT, nID, 
	                            SBPS_NOBORDERS|SBPS_POPOUT, iLampW );	
	

	return 0;
 }


void TMainFrame::OnShowWindow( BOOL bShow, UINT nStatus )
 {
   CMDIFrameWnd::OnShowWindow( bShow, nStatus );

   if( m_bFlShow == false ) 
	{	 
      m_bFlShow = true;

	  if( m_bFlLoadingFrame == true && m_optOptions.d.m_bRestoreMainFrame == true )
	   {
         int cx = GetSystemMetrics( SM_CXSCREEN );
	     int cy = GetSystemMetrics( SM_CYSCREEN );

	     if( GetSystemMetrics(SM_CXSCREEN) != m_optOptions.s.m_iScreenX ||
		     GetSystemMetrics(SM_CYSCREEN) != m_optOptions.s.m_iScreenY
		   ) 
	      DefaultPos();
	     else				  
	      SetWindowPlacement( &m_optOptions.s.m_wpFrame );	      				  		  		
	   }	
	  else
	    DefaultPos();
	}
 }

void TMainFrame::DefaultPos()
 {
   TMSStyleTooolBar	*pBar = (TMSStyleTooolBar*)GetDescendantWindow( AFX_IDW_TOOLBAR );
   if( !pBar || typeid(*pBar) != typeid(TMSStyleTooolBar) ) return;

   int iWidth = pBar->GetReqvWidth() + 20 + 2*GetSystemMetrics( SM_CXBORDER );
   int iScrW = GetSystemMetrics( SM_CXSCREEN );
   int iScrH = GetSystemMetrics( SM_CYSCREEN );
   int iWOpt = float(iScrW) * (2.0/3.0);
   int iHOpt = float(iScrH) * (2.0/3.0);

   int iW = min( max(iWidth, iWOpt), iScrW);

   SetWindowPos( NULL, (iScrW - iW)/2, (iScrH - iHOpt)/2, iW, iHOpt, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOREDRAW );
 }

