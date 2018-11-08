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
}pair;

 typedef struct hash{
    pair *table;
    unsigned int pairsStored:22;
    unsigned int curbits:22;
    unsigned int power:5;
    unsigned int maxbit:5;
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
    if(table[increment].notNull==0 && increment!=0 && increment!=p){start=increment; break;}}
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


void copyOver(hash *newHash, hash *oldHash, int index, int *mdict)
{
    //This could be sped up
    int pref= oldHash->table[index].pref;
    int nchar= oldHash->table[index].nchar;
    
	while(mdict[pref]==0)
	{
		
		copyOver(newHash, oldHash, pref, mdict);
	}
	mdict[index]=addToHash(newHash, newHash->curbits, mdict[pref], nchar);
	 
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
	int *matching = calloc(prevSize, sizeof(int));
	for(int i=0; i<256; i++)
	{
	int s = search(h->table, h->curbits, 0, i);
	matching[s]= search(newHash->table, newHash->curbits, 0, i);
	}
	for(int i = 0; i<h->curbits; i++)
	{
		if(matching[i]==0)
		{
		if(h->table[i].notNull){copyOver(newHash, h, i,matching);}
		}
	}	
	destroyTable(h);
	free(matching);
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
