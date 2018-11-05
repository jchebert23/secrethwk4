#include "hash.c"
#include "stack.c"
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
    FILE *newfile = fopen("encodeOutput.txt", "w");
    int c=0;
    int k;
    while((k =getc(stdin)) != EOF)
    {

	    if(debugPrint){printf("Testing char %c with prefix code: ", k); 
		    printCodeAtIndex(h->table, c);}	    		   
	    if(search(h->table, h->curbits, c, k)==0)
	    {
		    if(debugPrint){printf("Adding to outputfile: %c with prefix code: ", k); printCodeAtIndex(h->table, c);}	    
		    fprintf(newfile,"%d\n", c);
		    addToHash(h->table, h->curbits, c, k);
		    if(debugPrint){printf("Added to table at index %d\n", search(h->table, h->curbits, c, k));} 
		    c = search(h->table, h->curbits, 0, k);
	    }
	    else{c=search(h->table, h->curbits, c, k);}
    
    }

    fprintf(newfile,"%d\n", c);
    fclose(newfile);
    destroyHash(h);
}

void decode(void){
   hash *h = initHash( 2 << 11);
   pair **table=h->table;
   Stack s = STACK_EMPTY;
   int oldC = 0;
   int newC = 0;
   int finalK=0;
   char *buffer = malloc(sizeof(char) * 10);
   size_t sizeOfBuf = 10;
   //may have problem with two files open
   FILE *encodeOutput = fopen("encodeOutput.txt", "r");
   FILE *decodeOutput = fopen("decodeOutput.txt", "w");
   while((getline(&buffer, &sizeOfBuf,encodeOutput))!=-1)
    {
        int c = atoi(buffer);
        newC=c;
	if(table[c]==0){stackPush(&s, finalK); c = oldC;}
        while(table[c]->pref != 0)
        {
		if(debugPrint2){printf("Line %d in ed.c, pushing %d to the stack in decode", __LINE__, c);}
                stackPush(&s, table[c]->nchar);
                c=table[c]->pref;
        }
        finalK = table[c]->nchar;
        fprintf(decodeOutput,"%c", finalK);
        while(s){fprintf(decodeOutput, "%c", stackPop(&s));}
        if(oldC!=0)
        {
                addToHash(table, h->curbits,oldC, finalK);
		if(debugPrint){printf("Adding to outputfile: %c with prefix code: ", finalK); printCodeAtIndex(h->table, oldC);}	    
        }
        oldC = newC;
    }
    fclose(encodeOutput);
    fclose(decodeOutput);
    destroyHash(h);
}

int main(int argc, char **argv){
    encode();
    decode();
}
