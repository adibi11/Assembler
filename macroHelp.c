#include "macroLib.h"

#define OK 0

/*this function is used to get the macro command from the original file, this function runs under the assemption there is a endmcro flag*/
int getComm(char **c,FILE *f)
{
	/*set to char pointers for the malloc and realloc functions*/
	char *get,*org = *c;

	/*set varaibles for the loop, use count to count how mush apce we used, amount to ask from malloc end realloc, on for the main loop, and i for another loop, initalize i to 7*/
	int count = OK, amount = SIZE,on = 1, i = END_MACRO_SIZE;
	/*ask from space with malloc*/
	org = (char *)malloc(amount*sizeof(char));
	if(org == NULL)
	{
		fprintf(stderr,"Not enough space for your command \n");
		return FATAL_ERROR;
	}
	
	/*because we can assume there is an endmacro flag, we will run the loop until we find it in the command, and change on to 0*/
	while(on)
	{
		/*if we run out of space*/
		if(count == amount)
		{
			/*ask from realloc for space with another pointer*/
			get = (char *)realloc(org,(amount=ENLARGE(amount))*sizeof(char));
			if(get == NULL)
			{
				fprintf(stderr,"Not enough space for your command \n");
				*c = org;
				return FATAL_ERROR;
			}
			/*set org to get because the adress may have changed*/
			org = get;
		}
		/*set the current char in the array*/
		org[count] = fgetc(f);
		/*if count is greater than 7, check if there was already an endmcro flag*/

		if(count > END_MACRO_SIZE)
		{
			/*check for the flag with strstr*/
			if(strstr(((org + count) - END_MACRO_SIZE),END_MACRO))
			{
				/*run until i = 0, and set all the last char to null*/
				while(i)
				{
					org[count - i] = '\0';
					i--;
				}
				i = 0;
				/*increase i until we get to the first NULL char*/
				while(org[i] != '\0')
				{
					i++;
				}
				/*put instead new line char*/
				org[i] = NEW_LINE;
				/*stop the loop*/
				on = OK;
			}
		}
		/*increase count*/
		count++;
		/*set org to the original pointer*/
		*c = org;
		
		
			
		
	}
	/*check if there is anything after the endmcro flag*/
	if(checkForWhite(f))
	{
		return ERROR;
	}
	return OK;
}
/*this function gets a pointer to pinter to a macro node, and free the list*/
int freeMacro(node **head)
{
	/*set to varaibkes and save the addres of the head*/
	node *cur = *head ,*frr ;
	/*check if te head is null*/
	if(!cur)
	{
		return OK;
	}
	/*loop the nodes*/
	while(cur != NULL)
	{
		/*save the next node and free the current untilthe end of the list*/
		frr = cur;
		cur = cur->next;
		/*check if the command is empty*/
		if(frr->comm != NULL)
		{
			/*free the command*/
			free(frr->comm);
		}
		/*free the current node*/
		free(frr);
	}
	return OK;
}
/*this function check if there is a char which not supposed to be there in a certain line of a file*/
int checkForWhite(FILE *f)
{
	/*initialize c in case of EOF, altho the assemption is the mcro defined in the start of the file and there is an endmcro flag*/
	int c;
	/*loop until c is not EOF*/
	while((c = fgetc(f)) !=EOF)
	{
		/*if we got to anew linr return 0*/
		if(c == NEW_LINE)
		{
			return OK;
		}
		/*if this is not a white space return error*/
		if(c != TAB && c != SPACE)
		{
			fprintf(stderr,"There is an extra char after endmcro : %c \n",c);
			return ERROR;
		}
	}
	fprintf(stderr,"Shouldnt define a macro in the end of the file \n");
	return ERROR;
}
		
		
		
		
		
		

