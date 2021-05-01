#include "IOMngr.h"

FILE* inFile;
FILE* outFile;
char* readLine;
int line=0;
int column=0;

/*int main (int argc, char * argv []){
  int temp;
  char c;
  temp=openFiles("inFile.txt", "outFile.txt");
  c=getNextSourceChar();
  while(c!='\0'){
    printf("%c\nLine: %d\nColumn: %d\n--------\n", c, line, column);
    c=getNextSourceChar();
  }
  writeIndicator(1);
  writeMessage("\nTest writeMessage\n");
  closeFiles();
}*/

int openFiles(char * sourceName, char * listingName){
  inFile=fopen(sourceName, "r");
  if(inFile==NULL){
    return 0;
  }
  if(listingName!=NULL){
    outFile=fopen(listingName, "w");
    if(outFile==NULL){
      return 0;
    }
  }
  else{
    outFile=stdout;
  }
  return 1;
}

void closeFiles(){
  fclose(inFile);
  if(outFile!=NULL){
    fclose(outFile);
  }
}

char getNextSourceChar(){
  if(line==0 && column==0){
    readLine=(char *)malloc(sizeof(char)*MAXLINE);
    readLine=fgets(readLine, MAXLINE, inFile);
    line=1;
    fprintf(outFile, "Line\tContents\n");
    fprintf(outFile, "%d\t%s", line, readLine);
    return readLine[0];
  }
  else{
    column++;
    if(readLine[column]=='\0'){
      readLine=fgets(readLine, MAXLINE, inFile);
      line++;
      if(readLine!=NULL){
	fprintf(outFile, "%d\t%s", line, readLine);
      }
      column=0;
    }
    if(readLine==NULL){
      return EOF;
    }
  }
  return readLine[column];
}

void writeIndicator(int column){
  int i;
  fputs("\t", outFile);
  for(i=0; i<column; i++){
    fputs(" ", outFile);
  }
  fputs("^\n", outFile);
}

void writeMessage(char * message){
  fputs(message, outFile);
  fputs("\n", outFile);
}

int getCurrentLineNum(){
  return line;
}

int getCurrentColumnNum(){
  return column;
}
