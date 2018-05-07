#include "first.h"

struct twoCache *ducati;
struct twoCache *ducatiB;
struct fullCache *aprilia;
struct twoaddress *ktm;
struct fulladdress *mvagusta;
//char *op;
int reads=0, writes=0, hit=0, miss=0, z=0, breads=0, bwrites=0, bhit=0, bmiss=0, bZero=0;
int count = 0;

int binToDec(char *addr, int bits){
	int s, sum=0, exponent=0;
	for(s = bits-1; s >= 0; s--){
   	if(addr[s] == '1'){
      	sum = sum + power(2, exponent);
     		}
     		exponent++;
    }
	return sum;
}

int power(int base, int expo){
	int result=1;
	if(expo==0){
  	 return result;
   }
  	while (expo!=0){
  		result *= base;
  		--expo;
  	}
	return result;
}

void operateOnTwoCache(struct twoCacheBlock *block){
	//printf("The block index and tag are %d %d\n", block->indecks, block->tag);
	if(ducati->assoc==1){//direct map
		if(ducati->set[block->indecks]){//something in location of array already
			if(ducati->set[block->indecks]->tag==block->tag){//direct match
				hit++;
				if(block->op==0){
					writes++;
				}
				return;
			}
			else{//read miss, tags dont match, move new block into array
				miss++;
				if(block->op==1){//read
					reads++;
					ducati->set[block->indecks]=block;
					return;
				}
				else{//write miss
					reads++;
					writes++;
					ducati->set[block->indecks]=block;
					return;
				}
			}
		}
		else{//location at array does not contain anything
			ducati->set[block->indecks]=block;
			miss++;
			if(block->op==1){
				reads++;
				return;
			}
			else{
				reads++;
				writes++;
				return;
			}
		}
	}
	else{//n-way associative
		if(ducati->set[block->indecks]==NULL){
			miss++;
			if(block->op==1){//read miss

				reads++;
				ducati->set[block->indecks]=block;
				
				return;
			}
			else{//write miss
				
				reads++;
				writes++;
				ducati->set[block->indecks]=block;
	
				return;
			}
		}
		else{//something exists in arr
			struct twoCacheBlock *temp=ducati->set[block->indecks];
			int found=0, k=0;
			while(temp!=NULL){
				if(temp->tag==block->tag){
					found = 1;
					hit++;
					if(block->op==0){
						writes++;
					}
					break;
				}
				temp=temp->next;
			}
			if(found==0){
				miss++;
				if(block->op==1){
					reads++;
				}
				else{
					writes++;
					reads++;
				}
				temp=ducati->set[block->indecks];
				while(k<ducati->assoc){
					k++;
					if(k==ducati->assoc){
						temp->next=block;
						ducati->set[block->indecks]=ducati->set[block->indecks]->next;
						break;
					}
					else if(temp->next==NULL){
						temp->next=block;
						break;
					}
					temp=temp->next;
				}
			}
		
		}
	}
}
void boperateOnTwoCache(struct twoCacheBlock *block){

	if(ducatiB->assoc==1){//direct map
		if(ducatiB->set[block->indecks]){//something in location of array already
			if(ducatiB->set[block->indecks]->tag==block->tag){//direct match				
				bhit++;
				if(block->op==0){
					bwrites++;
				}
				return;
			}
			else{//read miss, tags dont match, move new block into array
				bmiss++;
				if(block->op==1){//read
					breads++;
					ducatiB->set[block->indecks]=block;
					return;
				}
				else{//write miss
					breads++;
					bwrites++;
					ducatiB->set[block->indecks]=block;
					return;
				}
			}
		}
		else{//location at array does not contain anything
			ducatiB->set[block->indecks]=block;
			bmiss++;
			if(block->op==1){
				breads++;
				return;
			}
			else{
				breads++;
				bwrites++;
				return;
			}
		}
	}
	else{//n-way associative
		if(ducatiB->set[block->indecks]==NULL){//something exists at location of array
			bmiss++;
			if(block->op==1){//read miss
				//printf("read miss\n");
				breads++;
				ducatiB->set[block->indecks]=block;
				//printf("Added block %d at index %d\n", block->tag, block->indecks);
				return;
			}
			else{//write miss
				//printf("write miss\n");
				breads++;
				bwrites++;
				ducatiB->set[block->indecks]=block;
				//printf("Added block %d at index %d\n", block->tag, block->indecks);
				return;
			}
		}
		else{//something exists in arr
			struct twoCacheBlock *temp=ducatiB->set[block->indecks];
			int found=0, k=0;
			while(temp!=NULL){
				if(temp->tag==block->tag){
					found = 1;
					bhit++;
					if(block->op==0){
						bwrites++;
					}
					break;
				}
				temp=temp->next;
			}
			if(found==0){
				bmiss++;
				if(block->op==1){
					breads++;
				}
				else{
					bwrites++;
					breads++;
				}
				temp=ducatiB->set[block->indecks];
				while(k<ducatiB->assoc){
					k++;
					if(k==ducatiB->assoc){
						temp->next=block;
						ducatiB->set[block->indecks]=ducatiB->set[block->indecks]->next;
						break;
					}
					else if(temp->next==NULL){
						temp->next=block;
						break;
					}
					temp=temp->next;
				}
			}
		
		}
	}
}


void operateOnFullCache(struct fullCacheBlock *block){
	int i, k=0, found=0;
	//printf("block tag is %d\n", block->tag);
	for(i=0; i<=aprilia->maxBlock-1; i++){//fully associative search tag at every spot in array for match
		if(aprilia->set[i]->valid==1){//line filled
			//printf("i=%d Comparing tag %d with tag %d\n", i, aprilia->set[i]->tag, block->tag);
			k=i;
			if(aprilia->set[i]->tag==block->tag){//tags match, hit
				//printf("WE GET AND HERE\n");
				hit++;
				if(block->op==0){
					writes++;
				}
				found=1;
				break;
			}
		}
		continue;
	}
	if(found==0){//did not find block in cache
		miss++;
		if(k==aprilia->maxBlock-1){//array is filled
				for (i=0; i<aprilia->maxBlock-1; i++){
					aprilia->set[i]=aprilia->set[i+1];//pops first item of array and shifts all elements
				}
				if(block->op==1){//read miss
					reads++;
				}
				else{
					reads++;
					writes++;
				}
				aprilia->set[i]=block;
				return;
		}
		else{//array is not full
			if(block->op==1){//read miss
				reads++;
				aprilia->set[k+1]=block;
				return;
			}
			else{//write miss
				writes++;
				reads++;
				aprilia->set[k+1]=block;
				return;
			}

		}
	}
	return;
}

void printResults(){
	printf("cache A\n");
	printf("Memory reads: %d\n", reads);
	printf("Memory writes: %d\n", writes);
	printf("Cache hits: %d\n", hit);
	printf("Cache misses: %d\n", miss);
	printf("cache B\n");
	printf("Memory reads: %d\n", breads);
	printf("Memory writes: %d\n", bwrites);
	printf("Cache hits: %d\n", bhit);
	printf("Cache misses: %d\n", bmiss);
}

void setupFullBlock(int n, int rw){
	struct fullCacheBlock *block=malloc(sizeof(struct fullCacheBlock));
	block->tag=n;
	block->valid=1;
	if(rw==1){
		block->op=1;
	}
	else{
		block->op=0;
	}
	operateOnFullCache(block);
	breads=reads;
	bwrites=writes;
	bhit=hit;
	bmiss=miss;
}

void setupTwoBlock(int n, int m, int rw){
	struct twoCacheBlock *block=malloc(sizeof(struct twoCacheBlock));
	block->tag=n;
	block->indecks=m;
	block->next=NULL;
	if(rw==1){
		block->op=1;
	}
	else{
		block->op=0;
	}
	operateOnTwoCache(block);
}

void bsetupTwoBlock(int n, int m, int rw){
	struct twoCacheBlock *block=malloc(sizeof(struct twoCacheBlock));
	block->tag=n;
	block->indecks=m;
	block->next=NULL;
	if(rw==1){
		block->op=1;
	}
	else{
		block->op=0;
	}
	boperateOnTwoCache(block);
}

void readAddress(FILE *tracefile){
	char op;
	char hexaddr[15];
	char *binStringAddr=malloc(sizeof(char)*49);
	int rw=0;
	while(fscanf(tracefile, "%*s %c %s\n", &op, hexaddr)!=EOF){
		//printf("HexTag: %s\n", hexaddr);
		binStringAddr=hexToBin(hexaddr);
		if(op=='R'){
			rw=1;
		}
		else{
			rw=0;
		}
		parseMemAddr(binStringAddr, rw);	
	}
}

void parseMemAddr(char *binStringAddr, int rw){
	if(z==1){
		int numTBits=ktm->numTagBits;
		int numIBits=ktm->numIndBits;
		char tag[ktm->numTagBits+1];
		tag[ktm->numTagBits]='\0';
		int i, k=0, decTag;
		for (i=0; i<ktm->numTagBits; i++){
			tag[k]=binStringAddr[i];
			k++;
		}
		//printf("BinTagA: %s\n", tag);
		decTag=binToDec(tag, numTBits);
		char ind[ktm->numIndBits+1];
		ind[ktm->numIndBits]='\0';
		int decInd;
		k=0;
		for (i=ktm->numTagBits; i<ktm->numTagBits+ktm->numIndBits; i++){
			ind[k]=binStringAddr[i];
			k++;
		}
		//printf("BinIndA: %s\n", ind);
		decInd=binToDec(ind, numIBits);
	
		
		
		int bdecTag, bdecInd;
		char bInd[ktm->numIndBits+1];
		char bTag[ktm->numTagBits+1];
		bInd[ktm->numIndBits]='\0';
		bTag[ktm->numTagBits]='\0';
		for(i=0; i<ktm->numIndBits; i++){
			bInd[i]=binStringAddr[i];
		}
		k=0;
		for(i=ktm->numIndBits; i<ktm->numIndBits+ktm->numTagBits; i++){
			bTag[k]=binStringAddr[i];
			k++;
		}
		//printf("BinTagB: %s\n", bTag);
		//printf("BinIndB: %s\n", bInd);
		bdecTag=binToDec(bTag, numTBits);
		bdecInd=binToDec(bInd, numIBits);

		
		
		setupTwoBlock(decTag, decInd, rw);
		bsetupTwoBlock(bdecTag, bdecInd, rw);
	}
	else{
		int numTFBits=mvagusta->numTagBits;
		char tag[mvagusta->numTagBits];
		int i, k=0, decTag;
		for (i=0; i<mvagusta->numTagBits; i++){
			tag[k]=binStringAddr[i];
			k++;
		}
		decTag=binToDec(tag, numTFBits);
		setupFullBlock(decTag, rw);
	}
}

void getAddrInfo(int cachesize, int blocksize, int numberofsets){
	if(z==1){
		int numBlockBits;
		ktm=malloc(sizeof(struct twoaddress));
		numBlockBits=logBits(blocksize);
		ktm->numIndBits=logBits(numberofsets);
		ktm->numTagBits=48-ktm->numIndBits-numBlockBits;
	}
	else{
		int numBlockBits;
		mvagusta=malloc(sizeof(struct fulladdress));
		numBlockBits=logBits(blocksize);
		mvagusta->numTagBits=48-numBlockBits;
	}
}

int logBits(int n){
	int i=0;
	while (n!=1){
		n=n/2;
		i++;
	}
	return i;
}

char* hexToBin(char *address){
	int i;
	int numSpace=strlen(address)-2;
	int numSpace1=4*numSpace;
	int finalSpace=numSpace1+1;
	char *res=malloc(sizeof(char)*finalSpace);
	char *bits;
	for (i=2; i<strlen(address); i++){
		//char *bits;
		if (address[i]=='0'){
			bits="0000";
			strcat(res, bits);
			//free(bits);
			continue;
		}
		else if (address[i]=='1'){
			bits="0001";
			strcat(res, bits);
			//free(bits);
			continue;
		}
		else if (address[i]=='2'){
			bits="0010";
			strcat(res, bits);
			//free(bits);
			continue;
		}
		else if (address[i]=='3'){
			bits="0011";
			strcat(res, bits);
			//free(bits);
			continue;
		}
		else if (address[i]=='4'){
			bits="0100";
			strcat(res, bits);
			//free(bits);
			continue;
		}
		else if (address[i]=='5'){
			bits="0101";
			strcat(res, bits);
			//free(bits);
			continue;
		}
		else if (address[i]=='6'){
			bits="0110";
			strcat(res, bits);
			//free(bits);
			continue;
		}
		else if (address[i]=='7'){
			bits="0111";
			strcat(res, bits);
			//free(bits);
			continue;
		}
		else if (address[i]=='8'){
			bits="1000";
			strcat(res, bits);
			//free(bits);
			continue;
		}
		else if (address[i]=='9'){
			bits="1001";
			strcat(res, bits);
			//free(bits);
			continue;
		}
		else if (address[i]=='a'){
			bits="1010";
			strcat(res, bits);
			//free(bits);
			continue;
		}
		else if (address[i]=='b'){
			bits="1011";
			strcat(res, bits);
			//free(bits);
			continue;
		}
		else if (address[i]=='c'){
			bits="1100";
			strcat(res, bits);
			//free(bits);
			continue;
		}
		else if (address[i]=='d'){
			bits="1101";
			strcat(res, bits);
			//free(bits);
			continue;
		}
		else if (address[i]=='e'){
			bits="1110";
			strcat(res, bits);
			//free(bits);
			continue;
		}
		else if (address[i]=='f'){
			bits="1111";
			strcat(res, bits);
			//free(bits);
			continue;
		}
	}
	bZero=48-strlen(res);
	char zeroAddr[bZero];
	i=0;
	while(i<bZero){
		zeroAddr[i]='0';
		i++;
	}
	int m=0;
	char *realAddr=malloc(sizeof(char)*49);
	realAddr[48]='\0';
	for(i=0;i<bZero;i++){
		realAddr[m]=zeroAddr[i];
		m++;
	}
	for(i=0; i<strlen(res);i++){
		realAddr[m]=res[i];
		m++;
	}
	//printf("BinADDR: %s\n", realAddr);
	return realAddr;
}

int main(int argc, char *argv[]){
	if(argc!=5){
		printf("ERROR:  Not enough command line inputs");
		exit(0);
	}
	else{
		int cSize=atoi(argv[1]);
		if(!isPowerOfTwo(cSize)||cSize==0){
			printf("ERROR:  Invalid cache size");
			exit(0);
		}
		int bSize=atoi(argv[3]);
		if(!isPowerOfTwo(bSize)||cSize==0){
			printf("ERROR:  Invalid block size");
			exit(0);
		}
		char *assoc=argv[2];
		int numberSets, av;
		if(strlen(assoc)>=6){
			if(assoc[6]=='\0'){
				av=1;
			}
			else{
				int i, k=0;
				char assocValue[strlen(assoc)-5];
				for (i=6; i<strlen(assoc); i++){
				assocValue[k]=assoc[i];
				k++;
				}
				av=atoi(assocValue);
			}
			numberSets=cSize/(av*bSize);
			ducati=malloc(sizeof(struct twoCache));
			ducati->assoc=av;
			ducati->set=malloc(sizeof(struct twoCacheBlock*)*numberSets);//star here
			ducati->numSets=numberSets;
			ducatiB=malloc(sizeof(struct twoCache));
			ducatiB->assoc=av;
			ducatiB->set=malloc(sizeof(struct twoCacheBlock*)*numberSets);//star here
			ducatiB->numSets=numberSets;
			z=1;
			int j;
			for(j=0; j<numberSets; j++){
				ducati->set[j]=malloc(sizeof(struct twoCacheBlock));
				ducati->set[j]=NULL;
				ducatiB->set[j]=malloc(sizeof(struct twoCacheBlock));
				ducatiB->set[j]=NULL;
				}
			}
		else{
			int numberBlocks=cSize/bSize;
			aprilia=malloc(sizeof(struct fullCache));
			aprilia->set=malloc(sizeof(struct fullCacheBlock*)*numberBlocks);//star here
			aprilia->maxBlock=numberBlocks;
			int j;
			for (j=0; j<numberBlocks; j++){
				aprilia->set[j]=malloc(sizeof(struct fullCacheBlock));
			}
		}
		getAddrInfo(cSize, bSize, numberSets);
		FILE *tracefile=fopen(argv[4], "r");
		readAddress(tracefile);
		printResults();
		fclose(tracefile);
		free(ducati);
		free(ducatiB);
		free(aprilia);
		free(mvagusta);
		free(ktm);
	}
	return 0;
}

int isPowerOfTwo(int n){
  if (n == 0){
    return 0;
 }
  while (n != 1){
    if (n%2 != 0){
      return 0;
   	}
    n = n/2;;
  }
  return 1;
}
