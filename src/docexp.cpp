#include "docexp.hpp"
#include "vs.hpp"
#include "mfrexp.hpp"
#include "expdlg.hpp"
#include "coding.hpp"
#include "autorc.hpp"
#include "viewexp.hpp"

#include <stdio.h>
#include <stdlib.h>



#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE( TExpertDoc, CDocument )

BEGIN_MESSAGE_MAP( TExpertDoc, CDocument )
	//ON_COMMAND(ID_FILE_SEND_MAIL, OnFileSendMail)
	//ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL, OnUpdateFileSendMail)
END_MESSAGE_MAP()


TExpertDoc::TExpertDoc(): CDocument(), m_sizeDoc(1, 1)
 {
   m_pSubjKey = NULL;
 }
 
TExpertDoc::~TExpertDoc()
 {
 }


bool TExpertDoc::CreateES( bool bAsk )
 {   
   if( bAsk == false )
     m_arcES = new TExpertSystem();
   else
	{
	  TExpertApp *pApp = (TExpertApp*)AfxGetApp();
      TMainFrame *pFram = (TMainFrame*)pApp->m_pMainWnd;  

	  CString strFolder;
      TFolderDlg dlgF( pFram, strFolder, true, *pFram->GetOptPtr() );	  

	  CString strName;
      if( dlgF.DoModal() == IDOK )
	   {	     
	     TDlgName dlgName( pFram, strName );
		 if( dlgName.DoModal() != IDOK ) return false; 
         //CString name( dlg.GetPathName() );
	   }
	  else return false;

	  /*char drive[ _MAX_DRIVE ];
      char dir[ _MAX_DIR ];
      char fname[ _MAX_FNAME ];
      char ext[ _MAX_EXT ];

	  char cBuf[ 300 ];
	  _splitpath( dlg.GetPathName(), drive, dir, fname, ext );
	  _makepath( cBuf, drive, dir, "", "" );	  	 
	  int iL = strlen( cBuf ) - 1;
	  if( cBuf[iL] != '\\' ) strcat( cBuf, "\\" );*/

	  m_arcES = new TExpertSystem( pFram->GetProtectLvl(), 
	      strFolder, strName );	 	  
	}

   return true;
 }

void TExpertDoc::OnCloseDocument()
 {   
   AfxGetApp()->AddToRecentFileList( GetPathName() );
   CDocument::OnCloseDocument();
 }

BOOL TExpertDoc::OnNewDocument()
 {
   if( !CDocument::OnNewDocument() ) return FALSE;

	//((CEditView*)m_viewList.GetHead())->SetWindowText(NULL);
   BOOL bRes = CreateES( true );	 	
   if( bRes ) SetTitle( GetTitle() );
   return bRes;
 } 

void TExpertDoc::DeleteContents()
 {   
   m_arcES.FreeRc();
 }

CFile* TExpertDoc::GetFile( LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError )
 {
   TMFOptions *pFOpt = ((TMainFrame*)(AfxGetApp()->m_pMainWnd))->GetOptPtr();

   TAutoMemoryRc<TCodingBAKMirrorFile*> arcF = 
	 new TCodingBAKMirrorFile( pFOpt->d.m_bUseCoding, 
	                           pFOpt->d.codUserFile, 
							   pFOpt->d.m_cKey 
							 );      

   CFileException except;
   if( !arcF()->Open(lpszFileName, nOpenFlags, &except) ) arcF.FreeRc();
	//{
      //if( except.m_cause == CFileException::fileNotFound ) throw TExceptCfgNotFound();
	  //AfxThrowFileException( except.m_cause, except.m_lOsError, (LPCSTR)strPath );	  
	  
	//}
   CFile *pF = arcF();
   arcF.ClearRc();

   return pF;
 }


BOOL TExpertDoc::CanCloseFrame( CFrameWnd* pFrame )
 {
   /*if( pFrame->GetActiveView() && 
	  ((TBaseView*)(pFrame->GetActiveView()))->CanClose() == false )
	 return 0;*/

   POSITION pos = GetFirstViewPosition();
   TBaseView *pV;
   if( pos && (pV = (TBaseView*)GetNextView(pos)) )   
	 if( pV->CanClose() == false ) return FALSE;

   return CDocument::CanCloseFrame( pFrame );
 }

BOOL TExpertDoc::OnSaveDocument( LPCTSTR lp )
 {
   char path_buffer[_MAX_PATH];
   char drive[_MAX_DRIVE];
   char dir[_MAX_DIR];
   char fname[_MAX_FNAME];
   char ext[_MAX_EXT];

   _splitpath( lp, drive, dir, fname, ext );
   _makepath( path_buffer, drive, dir, "", "" );

   if( _stricmp(((TExpertApp*)AfxGetApp())->GetDescrPath(), path_buffer) )   
	{
	  char cBuf[ 1024 ];
	  sprintf( cBuf, "Дескрипторы экспертных систем размещается в [%s]. - Выберете эту папку. " 
	                 "Изменение этого пути доступно только администратору ",
			   (LPCSTR)((TExpertApp*)AfxGetApp())->GetDescrPath()
             );
	  ::MessageBox( ((TExpertApp*)AfxGetApp())->m_pMainWnd->m_hWnd,
	     cBuf, "Ошибка", MB_OK|MB_ICONSTOP );

	  return FALSE;
	}
   if( _stricmp(ext, ".VES") )
	{
      char cBuf[ 1024 ];
	  sprintf( cBuf, "Ошибочное расширение файла-дескриптора [%s]. - Требуется .VES", 	  
			   lp
             );
	  ::MessageBox( ((TExpertApp*)AfxGetApp())->m_pMainWnd->m_hWnd,
	     cBuf, "Ошибка", MB_OK );

	  return FALSE;
	}

   return CDocument::OnSaveDocument( lp );
 }

void TExpertDoc::SetTitle( LPCTSTR lpszTitle )
 {
   CString str( lpszTitle );
   //SetTitleMy( str, true );

   CDocument::SetTitle( str );
 }

void TExpertDoc::SetTitleMy( CString& rTtl, bool bFlCallOutside )
 {
   if( !m_arcES() ) return;

   POSITION pos = GetFirstViewPosition();
   TBaseView *pV;
   if( pos && (pV = (TBaseView*)GetNextView(pos)) )
	{
	  if( pV->IsA() == TV_Expert )
        rTtl += CString(" - \"") + ((TBaseView*)pV)->GetName() + CString("\"");   
	  else
	    rTtl.Format( "%s - \"%s\"", pV->GetShortFileName(), pV->GetName() );   
	}
   else  
     rTtl += CString(" - \"") + m_arcES()->GetName() + CString("\"");
      
 }

void TExpertDoc::ReleaseFile( CFile* pFile, BOOL bAbort )
 {    
   CDocument::ReleaseFile( pFile, bAbort );
 }

void TExpertDoc::ReportSaveLoadException( LPCTSTR lpszPathName, CException* e, BOOL bSaving, UINT nIDPDefault )
 {
   CDocument::ReportSaveLoadException( lpszPathName, e, bSaving, nIDPDefault );
 }

BOOL TExpertDoc::OnOpenDocument( LPCTSTR lpszPathName )
 {
   BOOL bRes = CDocument::OnOpenDocument( lpszPathName );
   if( !bRes || !m_arcES() ) return FALSE;

   
//return FALSE;

   TExpertApp *pApp = (TExpertApp*)AfxGetApp();
   TMainFrame *pFram = (TMainFrame*)pApp->m_pMainWnd;  
   
   if( m_arcES()->KonfirmToObj(*pFram->GetProtectLvl()) == false )
	{
	  CString str;
	  str.Format( "Нарушение прав доступа к ЭС: '%s'", m_arcES()->GetName() );
      pApp->bwcc.BWCCMessageBox( pFram->m_hWnd, 
		(LPSTR)(LPCSTR)str, "Ошибка", MB_OK | MB_ICONSTOP );
	  m_arcES.FreeRc();

	  return FALSE;
	}			

   if( m_arcES()->IsModified() == true ) SetModifiedFlag();

   return TRUE;
 }

void TExpertDoc::Serialize( CArchive& ar )
 {
   //try {
	 CDocument::Serialize( ar );
	 if( ar.IsLoading() ) 
	  {
	    TExpertApp *pApp = (TExpertApp*)AfxGetApp();
        TMainFrame *pFram = (TMainFrame*)pApp->m_pMainWnd;  

		if( !m_arcES() ) CreateES( false );
		ar >> *m_arcES();	
	  }
	 else ar << *m_arcES();     
	//}
   /*catch( CArchiveException *pEx )
	{
	  int iErr;
	  switch( pEx->m_cause )
	   {
	     case CArchiveException::none:
		  iErr = ID_ERR_NONE;
		  break;

         case CArchiveException::generic:
		  iErr = ID_ERR_GENERIC;
		  break; 

		 case CArchiveException::readOnly:
		  iErr = ID_ERR_READONLY;
		  break; 

         case CArchiveException::endOfFile:
		  iErr = ID_ERR_ENDOFFILE;
		  break; 

         case CArchiveException::writeOnly:
		  iErr = ID_ERR_WRITEONLY;
		  break;

	     case CArchiveException::badIndex:
		  iErr = ID_ERR_BADINDEX;
		  break;

         case CArchiveException::badClass:
		  iErr = ID_ERR_BADCLASS;
		  break;

		 case CArchiveException::badSchema:
		  iErr = ID_ERR_BADSCHEMA;
		  break;

		 default:
		  iErr = ID_ERR_UNKNOWN;
		  break;
	   };
	  CString cStr; cStr.LoadString( iErr );
	  AfxMessageBox( (LPCSTR)cStr );	   
      pEx->Delete();
	}   
   catch( CException* pE )
	{
      pE->ReportError();
	  pE->Delete();
	}*/
 }

