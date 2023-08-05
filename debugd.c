#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXARG 30
#define STKSIZE 10

enum commands {
	NAC, ADD, SUB, MUL, MOD, RDV, END, EXIT, NL
		//NAC not a command
		//END run
};

int *argstack;//init in section A
int *comstack;

int getop(char s[], int size);
void push(int * mem, int *ptr,int a);
int pop(int * mem,int *ptr);
void dumpstack();

int main()
{
	//section A
	argstack = (int *) malloc(STKSIZE*sizeof(int));
	comstack = (int *) malloc(STKSIZE*sizeof(int));
	int argptr=0;
	int comptr=0;
	//sec B
	int words;
	int numval, argval;
	char string[MAXARG];
	//for loop, as long as not exit	
	for(;;) 
	{
		for(words=1;scanf("%s",string);words++ )
		{
			//op exp exp
			//=  sym exp
			numval=atoi(string);
			argval=getop(string,strlen(string));
			if (argval==END)	break;
			if (numval==0)	{
				if (argval==NAC)	{
					push(argstack,&argptr,numval);
				}
				else if (argval==NL)	{
					break;
				}
				else if (argval==END)	{
					break;
				}
				else if (argval==EXIT)	{
					return 1;
				}
				else if (argval!=NAC)	{
                                	push(comstack,&comptr,argval);
				}
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
			arg2 = pop(argstack, &argptr);
			//switch
			switch (command)	
			{
				case NAC:	{
                                        break;
				}
				case ADD:	{
					result=arg1+arg2;
					break;
				}
				case SUB:	{
					result=arg2-arg1;
					break;
				}
				case MUL:	{
					result=arg1*arg2;
					break;
				}
				case MOD:	{
					result= arg2%arg1;
					break;
				}
				case RDV:	{
					result= arg2/arg1;
					break;
				}
				default: {
					break;
				}
			}
			if (command==NAC || (argptr==1 && comptr==0))  {
                                printf("\n<\t%d\n\n",result);
                                break;
                        }
                        else    {
                                push(argstack, &argptr, result);
                        }
		}
		//
	}
	return 0;
}

int getop(char s[], int size)
{
	if (size==1)	{
		if (!strcmp("+",s))	{
			return ADD;
		}
		else if (!strcmp("\n",s))	{
			return NL;
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
