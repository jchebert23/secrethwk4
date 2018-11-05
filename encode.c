#include "hash.c"
int debugPrint=0;

void encode(void){
    hash *h = initHash(2 << 11);
    FILE *newfile = fopen("encodeOutput.txt", "w");
    int c=0;
    int k;
    while((k =getc(stdin)) != EOF)
    {

	    if(debugPrint){printf("Testing char %c with prefix %d\n", k, c);}	    		   
	    if(search(h->table, h->curbits, c, k)==0)
	    {
		    if(debugPrint){printf("Adding to outputfile: %c with prefix %d\n", k, c);}	    
		    printf("%d\n", c);
		    addToHash(h->table, h->curbits, c, k);
		    c = search(h->table, h->curbits, 0, k);
	    }
	    else{c=search(h->table, h->curbits, c, k);}
    
    }

    printf("%d\n", c);
    fclose(newfile);
    destroyHash(h);
}

int main(int argc, char **argv){
    encode();
}
