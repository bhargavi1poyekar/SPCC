/*

Name: Bhargavi S. Poyekar
Class: TE COMPS
Batch: C
UID: 2018130040
Roll No. 45
Exp No. 6a  - To implement different types of Parsers for the given grammar.
(Non-recursive Predictive Parser/ LL(1) parser)

*/

#include<stdio.h>
#include<stdlib.h>
# include<string.h>
#include<ctype.h>

//variables
int inp;		//input choice
int term;		//no.of terminals
char terms[100];	//array of terminals
int nonterm;		//no.of non terminals
char nonterms[100];	//array of non terminals
int numprod;		//no. of productions
char prods[50][20];	//2d array for productions
char leftprod[50][20];
int idx=0;
char start;		//starting non terminal symbol
FILE *fp;		//file pointer
int parseTab[20][20];  //array for parse table
char inpbuffer[20];	//input string buffer
int buffidx=0;		//input buffer index
char stack[50];	//stack
int stacktop=-1;	//stack top index
int usedprods[20];	//array to store productions used for parsing
int usedprodidx=0;	//index od used productions

//functions
void input();		//terminal input
void fileInput();	//file input
int isStackEmpty();	//to check if stack is empty
char pop();		//to pop a char from stack
void push(char data);	//to push a char in stack
void printStack();	//to print the content of stack
void printInpBuffer();	//Print Input Buffer
void addUsedProds(int prodno);	
//to add the productions used for parsing in array

int parse(); 

int main()
{
    printf("Enter your input choice: \n1.Terminal Input \n2.File Input\n");
    scanf("%d", &inp);
    
    switch(inp)
    {
    	case 1:input();
    		break;
    	case 2:fileInput();
    		break;
    }
    
    push('$');
    push(start);
    printInpBuffer();
    printStack();
    int result=parse();
    printInpBuffer();
    printStack();
    
    if(result>0)
    {
    	printf("\nThe parser has accepted the input string\n");
    }
    
    printf("\nProductions used in parsing the string are: ");
    for(int i=0;i<=usedprodidx;i++)
    {
    	printf("\n%s",leftprod[usedprods[i]-1]);
    }
    return 0;
}

void fileInput(){
    
    FILE *fp=fopen("input2.txt","r");
    
    fscanf(fp,"%d",&term);
    fgetc(fp);
    
    for(int i=0; i<term; i++)
    {
        fscanf(fp,"%s", &terms[i]);
        fgetc(fp);
    }
    

    fscanf (fp, "%d", &nonterm);
    fgetc(fp);

    for(int i=0; i<nonterm; i++)
    {
        fscanf(fp,"%s", &nonterms[i]);
        fgetc(fp);
    }

    fscanf (fp, "%d", &numprod);
    fgetc(fp);

    for(int i=0; i<numprod; i++)
    {
        fscanf(fp, "%s", prods[i]);
        char left[3];
        int k=0;
        for(int j=0;j<3;j++)
        {
        	left[j]=prods[i][j];
        	leftprod[idx][k++]=prods[i][j];
        }
        
        for(int j=3;j<strlen(prods[i]);j++)
        {
        	if(prods[i][j]!='|')
        	{
        		leftprod[idx][k++]=prods[i][j];
        		
        	}
        	else
        	{
        		
        		idx++;
        		k=0;
        		while(k<3)
        		{
        			leftprod[idx][k]=left[k];
        			k++;
        		}
        	}
        	
        }
        idx++;
        
    }
    
    fgetc(fp);
    fscanf(fp, "%s", &start);
    fgetc(fp);
    for(int i=0; i<nonterm; i++)
    {
    	for(int j=0;j<=term;j++)
    	{
        	fscanf(fp,"%d", &parseTab[i][j]);
        	fgetc(fp);
        }
    }
    
    fscanf(fp,"%s", inpbuffer);
    fgetc(fp);
    
    char end=inpbuffer[strlen(inpbuffer)];
    
    if(end!='$')
    {
    	strcat(inpbuffer,"$");
    }
    printf("\n");
    
    
}

void input()
{
    printf("Enter the number of terminals: ");
    scanf("%d", &term);
    
    for(int i=0; i<term; i++)
    {
        printf("\nEnter %d th terminal: ", i+1);
        scanf(" %c", &terms[i]);
        getchar();
    }

    printf("\nEnter the number of non terminals: ");
    scanf("%d", &nonterm);
    for(int i=0; i<nonterm; i++)
    {
        printf("\nEnter %d th non terminal: ", i+1);
        scanf(" %c", &nonterms[i]);
        getchar();
    }

    printf("\nEnter the number of productions: ");
    scanf("%d", &numprod);
    printf("\nEnter the grammar as E->E-A :\n");
    
    for(int i=0; i<numprod; i++)
    {
        printf("\nEnter the %d th production: ", i+1);
        scanf("%s", prods[i]);
        
        char left[3];
        int k=0;
        for(int j=0;j<3;j++)
        {
        	left[j]=prods[i][j];
        	leftprod[idx][k++]=prods[i][j];
        }
        
        for(int j=3;j<strlen(prods[i]);j++)
        {
        	if(prods[i][j]!='|')
        	{
        		leftprod[idx][k++]=prods[i][j];
        		
        	}
        	else
        	{
        		
        		idx++;
        		k=0;
        		while(k<3)
        		{
        			leftprod[idx][k]=left[k];
        			k++;
        		}
        	}
        	
        }
        idx++;
        
    }
    for(int i=0;i<=idx;i++)
    {
    	printf("%s\n",leftprod[i]);
    }
    printf("\nEnter the start symbol: ");
    scanf(" %c", &start);
    getchar();
    
    printf("Parse Table\n");
    for(int i=0;i<nonterm;i++)
    {
    	for(int j=0;j<=term;j++)
    	{
    		printf("Enter value for Parse Table[%d][%d]: ",i,j);
    		scanf("%d",&parseTab[i][j]);
    	}
    }
    
    for(int i=0;i<nonterm;i++)
    {
    	for(int j=0;j<=term;j++)
    	{
    		printf(" %d",parseTab[i][j]);
    		
    	}
    	printf("\n");
    }
    
    printf("\nEnter the input String: ");
    scanf(" %s", inpbuffer);
    char end=inpbuffer[strlen(inpbuffer)];
    
    if(end!='$')
    {
    	strcat(inpbuffer,"$");
    }
    printf("\n");
}

int parse()
{
	while(inpbuffer[buffidx]!='$' && stack[stacktop]!='$')
	{
		if(stack[stacktop]!=inpbuffer[buffidx])
		{
			int ntidx,tidx;
			
			for(int i=0; i<term;i++)
			{
				if(terms[i]==inpbuffer[buffidx])
				{
					tidx=i;
					break;
				}
			}
			
			for(int i=0; i<nonterm;i++)
			{
				if(nonterms[i]==stack[stacktop])
				{
					ntidx=i;
					break;
				}
			}
			
			int prodno=parseTab[ntidx][tidx];
			if(prodno==0)
			{
				printInpBuffer();
				printStack();
				printf("\nSyntax error, string will not be accepted");
				return 0;
			}
			
			printf("\nProduction: %s\n",leftprod[prodno-1]);
			pop();
			int n=strlen(leftprod[prodno-1])-1;
			for(int i=n;i>2;i--)
			{
				push(leftprod[prodno-1][i]);
			}
			addUsedProds(prodno);
			printInpBuffer();
			printStack();
			
		}
		else
		{
			pop();
			buffidx++;
		}
	}	
	
	if(inpbuffer[buffidx]!='$' || stack[stacktop]!='$')
	{
		printf("\nThe string is not accepted");
		return 0;
	}
}


int isStackEmpty()
{
	if(stacktop==0)
		return 1;
	else
		return 0;
}

char pop()
{
	char data;
	char emp='N';
	if(!isStackEmpty())
	{
		data=stack[stacktop];
		stacktop-=1;
		return data;
	}
	else
	{
		printf("Stack is empty\n");
		return emp;
	}
}

void push(char data)
{
	stacktop+=1;
	stack[stacktop]=data;
	
}

void printStack()
{
	printf("\nStack\n");
	for (int i=0;i<=stacktop;i++)
	{
		printf("%c",stack[i]);
	}
	printf("\n");
}

void printInpBuffer()
{
	printf("\nInput Buffer\n");
	for (int i=buffidx;i<strlen(inpbuffer);i++)
	{
		printf("%c",inpbuffer[i]);
	}
	printf("\n");
}

void addUsedProds(int prodno)
{
	int f=0;
	for(int i=0;i<=usedprodidx;i++)
	{
		if(prodno==usedprods[i])
		{
			f=1;
			break;
		}
	}
	
	if(f==0)
	{
		usedprods[usedprodidx++]=prodno;
	}
	
}
