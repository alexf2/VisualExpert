#include "mfrexp.hpp"

#include "vs.rh"
#include "vs.hpp"
#include "autorc.hpp"
#include "fr_opt.hpp"

#include <afxcmn.h>
#include <afxwin.h>
#include <typeinfo.h>

#include "dlgacess.hpp"


void TMainFrame::SaveGlobalConfig()
 {
   TCodingBAKMirrorFile f;	  

   CString strPath( CString(((TExpertApp*)AfxGetApp())->GetStartPath()) + CString(GLOBAL_CONFIG_FILENAME) );

   CFileException except;
   if( !f.Open((LPCSTR)strPath, CFile::shareExclusive | CFile::modeCreate | CFile::modeWrite, &except) )	      
	  AfxThrowFileException( except.m_cause, except.m_lOsError, (LPCSTR)strPath );	  
	   
   CArchive arch( &f, CArchive::store );

   arch << m_optOptions;	 
 }

void TMainFrame::LoadGlobalCongig()
 {
   TCodingBAKMirrorFile f;	  

   CString strPath( CString(((TExpertApp*)AfxGetApp())->GetStartPath()) + CString(GLOBAL_CONFIG_FILENAME) );

   CFileException except;
   if( !f.Open((LPCSTR)strPath, CFile::shareExclusive | CFile::modeRead, &except) )
	{
      if( except.m_cause == CFileException::fileNotFound ) throw TExceptCfgNotFound();
	  AfxThrowFileException( except.m_cause, except.m_lOsError, (LPCSTR)strPath );	  
	}
	   
	CArchive arch( &f, CArchive::load );

	arch >> m_optOptions;	 
    if( m_optOptions.CheckCRC() == false ) throw TExceptCfgBadCRC();	
 }

void TMainFrame::CreateGlobalConfig()
 {
    TCodingBAKMirrorFile f;	  

    CString strPath( CString(((TExpertApp*)AfxGetApp())->GetStartPath()) + CString(GLOBAL_CONFIG_FILENAME) );
	//LPSTR lpBuf = strPath.GetBuffer( 2*_MAX_PATH );
    //::GetTempFileName( ((TExpertApp*)AfxGetApp())->GetStartPath(), "vsp", 0, lpBuf );
	//strPath.ReleaseBuffer();

	CFileException except;
    if( !f.Open((LPCSTR)strPath, CFile::shareExclusive | CFile::modeCreate | CFile::modeWrite, &except) )
	   AfxThrowFileException( except.m_cause, except.m_lOsError, (LPCSTR)strPath );
	   
	CArchive arch( &f, CArchive::store );

	arch << m_optOptions;	 
 }

void TMainFrame::KeepScreenConfig()
 {
   m_optOptions.s.m_iScreenX = GetSystemMetrics( SM_CXSCREEN );
   m_optOptions.s.m_iScreenY = GetSystemMetrics( SM_CYSCREEN );
   GetWindowPlacement( &m_optOptions.s.m_wpFrame );
 }

void TMainFrame::CreateDefaultUsers()
 {
    TCodingBAKMirrorFile f( 	   
	   m_optOptions.d.m_bUseCoding, 
	   m_optOptions.d.codUserFile,
	   m_optOptions.d.m_cKey
	   );	  	

    CString strPath( CString(((TExpertApp*)AfxGetApp())->GetStartPath()) + CString(GLOBAL_USERS_FILENAME) );
	//LPSTR lpBuf = strPath.GetBuffer( 2*_MAX_PATH );
    //::GetTempFileName( ((TExpertApp*)AfxGetApp())->GetStartPath(), "vsp", 0, lpBuf );
	//strPath.ReleaseBuffer();

	CFileException except;
    if( !f.Open((LPCSTR)strPath, CFile::shareExclusive | CFile::modeCreate | CFile::modeWrite, &except) )
	   AfxThrowFileException( except.m_cause, except.m_lOsError, (LPCSTR)strPath );
	   
	CArchive arch( &f, CArchive::store );

	TUser adm( "", "" ); 
	adm.SetRights( TAcessLevel::TU_Administrator, TAcessLevel::TUS_Administrator );
	adm.SetPassword( DEF_PASS_ADMINISTRATOR );	
	TAutoMemoryRc<TUserLocal*> arcAdm = new TUserLocal( adm );

	TUser kognit( "", "" ); 
	kognit.SetRights( TAcessLevel::TU_Kognitolog, TAcessLevel::TUS_Anonime );
	kognit.SetPassword( DEF_PASS_KOGNITOLOG );
	TAutoMemoryRc<TUserLocal*> arcKognit( new TUserLocal(kognit) ); 	

	TUser expert( "", "" ); 
	expert.SetRights( TAcessLevel::TU_Expert, TAcessLevel::TUS_Anonime );
	expert.SetPassword( DEF_PASS_EXPERT );
	TAutoMemoryRc<TUserLocal*> arcExpert( new TUserLocal(expert) ); 	

	TUser user( "", "" ); 
	user.SetRights( TAcessLevel::TU_User, TAcessLevel::TUS_Anonime );
	user.SetPassword( DEF_PASS_USER );
    TAutoMemoryRc<TUserLocal*> arcUser( new TUserLocal(user) ); 	
	
	arch << *arcAdm() << *arcKognit() << *arcExpert() << *arcUser();	 
 }

class TPasswDlg: public CDialog
 {
public:
  TPasswDlg( CWnd *pPar ):     
    CDialog( IDD_DLG_PASSW, pPar )
   {  
   }	

  virtual BOOL OnInitDialog();
  virtual void DoDataExchange( CDataExchange* pDX );

  const CString& GetPassw() const { return m_strPassw; }
  
private:    
  CString m_strPassw;  
 };

void TPasswDlg::DoDataExchange( CDataExchange* pDX )
 {
   CDialog::DoDataExchange( pDX );

   DDX_Text( pDX, IDC_ED_PASSW, m_strPassw );      
   m_strPassw.MakeUpper();
 }

BOOL TPasswDlg::OnInitDialog()
 {
   CDialog::OnInitDialog();
   
   CenterWindow( GetParent() );

   HWND hw1 = ::GetDlgItem( m_hWnd, IDC_ED_PASSW );
   PostMessage( WM_NEXTDLGCTL, (WPARAM)hw1, 1 );	        

   return FALSE;
 }



int TMainFrame::LoadConfigOnStart()
 {
   m_bFlLoadingFrame = false;
   try {
     LoadGlobalCongig();
	 m_bFlLoadingFrame = true;
	}
   catch( TExceptCfgNotFound )
	{
      int iRes = ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( m_hWnd, 
	     "Файл конфигурации не найден. Создать его ?", 
		 "Вопрос", MB_YESNO | MB_ICONQUESTION );
	  if( iRes == IDYES )
	   {
	     try {
		    KeepScreenConfig();
            CreateGlobalConfig();
			//m_bFlLoadingFrame = true;
		  }
		 catch( CArchiveException* pE )
		  {	  		  
		    if( pE->m_strFileName.IsEmpty() ) pE->m_strFileName = GLOBAL_CONFIG_FILENAME;
            pE->ReportError( MB_OK | MB_ICONEXCLAMATION );
	        pE->Delete();  
		  }
		 catch( CFileException* pE )
		  {
            if( pE->m_strFileName.IsEmpty() ) pE->m_strFileName = GLOBAL_CONFIG_FILENAME;
            pE->ReportError( MB_OK | MB_ICONSTOP );
	        pE->Delete();  	        
		  }
	     catch( CException* pE )
		  {	  
            pE->ReportError( MB_OK | MB_ICONEXCLAMATION );
	        pE->Delete();  
		  } 
	   }
	}
   catch( TExceptCfgBadCRC )
	{
      ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( m_hWnd, 
	     "Файл конфигурации испорчен. Его можно стереть и перегрузить Visyal Expert.", 
		 "Предупреждение", MB_OK );
	}
   catch( CArchiveException* pE )
	{	  		  
	  if( pE->m_strFileName.IsEmpty() ) pE->m_strFileName = GLOBAL_CONFIG_FILENAME;
      pE->ReportError( MB_OK | MB_ICONEXCLAMATION );
	  pE->Delete();  
	}
   catch( CFileException* pE )
	{
      if( pE->m_strFileName.IsEmpty() ) pE->m_strFileName = GLOBAL_CONFIG_FILENAME;
      pE->ReportError( MB_OK | MB_ICONSTOP );
	  pE->Delete();  	  
	}
   catch( CException* pE )
	{	  
      pE->ReportError( MB_OK | MB_ICONEXCLAMATION );
	  pE->Delete();  
	} 


   TCodingBAKMirrorFile f;	  

   CString strPath( CString(((TExpertApp*)AfxGetApp())->GetStartPath()) + CString(GLOBAL_USERS_FILENAME) );

   CFileException except;
   try {
     if( !f.Open((LPCSTR)strPath, CFile::shareDenyWrite | CFile::modeRead, &except) )
	  {
        if( except.m_cause == CFileException::fileNotFound ) throw TExceptCfgNotFound();
	    AfxThrowFileException( except.m_cause, except.m_lOsError, (LPCSTR)strPath );	  
	  }
	}
   catch( TExceptCfgNotFound )
	{
      int iRes = ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( m_hWnd, 
	     "Файл описания пользователей не найден. Создать его ?", 
		 "Вопрос", MB_YESNO | MB_ICONQUESTION );
	  if( iRes == IDYES )
	   {
	     try {		    
            CreateDefaultUsers();			
		  }
		 catch( CFileException* pE )
		  {
            if( pE->m_strFileName.IsEmpty() ) pE->m_strFileName = strPath;
            pE->ReportError( MB_OK | MB_ICONSTOP );
	        pE->Delete();  	        
			return 0;
		  }
	     catch( CException* pE )
		  {	  
            pE->ReportError( MB_OK | MB_ICONEXCLAMATION );
	        pE->Delete();  
            return 0;
		  } 
	   }
	}
   catch( CArchiveException* pE )
	{	  		  
	  if( pE->m_strFileName.IsEmpty() ) pE->m_strFileName = GLOBAL_USERS_FILENAME;
      pE->ReportError( MB_OK | MB_ICONEXCLAMATION );
	  pE->Delete();  
	}
   catch( CFileException* pE )
	{
      if( pE->m_strFileName.IsEmpty() ) pE->m_strFileName = GLOBAL_USERS_FILENAME;
      pE->ReportError( MB_OK | MB_ICONSTOP );
	  pE->Delete();  	        
	}
   catch( CException* pE )
	{	  
      pE->ReportError( MB_OK | MB_ICONEXCLAMATION );
	  pE->Delete();  
	} 

   if( f.m_hFile != (UINT)CFile::hFileNull ) f.Close();

   TUser us;
   TUserLocal *pFndUs;
   int iResFnd = FindUser( us, &pFndUs, false );
   TAutoMemoryRc<TUserLocal*> arcFnd( pFndUs );

   if( iResFnd == 0 ) return 0;
   if( iResFnd == 2 ) 
	{
      m_protAcessLvl + *arcFnd();
      arcFnd.ClearRc();
	  return 1;
	}

   for( int i = 0; i < m_optOptions.d.m_iMaxRepass; i++ )
	{
      TPasswDlg dlg( this );

	  if( dlg.DoModal() == IDCANCEL ) return 0;

	  us.SetPassword( dlg.GetPassw() );
	  int iResFnd = FindUser( us, &pFndUs, false );
      TAutoMemoryRc<TUserLocal*> arcFnd( pFndUs );

	  if( iResFnd == 0 ) return 0;
	  if( iResFnd == 2 ) 
	   {
         m_protAcessLvl + *arcFnd();
         arcFnd.ClearRc();
	     return 1;
	   }

	  int iR = ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( m_hWnd, 
	        "Неверный пароль. Повторить попытку ?", 
		    "Ошибка", MB_YESNO|MB_ICONSTOP );
	  if( iR == IDNO ) return 0;
	}

   return 0;
 }

int TMainFrame::FindUser( TUser& rUsr, TUserLocal** pRet, bool bFlAsk )//0-отказ,1-повтор,2-разрешить
 {    
    CWaitCursor wCursor;

    *pRet = NULL;

    TCodingBAKMirrorFile f(
	   m_optOptions.d.m_bUseCoding, 
	   m_optOptions.d.codUserFile,
	   m_optOptions.d.m_cKey
	 );

    CString strPath( CString(((TExpertApp*)AfxGetApp())->GetStartPath()) + CString(GLOBAL_USERS_FILENAME) );
    
	
	CFileException except;
    try {
      if( !f.Open((LPCSTR)strPath, CFile::shareExclusive | CFile::modeRead, &except) )
	     AfxThrowFileException( except.m_cause, except.m_lOsError, (LPCSTR)strPath );	  	  
	 }
	catch( CFileException* pE )
	{
      if( pE->m_strFileName.IsEmpty() ) pE->m_strFileName = strPath;
      pE->ReportError( MB_OK | MB_ICONSTOP );
	  pE->Delete();  	 
	  return 0;
	}
    catch( CException* pE )
	 {	  
       pE->ReportError( MB_OK | MB_ICONEXCLAMATION );
	   pE->Delete();  
	   return 0;
	 } 	
	   
	CArchive arch( &f, CArchive::load );
	

	//TUserLocal  usrCfg;
	TAutoMemoryRc<TUserLocal*> arcCfg( new TUserLocal() );

	TAcessLevel::TUserClass uClass;
    TAcessLevel::TUserSubclass usSubClass;
    rUsr.GetRights( uClass, usSubClass );

	int iCmp = FALSE;
	while( true )
	 {
	   try {
         arch >> *arcCfg();      
		}
	   catch( CArchiveException* pE )
		{	  
		  if( pE->m_cause == CArchiveException::endOfFile )
		   {
             pE->Delete();  
			 break;
		   }
		  else
		   {
		     if( pE->m_strFileName.IsEmpty() ) pE->m_strFileName = strPath;
             pE->ReportError( MB_OK | MB_ICONEXCLAMATION );
	         pE->Delete();  
		     return 0;
		   }
		}
       catch( CFileException* pE )
		{
          if( pE->m_strFileName.IsEmpty() ) pE->m_strFileName = strPath;
          pE->ReportError( MB_OK | MB_ICONSTOP );
	      pE->Delete();  	
		  return 0;
		}
	   catch( CException* pE )
		{	  		  
          pE->ReportError( MB_OK | MB_ICONEXCLAMATION );
	      pE->Delete();  
		  return 0;
		} 

       if( arcCfg()->CheckCRC() == false )
		{
          ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( m_hWnd, 
	        "Файл пользователей испорчен. Его можно стереть и перегрузить Visyal Expert.", 
		    "Ошибка", MB_OK|MB_ICONSTOP );
	      return 0;
		}
	

	   if( usSubClass == TAcessLevel::TUS_Anonime )
		{
          if( rUsr.SameRights(*arcCfg()) )
		   {
		     if( rUsr.SamePassword(*arcCfg()) ) 
			  {
			    *pRet = arcCfg();
                arcCfg.ClearRc();  
			    return 2;
			  }
			 if( bFlAsk == false )
			  {
			    *pRet = arcCfg();
                arcCfg.ClearRc();  
			    return 1;
			  };
			 int iRes = ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( m_hWnd, 
	           "Ошибочный пароль. Повторить попытку входа ?", 
		       "Ошибка", MB_YESNO | MB_ICONSTOP );
			 if( iRes == IDYES ) 
			  {
			    *pRet = arcCfg();
                arcCfg.ClearRc();  
			    return 1;
			  };
			 return 0;
		   }
		}
	   else if( (iCmp=(rUsr == *arcCfg())) && rUsr.SameRights(*arcCfg()) )
		{
          if( rUsr.SamePassword(*arcCfg()) ) 
		   {
			 *pRet = arcCfg();
             arcCfg.ClearRc();  
			 return 2;
		   }
		  if( bFlAsk == false )
		   {
			 *pRet = arcCfg();
             arcCfg.ClearRc();  
			 return 1;
		   };
		  int iRes = ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( m_hWnd, 
	         "Ошибочный пароль. Повторить попытку входа ?", 
		     "Ошибка", MB_YESNO | MB_ICONSTOP );
		  if( iRes == IDYES ) 
		   {
			 *pRet = arcCfg();
             arcCfg.ClearRc();  
			 return 1;
		   }
		  return 0;
		}	   
	 }
	
   if( iCmp == TRUE )
	{
      ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( m_hWnd, 
	    "Нарушение привилегий доступа", 
		"Ошибка", MB_OK | MB_ICONSTOP );
	  return 0;
	}

   int iRes = ((TExpertApp*)AfxGetApp())->bwcc.BWCCMessageBox( m_hWnd, 
     "Пользователь не зарегистрирован. Добавить его ?", 
     "Вопрос", MB_YESNO | MB_ICONQUESTION );
   if( iRes == IDYES ) 
	{      
  	  try {
	    arch.Close();
	    f.Close();	  
	  
	    CFileException except;
        if( !f.Open((LPCSTR)strPath, CFile::shareExclusive | CFile::modeWrite, &except) )
	      AfxThrowFileException( except.m_cause, except.m_lOsError, (LPCSTR)strPath );
		
		f.Seek( 0,CFile::end);
		CArchive archW( &f, CArchive::store );
	  
	    TAutoMemoryRc<TUserLocal*> arcAdm( new TUserLocal(rUsr) ); 	
	    archW << *arcAdm();
		arcCfg = arcAdm(); arcAdm.ClearRc();
	  }
     catch( CArchiveException* pE )
	  {	  	
	    if( pE->m_strFileName.IsEmpty() ) pE->m_strFileName = strPath;
        pE->ReportError( MB_OK | MB_ICONEXCLAMATION );
	    pE->Delete();  
		return 0;
	  }
	 catch( CFileException* pE )
	  {
        if( pE->m_strFileName.IsEmpty() ) pE->m_strFileName = strPath;
        pE->ReportError( MB_OK | MB_ICONSTOP );
	    pE->Delete();  	
		return 0;
	  }
	 catch( CException* pE )
	  {	  
        pE->ReportError( MB_OK | MB_ICONEXCLAMATION );
	    pE->Delete();  
		return 0;
	  } 
	}

   *pRet = arcCfg();
   arcCfg.ClearRc();

   return 2;
 }
