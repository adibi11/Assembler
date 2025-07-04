#include "macroLib.h"
/*numbers to check how manny parameters*/
#define MAX_0 1
#define MAX_1 2
#define MAX_2 3

/*In this file we will initalize function to check the parameters*/
/*we assuming, that if the parameter wasnt an integer and contianed chars, this supposed to be a label*/

/*checj for rts or stop*/
/*this function gets a file name, a line number, array of instruction lines, a counter and a struct with data*/
int checkZeroParam(char *name,int lineNum,short *array,int *cnt,cmdLine *instr)
{
	/*check if there is parameters*/
	if((instr->cnt)>MAX_0)
	{
		/*print a massage and return error*/
		fprintf(stderr,"In file %s\nIn line %d, there souldnt be a parameter \n",name,lineNum);
		return ERROR;
	}
	insertSym(array,*cnt,instr->sign2,instr->sign1,instr->comNum);
	/*in the current index, insert the comand value in from the 5 bit*/
	array[*cnt] = MOVE_COM(instr->comNum);
	/*increase the counter*/
	(*cnt)++;
		
	return OK;
	
}

/*check for the commands:not,clr,inc.dec.jmp,bne,red,jsr*/
/*this function gets a name of file, a line number, an array of commands, a counter, a data structure and a label node*/
/*this function checks commands parameters, that can only get a register or a label*/
int checkOneParam1(char *name,int lineNum,short *array,int *cnt,cmdLine *instr,label **la)
{
	/*varaibles for the linked list of unknown labels*/
	label *head = *la,*current = *la,*newNode = NULL;
	/*check if there is more or less than one parameter and print a massage,and return ERROR*/
	if((instr->cnt) > MAX_1)
	{
		fprintf(stderr,"In file %s\n,In line %d, there is an extra parameter \n",name,lineNum);
		return ERROR;
	}
	if((instr->cnt)<MAX_1)
	{
		
		fprintf(stderr,"In file %s\n,In line %d, you missed a parameter\n",name,lineNum);
		return ERROR;
	}
	/*if the parameter is a digit print a massage and return ERROR*/
	if((instr->sign1) == DIGIT_FLAG)
	{
		fprintf(stderr,"In file %s\nIn line %d, this function cant get a decimal\n",name,lineNum);
		return ERROR;
	}
	/*in the current value, save the command number from the 5 bit*/
	insertSym(array,*cnt,instr->sign2,instr->sign1,instr->comNum);
	/*check if this is a register*/
	/*increase the counter*/
	(*cnt)++;
	if((instr->sign1)== REG_FLAG)
	{
		
		/*insert the number of the register to bit 2*/
		array[*cnt] |= MOVE_DEST(instr->value1);
		
	}
	else
	{
		/*check if the label from the line is not longer than 31 charachters*/
		if(strlen(instr->param1)>LABEL_SIZE)
		{
			/*print a massage and return ERROR*/
			fprintf(stderr,"In file %s\nIn line %d, the label is to long\n",name,lineNum);
			return ERROR;
		}
		/*check if there is already a node in the list*/
		if(head == NULL)
		{
			/*get location for the node*/
			head = (label *)malloc(sizeof(label));
			/*check if we got location seccusfully,if not print a masage and return FATAL_ERROR*/
			if(head == NULL)
			{
				fprintf(stderr,"Not enough space \n");
				
				return FATAL_ERROR;
			}
			/*set the head to current*/
			current = head;
			*la = head;
		}
		else
		{
			/*initalize current to head for the loop*/
			current = head;
			/*loop until the last node*/
			while(current->next != NULL)
			{
				current = current->next;
			}
			/*get space from malloc*/
			newNode = (label *)malloc(sizeof(label));
			/*check if success*/
			if(newNode == NULL)	
			{
				fprintf(stderr,"Not enough space \n");
				return FATAL_ERROR;
			}
			/*set the last node to the new node*/
			current->next = newNode;
			/*set current to be the new node*/
			current = current->next;
		}
		if(strlen(instr->param1)>= LABEL_SIZE)
		{
			fprintf(stderr,"IN file %s, In line %d, %s cant be a parameter",name,lineNum,instr->param1);
			return ERROR;
		}
		/*coppy the name of the parameter to the node name*/
		strcpy(current->name,instr->param1);
		/*save the current index of the parameter in the node*/
		current->addres = *cnt;
		/*set the next to NULL for later checks*/
		current->next = NULL;

	}
	(*cnt)++;
	return OK;
}

/*check for prn*/
/*this function gets a file name,  line number, array of chot, index to the array, and a pointer to a pounter to struct*/
/*the function check the quantity of parameters and initalize the parametes in the array or in the label struct*/
int checkOneParam2(char *name,int lineNum,short *array,int *cnt,cmdLine *instr,label **la)
{
	
	/*varaibles for the linked list of unknown labels*/
	label *head = *la,*current = *la,*newNode = NULL;

	/*check if there is more or less than one parameter and print a massage,and return ERROR*/
	if((instr->cnt) > MAX_1)
	{
		fprintf(stderr,"In file %s\n,In line %d, there is an extra parameter\n",name,lineNum);
		return ERROR;
	}
	if((instr->cnt)<MAX_1)
	{
		
		fprintf(stderr,"In file %s\n,In line %d, you missed a parameter\n",name,lineNum);
		return ERROR;
	}
	/*in the current value, save the command number from the 5 bit*/
	insertSym(array,*cnt,instr->sign2,instr->sign1,instr->comNum);
	/*continue to next index*/
	(*cnt)++;
	/*check if its a digit*/
	if(instr->sign1 == DIGIT_FLAG)
	{
		/*insert the digit from the 2 bit*/
		array[*cnt] = MOVE_DEST(instr->value1);
	}
	/*check if this a register*/
	else if(instr->sign1 == REG_FLAG)
	{
		/*insert the register value from the 7 bit*/
		array[*cnt] |= FIRST_REG(instr->value1);
	}
	/*if this is a label*/
	else
	{
		/*check the length of the label*/
		if(strlen(instr->param1)>=LABEL_SIZE)
		{
			/*print a massage and return ERROR*/
			fprintf(stderr,"In file %s\nIn line %d, the label is to long\n",name,lineNum);
			return ERROR;
		}
		/*check if the head is null*/
		if(head == NULL)
		{
			/*get location for the head*/
			head = (label *)malloc(sizeof(label));
			/*check success*/
			if(head == NULL)
			{
				fprintf(stderr,"Not enough space \n");
				return FATAL_ERROR;
			}
			/*set the head in the pointer*/
			*la = head;
			/*set the current to head*/
			current = head;
		}
		else
		{
			/*set the current to head*/
			current = head;
			/*move until the last node*/
			while(current->next != NULL)
			{
				current = current->next;
			}
			/*get space for newNode*/
			newNode = (label *)malloc(sizeof(label));
			/*check succuess*/
			if(newNode == NULL)
			{
				fprintf(stderr,"Not enough space \n");
				return FATAL_ERROR;
			}
			/*set the last next to be the new node*/
			current->next = newNode;
			/*set the current to be the last*/
			current = current->next;
			/*set the next to null*/
			current->next = NULL;
		}
		/*coppy the name of the parameter*/
		strcpy(current->name,instr->param1);
		/*get the index where the address should go in the current node*/
		current->addres = *cnt;
		/*set the next to null*/
		current->next = NULL;
	}
	/*increase the counter*/
	(*cnt)++;
	return 0;
}

/*check for lea*/
/*this function checks parameters for the command lea, the first parameter can only be a label, and the second parameter cant be a digit*/	
int checkTwoParam1(char *name,int lineNum,short *array,int *cnt,cmdLine *instr,label **la)
{

	/*set label varaibles to save the label in the function*/
	label *head = *la,*current = *la,*newNode = NULL;
	/*i for the while loop*/
	int i = OK;
	/*error flag*/
	int error = OK;
	/*check if there is enough parameters*/
	if(instr->cnt < MAX_2)
	{
		/*if not print a massage and return ERROR*/
		fprintf(stderr,"In file '%s'\nIn line %d, missing a parameter\n",name,lineNum);
		return ERROR;
	}
	/*check if the first parameter is a label*/
	if(instr->sign1 != LABEL_FLAG)
	{
		/*if not, print a massage and return error*/
		fprintf(stderr,"In file '%s'\nIn line %d, first parameter can only get a label\n",name,lineNum);
		return ERROR;
	}
	/*insert the values in the word index*/
	insertSym(array,*cnt,instr->sign2,instr->sign1,instr->comNum);
	/*increase the counter*/
	(*cnt)++;
	/*while i < 2*/
	while(i<MAX_1)
	{
		/*if we check the second parameters and it is not a label break the loop*/
		if(i && instr->sign2 != LABEL_FLAG)
		{
			break;
		}
		/*check if the head is NULL*/
		if(!head)
		{
			/*get space from malloc*/
			head = (label *)malloc(sizeof(label));
			/*check if successs*/
			if(head == NULL)
			{
				/*print a massage and return FATAL_ERROR*/
				fprintf(stderr,"Not enugh space \n");
				return FATAL_ERROR;
			}
			/*set the current to head*/
			current = head;
			/*set the pointer got as parameter to head*/
			*la = head;
			/*initialize the next node to null*/
			current->next = NULL;
		}
		else
		{
			/*loop until the next node*/
			while(current->next != NULL)
			{
				current = current->next;
			}
			/*get fro malloc space for the new node*/
			newNode = (label *)malloc(sizeof(label));
			/*check secuss*/
			if(newNode == NULL)
			{
				/*print a massage and return FATAL_ERROR*/
				fprintf(stderr,"Not enugh space \n");
				return FATAL_ERROR;
			}
			/*set the current next to the new node*/
			current->next = newNode;
			/*set the current to the next*/
			current = current->next;
			current->next = NULL;
		}
		/*check if its the first parameter*/
		if(!i)
		{
			/*check if the size of the parameter is longer than the size of label*/
			if(strlen(instr->param1)>=LABEL_SIZE)
			{
				/*print a massage and initialize the error flag*/
				fprintf(stderr,"In file %s\nIn line %d, '%s' cant be a parameter\n",name,lineNum,instr->param1);
				error = ERROR;
			}
			else
			{
				/*coppy the param name to the node*/
				strcpy(current->name,instr->param1);
				/*save the index of the label*/
				current->addres = *cnt;
				

			}
			
		}
		/*increase i*/
		i++;
	}
	/*increase the index*/
	(*cnt)++;
	/*check if the second parameter is a digit*/
	if(instr->sign2 == DIGIT_FLAG)
	{
		/*print a massage and set the error flag*/
		fprintf(stderr,"In file %s\nIn line %d, the parameter cant be a digit\n",name,lineNum);
		error = ERROR;
	}
	/*check if its a label*/
	else if(instr->sign2 == LABEL_FLAG)
	{
		/*check if the size of the parameter is longer than a label name*/
		if(strlen(instr->param2)>=LABEL_SIZE)
		{
			/*print an error and set the error flag*/
			fprintf(stderr,"In file %s\nIn line %d, '%s' cant be a parameter\n",name,lineNum,instr->param1);
			error = ERROR;
		}
		
		else
		{
			
			/*coppy the label name to the current node*/
			strcpy(current->name,instr->param2);
			/*set the index in the label node*/
			current->addres = *cnt;
		}
		current->next = NULL;
		
	}
	
	/*check if its a register*/
	else if(instr->sign2 == REG_FLAG)
	{
		/*set the value of the register in the current index from the xecond bit*/
		array[*cnt] |= FIRST_REG(instr->value2);
	}
	/*increase the counter*/
	(*cnt)++;
	/*return error*/
	return error;
}	

/*this function checks the parameters for cmp command*/
int checkTwoParam2(char *name,int lineNum,short *array,int *cnt,cmdLine *instr,label **la)
{
	/*set varaibles for the labels in the line*/
	label *head = *la,*current = *la,*newNode = NULL;
	int error = OK;
	/*check how many parameters*/
	if(instr->cnt < MAX_2)
	{
		/*print a massage and return error*/
		fprintf(stderr,"In file '%s'\nIn line %d you missed a parameter\n",name,lineNum);
		return ERROR;
	}
	insertSym(array,*cnt,instr->sign2,instr->sign1,instr->comNum);
	/*increase the counter*/
	(*cnt)++;
	/*check if th first parameter is a digit*/
	if(instr->sign1 == DIGIT_FLAG)
	{
		/*insert the value of the number from the second bit in the current index*/
		array[*cnt] |= MOVE_DEST(instr->value1);
	}
	/*if this is a register*/
	else if(instr->sign1 == REG_FLAG)
	{
		/*insert the value of the register to the 7 bit*/
		array[*cnt] |= FIRST_REG(instr->value1);
		/*if the second parameter is also a register*/
		if(instr->sign2 == REG_FLAG)
		{
			/*insert the value of the second symbol to the 2 bit in the current counter*/
			array[*cnt] |= MOVE_DEST(instr->value2);
			/*increase the counter*/
			(*cnt)++;
			return OK;
		}
	}
	/*this is a label*/
	else
	{
		/*check the size of he parameter*/
		if(strlen(instr->param1) > LABEL_SIZE)
		{
			/*if its to long, set the error flag*/
			fprintf(stderr,"In file %s\nIn line %d, %s cannot be a label\n",name,lineNum,instr->param1);
			error = ERROR;
		}
		else
		{
			/*check if the list is empty*/
			if(head == NULL)
			{
				/*initalize the head with malloc*/
				head = (label *)malloc(sizeof(label));
				/*check if we got space, if not return fatal error*/
				if(head == NULL)
				{
					fprintf(stderr,"Not enough space\n");
					return FATAL_ERROR;
				}
				/*set the pointer got as parameter to the head addres*/
				*la = head;
				/*set the current to head for later use*/
				current = head;
				/*set the next to NULL*/
				current->next = NULL;
			}
			else
			{
				/*while loop until the last node*/
				while(current->next != NULL)
				{
					current = current->next;
				}
				/*get space for new node varaible*/
				newNode = (label *)malloc(sizeof(label));
				/*check if null, if it do print a massage and return fatal error*/
				if(newNode == NULL)
				{
					fprintf(stderr,"Not enough space\n");
					return FATAL_ERROR;
				}
				/*set the next of current to the new node*/
				current->next = newNode;
				/*set current to be te next*/
				current = current->next;
				/*set the last node to null*/
				current->next = NULL;
			}
			/*coppy the parameter to the string in the current*/
			strcpy(current->name,instr->param1);
			/*save the index in the node*/
			current->addres = *cnt;
		}
	}
	/*increase the counter*/
	(*cnt)++;
	/*check if the second parameter is a digit*/
	if(instr->sign2 == DIGIT_FLAG)
	{
		/*insert the value in the second index*/
		array[*cnt] |= MOVE_DEST(instr->value2);
	}
	/*if this is a register*/
	else if(instr->sign2 == REG_FLAG)
	{
		/*in the  current index insert the value of the register in the second bit*/
		array[*cnt] |= MOVE_DEST(instr->value2);
	}
	else
	{
		/*same checks as last parameter, but with from the struct we check and get the param2 varaibles*/
		if(strlen(instr->param2) > LABEL_SIZE)
		{
			fprintf(stderr,"In file %s\nIn line %d, %s cannot be a label\n",name,lineNum,instr->param2);
			error = ERROR;
		}
		else
		{
			if(head == NULL)
			{
				head = (label *)malloc(sizeof(label));
				if(head == NULL)
				{
					fprintf(stderr,"Not enough space\n");
					return FATAL_ERROR;
				}
				*la = head;
				current = head;
				current->next = NULL;
			}
			else
			{
				while(current->next != NULL)
				{
					current = current->next;
				}
				newNode = (label *)malloc(sizeof(label));
				if(newNode == NULL)
				{
					fprintf(stderr,"Not enough space\n");
					return FATAL_ERROR;
				}
				current->next = newNode;
				current = current->next;
				current->next = NULL;
			}
			strcpy(current->name,instr->param2);
			current->addres = *cnt;
		}
	}
	/*increase the counter*/
	(*cnt)++;
	return error;	
	
}

/*a function that check tha parameters for the commands: mov add sub*/
int checkTwoParam3(char *name,int lineNum,short *array,int *cnt,cmdLine *instr,label **la)
{
	/*set varaibles for the labels in the line*/
	label *head = *la,*current = *la,*newNode = NULL;
	/*set an error flag*/
	int error = OK;
	/*increase the index*/
	
	/*check if there is a legal number of parameters*/
	if(instr->cnt <MAX_2)
	{
		/*print a massage and return error*/
		fprintf(stderr,"In file %s\nIn line %d you missed a parameter\n",name,lineNum);
		return ERROR;
	}
	insertSym(array,*cnt,instr->sign1,instr->sign2,instr->comNum);
	(*cnt)++;
	/*check if its a digit*/
	if(instr->sign1 == REG_FLAG)
	{
		/*set the value of the register in the current index from bit 7*/
		array[*cnt] |= FIRST_REG(instr->value1);
		/*check if the second parameter is also a register*/
		if(instr->sign2 == REG_FLAG)
		{
			/*set the value of the second register to the 2 bit in the current indes*/
			array[*cnt] |= MOVE_DEST(instr->value2);
			/*increase the counter and return the flag*/
			(*cnt)++;
			return error;
		}
	}
	/*if the parameter is digit*/
	else if(instr->sign1 == DIGIT_FLAG)
	{
		/*insert the value of the parameter in the current index from the second bit*/
		array[*cnt] |= MOVE_DEST(instr->value1);
	}
	else
	{
		/*check if the size is to big*/
		if(strlen(instr->param1) > LABEL_SIZE)
		{
			/*print an error and set the error flag*/
			fprintf(stderr,"In file %s\nIn line %d, %s cannot be a label\n",name,lineNum,instr->param1);
			error = ERROR;
		}
		else
		{
			/*check if the head is null*/
			if(head == NULL)
			{
				/*get space for the head with malloc*/
				head = (label *)malloc(sizeof(label));
				if(head == NULL)
				{
					/*print a massage and return fatal_error*/
					fprintf(stderr,"Not enough space\n");
					return FATAL_ERROR;
				}
				/*set the pointer got as parameter to head*/
				*la = head;
				/*set the current to head*/
				current = head;
				/*set the next to null*/
				current->next = NULL;
			}
			else
			{
				/*while loop until the last node*/
				while(current->next != NULL)
				{
					current = current->next;
				}
				/*get space with newNode varaible*/
				newNode = (label *)malloc(sizeof(label));
				/*if faulire, print a massage and return fatal_Error*/
				if(newNode == NULL)
				{
					fprintf(stderr,"Not enough space\n");
					return FATAL_ERROR;
				}
				/*set the next current to the new node*/
				current->next = newNode;
				/*set current to be his next*/
				current = current->next;
				/*set the next to null*/
				current->next = NULL;
			}
			/*coppy the parameter to the current name*/
			strcpy(current->name,instr->param1);
			/*save the current index*/
			current->addres = *cnt;
		}
	}
	/*increase the counter*/	
	(*cnt)++;
	/*check if the second parameter is a digit*/
	if(instr->sign2 == DIGIT_FLAG)
	{
		/*print a massage and return error*/
		fprintf(stderr,"In file %s\nIn line %d,THis cuntoion cant get a digit as a second argument\n",name,lineNum);
		return ERROR;
	}
	/*check if it is a register*/
	else if(instr->sign2 == REG_FLAG)
	{
		/*set the register value in the current indexs from the second bit*/
		array[*cnt] |= MOVE_DEST(instr->value2);
	}
	/*same checks as last label parameter but with the data of the second, and the symbol is inserted to the 2 bit*/
	else
	{
		if(strlen(instr->param2) > LABEL_SIZE)
		{
			fprintf(stderr,"In file %s\nIn line %d, %s cannot be a label\n",name,lineNum,instr->param2);
			error = ERROR;
		}
		else
		{
			if(head == NULL)
			{
				head = (label *)malloc(sizeof(label));
				if(head == NULL)
				{
					fprintf(stderr,"Not enough space\n");
					return FATAL_ERROR;
				}
				*la = head;
				current = head;
				current->next = NULL;
			}
			else
			{
				while(current->next != NULL)
				{
					current= current->next;
				}
				newNode = (label *)malloc(sizeof(label));
				if(newNode == NULL)
				{
					fprintf(stderr,"Not enough space\n");
					return FATAL_ERROR;
				}
				current->next = newNode;
				current = current->next;
				current->next = NULL;
			}
			strcpy(current->name,instr->param2);
			current->addres = *cnt;
		}
	}
	/*increase the counter*/
	(*cnt)++;
	return error;	
		
}
/*insert in the corrent index of the array the numbers*/
void insertSym(short *arr,int index,int addres,int send,int com)
{
	/*insert the numeric value of the command*/
	arr[index] |= MOVE_COM(com);
	/*insert the type of the sending varaible*/
	arr[index] |= MOVE_DEST(send);
	/*insert the rype of the getting varaible*/
	arr[index] |= MOVE_ORG(addres);
}

		

		
		
	
		
		
		
		
		
		
		
		
	
