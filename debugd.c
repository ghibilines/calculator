#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vars.h"
#define MAXARG 30
#define STKSIZE 10

enum commands {
	NAC,ZERO, ADD, SUB, MUL, MOD, RDV, ANS, END, EXIT, DEC
		//NAC not a command
		//END run
		//DEC declaration of var
};

int *argstack;//init in section A
int *comstack;
int *usestack;//for variables in use;
SYMBOL *symstack;

int getop(char s[], int size);
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
	int argptr=0;
	int comptr=0;
	int useptr=0;
	int symptr=0;
	//init symstack
	for (int i=0;i<STKSIZE;i++)	{
		(*(argstack+i))=0;
		(*(comstack+i))=0;
		(*(usestack+i))=0;
	}
	for (int i=0;i<SYMSIZE;i++)	{
		(symstack+i)->identity = 0;
		(symstack+i)->value = 0;
	}
	//sec B
	SYMBOL *tempa;
	int words, temp;
	int numval, argval;
	char string[MAXARG];
	int answer=0;
	int declaration=0;
	int symid, symval;
	//for loop, as long as not exit	
	for(;;) 
	{
		for(words=1;scanf("%s",string);words++ )
		{
			numval=atoi(string);
			argval=getop(string,strlen(string));
			if (argval==END)	break;
			if (numval==0)	{
				if (argval==ZERO)	{
					push(argstack,&argptr,numval);
				}
				else if (argval==END)	{
					break;
				}
				else if (argval==EXIT)	{
					free(argstack);
					free(comstack);
					free(usestack);
					free(symstack);
					return 0;
				}
				else if (argval==ANS)	{
					push(argstack,&argptr,answer);
				}
				else if (argval==DEC)	{
					//declaring variable
					printf("[i] declare variable\n");
					declaration=2;
					push(comstack,&comptr,argval);
				}
				else if (argval!=NAC)	{
                                	push(comstack,&comptr,argval);
				}//command
				else if (argval==NAC)	{
					if (declaration==2)	{
						printf("[i] got var identifier\n");
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
				}//variable
				else	{
					printf("[!] not a command or variable\n");
				}//syntaxerror
			}
			else if (numval!=0 && argval==NAC)	{
				push(argstack,&argptr,numval);
			}
			else	{
				printf("[!] not a command or argument\n");
			}

		}
		//for loop interpreting stack
		int command;
		int arg1, arg2;
		int result;
		for (;;)
		{	//common section
			command = pop(comstack, &comptr);
			arg1 = pop(argstack, &argptr);
			//arg2 = pop(argstack, &argptr);
			//switch
			switch (command)	
			{
				case NAC:	break;
				case DEC:	{
					//one is on use stack
					printf("[i] identified as declaration\n");
					symid = pop(usestack,&useptr);
					tempa = symcreate(symstack,&symptr,symid,arg1);
					break;
				}
				case ADD:	{
					arg2 = pop(argstack, &argptr);
					result=arg1+arg2;
					break;
				}
				case SUB:	{
					arg2 = pop(argstack, &argptr);
					result=arg2-arg1;
					break;
				}
				case MUL:	{
					arg2 = pop(argstack, &argptr);
					result=arg1*arg2;
					break;
				}
				case MOD:	{
					arg2 = pop(argstack, &argptr);
					result= arg1%arg2;
					break;
				}
				case RDV:	{
					arg2 = pop(argstack, &argptr);
					result= arg1/arg2;
					break;
				}
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

int getop(char s[], int size)
{
	if (!strcmp("0",s))	{
		printf("[getop] I: ZERO detected\n");
		return ZERO;
	}
	if (size==1)	{
		if (!strcmp("+",s))	{
			return ADD;
		}
		else if (!strcmp(";",s))	{
			return END;
		}
		else if (!strcmp("-",s))	{
			return SUB;
		}
		else if (!strcmp("*",s))	{
			return MUL;
		}
		else if (!strcmp("%",s))	{
			return MOD;
		}
		else if (!strcmp("/",s))	{
			return RDV;
		}
		else if (!strcmp("=",s))	{
			printf("[getop] declaration detected");
			return DEC;
		}
		else	{
			return NAC;
		}
	}
	else if (size==2)	{
		if (!strcmp("-",s))        {
                        return SUB;
                }
		else {
			return NAC;
		}
	}
	else if (size==3)	{
		if (!strcmp("END",s))	{
			return END;
		}
		else if (!strcmp("ans",s))	{
			return ANS;
		}
		else    {
                        return NAC;
                }
	}
	else if (size==4)	{
		if (!strcmp("EXIT",s))	{
			return EXIT;
		}
		else    {
                        return NAC;
                }
	}
	printf("getop: size is %d\n", size);
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
