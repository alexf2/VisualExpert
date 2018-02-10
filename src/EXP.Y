%pure_parser

%{
#define YYLSP_NEEDED
#define YYDEBUG             0
// Разpешить pасшиpеннyю инфоpмацию об ошибке
#if YYDEBUG != 0
#   define YYERROR_VERBOSE
#endif

// Слyжебные макpосы для yстановления соответствий междy именами
//#define yyparse             TCXScriptCompiler:: Parse
#define yylex               ((TExpression*)pExpress)->Lexer
#define yyerror             ((TExpression*)pExpress)->MyError

// Опpеделение типа YYLTYPE
/*#ifdef YYLTYPE
#   undef YYLTYPE
#endif
#   define YYLTYPE          TSYYLType
//#   define first_line       iFirstLine
//#   define first_column     iFirstColumn
//#   define last_line        iLastLine
//#   define last_column      iLastColumn
//#   define text             pszText
typedef struct
{
//    int         m_iFirstLine;
    int          m_iFirstColumn;
//    int          m_iLastLine;
    int          m_iLastColumn;
    char        *m_pszText;
}
TSYYLType;*/
%}

%{
#include "var.hpp"
#include "exp_tab_.hpp"

#define YYPARSE_PARAM pExpress
//#define YYLEX_PARAM   pExpress
%}

/*%union
{
    int         m_iTokenType;
    TokenInfo   m_TokenInfo;
}*/


%token  CHARACTER_CONSTANT  FLOAT_CONSTANT  INTEGER_CONSTANT
        BOOL_CONSTANT       STRING_CONSTANT

%token  IDENTIFIER

%token  OROR    ANDAND  EQU     NEQ     LEQ     GEQ

%right  '='

%left   OROR
%left   ANDAND
%left   EQU
%left   NEQ
%left   LEQ     GEQ
%left   '+'     '-'
%left   '*'     '/'     '%'

%right  '!'

%left   '('

/*--------------------------- start symbol ------------------------------*/

%start Expression

%%

Expression
  : Boolean_OR_Expression
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_Expression( TS_Boolean_OR_Expression,
	                &$$, &$1, NULL, NULL, &@1, NULL, NULL );
	  if( iRetSMC ) YYABORT;
	}
  | Unary_Expression '=' Expression
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_Expression( TS_UnaryExpression_EQ_Expression,
	                &$$, &$1, &$2, &$3, &@1, &@2, &@3 );
	  if( iRetSMC ) YYABORT;
	}
  ;

Boolean_OR_Expression
  : Boolean_AND_Expression
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_BooleanORExpr( TS_Boolean_AND_Expression,
	                &$$, &$1, NULL, NULL, &@1, NULL, NULL );
	  if( iRetSMC ) YYABORT;
	}
  | Boolean_OR_Expression OROR Boolean_AND_Expression
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_BooleanORExpr( TS_BOE_OROR_BAE,
	                &$$, &$1, &$2, &$3, &@1, &@2, &@3 );
	  if( iRetSMC ) YYABORT;
	}
  ;

Boolean_AND_Expression
  : Equality_Expression
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_BooleanANDExpr( TS_Equality_Expression,
	                &$$, &$1, NULL, NULL, &@1, NULL, NULL );
	  if( iRetSMC ) YYABORT;
	}
  | Boolean_AND_Expression ANDAND Equality_Expression
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_BooleanANDExpr( TS_BAE_ANDAND_EE,
	                &$$, &$1, &$2, &$3, &@1, &@2, &@3 );
	  if( iRetSMC ) YYABORT;
	}
  ;

Equality_Expression
  : Relational_Expression
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_EqualityExpr( TS_Relational_Expression,
	                &$$, &$1, NULL, NULL, &@1, NULL, NULL );
	  if( iRetSMC ) YYABORT;
	}
  | Equality_Expression EQU Relational_Expression
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_EqualityExpr( TS_EE_EQ_RE,
	                &$$, &$1, &$2, &$3, &@1, &@2, &@3 );
	  if( iRetSMC ) YYABORT;
	}
  | Equality_Expression NEQ Relational_Expression
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_EqualityExpr( TS_EE_NEQ_RE,
	                &$$, &$1, &$2, &$3, &@1, &@2, &@3 );
	  if( iRetSMC ) YYABORT;
	}
  ;

Relational_Expression
  : Additive_Expression
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_RelationalExpr( TS_Additive_Expression,
	                &$$, &$1, NULL, NULL, &@1, NULL, NULL );
	  if( iRetSMC ) YYABORT;
	}
  | Relational_Expression '<' Additive_Expression
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_RelationalExpr( TS_RE_LE_AE,
	                &$$, &$1, &$2, &$3, &@1, &@2, &@3 );
	  if( iRetSMC ) YYABORT;
	}
  | Relational_Expression '>' Additive_Expression
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_RelationalExpr( TS_RE_GE_AE,
	                &$$, &$1, &$2, &$3, &@1, &@2, &@3 );
	  if( iRetSMC ) YYABORT;
	}
  | Relational_Expression LEQ Additive_Expression
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_RelationalExpr( TS_RE_LEQ_AE,
	                &$$, &$1, &$2, &$3, &@1, &@2, &@3 );
	  if( iRetSMC ) YYABORT;
	}
  | Relational_Expression GEQ Additive_Expression
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_RelationalExpr( TS_RE_GEQ_AE,
	                &$$, &$1, &$2, &$3, &@1, &@2, &@3 );
	  if( iRetSMC ) YYABORT;
	}
  ;

Additive_Expression
  : Multiplicative_Expression
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_AdditiveExpr( TS_Multiplicative_Expression,
	                &$$, &$1, NULL, NULL, &@1, NULL, NULL );
	  if( iRetSMC ) YYABORT;
	}
  | Additive_Expression '+' Multiplicative_Expression
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_AdditiveExpr( TS_AE_PLUS_ME,
	                &$$, &$1, &$2, &$3, &@1, &@2, &@3 );
	  if( iRetSMC ) YYABORT;
	}
  | Additive_Expression '-' Multiplicative_Expression
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_AdditiveExpr( TS_AE_MINUS_ME,
	                &$$, &$1, &$2, &$3, &@1, &@2, &@3 );
	  if( iRetSMC ) YYABORT;
	}
  ;

Multiplicative_Expression
  : Unary_Expression
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_MultiplicativeExpr( TS_UnaryExpression,
	                &$$, &$1, NULL, NULL, &@1, NULL, NULL );
	  if( iRetSMC ) YYABORT;
	}
  | Multiplicative_Expression '*' Unary_Expression
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_MultiplicativeExpr( TS_ME_MUL_UnaryExpression,
	                &$$, &$1, &$2, &$3, &@1, &@2, &@3 );
	  if( iRetSMC ) YYABORT;
	}
  | Multiplicative_Expression '/' Unary_Expression
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_MultiplicativeExpr( TS_ME_DIV_UnaryExpression,
	                &$$, &$1, &$2, &$3, &@1, &@2, &@3 );
	  if( iRetSMC ) YYABORT;
	}
  | Multiplicative_Expression '%' Unary_Expression
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_MultiplicativeExpr( TS_ME_MOD_UnaryExpression,
	                &$$, &$1, &$2, &$3, &@1, &@2, &@3 );
	  if( iRetSMC ) YYABORT;
	}
  ;

Unary_Expression
  : Postfix_Expression
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_UnaryExpression( TS_PostfixExpression,
	                &$$, &$1, NULL, &@1, NULL );
	  if( iRetSMC ) YYABORT;
	}
  | Unary_operator Unary_Expression
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_UnaryExpression( TS_UnaryOperator_UnaryExpression,
	                &$$, &$1, &$2, &@1, &@2 );
	  if( iRetSMC ) YYABORT;
	}
  ;

Unary_operator
  : '-'
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_UnaryOperator( TS_UNMINUS,
	                &$$, &$1, &@1 );
	  if( iRetSMC ) YYABORT;
	}
  | '!'
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_UnaryOperator( TS_NE,
	                &$$, &$1, &@1 );
	  if( iRetSMC ) YYABORT;
	}
  ;


Postfix_Expression
  : Primary_Expression
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_PostfixExpression( TS_PrimaryExpression,
	                &$$, &$1, NULL, NULL, NULL, &@1, NULL, NULL, NULL );
	  if( iRetSMC ) YYABORT;
	}
  | Postfix_Expression '(' Argument_List ')'  
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_PostfixExpression( TS_PostfixExpress_BR_ArgList_BR,
	                &$$, &$1, &$2, &$3, &$4, &@1, &@2, &@3, &@4 );
	  if( iRetSMC ) YYABORT;
	}
  ;

Primary_Expression
  : IDENTIFIER
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_PrimaryExpression( TS_Identifier,
	                &$$, &$1, NULL, NULL, &@1, NULL, NULL );
	  if( iRetSMC ) YYABORT;
	}  
  | Constant
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_PrimaryExpression( TS_Constant,
	                &$$, &$1, NULL, NULL, &@1, NULL, NULL );
	  if( iRetSMC ) YYABORT;
	}  
  | '(' Expression ')'
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_PrimaryExpression( TS_BR_Expression_BR,
	                &$$, &$1, &$2, &$3, &@1, &@2, &@3 );
	  if( iRetSMC ) YYABORT;
	}  
  ;

Argument_List
  : Expression
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_ArgumentList( TS_Expression,
	                &$$, &$1, NULL, NULL, &@1, NULL, NULL );
	  if( iRetSMC ) YYABORT;
	}  
  | Argument_List ',' Expression
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_ArgumentList( TS_ArgumentList_ZP_Expression,
	                &$$, &$1, &$2, &$3, &@1, &@2, &@3 );
	  if( iRetSMC ) YYABORT;
	}  
  ;

Constant
  : INTEGER_CONSTANT
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_Constant( TS_INTEGER_CONSTANT,
	                 &$$, &$1, &@1 );
	  if( iRetSMC ) YYABORT;	   
	}
  | CHARACTER_CONSTANT
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_Constant( TS_CHARACTER_CONSTANT,
	                 &$$, &$1, &@1 );
	  if( iRetSMC ) YYABORT;	   
	}
  | FLOAT_CONSTANT
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_Constant( TS_FLOAT_CONSTANT,
	                 &$$, &$1, &@1 );
	  if( iRetSMC ) YYABORT;	   
	}
  | BOOL_CONSTANT
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_Constant( TS_BOOL_CONSTANT,
	                 &$$, &$1, &@1 );
	  if( iRetSMC ) YYABORT;	   
	}
  | STRING_CONSTANT
    {
      int iRetSMC = ((TExpression*)pExpress)->Simplify_Constant( TS_STRING_CONSTANT,
	                 &$$, &$1, &@1 );
	  if( iRetSMC ) YYABORT;	   
	}
  ;
%%