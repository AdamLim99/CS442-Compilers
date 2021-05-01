%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IOMngr.h"
#include "SymTab.h"
#include "Semantics.h"
#include "CodeGen.h"

extern int yylex();	/* The next token function. */
extern char *yytext;   /* The matched token text.  */
extern int yyleng;      /* The token text length.   */
extern int yyparse();
extern int yyerror(char *);
void dumpTable();

extern SymTab *table;

%}


%union {
  long val;
  char * string;
  struct ExprRes * ExprRes;
  struct InstrSeq * InstrSeq;
  struct Node * node;
  struct IdList * idL;
}

%type <string> Id
%type <ExprRes> Factor
%type <ExprRes> Expo
%type <ExprRes> Term
%type <ExprRes> Expr
%type <InstrSeq> StmtSeq
%type <InstrSeq> Stmt
%type <ExprRes> BExpr
%type <idL> IDList
%type <node> EList

%token Ident 		
%token IntLit 	
%token Int
%token Bool
%token Write
%token IF
%token ELSE
%token WHILE
%token FOR
%token EQ	
%token BitAnd
%token BitOr
%token setLE
%token setGE
%token notEQ
%token printL
%token printS
%token Read
%token Print
%token True
%token False
%token printB
%token Void
%token Ret

%%

Prog		:	Declarations StmtSeq													{Finish($2); } ;
Declarations	:	Dec Declarations													{ };
Declarations	:																{ };
Dec		:	Bool Id '(' ')' '{' StmtSeq '}'												{enterName(table, $2); setCurrentAttr(table, storeI("Func", NULL, $6));};
Dec		:	Int Id '(' ')' '{' StmtSeq '}'												{enterName(table, $2); setCurrentAttr(table, storeI("Func", NULL, $6));};
Dec		:	Void Id '(' ')' '{' StmtSeq '}'												{enterName(table, $2); setCurrentAttr(table, storeI("Func", NULL, $6));};
Dec		:	Bool Id {enterName(table, $2);} '[' IntLit {setCurrentAttr(table, storeI("Arr", strdup(yytext), NULL));} ']' ';'	{};
Dec		:	Int Id {enterName(table, $2);} '[' IntLit {setCurrentAttr(table, storeI("Arr", strdup(yytext), NULL));} ']' ';'		{};
Dec		:	Bool Id ';'														{enterName(table, $2);setCurrentAttr(table, storeI("Bool", NULL, NULL));};
Dec		:	Int Id ';'														{enterName(table, $2);};
StmtSeq 	:	Stmt StmtSeq														{$$ = AppendSeq($1, $2); } ;
StmtSeq		:																{$$ = NULL;} ;
EList		:	Expr															{$$ = add($1, NULL);};
EList		:	Expr ',' EList														{$$ = add($1, $3);};
IDList		:	Id															{$$ = addId($1, NULL);};
IDList		:	Id ',' IDList														{$$ = addId($1, $3);};
Stmt		:	Ret BExpr ';'														{$$ = doRet($2);};
Stmt		:	Id '(' ')' ';'														{$$ = doFunc($1);};
Stmt		:	FOR '(' Stmt BExpr ';' Stmt ')' '{' StmtSeq '}'										{$$ = doFor($3, $4, $6, $9);};
Stmt		:	printB BExpr ';'													{$$ = doPrintB($2);};
Stmt		:	Print EList ')' ';'													{$$ = doPList($2);};
Stmt		:	Read '(' Id '[' Expr ']' ')' ';'											{$$ = doReadArr($3, $5);};
Stmt		:	Read '(' IDList ')' ';'													{$$ = doRead($3);};
Stmt		:	Write Expr ';'														{$$ = doPrint($2); };
Stmt		:	Id '=' BExpr ';'													{$$ = doAssign($1, $3);};
Stmt		:	Id '[' Expr ']'	'=' False ';'												{$$ = arrAssignB($1, $3, 0);}
Stmt		:	Id '[' Expr ']'	'=' True ';'												{$$ = arrAssignB($1, $3, 1);}
Stmt		:	Id '[' Expr ']'	'=' BExpr ';'												{$$ = arrAssign($1, $3, $6);}
Stmt		:	Id '=' False ';'													{$$ = boolAssign($1, 0);};
Stmt		:	Id '=' True ';'														{$$ = boolAssign($1, 1);};
Stmt		:	WHILE '(' BExpr ')' '{' StmtSeq '}'											{$$ = doWhile($3, $6);};
Stmt		:	IF '(' BExpr ')' '{' StmtSeq '}'											{$$ = doIf($3, $6);};
Stmt		:	IF '(' BExpr ')' '{' StmtSeq '}' ELSE '{' StmtSeq '}'									{$$ = doIfElse($3, $6, $10);};
Stmt		:	printS '(' Expr ')' ';'													{$$ = doPrintS($3);};
Stmt		:	printL '(' Expr ')' ';'													{$$ = doPrintL($3);};
BExpr		:	BExpr BitOr Expr													{$$ = doOr($1, $3);};
BExpr		:	BExpr BitAnd Expr													{$$ = doAnd($1, $3);};
BExpr		:	BExpr notEQ Expr													{$$ = doEq($1, $3);};
BExpr		:	BExpr EQ Expr														{$$ = doEq($1, $3);};
BExpr		:	BExpr setGE Expr													{$$ = doSGE($1, $3);};
BExpr		:	BExpr setLE Expr													{$$ = doSLE($1, $3);};
BExpr		:	BExpr '>' Expr														{$$ = doLarger($1, $3);};
BExpr		:	BExpr '<' Expr														{$$ = doSmaller($1, $3);};
BExpr		:	Expr															{$$ = $1;};
Expr		:	Expr '-' Term														{$$ = doMinus($1, $3); } ;
Expr		:	Expr '+' Term														{$$ = doAdd($1, $3); } ;
Expr		:	Term															{$$ = $1; } ;
Term		:	Term '%' Expo														{$$ = doRem($1, $3);};
Term		:	Term '/' Expo														{$$ = doDiv($1, $3); } ;
Term		:	Term '*' Expo														{$$ = doMult($1, $3); } ;
Term		:	Expo															{$$ = $1;};
Expo		:	Expo '^' Factor														{$$ = doExp($1, $3);};
Expo		:	Factor															{$$ = $1;};
Factor		:	Id '(' ')'														{$$ = initFunc($1);};
Factor		:	Id '[' Expr ']'														{$$ = getArr($1, $3);};
Factor		:	'!' Factor														{$$ = doNot($2);};
Factor  	: 	'(' BExpr ')'      													{$$ = $2;};
Factor		:	'-'Factor														{$$ = doUnary($2);};
Factor		:	IntLit															{$$ = doIntLit(yytext);};
Factor		:	Id															{$$ = doRval($1);};
Id		: 	Ident															{$$ = strdup(yytext);}

 
%%

int yyerror(char *s)  {
  writeIndicator(getCurrentColumnNum());
  writeMessage("Illegal Character in YACC");
  return 1;
}
