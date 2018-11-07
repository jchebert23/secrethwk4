#include "hash.c"
#include "stack.c"
#include "/c/cs323/Hwk4/code.h"
int debugPrint=0;
int debugPrint2=0;

void printCodeAtIndex(pair **table, int c )
{
	Stack s= STACK_EMPTY;
	if(c){
	while(table[c]->pref != 0)
	{
		stackPush(&s, table[c]->nchar);
		c=table[c]->pref;
	}
	int finalK = table[c]->nchar;
	printf("%c", finalK);
        while(s){printf("%c", stackPop(&s));}
	}
	printf("\n");
	
}

void encode(void){
    hash *h = initHash(2 << 11);
    int c=0;
    int k;
    while((k =getc(stdin)) != EOF)
    {

	    if(debugPrint){printf("Testing char %c with prefix code: ", k); 
		    printCodeAtIndex(h->table, c);}	    		   
	    if(search(h->table, h->curbits, c, k)==0)
	    {
		    if(debugPrint){printf("Adding to outputfile: %c with prefix code: ", k); printCodeAtIndex(h->table, c);}	    
		    putBits(12, c);
		    addToHash(h, h->curbits, c, k);
		    if(debugPrint){printf("Added to table at index %d\n", search(h->table, h->curbits, c, k));} 
		    c = search(h->table, h->curbits, 0, k);
	    }
	    else{c=search(h->table, h->curbits, c, k);}
    
    }
    if(c){putBits(12, c);}
    flushBits();
    destroyHash(h);
}

void decode(void){
   hash *h = initHash( 2 << 11);
   pair **table=h->table;
   Stack s = STACK_EMPTY;
   int oldC = 0;
   int newC = 0;
   int finalK=0;
   int c=0;
   //may have problem with two files open
   while((c= newC= getBits(12))!=EOF)
    {
	if(table[c]==0){stackPush(&s, finalK); c = oldC;}
        while(table[c]->pref != 0)
        {
		if(debugPrint2){printf("Line %d in ed.c, pushing %d to the stack in decode", __LINE__, c);}
                stackPush(&s, table[c]->nchar);
                c=table[c]->pref;
        }
        finalK = table[c]->nchar;
        putchar(finalK);
        while(s){putchar(stackPop(&s));}
        if(oldC!=0)
        {
                addToHash(h, h->curbits,oldC, finalK);
		if(debugPrint){printf("Adding to outputfile: %c with prefix code: ", finalK); printCodeAtIndex(h->table, oldC);}	    
        }
        oldC = newC;
    }

    destroyHash(h);
}

int main(int argc, char **argv){
    if(strcmp(argv[0], "./encode")==0)
    {
	    encode();
    }
    else if(strcmp(argv[0], "./decode")==0)
    {
	    decode();
    }

}
