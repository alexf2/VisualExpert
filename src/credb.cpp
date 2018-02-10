#include "credb.hpp"
#include <odbcinst.h>



void CreateTable( CDaoTableDef& rTbl, TMyFieldInfo *pFinf )
 {
   for( ; pFinf->m_lSize != -1; pFinf++ )	
     rTbl.CreateField( pFinf->m_strName, pFinf->m_nType, 
	   pFinf->m_lSize, pFinf->m_lAttributes );
 }

void CreateMyDB( LPCSTR lp, LPCSTR lpName, TProtectedObj *pObj, TMyFieldInfo *pClm )
 {
   /*CDaoWorkspace wsp;
   CString strNam;
   pObj->GetUserID( strNam );
   wsp.Create( "Space", (LPCSTR)strNam, (LPCSTR)pObj->Password() );*/

   CDaoDatabase db;
   db.Create( lp, dbLangCyrillic, dbVersion30 );

   CDaoTableDef tb( &db );
   CString str;    

   CString strNam;
   pObj->GetUserID( strNam );   
   str.Format( "UID=%s;PWD=%s", (LPCSTR)strNam, (LPCSTR)pObj->Password() );

   tb.Create( lpName, dbAttachSavePWD, 0, (LPCSTR)str );

   CreateTable( tb, pClm ); 
   tb.Append();

   CDaoRecordset rset( &db );
   rset.Open( &tb );
   rset.AddNew();

   TMyFieldInfo *pFinf = pClm + 1;    
   COleVariant var( " " );
   rset.SetFieldValue( pClm->m_strName, COleVariant(1L) );
   for( ; pFinf->m_lSize != -1; pFinf++ )	
     rset.SetFieldValue( pFinf->m_strName, var );

   rset.Update();

   db.Close();   
 }

int TFormatException::GetSubCode() const
 {
   return TSC_Subcode;
 }

TFormatException::TFormatException( BOOL bFlDel, int iExcCode, LPSTR lpName ):
   TMyException( iExcCode, bFlDel )
 { 
   static char *cExMsg[] =
	{
      "Ошибочный формат базы данных '%s'",	  
	  "Неизвесная ошибка формата базы данных %s",
	  "%s"
	};   
   
   if( iExcCode == FRM_BadDBFormat )	
	 m_strMsg.Format( cExMsg[FRM_BadDBFormat], lpName ); 
   else if( iExcCode == FRM_Unknown )	
	 m_strMsg.Format( cExMsg[FRM_Unknown], lpName );   
   else if( iExcCode == FRM_SQLErr )	
	 m_strMsg.Format( cExMsg[FRM_SQLErr], lpName ); 
 }

void ThrowBadDBFormat( LPCSTR lpNam )
 {
   TFormatException *pE = new TFormatException( TRUE, 
	 FRM_BadDBFormat, (LPSTR)lpNam );
   throw pE;      
 }

void ThrowSQLErr( LPCSTR lpErr )
 {
   TFormatException *pE = new TFormatException( TRUE, 
	 FRM_SQLErr, (LPSTR)lpErr );
   throw pE;      
 }

void CheckDBFormat( LPCSTR lpPath, LPCSTR lpNameTable, TMyFieldInfo *pFinf, TProtectedObj *pObj )
 {   
   /*CDaoWorkspace wsp;
   CString strNam;
   pObj->GetUserID( strNam );
   wsp.Create( "Space", (LPCSTR)strNam, (LPCSTR)pObj->Password() );*/

   CDaoDatabase db;
   db.Open( lpPath );
	
   CDaoTableDefInfo tab;
   try {
     db.GetTableDefInfo( lpNameTable, tab );
	}
   catch( CDaoException *pE )
	{
      pE->Delete();	  
      ThrowBadDBFormat( (LPSTR)lpPath );
	}   
	
   CDaoTableDef tbl( &db );
   tbl.Open( lpNameTable );

   short sC = tbl.GetFieldCount();    
   for( short sI = 0; sI < sC && pFinf->m_lSize != -1; ++sI, ++pFinf )
	{
	  CDaoFieldInfo fInf;
      tbl.GetFieldInfo( sI, fInf );

	  if( fInf.m_strName.CompareNoCase(pFinf->m_strName) ||
	      fInf.m_nType != pFinf->m_nType 		  
		)
	      ThrowBadDBFormat( (LPSTR)lpPath );	   
	}

   if( sC != sI  ) ThrowBadDBFormat( lpPath );
 }


void CheckSQLCode( int retcode )
 {
   if( retcode == TRUE ) return;
	
   DWORD dwCod;
   unsigned short dwOutSiz;
   CString str;
   LPSTR lp = str.GetBuffer( 1024 );
   int iRes = SQLInstallerError( 1, &dwCod, lp, 1020, &dwOutSiz );

   if( iRes == SQL_ERROR )
	  ThrowSQLErr( "Двойная ошибка SQL" );
   else if( iRes == SQL_NO_DATA )
	  ThrowSQLErr( "Недоступна информация об ошибке SQL" );

	
   ThrowSQLErr( lp );
 }

void RegisterSource( LPCSTR lpPath, LPCSTR lpNameSrc, bool bFlRemove )
 {
   LPSTR   szAttributes =
       "DSN=\0"
	   "FIL=MS Access\0"
	   "JetIniPath=odbcddp.ini\0"

       "DBQ=f:\\mydb.mdb\0"
       "DefaultDir=f:\\\0"
       "Exclusive=0\0";

   CString strDSN;
   strDSN.Format( "DSN=%s", lpNameSrc );
   CString strDBQ;
   strDBQ.Format( "DBQ=%s", lpPath );

   char path_buffer[ _MAX_PATH ];
   char drive[ _MAX_DRIVE ];
   char dir[ _MAX_DIR ];
   char fname[ _MAX_FNAME ];
   char ext[ _MAX_EXT ];

   _splitpath( lpPath, drive, dir, fname, ext );
   _makepath( path_buffer, drive, dir, "", "" );
   if( path_buffer[ strlen(path_buffer) - 1 ] != '\\' )
	 strcat( path_buffer, "\\" );

   CString strDefDir;
   strDefDir.Format( "DefaultDir=%s", path_buffer );

   CString strStr;
   LPSTR lp = strStr.GetBuffer( 1024 );
   memset( lp, 0, 1024 );
   LPSTR lp2 = lp;

   memcpy( lp2, (LPCSTR)strDSN, strDSN.GetLength() );
   lp2 += strDSN.GetLength() + 1;

   memcpy( lp2, "FIL=MS Access", strlen("FIL=MS Access") );
   lp2 += strlen("FIL=MS Access") + 1;

   memcpy( lp2, "JetIniPath=odbcddp.ini", strlen("JetIniPath=odbcddp.ini") );
   lp2 += strlen("JetIniPath=odbcddp.ini") + 1;

   memcpy( lp2, (LPCSTR)strDBQ, strDBQ.GetLength() );
   lp2 += strDBQ.GetLength() + 1;

   memcpy( lp2, (LPCSTR)strDefDir, strDefDir.GetLength() );
   lp2 += strDefDir.GetLength() + 1;

   memcpy( lp2, "Exclusive=0", strlen("Exclusive=0") );
   lp2 += strlen("Exclusive=0") + 1;

   int retcode = SQLConfigDataSource( NULL,
	  bFlRemove == true  ? ODBC_REMOVE_DSN:ODBC_ADD_DSN,
     "Microsoft Access Driver (*.mdb)",
     lp );

   CheckSQLCode( retcode );
 }

TTempDSN::~TTempDSN()
 {
   Remove();
 }

void TTempDSN::Remove()
 {
   if( m_bFlInit == true )
	{
	  m_bFlInit = false;
      RegisterSource( (LPCSTR)m_strPath, (LPCSTR)m_strDSN, true );	
	}
 }

void TTempDSN::Create( LPCSTR lpPath, LPCSTR lpNameSrc )
 {
   m_strPath = lpPath;
   m_strDSN = lpNameSrc;

   RegisterSource( lpPath, lpNameSrc, false );	

   m_bFlInit = true;
 }
