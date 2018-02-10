#include "var.hpp"
#include <math.h>
#include <float.h>

#include <strstream>
#include <iomanip>
#include <algorithm>


BOOL ReadConstant( TVariantVar& rV, LPCSTR lpStr )
 {
   TNameSpace nsp;
   TExpression exp( lpStr, nsp );
   exp.InitParse();

   YYSTYPE yys[3];
   YYLTYPE yyl[3];
   memset( yys, 0 , sizeof(YYSTYPE)*3 );
   memset( yyl, 0 , sizeof(YYLTYPE)*3 );
   int iRes = exp.Lexer( &yys[0], &yyl[0] );
   int iRes2 = 0, iRes3 = 0;
   if( iRes ) iRes2 = exp.Lexer( &yys[1], &yyl[1] );
   if( iRes2 ) iRes3 = exp.Lexer( &yys[2], &yyl[2] );


   if( iRes3 ) return FALSE;
   if( !iRes2 )
	{
      if( iRes != STRING_CONSTANT &&
		  iRes != CHARACTER_CONSTANT &&
		  iRes != BOOL_CONSTANT &&
		  iRes != FLOAT_CONSTANT &&
		  iRes != INTEGER_CONSTANT
	    ) return FALSE;

	  exp.Simplify_Constant( yys[0].m_iTokenType, &yys[1], &yys[0], &yyl[0] );
	  rV = yys[1].m_TokenInfo.m_pNode->Val();

	  return TRUE;
	}

   if( iRes != '-' && iRes != '+' ) return FALSE;

   if( iRes2 != INTEGER_CONSTANT && iRes2 != FLOAT_CONSTANT ) return FALSE;

   exp.Simplify_Constant( yys[1].m_iTokenType, &yys[2], &yys[1], &yyl[1] );
   rV = yys[2].m_TokenInfo.m_pNode->Val();
   if( iRes == '-' ) 
	{
      TVariantVar v( -1 );
	  rV = rV * v;
	}

   return TRUE;
 }


int TVarException::GetSubCode() const
 {
   return TSC_Subcode;
 }

TVarException::TVarException( BOOL bFlDel, int iExcCode, const TVariantVar* pV1, 
							  const TVariantVar* pV2 ):
  TMyException( iExcCode, bFlDel )
 {
   static char *cExMsg[] =
	{
      "Переменная %s(%s) не инициализирована",
	  "Недопустимое приведение типов в операции [%s(%s)] %s [%s(%s)]",
	  "Переменная %s не связана с типом",
	  "Неизвесная ошибка операции с переманными",
	  "Операция '%s' запрещена для переменной %s(%s)"	  
	};

   p1 = (TVariantVar*)pV1, p2 = (TVariantVar*)pV2;
   

   if( iExcCode == VEX_NotInitialized )
	{
	  m_strMsg.Format( cExMsg[VEX_NotInitialized], (LPCSTR)pV1->m_strName, 
	                   (LPCSTR)pV1->GetTypeStr() ); 
	}
   else if( iExcCode == VEX_BadType )
	{
	  m_strMsg.Format( cExMsg[VEX_BadType], (LPCSTR)pV1->m_strName, 
	                   (LPCSTR)pV1->GetTypeStr(), (LPCSTR)pV1->GetKeyOPStr(),
					   (LPCSTR)pV2->m_strName, (LPCSTR)pV2->GetTypeStr()  );
	}	
   else if( iExcCode == VEX_NoID )
	{
	  m_strMsg.Format( cExMsg[VEX_NoID], (LPCSTR)pV1->m_strName );
	}
   else if( iExcCode == VEX_Unknown ) 
	 m_strMsg.Format( cExMsg[VEX_Unknown], (LPCSTR)pV1->m_strName );
   else if( iExcCode == VEX_OperDisabledForTyp )
	 m_strMsg.Format( cExMsg[VEX_OperDisabledForTyp], (LPCSTR)pV1->GetKeyOPStr(),
	                  (LPCSTR)pV1->m_strName, (LPCSTR)pV1->GetTypeStr() );   
 }



LPSTR TVariantVar::GetTypeStr() const
 {
   switch( m_iType )
	{
	  case VAR_Bool:
	    return "Boolean";
	  case VAR_Char:
	    return "Char";
	  case VAR_Int:
	    return "Integer";
	  case VAR_Double:
	    return "Double";
	  case VAR_Str: 
	    return "String";

	  case VAR_Undef:      
	    return "Undefined";      

      default:
	    return "Unknown";
	};
 }

struct TOpInf
 {
   int   m_iCod;
   char  *m_pStr;
 };

LPSTR TVariantVar::GetKeyOPStr() const
{
   TOpInf inf[] = 
	{
	 { TVariantVar::UN_MINUS, "Unar -" },
	 { TVariantVar::UN_NE, "!" },

	 { TVariantVar::OP_PLUS, "+" },
	 { TVariantVar::OP_MINUS, "-" },
	 { TVariantVar::OP_MUL, "*" },
	 { TVariantVar::OP_DIV, "/" },
	 { TVariantVar::OP_MOD, "%" },
	 { TVariantVar::OP_OROR, "||" },
	 { TVariantVar::OP_ANDAND, "&&" },

	 { TVariantVar::TP_EQ, "==" },
	 { TVariantVar::TP_NEQ, "!=" },
	 { TVariantVar::TP_LE, "<" },
	 { TVariantVar::TP_GE, ">" },

	 { TVariantVar::TP_LEQ, "<=" },
	 { TVariantVar::TP_GEQ, ">=" },

	 { TVariantVar::OPEQ_EQ, "=" },

     { FTF_MIN, "min" },
	 { FTF_MAX, "max" },
	 { FTF_POW, "pow" },
	 { FTF_FMOD, "mod" },
	 { FTF_CMPI, "Cmpi" },
	 { FTF_SCAS, "Scas" },	 

	 { FTF_SIN, "sin" },
	 { FTF_COS, "cos" },
	 { FTF_TAN, "tan" },
	 { FTF_ATAN, "atan" },
	 { FTF_ASIN, "asin" },
	 { FTF_ACOS, "acos" },
	 { FTF_LOG, "log" },
	 { FTF_LOG10, "log10" },
	 { FTF_EXP, "exp" },
	 { FTF_SQRT, "sqrt" },
	 { FTF_FLOOR, "floor" },
	 { FTF_CEIL, "ceil" },
	 { FTF_Bool, "Bool" },
	 { FTF_Char, "Char"},
     { FTF_Int, "Int" },
	 { FTF_Double, "Double" },
	 { FTF_String, "String" },

	 { -1, NULL }
	};
   
   for( int i = 0; inf[i].m_iCod != -1; i++ )
	 if( inf[i].m_iCod == m_iKeyLastOp ) return inf[i].m_pStr;

   return "Unknown";
 }

TVariantVar::TVariantVar( LPSTR lpStr ):
 m_iType( VAR_Str ),
 m_strName( "_Str_const_" )
 {
   IniFlags();
   m_bFIni = true, m_Str = lpStr;
 }

TVariantVar::TVariantVar( LPCSTR lpStr ):
  m_iType( VAR_Str ),
  m_strName( "_StrC_const_" )
 {
   IniFlags();
   m_bFIni = true, m_Str = lpStr;
 }

TVariantVar::TVariantVar( char ch ):
  m_iType( VAR_Char ),
  m_strName( "_Char_const_" )
 {
   IniFlags();
   m_bFIni = true, m_Char = ch;
 }

TVariantVar::TVariantVar( bool bv ):
  m_iType( VAR_Bool ),
  m_strName( "_Bool_const_" )
 {
   IniFlags();
   m_bFIni = true, m_Bool = bv;
 }

TVariantVar::TVariantVar( int iv ):
  m_iType( VAR_Int ),
  m_strName( "_Int_const_" )
 {
   IniFlags();
   m_bFIni = true, m_Int = iv;
 }

TVariantVar::TVariantVar( double dv ):
  m_iType( VAR_Double ),
  m_strName( "_Double_const_" )
 {
   IniFlags();
   m_bFIni = true, m_Double = dv;
 }

  


TVariantVar::TVariantVar():
  m_strName( "Unnamed" )
 { 
   IniFlags();
   m_iType = VAR_Undef; m_bFIni = false;    
 }

TVariantVar::TVariantVar( TVariantTypes iType, char* lpName ): 
  m_strName( lpName ? lpName:"Unnamed" )
 { 
   IniFlags();
   m_iType = iType; m_bFIni = false;   
 }

TVariantVar::TVariantVar( char* lpStr, int iLen, char* lpName ): 
   m_strName(lpName) 
 {
   IniFlags();

   LPSTR lp = m_Str.GetBuffer( iLen + 2 );
   strncpy( lp, lpStr, iLen );
   *(lp + iLen) = 0;
   m_Str.ReleaseBuffer();

   m_iType = VAR_Str;
   m_bFIni = true; 
 }

void TVariantVar::ThrowNotInit( const TVariantVar& rV )
 {
   TVarException  *pEx = 
	  new TVarException( TRUE, VEX_NotInitialized, &rV );
	
   throw pEx; 	
 }

void TVariantVar::ThrowOperDisabledForTyp( const TVariantVar& rV )
 {
   TVarException  *pEx = 
	  new TVarException( TRUE, VEX_OperDisabledForTyp, &rV );
	
   throw pEx; 	
 }

void TVariantVar::ThrowNotDefined( const TVariantVar& rV )
 {
   TVarException  *pEx = 
	  new TVarException( TRUE, VEX_NoID, &rV );
	
   throw pEx; 	
 }

void TVariantVar::ThrowNotCompatible( const TVariantVar& rV1, const TVariantVar& rV2 )
 {
   TVarException  *pEx = 
	  new TVarException( TRUE, VEX_BadType, &rV1, &rV2 );
	
   throw pEx; 	
 }

int TVariantVar::CheckCompatibleTyp( TCompatible* pCompat, 
									 const TVariantVar& rV1, 
									 const TVariantVar& rV2, bool bMax )
//return -2 - if not, -1 - if yes, TVariantTypes - if need conversion
 {
   if( rV1.m_iType == rV2.m_iType ) return -1;
   
   for( int i = 0; i < NUMB_TYP; i++, pCompat++ )
	  if( pCompat->m_iType == rV1.m_iType ) break;

   for( i = 0; i < NUMB_TYP; i++ )
	 if( pCompat->m_Compat[i] == rV2.m_iType ) 
	   return bMax == true ? 
	            max( rV1.m_iType, rV2.m_iType ):
	            min( rV1.m_iType, rV2.m_iType );
	 else if( pCompat->m_Compat[i] == -1 ) break;
	  
   return -2;
 }

void TVariantVar::TypeCast( TVariantVar& varTmp, const TVariantVar& rV, int iType )
 {
   varTmp.m_iType = iType;

   if( rV.m_iType == iType )
	{	  
      switch( rV.m_iType )
	   {
	     case VAR_Str:
	       varTmp.m_Str = rV.m_Str; return;
	     case VAR_Char:
  	       varTmp.m_Char = rV.m_Char; return;
	     case VAR_Int:
	       varTmp.m_Int = rV.m_Int; return;
	     case VAR_Double:
	       varTmp.m_Double = rV.m_Double; return;
	     case VAR_Bool:
	       varTmp.m_Bool = rV.m_Bool; return;
	   };
	}

   if( rV.m_iType == VAR_Bool )
	{	
	  if( iType == VAR_Bool ) { varTmp.m_Bool = rV.m_Bool; }
	  else if( iType == VAR_Char ) { varTmp.m_Char = rV.m_Bool; }
	  else if( iType == VAR_Int ) { varTmp.m_Int = rV.m_Bool; }
	  else if( iType == VAR_Double ) { varTmp.m_Double = rV.m_Bool; }
	  else ThrowNotCompatible( varTmp, rV );
	}
   else
   if( rV.m_iType == VAR_Char )
	{
	  if( iType == VAR_Char ) { varTmp.m_Char = rV.m_Char; }
	  else if( iType == VAR_Bool ) { varTmp.m_Bool = rV.m_Char; }
	  else if( iType == VAR_Int ) { varTmp.m_Int = rV.m_Char; }
	  else if( iType == VAR_Double ) { varTmp.m_Double = rV.m_Char; }
	  else if( iType == VAR_Str ) { varTmp.m_Str = rV.m_Char; }
	  else ThrowNotCompatible( varTmp, rV );
	} 
   else
   if( rV.m_iType == VAR_Int )
	{
	  if( iType == VAR_Bool ) { varTmp.m_Bool = rV.m_Int; }
	  else if( iType == VAR_Char ) { varTmp.m_Char = rV.m_Int; }
	  else if( iType == VAR_Int ) { varTmp.m_Int = rV.m_Int; }
	  else if( iType == VAR_Double ) { varTmp.m_Double = rV.m_Int; }
	  else ThrowNotCompatible( varTmp, rV );
	}
   else
   if( rV.m_iType == VAR_Double )
	{
	  if( iType == VAR_Bool ) { varTmp.m_Bool = rV.m_Double; }
	  else if( iType == VAR_Char ) { varTmp.m_Char = rV.m_Double; }
	  else if( iType == VAR_Int ) { varTmp.m_Int = rV.m_Double; }
	  else if( iType == VAR_Double ) { varTmp.m_Double = rV.m_Double; }
	  else ThrowNotCompatible( varTmp, rV );
	}
   else
   if( rV.m_iType == VAR_Str )
	{
	  if( iType == VAR_Str ) { varTmp.m_Str = rV.m_Str; }
	  else ThrowNotCompatible( varTmp, rV );
	}
 }


TVariantVar& TVariantVar::operator=( const TVariantVar& rV )
 {
   m_iKeyLastOp = ((TVariantVar&)rV).m_iKeyLastOp = OPEQ_EQ;

   m_dKD = rV.m_dKD;
   m_bCanceled = rV.m_bCanceled;
   m_bNeed = rV.m_bNeed;
   m_bCanImplicat = rV.m_bCanImplicat;
   //m_bConst = rV.m_bConst;

   m_bUsed = false;

   static TCompatible compat[ NUMB_TYP ] = 
	{
	 { VAR_Str, {VAR_Char,-1,-1,-1,-1} },
	 { VAR_Char, {VAR_Bool,VAR_Int,-1,-1,-1} },
	 { VAR_Int, {VAR_Bool,VAR_Char,VAR_Double,-1,-1} },
	 { VAR_Double, {VAR_Bool,VAR_Char,VAR_Int,-1,-1} },
	 { VAR_Bool, {VAR_Char,VAR_Int,VAR_Double,-1,-1} }
	};

   if( rV.m_bFIni == false ) ThrowNotInit( rV );
   if( rV.m_iType == VAR_Undef ) ThrowNotDefined( rV );

   if( m_iType == VAR_Undef ) 
	{
	  m_iType = rV.m_iType;
	  switch( rV.m_iType )
	   {
	     case VAR_Str:
	       m_Str = rV.m_Str; break;
	     case VAR_Char:
  	       m_Char = rV.m_Char; break;
	     case VAR_Int:
	       m_Int = rV.m_Int; break;
	     case VAR_Double:
	       m_Double = rV.m_Double; break;
	     case VAR_Bool:
	       m_Bool = rV.m_Bool; break;
	   };

	  m_bFIni = true;
	  return *this;
	}

   int iRes = CheckCompatibleTyp( compat, *this, rV, false );
   if( iRes == -2 ) ThrowNotCompatible( *this, rV );

   TVariantVar varTmp; varTmp.m_iKeyLastOp = OPEQ_EQ;
   if( iRes != -1 ) TypeCast( varTmp, rV, m_iType );	
   else varTmp = rV;

   switch( m_iType )
	{
	  case VAR_Str:
	    m_Str = varTmp.m_Str; break;
	  case VAR_Char:
  	    m_Char = varTmp.m_Char, m_Str = "";
		break;
	  case VAR_Int:
	    m_Int = varTmp.m_Int, m_Str = "";
		break;
	  case VAR_Double:
	    m_Double = varTmp.m_Double, m_Str = "";
		break;
	  case VAR_Bool:
	    m_Bool = varTmp.m_Bool, m_Str = "";
		break;
	};
   
   m_bFIni = true;
   return *this;
 }


static int CmpPred( bool v1, bool v2, int iPred )
 {
   switch( iPred )
	{
	  case TVariantVar::TP_EQ:
	    return v1 == v2;
	  case TVariantVar::TP_NEQ:
	    return v1 != v2;
	  case TVariantVar::TP_LE:
	    return v1 < v2;
	  case TVariantVar::TP_GE:
	    return v1 > v2;
	  case TVariantVar::TP_LEQ:
	    return v1 <= v2;
	  case TVariantVar::TP_GEQ:
	    return v1 >= v2;
	}
   return 0;
 }

static int CmpPred( char v1, char v2, int iPred )
 {
   switch( iPred )
	{
	  case TVariantVar::TP_EQ:
	    return v1 == v2;
	  case TVariantVar::TP_NEQ:
	    return v1 != v2;
	  case TVariantVar::TP_LE:
	    return v1 < v2;
	  case TVariantVar::TP_GE:
	    return v1 > v2;
	  case TVariantVar::TP_LEQ:
	    return v1 <= v2;
	  case TVariantVar::TP_GEQ:
	    return v1 >= v2;
	}
   return 0;
 }
  
static int CmpPred( int v1, int v2, int iPred )
 {
   switch( iPred )
	{
	  case TVariantVar::TP_EQ:
	    return v1 == v2;
	  case TVariantVar::TP_NEQ:
	    return v1 != v2;
	  case TVariantVar::TP_LE:
	    return v1 < v2;
	  case TVariantVar::TP_GE:
	    return v1 > v2;
	  case TVariantVar::TP_LEQ:
	    return v1 <= v2;
	  case TVariantVar::TP_GEQ:
	    return v1 >= v2;
	}
   return 0;
 }

static int CmpPred( double v1, double v2, int iPred )
 {
   switch( iPred )
	{
	  case TVariantVar::TP_EQ:
	    return v1 == v2;
	  case TVariantVar::TP_NEQ:
	    return v1 != v2;
	  case TVariantVar::TP_LE:
	    return v1 < v2;
	  case TVariantVar::TP_GE:
	    return v1 > v2;
	  case TVariantVar::TP_LEQ:
	    return v1 <= v2;
	  case TVariantVar::TP_GEQ:
	    return v1 >= v2;
	}
   return 0;
 }

static int CmpPred( CString& v1, CString& v2, int iPred )
 {
   switch( iPred )
	{
	  case TVariantVar::TP_EQ:
	    return v1 == v2;
	  case TVariantVar::TP_NEQ:
	    return v1 != v2;
	  case TVariantVar::TP_LE:
	    return v1 < v2;
	  case TVariantVar::TP_GE:
	    return v1 > v2;
	  case TVariantVar::TP_LEQ:
	    return v1 <= v2;
	  case TVariantVar::TP_GEQ:
	    return v1 >= v2;
	}
   return 0;
 }


void TVariantVar::Predicat( const TVariantVar& rV, int iPred, TVariantVar& rRes )
 {
   m_iKeyLastOp = ((TVariantVar&)rV).m_iKeyLastOp = iPred;

   static TCompatible compat[ NUMB_TYP ] = 
	{
	 { VAR_Str, {-1,-1,-1,-1,-1} },
	 { VAR_Char, {VAR_Int,VAR_Double,-1,-1,-1} },
	 { VAR_Int, {VAR_Bool,VAR_Char,VAR_Double,-1,-1} },
	 { VAR_Double, {VAR_Bool,VAR_Char,VAR_Int,-1,-1} },
	 { VAR_Bool, {VAR_Char,VAR_Int,VAR_Double,-1,-1} }
	};

   if( rV.m_bFIni == false ) ThrowNotInit( rV );
   if( rV.m_iType == VAR_Undef ) ThrowNotDefined( rV );

   int iRes = CheckCompatibleTyp( compat, *this, rV, true );
   if( iRes == -2 ) ThrowNotCompatible( *this, rV );

   TVariantVar varTmp1, varTmp2;
   varTmp1.m_iKeyLastOp = varTmp2.m_iKeyLastOp = iPred;
   if( iRes != -1 ) 
	 TypeCast( varTmp1, *this, iRes ), TypeCast( varTmp2, rV, iRes );
   else varTmp1 = *this, varTmp2 = rV;

   rRes.m_iType = VAR_Bool;
   rRes.m_bFIni = true;
   switch( varTmp1.m_iType )
	{
	  case VAR_Str:
	    rRes.m_Bool = CmpPred( varTmp1.m_Str, varTmp2.m_Str, iPred );
		break;
	  case VAR_Char:
  	    rRes.m_Bool = CmpPred( varTmp1.m_Char, varTmp2.m_Char, iPred );
		break;
	  case VAR_Int:
	    rRes.m_Bool = CmpPred( varTmp1.m_Int, varTmp2.m_Int, iPred );
		break;
	  case VAR_Double:
	    rRes.m_Bool = CmpPred( varTmp1.m_Double, varTmp2.m_Double, iPred );
		break;
	  case VAR_Bool:
	    rRes.m_Bool = CmpPred( varTmp1.m_Bool, varTmp2.m_Bool, iPred );
		break;
	  default:
	    rRes.m_bFIni = false;
		break;
	};

   return;
 }

TVariantVar TVariantVar::operator==( const TVariantVar& rV )
 {
   TVariantVar res;
   Predicat( rV, TP_EQ, res );
   return res;
 }
TVariantVar TVariantVar::operator!=( const TVariantVar& rV )
 {
   TVariantVar res;
   Predicat( rV, TP_NEQ, res );
   return res;
 }
TVariantVar TVariantVar::operator<( const TVariantVar& rV )
 {
   TVariantVar res;
   Predicat( rV, TP_LE, res );
   return res;
 }
TVariantVar TVariantVar::operator>( const TVariantVar& rV )
 {
   TVariantVar res;
   Predicat( rV, TP_GE, res );
   return res;
 }
TVariantVar TVariantVar::operator<=( const TVariantVar& rV )
 {
   TVariantVar res;
   Predicat( rV, TP_LEQ, res );
   return res;
 }
TVariantVar TVariantVar::operator>=( const TVariantVar& rV )
 {
   TVariantVar res;
   Predicat( rV, TP_GEQ, res );
   return res;
 }

static void MakeOper( bool v1, bool v2, bool& res, int iOp )
 { 
   switch( iOp )
	{	  
	  case TVariantVar::OP_OROR:
	    res = v1 || v2;
		break;
	  case TVariantVar::OP_ANDAND:
	    res = v1 && v2;
		break;
	};
 }

static void MakeOper( char v1, char v2, char& res, int iOp )
 { 
   switch( iOp )
	{
	  case TVariantVar::OP_PLUS:
	    res = v1 + v2;
		break;
	  case TVariantVar::OP_MINUS:
	    res = v1 - v2;
		break;
	  case TVariantVar::OP_MUL:
	    res = v1 * v2;
		break;
	  case TVariantVar::OP_DIV:
	    res = v1 / v2;
		break;
	  case TVariantVar::OP_MOD:
	    res = v1 % v2;
		break;

      case TVariantVar::OP_OROR:
	    res = v1 || v2;
		break;
	  case TVariantVar::OP_ANDAND:
	    res = v1 && v2;
		break;
	};
 }

static void MakeOper( int v1, int v2, int& res, int iOp )
 { 
   switch( iOp )
	{
	  case TVariantVar::OP_PLUS:
	    res = v1 + v2;
		break;
	  case TVariantVar::OP_MINUS:
	    res = v1 - v2;
		break;
	  case TVariantVar::OP_MUL:
	    res = v1 * v2;
		break;
	  case TVariantVar::OP_DIV:
	    res = v1 / v2;
		break;
	  case TVariantVar::OP_MOD:
	    res = v1 % v2;
		break;

	  case TVariantVar::OP_OROR:
	    res = v1 || v2;
		break;
	  case TVariantVar::OP_ANDAND:
	    res = v1 && v2;
		break;
	};
 }

static void MakeOper( double v1, double v2, double& res, int iOp )
 { 
   switch( iOp )
	{
	  case TVariantVar::OP_PLUS:
	    res = v1 + v2;
		break;
	  case TVariantVar::OP_MINUS:
	    res = v1 - v2;
		break;
	  case TVariantVar::OP_MUL:
	    res = v1 * v2;
		break;
	  case TVariantVar::OP_DIV:
	    res = v1 / v2;
		break;
	  case TVariantVar::OP_MOD:
	    //res = v1 % v2;
		break;

	  case TVariantVar::OP_OROR:
	    res = v1 || v2;
		break;
	  case TVariantVar::OP_ANDAND:
	    res = v1 && v2;
		break;
	};
 }

static void MakeOper( CString& v1, CString& v2, CString& res, int iOp )
 { 
   switch( iOp )
	{
	  case TVariantVar::OP_PLUS:
	    res = v1 + v2;
		break;
	};
 }

//enum TOper { OP_PLUS, OP_MINUS, OP_MUL, OP_DIV, OP_MOD, OP_OROR, OP_ANDAND }
void TVariantVar::Operacion( const TVariantVar& rV, int iOp, TVariantVar& rRes )
 {
   m_iKeyLastOp = ((TVariantVar&)rV).m_iKeyLastOp = iOp;

   static TCompatible compat[ NUMB_TYP ] = 
	{
	 { VAR_Str, {VAR_Char,-1,-1,-1,-1} },
	 { VAR_Char, {VAR_Int,VAR_Double,-1,-1,-1} },
	 { VAR_Int, {VAR_Char,VAR_Double,-1,-1,-1} },
	 { VAR_Double, {VAR_Char,VAR_Int,-1,-1,-1} },
	 { VAR_Bool, {-1,-1,-1,-1,-1} }
	};

   static TCompatible compatLogic[ NUMB_TYP ] = 
	{
	 { VAR_Str, {-1,-1,-1,-1,-1} },
	 { VAR_Char, {VAR_Bool, VAR_Int,VAR_Double,-1,-1} },
	 { VAR_Int, {VAR_Bool, VAR_Char,VAR_Double,-1,-1} },
	 { VAR_Double, {VAR_Bool, VAR_Char,VAR_Int,-1,-1} },
	 { VAR_Bool, {VAR_Char,VAR_Int,VAR_Double,-1,-1} }
	};

   static TEnableOper enblOp[ NUMB_TYP ] =
	{
	  { VAR_Str,    {OP_PLUS,-1,-1,-1,-1,-1,-1} },
	  { VAR_Char,   {OP_PLUS,OP_MINUS,OP_MUL,OP_DIV,OP_MOD,OP_OROR,OP_ANDAND} },
	  { VAR_Int,    {OP_PLUS,OP_MINUS,OP_MUL,OP_DIV,OP_MOD,OP_OROR,OP_ANDAND} },
	  { VAR_Double, {OP_PLUS,OP_MINUS,OP_MUL,OP_DIV,OP_OROR,OP_ANDAND,-1} },
	  { VAR_Bool,   {OP_OROR,OP_ANDAND,-1,-1,-1,-1,-1} }
	};
   

   if( rV.m_bFIni == false ) ThrowNotInit( rV );
   if( m_bFIni == false ) ThrowNotInit( *this );
   if( rV.m_iType == VAR_Undef ) ThrowNotDefined( rV );
   if( m_iType == VAR_Undef ) ThrowNotDefined( *this );

   int iRes = CheckCompatibleTyp( 
	 IsLogic(iOp) == true ? compatLogic:compat, *this, rV, true );
   if( iRes == -2 ) ThrowNotCompatible( *this, rV );

   if( IsLogic(iOp) == true ) iRes = VAR_Bool;

   TVariantVar varTmp1, varTmp2;
   varTmp1.m_iKeyLastOp = varTmp2.m_iKeyLastOp = iOp;
   if( iRes != -1 ) 
	 TypeCast( varTmp1, *this, iRes ), TypeCast( varTmp2, rV, iRes );
   else varTmp1 = *this, varTmp2 = rV;

   TEnableOper *pOp = enblOp;
   for( int i = 0; i < NUMB_TYP; i++, pOp++ )
	 if( pOp->m_iType == varTmp1.m_iType ) break;

   for( i = 0; i < NUMB_BINARY_OPER; i++ )
	 if( pOp->m_Oper[i] == iOp ) 
	  {
	    rRes.m_bFIni = true;
		rRes.m_iType = varTmp1.m_iType;
        switch( varTmp1.m_iType )
		 {
	       case VAR_Str:
	         MakeOper( varTmp1.m_Str, varTmp2.m_Str, rRes.m_Str, iOp );
			 break;
	       case VAR_Char:
  	         MakeOper( varTmp1.m_Char, varTmp2.m_Char, rRes.m_Char, iOp );
			 break;
	       case VAR_Int:
	         MakeOper( varTmp1.m_Int, varTmp2.m_Int, rRes.m_Int, iOp );
			 break;
	       case VAR_Double:
	         MakeOper( varTmp1.m_Double, varTmp2.m_Double, rRes.m_Double, iOp );
			 break;
	       case VAR_Bool:
	         MakeOper( varTmp1.m_Bool, varTmp2.m_Bool, rRes.m_Bool, iOp );
			 break;
		 };
		return;
	  }
	 else if( pOp->m_Oper[i] == -1 ) break;

   m_iKeyLastOp = iOp;
   ThrowOperDisabledForTyp( *this ); 	 
 }

TVariantVar TVariantVar::operator+( const TVariantVar& rV )
 {
   TVariantVar res;
   Operacion( rV, OP_PLUS, res );
   return res;
 }

TVariantVar TVariantVar::operator-( const TVariantVar& rV )
 {
   TVariantVar res;
   Operacion( rV, OP_MINUS, res );
   return res;
 }

TVariantVar TVariantVar::operator*( const TVariantVar& rV )
 {
   TVariantVar res;
   Operacion( rV, OP_MUL, res );
   return res;
 }

TVariantVar TVariantVar::operator/( const TVariantVar& rV )
 {
   TVariantVar res;
   Operacion( rV, OP_DIV, res );
   return res;
 }

TVariantVar TVariantVar::operator%( const TVariantVar& rV )
 {
   TVariantVar res;
   Operacion( rV, OP_MOD, res );
   return res;
 }

TVariantVar TVariantVar::operator||( const TVariantVar& rV )
 {
   TVariantVar res;
   Operacion( rV, OP_OROR, res );
   return res;
 }

TVariantVar TVariantVar::operator&&( const TVariantVar& rV )
 {
   TVariantVar res;
   Operacion( rV, OP_ANDAND, res );
   return res;
 }

static void MakeUnOper( CString& v1, CString& res, int iOp )
 {
   switch( iOp )
	{
	  case TVariantVar::UN_MINUS:
	    break;
	  case TVariantVar::UN_NE:
	    break;
	};
 }

static void MakeUnOper( char v1, char& res, int iOp )
 {
   switch( iOp )
	{
	  case TVariantVar::UN_MINUS:
	    res = -v1;
	    break;
	  case TVariantVar::UN_NE:
	    res = !v1;
	    break;	    
	};
 }

static void MakeUnOper( bool v1, bool& res, int iOp )
 {
   switch( iOp )
	{
	  case TVariantVar::UN_MINUS:
	    res = -v1;
	    break;
	  case TVariantVar::UN_NE:
	    res = !v1;
	    break;	    
	};
 }

static void MakeUnOper( int v1, int& res, int iOp )
 {
   switch( iOp )
	{
	  case TVariantVar::UN_MINUS:
	    res = -v1;
	    break;
	  case TVariantVar::UN_NE:
	    res = !v1;
	    break;	    
	};
 }

static void MakeUnOper( double v1, double& res, int iOp )
 {
   switch( iOp )
	{
	  case TVariantVar::UN_MINUS:
	    res = -v1;
	    break;
	  case TVariantVar::UN_NE:
	    res = !v1;
	    break;	    
	};
 }

//enum TOperUnary { UN_MINUS, UN_NE }
void TVariantVar::OperacionUnary( int iOp, TVariantVar& rRes )
 {
   m_iKeyLastOp = iOp;

   static TEnableOper enblOp[ NUMB_TYP ] =
	{
	  { VAR_Str,    {-1,-1,-1,-1,-1,-1,-1} },
	  { VAR_Char,   {UN_MINUS,UN_NE,-1,-1,-1,-1,-1} },
	  { VAR_Int,    {UN_MINUS,UN_NE,-1,-1,-1,-1,-1} },
	  { VAR_Double, {UN_MINUS,UN_NE,-1,-1,-1,-1,-1} },
	  { VAR_Bool,   {UN_MINUS,UN_NE,-1,-1,-1,-1,-1} }
	};
   
   
   if( m_bFIni == false ) ThrowNotInit( *this );   
   if( m_iType == VAR_Undef ) ThrowNotDefined( *this );

   TEnableOper *pOp = enblOp;
   for( int i = 0; i < NUMB_TYP; i++, pOp++ )
	 if( pOp->m_iType == m_iType ) break;

   for( i = 0; i < NUMB_UNARY_OPER; i++ )
	 if( pOp->m_Oper[i] == iOp ) 
	  {
	    rRes.m_bFIni = true;
		rRes.m_iType = m_iType;
        switch( m_iType )
		 {
	       case VAR_Str:
	         MakeUnOper( m_Str, rRes.m_Str, iOp );
			 break;
	       case VAR_Char:
  	         MakeUnOper( m_Char, rRes.m_Char, iOp );
			 break;
	       case VAR_Int:
	         MakeUnOper( m_Int, rRes.m_Int, iOp );
			 break;
	       case VAR_Double:
	         MakeUnOper( m_Double, rRes.m_Double, iOp );
			 break;
	       case VAR_Bool:
	         MakeUnOper( m_Bool, rRes.m_Bool, iOp );
			 break;
		 };
		return;
	  }
	 else if( pOp->m_Oper[i] == -1 ) break;

   m_iKeyLastOp = iOp;
   ThrowOperDisabledForTyp( *this ); 	 
 }

TVariantVar TVariantVar::operator-()
 {
   TVariantVar res;
   OperacionUnary( UN_MINUS, res );
   return res;
 }

TVariantVar TVariantVar::operator!()
 {
   TVariantVar res;
   OperacionUnary( UN_NE, res );
   return res;
 }

void TVariantVar::ValToStr( CString& rS, int iFloatPrec, int iModeDigit )
   {
     if( m_bFIni == false )
	  {
        rS = "Не инициализирована"; return;
	  }
     
     switch( m_iType )
	  {
	    case VAR_Bool:
		  rS.Format( "%s", m_Bool == true ? "True":"False" );
		  break;
	    case VAR_Char:
		  rS.Format( "%c", m_Char );
		  break;
	    case VAR_Int:
		  rS.Format( "%d", m_Int );
		  break;
	    case VAR_Double:
		 {
		   LPSTR lp = rS.GetBuffer( 100 );
		   ostrstream strm( lp, 99 );
		   long lFl = strm.flags();

		   if( iModeDigit == 1 ) lFl |= ios::fixed;
		   else if( iModeDigit == 2 ) lFl |= ios::scientific;

		   strm << setiosflags(lFl) << setprecision(iFloatPrec) << m_Double;
		   rS.ReleaseBuffer( strm.pcount() );
		  //rS.Format( "%f", m_Double );
		  break;
		 }
		case VAR_Str:
		  rS.Format( "%s", (LPCSTR)m_Str );
		  break;
		case VAR_Undef:
		  rS.Format( "Не определена" );
		  break;
	  }
   }

TNode::~TNode()
 {
 }

void TNodeVar::CalcVal()
 {
   if( m_rVariable.m_bFIni == false ) TVariantVar::ThrowNotInit( m_rVariable );   
   if( m_rVariable.m_iType == VAR_Undef ) TVariantVar::ThrowNotDefined( m_rVariable );   
 }

TVariantVar& TNodeVar::Val() 
 {
   return m_rVariable;
 }

TNodeVar::~TNodeVar()
 {
 }

int TNodeVar::IsA() const
 {
   return TN_Var;
 }

TFuncInfo TNodeFunction::m_fiInfo[] =
 {
  { &TNodeFunction::BinOp_OPEQ, FT_OPEQ, "=", 2 },
   
   { &TNodeFunction::BinOp_PLUS, FT_PLUS, "+", 2 },
   { &TNodeFunction::BinOp_MINUS, FT_MINUS, "-", 2 },
   { &TNodeFunction::BinOp_MUL, FT_MUL, "*", 2 },
   { &TNodeFunction::BinOp_DIV, FT_DIV, "/", 2 },
   { &TNodeFunction::BinOp_MOD, FT_MOD, "%", 2 },
   { &TNodeFunction::BinOp_OROR, FT_OROR, "||", 2 },
   { &TNodeFunction::BinOp_ANDAND, FT_ANDAND, "&&", 2 },

   { &TNodeFunction::UnOp_MINUS, FT_UNMINUS, "Un-", 1 },
   { &TNodeFunction::UnOp_NE, FT_NE, "!", 1 },
   
   { &TNodeFunction::Pred_EQ, FT_EQ, "==", 2 },
   { &TNodeFunction::Pred_NEQ, FT_NEQ, "!=", 2 },
   { &TNodeFunction::Pred_LE, FT_LE, "<", 2 },
   { &TNodeFunction::Pred_GE, FT_GE, ">", 2 },
   { &TNodeFunction::Pred_LEQ, FT_LEQ, "<=", 2 },
   { &TNodeFunction::Pred_GEQ, FT_GEQ, ">=", 2 },

   { &TNodeFunction::BinFunc_MIN, FTF_MIN, "min", 2 },
   { &TNodeFunction::BinFunc_MAX, FTF_MAX, "max", 2 },
   { &TNodeFunction::BinFunc_POW, FTF_POW, "pow", 2 },
   { &TNodeFunction::BinFunc_FMOD, FTF_FMOD, "fmod", 2 },
   { &TNodeFunction::BinFunc_CMPI, FTF_CMPI, "Cmpi", 2 },
   { &TNodeFunction::BinFunc_SCAS, FTF_SCAS, "Scas", 2 },
   

   { &TNodeFunction::UnFunc_SIN, FTF_SIN, "sin", 1 },
   { &TNodeFunction::UnFunc_COS, FTF_COS, "cos", 1 },
   { &TNodeFunction::UnFunc_TAN, FTF_TAN, "tan", 1 },
   { &TNodeFunction::UnFunc_ATAN, FTF_ATAN, "atan", 1 },
   { &TNodeFunction::UnFunc_ASIN, FTF_ASIN, "asin", 1 },
   { &TNodeFunction::UnFunc_ACOS, FTF_ACOS, "acos", 1 },
   { &TNodeFunction::UnFunc_LOG, FTF_LOG, "log", 1 },
   { &TNodeFunction::UnFunc_LOG10, FTF_LOG10, "log10", 1 },
   { &TNodeFunction::UnFunc_EXP, FTF_EXP, "exp", 1 },
   { &TNodeFunction::UnFunc_SQRT, FTF_SQRT, "sqrt", 1 },
   { &TNodeFunction::UnFunc_FLOOR, FTF_FLOOR, "floor", 1 },
   { &TNodeFunction::UnFunc_CEIL, FTF_CEIL, "ceil", 1 },
   { &TNodeFunction::UnFunc_EXP, FTF_EXP, "exp", 1 },

   { &TNodeFunction::UnFunc_Bool, FTF_Bool, "bool", 1 },
   { &TNodeFunction::UnFunc_Char, FTF_Char, "char", 1 },
   { &TNodeFunction::UnFunc_Int, FTF_Int, "integer", 1 },   
   { &TNodeFunction::UnFunc_Double, FTF_Double, "double", 1 },   
   { &TNodeFunction::UnFunc_Str, FTF_String, "string", 1 },

  { NULL, -1, NULL }
 };


int TNodeFunction::IsA() const
 {
   return TN_Func;
 }

TFuncInfo* TNodeFunction::FindFuncPtr( int iTyp )
 {
   TFuncInfo *pTbl = TNodeFunction::m_fiInfo;
   for( ; pTbl->m_pF; pTbl++ )
	 if( pTbl->m_iTyp == iTyp ) return pTbl;

   return NULL;
 }

TFuncInfo* TNodeFunction::FindFuncPtr( LPCSTR lpName )
 {
   TFuncInfo *pTbl = TNodeFunction::m_fiInfo;
   for( ; pTbl->m_pF; pTbl++ )
	 if( !_stricmp(pTbl->m_pName, lpName) ) return pTbl;

   return NULL;
 }

TNodeFunction::TNodeFunction( int iTyp, TNode* pN1, TNode* pN2 ): 
  TNode()
   {
	 m_pN1 = pN1;
 	 m_pN2 = pN2;

	 m_iType = iTyp;
	 m_pFunc = FindFuncPtr( iTyp )->m_pF;
   }
  

TNodeFunction::~TNodeFunction()
 {
 }

void TNodeFunction::CalcVal()
 {
   void* pV = _set_se_translator( ConvertSEHFunc );   
   //_control87( ~(_EM_ZERODIVIDE|_EM_OVERFLOW|_EM_INVALID|_EM_UNDERFLOW), _MCW_EM );
   unsigned int uiKeyFlag = _control87( 0, 0 );
   _control87( _EM_DENORMAL|_EM_INEXACT, _MCW_EM );
   //_control87( 0, _MCW_EM );

   try {
     if( m_iType != FT_OPEQ && m_pN1 ) m_pN1->CalcVal();
     if( m_pN2 ) m_pN2->CalcVal();
     (this->*m_pFunc)();
	}
   catch( TMyException* pE )
	{
      _set_se_translator( (_se_translator_function)pV );
	  _fpreset();
	  throw pE;
	}
   catch(...)
	{
	  _set_se_translator( (_se_translator_function)pV );
	  _fpreset();
	  throw;
	}

   _set_se_translator( (_se_translator_function)pV );
   _control87( uiKeyFlag, _MCW_EM );
 }

TVariantVar& TNodeFunction::Val() 
 {
   return m_Value;
 }

int TNodException::GetSubCode() const
 {
   return TSC_Subcode;
 }

TNodException::TNodException( BOOL bFlDel, int iExcCode, LPSTR lpName ):
   TMyException( iExcCode, bFlDel )
 { 
   static char *cExMsg[] =
	{
      "Ошибочное число аргументов в функции '%s'",	  
	  "Неизвесная ошибка вычисления выражения в функции '%s'",
	  "Присвоение значения функции '%s'",
	  "Присвоение константе '%s'"
	};   
   

   if( iExcCode == NOD_BadNumberArg )	
	 m_strMsg.Format( cExMsg[NOD_BadNumberArg], lpName ); 
   else if( iExcCode == NOD_Unknown )	
	 m_strMsg.Format( cExMsg[NOD_Unknown], lpName );   
   else if( iExcCode == NOD_AssInvalidNode )	
	 m_strMsg.Format( cExMsg[NOD_AssInvalidNode], lpName );
   else if( iExcCode == NOD_AssToConst )	
	 m_strMsg.Format( cExMsg[NOD_AssToConst], lpName );
 }

void TNodeFunction::ThrowBadNumArguments()
 {
   TNodException *pE = new TNodException( TRUE, 
	 NOD_BadNumberArg, FindFuncPtr(m_iType)->m_pName );
   throw pE;      
 }

void TNodeFunction::ThrowAssignInvalidNode()
 {
   TNodException *pE = new TNodException( TRUE, 
	 NOD_AssInvalidNode, FindFuncPtr(m_iType)->m_pName );
   throw pE;      
 }

void TNodeFunction::ThrowAssignConst()
 {
   CString str;
   ((TNodeVar*)m_pN1)->m_rVariable.ValToStr( str );   

   TNodException *pE = new TNodException( TRUE, 
	 NOD_AssToConst, (LPSTR)(LPCSTR)str );
   throw pE;      
 }

void TNodeFunction::CheckArguments( int iNumb )
 {
   if( iNumb == 1 && !m_pN1 ) ThrowBadNumArguments();
   if( iNumb == 2 && (!m_pN1||!m_pN2) ) ThrowBadNumArguments();
 }

void TNodeFunction::BinOp_OPEQ() // =
 {
   CheckArguments( 2 );
   //m_pN2->CalcVal();
   
   if( m_pN1->IsA() != TN_Var ) ThrowAssignInvalidNode();
   if( ((TNodeVar*)m_pN1)->m_bConst == true ) ThrowAssignConst();

   ((TNodeVar*)m_pN1)->m_rVariable = m_pN2->Val();
   //else ((TNodeFunction*)m_pN1)->m_Value = m_pN2->Val();

   m_Value = ((TNodeVar*)m_pN1)->m_rVariable;
 }
void TNodeFunction::BinOp_PLUS() // +
 {
   CheckArguments( 2 );
   //m_pN1->CalcVal(), m_pN2->CalcVal();   
   m_Value = m_pN1->Val() + m_pN2->Val();
 }
void TNodeFunction::BinOp_MINUS() // -
 {
   CheckArguments( 2 );
   //m_pN1->CalcVal(), m_pN2->CalcVal();   
   m_Value = m_pN1->Val() - m_pN2->Val();
 }
void TNodeFunction::BinOp_MUL() // *
 {
   CheckArguments( 2 );
   //m_pN1->CalcVal(), m_pN2->CalcVal();   
   m_Value = m_pN1->Val() * m_pN2->Val();
 }
void TNodeFunction::BinOp_DIV() // /
 {
   CheckArguments( 2 );
   //m_pN1->CalcVal(), m_pN2->CalcVal();   
   m_Value = m_pN1->Val() / m_pN2->Val();
 }
void TNodeFunction::BinOp_MOD() // %
 {
   CheckArguments( 2 );
   //m_pN1->CalcVal(), m_pN2->CalcVal();   
   m_Value = m_pN1->Val() % m_pN2->Val();
 }
void TNodeFunction::BinOp_OROR() // ||
 {
   CheckArguments( 2 );
   //m_pN1->CalcVal(), m_pN2->CalcVal();   
   m_Value = m_pN1->Val() || m_pN2->Val();
 }
void TNodeFunction::BinOp_ANDAND() // &&
 {
   CheckArguments( 2 );
   //m_pN1->CalcVal(), m_pN2->CalcVal();   
   m_Value = m_pN1->Val() && m_pN2->Val();
 }

void TNodeFunction::UnOp_MINUS() // -
 {
   CheckArguments( 1 );
   //m_pN1->CalcVal();
   m_Value = -(m_pN1->Val());
 }
void TNodeFunction::UnOp_NE() // !
 {
   CheckArguments( 1 );
   //m_pN1->CalcVal();
   m_Value = !(m_pN1->Val());
 }

void TNodeFunction::Pred_EQ() // ==
 {
   CheckArguments( 2 );
   //m_pN1->CalcVal(), m_pN2->CalcVal();   
   m_Value = m_pN1->Val() == m_pN2->Val();
 }
void TNodeFunction::Pred_NEQ() // !=
 {
   CheckArguments( 2 );
   //m_pN1->CalcVal(), m_pN2->CalcVal();   
   m_Value = m_pN1->Val() != m_pN2->Val();
 }
void TNodeFunction::Pred_LE() // <
 {
   CheckArguments( 2 );
   //m_pN1->CalcVal(), m_pN2->CalcVal();   
   m_Value = m_pN1->Val() < m_pN2->Val();
 }
void TNodeFunction::Pred_GE() // >
 {
   CheckArguments( 2 );
   //m_pN1->CalcVal(), m_pN2->CalcVal();   
   m_Value = m_pN1->Val() > m_pN2->Val();
 }
void TNodeFunction::Pred_LEQ() // <=
 {
   CheckArguments( 2 );
   //m_pN1->CalcVal(), m_pN2->CalcVal();   
   m_Value = m_pN1->Val() <= m_pN2->Val();
 }
void TNodeFunction::Pred_GEQ() // >=
 {
   CheckArguments( 2 );
   //m_pN1->CalcVal(), m_pN2->CalcVal();   
   m_Value = m_pN1->Val() >= m_pN2->Val();
 }

void My_min( TVariantVar* pRes, TVariantVar* p1, TVariantVar* p2 )
 {
   switch( p1->m_iType )
	{
	  case VAR_Char:
	    pRes->m_Char = min( p1->m_Char, p2->m_Char );
		break;
	  case VAR_Int:
	    pRes->m_Int = min( p1->m_Int, p2->m_Int );
		break;
	  case VAR_Double:
	    pRes->m_Double = min( p1->m_Double, p2->m_Double );
		break;
	}
 }

void My_max( TVariantVar* pRes, TVariantVar* p1, TVariantVar* p2 )
 {
   switch( p1->m_iType )
	{
	  case VAR_Char:
	    pRes->m_Char = max( p1->m_Char, p2->m_Char );
		break;
	  case VAR_Int:
	    pRes->m_Int = max( p1->m_Int, p2->m_Int );
		break;
	  case VAR_Double:
	    pRes->m_Double = max( p1->m_Double, p2->m_Double );
		break;
	}
 }

void My_pow( TVariantVar* pRes, TVariantVar* p1, TVariantVar* p2 )
 {
   switch( p1->m_iType )
	{
	  case VAR_Char:
	    pRes->m_Double = pow( double(p1->m_Char), double(p2->m_Char) );
		break;
	  case VAR_Int:
	    pRes->m_Double = pow( double(p1->m_Int), double(p2->m_Int) );
		break;
	  case VAR_Double:
	    pRes->m_Double = pow( p1->m_Double, p2->m_Double );
		break;
	}
 }

void My_mod( TVariantVar* pRes, TVariantVar* p1, TVariantVar* p2 )
 {
   switch( p1->m_iType )
	{
	  case VAR_Char:
	    pRes->m_Double = fmod( double(p1->m_Char), double(p2->m_Char) );
		break;
	  case VAR_Int:
	    pRes->m_Double = fmod( double(p1->m_Int), double(p2->m_Int) );
		break;
	  case VAR_Double:
	    pRes->m_Double = fmod( p1->m_Double, p2->m_Double );
		break;
	}
 }

void My_Cmpi( TVariantVar* pRes, TVariantVar* p1, TVariantVar* p2 )
 {
   switch( p1->m_iType )
	{
	  case VAR_Str:
	    pRes->m_Bool = !_stricmp( p1->m_Str, p2->m_Str );
		break;
	}
 }

void My_Scas( TVariantVar* pRes, TVariantVar* p1, TVariantVar* p2 )
 {
   switch( p1->m_iType )
	{
	  case VAR_Str:
	    pRes->m_Bool = (bool)strstr( p1->m_Str, p2->m_Str );
		break;
	}
 }


void My_sin( TVariantVar* pRes, TVariantVar* p1, TVariantVar* )
 {
   switch( p1->m_iType )
	{
	  case VAR_Char:
	    pRes->m_Double = sin( double(p1->m_Char) );
		break;
	  case VAR_Int:
	    pRes->m_Double = sin( double(p1->m_Int) );
		break;
	  case VAR_Double:
	    pRes->m_Double = sin( p1->m_Double );
		break;
	}
 }

void My_cos( TVariantVar* pRes, TVariantVar* p1, TVariantVar* )
 {
   switch( p1->m_iType )
	{
	  case VAR_Char:
	    pRes->m_Double = cos( double(p1->m_Char) );
		break;
	  case VAR_Int:
	    pRes->m_Double = cos( double(p1->m_Int) );
		break;
	  case VAR_Double:
	    pRes->m_Double = cos( p1->m_Double );
		break;
	}
 }

void My_tan( TVariantVar* pRes, TVariantVar* p1, TVariantVar* )
 {
   switch( p1->m_iType )
	{
	  case VAR_Char:
	    pRes->m_Double = tan( double(p1->m_Char) );
		break;
	  case VAR_Int:
	    pRes->m_Double = tan( double(p1->m_Int) );
		break;
	  case VAR_Double:
	    pRes->m_Double = tan( p1->m_Double );
		break;
	}
 }

void My_atan( TVariantVar* pRes, TVariantVar* p1, TVariantVar* )
 {
   switch( p1->m_iType )
	{
	  case VAR_Char:
	    pRes->m_Double = atan( double(p1->m_Char) );
		break;
	  case VAR_Int:
	    pRes->m_Double = atan( double(p1->m_Int) );
		break;
	  case VAR_Double:
	    pRes->m_Double = atan( p1->m_Double );
		break;
	}
 }

void My_acos( TVariantVar* pRes, TVariantVar* p1, TVariantVar* )
 {
   switch( p1->m_iType )
	{
	  case VAR_Char:
	    pRes->m_Double = acos( double(p1->m_Char) );
		break;
	  case VAR_Int:
	    pRes->m_Double = acos( double(p1->m_Int) );
		break;
	  case VAR_Double:
	    pRes->m_Double = acos( p1->m_Double );
		break;
	}
 }

void My_asin( TVariantVar* pRes, TVariantVar* p1, TVariantVar* )
 {
   switch( p1->m_iType )
	{
	  case VAR_Char:
	    pRes->m_Double = asin( double(p1->m_Char) );
		break;
	  case VAR_Int:
	    pRes->m_Double = asin( double(p1->m_Int) );
		break;
	  case VAR_Double:
	    pRes->m_Double = asin( p1->m_Double );
		break;
	}
 }

void My_log( TVariantVar* pRes, TVariantVar* p1, TVariantVar* )
 {
   switch( p1->m_iType )
	{
	  case VAR_Char:
	    pRes->m_Double = log( double(p1->m_Char) );
		break;
	  case VAR_Int:
	    pRes->m_Double = log( double(p1->m_Int) );
		break;
	  case VAR_Double:
	    pRes->m_Double = log( p1->m_Double );
		break;
	}
 }

void My_log10( TVariantVar* pRes, TVariantVar* p1, TVariantVar* )
 {
   switch( p1->m_iType )
	{
	  case VAR_Char:
	    pRes->m_Double = log10( double(p1->m_Char) );
		break;
	  case VAR_Int:
	    pRes->m_Double = log10( double(p1->m_Int) );
		break;
	  case VAR_Double:
	    pRes->m_Double = log10( p1->m_Double );
		break;
	}
 }

void My_exp( TVariantVar* pRes, TVariantVar* p1, TVariantVar* )
 {
   switch( p1->m_iType )
	{
	  case VAR_Char:
	    pRes->m_Double = exp( double(p1->m_Char) );
		break;
	  case VAR_Int:
	    pRes->m_Double = exp( double(p1->m_Int) );
		break;
	  case VAR_Double:
	    pRes->m_Double = exp( p1->m_Double );
		break;
	}
 }

/*static void ThrowBARGExc( LPCSTR lp )
 {
   TSEHException *pE = 
	 new TSEHException( EXCEPTION_FLT_DENORMAL_OPERAND, true, lp );
   throw pE;
 }
void Check_SQRT( double d )
 {
   if( d < 0 ) ThrowBARGExc( "Квадратный корень из отрицательного числа" );
 }*/

void My_sqrt( TVariantVar* pRes, TVariantVar* p1, TVariantVar* )
 {
   switch( p1->m_iType )
	{
	  case VAR_Char:
	    //Check_SQRT( p1->m_Char );
	    pRes->m_Double = sqrt( double(p1->m_Char) );
		break;

	  case VAR_Int:
	    //Check_SQRT( p1->m_Int );
	    pRes->m_Double = sqrt( double(p1->m_Int) );
		break;

	  case VAR_Double:
	    //Check_SQRT( p1->m_Double );
	    pRes->m_Double = sqrt( p1->m_Double );
		break;
	}
 }


void My_floor( TVariantVar* pRes, TVariantVar* p1, TVariantVar* )
 {
   switch( p1->m_iType )
	{	  	  	 
	  case VAR_Char:
	    pRes->m_Double = floor( double(p1->m_Char) );
		break;
	  case VAR_Int:
	    pRes->m_Double = floor( double(p1->m_Int) );
		break;
	  case VAR_Double:
	    pRes->m_Double = floor( p1->m_Double );
		break;
	}
 }

void My_ceil( TVariantVar* pRes, TVariantVar* p1, TVariantVar* )
 {
   switch( p1->m_iType )
	{
	  case VAR_Char:
	    pRes->m_Double = ceil( double(p1->m_Char) );
		break;
	  case VAR_Int:
	    pRes->m_Double = ceil( double(p1->m_Int) );
		break;
	  case VAR_Double:
	    pRes->m_Double = ceil( p1->m_Double );
		break;
	}
 }

void My_Bool( TVariantVar* pRes, TVariantVar* p1, TVariantVar* )
 {
   switch( p1->m_iType )
	{
	  case VAR_Bool:
	    pRes->m_Bool = bool( p1->m_Bool );
		break;
	  case VAR_Char:
	    pRes->m_Bool = bool( p1->m_Char );
		break;
	  case VAR_Int:
	    pRes->m_Bool = bool( p1->m_Int );
		break;
	  case VAR_Double:
	    pRes->m_Bool = bool( p1->m_Double );
		break;
	}
 }

void My_Int( TVariantVar* pRes, TVariantVar* p1, TVariantVar* )
 {
   switch( p1->m_iType )
	{
	  case VAR_Bool:
	    pRes->m_Int = int( p1->m_Bool );
		break;
	  case VAR_Char:
	    pRes->m_Int = int( p1->m_Char );
		break;
	  case VAR_Int:
	    pRes->m_Int = int( p1->m_Int );
		break;
	  case VAR_Double:
	    pRes->m_Int = int( p1->m_Double );
		break;
	}
 }

void My_Char( TVariantVar* pRes, TVariantVar* p1, TVariantVar* )
 {
   switch( p1->m_iType )
	{
	  case VAR_Bool:
	    pRes->m_Char = char( p1->m_Bool );
		break;
	  case VAR_Char:
	    pRes->m_Char = char( p1->m_Char );
		break;
	  case VAR_Int:
	    pRes->m_Char = char( p1->m_Int );
		break;
	  case VAR_Double:
	    pRes->m_Char = char( p1->m_Double );
		break;
	}
 }

void My_Double( TVariantVar* pRes, TVariantVar* p1, TVariantVar* )
 {
   switch( p1->m_iType )
	{
	  case VAR_Bool:
	    pRes->m_Double = double( p1->m_Bool );
		break;
	  case VAR_Char:
	    pRes->m_Double = double( p1->m_Char );
		break;
	  case VAR_Int:
	    pRes->m_Double = double( p1->m_Int );
		break;
	  case VAR_Double:
	    pRes->m_Double = double( p1->m_Double );
		break;
	}
 }

void My_String( TVariantVar* pRes, TVariantVar* p1, TVariantVar* )
 {
   switch( p1->m_iType )
	{
	  case VAR_Bool:
	    pRes->m_Str.Format( "%s", p1->m_Bool == true ? "TRUE":"FALSE" );
		break;
	  case VAR_Char:
	    pRes->m_Str.Format( "%c", p1->m_Char );
		break;
	  case VAR_Int:
	    pRes->m_Str.Format( "%d", p1->m_Int );
		break;
	  case VAR_Double:
	    pRes->m_Str.Format( "%f", p1->m_Double );
		break;
	  case VAR_Str:
	    pRes->m_Str = p1->m_Str;
		break;
	}
 }

void TNodeFunction::MakeFunc( int iOp )
 {
   static TCompatible_ForFunc compat[] =
	{
	 { FTF_MIN,    {VAR_Char,VAR_Int,VAR_Double,-1,-1}, -1, My_min, 2 },
	 { FTF_MAX,    {VAR_Char,VAR_Int,VAR_Double,-1,-1}, -1, My_max, 2 },
	 { FTF_POW,    {VAR_Char,VAR_Int,VAR_Double,-1,-1}, VAR_Double, My_pow, 2 },
	 { FTF_FMOD,    {VAR_Double,-1,-1,-1,-1}, VAR_Double, My_mod, 2 },
	 { FTF_CMPI,   {VAR_Str,-1,-1,-1,-1}, VAR_Bool, My_Cmpi, 2 },
	 { FTF_SCAS,   {VAR_Str,-1,-1,-1,-1}, VAR_Bool, My_Scas, 2 },

	 { FTF_SIN,    {VAR_Char,VAR_Int,VAR_Double,-1,-1}, VAR_Double, My_sin, 1 },
	 { FTF_COS,    {VAR_Char,VAR_Int,VAR_Double,-1,-1}, VAR_Double, My_cos, 1 },
	 { FTF_TAN,    {VAR_Char,VAR_Int,VAR_Double,-1,-1}, VAR_Double, My_tan, 1 },
	 { FTF_ATAN,   {VAR_Char,VAR_Int,VAR_Double,-1,-1}, VAR_Double, My_atan, 1 },
	 { FTF_ASIN,   {VAR_Char,VAR_Int,VAR_Double,-1,-1}, VAR_Double, My_asin, 1 },

	 { FTF_ACOS,   {VAR_Char,VAR_Int,VAR_Double,-1,-1}, VAR_Double, My_acos, 1 },
	 { FTF_LOG,    {VAR_Char,VAR_Int,VAR_Double,-1,-1}, VAR_Double, My_log, 1 },
	 { FTF_LOG10,  {VAR_Char,VAR_Int,VAR_Double,-1,-1}, VAR_Double, My_log10, 1 },
	 { FTF_EXP,    {VAR_Char,VAR_Int,VAR_Double,-1,-1}, VAR_Double, My_exp, 1 },
	 { FTF_SQRT,   {VAR_Char,VAR_Int,VAR_Double,-1,-1}, VAR_Double, My_sqrt, 1 },
	 { FTF_FLOOR,  {VAR_Double,-1,-1,-1,-1}, VAR_Double, My_floor, 1 },
	 { FTF_CEIL,   {VAR_Double,-1,-1,-1,-1}, VAR_Double, My_ceil, 1 },
	 { FTF_Bool,   {VAR_Char,VAR_Int,VAR_Double,VAR_Bool,-1}, VAR_Bool, My_Bool, 1 },
	 { FTF_Char,   {VAR_Char,VAR_Int,VAR_Double,VAR_Bool,-1}, VAR_Char, My_Char, 1 },
	 { FTF_Int,    {VAR_Char,VAR_Int,VAR_Double,VAR_Bool,-1}, VAR_Int, My_Int, 1 },
	 { FTF_Double, {VAR_Char,VAR_Int,VAR_Double,VAR_Bool,-1}, VAR_Double, My_Double, 1 },
	 { FTF_String, {VAR_Char,VAR_Int,VAR_Double,VAR_Bool,VAR_Str}, VAR_Str, My_String, 1 },

	 { NULL, {-1,-1,-1,-1,-1}, -1, NULL, -1 }
	};
 
   TCompatible_ForFunc *pCompat = compat;
   for( ; pCompat->m_iFunc != -1; pCompat++ )
	 if( pCompat->m_iFunc == iOp ) break;
   if( pCompat->m_iFunc == -1 ) return;


   CheckArguments( pCompat->m_iArn );
   /*m_pN1->CalcVal(),*/ m_pN1->Val().m_iKeyLastOp = iOp;
   if( pCompat->m_iArn == 2 ) /*m_pN2->CalcVal(),*/ m_pN2->Val().m_iKeyLastOp = iOp;

   bool bFlFound = false;
   for( int i = 0; i < NUMB_TYP; i++ )
	 if( pCompat->m_Compat[i] == m_pN1->Val().m_iType ) 
	  { bFlFound = true; break; }

   if( bFlFound == false ) TVariantVar::ThrowOperDisabledForTyp( m_pN1->Val() ); 	 

   int j = 0;
   if( pCompat->m_iArn == 2 )
	{
      bool bFlFound = false;
      for( j = 0; j < NUMB_TYP; j++ )
	   if( pCompat->m_Compat[j] == m_pN2->Val().m_iType ) 
		{ bFlFound = true; break; }

	   if( bFlFound == false ) TVariantVar::ThrowOperDisabledForTyp( m_pN2->Val() ); 	 
	}
   
   TVariantVar varTmp1, varTmp2;
   varTmp1.m_iKeyLastOp = varTmp2.m_iKeyLastOp = iOp;

   
   int iTypCast;
   //if( pCompat->m_iTypeRes == -1 )
	//{
	  if( pCompat->m_iArn == 1 ) varTmp1 = m_pN1->Val(), iTypCast = m_pN1->Val().m_iType;
	  else
	   {
         iTypCast = max( m_pN1->Val().m_iType, m_pN2->Val().m_iType );
		 TVariantVar::TypeCast( varTmp1, m_pN1->Val(), iTypCast ); 
		 TVariantVar::TypeCast( varTmp2, m_pN2->Val(), iTypCast );
	   }
	/*}
   else 
	{	  
	  iTypCast = pCompat->m_iTypeRes;
	  if( pCompat->m_iArn == 1 ) 
	     TVariantVar::TypeCast( varTmp1, m_pN1->Val(), pCompat->m_iTypeRes );
	  else
	   {
         TVariantVar::TypeCast( varTmp1, m_pN1->Val(), pCompat->m_iTypeRes );
		 TVariantVar::TypeCast( varTmp2, m_pN2->Val(), pCompat->m_iTypeRes );
	   }
	}*/
	
   m_Value.m_bFIni = true;
   m_Value.m_iType =  pCompat->m_iTypeRes == -1 ?  iTypCast:pCompat->m_iTypeRes;
   pCompat->pFunc( &m_Value,  &varTmp1, pCompat->m_iArn == 2 ? &varTmp2:NULL );      
 }



void TNodeFunction::BinFunc_MIN() // min
 {
   MakeFunc( FTF_MIN );   
 }
void TNodeFunction::BinFunc_MAX() // max
 {
   MakeFunc( FTF_MAX );
 }
void TNodeFunction::BinFunc_POW() // pow
 {
   MakeFunc( FTF_POW );
 }
void TNodeFunction::BinFunc_FMOD() // fmod
 {
   MakeFunc( FTF_FMOD );
 }
void TNodeFunction::BinFunc_CMPI() // cmpi
 {
   MakeFunc( FTF_CMPI );
 }
void TNodeFunction::BinFunc_SCAS() // scas
 {
   MakeFunc( FTF_SCAS );
 }

void TNodeFunction::UnFunc_SIN() // sin
 {
   MakeFunc( FTF_SIN );
 }
void TNodeFunction::UnFunc_COS() // cos
 {
   MakeFunc( FTF_COS );
 }
void TNodeFunction::UnFunc_TAN() // tan
 {
   MakeFunc( FTF_TAN );
 }
void TNodeFunction::UnFunc_ATAN() // atan
 {
   MakeFunc( FTF_ATAN );
 }
void TNodeFunction::UnFunc_ASIN() // asin
 {
   MakeFunc( FTF_ASIN );
 }
void TNodeFunction::UnFunc_ACOS() // acos
 {
   MakeFunc( FTF_ACOS );
 }
void TNodeFunction::UnFunc_LOG() // log
 {
   MakeFunc( FTF_LOG );
 }
void TNodeFunction::UnFunc_LOG10() // log10
 {
   MakeFunc( FTF_LOG10 );
 }
void TNodeFunction::UnFunc_EXP() // exp
 {
   MakeFunc( FTF_EXP );
 }
void TNodeFunction::UnFunc_SQRT() // sqrt
 {
   MakeFunc( FTF_SQRT );
 }
void TNodeFunction::UnFunc_FLOOR() // floor
 {
   MakeFunc( FTF_FLOOR );
 }
void TNodeFunction::UnFunc_CEIL() // ceil
 {
   MakeFunc( FTF_CEIL );
 }


void TNodeFunction::UnFunc_Bool() // Bool()
 {
   MakeFunc( FTF_Bool ); 
 }
void TNodeFunction::UnFunc_Char() // Char()
 {
   MakeFunc( FTF_Char ); 
 }
void TNodeFunction::UnFunc_Int() // Int()
 {
   MakeFunc( FTF_Int ); 
 }
void TNodeFunction::UnFunc_Double() // Double()
 {
   MakeFunc( FTF_Double ); 
 }
void TNodeFunction::UnFunc_Str() // Str()     
 {
   MakeFunc( FTF_String ); 
 }


TNameSpace::~TNameSpace()
 {
   list<TVariantVar*>::iterator itSta( m_lstVar.begin() );
   list<TVariantVar*>::iterator itEnd( m_lstVar.end() );
   for( ;itSta != itEnd; itSta++ )
	 delete *itSta;

   m_lstVar.clear();
 }

TVariantVar* TNameSpace::AddVar( LPSTR lpName, TVariantTypes iType )
 {   
   TVariantVar *pVar = FindVar( lpName );
   if( pVar ) return pVar;
   pVar = new TVariantVar( (TVariantTypes)iType, lpName );
   m_lstVar.push_back( pVar );

   return pVar;
 }

void TNameSpace::GetUnicname( CString& rStr )
 {
   LPSTR lp = rStr.GetBuffer( 50 );
   sprintf( lp, "Const#%d", m_iUnic++ );
   while( FindVar(lp) ) sprintf( lp, "_UNIK_#%d", m_iUnic++ );

   rStr.ReleaseBuffer();
 }

void TNameSpace::AddVar( TVariantVar* pV ) //only named variable
 {
   TVariantVar *pVar = FindVar( pV->m_strName );
   if( pVar ) delete pV;
   else m_lstVar.push_back( pV );
 }


TVariantVar* TNameSpace::FindVar( LPCSTR lpName )
 {
   list<TVariantVar*>::iterator itSta( m_lstVar.begin() );
   list<TVariantVar*>::iterator itEnd( m_lstVar.end() );

   for( ;itSta != itEnd; itSta++ )
	 if( !(*itSta)->m_strName.CompareNoCase(lpName) ) return *itSta;

   return NULL;
 }

TVariantVar* TNameSpace::AddVarConst( TVariantVar* pV )
 {
   TVariantVar* pVar = FindVar_ForVal( pV );
   if( pVar ) { delete pV; return pVar; }
   else m_lstVar.push_back( pV );
   pV->m_bConst = true;

   return pV;
 }

TVariantVar* TNameSpace::FindVar_ForVal( TVariantVar* pV )
 {
   list<TVariantVar*>::iterator itSta( m_lstVar.begin() );
   list<TVariantVar*>::iterator itEnd( m_lstVar.end() );

   for( ;itSta != itEnd; itSta++ )
	 if( (*itSta)->m_bFIni==true && (*itSta)->m_iType == pV->m_iType &&
	     (*(*itSta)==*pV).m_Bool==true 
	   ) return *itSta;

   return NULL;
 }

double TExpression::GetMulKD( TVariantVar* pVNot )
 {
   double dMul = 1;

   list<TNode*>::iterator itSta( m_lstGarbage.begin() );
   list<TNode*>::iterator itEnd( m_lstGarbage.end() );

   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->IsA() == TN_Var && (DWORD)(*itSta) != (DWORD)pVNot )
	  {
        TNodeVar *pVar = (TNodeVar *)*itSta;
		if( pVar->m_bConst == true ) continue;

		dMul = min( dMul, pVar->m_rVariable.m_dKD );
	  }

   return dMul;
 }

void ClrUsFl( TNode* pN )
 {
   if( pN->IsA() == TN_Var )
	 ((TNodeVar*)pN)->m_rVariable.m_bUsed = false;
 }

void TExpression::ClearUsed()
 {
   for_each( m_lstGarbage.begin(), m_lstGarbage.end(), ClrUsFl );
 }

TVariantVar* TExpression::GetEQVar()
 {
   list<TNode*>::iterator itSta( m_lstGarbage.begin() );
   list<TNode*>::iterator itEnd( m_lstGarbage.end() );

   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->IsA() == TN_Func && 
	     ((TNodeFunction*)(*itSta))->GetTyp() == FT_OPEQ
	   )
	  return &(((TNodeVar*)(((TNodeFunction*)(*itSta))->GetN1()))->m_rVariable);

   return NULL;
 }

bool TExpression::FindVar( TVariantVar* pV )
 {
   list<TNode*>::iterator itSta( m_lstGarbage.begin() );
   list<TNode*>::iterator itEnd( m_lstGarbage.end() );

   for( ; itSta != itEnd; ++itSta )
     if( (*itSta)->IsA() == TN_Var )
	  {
	    if( (DWORD)&((TNodeVar*)(*itSta))->m_rVariable == (DWORD)pV )
		  return true;
	  }
	 /*else if( (*itSta)->IsA() == TN_Func )
	  {
	  }*/

   return false;
 }

TExpression::~TExpression()
 {
   ClearList();
 }

void TExpression::ClearList()
 {
   list<TNode*>::iterator itSta( m_lstGarbage.begin() );
   list<TNode*>::iterator itEnd( m_lstGarbage.end() );
   for( ;itSta != itEnd; itSta++ )
	 delete *itSta;

   m_lstGarbage.clear(); 
 }

int TParseException::GetSubCode() const
 {
   return TSC_Subcode;
 }


TParseException::TParseException( BOOL bFlDel, int iExcCode, LPCSTR lp ):
   TMyException( iExcCode, bFlDel )
 {
   static char *cExMsg[] =
	{
      "%s",
	  "%s",
	  "Неизвесная ошибка парсера",
	  "Свёртка '%s' вызвана ошибочно"
	};
   
   if( iExcCode == PRS_SyntaxErr )
      m_strMsg.Format( cExMsg[PRS_SyntaxErr], lp );
   else	if( iExcCode == PRS_ParserErr )
	  m_strMsg.Format( cExMsg[PRS_ParserErr], lp );
   else if( iExcCode == PRS_Unknown )
	  m_strMsg.Format( cExMsg[PRS_Unknown] );	
   else if( iExcCode == PRS_IllegalSimplify )
	  m_strMsg.Format( cExMsg[PRS_IllegalSimplify], lp );	   
 }

void TExpression::Parse( bool bFlAddBool  )
 {
   iEndClm = iStaClm = 0;
   ClearList(); m_pndRoot = NULL; m_strError = "";
   lpToken = (LPSTR)(LPCSTR)m_strExpr;

   int iRes = yyparse( this );

   if( iRes )
	{
      TParseException *pE = new TParseException( true, 
	     iRes == 1 ? PRS_SyntaxErr:PRS_ParserErr, m_strError );
	  throw pE;
	}
   
   if( bFlAddBool == true )
	{	  
      TNodeFunction *pNd = new TNodeFunction( FTF_Bool, *--m_lstGarbage.end() );
      m_pndRoot = pNd;
      m_lstGarbage.push_back( pNd );
	}
   else m_pndRoot = *--m_lstGarbage.end();
 }

void TExpression::Test( LPSTR lpToken_ )
 {
   iEndClm = iStaClm = 0;
   ClearList(); m_pndRoot = NULL; m_strError = "";   
   lpToken = lpToken_;

   YYSTYPE Yys; YYLTYPE Yyl;
   char cBuf[1024];
   int iLex;
   for( ; (iLex=Lexer(&Yys, &Yyl)); )
	{
      if( Yys.m_TokenInfo.m_iTokenType == NODE_UNDEFINED )	   
         sprintf( cBuf, "Single token: '%c'", char(iLex) );
	   else
		{
		   char cBuf2[1024]; memset( cBuf2, 0, 1024 );
		   strncpy( cBuf2, lpToken_+Yyl.m_iStaClm, Yyl.m_iEndClm - Yyl.m_iStaClm + 1 );
		   switch( Yys.m_TokenInfo.m_iTokenType )
			{ 
              case NODE_STRING_CONST:
		        sprintf( cBuf, "NODE_STRING_CONST:'%s'", cBuf2 );
				break;
              case NODE_BOOL_CONST:
		        sprintf( cBuf, "NODE_BOOL_CONST:'%s'", cBuf2 );
				break;
			  case NODE_FLOAT_CONST:
		        sprintf( cBuf, "NODE_FLOAT_CONST:'%s'", cBuf2 );
				break;
			  case NODE_CHARACTER_CONST:
		        sprintf( cBuf, "NODE_CHARACTER_CONST:'%s'", cBuf2 );
				break;
			  case NODE_INTEGER_CONST:
		        sprintf( cBuf, "NODE_INTEGER_CONST:'%s'", cBuf2 );
				break;
			  case NODE_IDENTIFIER:
		        sprintf( cBuf, "NODE_IDENTIFIER:'%s'", cBuf2 );
				break;
			  case NODE_UNARY_OPERATOR:
		        sprintf( cBuf, "NODE_UNARY_OPERATOR:'%s'", cBuf2 );
				break;
			}
		}

	  int a = 1;
	}
 }

void TExpression::MyError( LPSTR lp )
 {
   m_strError = lp;
 }

inline BOOL IsAplhaMy( int ch )
 {
   return isalpha(ch) || (ch >= 192 && ch <= 255) || ch == '@' || ch == '_';   
 };

static LPSTR FindNotAlphaDig( LPSTR lpSta )
 {
   for( ;*lpSta && (IsAplhaMy((unsigned char)*lpSta)||isdigit(*lpSta)); lpSta++ );
   return lpSta;
 }

int TExpression::Lexer( YYSTYPE* pYys, YYLTYPE* pYyl )
 {
#define REASSIGN() pYyl->m_iStaClm = iStaClm, \
                   pYyl->m_iEndClm = iEndClm++, \
                   lpToken++

   //int iStaClm, iEndClm;
   //LPSTR lpToken;   
   pYys->m_TokenInfo.m_iTokenType = NODE_UNDEFINED;

   for( ; *lpToken == ' ' || *lpToken == '\t'; iEndClm++, lpToken++ );
   if( !*lpToken ) return 0;
   iStaClm = iEndClm;
   
   if( *lpToken == '"' )
	{      	  
	  LPSTR lp2;
	  if( (lp2=strchr(lpToken+1, '"')) ) 
	   {
	     iEndClm = iStaClm + (lp2 - lpToken); lpToken = lp2;

	     REASSIGN(); pYys->m_TokenInfo.m_iTokenType = NODE_STRING_CONST;
	     return STRING_CONSTANT;
	   } 
	  else { REASSIGN(); return	'"'; }   
	}

   if( *lpToken == 39 )
	{
      if( !(!*(lpToken+1) || *(lpToken+2) != 39) ) 
       {
	     iEndClm += 2, lpToken += 2;

	     REASSIGN(); pYys->m_TokenInfo.m_iTokenType = NODE_CHARACTER_CONST;
	     pYys->m_TokenInfo.m_cChar = *(lpToken - 2);
	     return CHARACTER_CONSTANT;
	   }
	  else { REASSIGN(); return	39; }   
	}

   if( IsAplhaMy((unsigned char)*lpToken) )
	{
	  char cKey = *lpToken;
      LPSTR lp1 = lpToken;
	  LPSTR lp2 = FindNotAlphaDig( lpToken + 1 );
	  iEndClm = iStaClm + (lp2 - lp1) - 1; lpToken = lp2 - 1;
	  int iSz = iEndClm - iStaClm + 1;

	  REASSIGN(); 
      int iTr;
	  if( !(iTr=_strnicmp(lp1,"TRUE", 4)) || !_strnicmp(lp1,"FALSE", 5) )
	   {
	     pYys->m_TokenInfo.m_iTokenType = NODE_BOOL_CONST;
		 pYys->m_TokenInfo.m_bBool = !iTr ? true:false;
		 if( (pYys->m_TokenInfo.m_bBool == true && iSz == 4) ||
		     (pYys->m_TokenInfo.m_bBool == false && iSz == 5)
           )
	       return BOOL_CONSTANT;
	   }

	  pYys->m_TokenInfo.m_iTokenType = NODE_IDENTIFIER;
	  return IDENTIFIER;
	}

   if( isdigit(*lpToken) )
	{
	  LPSTR lp1 = lpToken;
	  LPSTR lp2 = NULL;

	  LPSTR lpZP = strchr(lpToken, ',');
	  LPSTR lpPT = strchr(lpToken, '.');
	  if( lpZP ) *lpZP = '.';
	  if( lpPT ) *lpPT = ',';

      double dbl = strtod( lpToken, &lp2 );

	  if( lpZP ) *lpZP = ',';
	  if( lpPT ) *lpPT = '.';
	  
	  iEndClm = iStaClm + (lp2 - lp1) - 1; lpToken = --lp2;

	  bool bFlFound = false;
	  for( ;lp1 <= lp2; lp1++ ) 
	    if( !isdigit(*lp1) ) { bFlFound = true; break; }

      REASSIGN(); 
	  if( bFlFound == true )
	   {
	     pYys->m_TokenInfo.m_iTokenType = NODE_FLOAT_CONST;
		 pYys->m_TokenInfo.m_dDouble = dbl;
	     return FLOAT_CONSTANT;
	   }
	  else
	   {
	     pYys->m_TokenInfo.m_iTokenType = NODE_INTEGER_CONST;
		 pYys->m_TokenInfo.m_iInt = dbl;
	     return INTEGER_CONSTANT;
	   }
	}

   if( *lpToken == '|' || *lpToken == '&' || *lpToken == '='  )
	{
	  char cKey = *lpToken;
      if( *(lpToken + 1) == *lpToken )
	   {	     
         iEndClm++, lpToken++;
		 REASSIGN();
		 switch( cKey )
		  {
		    case '|':
			  return OROR;
			case '&':
			  return ANDAND;
			case '=':
			  return EQU;
		  };
	   }	  
	}

   if( *lpToken == '!' )
	{
      char cKey = *lpToken;
      if( *(lpToken + 1) == '=' )
	   {	     
         iEndClm++, lpToken++;
		 REASSIGN();		 
		 return NEQ;		 
	   }	  
	}

   if( *lpToken == '<' )
	{
      char cKey = *lpToken;
      if( *(lpToken + 1) == '=' )
	   {	     
         iEndClm++, lpToken++;
		 REASSIGN();		 
		 return LEQ;		 
	   }	  
	}

   if( *lpToken == '>' )
	{
      char cKey = *lpToken;
      if( *(lpToken + 1) == '=' )
	   {	     
         iEndClm++, lpToken++;
		 REASSIGN();		 
		 return GEQ;		 
	   }	  
	}
 
   char cKey = *lpToken;
   REASSIGN();
   if( cKey == '-' || cKey == '!' )
	{
	  pYys->m_TokenInfo.m_iTokenType = NODE_UNARY_OPERATOR;
	  pYys->m_TokenInfo.m_cChar = cKey;
	}
   return cKey; 
 }

void ThrowIllegalSimplify( LPCSTR lp )
 {
   TParseException *pE = new TParseException( true, 
	     PRS_IllegalSimplify, lp );
   throw pE;
 }


int TExpression::Simplify_Constant( int iTyp, YYSTYPE *yysTarg_, YYSTYPE *yysTarg1, 
	                      YYLTYPE *yylTarg1 )
 {
   CString strName; m_rNsp.GetUnicname( strName );
   TVariantVar *pVar;

   switch( iTyp )
	{
	  case TS_STRING_CONSTANT:
	   {
	     CString str;
		 int iS = yylTarg1->m_iEndClm - 
		          yylTarg1->m_iStaClm - 1;
		 if( iS < 1 ) str = "";
		 else
		  {
            LPSTR lp = str.GetBuffer( iS + 1 );
			strncpy( lp, 
			  &((LPSTR)(LPCSTR)m_strExpr)[yylTarg1->m_iStaClm + 1], iS );
			str.ReleaseBuffer( iS );
		  }		 
		 pVar = new TVariantVar( str, (LPSTR)(LPCSTR)strName );		 		 
	   }
	   break;

	  case TS_BOOL_CONSTANT:
	    pVar = new TVariantVar( yysTarg1->m_TokenInfo.m_bBool, (LPSTR)(LPCSTR)strName );
	    break;
	  case TS_FLOAT_CONSTANT:
	    pVar = new TVariantVar( yysTarg1->m_TokenInfo.m_dDouble, (LPSTR)(LPCSTR)strName );
	    break;
	  case TS_CHARACTER_CONSTANT:
	    pVar = new TVariantVar( yysTarg1->m_TokenInfo.m_cChar, (LPSTR)(LPCSTR)strName );
	    break;
	  case TS_INTEGER_CONSTANT:
	    pVar = new TVariantVar( yysTarg1->m_TokenInfo.m_iInt, (LPSTR)(LPCSTR)strName );
	    break;

      default:
	    m_strError.Format( "Свёртка 'Constant' вызвана ошибочно" );
	    return 1;
	}

   pVar = m_rNsp.AddVarConst( pVar );
   TNodeVar *pNod = new TNodeVar( *pVar, true );
   m_lstGarbage.push_back( pNod );

   yysTarg_->m_TokenInfo.m_iTokenType = NODE_NODE;
   yysTarg_->m_TokenInfo.m_pNode = pNod;

   return 0;
 }

int TExpression::Simplify_ArgumentList( int iTyp, YYSTYPE *yysTarg_, 
	 YYSTYPE *yysTarg1, YYSTYPE *yysTarg2, YYSTYPE *yysTarg3,	 
	 YYLTYPE *yylTarg1, YYLTYPE *yylTarg2, YYLTYPE *yylTarg3 )
 {
   switch( iTyp )
	{
	  case TS_ArgumentList_ZP_Expression:
	   {
	     if( yysTarg1->m_TokenInfo.m_iTokenType != NODE_NODE ||
		     yysTarg3->m_TokenInfo.m_iTokenType != NODE_NODE
		   )
		  {
		    bool bFl = yysTarg1->m_TokenInfo.m_iTokenType == NODE_NODE2 ||
		               yysTarg3->m_TokenInfo.m_iTokenType == NODE_NODE2;
		    m_strError.Format( "Свёртка 'ArgumentList:TS_Expression' не может свернуть %s",
			 bFl == true ? "список более 2-х аргументов":"не узел" );
	        return 1;
		  }
         yysTarg_->m_TokenInfo.m_iTokenType = NODE_NODE2;
         yysTarg_->m_TokenInfo.m_pNode = yysTarg1->m_TokenInfo.m_pNode;
		 yysTarg_->m_TokenInfo.m_pNode2 = yysTarg3->m_TokenInfo.m_pNode;
	   }
	  break;

	  case TS_Expression:
	   {
	     if( yysTarg1->m_TokenInfo.m_iTokenType != NODE_NODE )
		  {
		    bool bFl = yysTarg1->m_TokenInfo.m_iTokenType == NODE_NODE2;
		    m_strError.Format( "Свёртка 'ArgumentList:TS_Expression' не может свернуть %s",
			 bFl == true ? "список более 2-х аргументов":"не узел" );
	        return 1;
		  }
         yysTarg_->m_TokenInfo.m_iTokenType = NODE_NODE;
         yysTarg_->m_TokenInfo.m_pNode = yysTarg1->m_TokenInfo.m_pNode;
	   }
	  break;

	  default:
	    m_strError.Format( "Свёртка 'ArgumentList' вызвана ошибочно" );
	    return 1;
	}

   return 0;
 }

int TExpression::Simplify_PrimaryExpression( int iTyp, YYSTYPE *yysTarg_, 
	 YYSTYPE *yysTarg1, YYSTYPE *yysTarg2, YYSTYPE *yysTarg3,	 
	 YYLTYPE *yylTarg1, YYLTYPE *yylTarg2, YYLTYPE *yylTarg3 )
 {
   switch( iTyp )
	{
	  case TS_BR_Expression_BR:
	   {
	     if( yysTarg2->m_TokenInfo.m_iTokenType != NODE_NODE )
		  {		 
		    m_strError.Format( "Свёртка 'PrimaryExpression:TS_BR_Expression_BR' не может свернуть не узел" );
	        return 1;
		  }
         yysTarg_->m_TokenInfo.m_iTokenType = NODE_NODE;
         yysTarg_->m_TokenInfo.m_pNode = yysTarg2->m_TokenInfo.m_pNode;		 
	   }
	  break;

	  case TS_Constant:
	   {
	     if( yysTarg1->m_TokenInfo.m_iTokenType != NODE_NODE )
		  {
		    m_strError.Format( "Свёртка 'PrimaryExpression:TS_Constant' не может свернуть не узел" );
	        return 1;
		  }
         yysTarg_->m_TokenInfo.m_iTokenType = NODE_NODE;
         yysTarg_->m_TokenInfo.m_pNode = yysTarg1->m_TokenInfo.m_pNode;
	   }
	  break;

	  case TS_Identifier:
	   yysTarg_->m_TokenInfo.m_iTokenType = NODE_IDENTIFIER;        
	   yysTarg_->m_TokenInfo.m_dDouble = yylTarg1->m_iStaClm;
       yysTarg_->m_TokenInfo.m_iInt = yylTarg1->m_iEndClm;
	   break;	   	  

	  default:
	    m_strError.Format( "Свёртка 'PrimaryExpression' вызвана ошибочно" );
	    return 1;
	}

   return 0;
 }

int TExpression::Simplify_PostfixExpression( int iTyp, YYSTYPE *yysTarg_, 
	 YYSTYPE *yysTarg1, YYSTYPE *yysTarg2, YYSTYPE *yysTarg3, YYSTYPE *yysTarg4,	 
	 YYLTYPE *yylTarg1, YYLTYPE *yylTarg2, YYLTYPE *yylTarg3, YYLTYPE *yylTarg4 )
 {
   switch( iTyp )
	{
	  case TS_PostfixExpress_BR_ArgList_BR:
	   {
	     if( yysTarg1->m_TokenInfo.m_iTokenType != NODE_IDENTIFIER )
		  {		 
		    m_strError.Format( "У свёртки 'PostfixExpression:TS_PostfixExpress_BR_ArgList_BR' первый аргумент должен быть IDENTIFIER" );
	        return 1;
		  }
		 if( yysTarg3->m_TokenInfo.m_iTokenType != NODE_NODE &&
		     yysTarg3->m_TokenInfo.m_iTokenType != NODE_NODE2
		   )
		  {		 
		    m_strError.Format( "У свёртки 'PostfixExpression:TS_PostfixExpress_BR_ArgList_BR' третий аргумент должен быть Argument_List" );
	        return 1;
		  }

         CString str;
		 int iS = yysTarg1->m_TokenInfo.m_iInt - 
		          yysTarg1->m_TokenInfo.m_dDouble + 1;
		 if( iS < 1 ) str = "";
		 else
		  {
            LPSTR lp = str.GetBuffer( iS + 1 );
			strncpy( lp, 
			  &((LPSTR)(LPCSTR)m_strExpr)[(int)yysTarg1->m_TokenInfo.m_dDouble], iS );
			str.ReleaseBuffer( iS );
		  }	
		 TFuncInfo* pFInf = TNodeFunction::FindFuncPtr( str );
		 if( !pFInf )
		  {
            m_strError.Format( "Ошибочная функция '%s'", (LPSTR)(LPCSTR)str );
	        return 1;
		  }
		 if( pFInf->m_iNumbArg == 1 && yysTarg3->m_TokenInfo.m_iTokenType == NODE_NODE2 ||
		     pFInf->m_iNumbArg == 2 && yysTarg3->m_TokenInfo.m_iTokenType == NODE_NODE
		   )
		  {
		    m_strError.Format( "Ошибочное число аргументов функции '%s'", (LPSTR)(LPCSTR)str );
	        return 1;
		  }
		 TNodeFunction *pNF = new TNodeFunction( pFInf->m_iTyp,
		   yysTarg3->m_TokenInfo.m_pNode, 
		   yysTarg3->m_TokenInfo.m_iTokenType == NODE_NODE2 ?
		      yysTarg3->m_TokenInfo.m_pNode2:NULL );

		 yysTarg_->m_TokenInfo.m_iTokenType = NODE_NODE;
         yysTarg_->m_TokenInfo.m_pNode = pNF;

		 m_lstGarbage.push_back( pNF );		  
	   }
	  break;	  

	  case TS_PrimaryExpression:
	    memcpy( &yysTarg_->m_TokenInfo, &yysTarg1->m_TokenInfo, sizeof(TTokenInfo) );
		break;

	  default:
	    m_strError.Format( "Свёртка 'PrimaryExpression' вызвана ошибочно" );
	    return 1;
	}

   return 0;
 }

int TExpression::Simplify_UnaryOperator( int iTyp, YYSTYPE *yysTarg_, 
	                           YYSTYPE *yysTarg1, 
							   YYLTYPE *yylTarg1 )
 {
   switch( iTyp )
	{
	  case TS_NE:	   
	   yysTarg_->m_TokenInfo.m_iTokenType = NODE_UNARY_OPERATOR; 
	   yysTarg_->m_TokenInfo.m_cChar = '!';
	   break;	  

	  case TS_UNMINUS:	   
	   yysTarg_->m_TokenInfo.m_iTokenType = NODE_UNARY_OPERATOR; 
	   yysTarg_->m_TokenInfo.m_cChar = '-';
	   break;	  

      default:
	    m_strError.Format( "Свёртка 'UnaryOperator' вызвана ошибочно" );
	    return 1;
	}   
   
   return 0;
 }

int TExpression::Simplify_UnaryExpression( int iTyp, YYSTYPE *yysTarg_, 
	 YYSTYPE *yysTarg1, YYSTYPE *yysTarg2,	 
	 YYLTYPE *yylTarg1, YYLTYPE *yylTarg2 )
 {   
   
   switch( iTyp )
	{
	  case TS_PostfixExpression:
	   {
         if( yysTarg1->m_TokenInfo.m_iTokenType == NODE_IDENTIFIER )
		  {
		    CString str;
		    int iS = yysTarg1->m_TokenInfo.m_iInt - 
		             yysTarg1->m_TokenInfo.m_dDouble + 1;
		     if( iS < 1 ) str = "";
		     else
			  {
                LPSTR lp = str.GetBuffer( iS + 1 );
			    strncpy( lp, 
			      &((LPSTR)(LPCSTR)m_strExpr)[(int)yysTarg1->m_TokenInfo.m_dDouble], iS );
			    str.ReleaseBuffer( iS );
			  }	
		     TFuncInfo* pFInf = TNodeFunction::FindFuncPtr( str );
			 if( pFInf )
			  {
                m_strError.Format( "Имя переменной совпадает с именем зарезервированным для функции '%s'", (LPSTR)(LPCSTR)str );
	            return 1;
			  }
		     TVariantVar *pVar = m_rNsp.AddVar( (LPSTR)(LPCSTR)str );
			 TNodeVar *pNV = new TNodeVar( *pVar, false );

			 yysTarg_->m_TokenInfo.m_iTokenType = NODE_NODE; 
	         yysTarg_->m_TokenInfo.m_pNode = pNV;			  
			 m_lstGarbage.push_back( pNV );
		  }
		 else if( yysTarg1->m_TokenInfo.m_iTokenType == NODE_NODE )
		  {
		    yysTarg_->m_TokenInfo.m_iTokenType = NODE_NODE; 
	        yysTarg_->m_TokenInfo.m_pNode = yysTarg1->m_TokenInfo.m_pNode;
		  }
		 else
		  {
		    m_strError.Format( "Свёртка 'UnaryExpression': нужен идентификатор либо узел" );
	        return 1;
		  }	     		 
	   }
	   break;

	  case TS_UnaryOperator_UnaryExpression:
	   {
         if( yysTarg1->m_TokenInfo.m_iTokenType != NODE_UNARY_OPERATOR )
		  {		 
		    m_strError.Format( "У свёртки 'UnaryExpression:TS_UnaryOperator_UnaryExpression' первый аргумент должен быть UNARY_OP" );
	        return 1;
		  }
		 if( yysTarg2->m_TokenInfo.m_iTokenType != NODE_NODE )
		  {		 
		    m_strError.Format( "У свёртки 'UnaryExpression:TS_UnaryOperator_UnaryExpression' второй аргумент должен быть узлом" );
	        return 1;
		  }
		 TNodeFunction *pNF = new TNodeFunction( 
		   yysTarg1->m_TokenInfo.m_cChar == '!' ? FT_NE:FT_UNMINUS,
		   yysTarg2->m_TokenInfo.m_pNode, NULL );		   

		 yysTarg_->m_TokenInfo.m_iTokenType = NODE_NODE;
         yysTarg_->m_TokenInfo.m_pNode = pNF;

		 m_lstGarbage.push_back( pNF );		  
	   }
	  break;

	  default:
	    m_strError.Format( "Свёртка 'UnaryExpression' вызвана ошибочно" );
	    return 1;
	}

   return 0;
 }

int TExpression::Simplify_MultiplicativeExpr( int iTyp, YYSTYPE *yysTarg_, 
	 YYSTYPE *yysTarg1, YYSTYPE *yysTarg2, YYSTYPE *yysTarg3,	 
	 YYLTYPE *yylTarg1, YYLTYPE *yylTarg2, YYLTYPE *yylTarg3 )
 {
   TNodeFunction *pNF;
   switch( iTyp )
	{
	  case TS_ME_MOD_UnaryExpression:	   
	    pNF = new TNodeFunction( FT_MOD, 
		  yysTarg1->m_TokenInfo.m_pNode, yysTarg3->m_TokenInfo.m_pNode );		   		  
	   break;	  	  

	  case TS_ME_DIV_UnaryExpression:	   
	    pNF = new TNodeFunction( FT_DIV, 
		  yysTarg1->m_TokenInfo.m_pNode, yysTarg3->m_TokenInfo.m_pNode );		   		  
	   break;	  	  

	  case TS_ME_MUL_UnaryExpression:	   
	    pNF = new TNodeFunction( FT_MUL, 
		  yysTarg1->m_TokenInfo.m_pNode, yysTarg3->m_TokenInfo.m_pNode );		   		  
	   break;	  	  

	  case TS_UnaryExpression:	   
	   {
	     if( yysTarg_->m_TokenInfo.m_iTokenType != NODE_NODE )
		  {
		    m_strError.Format( "У свёртки 'MultiplicativeExpr:TS_UnaryExpression' аргумент должен быть узлом" );
	        return 1;
		  }
	     yysTarg_->m_TokenInfo.m_iTokenType = NODE_NODE; 
	     yysTarg_->m_TokenInfo.m_pNode = yysTarg1->m_TokenInfo.m_pNode;
	     return 0;
	   }	    

      default:
	    m_strError.Format( "Свёртка 'MultiplicativeExpr' вызвана ошибочно" );
	    return 1;
	}   

   yysTarg_->m_TokenInfo.m_iTokenType = NODE_NODE;
   yysTarg_->m_TokenInfo.m_pNode = pNF;

   m_lstGarbage.push_back( pNF );		  

   return 0;
 }

int TExpression::Simplify_AdditiveExpr( int iTyp, YYSTYPE *yysTarg_, 
	 YYSTYPE *yysTarg1, YYSTYPE *yysTarg2, YYSTYPE *yysTarg3,	 
	 YYLTYPE *yylTarg1, YYLTYPE *yylTarg2, YYLTYPE *yylTarg3 )
 {
   TNodeFunction *pNF;
   switch( iTyp )
	{
	  case TS_AE_MINUS_ME:	   
	    pNF = new TNodeFunction( FT_MINUS, 
		  yysTarg1->m_TokenInfo.m_pNode, yysTarg3->m_TokenInfo.m_pNode );		   		  
	   break;	  	  

	  case TS_AE_PLUS_ME:	   
	    pNF = new TNodeFunction( FT_PLUS, 
		  yysTarg1->m_TokenInfo.m_pNode, yysTarg3->m_TokenInfo.m_pNode );		   		  
	   break;	  	  	  

	  case TS_Multiplicative_Expression:	   
	   {
	     if( yysTarg_->m_TokenInfo.m_iTokenType != NODE_NODE )
		  {
		    m_strError.Format( "У свёртки 'AdditiveExpr:TS_Multiplicative_Expression' аргумент должен быть узлом" );
	        return 1;
		  }
	     yysTarg_->m_TokenInfo.m_iTokenType = NODE_NODE; 
	     yysTarg_->m_TokenInfo.m_pNode = yysTarg1->m_TokenInfo.m_pNode;
	     return 0;
	   }	    

      default:
	    m_strError.Format( "Свёртка 'AdditiveExpr' вызвана ошибочно" );
	    return 1;
	}   

   yysTarg_->m_TokenInfo.m_iTokenType = NODE_NODE;
   yysTarg_->m_TokenInfo.m_pNode = pNF;

   m_lstGarbage.push_back( pNF );		  

   return 0;
 }

int TExpression::Simplify_RelationalExpr( int iTyp, YYSTYPE *yysTarg_, 
	 YYSTYPE *yysTarg1, YYSTYPE *yysTarg2, YYSTYPE *yysTarg3,	 
	 YYLTYPE *yylTarg1, YYLTYPE *yylTarg2, YYLTYPE *yylTarg3 )
 {
   TNodeFunction *pNF;
   switch( iTyp )
	{
	  case TS_RE_GEQ_AE:	   
	    pNF = new TNodeFunction( FT_GEQ, 
		  yysTarg1->m_TokenInfo.m_pNode, yysTarg3->m_TokenInfo.m_pNode );		   		  
	   break;	  	  

	  case TS_RE_LEQ_AE:	   
	    pNF = new TNodeFunction( FT_LEQ, 
		  yysTarg1->m_TokenInfo.m_pNode, yysTarg3->m_TokenInfo.m_pNode );		   		  
	   break;	  	  

	  case TS_RE_GE_AE:	   
	    pNF = new TNodeFunction( FT_GE, 
		  yysTarg1->m_TokenInfo.m_pNode, yysTarg3->m_TokenInfo.m_pNode );		   		  
	   break;	  	  

	  case TS_RE_LE_AE:	   
	    pNF = new TNodeFunction( FT_LE, 
		  yysTarg1->m_TokenInfo.m_pNode, yysTarg3->m_TokenInfo.m_pNode );		   		  
	   break;	  	  

	  case TS_Additive_Expression:	   
	   {
	     if( yysTarg_->m_TokenInfo.m_iTokenType != NODE_NODE )
		  {
		    m_strError.Format( "У свёртки 'RelationalExpr:TS_Additive_Expression' аргумент должен быть узлом" );
	        return 1;
		  }
	     yysTarg_->m_TokenInfo.m_iTokenType = NODE_NODE; 
	     yysTarg_->m_TokenInfo.m_pNode = yysTarg1->m_TokenInfo.m_pNode;
	     return 0;
	   }	    

      default:
	    m_strError.Format( "Свёртка 'RelationalExpr' вызвана ошибочно" );
	    return 1;
	}   

   yysTarg_->m_TokenInfo.m_iTokenType = NODE_NODE;
   yysTarg_->m_TokenInfo.m_pNode = pNF;

   m_lstGarbage.push_back( pNF );		  

   return 0;
 }

int TExpression::Simplify_EqualityExpr( int iTyp, YYSTYPE *yysTarg_, 
	 YYSTYPE *yysTarg1, YYSTYPE *yysTarg2, YYSTYPE *yysTarg3,	 
	 YYLTYPE *yylTarg1, YYLTYPE *yylTarg2, YYLTYPE *yylTarg3 )
 {
   TNodeFunction *pNF;
   switch( iTyp )
	{
	  case TS_EE_NEQ_RE:	   
	    pNF = new TNodeFunction( FT_NEQ, 
		  yysTarg1->m_TokenInfo.m_pNode, yysTarg3->m_TokenInfo.m_pNode );		   		  
	   break;	  	  

	  case TS_EE_EQ_RE:	   
	    pNF = new TNodeFunction( FT_EQ, 
		  yysTarg1->m_TokenInfo.m_pNode, yysTarg3->m_TokenInfo.m_pNode );		   		  
	   break;	  	  	  

	  case TS_Relational_Expression:	   
	   {
	     if( yysTarg_->m_TokenInfo.m_iTokenType != NODE_NODE )
		  {
		    m_strError.Format( "У свёртки 'EqualityExpr:TS_Relational_Expression' аргумент должен быть узлом" );
	        return 1;
		  }
	     yysTarg_->m_TokenInfo.m_iTokenType = NODE_NODE; 
	     yysTarg_->m_TokenInfo.m_pNode = yysTarg1->m_TokenInfo.m_pNode;
	     return 0;
	   }	    

      default:
	    m_strError.Format( "Свёртка 'EqualityExpr' вызвана ошибочно" );
	    return 1;
	}   

   yysTarg_->m_TokenInfo.m_iTokenType = NODE_NODE;
   yysTarg_->m_TokenInfo.m_pNode = pNF;

   m_lstGarbage.push_back( pNF );		  
   return 0;
 }

int TExpression::Simplify_BooleanANDExpr( int iTyp, YYSTYPE *yysTarg_, 
	 YYSTYPE *yysTarg1, YYSTYPE *yysTarg2, YYSTYPE *yysTarg3,	 
	 YYLTYPE *yylTarg1, YYLTYPE *yylTarg2, YYLTYPE *yylTarg3 )
 {
   TNodeFunction *pNF;
   switch( iTyp )
	{
	  case TS_BAE_ANDAND_EE:	   
	    pNF = new TNodeFunction( FT_ANDAND, 
		  yysTarg1->m_TokenInfo.m_pNode, yysTarg3->m_TokenInfo.m_pNode );		   		  
	   break;	  	  
	  
	  case TS_Equality_Expression:	   
	   {
	     if( yysTarg_->m_TokenInfo.m_iTokenType != NODE_NODE )
		  {
		    m_strError.Format( "У свёртки 'BooleanANDExpr:TS_Equality_Expression' аргумент должен быть узлом" );
	        return 1;
		  }
	     yysTarg_->m_TokenInfo.m_iTokenType = NODE_NODE; 
	     yysTarg_->m_TokenInfo.m_pNode = yysTarg1->m_TokenInfo.m_pNode;
	     return 0;
	   }	    

      default:
	    m_strError.Format( "Свёртка 'BooleanANDExpr' вызвана ошибочно" );
	    return 1;
	}   

   yysTarg_->m_TokenInfo.m_iTokenType = NODE_NODE;
   yysTarg_->m_TokenInfo.m_pNode = pNF;

   m_lstGarbage.push_back( pNF );		  
   return 0;
 }

int TExpression::Simplify_BooleanORExpr( int iTyp, YYSTYPE *yysTarg_, 
	 YYSTYPE *yysTarg1, YYSTYPE *yysTarg2, YYSTYPE *yysTarg3,	 
	 YYLTYPE *yylTarg1, YYLTYPE *yylTarg2, YYLTYPE *yylTarg3 )
 {
   TNodeFunction *pNF;
   switch( iTyp )
	{
	  case TS_BOE_OROR_BAE:	   
	    pNF = new TNodeFunction( FT_OROR, 
		  yysTarg1->m_TokenInfo.m_pNode, yysTarg3->m_TokenInfo.m_pNode );		   		  
	   break;	  	  
	  
	  case TS_Boolean_AND_Expression:	   
	   {
	     if( yysTarg_->m_TokenInfo.m_iTokenType != NODE_NODE )
		  {
		    m_strError.Format( "У свёртки 'BooleanORExpr:TS_Boolean_AND_Expression' аргумент должен быть узлом" );
	        return 1;
		  }
	     yysTarg_->m_TokenInfo.m_iTokenType = NODE_NODE; 
	     yysTarg_->m_TokenInfo.m_pNode = yysTarg1->m_TokenInfo.m_pNode;
	     return 0;
	   }	    

      default:
	    m_strError.Format( "Свёртка 'BooleanORExpr' вызвана ошибочно" );
	    return 1;
	}   

   yysTarg_->m_TokenInfo.m_iTokenType = NODE_NODE;
   yysTarg_->m_TokenInfo.m_pNode = pNF;

   m_lstGarbage.push_back( pNF );		  
   return 0;
 }

int TExpression::Simplify_Expression( int iTyp, YYSTYPE *yysTarg_, 
	 YYSTYPE *yysTarg1, YYSTYPE *yysTarg2, YYSTYPE *yysTarg3,	 
	 YYLTYPE *yylTarg1, YYLTYPE *yylTarg2, YYLTYPE *yylTarg3 )
 {
   TNodeFunction *pNF;
   switch( iTyp )
	{
	  case TS_UnaryExpression_EQ_Expression:	   
	    pNF = new TNodeFunction( FT_OPEQ, 
		  yysTarg1->m_TokenInfo.m_pNode, yysTarg3->m_TokenInfo.m_pNode );		   		  
	   break;	  	  
	  
	  case TS_Boolean_OR_Expression:	   
	   {
	     if( yysTarg_->m_TokenInfo.m_iTokenType != NODE_NODE )
		  {
		    m_strError.Format( "У свёртки 'Expression:TS_Boolean_OR_Expression' аргумент должен быть узлом" );
	        return 1;
		  }
	     yysTarg_->m_TokenInfo.m_iTokenType = NODE_NODE; 
	     yysTarg_->m_TokenInfo.m_pNode = yysTarg1->m_TokenInfo.m_pNode;
	     return 0;
	   }	    

      default:
	    m_strError.Format( "Свёртка 'Expression' вызвана ошибочно" );
	    return 1;
	}   

   yysTarg_->m_TokenInfo.m_iTokenType = NODE_NODE;
   yysTarg_->m_TokenInfo.m_pNode = pNF;

   m_lstGarbage.push_back( pNF );		  

   return 0;
 }
