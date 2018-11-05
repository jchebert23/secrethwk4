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
typedef struct pair{
    int nchar;
    int pref;
}pair;
 typedef struct hash{
    pair **table;
    int curbits;
}hash;
 int init(int p, int c, int max)
{
	return (p % max);
}
 int stepfunc(int p, int c, int max){
	return ((2 * (((p)<<CHAR_BIT)|(c)) + 1) % max);
}
 void addToHash(pair **table,int max, int p , int c){
    int start = init(p, c, max);
    int step = stepfunc(p,c, max);
    if(debugPrint1){printf("Adding nchar: %d, and pref: %d to with start: %d and step: %d\n", c, p, start, step);}
    while(table[start] || start==0){start= (start + step) % max;}
    table[start]=malloc(sizeof(pair));
    table[start]->nchar=c;
    table[start]->pref=p;
    if(debugPrint1){printf("Adding nchar: %d, and pref: %d to hash at index %d\n", c, p, start);}
    }
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
    if(debugPrint1){printf("Line %d; num of rows: %d\n", __LINE__, rows);}
    h->curbits= rows;
    h->table = calloc( h->curbits, sizeof(pair *));
    pair ** table= h->table;
    for(int i=0; i<256; i++)
    {
	addToHash(table, h->curbits, 0, i);	
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
