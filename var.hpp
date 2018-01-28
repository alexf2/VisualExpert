#if !defined(_VAR_CALC_)
#define _VAR_CALC_

#include "stdafx.h"
#include "autorc.hpp"
#include "exp_tab_.hpp"

#include <vector>
#include <list>
using namespace std;

#include "..\\RUDAKOW\\SERVIS\\myexc.hpp"

//Реестр субкодов исключений TMyException !!!
#include "..\\RUDAKOW\\SERVIS\\subcode.hpp"

class TVariantVar;

BOOL ReadConstant( TVariantVar& rV, LPCSTR lpStr );


enum VAREXC { VEX_NotInitialized = 0, VEX_BadType = 1, VEX_NoID = 2, 
              VEX_Unknown = 3, VEX_OperDisabledForTyp = 4 };
class TVarException: public TMyException
 {

  enum { TSC_Subcode = TREG_TVarException }; //! - у каждого субкласса должен быть свой

public:
  TVarException( BOOL bFlDel, int iExcCode, const TVariantVar* pV1, 
                 const TVariantVar* pV2 = NULL );

  virtual int GetSubCode() const;
   

  TVariantVar *p1, *p2;    
 };


#define NUMB_TYP 5
enum TVariantTypes { VAR_Bool   = 0, 
                     VAR_Char   = 1,
					 VAR_Int    = 2,
					 VAR_Double = 3,
                     VAR_Str    = 4, //the highest power                                                                                   
					 VAR_Undef  = 5
                  };

struct TCompatible
 {
   int m_iType;
   int m_Compat[NUMB_TYP];
 };

struct TCompatible_ForFunc
 {
   int m_iFunc;
   int m_Compat[NUMB_TYP]; //-2 - any arg.
   int m_iTypeRes; //если код типа, то всегда этот тип, если -1,то тип аргументов приведённый к максимальному   
   void    (*pFunc)(TVariantVar* pRes, TVariantVar* p1, TVariantVar* p2=NULL);   
   int m_iArn;
 };


#define NUMB_BINARY_OPER 7
#define NUMB_UNARY_OPER  2
#define NUMB_OPER_MAX    7
struct TEnableOper
 {
   int m_iType;
   int m_Oper[NUMB_OPER_MAX];
 };


class TVariantVar
 {
public:
  TVariantVar();   
  explicit TVariantVar( TVariantTypes iType, char* lpName = NULL );


  TVariantVar( LPSTR lpStr, char* lpName ): 
    m_Str(lpStr), m_iType(VAR_Str), m_strName(lpName)  
   { IniFlags(); m_bFIni = true; };

  TVariantVar( LPCSTR lpStr, char* lpName ): 
    m_Str(lpStr), m_iType(VAR_Str), m_strName(lpName)  
   { IniFlags(); m_bFIni = true; };

  TVariantVar( char* lpStr, int iLen, char* lpName );  


  TVariantVar( char chr, char* lpName ): 
    m_Char(chr), m_iType(VAR_Char), m_strName(lpName)  
   { IniFlags(); m_bFIni = true; };

  TVariantVar( int iInt, char* lpName ): 
    m_Int(iInt), m_iType(VAR_Int), m_strName(lpName)  
   { IniFlags(); m_bFIni = true; };

  TVariantVar( double dDbl, char* lpName ): 
    m_Double(dDbl), m_iType(VAR_Double), m_strName(lpName) 
   { IniFlags(); m_bFIni = true; };

  TVariantVar( bool bBool, char* lpName ): 
    m_Bool(bBool), m_iType(VAR_Bool), m_strName(lpName) 
   { IniFlags(); m_bFIni = true; };


  explicit TVariantVar( LPSTR lpStr );
  explicit TVariantVar( LPCSTR lpStr );
  TVariantVar( char ch );
  TVariantVar( bool bv );
  TVariantVar( int iv );
  TVariantVar( double dv );


  TVariantVar( const TVariantVar& rV )
   {
     m_iType = VAR_Undef;
	 m_bFIni = false;
     operator=( rV );
   }
  

  TVariantVar& operator=( const TVariantVar& );  

  enum TPreds { TP_EQ=0, TP_NEQ, TP_LE, TP_GE, TP_LEQ, TP_GEQ };
  void Predicat( const TVariantVar& rV, int iPred, TVariantVar& rRes );

  TVariantVar operator==( const TVariantVar& rV );  
  TVariantVar operator!=( const TVariantVar& rV );  
  TVariantVar operator<( const TVariantVar& rV );  
  TVariantVar operator>( const TVariantVar& rV );  
  TVariantVar operator<=( const TVariantVar& rV );  
  TVariantVar operator>=( const TVariantVar& rV );
  

  enum TOperSpec { OPEQ_EQ = 99 };
  enum TOper { OP_PLUS=20, OP_MINUS, OP_MUL, OP_DIV, OP_MOD };
  enum TOperLogic { OP_OROR=30, OP_ANDAND=31, OPL_END_=32 };
  bool IsLogic( int iOp ) const 
   { return iOp >= OP_OROR && iOp < OPL_END_; }
  void Operacion( const TVariantVar& rV, int iOp, TVariantVar& rRes );

  TVariantVar operator+( const TVariantVar& rV );
  TVariantVar operator-( const TVariantVar& rV );
  TVariantVar operator*( const TVariantVar& rV );
  TVariantVar operator/( const TVariantVar& rV );
  TVariantVar operator%( const TVariantVar& rV );

  TVariantVar operator||( const TVariantVar& rV );
  TVariantVar operator&&( const TVariantVar& rV );

  enum TOperUnary { UN_MINUS=50, UN_NE };
  void OperacionUnary( int iOp, TVariantVar& rRes );

  TVariantVar operator-();
  TVariantVar operator!();


  int  m_iType;
  CString  m_Str;
  union {    
    char     m_Char;
    int      m_Int;
    double   m_Double;
    bool     m_Bool;
  };

  bool m_bFIni;
  CString m_strName;

// _______________
  double m_dKD;
  bool m_bCanceled;
  bool m_bNeed;
  bool m_bConst;
  bool m_bCanImplicat;

  bool m_bUsed;

  void IniFlags()
   {
     m_dKD = 1, m_bCanceled = false, m_bNeed = true;
	 m_bCanImplicat = false;
	 m_bConst = false;
	 m_bUsed = false;
   }
// _______________

  void ValToStr( CString&, int iFloatPrec = 8, int iModeDigit = 0 );  
   
  int m_iKeyLastOp; //for make error msg всегда в первом операнде

  static int CheckCompatibleTyp( TCompatible* pCompat, const TVariantVar& rV1, const TVariantVar& rV2, bool bMax );
  //return -2 - if not, -1 - if yes, TVariantTypes - if need conversion
  //bMax == true - need casting up, else down
  static void TypeCast( TVariantVar& varTmp, const TVariantVar& rV, int iType );

  static void ThrowNotInit( const TVariantVar& rV );
  static void ThrowNotDefined( const TVariantVar& rV );
  static void ThrowOperDisabledForTyp( const TVariantVar& rV );
  static void ThrowNotCompatible( const TVariantVar& rV1, const TVariantVar& rV2 );

  LPSTR GetTypeStr() const;
  LPSTR GetKeyOPStr() const;

  void UnInit()
   {
     m_bFIni = false;
   }
 };



enum TNodeTypes { TN_Var, TN_Func };

class TNode
 {
public:
   TNode() {};

   virtual ~TNode();

   virtual void CalcVal() = 0;
   virtual TVariantVar& Val() = 0;
   virtual int IsA() const = 0;
 };

class TNodeVar: public TNode
 {

friend class TNodeFunction;
friend class TExpression;

public:
  TNodeVar( TVariantVar& rVar, bool bFlConst ): TNode(),
    m_rVariable( rVar )
	 {
	   m_bConst = bFlConst;
	 }

   virtual ~TNodeVar();

   virtual void CalcVal();
   virtual TVariantVar& Val();
   virtual int IsA() const;

  TVariantVar& m_rVariable;
protected:  
  bool m_bConst;
 };


class TNodeFunction;
typedef void (TNodeFunction::* TPFunctional)();


enum FT_BinOp { FT_OPEQ=200, FT_PLUS, FT_MINUS, FT_MUL, FT_DIV, FT_MOD,
                FT_OROR, FT_ANDAND };

enum FT_UnOp { FT_UNMINUS=220, FT_NE };

enum FT_Pred { FT_EQ=230, FT_NEQ, FT_LE, FT_GE, FT_LEQ, FT_GEQ };

enum FT_BinFunc { FTF_MIN=250, FTF_MAX, FTF_POW, FTF_FMOD, 
                  FTF_CMPI, FTF_SCAS  };

enum FT_UnFunc { FTF_SIN=270, FTF_COS, FTF_TAN, FTF_ATAN,
                 FTF_ASIN, FTF_ACOS, FTF_LOG, FTF_LOG10,
				 FTF_EXP, FTF_SQRT, FTF_FLOOR, FTF_CEIL,
				 
				 FTF_Bool, FTF_Char, FTF_Int, FTF_Double, FTF_String };

struct TFuncInfo
 {
   TPFunctional  m_pF;
   int           m_iTyp;
   char          *m_pName;
   int           m_iNumbArg;
 };

enum NODEXC { NOD_BadNumberArg = 0, NOD_Unknown = 1, 
              NOD_AssInvalidNode = 2, NOD_AssToConst = 3 };
        
class TNodException: public TMyException
 {

  enum { TSC_Subcode = TREG_TNodException }; //! - у каждого субкласса должен быть свой

public:
  TNodException( BOOL bFlDel, int iExcCode, LPSTR lpName );    

  virtual int GetSubCode() const;
 };

 
class TNodeFunction: public TNode
 {
public:
  TNodeFunction( int iTyp, TNode* pN1, TNode* pN2 = NULL );

   virtual ~TNodeFunction();

   virtual void CalcVal();
   virtual TVariantVar& Val();

   void AssName( LPCSTR lp )
	{
      m_Value.m_strName = lp;
	}
   virtual int IsA() const;

   static TFuncInfo* FindFuncPtr( int iTyp );
   static TFuncInfo* FindFuncPtr( LPCSTR lpName );

   int GetTyp() const { return m_iType; }
   TNode *GetN1() { return m_pN1; }
   
protected:
  TNode *m_pN1, *m_pN2;
  TVariantVar m_Value;

  TPFunctional m_pFunc;
  int m_iType;  

  static TFuncInfo m_fiInfo[];

  void CheckArguments( int iNumb );
  void ThrowBadNumArguments();

  void ThrowAssignInvalidNode();
  void ThrowAssignConst();

  void MakeFunc( int iOp );

  void BinOp_OPEQ(); // =
  void BinOp_PLUS(); // +
  void BinOp_MINUS(); // -
  void BinOp_MUL(); // *
  void BinOp_DIV(); // /
  void BinOp_MOD(); // %
  void BinOp_OROR(); // ||
  void BinOp_ANDAND(); // &&

  void UnOp_MINUS(); // -
  void UnOp_NE(); // !

  void Pred_EQ(); // ==
  void Pred_NEQ(); // !=
  void Pred_LE(); // <
  void Pred_GE(); // >
  void Pred_LEQ(); // <=
  void Pred_GEQ(); // >=

  void BinFunc_MIN(); // min
  void BinFunc_MAX(); // max
  void BinFunc_POW(); // pow
  void BinFunc_FMOD(); // fmod
  void BinFunc_CMPI(); // cmpi
  void BinFunc_SCAS(); // scas
  

  void UnFunc_SIN(); // sin
  void UnFunc_COS(); // cos
  void UnFunc_TAN(); // tan
  void UnFunc_ATAN(); // atan
  void UnFunc_ASIN(); // asin
  void UnFunc_ACOS(); // acos
  void UnFunc_LOG(); // log
  void UnFunc_LOG10(); // log10
  void UnFunc_EXP(); // exp
  void UnFunc_SQRT(); // sqrt
  void UnFunc_FLOOR(); // floor
  void UnFunc_CEIL(); // ceil

  void UnFunc_Bool(); // Bool()
  void UnFunc_Char(); // Char()
  void UnFunc_Int(); // Int()
  void UnFunc_Double(); // Double()
  void UnFunc_Str(); // Str()     
 };

class TNameSpace //case no sensitive
 {
public:
  TNameSpace() 
   { 
     m_iUnic = 1;
   }

  ~TNameSpace();

  TVariantVar* AddVar( LPSTR lpName, TVariantTypes iType = VAR_Undef );
  void AddVar( TVariantVar* ); //only named variable
  TVariantVar* AddVarConst( TVariantVar* );

  TVariantVar* FindVar( LPCSTR lpName );  
  TVariantVar* FindVar_ForVal( TVariantVar* );
  void GetUnicname( CString& );

  list<TVariantVar*>& GetLstRef() { return m_lstVar; }

protected:
  list<TVariantVar*> m_lstVar;
  int m_iUnic;
 };


#define NODE_STRING_CONST        0 //used location stack
#define NODE_BOOL_CONST          1
#define NODE_FLOAT_CONST         2
#define NODE_CHARACTER_CONST     3
#define NODE_INTEGER_CONST       4
#define NODE_IDENTIFIER          5 //used double(start) and int(end)
#define NODE_UNARY_OPERATOR      6 //used m_cChar for store operator

#define NODE_NODE                7
#define NODE_NODE2               8 //if list of two arguments
#define NODE_UNDEFINED           9

typedef struct
 {
   int m_iTokenType;
   union {
     char    m_cChar;
     double  m_dDouble;     
     bool    m_bBool;     
	 TNode   *m_pNode;
   };
   int     m_iInt;
   TNode   *m_pNode2;
 } TTokenInfo;


int yyparse( void* );
#ifdef YYSTYPE
#undef YYSTYPE
#endif
#ifdef YYLTYPE
#undef YYLTYPE
#endif
typedef union
 {
   int       m_iTokenType;
   TTokenInfo m_TokenInfo;
 } TYYSTYPE;
typedef struct
 {
   int m_iStaClm, m_iEndClm;
   int first_line, last_line;
   char *text;
 } TYYLTYPE;
#define first_column m_iStaClm
#define last_column m_iEndClm


#define YYSTYPE TYYSTYPE
#define YYLTYPE TYYLTYPE

enum PARSEXC { PRS_SyntaxErr = 0, PRS_ParserErr = 1, PRS_Unknown = 2,
               PRS_IllegalSimplify = 3 };
class TParseException: public TMyException
 {

  enum { TSC_Subcode = TREG_TParseException }; //! - у каждого субкласса должен быть свой

public:
  TParseException( BOOL bFlDel, int iExcCode, LPCSTR lp );    

  virtual int GetSubCode() const;
 };

enum TSimplifyTypes { 
                      //Constant
                      TS_STRING_CONSTANT, TS_BOOL_CONSTANT,
                      TS_FLOAT_CONSTANT, TS_CHARACTER_CONSTANT,
					  TS_INTEGER_CONSTANT, 
					  //Argument_List
					  TS_ArgumentList_ZP_Expression,
					  TS_Expression,
					  //Primary_Expression
					  TS_BR_Expression_BR,
					  TS_Constant,
					  TS_Identifier,
					  //Postfix_expression
					  TS_PostfixExpress_BR_ArgList_BR,
					  TS_PrimaryExpression,
					  //Unary_operator
					  TS_NE,
					  TS_UNMINUS,
					  //Unary_Expression
					  TS_UnaryOperator_UnaryExpression,
					  TS_PostfixExpression,
					  //Multiplicative_Expression
					  TS_ME_MOD_UnaryExpression,
					  TS_ME_DIV_UnaryExpression,
					  TS_ME_MUL_UnaryExpression,
                      TS_UnaryExpression,
					  //Additive_Expression
					  TS_AE_MINUS_ME,
					  TS_AE_PLUS_ME,
					  TS_Multiplicative_Expression,
					  //Relational_Expression
					  TS_RE_GEQ_AE,
					  TS_RE_LEQ_AE,
					  TS_RE_GE_AE,
					  TS_RE_LE_AE,
					  TS_Additive_Expression,
					  //Equality_Expression
					  TS_EE_NEQ_RE,
					  TS_EE_EQ_RE,
					  TS_Relational_Expression,
					  //Boolean_AND_Expression
					  TS_BAE_ANDAND_EE,
					  TS_Equality_Expression,
					  //Boolean_OR_Expression
					  TS_BOE_OROR_BAE,
					  TS_Boolean_AND_Expression,
					  //Expression
					  TS_UnaryExpression_EQ_Expression,
					  TS_Boolean_OR_Expression };

class TExpression
 {

friend int yyparse( void* );

public:
   TExpression( LPCSTR lpStr, TNameSpace& rNsp ): 
	  m_strExpr( lpStr ), m_rNsp( rNsp )
	   { 
	     m_pndRoot = NULL;
	   }
   ~TExpression();

   void Val( TVariantVar& rV )
	{
      m_pndRoot->CalcVal();
	  rV = m_pndRoot->Val();
	}

   void Parse( bool bFlAddBool = false );
   int Lexer( YYSTYPE* yys, YYLTYPE* yyl );
   void InitParse()
	{
      iEndClm = iStaClm = 0;
      ClearList(); m_pndRoot = NULL; m_strError = "";
      lpToken = (LPSTR)(LPCSTR)m_strExpr;
	}
   void Test( LPSTR );   

   int Simplify_Constant( int iTyp, YYSTYPE *yysTarg_, YYSTYPE *yysTarg1, 
	                      YYLTYPE *yylTarg1 ); 

   bool FindVar( TVariantVar* );
   list<TNode*>& GetRefList() { return m_lstGarbage; }
   TVariantVar* GetEQVar();
   double GetMulKD( TVariantVar* );

   void ClearUsed();
   
protected:
   CString m_strError;
   int iStaClm, iEndClm;
   LPSTR lpToken;        
	
   
   void MyError( LPSTR );
   

   int Simplify_ArgumentList( int iTyp, YYSTYPE *yysTarg_, 
	 YYSTYPE *yysTarg1, YYSTYPE *yysTarg2, YYSTYPE *yysTarg3,	 
	 YYLTYPE *yylTarg1, YYLTYPE *yylTarg2, YYLTYPE *yylTarg3 );

   int Simplify_PrimaryExpression( int iTyp, YYSTYPE *yysTarg_, 
	 YYSTYPE *yysTarg1, YYSTYPE *yysTarg2, YYSTYPE *yysTarg3,	 
	 YYLTYPE *yylTarg1, YYLTYPE *yylTarg2, YYLTYPE *yylTarg3 );

   int Simplify_PostfixExpression( int iTyp, YYSTYPE *yysTarg_, 
	 YYSTYPE *yysTarg1, YYSTYPE *yysTarg2, YYSTYPE *yysTarg3, YYSTYPE *yysTarg4,	 
	 YYLTYPE *yylTarg1, YYLTYPE *yylTarg2, YYLTYPE *yylTarg3, YYLTYPE *yylTarg4 );

   int Simplify_UnaryOperator( int iTyp, YYSTYPE *yysTarg_, 
	                           YYSTYPE *yysTarg1, 
							   YYLTYPE *yylTarg1 );

   int Simplify_UnaryExpression( int iTyp, YYSTYPE *yysTarg_, 
	 YYSTYPE *yysTarg1, YYSTYPE *yysTarg2,	 
	 YYLTYPE *yylTarg1, YYLTYPE *yylTarg2 );

   int Simplify_MultiplicativeExpr( int iTyp, YYSTYPE *yysTarg_, 
	 YYSTYPE *yysTarg1, YYSTYPE *yysTarg2, YYSTYPE *yysTarg3,	 
	 YYLTYPE *yylTarg1, YYLTYPE *yylTarg2, YYLTYPE *yylTarg3 );

   int Simplify_AdditiveExpr( int iTyp, YYSTYPE *yysTarg_, 
	 YYSTYPE *yysTarg1, YYSTYPE *yysTarg2, YYSTYPE *yysTarg3,	 
	 YYLTYPE *yylTarg1, YYLTYPE *yylTarg2, YYLTYPE *yylTarg3 );

   int Simplify_RelationalExpr( int iTyp, YYSTYPE *yysTarg_, 
	 YYSTYPE *yysTarg1, YYSTYPE *yysTarg2, YYSTYPE *yysTarg3,	 
	 YYLTYPE *yylTarg1, YYLTYPE *yylTarg2, YYLTYPE *yylTarg3 );

   int Simplify_EqualityExpr( int iTyp, YYSTYPE *yysTarg_, 
	 YYSTYPE *yysTarg1, YYSTYPE *yysTarg2, YYSTYPE *yysTarg3,	 
	 YYLTYPE *yylTarg1, YYLTYPE *yylTarg2, YYLTYPE *yylTarg3 );

   int Simplify_BooleanANDExpr( int iTyp, YYSTYPE *yysTarg_, 
	 YYSTYPE *yysTarg1, YYSTYPE *yysTarg2, YYSTYPE *yysTarg3,	 
	 YYLTYPE *yylTarg1, YYLTYPE *yylTarg2, YYLTYPE *yylTarg3 );

   int Simplify_BooleanORExpr( int iTyp, YYSTYPE *yysTarg_, 
	 YYSTYPE *yysTarg1, YYSTYPE *yysTarg2, YYSTYPE *yysTarg3,	 
	 YYLTYPE *yylTarg1, YYLTYPE *yylTarg2, YYLTYPE *yylTarg3 );

   int Simplify_Expression( int iTyp, YYSTYPE *yysTarg_, 
	 YYSTYPE *yysTarg1, YYSTYPE *yysTarg2, YYSTYPE *yysTarg3,	 
	 YYLTYPE *yylTarg1, YYLTYPE *yylTarg2, YYLTYPE *yylTarg3 );

protected:
  CString m_strExpr;  
  TNameSpace& m_rNsp;

  TNode *m_pndRoot;
  list<TNode*> m_lstGarbage;

  void ClearList();
  void ThrowIllegalSimplify( LPCSTR );
 };


#endif
