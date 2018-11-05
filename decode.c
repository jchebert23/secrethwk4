#define _GNU_SOURCE
#include "hash.c"
#include "stack.c"
#include <stdlib.h>
#include <stdio.h>


int debugPrint2=0;

void decode(void){
   hash *h = initHash( 2 << 11);
   pair **table=h->table;
   Stack *s = 0;
   int oldC = 0;
   int newC = 0;
   char *buffer = malloc(sizeof(char) * 10);
   size_t sizeOfBuf = 10;
   while((getline(&buffer, &sizeOfBuf,stdin))!=-1)
    {
	int c = atoi(buffer);
	newC=c;
	if(debugPrint2){printf("Int at line: %d\n", c);}
	while(table[c]->pref != 0)
	{
		stackPush(s, table[c]->nchar);
		c=table[c]->pref;
	}
	int finalK = table[c]->nchar;
	printf("%c", finalK);
	while(s){printf("%d", stackPop(s));}
	if(oldC!=0)
	{
		addToHash(table, h->curbits,oldC, finalK);
	}
	oldC = newC;
    }	
    destroyHash(h);
}

int main(int argc, char **argv)
{
   decode(); 
}
