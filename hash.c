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
typedef struct pair{
    unsigned int nchar:8;
    unsigned int pref:23;
}pair;
 typedef struct hash{
    pair **table;
    int pairsStored;
    int curbits;
}hash;
 int init(int p, int c, int max)
{
	return (p % max);
}
 int stepfunc(int p, int c, int max){
	return ((2 * (((p)<<CHAR_BIT)|(c)) + 1) % max);
}


hash * extendhash(hash *h);

void addToHash(hash *h,int max, int p , int c){
    double loadAverage =(double) (h->pairsStored+1) / (double)h->curbits;
    if(loadAverage <= .99)
    {
    //extendhash(h);
    
    pair ** table = h->table;
    
    h->pairsStored++;
    int start = init(p, c, max);
    int step = stepfunc(p,c, max);
    if(debugPrint1){printf("Adding nchar: %d, and pref: %d to with start: %d and step: %d\n", c, p, start, step);}
    while(table[start] || start==0){start= (start + step) % max;}
    table[start]=malloc(sizeof(pair));
    table[start]->nchar=c;
    table[start]->pref=p;
    if(debugPrint1){printf("Adding nchar: %d, and pref: %d to hash at index %d\n", c, p, start);}
    }}
int  search(pair **table, int max, int p, int c){
    int start = init(p, c, max);
    while(table[start] || start==0){
    if(start!=0){if(table[start]->pref == p && table[start]->nchar == c){ return start;}}
    start= (start + stepfunc(p, c, max)) % max;}
    return 0;
}
 hash * initHash(int rows)
{
    hash *h=malloc(sizeof(hash));
    h->pairsStored=0;
    if(debugPrint1){printf("Line %d in intialize hash; num of rows: %d\n", __LINE__, rows);}
    h->curbits= rows;
    h->table = calloc( h->curbits, sizeof(pair *));
    for(int i=0; i<256; i++)
    {
	addToHash(h, h->curbits, 0, i);
    }
    return h;
}
 void printHash(hash *h)
{
	for(int i=0; i<h->curbits; i++){if(h->table[i]){printf("At Index: %d, pref: %d , char: %d\n",i, h->table[i]->pref, h->table[i]->nchar);}}
}
 void destroyHash(hash *h)
{
	for(int i=0; i<h->curbits; i++){if(h->table[i]){free(h->table[i]);}}
	free(h->table);
	free(h);
}


int copyOver(hash *newHash, hash *oldHash, int nchar, int pref)
{
    //This could be sped up
    if(pref!=0)
    {
	pref = copyOver(newHash, oldHash, oldHash->table[pref]->nchar, oldHash->table[pref]->pref);
	if(search(newHash->table, newHash->curbits, nchar, pref)==0)
	{
	addToHash(newHash, newHash->curbits, pref, nchar);
	}
    }
    return search(newHash->table, newHash->curbits, pref, nchar);

}

hash * extendhash(hash *h)
{
	//IMPORTANT, should this be an int
	int prevSize= h->curbits;
	hash *newHash = initHash(prevSize << 1);
	for(int i = 0; i<h->curbits; i++)
	{
		if(h->table[i]){copyOver(newHash, h, h->table[i]->nchar, h->table[i]->pref);}
	}
	destroyHash(h);
	return newHash;
}

/*
int main(int argc, char **argv)
{
hash *h = initHash( 2 << 9 );
addToHash(h, h->curbits, search(h->table, h->curbits, 0, 'a')  , 'a');
addToHash(h, h->curbits, search(h->table, h->curbits, 0, 'a')  , 'b');
hash *newHash = extendhash(h);
printf("Location of \'aa\' in new Hash: %d\n", search(newHash->table, newHash->curbits, search(newHash->table, newHash->curbits, 0, 'a'), 'a'));
}
*/
