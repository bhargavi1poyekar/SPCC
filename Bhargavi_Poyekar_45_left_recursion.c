/*

Name: Bhargavi S. Poyekar
Class: TE COMPS
Batch: C
UID: 2018130040
Roll No. 45
Experiment No. 5  - To remove left recursion from given grammar.

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
char start;		//starting non terminal symbol
FILE *fp;		//file pointer
char oriProds[50][20];	//array of original productions
int n_var=82;	//ascii value for new variable
char extraProd[50][20];//new Productions
char newVars[20];	//new variables
int extprodidx=0;	//number of new extra productions


//functions
void input();		//terminal input
void fileInput();	//file input
int searchLet(char *list, char letter);  //find existing nonterm
char newVar(); 		//generate new Variable
void elimDirRec(char temp[20],int i); //remove direct recursion
void elimLeftRec();	//removal of left recursion
void printGrammar(char *term, char *nonTerm, char prod[][20], int n, char st);
void printoutput();

int main()
{
    fp = fopen("input.txt", "r");
    printf("Enter your input choice: \n1.Terminal Input \n2.File Input\n");
    scanf("%d", &inp);
    
    switch(inp)
    {
    	case 1:input();
    		break;
    	case 2:fileInput();
    		break;
    }
    
    printf("\nOriginal Grammar as a tuple (V, T, P S) :\n");
    printGrammar(terms, nonterms, prods, numprod, start);
    elimLeftRec();
    printf("\n\nRevised Grammar after removing left recursion as a tuple (V, T, P, S):\n");
    printoutput();
    
    return 0;
}

void fileInput(){
    
    FILE *fp=fopen("input.txt","r");
    
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
    }
    fgetc(fp);
    fscanf(fp, "%s", &start);
    
}


void printoutput()
{
    printf("([");
    for(int i=0;i<nonterm;i++){
        printf(" %c,",nonterms[i]);
    }
    for(int i=0;i<strlen(newVars);i++){
        printf(" %c,",newVars[i]);
    }
    printf(" ],[");
    for(int i=0;i<term;i++){
        printf(" %c,",terms[i]);
    }
    printf(" ],[");
    for(int i=0;i<numprod;i++){
        printf(" %s,",prods[i]);
    }
    for(int i=0;i<extprodidx;i++){
        printf(" %s,",extraProd[i]);
    }
    printf(" ], %c)\n\n",start);
    printf("Number of New non terminals: %d\n\n",(int)strlen(newVars));
    printf("Set of New non terminals:\n");
    for(int i=0;i<strlen(newVars);i++){
        printf(" %c,",newVars[i]);
    }
    printf("\n\nSet of new Productions:\n");
    for(int i=0;i<extprodidx;i++){
        printf(" %s\n\n",extraProd[i]);
    }
}

void elimLeftRec()
{
	for(int i=0;i<nonterm;i++){
        char temp[20]; 
        strcpy(temp,prods[i]);
        strcpy(oriProds[i],prods[i]);
        
        for(int j=0;j<i;j++){
        
            //search nonterms[j] in temp
            
            for(int ind=0;ind<strlen(temp);ind++){
                if((temp[ind]==nonterms[j]) && (ind==3 || temp[ind-1]=='|')){
                
                    //get terminals in Ai->Ajalpha
                    int t=ind+1, k=0;
                    char alpha[20];
                    while(temp[t]!='|' && (t<strlen(temp))){
                        alpha[k++] = temp[t++];
                    }
                    alpha[k] ='\0';

                    //replace Aj with its productions
                    char prodtemp[20];
                    strcpy(prodtemp,prods[j]);
                    char temp2[20] ="", pro[20] ="";
                    
                    for(k=3;k<strlen(prodtemp);k++){
                        if(prodtemp[k]!='|'){
                            strncat(pro, &prodtemp[k],1);
                            
                        }
                        else{
                            strcat(pro,alpha);
                            strcat(temp2,pro);
                            strcat(temp2,"|");
                            strcpy(pro,"");
                        }
                        
                    }
                    
                    strcat(pro,alpha);
                    strcat(temp2,pro);
                    strcpy(pro,"");
                    
                    int p=0;
                    
                    for(k=t;k<strlen(temp);k++){
                        pro[p++] = temp[k];
                    }
                    
                    pro[p]='\0';
                    strcat(temp2,pro);
                
                    char st[20];
                    for(k=0;k<ind;k++){
                        st[k] = prods[i][k];
                    }
                    st[k] ='\0';
                    
                    strcat(st,temp2);   
                    strcpy(prods[i],st);
                    strcpy(temp,prods[i]);   
                }
            }
        }
        
        
    }
    
    for(int i=0;i<nonterm;i++)
    {
    
        //eliminate direct recursion
         
        char temp[20]; 
        strcpy(temp,prods[i]);
        elimDirRec(temp,i);
    }
    
}

void elimDirRec(char temp[20],int i){
    char new[20]="\0", newProd[20];
    int f=0;
    char var = newVar();
    for(int ind=0;ind<strlen(temp);ind++)
    {
        if((temp[0]==temp[ind]) && (ind==3 || temp[ind-1]=='|'))
        {
            f=1;
            int t=ind+1,k=0;
            char alpha[20];
            
            if(strlen(new)!=0) alpha[k++] = '|';
            while(temp[t]!='|' && (t<strlen(temp)))
            {
                alpha[k++] = temp[t++];
            }
            alpha[k++] = var;
            alpha[k++] = '\0';
            strcat(new,alpha);
        } 
    }
    
    if(f==0)
    {
        strcpy(prods[i],oriProds[i]);
    }
    else if(f==1)
    {
        strcat(new,"|#\0");
        newProd[0] = var;
        newProd[1] ='\0';
        strcat(newProd,"->");
        strcat(newProd,new);
        strcpy(extraProd[extprodidx++],newProd);
        
        char newnew[20];
        
        //identify terminals
        char new1[20]="\0";
        char *token = strtok(temp,"|");
        while(token!=NULL)
        {
            f = 1;
            for(int p=0;p<strlen(token);p++)
            {
                if(token[p]<97 || token[p]>122)
                {
                    f = 0;
                    break;
                }
            }
            if(f!=0)
            {
                strcat(new1,token);
                strncat(new1,&var,1);
                strcat(new1,"|");
            }
            token = strtok(NULL,"|");
        }
        strcat(new1,"\0");
        newnew[0] = temp[0];
        newnew[1] = '\0';
        strcat(newnew,"->");

        if(strlen(new1)==0)
        {
            strncat(newnew,&var,1);
            
        }
        else
        {
            strcat(newnew,new1);
            newnew[strlen(newnew)-1] ='\0';
        }
        strcpy(prods[i],newnew);
        strncat(newVars,&var,1);
    }
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
    }

    printf("\nEnter the start symbol: ");
    scanf(" %c", &start);
    getchar();
}


int searchLet(char *list, char letter)
{
    for(int r=0;r<strlen(list);r++)
    {
        if(list[r] == letter)
        {
            return 1;
        }
    }
    return 0;
}

char newVar()
{
    char c = (char)n_var;
    while(searchLet(nonterms,c) != 0)
    {
        n_var--;
        c = (char)n_var;
    }
    return c;
}

void printGrammar(char *term, char *nonTerm, char prod[][20], int n, char st)
{
    printf("\n( [");
    for(int i=0; i<strlen(nonTerm); i++)
    {
        printf("%c, ",nonTerm[i]);
    }
    printf(" ], [ ");
    
    for(int i=0; i<strlen(term); i++)
    {
        printf("%c, ",term[i]);
    }
    printf(" ], [ ");
    for(int i=0; i<n; i++)
    {
        printf("%s ,",prod[i]);
    }

    printf("], %c )\n",start);
}




