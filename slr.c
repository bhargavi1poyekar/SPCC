/*

Name: Bhargavi S. Poyekar
Class: TE COMPS
Batch: C
UID: 2018130040
Roll No. 45
Exp No. 6c  - To implement different types of Parsers for the given grammar.
(Bottom-up SLR parser)

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
char parseTabAction[20][20][20]={
	{"  ","  ","s4","s5","  "},
	{"s6","  ","  ","  ","a"},
	{"r2","  ","s4","s5","r2"},
	{"r4","s8","r4","r4","r4"},
	{"r6","r6","r6","r6","r6"},
	{"r7","r7","r7","r7","r7"},
	{"  ","  ","s4","s5","  "},
	{"r3","s8","r3","r3","r3"},
	{"r5","r5","r5","r5","r5"},
	{"r1","  ","s4","s5","r1"},
};  //array for parse table Action
int parseTabGoTo[20][20]={
	{1,2,3},
	{0,0,0},
	{0,0,7},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,9,3},
	{0,0,0},
	{0,0,0},
	{0,0,7}
};//array for parse Table GoTo

char inpbuffer[20];	//input string buffer
int buffidx=0;		//input buffer index
char stack[50];	//stack
int stacktop=-1;	//stack top index


//functions
void input();		//terminal input
void fileInput();	//file input
int isStackEmpty();	//to check if stack is empty
char pop();		//to pop a char from stack
void push(char data);	//to push a char in stack
void printStack();	//to print the content of stack
void printInpBuffer();	//Print Input Buffer
int isnonterm(char c); //check if character is non terminal  
int parse(); 		//parsing function

int main()
{
    printf("Enter your input choice: \n1.Terminal Input \n2.File Input\n");
    scanf("%d", &inp);
    
    switch(inp)
    {
    	case 1:input();//terminal input
    		break;
    	case 2:fileInput();//file input
    		break;
    }
   
    push('$');//pushing $ in stack
    push('0');//pushing start symbol in stack
    
    printf("\nStack\t\tInput Buffer\tAction\n");//printing stack and input buffer
    printStack();
    printInpBuffer();
    int result=parse();//parsing the input string
    
    
    if(result>0)
    {//if parsing successfull
    	printf("\nThe parser has accepted the input string\n");
    }
    
    return 0;
}

void fileInput(){
    
    FILE *fp=fopen("input.txt","r");//opening file
    
    fscanf(fp,"%d",&term);//no. of terminal
    fgetc(fp);
    
    for(int i=0; i<term; i++)
    {
        fscanf(fp,"%s", &terms[i]);//input of terminal symbols
        fgetc(fp);
    }
    terms[term]='$';

    fscanf (fp, "%d", &nonterm);//no. of non terminals
    fgetc(fp);

    for(int i=0; i<nonterm; i++)
    {
        fscanf(fp,"%s", &nonterms[i]);//input of nonterminal symbols
        fgetc(fp);
    }

    fscanf (fp, "%d", &numprod);//no. of productions
    fgetc(fp);

    for(int i=0; i<numprod; i++)
    {
        fscanf(fp, "%s", prods[i]);
        char left[3];
        int k=0;
        //separating all the productions 
        for(int j=0;j<3;j++)
        {
        	left[j]=prods[i][j];
        	leftprod[idx][k++]=prods[i][j];
        }
        
        for(int j=3;j<strlen(prods[i]);j++)
        {
        	if(prods[i][j]!='|')
        	{
        	//if | is present separate the production
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
    fscanf(fp,"%s", &start);//input of start symbol
    fgetc(fp);
    
    fscanf(fp,"%s", inpbuffer);//input string
    fgetc(fp);
    
    char end=inpbuffer[strlen(inpbuffer)-1];//checking if $ present at end 							of string
    
    if(end!='$')
    {
    //if $ not present add $ at end of string
    	strcat(inpbuffer,"$");
    }
    printf("\n");
    
    
}

void input()
{
    printf("Enter the number of terminals: ");
    scanf("%d", &term);
    //terminal symbols input
    for(int i=0; i<term; i++)
    {
        printf("\nEnter %d th terminal: ", i+1);
        scanf(" %c", &terms[i]);
        getchar();
    }
    terms[term]='$';
	//non terminal symbols input
    printf("\nEnter the number of non terminals: ");
    scanf("%d", &nonterm);
    for(int i=0; i<nonterm; i++)
    {
        printf("\nEnter %d th non terminal: ", i+1);
        scanf(" %c", &nonterms[i]);
        getchar();
    }
	//productions input
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
        //separate the productions 
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
    //input of start symbol
    printf("\nEnter the start symbol: ");
    scanf(" %c", &start);
    getchar();
    
	//input string    
    printf("\nEnter the input String: ");
    scanf(" %s", inpbuffer);
    char end=inpbuffer[strlen(inpbuffer)];
    
    if(end!='$')//adding $ at the end of string if not present
    {
    	strcat(inpbuffer,"$");
    }
    printf("\n");
}

int isnonterm(char c)//check if character is non terminal
{
	for(int i=0;i<nonterm;i++)
	{
		if(c==nonterms[i])
		{
			return 1;//if non term return 1
		}
	}
	return 0;//if not a non term return 0
}

int parse()
{
	while(1)  
	{	//if stacktop is non terminal
		if(isnonterm(stack[stacktop])==1)
		{
			//prev character in stack
			int prev=stack[stacktop-1]-'0';
			int ntidx;//to get the index of non terminal
			int f=0; //flag
			for(int i=0;i<=nonterm;i++)
			{	//finding index of non terminal
				if(stack[stacktop]==nonterms[i])
				{
					ntidx=i;
					f=1;//if found flag=1
				}
			}
			if(f!=1)//if flag is not 1, then non terminal not 					found
			{
				
				printf("\nSyntax error, string cannot be parsed");//print error
				return 0; //end while
			}
			
			// find GOTO value for corresponding state and non 				term
			
			if(parseTabGoTo[prev][ntidx]!=0)
			{	
				printf("%d",parseTabGoTo[prev][ntidx]);
				//push the goto value in stack
				push(parseTabGoTo[prev][ntidx]+'0');
				//print output
				printf("\n");
				printStack();
	    			printInpBuffer();
    			}
    			else  //if no goto value found
    			{
    				printf("\nSyntax error, string cannot be parsed");
				return 0;
    			}
			
		}
		else 	//if stacktop is number
		{
			int num = stack[stacktop]-'0'; //number at stacktop
			int tidx; //terminal index
			int f=0; //flag
			for(int i=0;i<=term;i++)
			{	//finding index of terminal
				if(inpbuffer[buffidx]==terms[i])
				{
					tidx=i;
					f=1;//if found, flag=1
				}
			}
			if(f!=1) //if flag not 1 then error
			{
				printf("\nSyntax error, string cannot be parsed");		//print error
				return 0; //end while loop and return to 						main function
			}
			
			char action[10]; //store the action 
			strcpy(action,parseTabAction[num][tidx]);//copy 									action
			printf("%s",action); //print action
			
			if(action[0]=='s') //if action is shift
			{
				printf(" Shift %c ", inpbuffer[buffidx]);
				//push input character to stack
				push(inpbuffer[buffidx]);
				buffidx++;
				push(action[1]);//push action number to 							stack
				
				//print output
				printf("\n");
				printStack();
    				printInpBuffer();
				
			}
			else if(action[0]=='r') //if action is reduce
			{
				char product[10]; //product string
				strcpy(product,leftprod[action[1]-'0']);
				//copy product string
				printf(" Reduce %s",product);
				int len=strlen(product)-3;
				//pop the right side of production
				for(int i=0;i<len;i++)
				{
					pop();
					pop();
				}
				//push left side of production
				push(product[0]);
				//print output
				printf("\n");
				printStack();
    				printInpBuffer();
				
			}
			else if(action[0]=='a') //accept the string
			{
				//print accept and end while loop
				printf(" Accept");
				return 1;
			}
			else
			{
				//if no value in action table
				printf("\nSyntax error, string cannot be parsed");
				return 0;
			}
			
			
		}
	}
}


int isStackEmpty()
{//return 1 if stack is empty
	if(stacktop==0)
		return 1;
	else
		return 0;
}

char pop()
{
	char data;
	char emp='N';
	if(!isStackEmpty())//if stack not empty pop the stack top 
	{
		data=stack[stacktop];
		stacktop-=1;
		return data;
	}
	else
	{
		printf("Stack is empty\n");
		return emp;//if stack is empty return N
	}
}

void push(char data)
{
	stacktop+=1;
	stack[stacktop]=data;
	//push data in stack
}

void printStack()
{
	//print stack
	for (int i=0;i<=stacktop;i++)
	{
		printf("%c",stack[i]);
	}
	if(stacktop>=7)
	{
		printf("\t");
	}
	else
	printf("\t\t");
}

void printInpBuffer()
{
	//print buffer
	for (int i=buffidx;i<strlen(inpbuffer);i++)
	{
		printf("%c",inpbuffer[i]);
	}
	printf("\t\t");
}



