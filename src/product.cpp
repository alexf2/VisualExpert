#include "product.hpp"

#include <algorithm>

void ThrowDisabledEQU( LPCSTR lpProd )
 {
   TSPException *pE = new TSPException( true, 
	     SPE_DisblEQU, (LPSTR)lpProd );
   throw pE;
 }

TExpressionFram *TSimpleProduct::FindVarInPos( TVariantVar *pV, TExpression*& pExpr )
 {
   vector<TExpressionFram*>::iterator itSta( m_vecPos.begin() );
   vector<TExpressionFram*>::iterator itEnd( m_vecPos.end() );

   for( ; itEnd != itSta; ++itSta )
	 if( (*itSta)->m_arcExpr()->FindVar(pV) )
	  {
        pExpr = (*itSta)->m_arcExpr();
		return *itSta;
	  }	

   if( (DWORD)pV != (DWORD)m_pImplVar && m_arcImpl()->FindVar(pV) )
	{
      pExpr = m_arcImpl();
	  return NULL;
	}

   pExpr = NULL;
   return NULL;
 }

void TExpressionFram::Create( LPCSTR lpExpr, TNameSpace *pSpc, double dKD )
 {
   m_strExpr = lpExpr;
   m_arcExpr = new TExpression( lpExpr, *pSpc );
   m_dKD = dKD;

   TNameSpace nsp;
   TExpression exp( (LPCSTR)lpExpr, nsp );
   exp.InitParse();

   YYSTYPE yys;
   YYLTYPE yyl;
   memset( &yys, 0 , sizeof(YYSTYPE) );
   memset( &yyl, 0 , sizeof(YYLTYPE) );
   int iEQCnt = 0;
   int iRes;
   while( iRes = exp.Lexer(&yys, &yyl) )
	 if( iRes == '=' ) iEQCnt++;

   if( iEQCnt ) ThrowDisabledEQU( lpExpr );

   m_arcExpr()->Parse( true );
 }


TSimpleProduct::~TSimpleProduct()
 {   
   ClearVec();
 }

void TSimpleProduct::ClearVec()
 {
   vector<TExpressionFram*>::iterator itSta( m_vecPos.begin() );
   vector<TExpressionFram*>::iterator itEnd( m_vecPos.end() );

   for( ; itSta != itEnd; ++itSta )
	 delete *itSta;

   m_vecPos.clear();
 }



void ThrowKDRange( LPCSTR lpProd )
 {
   TSPException *pE = new TSPException( true, 
	     SPE_KDRange, (LPSTR)lpProd );
   throw pE;
 }


void ThrowNeedVarName( LPCSTR lpProd )
 {
   TSPException *pE = new TSPException( true, 
	     SPE_NeedVarName, (LPSTR)lpProd );
   throw pE;
 }

void ThrowBadQList( LPCSTR lpProd )
 {
   TSPException *pE = new TSPException( true, 
	     SPE_BadQList, (LPSTR)lpProd );
   throw pE;
 }

void ThrowNeedKDFac( LPCSTR lpProd )
 {
   TSPException *pE = new TSPException( true, 
	     SPE_NeedKDFac, (LPSTR)lpProd );
   throw pE;
 }


void ThrowVarAlradyInit( LPCSTR lpProd )
 {
   TSPException *pE = new TSPException( true, 
	     SPE_VarAlradyInit , (LPSTR)lpProd );
   throw pE;
 }


void ThrowInvalidValue( LPCSTR lpProd )
 {
   TSPException *pE = new TSPException( true, 
	     SPE_InvalValue , (LPSTR)lpProd );
   throw pE;
 }

void ThrowNeedKD( LPCSTR lpProd )
 {
   TSPException *pE = new TSPException( true, 
	     SPE_NeedKD, (LPSTR)lpProd );
   throw pE;
 }

void ThrowInvalidKD( LPCSTR lpProd )
 {
   TSPException *pE = new TSPException( true, 
	     SPE_InvalidKD, (LPSTR)lpProd );
   throw pE;
 }

void ThrowNeedEQU( LPCSTR lpProd )
 {
   TSPException *pE = new TSPException( true, 
	     SPE_NeedEQU, (LPSTR)lpProd );
   throw pE;
 }

void ThrowOnlyOneEQU( LPCSTR lpProd )
 {
   TSPException *pE = new TSPException( true, 
	     SPE_OnlyOneEQU, (LPSTR)lpProd );
   throw pE;
 }


int TSPException::GetSubCode() const
 {
   return TSC_Subcode;
 }

TSPException::TSPException( BOOL bFlDel, int iExcCode, LPSTR lpName ):
   TMyException( iExcCode, bFlDel )
 { 
   static char *cExMsg[] =
	{
      "Неизвестное исключение",	  
	  "В [%s] требуется коэффициент доверия (ЕСЛИ пос. ТО закл.:0.7)",
	  "В [%s] ошибочный коэффициент доверия",
	  "В импликации [%s] требуется присвоение (имя_переменной=выражение)",
	  "В импликации [%s] слишком много присвоений. Допустимо только одно (имя_переменной=выражение)",
	  "В факте [%s] требуется коэффициент доверия (A = 5:0.7)",
	  "Ошибочное значение [%s]",
	  "Переменная \"%s\" уже инициализирована",
	  "Ошибка в списке переменных-целей [%s]. Нужно: имя1,имя2,..,имяN",
	  "Ошибочное значение [%s]. Требуется имя переменной",
	  "%s",
	  "В [%s] имеется присвоение. В посылках это недопустимо."
	};      

   if( iExcCode == SPE_Unknown )	
	 m_strMsg.Format( cExMsg[SPE_Unknown] ); 

   else if( iExcCode == SPE_NeedKD )	
	 m_strMsg.Format( cExMsg[SPE_NeedKD], lpName );      

   else if( iExcCode == SPE_InvalidKD )	
	 m_strMsg.Format( cExMsg[SPE_InvalidKD], lpName );         

   else if( iExcCode == SPE_NeedEQU )	
	 m_strMsg.Format( cExMsg[SPE_NeedEQU], lpName );         

   else if( iExcCode == SPE_OnlyOneEQU )	
	 m_strMsg.Format( cExMsg[SPE_OnlyOneEQU], lpName );         

   else if( iExcCode == SPE_NeedKDFac )	
	 m_strMsg.Format( cExMsg[SPE_NeedKDFac], lpName );         

   else if( iExcCode == SPE_InvalValue )	
	 m_strMsg.Format( cExMsg[SPE_InvalValue], lpName );         

   else if( iExcCode == SPE_VarAlradyInit )	
	 m_strMsg.Format( cExMsg[SPE_VarAlradyInit], lpName );         

   else if( iExcCode == SPE_BadQList )	
	 m_strMsg.Format( cExMsg[SPE_BadQList], lpName );         

   else if( iExcCode == SPE_NeedVarName )	
	 m_strMsg.Format( cExMsg[SPE_NeedVarName], lpName );         

   else if( iExcCode == SPE_KDRange )	
	 m_strMsg.Format( cExMsg[SPE_KDRange], lpName );         

   else if( iExcCode == SPE_DisblEQU )	
	 m_strMsg.Format( cExMsg[SPE_DisblEQU], lpName );         
 }

void ValidateKD( double dKD, LPCSTR lp )
 {
   if( dKD < 0 || dKD > 1 )
	{
      CString str;
	  str.Format( "В [%s] ошибочный коэффициент доверия [%1.1f]. Должен быть 0...1", lp, dKD );
	  ThrowKDRange( str );
	}
 }

bool TSimpleProduct::CalcPos()
 {
   vector<TExpressionFram*>::iterator itSta( m_vecPos.begin() );
   vector<TExpressionFram*>::iterator itEnd( m_vecPos.end() );

   for( ; itSta != itEnd; ++itSta )
	{
      TVariantVar val;
	  (*itSta)->m_arcExpr()->Val( val );
	  if( val.m_Bool == false ) return false;
	}

   return true;
 }

void TSimpleProduct::CalcImpl()
 {      
   bool bCanceled = m_pImplVar->m_bCanceled;
   bool bNeed = m_pImplVar->m_bNeed;
   bool bConst = m_pImplVar->m_bConst;
   bool bCanImplicat = m_pImplVar->m_bCanImplicat;
   
   TVariantVar val;
   m_arcImpl()->Val( val );

   m_pImplVar->m_bCanceled = bCanceled;
   m_pImplVar->m_bNeed = bNeed;
   m_pImplVar->m_bConst = bConst;
   m_pImplVar->m_bCanImplicat = bCanImplicat;
   
   vector<TExpressionFram*>::iterator itSta( m_vecPos.begin() );
   vector<TExpressionFram*>::iterator itEnd( m_vecPos.end() );
   double dMul = 1;
   for( ; itSta != itEnd; ++itSta )
	 dMul = min( dMul, (*itSta)->m_arcExpr()->GetMulKD(NULL) );

   m_pImplVar->m_dKD = m_dKDImpl * min( dMul, m_arcImpl()->GetMulKD(m_pImplVar) );
 }

void TSimpleProduct::Create( LPCSTR lpPos, LPCSTR lpImpl, LPCSTR lpCmnt, TNameSpace *pSpc )
 {
   ClearVec();

   m_pNamSpc = pSpc;
   m_strComment = lpCmnt;   

   m_strImpl = lpImpl;
   LPSTR lpLast = strrchr( (LPSTR)(LPCSTR)m_strImpl, ':' );
   if( !lpLast )  ThrowNeedKD( m_strProd );
   else
	{      
      TVariantVar vVar;  
	  if( !ReadConstant(vVar, lpLast+1) ) ThrowInvalidKD( m_strProd );
	  if( vVar.m_iType != VAR_Int && vVar.m_iType != VAR_Double ) ThrowInvalidKD( m_strProd );

	  m_dKDImpl = (vVar.m_iType == VAR_Int ? vVar.m_Int:vVar.m_Double);

	  ValidateKD( m_dKDImpl, m_strProd );

	  m_strImpl = m_strImpl.Left( (DWORD)lpLast - (DWORD)((LPSTR)(LPCSTR)m_strImpl) );
	}

   m_strProd.Format( "ЕСЛИ %s ТО %s :%1.2f", lpPos, 
	 (LPCSTR)m_strImpl, double(m_dKDImpl) );

   TNameSpace nsp;
   TExpression exp( (LPCSTR)m_strImpl, nsp );
   exp.InitParse();

   YYSTYPE yys;
   YYLTYPE yyl;
   memset( &yys, 0 , sizeof(YYSTYPE) );
   memset( &yyl, 0 , sizeof(YYLTYPE) );
   int iEQCnt = 0;
   int iRes;
   while( iRes = exp.Lexer(&yys, &yyl) )
	 if( iRes == '=' ) iEQCnt++;

   if( !iEQCnt ) ThrowNeedEQU( m_strImpl );
   if( iEQCnt != 1 ) ThrowOnlyOneEQU( m_strImpl );

   m_arcImpl = new TExpression( (LPCSTR)m_strImpl, *pSpc );
   m_arcImpl()->Parse( false );
   //TVariantVar *pImplVar = m_arcImpl()->GetEQVar();
   m_pImplVar = m_arcImpl()->GetEQVar();
   m_pImplVar->m_bCanImplicat = true;

   m_strPos = lpPos;
   LPSTR lpSta = (LPSTR)lpPos;
   LPSTR lpPrev = (LPSTR)lpPos;
   bool bFlFnd = false;
   bool bFlF = true;
   for( ; *lpPos; lpPos++ )
	 if( *lpPos == ';' )
	  {
	    bFlFnd = true;

        TAutoMemoryRc<TExpressionFram*> arcFr( new TExpressionFram() );
		CString str = m_strPos.Mid( 
		  (DWORD)lpPrev - (DWORD)lpSta + (bFlF == true ? 0:1),
		  (DWORD)lpPos - (DWORD)lpPrev - (bFlF == true ? 0:1));
		arcFr()->Create( str, pSpc, 1.0 );	
		
		lpPrev = (LPSTR)lpPos;
		bFlF = false;

		TExpressionFram* pTmp = arcFr(); arcFr.ClearRc();
		m_vecPos.push_back( pTmp );
	  }

   LPSTR lpp = lpPrev;
   bool bFlSpc = true;
   for( ; lpp != lpPos && *lpp; lpp++ )
     if( *lpp != ';' && *lpp != ' ' ) bFlSpc = false;

   if( bFlSpc == false )
	{
      TAutoMemoryRc<TExpressionFram*> arcFr( new TExpressionFram() );
	  CString str = m_strPos.Mid( (DWORD)lpPrev - (DWORD)lpSta + (bFlFnd == true ? 1:0),
		  (DWORD)lpPos - (DWORD)lpPrev - (bFlFnd == true ? 1:0) );
	  arcFr()->Create( str, pSpc, 1.0 );	
			
	  TExpressionFram* pTmp = arcFr(); arcFr.ClearRc();
	  m_vecPos.push_back( pTmp );
	}
 }

void TFactWork::Create( LPCSTR lpVar, LPCSTR lpVal, LPCSTR lpCmnt, TNameSpace *pSpc )
 {
   m_strFac.Format( "%s = %s", lpVar, lpVal );
   m_strComment = lpCmnt;
   m_strVar = lpVar;

	{
	  TNameSpace nsp;
	  TExpression exp( lpVar, nsp );
	  exp.InitParse();

      YYSTYPE yys;
	  YYLTYPE yyl;
	  memset( &yys, 0 , sizeof(YYSTYPE) );
	  memset( &yyl, 0 , sizeof(YYLTYPE) );
	  int iRes = exp.Lexer( &yys, &yyl );
	  int iRes2 = exp.Lexer( &yys, &yyl );

	  if( iRes != IDENTIFIER || iRes2 ) ThrowNeedVarName( lpVar );
	}

   CString stv = lpVal;

   LPSTR lpLast = strrchr( (LPSTR)(LPCSTR)stv, ':' );
   if( !lpLast )  ThrowNeedKDFac( m_strFac );
   else
	{      
	  TVariantVar vVar;  
	  if( !ReadConstant(vVar, lpLast+1) ) ThrowInvalidKD( m_strFac );
	  if( vVar.m_iType != VAR_Int && vVar.m_iType != VAR_Double ) ThrowInvalidKD( m_strFac );

	  m_dKD = (vVar.m_iType == VAR_Int ? vVar.m_Int:vVar.m_Double);

	  ValidateKD( m_dKD, m_strFac );

	  stv = stv.Left( (DWORD)lpLast - (DWORD)((LPSTR)(LPCSTR)stv) );
	}
  
   TVariantVar vVar;  
   if( !ReadConstant(vVar, stv) ) ThrowInvalidValue( (LPCSTR)stv );   

   TVariantVar* pvarVar = pSpc->FindVar( lpVar );
   if( pvarVar )
	{
      if( pvarVar->m_bFIni == true ) ThrowVarAlradyInit( lpVar );
	  *pvarVar = vVar;

	  m_pVar = pvarVar; 
	  m_pVar->m_dKD = m_dKD;
	}
   else
	{
	  TVariantVar *pV = new TVariantVar( 
	    (TVariantTypes)vVar.m_iType,
		(char*)(LPCSTR)m_strVar );
	   
	  *pV = vVar;
      pSpc->AddVar( pV );

	  m_pVar = pV;
	  m_pVar->m_dKD = m_dKD;
	}
 }

TQWork::~TQWork()
 {
   ClearVec();
 }

void TQWork::ClearVec()
 {
   vector<TVarDescr*>::iterator itSta( m_vecVarQ.begin() );
   vector<TVarDescr*>::iterator itEnd( m_vecVarQ.end() );

   for( ; itSta != itEnd; ++itSta )
	 delete *itSta;

   m_vecVarQ.clear();
 }

void TQWork::Create( LPCSTR lpQ, LPCSTR lpCmnt, TNameSpace* pNamSpc )
 {
   m_strQ = lpQ;
   m_strComment = lpCmnt;
   m_pNamSpc = pNamSpc;

   TNameSpace nsp;
   TExpression exp( lpQ, nsp );
   exp.InitParse();

   YYSTYPE yys;
   YYLTYPE yyl;
   memset( &yys, 0 , sizeof(YYSTYPE) );
   memset( &yyl, 0 , sizeof(YYLTYPE) );

   int iRes;
   while( iRes = exp.Lexer(&yys, &yyl) )
	{
      if( iRes == IDENTIFIER )
	   {
         TAutoMemoryRc<TVarDescr*> arcD = new TVarDescr; 
		 int iSz = abs( yyl.m_iStaClm - yyl.m_iEndClm ) + 1;		  
		 LPSTR lpp = arcD()->m_strName.GetBuffer( iSz + 10 );
		 //memset( lpp, 0, iSz + 1 );
		 strncpy( lpp, (LPSTR)(&lpQ[ yyl.m_iStaClm ]), iSz );

		 arcD()->m_strName.ReleaseBuffer( iSz );

		 arcD()->m_pVar = pNamSpc->FindVar( (LPCSTR)arcD()->m_strName );
		 TVarDescr *pp = arcD();
		 arcD.ClearRc();

		 m_vecVarQ.push_back( pp );
	   }
	  else if( iRes != ',' ) ThrowBadQList( lpQ );
	}   
 }

void CallClrUsed( TExpressionFram *pEF )
 {
   pEF->m_arcExpr()->ClearUsed();
 }

void TSimpleProduct::ClearUsedFlag()
 {
   for_each( m_vecPos.begin(), m_vecPos.end(), CallClrUsed );   
   m_arcImpl()->ClearUsed();
 }
