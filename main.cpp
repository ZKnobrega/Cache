#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;
 
#define STREAM 				(0)		// 0 - Print to Screen, 1 - Write to cache.out.
#define MAIN_MEMORY			(1024)	// 1024 Addressable memory.
#define CACHE_LINE 			(10)	// 10 cache lines.
#define CACHE_BLOCK_SIZE	(20)		// 8 words in each cache lines.
 
int cache_tag[CACHE_LINE];			// cacahe tag, if(cache_tag[i] == 0) its a MISS.
 
int total_memory_access = 0;		// quantidade de acesso à memória ou solicitação de endereço da CPU.
int total_hit = 0;					// amount of cache hit.
 
void print_hit_ratio(void);
void write_cache_table(void);
int usage(void);
 
 
// print the hit ratio.
void print_hit_ratio(void)
{
	float ratio = 0.0;
 
	if(total_hit == 0)
		ratio = 0.0;
	else
		ratio = (float)total_hit / (float)total_memory_access;
		
	//get percentage
	ratio *= 100;
	cout <<"Porcentagem acerto: " << ratio << "%" << endl;
	cout << "Hit : " << total_hit << endl;
	cout << "Miss: " << total_memory_access - total_hit << endl;
}
 
 
//Print the content of cache table
void write_cache_table(void)
{
	int i, j;
	FILE *ofp;						//Output file pointer
 
	if(STREAM)
		ofp = fopen("cache.out", "w");
	else
		ofp = stdout;
 
	//print table header
	fprintf(ofp, "%6s|", "Lines ");
 
	for(i = 0; i < CACHE_BLOCK_SIZE; i++)
	{
		fprintf(ofp, "%6d|", i);
	}
	fprintf(ofp, "\n");
 
	for(i = -1; i < CACHE_BLOCK_SIZE; i++)
	{
		fprintf(ofp, "-----");
	}
	fprintf(ofp, "\n");
 
	//loop each cache line
	for(i = 0; i < CACHE_BLOCK_SIZE; i++)
	{
		fprintf(ofp, "%6d|", i);
 
		for(j = 0; i < CACHE_BLOCK_SIZE; i++)
		{
			//no content cache
			if(cache_tag[i] == 0)
				fprintf(ofp, "%6d|", 0);
			else
				fprintf(ofp, "%6d|", cache_tag[i] + j);
		}
		fprintf(ofp, "\n");
	}
 
	if(STREAM)
		fclose(ofp);
}
 
 
int usage(void)
{
	cout <<("Please pass a file\n");
	cout <<("Usage: cachesim <file>\n");
 
	return -1;
}
 
 
int main(int argc, char *argv[])
{
	FILE *ifp;						//input file
	int address_requested = 0;		//address requested
 
	int i;
	int lastwrote = -1;
 
	if(argc != 2)
	{
		exit(usage());
	}
 
	//Read input file from command line.
	ifp = fopen(argv[1], "r");
 
	if(ifp == NULL)
	{
		cout <<"ERROR: File %s does not exist\n", argv[1];
		exit(usage());
	}
 
	/* ************************************************
						START CODE
	************************************************ */
	for(i = 0; i < CACHE_LINE; i++)
	{
		cache_tag[i] = 0;
	}
 
	//Read input file line by line and store CPU request in variable 'address_requested'
	while(fscanf(ifp, "%d\n", &address_requested) != EOF)
	{
		int done = 0;
 
		for(i = 0; i < CACHE_LINE; i++)
		{
			if((address_requested >= cache_tag[i]) && ((address_requested - cache_tag[i]) < CACHE_BLOCK_SIZE))
			{
				total_hit++;
				done = 1;
				break;
			}
		}
 
		if(done == 0)
		{
			lastwrote = (++lastwrote) % CACHE_LINE;
			cache_tag[lastwrote] = address_requested;
		}
 
		total_memory_access++;
	}
	/* ************************************************
						END CODE
	************************************************ */
 
	print_hit_ratio();
	write_cache_table();
 
	fclose(ifp);
}
