#include "creator.hpp"
#include "mfrexp.hpp"

#include <io.h>

bool CheckExist( LPCSTR strName )
 {
   _finddata_t fnd;
   memset( &fnd, 0, sizeof(_finddata_t) );
   fnd.attrib = _A_ARCH | _A_SYSTEM | _A_HIDDEN | _A_RDONLY | _A_NORMAL;

   long lHandl = _findfirst( (LPSTR)(LPCSTR)strName, &fnd );
   if( lHandl != -1 ) 
	{
      _findclose( lHandl );
	  if( !(fnd.attrib & _A_SUBDIR) )  return true;	   
	}
   
   return false;
 }

TSubject* CreateMgr( TC_Creation tcObj, LPCSTR lpFileNam, LPCSTR lpPath, LPCSTR lpName )
 {
   TAutoMemoryRc<TSubject*> arcSubj;

   TProtectedObj* pPar = ((TMainFrame*)(AfxGetApp()->m_pMainWnd))->GetProtectLvl();	

   switch( tcObj )
	{
	  case TC_Rules:
	    arcSubj = new TPro( pPar, lpFileNam );
		break;
	  case TC_Facts:
	    arcSubj = new TFac( pPar, lpFileNam );
		break;
	  case TC_Quest:
	    arcSubj = new TQuery( pPar, lpFileNam );
		break;
	  case TC_Net:
	    arcSubj = new TNet( pPar, lpFileNam );
		break;
	  case TC_Frames:
	    arcSubj = new TFrm( pPar, lpFileNam );
		break;
	};   

   arcSubj()->SetName( lpName );
   arcSubj()->CreateEmtyFile( CString(lpPath) + CString(lpFileNam)  );

   TSubject* pSubj = arcSubj(); arcSubj.ClearRc();
   return pSubj;
 }

void Create_CREATOR_Null( LPCSTR lp )
 {
   CFileException except;
   CFile f;
   if( !f.Open(lp, CFile::modeCreate|CFile::modeReadWrite|CFile::shareDenyRead, &except) )	
      AfxThrowFileException( except.m_cause, except.m_lOsError, lp );	  	  	

   f.Close();
 }

TSubject* ImportMgr( TC_Creation tcObj, LPCSTR lpFileNam, LPCSTR lpPath, LPCSTR lpName, LPSTR lpImportPath )
 {
   TAutoMemoryRc<TSubject*> arcSubj;

   TProtectedObj* pPar = ((TMainFrame*)(AfxGetApp()->m_pMainWnd))->GetProtectLvl();	

   switch( tcObj )
	{
	  case TC_Rules:
	    arcSubj = new TPro( pPar, lpFileNam );
		break;
	  case TC_Facts:
	    arcSubj = new TFac( pPar, lpFileNam );
		break;
	  case TC_Quest:
	    arcSubj = new TQuery( pPar, lpFileNam );
		break;
	  case TC_Net:
	    arcSubj = new TNet( pPar, lpFileNam );
		break;
	  case TC_Frames:
	    arcSubj = new TFrm( pPar, lpFileNam );
		break;
	};   

   arcSubj()->SetName( lpName );
   arcSubj()->ImportFile( CString(lpPath) + CString(lpFileNam),  
     CString(lpImportPath) + CString(lpFileNam) );

   TSubject* pSubj = arcSubj(); arcSubj.ClearRc();
   return pSubj;
 }
