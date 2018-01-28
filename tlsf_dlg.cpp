#include "tlsf_dlg.hpp"

#include "vs.hpp"
#include "vs.rh"

#include "docexp.hpp"
#include "mfrexp.hpp"
/*#include "mdiexp.hpp"
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

#include "var.hpp"*/


BEGIN_MESSAGE_MAP( TLSFileDlg, CFileDialog )
  ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


bool TLSFileDlg::CheckChanged()
 {   
   char drive[ _MAX_DRIVE ];
   char dir[ _MAX_DIR ];
   char fname[ _MAX_FNAME ];
   char ext[ _MAX_EXT ];
   char path_buf[ _MAX_PATH ];  

   _splitpath( (LPSTR)(LPCSTR)GetPathName(), drive, dir, fname, ext );
   _makepath( path_buf, drive, dir, NULL, NULL );

   if( m_csPrivPath.CompareNoCase(path_buf) )
	{
      m_csPrivPath = path_buf;
	  return true;
	}
   return false;
 }

BOOL TLSFileDlg::OnInitDialog( )
 {
   BOOL bRes = CFileDialog::OnInitDialog();

   if( !m_bOpen ) return bRes;
   
   return bRes;
 }

HBRUSH TLSFileDlg::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
 {
   HBRUSH hbr = CFileDialog::OnCtlColor( pDC, pWnd, nCtlColor );

   if( nCtlColor == CTLCOLOR_STATIC )
	{
	  int id = pWnd->GetDlgCtrlID();
	  if( id == IDC_STATIC_ES )
	    pDC->SetTextColor( m_bRedTtl == true ? RGB(255,0,0):RGB(0,0,255) );
	}

   return hbr;
 }

static void RemoveSYSChars( CString& str )
 {
   if( str.IsEmpty() ) return;

   CString str2;
   LPSTR lp = str2.GetBuffer( str.GetLength() + 1 );
   memset( lp, 0, str.GetLength() + 1 );

   LPSTR lp1 = (LPSTR)(LPCSTR)str;
   for( ; *lp1; lp1++ )
	 if( isalpha(*lp1) || isdigit(*lp1) ) *lp++ = *lp1;
	   
   str2.ReleaseBuffer();
   str = str2;
 }

struct TChangeTtl
 {
   LPSTR lpOrg, lpSub;
 };

void TLSFileDlg::OnInitDone()
 {
   static TChangeTtl chT[] = 
	{
	 {"Open", "Дальше"},
	 {"Cancel", "Отменить"},
	 {"Lookin", "Смотреть в:"},
	 {"Filename", "Файл:"},
	 {"Filesoftype", "Файлы типа:"},
	 {"Save", "Сохранить"},
	 {"Savein", "Папка:"},
	 {"Saveastype", "Файлы типа:"},
	 {NULL, NULL}
	};

   CFileDialog::OnInitDone();

   //if( !m_bOpen ) return;

   CWnd *pPar;
   CenterWindow( pPar = GetParent() );    

   CWnd *pW = pPar->GetWindow( GW_CHILD );
   HWND hwF = pW ? pW->m_hWnd:NULL;
   BOOL bStart = TRUE;
   for( ; pW && (pW->m_hWnd != hwF || bStart) ; pW = pW->GetWindow(GW_HWNDNEXT) )
	{
	  bStart = FALSE;
	  if( ::GetParent(pW->m_hWnd) != pPar->m_hWnd ) continue;

	  CString str; pW->GetWindowText( str );
	  RemoveSYSChars( str );
	  TChangeTtl *p = chT;
      for( ; p->lpOrg; p++ )
	    if( !str.CompareNoCase(p->lpOrg) )
		 {
           pW->SetWindowText( p->lpSub ); break;
		 }
	}   
 }

TLSFileDlg::TLSFileDlg( BOOL bOpenFileDialog, LPCTSTR lpszDefExt, 
  LPCTSTR lpszFileName, DWORD dwFlags, LPCTSTR lpszFilter, 
  CWnd* pParentWnd, bool bFlShowTtl, bool bFlWarnPath ):
  CFileDialog( bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd  )
   {
	 m_bFirstCall = true;
	 m_bOpen = bOpenFileDialog;
     m_bRedTtl = false;

	 m_bFlWarnPath = bFlWarnPath;

	 m_bFlShowTtl = bFlShowTtl;
	
	 m_ofn.hInstance = AfxGetResourceHandle();
	 if( bOpenFileDialog && m_bFlShowTtl == true )
	  {
	    m_ofn.Flags |= OFN_ENABLETEMPLATE | OFN_EXPLORER;
        m_ofn.lpTemplateName = MAKEINTRESOURCE( IDD_MYOPENADD );	
	  }
   }

void TLSFileDlg::OnFileNameChange()
 {
   if( !m_bOpen ) return;

   char drive[ _MAX_DRIVE ];
   char dir[ _MAX_DIR ];
   char fname[ _MAX_FNAME ];
   char ext[ _MAX_EXT ];
   char path_buf[ _MAX_PATH ];

   CString sTtl, sName;

   _splitpath( (LPSTR)(LPCSTR)GetFileName(), drive, dir, fname, ext );
   if( !_stricmp(ext, ".VES") )
	{
	  TAutoMemoryRc<TExpertSystem*> arcES( new TExpertSystem() );

	  TMFOptions *pFOpt = ((TMainFrame*)(AfxGetApp()->m_pMainWnd))->GetOptPtr();

      TAutoMemoryRc<TCodingBAKMirrorFile*> arcF = 
	    new TCodingBAKMirrorFile( pFOpt->d.m_bUseCoding, 
	                              pFOpt->d.codUserFile, 
							      pFOpt->d.m_cKey 
							    );      

      CFileException except;
	  if( strchr((LPCSTR)GetPathName(), '*') || strchr((LPCSTR)GetPathName(), '?') )
	   {
         sTtl = "Экспертная система:";
	     sName = "";
		 m_bRedTtl = false;
	   }
	  else
      if( !arcF()->Open( (LPCSTR)GetPathName(), 
	      CFile::modeRead|CFile::shareDenyWrite, &except) )
	   {
         sTtl  = "Ошибка доступа";
		 LPSTR lp = sName.GetBuffer( 1024 );
		 except.GetErrorMessage( lp, 1023 );
		 sName.ReleaseBuffer();
		 m_bRedTtl = true;
	   }
      else
	   {
	     bool bOK = false;
	     try {
	       CArchive arch( arcF(), CArchive::load );
		   arcES()->LoadHdrOnly( arch );
		   bOK = true;
		  }
	     catch( CException *pE )
		  {
		    sTtl  = "Ошибка чтения";
		    LPSTR lp = sName.GetBuffer( 1024 );
		    pE->GetErrorMessage( lp, 1023 );
		    sName.ReleaseBuffer();         
			pE->Delete();
			m_bRedTtl = true;
		  }

		 if( bOK == true )
		  {
            sTtl = "Экспертная система:";
	        sName = arcES()->GetName();
			m_bRedTtl = false;
		  }
	   }	  	      
	}
   else
	{	  
	  sTtl = "Это не экспертная система";
	  sName = "";
	  m_bRedTtl = true;
	}

   SetDlgItemText( IDC_STATIC_ES, sTtl );
   SetDlgItemText( IDC_EDIT_NAME, sName );
 }

void TLSFileDlg::OnFolderChange()
 {
   if( m_bFirstCall == true )
	{
      m_bFirstCall = false; 	  

      char drive[ _MAX_DRIVE ];
      char dir[ _MAX_DIR ];
      char fname[ _MAX_FNAME ];
      char ext[ _MAX_EXT ];
      char path_buf[ _MAX_PATH ];  

      _splitpath( (LPSTR)(LPCSTR)GetPathName(), drive, dir, fname, ext );
      _makepath( path_buf, drive, dir, NULL, NULL );

	  m_csPrivPath = path_buf;

	  return;
	}

   if( CheckChanged() == false ) return;

   if( m_bFlWarnPath == false ) return;

   CString str;
   str.Format( "Нельзя менять папку: положение этих файлов определяется только администратором" );

   ::MessageBox( m_hWnd,
	     (LPSTR)(LPCSTR)str, "Предупреждение", MB_OK|MB_ICONEXCLAMATION );
 }
