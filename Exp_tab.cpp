
/*  A Bison parser, made from exp.y with Bison version GNU Bison version 1.24
  */

#define YYBISON 1  /* Identify Bison output.  */

#define YYLSP_NEEDED
/*
#define	CHARACTER_CONSTANT	258
#define	FLOAT_CONSTANT	259
#define	INTEGER_CONSTANT	260
#define	BOOL_CONSTANT	261
#define	STRING_CONSTANT	262
#define	IDENTIFIER	263
#define	OROR	264
#define	ANDAND	265
#define	EQU	266
#define	NEQ	267
#define	LEQ	268
#define	GEQ	269
*/
#line 3 "exp.y"

#define YYLSP_NEEDED
#define YYDEBUG             0
// ���p���� p���p���y� ���p���� �� �訡��
#if YYDEBUG != 0
#   define YYERROR_VERBOSE
#endif

// ��y����� ���p��� ��� y�⠭������� ᮮ⢥��⢨� ����y �������
//#define yyparse             TCXScriptCompiler:: Parse
#define yylex               ((TExpression*)pExpress)->Lexer
#define yyerror             ((TExpression*)pExpress)->MyError

// ��p�������� ⨯� YYLTYPE
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
#line 37 "exp.y"

#include "var.hpp"
#include "exp_tab_.hpp"

#define YYPARSE_PARAM pExpress
//#define YYLEX_PARAM   pExpress

#ifndef YYLTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YYLTYPE yyltype
#endif

#ifndef YYSTYPE
#define YYSTYPE int
#endif
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		61
#define	YYFLAG		-32768
#define	YYNTBASE	27

#define YYTRANSLATE(x) ((unsigned)(x) <= 269 ? yytranslate[x] : 40)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    21,     2,     2,     2,    20,     2,     2,    22,
    25,    18,    16,    26,    17,     2,    19,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,    23,
    15,    24,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     6,     8,    12,    14,    18,    20,    24,    28,
    30,    34,    38,    42,    46,    48,    52,    56,    58,    62,
    66,    70,    72,    75,    77,    79,    81,    86,    88,    90,
    94,    96,   100,   102,   104,   106,   108
};

static const short yyrhs[] = {    28,
     0,    34,    15,    27,     0,    29,     0,    28,     9,    29,
     0,    30,     0,    29,    10,    30,     0,    31,     0,    30,
    11,    31,     0,    30,    12,    31,     0,    32,     0,    31,
    23,    32,     0,    31,    24,    32,     0,    31,    13,    32,
     0,    31,    14,    32,     0,    33,     0,    32,    16,    33,
     0,    32,    17,    33,     0,    34,     0,    33,    18,    34,
     0,    33,    19,    34,     0,    33,    20,    34,     0,    36,
     0,    35,    34,     0,    17,     0,    21,     0,    37,     0,
    36,    22,    38,    25,     0,     8,     0,    39,     0,    22,
    27,    25,     0,    27,     0,    38,    26,    27,     0,     5,
     0,     3,     0,     4,     0,     6,     0,     7,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    80,    86,    95,   101,   110,   116,   125,   131,   137,   146,
   152,   158,   164,   170,   179,   185,   191,   200,   206,   212,
   218,   227,   233,   242,   248,   258,   264,   273,   279,   285,
   294,   300,   309,   315,   321,   327,   333
};

static const char * const yytname[] = {   "$","error","$undefined.","CHARACTER_CONSTANT",
"FLOAT_CONSTANT","INTEGER_CONSTANT","BOOL_CONSTANT","STRING_CONSTANT","IDENTIFIER",
"OROR","ANDAND","EQU","NEQ","LEQ","GEQ","'='","'+'","'-'","'*'","'/'","'%'",
"'!'","'('","'<'","'>'","')'","','","Expression","Boolean_OR_Expression","Boolean_AND_Expression",
"Equality_Expression","Relational_Expression","Additive_Expression","Multiplicative_Expression",
"Unary_Expression","Unary_operator","Postfix_Expression","Primary_Expression",
"Argument_List","Constant",""
};
#endif

static const short yyr1[] = {     0,
    27,    27,    28,    28,    29,    29,    30,    30,    30,    31,
    31,    31,    31,    31,    32,    32,    32,    33,    33,    33,
    33,    34,    34,    35,    35,    36,    36,    37,    37,    37,
    38,    38,    39,    39,    39,    39,    39
};

static const short yyr2[] = {     0,
     1,     3,     1,     3,     1,     3,     1,     3,     3,     1,
     3,     3,     3,     3,     1,     3,     3,     1,     3,     3,
     3,     1,     2,     1,     1,     1,     4,     1,     1,     3,
     1,     3,     1,     1,     1,     1,     1
};

static const short yydefact[] = {     0,
    34,    35,    33,    36,    37,    28,    24,    25,     0,     1,
     3,     5,     7,    10,    15,    18,     0,    22,    26,    29,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    23,     0,    30,     4,    18,
     6,     8,     9,    13,    14,    11,    12,    16,    17,    19,
    20,    21,     2,    31,     0,    27,     0,    32,     0,     0,
     0
};

static const short yydefgoto[] = {    21,
    10,    11,    12,    13,    14,    15,    40,    17,    18,    19,
    55,    20
};

static const short yypact[] = {    -1,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    -1,    18,
    -2,     3,     5,     9,     4,    28,    -1,    22,-32768,-32768,
    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,-32768,    -1,-32768,    -2,-32768,
     3,     5,     5,     9,     9,     9,     9,     4,     4,-32768,
-32768,-32768,-32768,-32768,    14,-32768,    -1,-32768,    46,    47,
-32768
};

static const short yypgoto[] = {     1,
-32768,    26,    27,     6,   -16,    11,     0,-32768,-32768,-32768,
-32768,-32768
};


#define	YYLAST		58


static const short yytable[] = {    16,
    59,     1,     2,     3,     4,     5,     6,    23,    16,    44,
    45,    46,    47,    24,    25,     7,    36,    26,    27,     8,
     9,    32,    33,    34,    30,    31,    22,    28,    29,    42,
    43,    50,    51,    52,    16,    53,    16,    54,    56,    57,
    48,    49,    35,    37,    38,    60,    61,    39,     0,    41,
     0,     0,     0,     0,     0,     0,    16,    58
};

static const short yycheck[] = {     0,
     0,     3,     4,     5,     6,     7,     8,    10,     9,    26,
    27,    28,    29,    11,    12,    17,    17,    13,    14,    21,
    22,    18,    19,    20,    16,    17,     9,    23,    24,    24,
    25,    32,    33,    34,    35,    35,    37,    37,    25,    26,
    30,    31,    15,    22,    25,     0,     0,    22,    -1,    23,
    -1,    -1,    -1,    -1,    -1,    -1,    57,    57
};
#define YYPURE 1

/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "bison.simple"

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#define alloca _alloca
#include <malloc.h>

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void* _alloca(unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         -2
#define YYEOF           0
#define YYACCEPT        return(0)
#define YYABORT         return(1)
#define YYERROR         goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL          goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do                                       \
  if (yychar == YYEMPTY && yylen == 1)   \
    { yychar = (token), yylval = (value); \
      yychar1 = YYTRANSLATE (yychar);     \
      YYPOPSTACK;                         \
      goto yybackup;                      \
    }                                     \
  else                                    \
    { yyerror ("�������������� ������: ������ ��������� back up"); YYERROR; }\
while( 0 );
	       //"syntax error: cannot back up" !!!!!!!!!!!!!

#define YYTERROR        1
#define YYERRCODE       256

#ifndef YYPURE
#define YYLEX           yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX           yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX           yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX           yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX           yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int     yychar;                 /*  the lookahead symbol                */
YYSTYPE yylval;                 /*  the semantic value of the           */
                                /*  lookahead symbol                    */

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;                 /*  location data for the lookahead     */
                                /*  symbol                              */
#endif

int yynerrs;                    /*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;                    /*  nonzero means print parse trace     */
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks       */

#ifndef YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1                /* GNU C and GNU C++ define this.  */
#define __yy_memcpy(FROM,TO,COUNT)      __builtin_memcpy(TO,FROM,COUNT)
#else                           /* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (from, to, count)
     char *from;
     char *to;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
/*static void
__yy_memcpy (char *from, char *to, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}*/
//!!!!!!!!!!!!
#define __yy_memcpy(FROM_, TO_, CNT_) memcpy(TO_,FROM_,CNT_)

#endif
#endif

#line 192 "bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM
#else
#define YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#endif

//!!!!!!!!!!!!!
//int
//yyparse(YYPARSE_PARAM)
//     YYPARSE_PARAM_DECL
//!!!!!!!!!!
int yyparse( YYPARSE_PARAM_DECL )
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;      /*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;              /*  lookahead token as an internal (translated) token number */

  short yyssa[YYINITDEPTH];     /*  the state stack                     */
  YYSTYPE yyvsa[YYINITDEPTH];   /*  the semantic value stack            */
  memset( yyvsa, 0, sizeof(YYSTYPE)*YYINITDEPTH );//!!!!!!!!!!!!

  short *yyss = yyssa;          /*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;        /*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];   /*  the location stack                  */
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
  memset( yylsa, 0, sizeof(YYLTYPE)*YYINITDEPTH );//!!!!!!!!!!!!!!

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;                /*  the variable used to return         */
                                /*  semantic values from the action     */
                                /*  routines                            */

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "������ �������\n"); //"Starting parse\n"!!!!!!!
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;             /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
         the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
         but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("������������ ����� �������", //"parser stack overflow"!!!!!!
                 &yyss1, size * sizeof (*yyssp),
                 &yyvs1, size * sizeof (*yyvsp),
                 &yyls1, size * sizeof (*yylsp),
                 &yystacksize);
#else
      yyoverflow("������������ ����� �������", //"parser stack overflow"!!!!!!
                 &yyss1, size * sizeof (*yyssp),
                 &yyvs1, size * sizeof (*yyvsp),
                 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
        {
          yyerror("������������ ����� �������"); //"parser stack overflow"!!!!!!
          return 2;
        }
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
        yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
        fprintf(stderr, "����� ����� ��������� �� %d\n", yystacksize);
		//Stack size increased to !!!!!!!
#endif

      if (yyssp >= yyss + yystacksize - 1)
        YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
        fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)              /* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;           /* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
        fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
        {
          fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
          /* Give the individual parser a way to print the precise meaning
             of a token, for further debugging info.  */
#ifdef YYPRINT
          YYPRINT (stderr, yychar, yylval);
#endif
          fprintf (stderr, ")\n");
        }
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
               yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
        fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 81 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_Expression( TS_Boolean_OR_Expression,
	                &yyval, &yyvsp[0], NULL, NULL, &yylsp[0], NULL, NULL );
	  if( iRetSMC ) YYABORT;
	;
    break;}
case 2:
#line 87 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_Expression( TS_UnaryExpression_EQ_Expression,
	                &yyval, &yyvsp[-2], &yyvsp[-1], &yyvsp[0], &yylsp[-2], &yylsp[-1], &yylsp[0] );
	  if( iRetSMC ) YYABORT;
	;
    break;}
case 3:
#line 96 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_BooleanORExpr( TS_Boolean_AND_Expression,
	                &yyval, &yyvsp[0], NULL, NULL, &yylsp[0], NULL, NULL );
	  if( iRetSMC ) YYABORT;
	;
    break;}
case 4:
#line 102 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_BooleanORExpr( TS_BOE_OROR_BAE,
	                &yyval, &yyvsp[-2], &yyvsp[-1], &yyvsp[0], &yylsp[-2], &yylsp[-1], &yylsp[0] );
	  if( iRetSMC ) YYABORT;
	;
    break;}
case 5:
#line 111 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_BooleanANDExpr( TS_Equality_Expression,
	                &yyval, &yyvsp[0], NULL, NULL, &yylsp[0], NULL, NULL );
	  if( iRetSMC ) YYABORT;
	;
    break;}
case 6:
#line 117 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_BooleanANDExpr( TS_BAE_ANDAND_EE,
	                &yyval, &yyvsp[-2], &yyvsp[-1], &yyvsp[0], &yylsp[-2], &yylsp[-1], &yylsp[0] );
	  if( iRetSMC ) YYABORT;
	;
    break;}
case 7:
#line 126 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_EqualityExpr( TS_Relational_Expression,
	                &yyval, &yyvsp[0], NULL, NULL, &yylsp[0], NULL, NULL );
	  if( iRetSMC ) YYABORT;
	;
    break;}
case 8:
#line 132 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_EqualityExpr( TS_EE_EQ_RE,
	                &yyval, &yyvsp[-2], &yyvsp[-1], &yyvsp[0], &yylsp[-2], &yylsp[-1], &yylsp[0] );
	  if( iRetSMC ) YYABORT;
	;
    break;}
case 9:
#line 138 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_EqualityExpr( TS_EE_NEQ_RE,
	                &yyval, &yyvsp[-2], &yyvsp[-1], &yyvsp[0], &yylsp[-2], &yylsp[-1], &yylsp[0] );
	  if( iRetSMC ) YYABORT;
	;
    break;}
case 10:
#line 147 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_RelationalExpr( TS_Additive_Expression,
	                &yyval, &yyvsp[0], NULL, NULL, &yylsp[0], NULL, NULL );
	  if( iRetSMC ) YYABORT;
	;
    break;}
case 11:
#line 153 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_RelationalExpr( TS_RE_LE_AE,
	                &yyval, &yyvsp[-2], &yyvsp[-1], &yyvsp[0], &yylsp[-2], &yylsp[-1], &yylsp[0] );
	  if( iRetSMC ) YYABORT;
	;
    break;}
case 12:
#line 159 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_RelationalExpr( TS_RE_GE_AE,
	                &yyval, &yyvsp[-2], &yyvsp[-1], &yyvsp[0], &yylsp[-2], &yylsp[-1], &yylsp[0] );
	  if( iRetSMC ) YYABORT;
	;
    break;}
case 13:
#line 165 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_RelationalExpr( TS_RE_LEQ_AE,
	                &yyval, &yyvsp[-2], &yyvsp[-1], &yyvsp[0], &yylsp[-2], &yylsp[-1], &yylsp[0] );
	  if( iRetSMC ) YYABORT;
	;
    break;}
case 14:
#line 171 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_RelationalExpr( TS_RE_GEQ_AE,
	                &yyval, &yyvsp[-2], &yyvsp[-1], &yyvsp[0], &yylsp[-2], &yylsp[-1], &yylsp[0] );
	  if( iRetSMC ) YYABORT;
	;
    break;}
case 15:
#line 180 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_AdditiveExpr( TS_Multiplicative_Expression,
	                &yyval, &yyvsp[0], NULL, NULL, &yylsp[0], NULL, NULL );
	  if( iRetSMC ) YYABORT;
	;
    break;}
case 16:
#line 186 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_AdditiveExpr( TS_AE_PLUS_ME,
	                &yyval, &yyvsp[-2], &yyvsp[-1], &yyvsp[0], &yylsp[-2], &yylsp[-1], &yylsp[0] );
	  if( iRetSMC ) YYABORT;
	;
    break;}
case 17:
#line 192 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_AdditiveExpr( TS_AE_MINUS_ME,
	                &yyval, &yyvsp[-2], &yyvsp[-1], &yyvsp[0], &yylsp[-2], &yylsp[-1], &yylsp[0] );
	  if( iRetSMC ) YYABORT;
	;
    break;}
case 18:
#line 201 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_MultiplicativeExpr( TS_UnaryExpression,
	                &yyval, &yyvsp[0], NULL, NULL, &yylsp[0], NULL, NULL );
	  if( iRetSMC ) YYABORT;
	;
    break;}
case 19:
#line 207 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_MultiplicativeExpr( TS_ME_MUL_UnaryExpression,
	                &yyval, &yyvsp[-2], &yyvsp[-1], &yyvsp[0], &yylsp[-2], &yylsp[-1], &yylsp[0] );
	  if( iRetSMC ) YYABORT;
	;
    break;}
case 20:
#line 213 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_MultiplicativeExpr( TS_ME_DIV_UnaryExpression,
	                &yyval, &yyvsp[-2], &yyvsp[-1], &yyvsp[0], &yylsp[-2], &yylsp[-1], &yylsp[0] );
	  if( iRetSMC ) YYABORT;
	;
    break;}
case 21:
#line 219 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_MultiplicativeExpr( TS_ME_MOD_UnaryExpression,
	                &yyval, &yyvsp[-2], &yyvsp[-1], &yyvsp[0], &yylsp[-2], &yylsp[-1], &yylsp[0] );
	  if( iRetSMC ) YYABORT;
	;
    break;}
case 22:
#line 228 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_UnaryExpression( TS_PostfixExpression,
	                &yyval, &yyvsp[0], NULL, &yylsp[0], NULL );
	  if( iRetSMC ) YYABORT;
	;
    break;}
case 23:
#line 234 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_UnaryExpression( TS_UnaryOperator_UnaryExpression,
	                &yyval, &yyvsp[-1], &yyvsp[0], &yylsp[-1], &yylsp[0] );
	  if( iRetSMC ) YYABORT;
	;
    break;}
case 24:
#line 243 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_UnaryOperator( TS_UNMINUS,
	                &yyval, &yyvsp[0], &yylsp[0] );
	  if( iRetSMC ) YYABORT;
	;
    break;}
case 25:
#line 249 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_UnaryOperator( TS_NE,
	                &yyval, &yyvsp[0], &yylsp[0] );
	  if( iRetSMC ) YYABORT;
	;
    break;}
case 26:
#line 259 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_PostfixExpression( TS_PrimaryExpression,
	                &yyval, &yyvsp[0], NULL, NULL, NULL, &yylsp[0], NULL, NULL, NULL );
	  if( iRetSMC ) YYABORT;
	;
    break;}
case 27:
#line 265 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_PostfixExpression( TS_PostfixExpress_BR_ArgList_BR,
	                &yyval, &yyvsp[-3], &yyvsp[-2], &yyvsp[-1], &yyvsp[0], &yylsp[-3], &yylsp[-2], &yylsp[-1], &yylsp[0] );
	  if( iRetSMC ) YYABORT;
	;
    break;}
case 28:
#line 274 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_PrimaryExpression( TS_Identifier,
	                &yyval, &yyvsp[0], NULL, NULL, &yylsp[0], NULL, NULL );
	  if( iRetSMC ) YYABORT;
	;
    break;}
case 29:
#line 280 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_PrimaryExpression( TS_Constant,
	                &yyval, &yyvsp[0], NULL, NULL, &yylsp[0], NULL, NULL );
	  if( iRetSMC ) YYABORT;
	;
    break;}
case 30:
#line 286 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_PrimaryExpression( TS_BR_Expression_BR,
	                &yyval, &yyvsp[-2], &yyvsp[-1], &yyvsp[0], &yylsp[-2], &yylsp[-1], &yylsp[0] );
	  if( iRetSMC ) YYABORT;
	;
    break;}
case 31:
#line 295 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_ArgumentList( TS_Expression,
	                &yyval, &yyvsp[0], NULL, NULL, &yylsp[0], NULL, NULL );
	  if( iRetSMC ) YYABORT;
	;
    break;}
case 32:
#line 301 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_ArgumentList( TS_ArgumentList_ZP_Expression,
	                &yyval, &yyvsp[-2], &yyvsp[-1], &yyvsp[0], &yylsp[-2], &yylsp[-1], &yylsp[0] );
	  if( iRetSMC ) YYABORT;
	;
    break;}
case 33:
#line 310 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_Constant( TS_INTEGER_CONSTANT,
	                 &yyval, &yyvsp[0], &yylsp[0] );
	  if( iRetSMC ) YYABORT;	   
	;
    break;}
case 34:
#line 316 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_Constant( TS_CHARACTER_CONSTANT,
	                 &yyval, &yyvsp[0], &yylsp[0] );
	  if( iRetSMC ) YYABORT;	   
	;
    break;}
case 35:
#line 322 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_Constant( TS_FLOAT_CONSTANT,
	                 &yyval, &yyvsp[0], &yylsp[0] );
	  if( iRetSMC ) YYABORT;	   
	;
    break;}
case 36:
#line 328 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_Constant( TS_BOOL_CONSTANT,
	                 &yyval, &yyvsp[0], &yylsp[0] );
	  if( iRetSMC ) YYABORT;	   
	;
    break;}
case 37:
#line 334 "exp.y"
{
      int iRetSMC = ((TExpression*)pExpress)->Simplify_Constant( TS_STRING_CONSTANT,
	                 &yyval, &yyvsp[0], &yylsp[0] );
	  if( iRetSMC ) YYABORT;	   
	;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 487 "bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
        fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
        {
          int size = 0;
          char *msg;
          int x, count;

          count = 0;
          /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
          for (x = (yyn < 0 ? -yyn : 0);
               x < (sizeof(yytname) / sizeof(char *)); x++)
            if (yycheck[x + yyn] == x)
              size += strlen(yytname[x]) + 15, count++;
          msg = (char *) malloc(size + 15);
          if (msg != 0)
            {
              strcpy(msg, "�������������� ������");//"parse error"!!!!!!!

              if (count < 5)
                {
                  count = 0;
                  for (x = (yyn < 0 ? -yyn : 0);
                       x < (sizeof(yytname) / sizeof(char *)); x++)
                    if (yycheck[x + yyn] == x)
                      {
                        strcat(msg, count == 0 ? ", ��������� `" : " or `");//expecting!!!!!!!!
                        strcat(msg, yytname[x]);
                        strcat(msg, "'");
                        count++;
                      }
                }
              yyerror(msg);
              free(msg);
            }
          else
            yyerror ("������ �������: ��������� ����������� ������");
			//"parse error; also virtual memory exceeded"!!!!!!!!!
        }
      else
#endif /* YYERROR_VERBOSE */
        yyerror("������ �������");//"parse error"!!!!!!!!
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
        YYABORT;

#if YYDEBUG != 0
      if (yydebug)
        fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;              /* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
        fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
        goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
#line 340 "exp.y"
