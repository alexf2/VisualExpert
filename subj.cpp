#include "subj.hpp"


TSubject::TSubject(): TProtectedObj()
 {
   m_fDirty = false;
   m_bSelected = false;
 }
  

TSubject::TSubject( TProtectedObj *pPar, LPCSTR lpShortPath ):
  TProtectedObj( pPar ),
  m_csSPath( lpShortPath )
 {
   m_fDirty = false;
   m_bSelected = false;
 }
  

TSubject::~TSubject()
 {
 }

/*TP_ClassTyp TSubject::IsA() const
 {
   return TPC_TSubject;
 }*/

void TSubject::ImportFile( LPCSTR lpToPath, LPCSTR lpFromPath )
 {
   if( !_stricmp(lpToPath, lpFromPath) ) return;
   if( !::CopyFile(lpFromPath, lpToPath, TRUE) )
	{
	  int iErr = ::GetLastError();
	  CFileException *pError = new CFileException();

      pError->m_lOsError    = iErr;
      pError->m_cause       = CFileException::OsErrorToException( pError->m_lOsError );
      pError->m_strFileName = lpFromPath;

	  throw pError;
	}
 }

bool TSubject::RemoveObj( LPCSTR lpPathES, int iAction )
 { 
   if( iAction == IDNO ) return true;
   CString str = CString( lpPathES ) + CString( m_csSPath );
   if( !::DeleteFile(str) )
	{
      int iErr = ::GetLastError();
	  CFileException *pError = new CFileException();

      pError->m_lOsError    = iErr;
      pError->m_cause       = CFileException::OsErrorToException( pError->m_lOsError );
      pError->m_strFileName = str;

	  throw pError;
	}

   return true;
 }
