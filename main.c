/*
	Name: R.Shyam Sundar
	Roll: CS20B1029
	Date: 28/09/2022
	Compiler design Lab 9: Intermediate code generator
*/

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<limits.h>
#include<assert.h>

char str[1000];

/*
	Increasing order of priority:  (highest to lowest)
	<, <=, >, >=, &&, ||
*/

void insertString(char* destination, int pos, char* seed)
{
    char * strC;

    strC = (char*)malloc(strlen(destination)+strlen(seed)+1);
    strncpy(strC,destination,pos);
    strC[pos] = '\0';
    strcat(strC,seed);
    strcat(strC,destination+pos);
    strcpy(destination,strC);
    free(strC);
}

void add_primary_brackets()
{
	bool found_var = false;
	for(int i=0;i<strlen(str);i++){
		if(!found_var && ((str[i]>='A'&&str[i]<='B')||(str[i]>='a'&&str[i]<='z')))
		{
			found_var = true;
			char temp[10] = "(";
			insertString(str,i,temp);
			i++;
		}
		else if(found_var && ((str[i]>='A'&&str[i]<='B')||(str[i]>='a'&&str[i]<='z')))
		{
			found_var = false;
			char temp[10] = ")";
			insertString(str,i+1,temp);
			i++;
		}
	}
}	

int interval[2][100];
int inter_index;

void print_bracket_sequence()
{
	for(int i=0;i<inter_index;i++){
		printf("\n[%d %d]",interval[i][0],interval[i][1]);
	}
	printf("\n");
}

int remove_expr_at_index(int left,int right)
{
	printf("\n<left = %d, right = %d>",left,right);
	int left_index = -1,right_index = -1;
	for(int i=0;i<inter_index;i++){
		if(interval[i][0]==left){
			left_index = i;
			interval[i][1] = right;
			i++;
			while(true){
				// assert(i < inter_index);
				bool breaknow = false;
				if(interval[i][1] == right)breaknow = true;
				interval[i][0] = 0;
				interval[i][1] = 0;
				if(breaknow)break;
				i++;
				print_bracket_sequence();
				printf("\n<i val = %d | left_index = %d | right_val = %d>",i,left_index,interval[i][1]);
			}
			break;
		}	
	}
	print_bracket_sequence();
	for(int i=left_index+1;i<inter_index;i++){
		interval[i][0] = interval[i+1][0];
		interval[i][1] = interval[i+1][1];
	}
	print_bracket_sequence();
}

void update_inter_index()
{
	int tem_len = 0;
	for(int i=0;i<inter_index;i++){
		if(interval[i][0] != 0 && interval[i][0] != 0){
			tem_len++;
		}
	}
	inter_index = tem_len+1;
	printf("\nNew interval length = %d",inter_index);
}

void calc_bracket_intervals()
{
	printf("\nBracket Intervals:");
	printf("\nStr = %s",str);

	inter_index = 0;
	int stack = 0;
	int left = -2;
	for(int i=0;i<strlen(str);i++){
		if(str[i] == '('){
			stack++;
			if(stack == 1)left = i;
		}
		if(str[i] == ')'){
			stack--;
			if(stack == 0){
				interval[inter_index][0] = left;
				interval[inter_index][1] = i;
				inter_index++;
				left = -2;
			}
		}
	}
}

void add_brackets_acc_to_priority()
{
	memset(interval,0,sizeof(interval));
	int expr[100],index = 0;
	
	// && , || indices
	for(int i=0;i<strlen(str);i++){
		if(str[i] == '&'){
			expr[index] = i;
			i++;
			index++;
		}
	}
	for(int i=0;i<strlen(str);i++){
		if(str[i] == '|'){
			expr[index] = i;
			i++;
			index++;
		}
	}
	printf("\nIndexes containing '&&' and '||' (sorted according to priority)  :  ");
	for(int i=0;i<index;i++){
		printf("%d ",expr[i]);
	}	
	inter_index = 0;
	// First bracket interval
	calc_bracket_intervals();
	print_bracket_sequence();


	for(int i=0;i<index;i++){
		int x = expr[i];
		int mx_val = INT_MIN;
		for(int j=0;j<inter_index;j++){
			if(interval[j][0]<x && interval[j][0] > mx_val){
				mx_val = interval[j][0];
			}
		}
		int left = mx_val;
		int right;
		for(int j=0;j<inter_index;j++){
			if(interval[j][1]>x){
				right = interval[j][1];
				
				break;
			}
		}
	}
}

int main()
{	
	printf("\nEnter the boolean expression:  ");
	int index = 0;
	char x;
	scanf("%c",&x);
	str[0] = x;
	index++;	
	while(x != '\n'){
		scanf("%c",&x);
		str[index] = x;
		index++;
	}
	str[index] = '\0';
	printf("\nGiven Expression = %s",str);
	add_primary_brackets();
	printf("\nAdded Primary Brackets:  %s",str);

	add_brackets_acc_to_priority();


	return 0;
}
