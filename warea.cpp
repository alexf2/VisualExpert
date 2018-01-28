#include "warea.hpp"

#include <afxdao.h>


int TWAException::GetSubCode() const
 {
   return TSC_Subcode;
 }

TWAException::TWAException( BOOL bFlDel, int iExcCode, LPSTR lpName ):
   TMyException( iExcCode, bFlDel )
 { 
   static char *cExMsg[] =
	{
      "Неизвестное исключение",	  
	  "Для базы правил [%s] требуется база вопросов",
	  "Для базы правил [%s] требуется база фактов"
	};

   if( iExcCode == WA_Unknown )
	 m_strMsg.Format( cExMsg[WA_Unknown] );

   else if( iExcCode == WA_NeedQBase )
	 m_strMsg.Format( cExMsg[WA_NeedQBase], lpName );

   else if( iExcCode == WA_NeedFBase )
	 m_strMsg.Format( cExMsg[WA_NeedFBase], lpName );   
 }

void ThrowNeedQBase( LPCSTR lpProd )
 {
   TWAException *pE = new TWAException( true, 
	     WA_NeedQBase, (LPSTR)lpProd );
   throw pE;
 }

void ThrowNeedFBase( LPCSTR lpProd )
 {
   TWAException *pE = new TWAException( true, 
	     WA_NeedFBase, (LPSTR)lpProd );
   throw pE;
 }


TWorkArea::~TWorkArea()
 {
   ClearV();
   ClearVarTree();
 }

void TWorkArea::ClearV()
 {
   vector<TSimpleProduct*>::iterator itSta( m_vecProd.begin() );
   vector<TSimpleProduct*>::iterator itEnd( m_vecProd.end() );

   for( ; itSta != itEnd; ++itSta )
	 delete *itSta;
   m_vecProd.clear();

   vector<TFactWork*>::iterator itSta2( m_vecWork.begin() );
   vector<TFactWork*>::iterator itEnd2( m_vecWork.end() );

   for( ; itSta2 != itEnd2; ++itSta2 )
	 delete *itSta2;
   m_vecWork.clear();

   vector<TQWork*>::iterator itSta3( m_vecQ.begin() );
   vector<TQWork*>::iterator itEnd3( m_vecQ.end() );

   for( ; itSta3 != itEnd3; ++itSta3 )
	 delete *itSta3;
   m_vecQ.clear();

   m_arcNam.FreeRc();
 }

void TWorkArea::Create()
 { 
   CWaitCursor wt;

   if( !(m_pProdDescr->m_arcQ()) ) ThrowNeedQBase( m_pProdDescr->GetName() );
   if( !(m_pProdDescr->m_arcF()) ) ThrowNeedFBase( m_pProdDescr->GetName() );

   m_arcNam = new TNameSpace(); 

   CString str;   
   m_pES->GetFullObjPath( str, m_pProdDescr );

   m_pProdDescr->CheckDBFormat( str );

   CDaoDatabase db;
   db.Open( str );   

   CDaoTableDef tb( &db );
   tb.Open( m_pProdDescr->MyName() );

   CDaoRecordset rset( &db );
   rset.Open( &tb ), rset.MoveFirst();

   while( !rset.IsEOF() )
	{
      TAutoMemoryRc<TSimpleProduct*> arcProd = 
	    new TSimpleProduct( m_arcNam(), 1 );

      COleVariant varPos, varImpl, varCmnt;
	  rset.GetFieldValue( 1, varPos ), rset.GetFieldValue( 2, varImpl ), rset.GetFieldValue( 3, varCmnt );

	  arcProd()->Create( V_BSTRT(&varPos ), V_BSTRT(&varImpl ), V_BSTRT(&varCmnt), m_arcNam() );	  

	  TSimpleProduct *p = arcProd(); arcProd.ClearRc();
	  m_vecProd.push_back( p );

	  rset.MoveNext();
	}

   rset.Close(), tb.Close(), db.Close();


   m_pES->GetFullObjPath( str, m_pProdDescr->m_arcQ() );

   m_pProdDescr->m_arcQ()->CheckDBFormat( str );
   
   db.Open( str );      
   tb.Open( m_pProdDescr->m_arcQ()->MyName() );   
   rset.Open( &tb ), rset.MoveFirst();

   while( !rset.IsEOF() )
	{
      TAutoMemoryRc<TQWork*> arcProd = new TQWork();

      COleVariant varVar, varCmnt;
	  rset.GetFieldValue( 1, varVar ), rset.GetFieldValue( 2, varCmnt );

	  arcProd()->Create( V_BSTRT(&varVar), V_BSTRT(&varCmnt), m_arcNam() );	  
	  //  void Create( LPCSTR lpQ, LPCSTR lpCmnt, TNameSpace* pNamSpc );

	  TQWork *p = arcProd(); arcProd.ClearRc();
	  m_vecQ.push_back( p );

	  rset.MoveNext();
	}

   rset.Close(), tb.Close(), db.Close();


   m_pES->GetFullObjPath( str, m_pProdDescr->m_arcF() );

   m_pProdDescr->m_arcF()->CheckDBFormat( str );
   
   db.Open( str );      
   tb.Open( m_pProdDescr->m_arcF()->MyName() );   
   rset.Open( &tb ), rset.MoveFirst();

   while( !rset.IsEOF() )
	{
      TAutoMemoryRc<TFactWork*> arcProd = new TFactWork();

      COleVariant varVar, varVal, varCmnt;
	  rset.GetFieldValue( 1, varVar ), rset.GetFieldValue( 2, varVal ), rset.GetFieldValue( 3, varCmnt );

	  arcProd()->Create( V_BSTRT(&varVar), V_BSTRT(&varVal), V_BSTRT(&varCmnt), m_arcNam() );	  
	  //void Create( LPCSTR lpVar, LPCSTR lpVal, LPCSTR lpCmnt, TNameSpace *pSpc );

	  TFactWork *p = arcProd(); arcProd.ClearRc();
	  m_vecWork.push_back( p );

	  rset.MoveNext();
	}

   rset.Close(), tb.Close(), db.Close();
 }


void TWorkArea::ClearVarTree()
 { 
   /*list<TTreeVar*>::iterator itSta( m_lstTrees.begin() ); 
   list<TTreeVar*>::iterator itEnd( m_lstTrees.end() ); 

   for( ; itSta != itEnd; itSta++ )
	 delete *itSta;

   m_lstTrees.clear();*/
 }


