#include <stdio.h>
#define SYMSIZE 16
typedef struct symbol{
	int identity;
	int value;
}SYMBOL;

SYMBOL * symlookup(SYMBOL * mem, int ptr, int query);
SYMBOL * symcreate(SYMBOL * mem, int *ptr, int ident, int val);

SYMBOL * symlookup(SYMBOL * mem, int ptr, int query )
{   
    for(int i=0;i<ptr;i++)
    {
        if ((mem+i)->identity==query)   return (mem+i);
    }    
    return NULL;	
    /*int size=ptr/2;
	int step=size;
    int i=0;
    printf("[symlookup] I: pointer passed %d\n",ptr);
	for(;0<=size<ptr && i<=50;i++)	{
        if (step==0) step=1;
        printf("[symlookup] size:%d, step:%d\n",size,step);
		if (((mem+size)->identity)>query)	{
			step /= 2;
			size += step;
			printf("[symlookup] I: id %d, qu %d, loc %d,%d\n",(mem+size)->identity, query,size,step);
		}
		else if (((mem+size)->identity)<query)	{
			step /= 2;
			size -= step;
			printf("[symlookup] I: id %d, qu %d, loc %d,%d\n",(mem+size)->identity, query,size,step);
		}
		else if (((mem+size)->identity)==query)	{
			printf("[symlookup] I: id %d, qu %d, loc %d,%d\n",(mem+size)->identity, query,size,step);
			return (mem+size)->value;
		}
	}
	printf("[symlookup] E: no match was found %d %d", size, ptr);
	return 0;*/
}

SYMBOL * symcreate(SYMBOL * mem,int * ptr ,int ident, int val)
{
	(*ptr)++;
	if ((*ptr)+1>SYMSIZE)	{
		printf("[vars.h->symcreate] E: cannot create more than %d vars\n",SYMSIZE);
		return mem;
	}
	int i;
	for (i=0;ident<((mem+i)->identity);i++)	{
		printf("[vars.h->symcreate] I: found id bigger at %d: %d\n",i,(mem+i)->identity);
	} //kindof a trick
	printf("[vars.h->symcreate] I: found id smaller at %d: %d,%d\n",i,ident,(mem+i)->identity);
	if (0<=i<=(*ptr))	{
		if (i!=(*ptr))	{//shift upwards
			printf("[vars.h->symcreate] I: location is smaller than ptr, shifting up\n");
			int j;
			for(j=(*ptr)-1;j>=i;j--)	{
				(mem+j+1)->identity = (mem+j)->identity;
				(mem+j+1)->value = (mem+j)->value;
			}
		}
		//if ==ptr skips if statement
		(mem+i)->identity = ident;
		(mem+i)->value = val;
	}
	else	{//i>ptr something went wrong
		printf("[vars.h->symcreate] E: i is bigger than ptr %d,%d\n",i,(*ptr));
		//print err msg
		return NULL;
	}
	return (mem+i);//loc of inserted value
}
