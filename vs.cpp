#include "vs.hpp"
#include <locale.h>
#include "vs.rh"

#include "docexp.hpp"
#include "mfrexp.hpp"
#include "mdiexp.hpp"
#include "viewexp.hpp"
#include "v_expert.hpp"
#include "autorc.hpp"
#include "..\\RUDAKOW\\SERVIS\\texcept.hpp"

#include "locrc.hpp"
//#include "mgraf.rh"

#include "..\\RUDAKOW\\SERVIS\\servis.hpp"
#include "..\\RUDAKOW\\SERVIS\\mydib.hpp"
#include "..\\RUDAKOW\\SERVIS\\pathex.hpp"

#include <afxcmn.h>
#include <afxdisp.h>

#include "var.hpp"
#include "tlsf_dlg.hpp"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP( TExpertApp, CWinApp )
	ON_COMMAND( ID_FILE_NEW, CWinApp::OnFileNew )
	ON_COMMAND( ID_FILE_OPEN, CWinApp::OnFileOpen )
	//ON_COMMAND( ID_FILE_SAVE, CWinApp::OnFileSave )
	//ON_COMMAND( ID_FILE_CLOSE, CWinApp::OnFileClose )

	ON_COMMAND( ID_APP_ABOUT, OnAbout )	
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC( TMyDocTemplate, CMultiDocTemplate )

IMPLEMENT_DYNCREATE( TMyDocManeger, CDocManager )


TMyDocManeger::~TMyDocManeger()
 {
 }


static BOOL MyOwerwritePropt( CString& rStr )
 {
   CString str;
   str.Format( "Файл \"%s\" уже существует. Переписать его ?", (LPCSTR)rStr );

   return (::MessageBox( ((TExpertApp*)AfxGetApp())->m_pMainWnd->m_hWnd,
	        (LPSTR)(LPCSTR)str, "Предупреждение", MB_YESNO|MB_ICONEXCLAMATION ) == IDYES);
 }


static void AppendFilterSuffix(CString& filter, OPENFILENAME& ofn,
	CDocTemplate* pTemplate, CString* pstrDefaultExt)
{
	ASSERT_VALID(pTemplate);
	ASSERT_KINDOF(CDocTemplate, pTemplate);

	CString strFilterExt, strFilterName;
	if (pTemplate->GetDocString(strFilterExt, CDocTemplate::filterExt) &&
	 !strFilterExt.IsEmpty() &&
	 pTemplate->GetDocString(strFilterName, CDocTemplate::filterName) &&
	 !strFilterName.IsEmpty())
	{
		// a file based document template - add to filter list
#ifndef _MAC
		ASSERT(strFilterExt[0] == '.');
#endif
		if (pstrDefaultExt != NULL)
		{
			// set the default extension
#ifndef _MAC
			*pstrDefaultExt = ((LPCTSTR)strFilterExt) + 1;  // skip the '.'
#else
			*pstrDefaultExt = strFilterExt;
#endif
			ofn.lpstrDefExt = (LPTSTR)(LPCTSTR)(*pstrDefaultExt);
			ofn.nFilterIndex = ofn.nMaxCustFilter + 1;  // 1 based number
		}

		// add to filter
		filter += strFilterName;
		ASSERT(!filter.IsEmpty());  // must have a file type name
		filter += (TCHAR)'\0';  // next string please
#ifndef _MAC
		filter += (TCHAR)'*';
#endif
		filter += strFilterExt;
		filter += (TCHAR)'\0';  // next string please
		ofn.nMaxCustFilter++;
	}
}


BOOL TMyDocManeger::DoPromptFileName( CString& fileName, UINT nIDSTitle,
                                      DWORD lFlags, BOOL bOpenFileDialog, 
								      CDocTemplate* pTemplate
							        )
 {
    CString strKey( fileName ); 
    CString strT( ((TExpertApp*)AfxGetApp())->GetDescrPath() );
	//strT = strT.Left( strT.GetLength() - 1 );
	if( fileName.IsEmpty() ) strT += "*.ves";
	else 
	 {	 
	   char drive[ _MAX_DRIVE ];
       char dir[ _MAX_DIR ];
       char fname[ _MAX_FNAME ];
       char ext[ _MAX_EXT ];
	   char path_buf[ _MAX_PATH ];

	   _splitpath( (LPSTR)(LPCSTR)fileName, drive, dir, fname, ext );
	   _makepath( path_buf, "", "", fname, ext );
	   strT += path_buf;
	 }
	TLSFileDlg dlgFile( bOpenFileDialog, 
	                    NULL,
					    (LPCSTR)strT,
                        OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
						NULL, 
						AfxGetApp()->m_pMainWnd,
						true,
						true
					  );
	

	CString title;
	VERIFY(title.LoadString(nIDSTitle));

	dlgFile.m_ofn.Flags |= lFlags;
	DWORD dwKey = dlgFile.m_ofn.Flags;
	dlgFile.m_ofn.Flags &= ~OFN_OVERWRITEPROMPT;


	CString strFilter;
	CString strDefault;
	if (pTemplate != NULL)
	{
		ASSERT_VALID(pTemplate);
		AppendFilterSuffix(strFilter, dlgFile.m_ofn, pTemplate, &strDefault);
	}
	else
	{
		// do for all doc template
		POSITION pos = m_templateList.GetHeadPosition();
		BOOL bFirst = TRUE;
		while (pos != NULL)
		{
			CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);
			AppendFilterSuffix(strFilter, dlgFile.m_ofn, pTemplate,
				bFirst ? &strDefault : NULL);
			bFirst = FALSE;
		}
	}

	// append the "*.*" all files filter
	CString allFilter;
	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += (TCHAR)'\0';   // next string please

	strFilter += _T("*.*");

	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter;

	dlgFile.m_ofn.lpstrTitle = title;
	//dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

	BOOL bResult = dlgFile.DoModal() == IDOK ? TRUE : FALSE;
	//fileName.ReleaseBuffer();

	fileName = dlgFile.GetPathName();

	if( bResult && !bOpenFileDialog && (dwKey & OFN_OVERWRITEPROMPT) )	 
       if( strKey.CompareNoCase(fileName) && CheckExist(fileName) ) 
		  return MyOwerwritePropt( fileName );
	
	return bResult;
 }


void TMyDocTemplate::AddDocument( CDocument* pDoc )
 {
   CMultiDocTemplate::AddDocument( pDoc );
 }

void TMyDocTemplate::RemoveDocument( CDocument* pDoc )
 {
   CMultiDocTemplate::RemoveDocument( pDoc );   
 }



BOOL TMyDocTemplate::GetDocString( CString& rString, enum DocStringIndex index ) const
 {
   if( index == CDocTemplate::docName ) 
	{
	  rString = "Без_имени";
	  return TRUE;
	}
   
   return CMultiDocTemplate::GetDocString( rString, index );
 }

/*void TMyDocTemplate::LoadTemplate()
 {
   CMultiDocTemplate::LoadTemplate();
   if( m_hMenuShared )
	
 }*/

TExpertApp::TExpertApp(): CWinApp( "TExpertApplication" ), bwcc( NULL )
 {

 }

BOOL TExpertApp::OnIdle( LONG lCount )
 {
   //if( m_pMainWnd ) ((TMainFrame*)m_pMainWnd)->MyIdle( lCount );
   return CWinApp::OnIdle( lCount );
 }

void TExpertApp::OnAbout()
 {
   //CDialog about( IDD_DLG_ABOUT );
   //about.DoModal();
 }

BOOL TExpertApp::InitInstance()
 {   
   setlocale( LC_ALL, "" );
   
   AfxEnableControlContainer();

   /*TNameSpace nmSpc;
   char ch[] = "81/9.0 / (-sqrt(max(9,2.0))/-double((false || !(true && true)) || 'A' ))";
   LPSTR lp = ch;
   for( ; *lp; lp++ ) if( *lp == '#' ) *lp = 0x22;
   TExpression expr( ch, nmSpc );       

   TVariantVar var;
   try {
     expr.Parse( false );   
     expr.Val( var );
	}
   catch( TMyException *pE )
	{
      CString str( pE->GetMsg() );
	  pE->Delete();
	}
   catch(...)
	{
       int a = 1;
	}
   //expr.Test( " -Name1%'a ''b' +Name2*Sin(@a+1/2.5E+2) || _A12<=B12_ + !CABb<" );	
   */


   LPSTR lpBuf = m_Startpath.GetBuffer( _MAX_PATH );
   ::GetModuleFileName( m_hInstance, lpBuf, _MAX_PATH );
   
    char drive[ _MAX_DRIVE ];
    char dir[ _MAX_DIR ];
    char fname[ _MAX_FNAME ];
    char ext[ _MAX_EXT ];

	_splitpath( lpBuf, drive, dir, fname, ext );
	_makepath( lpBuf, drive, dir, "", "" );	  	 

	m_Startpath.ReleaseBuffer();

	m_Descriptpath = m_Startpath + "VES\\";

   CMultiDocTemplate* pDocTemplate = NULL;
   try {

     InitServisDLL();
	 InitMyDIBDLL();

     Enable3dControls();
	 //AfxEnableControlContainer();
     bwcc.EnableBWCC( m_hInstance, TRUE );

     pDocTemplate = new TMyDocTemplate(
       IDR_MAINFRAME,
	   RUNTIME_CLASS( TExpertDoc ),
	   RUNTIME_CLASS( TMDIFrame ),       
	   RUNTIME_CLASS( TExpertView )
	  );
	}
   catch( TExcept rExept )
    {
      rExept.ReportExcept();
	  rExept.Delete();

	  if( pDocTemplate ) delete pDocTemplate;
	  return FALSE;
	}
   catch( CException* pE )
	{
      pE->ReportError();
	  pE->Delete();

	  if( pDocTemplate ) delete pDocTemplate;
	  return FALSE;
	}
   catch(...)
    {
      AfxMessageBox( "Any exception", MB_OK ); 

	  if( pDocTemplate ) delete pDocTemplate;
	  return FALSE;
	}

   LoadStdProfileSettings();

   m_pDocManager = new TMyDocManeger(); //!!! only here

   AddDocTemplate( pDocTemplate );

   m_pMainWnd = new TMainFrame();
   if( !((TMainFrame*)m_pMainWnd)->LoadFrame(IDR_MAINFRAME) ) return FALSE;
      

   m_pMainWnd->ShowWindow( m_nCmdShow );
   m_pMainWnd->UpdateWindow();

   return TRUE;
 }

void TExpertApp::WinHelp( DWORD dwData, UINT nCmd ) 
 {
    switch (dwData) {

	case 387072: dwData = 3000;//toolbar
        break;
    // menu file
    case 123136: dwData = 3001;//new
        break;
    case 123137: dwData = 3002;//open
        break;
    case 123138: dwData = 3003;//close
        break;
    case 123139: dwData = 3004;
        break;
    case 123140: dwData = 3005;//save
        break;
	case 123201: dwData = 3007;//exit
        break;

   case 123152: dwData = 3006; //ricent
        break;
    
    // menu admin
    case 65546: dwData = 3020;
        break;
    case 105582: dwData = 3021;
        break;
    case 65547: dwData = 3022;
        break;
    
    // menu windows
    case 123186: dwData = 3030;
        break;
    case 123187: dwData = 3030;
        break;
    case 123188: dwData = 3030;
        break;
    case 123185: dwData = 3030;
        break;
    case 65549: case 65548: dwData = 3030;
        break;
    case 105579: dwData = 3030;
        break;
	case 105580: dwData = 3030;
	    break;
    
    // menu help
    case 123203: case 123205: dwData = 3040;
        break;
    case 70537: dwData = 3040;
        break;

	case 262146: dwData = 2999; //userinterface
	    break;

    // status bar
    case 387073: dwData = 3050;
        break;

    // view window
    case 131173: dwData = 2999;
        break;
    /*case 131173: dwData = 3100;
        break;
    case 131173: dwData = 3100;
        break;
    case 131173: dwData = 3100;
        break;*/

	case 131077: dwData = 2997; //диалог логина
	 break;

	//диалог Администрирование
	case 131250: dwData = 2996;
	 break;
    case 131249: dwData = 2995;
	 break;
	case 131248: dwData = 2994;
	 break;

	//диалог изменение уровня доступа
	case 131243: case 131245: case 131246: case 131073:
	  dwData = 3020;
	  break;	

    //диалог выбор папки
	case 131075:
	  dwData = 2993;
	  break;	

	//калькулятор
	case 131178:
	  dwData = 2992;
	  break;	

    case 131179:
	  dwData = 2991;
	  break;	

   //explorer
   case 12345:
	  dwData = 2989;
	  break;	
   //explorer pane
   case 12355:
	  dwData = 2988;
	  break;	

	  //product edit
   case 12348:
	  dwData = 3988;
	  break;	
	  //product pane
   case 12358:
	  dwData = 3987;
	  break;	

	  //work
   case 12347:
	  dwData = 3986;
	  break;	
	  //work pane
   case 12357:
	  dwData = 3985;
	  break;	

	  //work opt
   case 131353:
	  dwData = 3983;
	  break;	

	  //create new obj
   case 131079:
	  dwData = 3982;
	  break;	
	 
    };
    
    CWinApp::WinHelp( dwData, nCmd);    

}

TExpertApp  myApp;

