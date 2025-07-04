#include "macroLib.h"
#include <math.h>
#define S_LARGE(X) (X) + 1
/*macros for the command sizes*/
#define PARA_SIZE 3
#define FIRST_P 1
#define SECOND_P 2
/*macro fir the sizes of the string labels and the data labels*/
#define S_SIZE 7
#define D_SIZE 5
/*a macro to check if a digit is 10 bits long*/
#define CHECK_DIGIT_10(X) ((X) <(-512)||(X) > (511))?1:0
/*a macro to check if a digit is 12 bits long*/
#define CHECK_DIGIT_12(X) ((X)<(-2048)||(X)> 2047)?1:0


/*this function gets the line pointer, a pointer for a pointer to an array, a pointer for the ccounter for the data, the current line number and name of the file*/
/*the function use strtok and check if each part of the line is correct, it check the parameters by commas and if they are illigal number*/
/*not that the number can only be with 10 bits so we check if the number is to big for the compiler*/
/*the function return 0 if the function succeded or not*/
int getIntData(char *line, short **arr,int *cnt,int *amount, int lineNum, char *name)
{
	/*initalize varaibles*/
	/* cnt to store the values in the array, erro  
	\
	r flag, and a checkData flag to check how many times they wrote data*/
	int error = OK, checkData = OK;
	/*initialize pointer to malloc and realloc functions*/
	short *dataArr = *arr, *newArr;
	/*count the commas*/
	int cntComa = countCommas(line);
	/*total to check how many indexes we supposed to get*/
	int total = ++cntComa;
	/*token for strtok, get for strtod*/
	char *token, *get;
	/*dl to get the value from strtod*/
	double dl;
	/*initalize a data counter*/
	int countDa = OK;
	/*check if the data hasnt been initailzed yet*/
	if (dataArr == NULL)
	{
		/*use malloc for alocation*/
		dataArr = (short *)malloc((*amount) * sizeof(short));
		/*check if there wasnt place, if so print a massage*/
		if (dataArr == NULL)
		{
    			fprintf(stderr, "Not enough space\n");
		    	return FATAL_ERROR;
		}
	}
	/*if there was already a pointer*/
	else if((*cnt + total) >= *amount)
	{

		/*use realloc for data allocation*/
        	newArr = (short *)realloc(dataArr, (*amount = (*amount) + total) * sizeof(short));
        	/*check if there was space*/
        	if (newArr == NULL)
        	{
        	        fprintf(stderr, "Not enough space\n");
			return FATAL_ERROR;
        	}
        	/*initalize the realloc pointer to the data pointer for later use*/
        	dataArr = newArr;
        	
    	}
    	/*initalize total to the size of the *size*/
    	total = *cnt;
	/*use str tok and separate the line by white spaces, new line and commas*/
    	token = strtok(line, TOK_LINE);
    	/*while loop until the end of the string*/
    	while (token != NULL)
    	{
    		/*check if the length of the token larger than 0*/
        	if (strlen(token) > OK)
        	{
        		/*if we identified the .data sub string count it in checkData*/
            		if (strstr(token, DATA))
            		{
            			/*check if the .data is defined wrong by size*/
            			if(strlen(token) > D_SIZE)
            			{
            				fprintf(stderr,"In file %s,In line %d, there is an illegal definition : '%s'\n",name,lineNum,token);
            				error = ERROR;
            				return ERROR;
            			}
                		checkData++;
            		}
            		else
            		{
            			/*initalize get to null*/
                		get = NULL;
                		/*use strtod on the corret token*/
                		dl = strtod(token, &get);
                		/*check if get is not the null index in the substring*/
               			if (get != token + strlen(token))
                		{
                			/*if it is pront a massage and initialize the error flag*/
                    			fprintf(stderr, "In file '%s'\n line %d, there is an illegal number in the data instruction: '%s'\n", name, lineNum, token);
                    			error = ERROR;
                		}
                		countDa++;
                		/*check if there is more values than allocated place*/
                		if (countDa > cntComa)
                		{
                			/*print a massage that the user missed a comma*/
                    			fprintf(stderr, "In file '%s'\n line %d, you missed a comma in the data definition\n", name, lineNum);
                    			/*set the error flag and break the loop*/
                    			error = ERROR;
                    			break;
                		}
                		
                		/*check if the numbr is larger than 10 bits*/
                		if(CHECK_DIGIT_12(dl))
                		{
                			/*print a massage and set the error flag*/
                    			fprintf(stderr, "In file '%s'\n line %d, the number %d is illegal\n", name, lineNum, (int)dl);
                    			error = ERROR;
                    			
                    			
                		}
                		else
                		{
                			/*initalize the number to the location in the line*/
                			dataArr[*cnt] = (short)dl;
                		
                			/*increase cnt again*/
                			(*cnt)++;
                		}
            		}
        	}
        	/*move to the next token*/
        	token = strtok(NULL, TOK_LINE);
    	}
    	/*initalize the location of dataArr to the first string*/
	*arr = dataArr;
	/*check if there was more than 1 definitions of data*/
    	if (checkData > ERROR)
    	{	
    		/**print a massage and set the error flag*/
        	fprintf(stderr, "In file '%s', line %d, you wrote '.data' more than once\n", name, lineNum);
        	error = ERROR;
    	}
	/*check if there is less data than supposed to be*/
    	if (countDa < cntComa)
    	{	
    		/*print a massage and set the error flag*/
        	fprintf(stderr, "In file '%s'\n line %d, there is a spare comma or the instruction is empty\n", name, lineNum);
        	error = ERROR;
    	}
	/*return the error flag*/
   	return error;
}

/*this function gets a name of a file, a line string, a line number, a pointer for a pointer for label,and instruction string*/
/*the function check the legality of the labels and print the error massages to stderr, if there arent any, the function sets the pointer to the new linked list*/
int getLabels(char *name,char *line, int lineNum,label **la,char *instr)
{
	/*set varaibles to count the commas, error flag, and counter of labels*/
	int cntComma,error = OK,cntE = OK;
	/*token varaible to tokenize the line*/
	char *token;
	/*label definition to get the label list*/
	label *head = NULL,*current,*newNode;
	/*counter for the instruction*/
	int countIns = OK;
	/*count how manny commas in the line*/
	cntComma = countCommas(line);
	/*tokenize the string by white spaces, comma and new line*/
	token = strtok(line,TOK_LINE);
	while(token != NULL)
	{
		/*if the substring is not empty*/
		if(strlen(token) > 0)
		{
			/*if the substring is not .extern*/
			if(strcmp(token,instr) != 0)
			{
				/*check if the chars are legal*/
				if(checkName(token))
				{
					/*print a massage and update the error flag and continue to next token*/
					fprintf(stderr,"In file %s \nin line %d, the %s label ' %s ' is illegal\n",name,lineNum,instr,token);
					error = ERROR;
						
				}
				/*check if the name is a saved name of assembly*/
				else if(checkLegalName(token)<=COM_ERROR)
				{
					/*print a massage and update the error flag and continue*/
					fprintf(stderr,"In file %s \nin line %d, the %s label ' %s ' is illegal\n",name,lineNum,instr,token);
					error = ERROR;
							
				}
				/*if the name is legal*/
				else
				{
					/*use malloc for new node location*/
					newNode = (label *)malloc(sizeof(label));
					/*check if there was memory for the node*/
					if(newNode == NULL)
					{
						fprintf(stderr,"Not enough space\n");
						return FATAL_ERROR;
					}
					/*coppy the name of the token to the node*/
					strcpy(newNode->name,token);
					/*count the node to check legality of commas*/
					cntE++;
					/*if the head is null*/
					if(head == NULL)
					{
						/*set the head and the current to the new Node*/
						head =	newNode;
						current = newNode;
						/*set the next of the node to NULL*/
						current->next = NULL;
					}
					else
					{
						/*initalize the next node to the new node*/
						current->next = newNode;
						/*initialize current to be the new node*/
						current = current->next;
						/*initialize the next node to NULL*/
						current->next = NULL;
					}
				}
			}
			else
			{
				countIns++;
			}
		}
		
		/*continue to next token*/
		token = strtok(NULL,TOK_LINE);
	}
	if(countIns > ERROR)
	{
		fprintf(stderr,"In file %s\nIn line %d tou wrote %s more than once\n",name,lineNum,instr);
		error = ERROR;
	}
	/*check if the number of nodes is larger than the number of commas*/
	if((cntE - cntComma) > ERROR && !error)
	{
		/*print a massage and initialize the error flag*/
		fprintf(stderr,"in file %s \nin line %d, you missed a comma in the %s definition \n",name,lineNum,instr);
		error = ERROR;
	}
	/*check if the number of nodes is smalled than the number of commas*/
	if((cntE - cntComma) < ERROR && !error)
	{
		/*print a massage and initialize the error flag*/
		fprintf(stderr,"in file %s \nin line %d,there is not a label or there is a spare comma in the %s definition\n",name,lineNum,instr);
		error = ERROR;
	}
	/*set the node that was sent to the head of the new node*/
	*la = head;
	return error;
}

/*function getString, gets a pointer for the line, a pointer to pointer for the to save the strings, a line number and a name of file*/
/*this function gets a line, check how many strings there is in the line, and if they are legal and save them in he data array*/
int getString(char *line,short **arr,int *cnt,int *amount,int lineNum,char *name)
{
	/*iniaitalize varaibles*/
	/*newValue for realloc, saveData for malloc and to save the addres from the parameter, token for strtok*/
	short *newValue,*saveData = *arr;
	char *token,*mid;
	/*a size counter end error flag*/
	int error = OK;
	/*stringCnt to count how many .string flags, countComa to count the commas*/
	int stringCnt = OK,countComa = countCommas(line);
	/*countS to count how many strings*/
	int countS = OK;
	/*save the new size for realloc*/
	int newSize = SIZE, i = OK;
	countComa++;
	/*check if the array is nukk*/
	if(saveData == NULL)
	{
		/*get space for the array */
		saveData = (short *)malloc((*amount = SIZE) * sizeof(short));
		/*check secces and print a massage*/
		if(saveData == NULL)
		{
			fprintf(stderr,"Not enough space \n");
			return FATAL_ERROR;
		}
	}
	/*token the line by commas and white space*/
	token = strtok(line,TOK_LINE);
	/*loop until the last token*/
	while(token != NULL)
	{
		/*if the length of the token is larger than 0*/
		if(strlen(token)>OK)
		{
			/*check if in the token there is a string definition*/
			if(strstr(token,STRING))
			{
				/*check if the definition is wrong and print a massage*/
				if(strcmp(token,STRING) != OK)
				{
					fprintf(stderr,"In file '%s'\nIn line %d there is a wrong definition of .string\n",name,lineNum);
				}
				/*count the string definition*/
				stringCnt++;
			}
			else
			{
				/*check if the first and last chars are "*/
				if(token[OK] != ST_SYM || token[strlen(token) - 1] != ST_SYM || strlen(token) == ERROR)
				{
					/*if not print a massage and set the error flag*/
					fprintf(stderr,"In file %s\nIn line %d there is an illegal string \n",name,lineNum);
					error = ERROR;
				}
				else
				{
					/*calculate the new size*/
					newSize = strlen(token) + (*cnt);
					/*increase the size by one for the null charachter*/
					newSize++;
					/*check if the new size is larger than the amount*/
					if(newSize > *amount)
					{
						/*get new space from realloc and set the amount varaible to the new size*/
						newValue = (short *)realloc(saveData,(*amount = newSize)*sizeof(short));
						/*check if succed and print a massage*/
						if(newValue == NULL)
						{
							fprintf(stderr,"Not enough space \n");
							return FATAL_ERROR;
						}
						/*set the new addres in the varaible saveData and in the pointer*/
						saveData = newValue;
						*arr = saveData;
							
					}
					/*get the string without the first "*/
					mid = (++token);
					/*set i to 0*/
					i = OK;
					while(i<strlen(mid))
					{
						/*in the current index save the char value with casting to short*/
						saveData[*cnt] = (short)mid[i];
						/*increase i and cnt*/
						i++;
						(*cnt)++;
					}
					/*decrease cnt*/
					(*cnt)--;
					/*initalize 0 to mark the end of string*/
					saveData[*cnt] = OK;
					/*increase cnt and the string counter*/
					(*cnt)++;
					countS++;
					
				}
			}
		}
		/*continue to next token*/
		token = strtok(NULL,TOK_LINE);
	}
	/*save the new addres in the pointer*/
	*arr = saveData;
	/*check if there was to manny commas*/
	if(countS < countComa && !error)
	{
		fprintf(stderr,"In file %s\nIn line %d there is a spare comma\n",name,lineNum);
		error = ERROR;
	}
	/*check if there was to less commas*/
	if(countS>countComa && !error)
	{
		fprintf(stderr,"In file %s\nIn line %d missing a comma\n",name,lineNum);
		error = ERROR;
	}
	/*check if .string called more than 1 time*/
	if(stringCnt > ERROR)
	{
		fprintf(stderr,"In file %s\nIn line %d you cant write .string more than once\n",name,lineNum);
		error = ERROR;
	}
	return error;
	
}

int getParam(char *line,cmdLine **cm,char *name, int lineNum)
{
	/*set pointers to tokenize and check if its a digit*/
	char *token,*checkAd = NULL;
	/*count the commas*/
	int cntComa = countCommas(line);
	/*count how manny tokens, checkVal to check the functions, and error flag*/
	int countTok = OK,checkVal,error = OK;
	/*for strtod function*/
	double dl;
	/*use another for pointer for the pointer to the struct that got as a parameter*/
	cmdLine *cmd = *cm;
	/*use malloc for space*/
	cmd = (cmdLine *)malloc(sizeof(cmdLine));
	/*check if there is space*/
	if(cmd == NULL)
	{
		/*prnit a massage and return fatal error*/
		fprintf(stderr,"Not enough space \n");
		return FATAL_ERROR;
	}
	/*set the parameter counter to 0*/
	cmd->cnt = OK;
	/*set the signs of parameters to 0*/
	cmd->sign1 = EMPTY;
	cmd->sign2 = EMPTY;
	cmd->param1 = NULL;
	cmd->param2 = NULL;
	/*tokenize the line by white spaces and commas*/
	token = strtok(line,TOK_LINE);
	/*loop until the last token*/
	while(token != NULL)
	{
		/*check if the token is not empty*/
		if(strlen(token) > OK)
		{
			/*count the token*/
			countTok++;
			/*check if this is the command section*/
			
			if(cmd->cnt == OK)
			{
				/*get and check if this is a legal command*/
				if((checkVal = checkLegalName(token)) > LAST_COM)
				{
					/*print a massage and set the error flag to 1*/
					fprintf(stderr,"In file %s\nIn line %d, there is an undifined command: '%s'\n",name,lineNum,token);
					error = ERROR;
					break;
					
				}
				/*if not, set the number of the command in the struct*/
				else
				{
					cmd->comNum = checkVal;
					
				}
				/*check if the command is not exist*/
				if(error)
				{
					return error;
				}
				cmd->cnt++;
			}
			/*check if this is a parameter*/
			else if(cmd->cnt == FIRST_P)
			{
				
				/*get location to the parameter*/
				cmd->param1 = (char *)malloc(S_LARGE(strlen(token))*sizeof(char));
				/*check if we got location*/
				if(cmd->param1 == NULL)
				{
					/*if not, print a massage and return fatal error*/
					fprintf(stderr,"Not enough space\n");
					return FATAL_ERROR;
				}
				/*coppy the parameter*/
				strcpy(cmd->param1,token);
				/*check if this suppsued to be a regsiter but there is a probleem*/
				if((checkVal = checkRegister(cmd->param1,lineNum,name))==REG_FATAL)
				{
					/*set the error flag*/
					error = ERROR;
				}
				
				/*check if it is a register*/
				else if(checkVal != REG_ERROR)
				{
					/*save the number of the register*/
					cmd->value1 = checkVal;
					/*set the sign of the parameter to a register flag*/
					cmd->sign1 = REG_FLAG;
				}
				/*check if its a digit or a label*/
				/*WE ASSUME THAT IF THERE IS A CHAR IN THEIS SECTION, THIS IS A LABEL,IF NOT THAN THIS IS A DIGIT*/
				else
				{
					/*get the number in the string*/
					dl = strtod(token,&checkAd);
					/*check if it is a number*/
					if(*checkAd != '\0' )
					{
						/*check if this could be a label*/
						if(checkLegalName(token) <= COM_ERROR||checkName(token))
						{
							/*if not print a massage and initialize the error flag*/
							fprintf(stderr,"In file %s\nIn line %d this cant be a parameter : '%s'\n",name,lineNum,token);
							error = ERROR;
						}
						/*initalize the first parameter sign*/
						cmd->sign1 = LABEL_FLAG;
					}
					else
					{
						/*check if the digit is larger than 10 bits*/
						if(CHECK_DIGIT_10(dl))
						{
							/*print a massage and initalize the error flag*/
							fprintf(stderr,"In file %s\nIn line %d the number should be max 10 bits : %f\n",name,lineNum,dl);
							error = ERROR;
						}
						/*get the digit in the struct and asign the digit sign*/
						cmd->sign1 = DIGIT_FLAG;
						cmd->value1 = (int)dl;
					}
					checkAd = NULL;
				}
				/*set the next parameter to null and increase the parameter counter*/
				cmd->param2 = NULL;
				cmd->cnt++;
			}
			/*chec if it is the second parameter*/
			else if(cmd->cnt == SECOND_P)
			{
				/*the same checks we did in the first parameter*/
				cmd->param2 = (char *)malloc((S_LARGE(strlen(token)))*sizeof(char));
				if(cmd->param2 == NULL)
				{
					fprintf(stderr,"Not enough space\n");
					return FATAL_ERROR;
				}
				strcpy(cmd->param2,token);
				if((checkVal = checkRegister(cmd->param2,lineNum,name))==REG_FATAL)
				{
					error = ERROR;
				}
				else if(checkVal != REG_ERROR)
				{
					cmd->value2 = checkVal;
					cmd->sign2 = REG_FLAG;
				}
				else
				{
					dl = strtod(token,&checkAd);
					if(*checkAd != '\0')
					{
						if(checkLegalName(token) <= COM_ERROR||checkName(token))
						{
							fprintf(stderr,"In file %s\nIn line %d this cant be a parameter : '%s'\n",name,lineNum,token);
							error = ERROR;
						}
						cmd->sign2 = LABEL_FLAG;
						
					}
					else
					{
						if(CHECK_DIGIT_10(dl))
						{
							fprintf(stderr,"In file %s\nIn line %d the number should be max 10 bits : %f\n",name,lineNum,dl);
							error = ERROR;
						}
						cmd->sign2 = DIGIT_FLAG;
						cmd->value2 = (int)dl;
						
					}
					checkAd = NULL;
				}
				cmd->cnt++;
				
			}
			
		}
		/*continue tokenizeing the string*/
		token = strtok(NULL,TOK_LINE);
	}
	/*check if there was more parameters than 2*/
	if(countTok > cmd->cnt)
	{
		fprintf(stderr,"In file %s\nIn line %d there is an extra parameter\n",name,lineNum);
		error = ERROR;
	}
	/*check if the command line has only a command or command and parameter with commas*/
	if((countTok == FIRST_P && cntComa) || (countTok == SECOND_P && cntComa))
	{
		/*print a massage and set the error flag*/
		fprintf(stderr,"In file %s\nIn line %d there is an extra comma\n",name,lineNum);
		error = ERROR;
	}
	/*check if there was a command with two parameters and if there is more or less than 1 coma*/
	if(countTok == 3 && cntComa != ERROR)
	{
		/*print a massage and set the error flag*/
		fprintf(stderr,"In file %s\nIn line %d the parameters should be saparated by 1 comma\n",name,lineNum);
		error = ERROR;
	}
	/*save the new pointer*/
	*cm = cmd;
	return error;
}

	
	
	
	
	
	
	
	
	


