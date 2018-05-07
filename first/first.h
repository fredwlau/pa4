#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#ifndef _HEADER_H
#define _HEADER_H

typedef struct twoCacheBlock{
	int tag;
	int indecks;
	int addrInd;
	int op;
	struct twoCacheBlock *next;
}twoCacheBlock;

typedef struct fullCacheBlock{
	int tag;
	int valid;
	int op;
}fullCacheBlock;

typedef struct twoaddress{
	int numIndBits;
	int numTagBits;
}twoaddress;

typedef struct fulladdress{
	int numTagBits;
}fulladdress;

typedef struct twoCache{
	struct twoCacheBlock **set;
	int numSets;
	int assoc;
}twoCache;

typedef struct fullCache{
	struct fullCacheBlock **set;
	int maxBlock;
}fullCache;

int logBits(int n);
int binToDec(char *addr, int bits);
int power(int base, int expo);
int isPowerOfTwo(int n);
char* hexToBin(char *address);
void getAddrInfo(int cachesize, int blocksize, int numberofsets);
void parseMemAddr(char *binStringAddr, int rw);
void readAddress(FILE *tracefile);
void setupTwoBlock(int n, int m, int rw);
void setupFullBlock(int n, int rw);
void printResults();
void operateOnFullCache(struct fullCacheBlock *block);
void operateOnTwoCache(struct twoCacheBlock *block);



#endif