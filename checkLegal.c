#include <math.h>
#include "macroLib.h"
/*initalize finals to check the legality of a certain label*/
#define ZERO '0'
#define NINE '9'
#define START_CAP_LETTER 'A'
#define END_CAP_LETTER 'Z'
#define START_S_LETTER 'a'
#define END_S_LETTER 'z'
/*varaible for the number of commands*/
#define NUM_OF_COMMANDS 16
#define NUM_OF_INST 4
/*varaibles to check a register*/
#define REG_ST '@'
#define REG_CH 'r'
#define MAX_REG '7'
#define MIN_REG '0'
#define REG_SIZE 3


/*this function check if a name is legal in terms of charachters inside the name*/
int checkName(char *name)
{
	/*intialize varaible with the size of the name and i*/
	int len = strlen(name);
	int i = 0;
	while(i < len)
	{
		/*check with the askii code of the charchter if its not a number or a letter*/
		if((name[i] < ZERO || name[i] > NINE) &&(name[i] < START_CAP_LETTER || name[i] > END_CAP_LETTER) && (name[i] < START_S_LETTER || name[i] > END_S_LETTER))
		{
			return !OK;
		}
		i++;
	}
	return OK;
}
/*this function gets a name and check id its a saved name of assembly, if it is it return the number related to the name.*/
int checkLegalName(char *name)
{
	/*initalize strings arrays to loop and check the name*/
	static char *buildInComm[] = {"mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","red","prn","jsr","rts","stop"};
	static char *buildInInst[] = {".string",".data",".entry",".extern"};
	int i = 0,j =0;
	/*in each loop, check if the string is indentical to the a certain command and return the index its in*/
	while(i < NUM_OF_COMMANDS)
	{
		/*check with strcmp*/
		if(strcmp(name,buildInComm[i]) == 0)
		{
			return i;
		}
		i++;
	}
	while(j < NUM_OF_INST)
	{
		if(strcmp(name,buildInInst[j]) == 0)
		{
			return i;
		}
		i++;
		j++;
	}
	return i;
}
/*this function check if there is somthing before the pointer that is not a white space,it gets a pointer to a certian location on the string and the whole string*/
int checkBefore(char *c,char *str)
{
	int i = 0;
	/*while loop until we get to the pointer we want to check*/
	while(&str[i] != c)
	{
		/*check if there is somthing that is not a white space*/
		if(str[i] != TAB && str[i] != SPACE)
		{
			return !OK;
		}
		i++;
	}
	return OK;
}
/*this function gets a string and count his commas*/
int countCommas(char *str)
{
	/*define varaibles to count and loop*/
	int i = 0;
	int count = 0;
	/*loop until the size of the string*/
	while(i < strlen(str))
	{
		/*if it is a comma count if*/
		if(str[i] == COMMA)
		{
			count++;
		}
		i++;
	}
	/*return count*/
	return count;
}
/*this function gets a string and make all of him null*/
void terminate(char *line)
{
	/*initalize i*/
	int i = 0;
	/*loop until the langth of the line*/
	while(i < LINELENGTH)
	{
		/*set the current index to null*/
		line[i] = '\0';
		i++;
	}
}	
/*this function gets a linked list and free ot*/
void freeLabels(label **head)
{
	/*set varaibles to loop threw the linked list*/
	label *current = *head, *frr;
	/*while its not the last node*/
	while(current != NULL)
	{
		/*save the value of the current node*/
		frr = current;
		/*save the value of the next node*/
		current = current->next;
		/*free the current*/
		free(frr);
	}
}
/*this function gets all the pointers from the firstPass function and free them*/
int freeAll(label **la,label **ex,label **ent,label **wa,short **comArr,short **dataArr,label **da)
{
	/*set varaibles to the arrays*/
	short *aData = *comArr,*dArr = *dataArr;
	/*for each pointer, check if he is null, if not free it with the realted function*/
	if(*wa != NULL)
	{
		freeLabels(wa);
	}
	if(*la != NULL)
	{
		freeLabels(la);
	}
	if(*ex != NULL)
	{
		freeLabels(ex);
	}
	
	if(*ent != NULL)
	{
		freeLabels(ent);
	}
	if(*da != NULL)
	{
		freeLabels(da);
	}
	if(aData != NULL)
	{
		free(aData);
	}
	if(dArr != NULL)
	{
		free(dArr);
	}
	return OK;
}
/*a function that check if this is a register and if he is legal*/
/*the function gets a token, a line number and a file name*/
int checkRegister(char *token,int lineNum , char *name)
{
	/*set i to 0 for indexes in the token*/
	int i = OK;
	/*check if the first char is not '@' or longer than 3 chars*/
	if(token[i] != REG_ST || strlen(token) > REG_SIZE)
	{
		/*return identification that this cant be a register*/
		return REG_ERROR;
	}
	/*increase i*/
	i++;
	/*check if the next char is 'r' for register*/
	if(token[i] != REG_CH)
	{
		/*if its not, print a massage and retur an error flag*/
		fprintf(stderr,"In file %s\n In line %d ,'%s' is an illegal regiser\n",name,lineNum,token);
		return REG_FATAL;
	}
	i++;
	/*check if the register num is between 0 to 7*/
	if(token[i] < MIN_REG || token[i] > MAX_REG)
	{
		/*if not print a massage and return an error*/
		fprintf(stderr,"In file %s\nIn line %d the register should be between 0-7\n",name,lineNum);
		return REG_FATAL;
		
	}
	
	/*return the number of token by substructing from the number charachter '0'*/
	return (token[i]-ZE);
	
}
/*a function that free te struct that stores the parameters*/
int freeCommLine(cmdLine **cm)
{
	/*define varaibles to save the addres of the strruct*/
	cmdLine *cmd = *cm;
	/*check if null*/
	if(cmd != NULL)
	{
		/*check if the parameter is null*/
		if(cmd->param1 != NULL)
		{
			/*free the parameter*/
			free(cmd->param1);
		}
		/*check if the second parameter is null*/
		if(cmd->param2 != NULL)
		{
			/*free the parameter*/
			free(cmd->param2);
		}
		/*free the struct*/
		free(cmd);
	}
	return OK;
}

/*this function set an array fron the current index to 0*/
void makeZero(short *array, int *index,int amount)
{
	/*save the index in i*/
	int i = *index;
	/*loop until end of size*/
	while(i<amount)
	{
		/*set the index to 0*/
		array[i] = OK;
		i++;
	}
}

/*this function gets to arrays and their sizes and unite them*/
int connect(short **arr1,short *arr2,int size1,int size2)
{
	/*calculate the new size*/
	int newSize = size1 + size2;
	/*set varaibles for malloc and realloc func*/
	short *newArr = NULL,*fArr = *arr1;
	/*set varaibles to loop threw the arrays*/
	int i = size1, j = 0;
	/*check if the command array is null*/
	if(!(*arr1))
	{
		/*use malloc to get space with the new size*/
		newArr = (short *)malloc(newSize * sizeof(short));
		/*check seccus*/
		if(newArr == NULL)
		{
			/*if not print a massage and return error*/
			fprintf(stderr,"Not enough space \n");
			return FATAL_ERROR;
		}
	}
	else
	{
		/*get new space with realloc and the command array*/
		newArr = (short *)realloc(fArr,newSize * sizeof(short));
		/*check if null and print a massage*/
		if(newArr== NULL)
		{
			fprintf(stderr,"Not enough space \n");
			return FATAL_ERROR;
		}
	}
	/*set the addres to fArr*/
	fArr = newArr;
	/*loop until the end of the new size*/
	while(i < newSize)
	{
		/*in the current index put the current of the data array*/
		fArr[i] = arr2[j];
		/*increase i and j*/
		i++;
		j++;
	}
	/*set the addres to the pointer*/
	*arr1 = fArr;
	return OK;
	
	
}
void printBinary(short num) {
	int i;
	for (i = sizeof(num) * 8 - 1; i >= 0; i--) {
		putchar((num & (1 << i)) ? '1' : '0');
	}
}

void printShortArrayAsBinary(const short* arr, int size) {
	int i;
	for (i = 0; i < size; i++) {
		printBinary(arr[i]);
		putchar('\n');
	}
	putchar('\n');
}
