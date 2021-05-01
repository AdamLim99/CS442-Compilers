/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    Ident = 258,
    IntLit = 259,
    Int = 260,
    Bool = 261,
    Write = 262,
    IF = 263,
    ELSE = 264,
    WHILE = 265,
    FOR = 266,
    EQ = 267,
    BitAnd = 268,
    BitOr = 269,
    setLE = 270,
    setGE = 271,
    notEQ = 272,
    printL = 273,
    printS = 274,
    Read = 275,
    Print = 276,
    True = 277,
    False = 278,
    printB = 279,
    Void = 280,
    Ret = 281
  };
#endif
/* Tokens.  */
#define Ident 258
#define IntLit 259
#define Int 260
#define Bool 261
#define Write 262
#define IF 263
#define ELSE 264
#define WHILE 265
#define FOR 266
#define EQ 267
#define BitAnd 268
#define BitOr 269
#define setLE 270
#define setGE 271
#define notEQ 272
#define printL 273
#define printS 274
#define Read 275
#define Print 276
#define True 277
#define False 278
#define printB 279
#define Void 280
#define Ret 281

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 22 "ExprEval.y" /* yacc.c:1909  */

  long val;
  char * string;
  struct ExprRes * ExprRes;
  struct InstrSeq * InstrSeq;
  struct Node * node;
  struct IdList * idL;

#line 115 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
