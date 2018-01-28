#ifndef YYSTYPE
#define YYSTYPE int
#endif

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

