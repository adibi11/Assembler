#include "macroLib.h"
/*define a macro to enlarge the string size*/
#define ENLARGE_NAME(X) (X) + 5
/*define macros to connect a ending to a file*/
#define EXT_END ".ext"
#define ENT_END ".ent"
#define OB_END ".ob"
/*define a maske with 6 first bits on*/
#define MASK 63
/*define a the movemoent of the bits*/
#define MOVE_OB(X) (X)>>6

/*define macros for the base 64*/
/*max capitel letters*/
#define MAX_CAP 26
/*first capital letter*/
#define F_CAP 'A'
/*max small letter*/
#define MAX_S 52
/*first small letter*/
#define F_SMALL 'a'
/*max digits*/
#define MAX_DIG 62
/*check for slash*/
#define SLASH_DIG 63
/*the char '0'*/
#define ZE '0'
/*chars to return*/
#define PLUS '+'
#define SLASH '/'
#define EQ '='
/*in this function we get a linked list pf labels and print its values*/
int printLabels(label *head,char *name,int sign)
{
	/*pointer to a file to open new file*/
	FILE *f;
	/*pointer to a char to new file name*/
	char *newName;
	/*pointer to the linked list*/
	label *current = head;
	/*get new space from malloc to the new name*/
	newName = (char *)malloc((ENLARGE_NAME(strlen(name))) * sizeof(char));
	/*check if success*/
	if(newName == NULL)
	{
		/*print a massage and return FATAL_ERROR*/
		fprintf(stderr,"Not enough space\n");
		return FATAL_ERROR;
	}
	/*copyy the name of the original file*/
	strcpy(newName,name);
	/*if its an extern*/
	if(sign)
	{
		/*add .ext to the end of the file name*/
		strcat(newName,EXT_END);
	}
	else
	{
		/*add .ent to the end of the file name*/
		strcat(newName,ENT_END);
	}
	/*open the file for writing*/
	f = fopen(newName,"w");
	/*check seccuss*/
	if(f == NULL)
	{
		/*print a massage, free the name and return FATAL_ERROR*/
		fprintf(stderr,"Cant open file \n");
		free(newName);
		return FATAL_ERROR;
	}
	/*loop until the end of the linked list*/
	while(current != NULL)
	{
		/*print the name and the address*/
		fprintf(f,"%s\t%d\n",current->name,current->addres);
		/*move to the next node*/
		current = current->next;
	}
	/*free the name and close the file and return OK*/
	free(newName);
	fclose(f);
	return OK;
	

	
}
/* Function to convert a 6-bit value to a base64 character */
char base64char(int value) 
{
	if (value < MAX_CAP) 
	{
		/* Characters 'A' to 'Z'*/
        	return F_CAP + value;
    	}
    	else if (value < MAX_S) 
    	{
        	/* Characters 'a' to 'z'*/
        	return F_SMALL + (value - MAX_CAP);
        }
    	else if (value < MAX_DIG) 
    	{
        	/* Digits '0' to '9'*/
        	return ZE + (value - MAX_S);
        }
    	else if (value == MAX_DIG) 
    	{
        	/* Character '+'*/
        	return PLUS;
    	}
    	else if (value == SLASH_DIG) 
    	{
        	/* Character '/'*/
        	return SLASH;
   	}
    	/* Padding character '='*/
    	return EQ;
}


int printOb(char *name,int sizeI,int sizeD, short *arr)
{
	/*define a pointer to a file*/
	FILE *f;
	/*define a pointer to new name*/
	char *newName;
	/*define a char to get the base64 values*/
	char ch1,ch2;
	/*define i for loop and the size of the array*/
	int i = OK,size = sizeI + sizeD, curr;
	/*get space from malloc to the new name*/
	newName = (char *)malloc(ENLARGE_NAME(strlen(name))*sizeof(char));
	/*check success*/
	if(!newName)
	{
		fprintf(stderr,"Not enough space \n");
		return FATAL_ERROR;
	}
	/*coppy the name to the new string*/
	strcpy(newName,name);
	/*add .ob in the end*/
	strcat(newName,OB_END);
	/*open the .ob file*/
	f = fopen(newName,"w");
	/*check succes*/
	if(!f)
	{
		fprintf(stderr,"Cant open file \n");
		/*free the string*/
		free(newName);
		return FATAL_ERROR;
	}

	/*print the size of instructions and size of data*/
	fprintf(f,"%d   %d\n",sizeI,sizeD);
	while(i < size)
	{
		/*get the 6 first bits*/
		curr = (arr[i]&MASK);
		/*get the char using a function that converts bits to base64*/
		ch1 = base64char(curr);
		/*save the next 6 bits by moving and initalizeing by 6 bits*/
		curr = MOVE_OB(arr[i]);
		/*get the next 6 bits char*/
		ch2 = base64char(curr);
		/*print the next line*/
		fprintf(f,"%c%c\n",ch2,ch1);
		i++;
	}
	/*free the name and close the file*/
	free(newName);
	fclose(f);
	return OK;
	
	
}
