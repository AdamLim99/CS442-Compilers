/* Semantics.c
   Support and semantic action routines.
*/

#include <string.h>
#include <stdlib.h>
#include "CodeGen.h"
#include "Semantics.h"
#include "SymTab.h"
#include "IOMngr.h"

extern SymTab *table;

/* Semantics support routines */
extern struct storeInfo * storeI (char * n, char * s, struct InstrSeq * code){
  struct storeInfo * SI = (struct storeInfo *)malloc(sizeof(struct storeInfo));
  SI->name=n;
  SI->size=s;
  SI->seq=code;
  return SI;
}

struct ExprRes *  doIntLit(char * digits)  { 
  struct ExprRes *res;
  res = (struct ExprRes *) malloc(sizeof(struct ExprRes));
  res->Reg = AvailTmpReg();
  res->Instrs = GenInstr(NULL,"li",TmpRegName(res->Reg),digits,NULL);
  return res;
}

struct ExprRes *  doUnary(struct ExprRes * Res1)  { 
  int reg;
  reg = AvailTmpReg();
  AppendSeq(Res1->Instrs, GenInstr(NULL,"sub",TmpRegName(reg),"$zero",TmpRegName(Res1->Reg)));
  ReleaseTmpReg(Res1->Reg);
  Res1->Reg = reg;
  return Res1;
}

struct ExprRes *  doRval(char * name)  { 
  struct ExprRes *res;
  if (!findName(table, name)) {
	writeIndicator(getCurrentColumnNum());
	writeMessage("Undeclared variable");
  }
  res = (struct ExprRes *) malloc(sizeof(struct ExprRes));
  res->Reg = AvailTmpReg();
  res->Instrs = GenInstr(NULL,"lw",TmpRegName(res->Reg),name,NULL);
  return res;
}

struct ExprRes *  doAdd(struct ExprRes * Res1, struct ExprRes * Res2)  { 
  int reg;
  reg = AvailTmpReg();
  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL,"add",TmpRegName(reg),TmpRegName(Res1->Reg),TmpRegName(Res2->Reg)));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

struct ExprRes *  doMinus(struct ExprRes * Res1, struct ExprRes * Res2)  { 
  int reg;
  reg = AvailTmpReg();
  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL,"sub",TmpRegName(reg),TmpRegName(Res1->Reg),TmpRegName(Res2->Reg)));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

struct ExprRes *  doMult(struct ExprRes * Res1, struct ExprRes * Res2)  { 
  int reg;
  reg = AvailTmpReg();
  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL,"mul",TmpRegName(reg),TmpRegName(Res1->Reg),TmpRegName(Res2->Reg)));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

struct ExprRes *  doDiv(struct ExprRes * Res1, struct ExprRes * Res2)  { 
  int reg;
  reg = AvailTmpReg();
  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL,"div",TmpRegName(reg),TmpRegName(Res1->Reg),TmpRegName(Res2->Reg)));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

struct ExprRes *  doRem(struct ExprRes * Res1, struct ExprRes * Res2)  { 
  int reg;
  reg = AvailTmpReg();
  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL,"rem",TmpRegName(reg),TmpRegName(Res1->Reg),TmpRegName(Res2->Reg)));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

struct ExprRes *  doExp(struct ExprRes * Res1, struct ExprRes * Res2)  { 
  int reg1, reg2, reg3;
  char * label1;
  char * label2;
  reg1 = AvailTmpReg();
  reg2 = AvailTmpReg();
  reg3 = AvailTmpReg();
  label1 = GenLabel();
  label2 = GenLabel();
  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL,"addi",TmpRegName(reg1), "$zero","0"));
  AppendSeq(Res1->Instrs,GenInstr(NULL,"addi",TmpRegName(reg3), "$zero","1"));
  AppendSeq(Res1->Instrs,GenInstr(label1,"slt",TmpRegName(reg2), TmpRegName(reg1), TmpRegName(Res2->Reg)));
  AppendSeq(Res1->Instrs,GenInstr(NULL,"beq",TmpRegName(reg2), "$zero", label2));
  AppendSeq(Res1->Instrs,GenInstr(NULL,"mul",TmpRegName(reg3), TmpRegName(reg3), TmpRegName(Res1->Reg)));
  AppendSeq(Res1->Instrs,GenInstr(NULL,"addi",TmpRegName(reg1), TmpRegName(reg1),"1"));
  AppendSeq(Res1->Instrs,GenInstr(NULL,"j",label1, NULL, NULL));
  AppendSeq(Res1->Instrs,GenInstr(label2, NULL, NULL, NULL, NULL));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  ReleaseTmpReg(reg1);
  ReleaseTmpReg(reg2);
  Res1->Reg = reg3;
  free(Res2);
  return Res1;
}

struct ExprRes *  doSLE(struct ExprRes * Res1, struct ExprRes * Res2)  { 
  int reg;
  reg = AvailTmpReg();
  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL,"sle",TmpRegName(reg),TmpRegName(Res1->Reg),TmpRegName(Res2->Reg)));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

struct ExprRes *  doSGE(struct ExprRes * Res1, struct ExprRes * Res2)  { 
  int reg;
  reg = AvailTmpReg();
  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL,"sge",TmpRegName(reg),TmpRegName(Res1->Reg),TmpRegName(Res2->Reg)));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

struct ExprRes *  doAnd(struct ExprRes * Res1, struct ExprRes * Res2)  { 
  int reg;
  reg = AvailTmpReg();
  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL,"and",TmpRegName(reg),TmpRegName(Res1->Reg),TmpRegName(Res2->Reg)));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

struct ExprRes *  doOr(struct ExprRes * Res1, struct ExprRes * Res2)  { 
  int reg;
  reg = AvailTmpReg();
  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL,"or",TmpRegName(reg),TmpRegName(Res1->Reg),TmpRegName(Res2->Reg)));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

struct ExprRes *  doNot(struct ExprRes * Res1)  { 
  char * label;
  char * label2;
  label = GenLabel();
  label2 = GenLabel();
  AppendSeq(Res1->Instrs,GenInstr(NULL,"beq",TmpRegName(Res1->Reg),"$zero",label));
  AppendSeq(Res1->Instrs,GenInstr(NULL,"addi",TmpRegName(Res1->Reg),"$zero","0"));
  AppendSeq(Res1->Instrs,GenInstr(NULL,"j",label2,NULL,NULL));
  AppendSeq(Res1->Instrs,GenInstr(label,"not",TmpRegName(Res1->Reg),TmpRegName(Res1->Reg),NULL));
  AppendSeq(Res1->Instrs,GenInstr(label2,NULL,NULL,NULL,NULL));
  return Res1;
}

struct ExprRes *  doSmaller(struct ExprRes * Res1, struct ExprRes * Res2)  { 
  int reg;
  reg = AvailTmpReg();
  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL,"slt",TmpRegName(reg),TmpRegName(Res1->Reg),TmpRegName(Res2->Reg)));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

struct ExprRes *  doLarger(struct ExprRes * Res1, struct ExprRes * Res2)  { 
  int reg;
  reg = AvailTmpReg();
  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL,"slt",TmpRegName(reg),TmpRegName(Res2->Reg),TmpRegName(Res1->Reg)));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

struct InstrSeq * doPrint(struct ExprRes * Expr) { 
  struct InstrSeq *code; 
  code = Expr->Instrs;
  
  AppendSeq(code,GenInstr(NULL,"li","$v0","1",NULL));
  AppendSeq(code,GenInstr(NULL,"move","$a0",TmpRegName(Expr->Reg),NULL));
  AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));

  AppendSeq(code,GenInstr(NULL,"li","$v0","4",NULL));
  AppendSeq(code,GenInstr(NULL,"la","$a0","_nl",NULL));
  AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));

  ReleaseTmpReg(Expr->Reg);
  free(Expr);

  return code;
}

struct InstrSeq * doPrintL(struct ExprRes * Expr) { 
  int reg1, reg2;
  char * label1;
  char * label2; 
  struct InstrSeq *code;
  code = Expr->Instrs;
  reg1 = AvailTmpReg();
  reg2 = AvailTmpReg();
  label1 = GenLabel();
  label2 = GenLabel();

  AppendSeq(code,GenInstr(NULL,"addi",TmpRegName(reg1),"$zero","0"));
  AppendSeq(code,GenInstr(label1,"slt",TmpRegName(reg2),TmpRegName(reg1),TmpRegName(Expr->Reg)));
  AppendSeq(code,GenInstr(NULL,"beq",TmpRegName(reg2),"$zero",label2));
  AppendSeq(code,GenInstr(NULL,"li","$v0","4",NULL));
  AppendSeq(code,GenInstr(NULL,"la","$a0","_nl",NULL));
  AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));
  AppendSeq(code,GenInstr(NULL,"addi",TmpRegName(reg1),TmpRegName(reg1),"1"));
  AppendSeq(code,GenInstr(NULL,"j",label1, NULL, NULL));
  AppendSeq(code,GenInstr(label2, NULL, NULL, NULL, NULL));

  ReleaseTmpReg(Expr->Reg);
  ReleaseTmpReg(reg1);
  ReleaseTmpReg(reg2);
  free(Expr);

  return code;
}

struct InstrSeq * doPrintS(struct ExprRes * Expr) { 
  int reg1, reg2;
  char * label1;
  char * label2; 
  struct InstrSeq *code;
  code = Expr->Instrs;
  reg1 = AvailTmpReg();
  reg2 = AvailTmpReg();
  label1 = GenLabel();
  label2 = GenLabel();

  AppendSeq(code,GenInstr(NULL,"addi",TmpRegName(reg1),"$zero","0"));
  AppendSeq(code,GenInstr(label1,"slt",TmpRegName(reg2),TmpRegName(reg1),TmpRegName(Expr->Reg)));
  AppendSeq(code,GenInstr(NULL,"beq",TmpRegName(reg2),"$zero",label2));
  AppendSeq(code,GenInstr(NULL,"li","$v0","4",NULL));
  AppendSeq(code,GenInstr(NULL,"la","$a0","space",NULL));
  AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));
  AppendSeq(code,GenInstr(NULL,"addi",TmpRegName(reg1),TmpRegName(reg1),"1"));
  AppendSeq(code,GenInstr(NULL,"j",label1, NULL, NULL));
  AppendSeq(code,GenInstr(label2, NULL, NULL, NULL, NULL));

  ReleaseTmpReg(Expr->Reg);
  ReleaseTmpReg(reg1);
  ReleaseTmpReg(reg2);
  free(Expr);

  return code;
}

struct InstrSeq * doRead(struct IdList* list){
  struct IdList * temp;
  struct InstrSeq * code;
  struct ExprRes * expr = doRval(list->TheEntry->name);
  code = expr->Instrs;

  AppendSeq(code,GenInstr(NULL,"li","$v0","5",NULL));
  AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));
  AppendSeq(code,GenInstr(NULL,"sw", "$v0",list->TheEntry->name,NULL));

  temp = list->Next;
  while (temp != NULL) {
    expr = doRval(temp->TheEntry->name);
    AppendSeq(code, expr->Instrs);
    AppendSeq(code,GenInstr(NULL,"li","$v0","5",NULL));
    AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));
    AppendSeq(code,GenInstr(NULL,"sw","$v0",temp->TheEntry->name,NULL));
    temp = temp->Next;
  }

  ReleaseTmpReg(expr->Reg);
  free(expr);
  free(list);
  return code;
}

struct InstrSeq * doReadArr(char *name, struct ExprRes * Res){
	struct InstrSeq * code;
	code = Res->Instrs;
	int reg;
	reg = AvailTmpReg();
	char sw [10];

	AppendSeq(Res->Instrs, GenInstr(NULL, "la", TmpRegName(reg), name, NULL));
	AppendSeq(Res->Instrs, GenInstr(NULL, "sll", TmpRegName(Res->Reg), TmpRegName(Res->Reg), "2"));
	AppendSeq(Res->Instrs, GenInstr(NULL, "add", TmpRegName(reg), TmpRegName(reg), TmpRegName(Res->Reg)));
	sprintf(sw, "0(%s)", TmpRegName(reg));
	AppendSeq(code,GenInstr(NULL,"li","$v0","5",NULL));
  	AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));
  	AppendSeq(code,GenInstr(NULL,"sw", "$v0",sw,NULL));

	ReleaseTmpReg(Res->Reg);
	ReleaseTmpReg(reg);
	free(Res);
	return code;
}

struct InstrSeq * doPList(struct Node * n){
  Node * temp;
  struct InstrSeq * code;
  code = n->Expr->Instrs;

  AppendSeq(code,GenInstr(NULL,"li","$v0","1",NULL));
  AppendSeq(code,GenInstr(NULL,"move","$a0",TmpRegName(n->Expr->Reg),NULL));
  AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));
  AppendSeq(code,GenInstr(NULL,"li","$v0","4",NULL));
  AppendSeq(code,GenInstr(NULL,"la","$a0","space",NULL));
  AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));

  temp = n->next;

  while (temp != NULL) {
    AppendSeq(code, temp->Expr->Instrs);
    AppendSeq(code,GenInstr(NULL,"li","$v0","1",NULL));
    AppendSeq(code,GenInstr(NULL,"move","$a0",TmpRegName(temp->Expr->Reg),NULL));
    AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));
    AppendSeq(code,GenInstr(NULL,"li","$v0","4",NULL));
    AppendSeq(code,GenInstr(NULL,"la","$a0","space",NULL));
    AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));

    temp = temp->next;
  }

  free(n);
  return code;
}

struct Node * add (struct ExprRes * Expr, struct Node * h){
  Node * h1 = (Node *) malloc(sizeof(Node));
  h1->Expr = Expr;
  h1->next = h;
  return h1;
}

struct IdList * addId (char * n, struct IdList * list){
  IdList * entry = (IdList *) malloc (sizeof(IdList));
  SymEntry * temp = (SymEntry *)malloc(sizeof(SymEntry));
  temp->name = n;
  entry->TheEntry = temp;
  entry->Next = list;
  return entry; 
}

struct InstrSeq * doAssign(char *name, struct ExprRes * Expr) { 
  struct InstrSeq *code;
  if (!findName(table, name)) {
	writeIndicator(getCurrentColumnNum());
	writeMessage("Undeclared variable");
  }
  code = Expr->Instrs;
  AppendSeq(code,GenInstr(NULL,"sw",TmpRegName(Expr->Reg), name,NULL));
  ReleaseTmpReg(Expr->Reg);
  free(Expr);
  return code;
}

struct ExprRes * doEq(struct ExprRes * Res1,  struct ExprRes * Res2) {
	struct ExprRes * Res;
	int reg = AvailTmpReg();
	AppendSeq(Res1->Instrs, Res2->Instrs);
 	Res = (struct ExprRes *) malloc(sizeof(struct ExprRes));
	AppendSeq(Res1->Instrs, GenInstr(NULL, "seq", TmpRegName(reg), TmpRegName(Res1->Reg), TmpRegName(Res2->Reg)));
	Res->Reg = reg;
	Res->Instrs = Res1->Instrs;
	ReleaseTmpReg(Res1->Reg);
  	ReleaseTmpReg(Res2->Reg);
	free(Res1);
	free(Res2);
	return Res;
}

struct InstrSeq * doIf(struct ExprRes *Res, struct InstrSeq * seq) {
	struct InstrSeq *seq2;
	char * label;
	label = GenLabel();
	AppendSeq(Res->Instrs, GenInstr(NULL, "beq", "$zero", TmpRegName(Res->Reg), label));
	seq2 = AppendSeq(Res->Instrs, seq);
	AppendSeq(seq2, GenInstr(label, NULL, NULL, NULL, NULL));
	ReleaseTmpReg(Res->Reg);
	free(Res);
	return seq2;
}

struct InstrSeq * doIfElse(struct ExprRes *Res, struct InstrSeq * seq, struct InstrSeq * seq2) {
	struct InstrSeq *seq3;
	char * label;
	char * label2;
	label = GenLabel();
	label2 = GenLabel ();
	AppendSeq(Res->Instrs, GenInstr(NULL, "beq", "$zero", TmpRegName(Res->Reg), label));
	seq3 = AppendSeq(Res->Instrs, seq);
	AppendSeq(Res->Instrs, GenInstr(NULL, "j", label2, NULL, NULL));
	AppendSeq(seq3, GenInstr(label, NULL, NULL, NULL, NULL));
	seq3 = AppendSeq(Res->Instrs, seq2);
	AppendSeq(seq3, GenInstr(label2, NULL, NULL, NULL, NULL));
	ReleaseTmpReg(Res->Reg);
	free(Res);
	return seq3;
}

struct InstrSeq * doWhile(struct ExprRes *Res, struct InstrSeq * seq) {
	struct InstrSeq *seq2;
	char * label;
	char * label2;
	label = GenLabel();
	label2 = GenLabel();
	Res->Instrs->Label = label;
	AppendSeq(Res->Instrs, GenInstr(NULL, "beq", "$zero", TmpRegName(Res->Reg), label2));
	seq2 = AppendSeq(Res->Instrs, seq);
	AppendSeq(Res->Instrs, GenInstr(NULL, "j", label, NULL, NULL));
	AppendSeq(seq2, GenInstr(label2, NULL, NULL, NULL, NULL));
	ReleaseTmpReg(Res->Reg);
	free(Res);
	return seq2;
}

struct InstrSeq * doFor(struct InstrSeq *seq1, struct ExprRes * Res, struct InstrSeq * seq2, struct InstrSeq * seq3){
	struct InstrSeq *seq4;
	char * label;
	char * label2;
	label = GenLabel();
	label2 = GenLabel();
	Res->Instrs->Label = label;
	AppendSeq(seq1, Res->Instrs);
	AppendSeq(seq1, GenInstr(NULL, "beq", "$zero", TmpRegName(Res->Reg), label2));
	AppendSeq(seq1, seq3);
	seq4 = AppendSeq(seq1, seq2);
	AppendSeq(seq4, GenInstr(NULL, "j", label, NULL, NULL));
	AppendSeq(seq4, GenInstr(label2, NULL, NULL, NULL, NULL));
	ReleaseTmpReg(Res->Reg);
	free(Res);
	return seq4;
}

struct ExprRes * getArr(char * name, struct ExprRes *Res){
	int reg;
	char lw[10];
	reg = AvailTmpReg();
	AppendSeq(Res->Instrs, GenInstr(NULL, "la", TmpRegName(reg), name, NULL));
	AppendSeq(Res->Instrs, GenInstr(NULL, "sll", TmpRegName(Res->Reg), TmpRegName(Res->Reg), "2"));
	AppendSeq(Res->Instrs, GenInstr(NULL, "add", TmpRegName(reg), TmpRegName(reg), TmpRegName(Res->Reg)));
	sprintf(lw, "0(%s)", TmpRegName(reg));
	AppendSeq(Res->Instrs, GenInstr(NULL, "lw", TmpRegName(reg), lw, NULL));
	ReleaseTmpReg(Res->Reg);
	Res->Reg=reg;
	return Res;
}

struct InstrSeq * arrAssign(char * name, struct ExprRes *Res1, struct ExprRes *Res2){
	struct InstrSeq * seq;
   	int reg;
	char sw[10];
	reg = AvailTmpReg();
	seq = Res1->Instrs;
	AppendSeq(seq, Res2->Instrs);
	AppendSeq(seq, GenInstr(NULL, "la", TmpRegName(reg), name, NULL));
	AppendSeq(seq, GenInstr(NULL, "sll", TmpRegName(Res1->Reg), TmpRegName(Res1->Reg), "2"));
	AppendSeq(seq, GenInstr(NULL, "add", TmpRegName(reg), TmpRegName(reg), TmpRegName(Res1->Reg)));
	sprintf(sw, "0(%s)", TmpRegName(reg));
	AppendSeq(seq, GenInstr(NULL, "sw", TmpRegName(Res2->Reg), sw, NULL));
	ReleaseTmpReg(Res1->Reg);
	ReleaseTmpReg(Res2->Reg);
	ReleaseTmpReg(reg);
	free(Res1);
	free(Res2);
	return seq;
}

struct InstrSeq * boolAssign(char * name, int bool){
	struct InstrSeq * seq;
	struct ExprRes * Res = doRval(name);
	seq = Res->Instrs;
	if(bool==0){
	  AppendSeq(seq, GenInstr(NULL, "addi", TmpRegName(Res->Reg), "$zero", "1"));
	  AppendSeq(seq, GenInstr(NULL, "sw", TmpRegName(Res->Reg), name, NULL));
	}
	else{
	  AppendSeq(seq, GenInstr(NULL, "addi", TmpRegName(Res->Reg), "$zero", "0"));
	  AppendSeq(seq, GenInstr(NULL, "sw", TmpRegName(Res->Reg), name, NULL));
	}
	ReleaseTmpReg(Res->Reg);
	free(Res);
	return seq;
}

struct InstrSeq *  doPrintB(struct ExprRes * Expr){
	struct InstrSeq * code;
	char * label1;
	char * label2;
	label1 = GenLabel();
	label2 = GenLabel();
	code = Expr->Instrs;

	AppendSeq(code, GenInstr(NULL, "bne", "$zero", TmpRegName(Expr->Reg), label1));
	AppendSeq(code,GenInstr(NULL,"li","$v0","4",NULL));
   	AppendSeq(code,GenInstr(NULL,"la","$a0","false",NULL));
    	AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));
    	AppendSeq(code,GenInstr(NULL,"li","$v0","4",NULL));
    	AppendSeq(code,GenInstr(NULL,"la","$a0","_nl",NULL));
   	AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));
	AppendSeq(code,GenInstr(NULL,"j",label2,NULL,NULL));

	AppendSeq(code,GenInstr(label1,NULL,NULL,NULL,NULL)); 	
	AppendSeq(code,GenInstr(NULL,"li","$v0","4",NULL));
   	AppendSeq(code,GenInstr(NULL,"la","$a0","true",NULL));
    	AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));
    	AppendSeq(code,GenInstr(NULL,"li","$v0","4",NULL));
    	AppendSeq(code,GenInstr(NULL,"la","$a0","_nl",NULL));
   	AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));
	AppendSeq(code,GenInstr(label2,NULL,NULL,NULL,NULL));

	ReleaseTmpReg(Expr->Reg);
	free(Expr);
	return code;
}

struct InstrSeq * arrAssignB(char * name, struct ExprRes *Res1, int bool){
	struct InstrSeq * seq;
   	int reg, reg2;
	char sw[10];
	reg = AvailTmpReg();
	reg2 = AvailTmpReg();
	seq = Res1->Instrs;
	AppendSeq(seq, GenInstr(NULL, "la", TmpRegName(reg), name, NULL));
	AppendSeq(seq, GenInstr(NULL, "sll", TmpRegName(Res1->Reg), TmpRegName(Res1->Reg), "2"));
	AppendSeq(seq, GenInstr(NULL, "add", TmpRegName(reg), TmpRegName(reg), TmpRegName(Res1->Reg)));
	sprintf(sw, "0(%s)", TmpRegName(reg));
	if(bool==0){
	  AppendSeq(seq, GenInstr(NULL, "addi", TmpRegName(reg2), "$zero", "1"));
	  AppendSeq(seq, GenInstr(NULL, "sw", TmpRegName(reg2), sw, NULL));
	}
	else{
	  AppendSeq(seq, GenInstr(NULL, "addi", TmpRegName(reg2), "$zero", "0"));
	  AppendSeq(seq, GenInstr(NULL, "sw", TmpRegName(reg2), sw, NULL));
	}
	ReleaseTmpReg(Res1->Reg);
	ReleaseTmpReg(reg);
	ReleaseTmpReg(reg2);
	free(Res1);
	return seq;
}

struct InstrSeq * doFunc(char * name){
	struct InstrSeq * seq;
  	if (!findName(table, name)) {
		writeIndicator(getCurrentColumnNum());
		writeMessage("Undeclared variable");
   	}
  	seq = GenInstr(NULL,"jal",name,NULL,NULL);
	return seq;
}

struct ExprRes * initFunc(char * name){
	struct ExprRes *res;
	if (!findName(table, name)) {
		writeIndicator(getCurrentColumnNum());
		writeMessage("Undeclared variable");
   	}
  	res = (struct ExprRes *) malloc(sizeof(struct ExprRes));
  	res->Reg = AvailTmpReg();
	res->Instrs = SaveSeq();
	AppendSeq(res->Instrs, GenInstr(NULL,"jal",name,NULL,NULL));
	AppendSeq(res->Instrs, RestoreSeq());
	AppendSeq(res->Instrs, GenInstr(NULL,"move",TmpRegName(res->Reg),"$v0",NULL));
	return res;
}

struct InstrSeq * doRet(struct ExprRes * Res){
	struct InstrSeq * seq;
	seq = Res->Instrs;
	AppendSeq(seq, GenInstr(NULL,"move","$v0",TmpRegName(Res->Reg),NULL));
	return seq;
}

void							 
Finish(struct InstrSeq *Code)
{ struct InstrSeq *code;
  int hasMore;

  code = GenInstr(NULL,".text",NULL,NULL,NULL);
  AppendSeq(code,GenInstr(NULL,".globl","main",NULL,NULL));
  AppendSeq(code, GenInstr("main",NULL,NULL,NULL,NULL));
  AppendSeq(code,Code);
  AppendSeq(code, GenInstr(NULL, "li", "$v0", "10", NULL)); 
  AppendSeq(code, GenInstr(NULL,"syscall",NULL,NULL,NULL));
  hasMore = startIterator(table);
  while(hasMore){
    if(getCurrentAttr(table)!=NULL){
      struct storeInfo * SI = (struct storeInfo *)getCurrentAttr(table);
      if(strcmp(SI->name, "Func")==0){
	AppendSeq(code,GenInstr((char *)getCurrentName(table),NULL,NULL,NULL,NULL));
	AppendSeq(code,SaveSeq());
	AppendSeq(code,GenInstr(NULL,"sw","$ra","($sp)",NULL));
	AppendSeq(code,SI->seq);
	AppendSeq(code,GenInstr(NULL,"lw","$ra","($sp)",NULL));
	AppendSeq(code,RestoreSeq());
	AppendSeq(code,GenInstr(NULL,"jr","$ra",NULL,NULL));
      }
    }
    hasMore = nextEntry(table);
  }
  AppendSeq(code,GenInstr(NULL,".data",NULL,NULL,NULL));
  AppendSeq(code,GenInstr("_nl",".asciiz","\"\\n\"",NULL,NULL));
  AppendSeq(code,GenInstr(NULL,".align","2",NULL,NULL));
  AppendSeq(code,GenInstr("space",".asciiz","\" \"",NULL,NULL));
  AppendSeq(code,GenInstr(NULL,".align","2",NULL,NULL));
  AppendSeq(code,GenInstr("true",".asciiz","\"true\"",NULL,NULL));
  AppendSeq(code,GenInstr(NULL,".align","2",NULL,NULL));
  AppendSeq(code,GenInstr("false",".asciiz","\"false\"",NULL,NULL));
  AppendSeq(code,GenInstr(NULL,".align","2",NULL,NULL));

  hasMore = startIterator(table);
  while (hasMore) {
    if(getCurrentAttr(table)==NULL){
      AppendSeq(code,GenInstr((char *) getCurrentName(table),".word","0",NULL,NULL));
    }
    else{
      struct storeInfo * SI = (struct storeInfo *)getCurrentAttr(table);
      if(strcmp(SI->name, "Bool")==0){
	AppendSeq(code,GenInstr((char *) getCurrentName(table),".word","0",NULL,NULL));
      }
      else if(strcmp(SI->name, "Arr")==0){
        int temp=atoi(SI->size);
        temp=temp*4;
        char print[10];
        sprintf(print, "%d", temp);
        AppendSeq(code,GenInstr((char *) getCurrentName(table),".space",print,NULL,NULL));
      }
    }
    hasMore = nextEntry(table);
 }
  
  WriteSeq(code);
  
  return;
}