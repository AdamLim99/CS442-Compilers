#include "SymTab.h"

/*int main (int argc, char * argv []){
  printf("Start\n");
  SymTab* table=createSymTab(5);
  int i, entN, fN, temp;
  printf("Test startIter-Expected 0: %d\n", startIterator(table));
  char* testN[]={"Aa", "Bb", "Cc", "Dd", "Ee", "Ff"};
  printf("EnterName: ");
  for(i=0; i<6; i++){
    entN=enterName(table, testN[i]);
    printf("%s : %d\t", testN[i], entN);
  }
  printf("\n findName: ");
  for(i=0; i<6; i++){
    fN=findName(table, testN[i]);
    printf("%s : %d\t", testN[i], fN);
  }
  temp=enterName(table, "Cc");
  printf("\nEnter Cc, expected 0: %d\n", temp);
  printf("Check current after enter, expected Cc: %s\n", getCurrentName(table));
  temp=findName(table, "Zz");
  printf("Find Zz, expected 0: %d\n", temp);
  printf("Check current after find returned 0, expected Cc: %s\n", getCurrentName(table));
  printf("Test getCurrentAttr: %p\n", getCurrentAttr(table));
  void * testSCA=&i;
  setCurrentAttr(table, testSCA);
  printf("Test getCurrentAttr after calling set: %p\n", getCurrentAttr(table));
  printf("Test hasCurrent-Expected 1: %d\n", hasCurrent(table));
  printf("Test startIter-Expected 1: %d\n", startIterator(table));
  printf("Test getCurrentName-Expected Ee: %s\n", getCurrentName(table));
  printf("Test nextEntry-Expected 1: %d\n", nextEntry(table));
  printf("getCurrentName after next-Expected Cc: %s\n", getCurrentName(table));
  while(nextEntry(table)==1){
    printf("%s\t", getCurrentName(table));
  }
  printf("\ngetCurrentName at the end-Expected Bb: %s\n", getCurrentName(table));
  SymTab* table2=createSymTab(10);
  destroySymTab(table);
  destroySymTab(table2);
  printf("Done\n");
  }*/

SymTab * createSymTab(int size){
  if(size<=0){
    fprintf(stderr, "Error: size should be >=1!\n");
    exit(-1);
  }
  else{
    int i;
    SymTab* table=(SymTab *)malloc(sizeof(SymTab *));
    table->size=size;
    table->contents=(SymEntry **)malloc(sizeof(SymEntry *)*size);
    table->current=(SymEntry *)malloc(sizeof(SymEntry *));
    for(i=0; i<size; i++){
      table->contents[i]=(SymEntry *)malloc(sizeof(SymEntry *));
      table->contents[i]->name=NULL;
      table->contents[i]->attribute=NULL;
      table->contents[i]->next=NULL;
    }
    table->current=NULL;
    return table;
  }
}

void destroySymTab(SymTab *table){
  int i;
  SymEntry* temp;
  for(i=0; i<table->size; i++){
    if(table->contents[i]->next!=NULL){
      table->current=table->contents[i]->next;
      free(table->contents[i]->name);
      free(table->contents[i]);
      if(table->current->next==NULL){
	free(table->current->name);
	free(table->current);
      }
      else{
	while(table->current->next!=NULL){
	  free(table->current->name);
	  temp=table->current->next;
	  free(table->current);
	  table->current=temp;
	}
      }
    }
    else{
      free(table->contents[i]->name);
      free(table->contents[i]);
    }
  }
  free(table->contents);
  free(table);
}

int enterName(SymTab *table, char *name){
  if(findName(table, name)==1){
    return 0;
  }
  else{
    int i, len, total;
    len=strlen(name);
    total=0;
    for(i=0; i<len; i++){
      total=37*total+name[i];
    }
    total=(total)%(table->size);
    if(total<0){
      total=total+(table->size);
    }
    if(table->contents[total]->name==NULL){
      table->contents[total]->name=strdup(name);
      table->current=table->contents[total];
    }
    else{
      SymEntry* temp=(SymEntry *)malloc(sizeof(SymEntry *));
      temp->name=strdup(name);
      if(table->contents[total]->next==NULL){
	table->contents[total]->next=temp;
	table->current=temp;
      }
      else{
	table->current=table->contents[total];
	while(table->current->next!=NULL){
	  table->current=table->current->next;
	}
	table->current->next=temp;
	table->current=temp;
      }
    }
    return 1;
  }
}

int findName(SymTab *table, char *name){
  int i, result;
  SymEntry* temp=(SymEntry *)malloc(sizeof(SymEntry *));
  temp=table->current;
  for(i=0; i<table->size; i++){
    if(table->contents[i]->name!=NULL){
      result=strcmp(name, table->contents[i]->name);
      if(result==0){
	table->current=table->contents[i];
	return 1;
      }
      if(table->contents[i]->next!=NULL){
	table->current=table->contents[i]->next;
	result=strcmp(name, table->current->name);
	if(result==0){
	  return 1;
	}
	else{
	  while(table->current->next!=NULL){
	    table->current=table->current->next;
	    result=strcmp(name, table->current->name);
	    if(result==0){
	      return 1;
	    }
	  }
	}
      }
    }
  }
  table->current=temp;
  return 0;
}

int hasCurrent(SymTab *table){
  if(table->current!=NULL){
    return 1;
  }
  return 0;
}

void setCurrentAttr(SymTab *table, void *attr){
  if(hasCurrent(table)==1){
    table->current->attribute=attr;
  }
}

void * getCurrentAttr(SymTab *table){
  if(hasCurrent(table)==1){
    return table->current->attribute;
  }
  return NULL;
}

char * getCurrentName(SymTab *table){
  if(hasCurrent(table)==1){
    return table->current->name;
  }
  return NULL;
}

int startIterator(SymTab *table){
  int i;
  for(i=0; i<table->size; i++){
    if(table->contents[i]->name!=NULL){
      table->current=table->contents[i];
      return 1;
    }
  }
  return 0;
}

int nextEntry(SymTab *table){
  SymEntry* temp=(SymEntry *)malloc(sizeof(SymEntry *));
  if(table->current->next==NULL){
    int i, j, check, result;
    check=0;
    for(i=0; i<table->size; i++){
      if(table->contents[i]->name!=NULL){
	result=strcmp(table->current->name, table->contents[i]->name);
	if(result==0){
	  break;
	}
	if(table->contents[i]->next!=NULL){
	  temp=table->contents[i]->next;
	  result=strcmp(temp->name, table->current->name);
	  if(result==0){
	    break;
	  }
	  else{
	    while(temp->next!=NULL){
	      temp=temp->next;
	      result=strcmp(temp->name, table->current->name);
	      if(result==0){
		check=1;
		break;
	      }
	    }
	    if(check==1){
	      check=0;
	      break;
	    }
	  }
	}
      }
    }
    for(j=i+1; j<table->size; j++){
      if(table->contents[j]->name!=NULL){
	table->current=table->contents[j];
	break;
      }
    }
    //printf("j = %d\t", j);
    if(j==table->size){
      return 0;
    }
  }
  else{
    table->current=table->current->next;
  }
  return 1;
}
