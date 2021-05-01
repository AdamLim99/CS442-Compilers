/* Semantics.h
   The action and supporting routines for performing semantics processing.
*/

/* Semantic Records */
typedef struct IdList {
  struct SymEntry * TheEntry;
  struct IdList * Next;
} IdList;

struct ExprRes {
  int Reg;
  struct InstrSeq * Instrs;
};

struct ExprResList {
	struct ExprRes *Expr;
	struct ExprResList * Next;
};

struct BExprRes {
  char * Label;
  struct InstrSeq * Instrs;
};

typedef struct Node {
  struct ExprRes * Expr;
  struct Node * next;
} Node;

typedef struct storeInfo {
  char * name;
  char * size;
  struct InstrSeq * seq;
} storeInfo;

/* Semantics Actions */
extern struct storeInfo * storeI (char * name, char * value, struct InstrSeq * seq);
extern struct Node * add (struct ExprRes * Expr, struct Node * h);
extern struct IdList * addId (char * name, struct IdList * list);
extern struct ExprRes *  doIntLit(char * digits);
extern struct ExprRes *  doUnary(struct ExprRes * Res1);
extern struct ExprRes *  doRval(char * name);
extern struct InstrSeq *  doAssign(char * name,  struct ExprRes * Res1);
extern struct ExprRes *  doAdd(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes *  doMinus(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes *  doMult(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes *  doDiv(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes *  doRem(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes *  doExp(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes *  doAnd(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes *  doOr(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes *  doNot(struct ExprRes * Res1);
extern struct ExprRes *  doSmaller(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes *  doLarger(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes *  doSLE(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes *  doSGE(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct InstrSeq *  doPrint(struct ExprRes * Expr);
extern struct InstrSeq *  doPrintL(struct ExprRes * Expr);
extern struct InstrSeq *  doPrintS(struct ExprRes * Expr);
extern struct InstrSeq * doRead(struct IdList * list);
extern struct InstrSeq * doPList(struct Node * list);
extern struct ExprRes * doEq (struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct InstrSeq * doIf(struct ExprRes *Res, struct InstrSeq * seq);
extern struct InstrSeq * doIfElse(struct ExprRes *Res, struct InstrSeq * seq, struct InstrSeq * seq2);
extern struct InstrSeq * doWhile(struct ExprRes *Res, struct InstrSeq * seq);
extern struct InstrSeq * doFor(struct InstrSeq *Res1, struct ExprRes * Res2, struct InstrSeq * Res3, struct InstrSeq * seq);
extern struct ExprRes * getArr(char * name, struct ExprRes *Res);
extern struct InstrSeq * arrAssign(char * name, struct ExprRes *Res1, struct ExprRes *Res2);
extern struct InstrSeq * boolAssign(char * name, int bool);
extern struct InstrSeq *  doPrintB(struct ExprRes * Expr);
extern struct InstrSeq * arrAssignB(char * name, struct ExprRes *Res1, int bool);
extern struct InstrSeq * doFunc(char * name);
extern struct ExprRes * initFunc(char * name);
extern struct InstrSeq * doRet(struct ExprRes * Res);
extern struct InstrSeq * doReadArr(char * name, struct ExprRes * Res);

extern void	Finish(struct InstrSeq *Code);
