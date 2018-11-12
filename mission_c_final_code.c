#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
typedef struct node{
	struct node *next;
	int element;
}NODE;
typedef struct variable{
	char cheracter;
	int flag;
}VARIABLE;
typedef struct tree{
	char vertex,*string;
	int print_flag;
	struct tree *left_child,*right_child;
}TREE;
char *eqn,*new_eqn,*postorder_eqn,**table_header,*operator_list[]={"4'","4\"","2.","3@NAND","3#NOR","3$XOR","3%XNOR","1+"};//related to isoperater(32),changes may apply in isoperator,issubstring(31)
int eqn_length,variable_list_length,new_eqn_length,postorder_eqn_length,temp,line_no,col_no,**table,table_header_length;
VARIABLE *variable_list;
TREE *root=NULL;
void error(int x)
{
	switch(x)
	{
		case 0:
			printf("\a\nError! Syntax error\n");
			break;
		case 1:
			printf("\a\nError! Operand missing\n");
			break;
		case 2:
			printf("\a\nError! Operator missing\n");
			break;
		case 3:
			printf("\a\nError! '(' missing\n");
			break;
		case 4:
			printf("\a\nError! '(' missing\n");
			break;
		case 5:
			printf("\a\nError! Unavailable memory space\n");
			break;
		case 34:
			printf("\a\nError! Misplaced \"\n");
			break;
		case 39:
			printf("\a\nError! Misplaced \'\n");
			break;
	}
	exit(0);
}
void assaign_table()
{
	int i,j,k,binary,temp=pow(2,variable_list_length);
	for(i=0;i<temp;i++)
	{
		for(j=variable_list_length-1,binary=i;j>=0;j--)
		{
			table[i][j]=binary%2;
			binary/=2;
		}
	}
	for(i=0,k=variable_list_length;i<variable_list_length;i++)
	{
		if(variable_list[i].flag)
		{
			for(j=0;j<temp;j++)
			{
				table[j][k]=1-table[j][i];
			}
			k++;
		}
	}
	col_no=k;	
}
void user_realloc_variable(VARIABLE **ptr,int n)
{
	VARIABLE *temp;
	int i;
	temp=(VARIABLE*)malloc(sizeof(VARIABLE)*n);
	if(temp==NULL)
	error(5);
	for(i=0;i<n-1;i++)
	temp[i]=*(*ptr+i);
	temp[i].flag=0;
	free(*ptr);
	*ptr=temp;
}
void user_realloc_char_pointer(char ***ptr,int new_size)
{
	char **temp;
	int i;
	temp=(char**)malloc(sizeof(char*)*new_size);
	if(temp==NULL)
	error(5);
	for(i=0;i<new_size-1;i++)
	temp[i]=*(*ptr+i);
	free(*ptr);
	*ptr=temp;
}
void user_realloc_char(char **ptr,int present_size,int new_size)
{
	char *temp;
	int i;
	temp=(char*)malloc(sizeof(char)*new_size);
	if(temp==NULL)
	error(5);
	for(i=0;i<present_size&&i<new_size;i++)
	temp[i]=*(*ptr+i);
	free(*ptr);
	*ptr=temp;
}
void create_2d_table()
{
	int i,temp;
	temp=pow(2,variable_list_length);
	table=(int**)malloc(sizeof(int*)*temp);
	if(table==NULL)
	error(5);
	for(i=0;i<temp;i++)
	{
		table[i]=(int*)malloc(sizeof(int)*table_header_length);
		if(table[i]==NULL)
		error(5);
	}
}
char issubstring(char *substring,char *string)
{
	int i,j;
	if(strlen(substring)!=strlen(string)-2)
	return(0);
	for(i=2,j=0;i<strlen(string);i++,j++)
	{
		if(*(substring+j)!=*(string+i))
		return('\0');
	}
	return(*(string+1));
}
char isoperator(char temp[])
{
	int i;
	char temporary;
	for(i=3;i<7;i++)
	{
		temporary=issubstring(temp,operator_list[i]);
		if(temporary)
		return(temporary);
	}
	return('\0');
}
int input_variable(char temp,int no,int mode)
{
	int i;
	for(i=0;i<variable_list_length;i++)
	{
		if(variable_list[i].cheracter==temp)
		break;
	}
	if(i<variable_list_length&&variable_list[i].flag==-1)
	return(0);
	else if(mode)
	variable_list[i].flag=(variable_list[i].flag%2==1)?-1:0;
	else if(i<variable_list_length)
	variable_list[i].flag+=no;
    else
    {
        user_realloc_variable(&variable_list,variable_list_length+1);
		variable_list[variable_list_length].cheracter=temp;
		variable_list_length=variable_list_length+1;
	}
}
void operator_check_and_varibale_list()
{
	int i,operand_flag=0,temp,r;
	char string[5],last_variable='\0',temporary; 
	variable_list=(VARIABLE*)malloc(sizeof(VARIABLE));
	if(variable_list==NULL)
	error(5);
	new_eqn=(char*)malloc(sizeof(char));
	if(new_eqn==NULL)
	error(5);		
	for(i=0;i<eqn_length;)
	{
		for(temp=i;eqn[temp]==32&&temp<eqn_length;temp++);
		i=temp;
		if(isalpha(eqn[i]))
		{
			if(i+1<eqn_length&&isalpha(eqn[i+1]))
			{
				string[0]=string[1]=string[2]=string[3]=string[4]='\0';
				string[0]=eqn[i];
				for(temp=1;temp<4&&i+temp<eqn_length&&isalpha(eqn[i+temp]);temp++)
				string[temp]=eqn[i+temp];
				temporary=isoperator(string);
				if(temp==4&&isalpha(eqn[i+temp]))
				error(0);
				else if(temporary)
				{
					if(!operand_flag)
					error(1);
					i=i+temp;
					operand_flag=0;
					input_variable(last_variable,0,1);
				    user_realloc_char(&new_eqn,new_eqn_length,++new_eqn_length);
					new_eqn[new_eqn_length-1]=temporary;
				}
				else
				error(0);	
			}
			else
			{
				if(operand_flag)
				error(2);
				operand_flag=1;
				last_variable=eqn[i];
			    user_realloc_char(&new_eqn,new_eqn_length,++new_eqn_length);
				new_eqn[new_eqn_length-1]=eqn[i];
				input_variable(last_variable,0,0);
				i++;
			}	
	   }
		else if(eqn[i]==34||eqn[i]==39)
		{
			if(!operand_flag)
			error((int)eqn[i]);
			else
			{
				if(eqn[i]==34)
				input_variable(last_variable,2,0);
				else
				{
					input_variable(last_variable,1,0);
					if(new_eqn[new_eqn_length-1]==39)
					user_realloc_char(&new_eqn,new_eqn_length,--new_eqn_length);
					else
					{
					    user_realloc_char(&new_eqn,new_eqn_length,++new_eqn_length);
						new_eqn[new_eqn_length-1]=eqn[i];
					}	
				}
				operand_flag=1;
				i++;
			}
    	}
    	else if(eqn[i]=='0'||eqn[i]=='1')
    	{
    		if(operand_flag)
			error(2);
		    user_realloc_char(&new_eqn,new_eqn_length,++new_eqn_length);
			new_eqn[new_eqn_length-1]=eqn[i];
			i++;
			operand_flag=1;
			last_variable='\0';
		}
		else if(eqn[i]=='.'||eqn[i]=='+'||eqn[i]=='-')	
		{
			if(!operand_flag)
			error(1);
		    user_realloc_char(&new_eqn,new_eqn_length,++new_eqn_length);
			new_eqn[new_eqn_length-1]=eqn[i];
			i++;
			operand_flag=0;	
			input_variable(last_variable,0,1);
		}
		else if(eqn[i]=='(')
		{
			if(operand_flag)
			error(2);
		    user_realloc_char(&new_eqn,new_eqn_length,++new_eqn_length);
			new_eqn[new_eqn_length-1]=eqn[i];
			i++;
			operand_flag=0;
		}
		else if(eqn[i]==')')
		{
			if(!operand_flag)
			error(1);
		    user_realloc_char(&new_eqn,new_eqn_length,++new_eqn_length);
			new_eqn[new_eqn_length-1]=eqn[i];
			i++;
			operand_flag=1;
			input_variable(last_variable,0,1);
			last_variable='\0';
		}
		if(i==eqn_length)
		input_variable(last_variable,0,1);	
	}
	if(!operand_flag)
	error(0);
}
void push(NODE **stack,int data)
{
	NODE *temp;
	temp=(NODE *)malloc(sizeof(NODE));
	if(temp==NULL)
	error(5);
	temp->element=data;
	temp->next=*stack;
	*stack=temp;
}
char pop(NODE **stack)
{
	NODE *temp;
	char x;
	x=(*stack)->element;
	temp=*stack;
	*stack=(*stack)->next;
	free(temp);
	return(x);
}
int priority(char temp)
{
	int i;
	for(i=0;i<8;i++)
	{
		if(operator_list[i][1]==temp)
		return(operator_list[i][0]);
	}
}
void inorder_to_postorder()
{
	int i;
	NODE *stack=NULL;
	for(i=0;i<new_eqn_length;i++)
	{
		if(new_eqn[i]=='(')
		push(&stack,(int)new_eqn[i]);
		else if(new_eqn[i]==')')
		{
			while(stack->element!='(')
			{
				user_realloc_char(&postorder_eqn,postorder_eqn_length,postorder_eqn_length+1);
				postorder_eqn[postorder_eqn_length]=pop(&stack);
				postorder_eqn_length++;
			}
			pop(&stack);
		}
		else if(isalpha(new_eqn[i])||new_eqn[i]=='0'||new_eqn[i]=='1')
		{
			user_realloc_char(&postorder_eqn,postorder_eqn_length,postorder_eqn_length+1);
			postorder_eqn[postorder_eqn_length]=new_eqn[i];
			postorder_eqn_length=postorder_eqn_length+1;
		}
		else
		{
			while(stack!=NULL&&stack->element!='('&&priority(stack->element)>=priority(new_eqn[i]))
			{
				user_realloc_char(&postorder_eqn,postorder_eqn_length,postorder_eqn_length+1);
				postorder_eqn[postorder_eqn_length]=pop(&stack);
				postorder_eqn_length++;
			}
			push(&stack,(int)new_eqn[i]);
		}
	}
	while(stack!=NULL)
	{
		user_realloc_char(&postorder_eqn,postorder_eqn_length,postorder_eqn_length+1);
		postorder_eqn[postorder_eqn_length]=pop(&stack);
		postorder_eqn_length++;
	}
}
int bracket_check()
{
	NODE *stack=NULL; 
	int i;
	for(i=0;i<eqn_length;i++)
	{
		if(eqn[i]=='(')
		push(&stack,i);
		else if(eqn[i]==')')
		{
			if(stack==NULL)
			{
				printf("\a\nError! Missing'('\n");
				exit(0);
			}
			else
			pop(&stack);
		}
	}
	if(stack!=NULL)
	{
		printf("\a\nError! Missing')'\n");
	    exit(0);
	}
}
int valid(char temp)
{
	if((temp>=65&&temp<=90)||(temp=='.')||(temp=='+')||(temp=='-')||(temp==34)||(temp==39)||(temp=='(')||(temp==')')||(temp==32)||(temp=='0')||(temp=='1'))// ASCII* 34=' and 39=" and 32=SPACE
	return(1);
	return(0);
}
void print_array(char *base,int size)
{
	int i;
	for(i=0;i<size;i++)
	printf("%c",*(base+i));
}
int eqn_read()
{
	char temp;
	int i=0;
	eqn=(char*)malloc(sizeof(char));
	if(eqn==NULL)
	error(5);
	temp=toupper(getch());
	while(temp!=13)
	{
		while(!valid(temp))
		{
			printf("\a\nError! Invalid cheracter'%c'\n",temp);
			print_array(eqn,i);
			temp=toupper(getch());
			if(temp==13)
			{
			    user_realloc_char(&eqn,i,++i);
				eqn[i]='\0';
				eqn_length=i-1;
				return(0);
			}
		}
		printf("%c",temp);
		user_realloc_char(&eqn,i,++i);
		eqn[i-1]=temp;
		temp=toupper(getch());
	}
	eqn[i]='\0';
	eqn_length=i;
	return(0);
}
void sort_variable_list()
{
	int i,j;
	VARIABLE t;
	for(i=0;i<variable_list_length-1;i++)
	{
		for(j=0;j<variable_list_length-i-1;j++)
		{
			if(variable_list[j].cheracter>variable_list[j+1].cheracter)
			{
				t=variable_list[j];
				variable_list[j]=variable_list[j+1];
				variable_list[j+1]=t;
			}
		}
	}
	if(variable_list[0].cheracter=='\0')
	{
		variable_list=&(variable_list[1]);
		variable_list_length--;
	}
}
void set_tree(TREE *ptr)
{
	ptr->string=NULL;
	ptr->left_child=NULL;
	ptr->right_child=NULL;
	ptr->print_flag=0;
}
int create_tree(TREE *ptr)
{
	TREE *x;
	if(ptr->left_child!=NULL&&ptr->right_child!=NULL||ptr->vertex==39&&ptr->right_child!=NULL||temp<0)
	return(0);
	if(ptr->right_child==NULL)
	{
		x=(TREE *)malloc(sizeof(TREE));
		if(x==NULL)
		exit(5);
		set_tree(x);
		x->vertex=postorder_eqn[temp--];
		ptr->right_child=x;
		if(ispunct(x->vertex))
        create_tree(x);
        else
        create_tree(ptr);
	}
	if(ptr->left_child!=NULL&&ptr->right_child!=NULL||ptr->vertex==39&&ptr->right_child!=NULL||temp<0)
	return(0);
	if(ptr->left_child==NULL)
	{
		x=(TREE *)malloc(sizeof(TREE));
		if(x==NULL)
		exit(5);
		set_tree(x);
		x->vertex=postorder_eqn[temp--];
		ptr->left_child=x;
		if(ispunct(x->vertex))
        create_tree(x);
        else
        create_tree(ptr);
	}
	return(0);
}
int and_operator(int x,int y)
{
	return(x*y);
}
int or_operator(int x,int y)
{
	if(x==1||y==1)
	return(1);
	else
	return(0);
}
int not_operator(int x)
{
	if(x==0)
	return(1);
	else
	return(0);
}
int nand_operator(int x,int y)
{
	return(not_operator(x*y));
}
int nor_operator(int x,int y)
{
	return(not_operator(x||y));
}
int xor_operator(int x,int y)
{
	return(x*not_operator(y)||y*not_operator(x));
}
int xnor_operator(int x,int y)
{
	return(not_operator(xor_operator(x,y)));
}
int calculate_value(TREE *ptr)
{
	int i;
	if(ptr->right_child==NULL&&ptr->left_child==NULL)
	{
		if(ptr->vertex=='1')
		return(1);
		else if(ptr->vertex=='0')
		return(0);
		else
		{
			for(i=0;i<variable_list_length;i++)
			if(variable_list[i].cheracter==ptr->vertex)
			break;
			return(table[line_no][i]);
		}
	}
	else
	{
		switch(ptr->vertex)
		{
			case 39:
				return(not_operator(calculate_value(ptr->right_child)));
				break;
			case 46:
				return(and_operator(calculate_value(ptr->left_child),calculate_value(ptr->right_child)));
				break;
			case 43:
				return(or_operator(calculate_value(ptr->left_child),calculate_value(ptr->right_child)));
				break;
			case 64:
				return(nand_operator(calculate_value(ptr->left_child),calculate_value(ptr->right_child)));
				break;
			case 35:
				return(nor_operator(calculate_value(ptr->left_child),calculate_value(ptr->right_child)));
				break;
			case 36:
				return(xor_operator(calculate_value(ptr->left_child),calculate_value(ptr->right_child)));
				break;
			case 37:
				return(xnor_operator(calculate_value(ptr->left_child),calculate_value(ptr->right_child)));
				break;
		}
	}
}
void fill_table()
{
	for(line_no=0;line_no<pow(2,variable_list_length);line_no++)
    table[line_no][col_no]=calculate_value(root);
}
void create_table_header()
{
	char *temp;
	int i;
	table_header=(char**)malloc(sizeof(char*));
	if(table_header==NULL)
	exit(5);
	for(i=0;i<variable_list_length;i++)
	{
		temp=(char*)malloc(sizeof(char)*2);
		if(temp==NULL)
		exit(5);
		temp[0]=variable_list[i].cheracter;
		temp[1]='\0';
	    user_realloc_char_pointer(&table_header,table_header_length+1);
		table_header[table_header_length]=temp;
		table_header_length++;
	}
	for(i=0;i<variable_list_length;i++)
	{
		if(variable_list[i].flag)
		{
			temp=(char*)malloc(sizeof(char)*3);
			if(temp==NULL)
		    exit(5);
			temp[0]=variable_list[i].cheracter;
			temp[1]=39;
			temp[2]='\0';
		    user_realloc_char_pointer(&table_header,table_header_length+1);
			table_header[table_header_length]=temp;
			table_header_length++;
		}
	}
	temp=(char*)malloc(sizeof(char)*eqn_length+1);
	if(temp==NULL)
	exit(5);
	for(i=0;i<eqn_length;i++)
	temp[i]=eqn[i];
	temp[i]='\0';
	user_realloc_char_pointer(&table_header,table_header_length+1);
	table_header[table_header_length]=temp;
	table_header_length++;
}
void main()
{
	printf("PROJECT TRUTH TABLE GENERATOR\n");
	printf("For nand operator use 'NAND'.\nFor xor operator use 'XOR'.\nFor nor operator use 'NOR'\nFor xnor operator use 'XNOR'\n ");
	printf("Enter boolean expression:");
	int i,j;
	eqn_read();
	bracket_check();
	operator_check_and_varibale_list();
	sort_variable_list();
	create_table_header();
	create_2d_table();
	assaign_table();
	inorder_to_postorder();
	root=(TREE *)malloc(sizeof(TREE));
	if(root==NULL)
	exit(5);
	set_tree(root);
	temp=postorder_eqn_length-1;
	root->vertex=postorder_eqn[temp--];
    if(temp>=0)
    create_tree(root);
    fill_table();
    printf("\n");
	for(i=0;i<table_header_length;i++)
	printf("%s\t",table_header[i]);
	for(i=0;i<pow(2,variable_list_length);i++)
	{
		printf("\n");
		for(j=0;j<table_header_length;j++)
		printf("%d\t",table[i][j]);
	} 		
}
