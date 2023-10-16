#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vars.h"
#define MAXARG 30
#define STKSIZE 10

enum commands {
	NAC,ZERO, AND, OR, ADD, SUB, MUL, MOD, RDV, ANS, END, EXIT, DEC
		//NAC not a command
		//END run
		//DEC declaration of var
};

int *argstack;//init in section A
int *comstack;
int *usestack;//for variables in use;
SYMBOL *symstack;

int getop(int s);
void push(int * mem, int *ptr,int a);
int pop(int * mem,int *ptr);
void dumpstack();
int strid(char *s);

int main()
{
	//section A init stacks
	argstack = (int *) malloc(STKSIZE*sizeof(int));
	comstack = (int *) malloc(STKSIZE*sizeof(int));
	usestack = (int *) malloc(STKSIZE*sizeof(int));
	symstack = (SYMBOL*) malloc(SYMSIZE*sizeof(SYMBOL));
	//init stacks
	bzero(argstack,STKSIZE);
	bzero(comstack,STKSIZE);
	bzero(usestack,STKSIZE);
	bzero(symstack,SYMSIZE);
	int argptr=0;
	int comptr=0;
	int useptr=0;
	int symptr=0;
	//sec B
	//for loop, as long as not exit	
	
	//var interpret loop
	SYMBOL *tempa;
        int symid, symval;
        int command;
        int arg1, arg2;
        int answer=0;
        int declaration=0;
        int result;
	for(;;) 
	{
		printf("user>\t");
		int words, temp;
		int numval, argval;
		char string[MAXARG];
		for(words=1;scanf("%s",string);words++ )
		{
			numval=atoi(string);
			argval=getop(strid(string));
			if (argval==END)	break;
			if (numval!=0 && argval==NAC)      {
                                push(argstack,&argptr,numval);
                        }
			else if (numval==0)	{
				switch (argval)	{
					case ZERO:
						push(argstack,&argptr,numval);
						break;
					case EXIT:
						free(argstack);
						free(comstack);
						free(usestack);
						free(symstack);
						return 0;
					case ANS:
						push(argstack,&argptr,answer);
						break;
					case DEC:
						//declaring variable
						declaration=2;
						push(comstack,&comptr,argval);
						break;
					case AND:
					case OR:
					case ADD:
					case SUB: 
					case MUL: 
					case MOD: 
					case RDV:
                                		push(comstack,&comptr,argval);
						break;
					case NAC:
						if (declaration==2)	{
							symid = strid(string);
							push(usestack,&useptr,symid);
							//push onto immediate usestack
						}
						else if (declaration==0)	{//not a declaration
							tempa = symlookup(symstack,symptr,strid(string));
							if (tempa==NULL)	{
								printf("[!] no such variable\n");
								break;
							}//otherwise push
							push(argstack,&argptr,tempa->value);
						}
						declaration=0;
						break;
					default:
						printf("[!] not a command or variable\n");
						break;
				}
			}
			else	{
				printf("[!] not a command or argument\n");
			}

		}
		//for loop interpreting stack
		for (;;)
		{	//common section
			command = pop(comstack, &comptr);
			arg1 = pop(argstack, &argptr);
			arg2 = pop(argstack, &argptr);
			//switch
			switch (command)	
			{
				case NAC:	break;
				case DEC:	{
					//one is on use stack
					symid = pop(usestack,&useptr);
					tempa = symcreate(symstack,&symptr,symid,arg1);
					push(argstack,&argptr,arg2); //lame and bad but wtever
					result = arg1;
					break;
				}
				case AND:
					result=arg1&arg2;
					break;
				case OR:
					result=arg1|arg2;
					break;
				case ADD:
					result=arg1+arg2;
					break;
				case SUB:
					result=arg2-arg1;
					break;
				case MUL:
					result=arg1*arg2;
					break;
				case MOD:
					result= arg1%arg2;
					break;
				case RDV:
					result= arg1/arg2;
					break;
				default:	break;
			}
			if (command==NAC || (argptr==1 && comptr==0))  {
                                answer=result;
				printf("\n<\t%d\n\n",result);
                                break;
                        }
                        else    {
                                push(argstack, &argptr, result);
                        }
		}
		//
	}
	free(argstack);
	free(comstack);
	free(usestack);
	free(symstack);
	return 0;
}

int getop(int s)
{
/*	strid("0"),48
	strid("+"), 43
	strid(";"), 59
	strid("-"), 45
	strid("&"), 38
	strid("|"), 124
	strid("*"), 42
	strid("%"), 37
	strid("/"), 47
	strid("="), 61
	strid("ans"), 1462
	strid("exit") 4538
	strid("EXIT") 3258
	magick values*/
	switch (s)	{
		case 48:
			return ZERO;
		case 43:
			return ADD;
		case 59:
			return END;
		case 45:
			return SUB;
		case 38:
			return AND;
		case 124:
			return OR;
		case 42:
			return MUL;
		case 37:
			return MOD;
		case 47:
			return RDV;
		case 61:
			return DEC;
		case 1462:
			return ANS;
		case 4538:
		case 3258:
			return EXIT;
		default: return NAC;
	}
	return NAC;
}

void push(int * mem,int *ptr,int a)
{
	(*(mem+(*ptr) ))=a;
	(*ptr)++;
}

int pop(int * mem, int *ptr)
{
	if ((*ptr)!=0 ) (*ptr)--;
	int temp = (*(mem+(*ptr) ));
	(*(mem+(*ptr) ))=0;;
	return temp;
}

int strid(char * s)
{
        int temp=0, fac=1;
        int dist=3;
        int i,j;
        for (i=0;(*(s+i))!=0;i++, fac*=dist)    {
                j = (int) (*(s+i));//idiot
                                   //now it works
                temp += j *fac;
        }
        return temp;
}
