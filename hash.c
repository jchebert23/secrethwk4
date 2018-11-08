#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <linux/limits.h>
#include <inttypes.h>
#include <errno.h>
#include <limits.h>
int debugPrint1=0;
int dePrint=0;
int debugPrint7=0;
#define INIT(p,c) (p)
#define STEP(p,c) (2 * (((p) <<CHAR_BIT)|(c)) +1)
#define RED(x,m) ((x) & (m - 1))

typedef struct pair{
    unsigned int notNull:1;
    unsigned int nchar:8;
    unsigned int pref:22;
    unsigned int npref:22;
    unsigned int match:1;
}pair;

 typedef struct hash{
    pair *table;
    int pairsStored;
    int curbits;
    int power;
    int maxbit;
    int block;
    double ratio;
    int rOption;
}hash;

void extendhash(hash *h);

int addToHash(hash *h,int max, int p , int c){
   

    double loadAverage = (double) (h->pairsStored+1) / (double)h->curbits;

    if(loadAverage <= .99)
    {
    
    pair *table = h->table;
    
    h->pairsStored++;
    int start;
    for(int i=0; i<max; i++){
    int increment = RED(INIT(p,c) + (i * STEP(p,c)), h->curbits);
    if(table[increment].notNull==0 && increment!=0){start=increment; break;}}
    table[start].nchar=c;
    table[start].pref=p;
    table[start].notNull=1;
    if(debugPrint1){printf("Adding nchar: %d, and pref: %d to hash at index %d\n", c, p, start);}
    return start;
    }
return 0;
}
int  search(pair *table, int max, int p, int c){
    for(int i=0; i<max; i++){
    int increment = RED(INIT(p,c) + (i * STEP(p,c)), max);
    if(table[increment].notNull && table[increment].pref==p && table[increment].nchar==c){return increment;}}
    return 0;
}
 hash * initHash(int rows, int maxbits)
{

    hash *h=malloc(sizeof(hash));
    h->maxbit = maxbits;
    h->power =9;
    h->pairsStored=0;
    h->rOption=0;
    if(debugPrint1){printf("Line %d in intialize hash; num of rows: %d\n", __LINE__, rows);}
    h->curbits= rows;
    h->table = malloc(sizeof(pair) * rows);
    for(int i=0; i<rows; i++){h->table[i].notNull=0;}
    for(int i=0; i<256; i++)
    {
	addToHash(h, h->curbits, 0, i);
    }
    return h;
}
 void printHash(hash *h)
{
//	for(int i=0; i<h->curbits; i++){if(h->table[i]){printf("At Index: %d, pref: %d , char: %d\n",i, h->table[i].pref, h->table[i].nchar);}}
}
 void destroyHash(hash *h)
{
	free(h->table);
	free(h);
}


int copyOver(hash *newHash, hash *oldHash, int nchar, int pref)
{
    //This could be sped up
    int s;
    if(pref!=0)
    {
	if(oldHash->table[pref].match==0)
	{
		int oldpref=pref;
		oldHash->table[pref].match=1;
		pref = copyOver(newHash, oldHash, oldHash->table[pref].nchar, oldHash->table[pref].pref);
		oldHash->table[oldpref].npref=pref;
		s = addToHash(newHash, newHash->curbits, pref, nchar);
	}
	else{s = addToHash(newHash, newHash->curbits, oldHash->table[pref].npref, nchar);}
    }
    else{s=search(newHash->table, newHash->curbits, pref, nchar);}
    return s;

}

void destroyTable(hash *h)
{

	free(h->table);
}



void  extendhash(hash *h)
{
	//IMPORTANT, should this be an int
	
	int prevSize= h->curbits;
	hash *newHash = initHash(prevSize << 1, h->maxbit);
	newHash->power= h->power +1;
	//int *matching = malloc(sizeof(int) * prevSize);
	for(int i=0; i<prevSize; i++){h->table[i].match=0;}
	for(int i=0; i<256; i++)
	{
	int s = search(h->table, h->curbits, 0, i);
	h->table[s].match=1;
	h->table[s].npref=search(newHash->table, newHash->curbits, 0, i);
	}
	for(int i = 0; i<h->curbits; i++)
	{
		if(h->table[i].match==0)
		{
		if(h->table[i].notNull){h->table[i].npref=copyOver(newHash, h, h->table[i].nchar, h->table[i].pref); h->table[i].match=1;}
		}
	}	
	destroyTable(h);
	h->curbits=newHash->curbits;
	h->power= newHash->power;
	h->table= newHash->table;
	if(debugPrint7){printHash(h);}
	free(newHash);
}

/*
int main(int argc, char **argv)
{
hash *h = initHash( 2 << 8 );
printf("new power: %d, new curbits, %d\n", h->power, h->curbits);
addToHash(h, h->curbits, search(h->table, h->curbits, 0, 'a')  , 'a');
addToHash(h, h->curbits, search(h->table, h->curbits, 0, 'a')  , 'b');
addToHash(h, h->curbits, search(h->table, h->curbits,search(h->table, h->curbits, 0, 'a'), 'a')  , 'a');
extendhash(h);
printf("new power: %d, new curbits, %d\n", h->power, h->curbits);
extendhash(h);
printf("new power: %d, new curbits, %d\n", h->power, h->curbits);
printHash(h);
}
*/
