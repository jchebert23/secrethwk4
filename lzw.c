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


void emptyTable(hash *h)
{

    destroyTable(h);
    h->curbits = (2 << 8);
    h->power= 9;
    h->pairsStored= 0;
    h->table=calloc(h->curbits, sizeof(pair *));
    for(int i=0; i<256; i++)
    {
	addToHash(h, h->curbits, 0, i);
    }
}


void encode(hash *h){
    int c=0;
    int k;
    int i;
    putBits(8, h->maxbit);
    int nread=0;
    int nsent=0;
    int codes=0;
    while((k =getc(stdin)) != EOF)
    {
	    nread+=8;
	    if(debugPrint){printf("Testing char %c with prefix code: ", k); 
		    printCodeAtIndex(h->table, c);}	    		   
	    if((i=search(h->table, h->curbits, c, k))==0)
	    {
		    if(debugPrint){printf("Adding to outputfile: %c with prefix code: ", k); printCodeAtIndex(h->table, c);}	    
		    putBits(h->power, c);
		    nsent+=h->power;
		    codes+=1;
			double loadAverage =(double) (h->pairsStored+1) / (double)h->curbits;
			if(loadAverage>=.99){
			    if(h->power<h->maxbit) 
			    {extendhash(h);}}
		    addToHash(h, h->curbits, c, k);
		    if(debugPrint){printf("Added to table at index %d\n", search(h->table, h->curbits, c, k));} 
		    
		    
		    if(h->rOption)
		    {
			    if(h->block==codes)
			    {
				    if((double) nsent > (h->ratio * (double) nread))
				    {
				    emptyTable(h);	    
				    putBits(h->power, 0);
				    }
				    nread=0;
				    nsent=0;
				    codes=0;
			    }
		    }
		    
		    
		    c = search(h->table, h->curbits, 0, k);
	    }
	    else{c=i;}
    
    }
    if(c){putBits(h->power, c);}
    flushBits();
    destroyHash(h);
}

void decode(hash *h){
   Stack s = STACK_EMPTY;
   int oldC = 0;
   int newC = 0;
   int finalK=0;
   int c=0;
   //may have problem with two files open
   while((c= newC= getBits(h->power))!=EOF)
    {	
	if(c==0)
	{
	oldC=0;
	emptyTable(h);
	continue;}
	if(h->table[c]==0){stackPush(&s, 256); c = oldC;}
        while(h->table[c]->pref != 0)
        {
		if(debugPrint2){printf("Line %d in ed.c, pushing %d to the stack in decode", __LINE__, c);}
                stackPush(&s, h->table[c]->nchar);
                c=h->table[c]->pref;
        }
        finalK = h->table[c]->nchar;
        putchar(finalK);
        while(s){int i = stackPop(&s);
		if(i==256){putchar(finalK);}
		else{putchar(i);}}	
        if(oldC!=0)
        {

                addToHash(h, h->curbits,oldC, finalK);
		double loadAverage =(double) (h->pairsStored+1) / (double)h->curbits;
		if(loadAverage>=.99){
		    if(h->power<h->maxbit) 
		    {extendhash(h);}}
		if(debugPrint){printf("Adding to outputfile: %c with prefix code: ", finalK); printCodeAtIndex(h->table, oldC);}
        }
        oldC = newC;
    }

    destroyHash(h);
}

int maxbits(int argc, char **argv){
    int output=12;
    for(int i=0; i<argc; i++)
    {
	if(strcmp(argv[i], "-m")==0)
	{
	    if(i+1 >= argc)
	    {
		    fprintf(stderr, "bad m option\n");
		    exit(0);
	    }
	    output = atoi(argv[i+1]);
	}
    }
    if(output<=8 || output>22){output=12;}
    return output;	
}

char * blockRatio(int argc, char **argv)
{
	char *blockR=0;
	for(int i=0; i<argc; i++)
	{
	    if(strcmp(argv[i], "-r")==0)
	    {
		    if(i+1 >= argc)
		    {

			    fprintf(stderr, "bad m option\n");
			    exit(0);
		    }
		    blockR=argv[i+1];
	    }
	}
	return blockR;


}

int getIntFromBlock(char *s)
{
	int x = (int) atof(s);
	if(x<1)
	{
	
			    fprintf(stderr, "bad m option\n");
			    exit(0);
	}
	return (int) x; 
}

int main(int argc, char **argv){
    int maxbit = maxbits(argc, argv);
    char *block = blockRatio(argc, argv);
    int rOption=0;
    double ratio;
    int blockSize;
    if(block){rOption=1;
    blockSize =  (getIntFromBlock(block));
    ratio = atof(block) -  (double) (int) atof(block);}
    if(strcmp(argv[0], "./encode")==0)
    {
	    
	    hash *h = initHash(2 << 8, maxbit);
	    if(rOption)
	    {
	    h->rOption=rOption;
	    h->block=blockSize;
	    h->ratio=ratio;
	    }
	    encode(h);
    }
    else if(strcmp(argv[0], "./decode")==0)
    {
	    int max = getBits(8);
	    hash *h = initHash(2 << 8, max);
	    decode(h);
    }
}
