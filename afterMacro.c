/*This file is used to create the functions that are used to open files for the assembler procces*/
/*include the standard library and a header file which is in the folder*/
#include "macroLib.h"
/*initialize a macro string*/
#define MACRO "mcro"
/*search for a comment*/
#define COMMENT ';'
#define MACRONAME 77
#define MACRO_DEF_SIZE 4
#define ADD_NAME_LENGTH 4
#define NEW_ENDING ".as"
#define LAST_AFTER_AS 'm'
/*a function that gets a file and a file name, change the file ending to .am and open the file to write in, search for a macros definition and replace them where its needed, in this function, we assume that the macros are defined correctly, as it says in the instructions of the assembler*/
int afterMacro(FILE **ofile,char *fname)
{
	/*varaibles for the loops in the program, check varailble to check if there is an error*/
	int i = 0,check, lineNum = 0;
	/*line with max length of 81 as it says in the instructions, a pointer for the strstr function*/
	char line[LINELENGTH], *mcroname,*filename,*token,error = 0;
	/*file pointer to open the necaccary files,fmac for the after macros file*/
	FILE *fmac,*f;
	/*varaibles to get the macro definitions in a linked list*/
	node *head = NULL,*current,*newNode = NULL;
	/*get a location for the new file name and check if it succed*/
	filename=(char *)malloc((strlen(fname)+ADD_NAME_LENGTH)*sizeof(char));
	if(filename == NULL)
	{
		fprintf(stderr,"Not enogh space for file name\n");
		return FATAL_ERROR;
	}
	/*add the .as ending and coppy the name thatgot in the function*/
	strcpy(filename,fname);
	strcat(filename,NEW_ENDING);
	/*open the .as file for reading and check*/
	f = fopen(filename,"r");
	if(f == NULL)
	{
		fprintf(stderr,"couldnt open file for the pre assembler stage \n");
		free(filename);
		return FATAL_ERROR;
	}
	
	
	/*while loop to collect all the macro definitions in the original file*/
	while(fgets(line,LINELENGTH,f))
	{
		/*find a micro definition*/
		if((mcroname = strstr(line,MACRO)) != NULL)
		{
			/*initialize i to 0*/
			i=0;
			/*loop until we get to the pointer of the mcro flag and check if there is anything else except white spaces*/
			while(&line[i]  != mcroname)
			{
				if(line[i] != TAB && line[i] != SPACE)
				{
					/*print a massage and set error to !OK to continue with the other macros*/
					fprintf(stderr,"in line %d Undifined char before macro : '%c' \n",lineNum,line[i]);
					error = !OK;
				}
				i++;
			}
			/*set the node varaible to NULL for later check*/
			newNode = NULL;
			/*use strtok from the start of the mcro flag until the end of line, token by white spaces*/
			token = strtok(mcroname,TOKEN_WHITE);
			/*while we didnt get to end of line*/
			while(token != NULL)
			{
				/*check if the token is empty*/
				if(strlen(token) > 0)
				{
					/*check if the token is the mcro flag*/
					if(strcmp(token,MACRO) != 0)
					{
						/*check if we initialize a node yet*/
						
						if(!newNode)
						{
							/*get a space for a new node*/
							newNode =(node *)malloc(sizeof(node));
							/*check if there is space*/
							if(!newNode)
							{
								fprintf(stderr,"Not enogh space \n");
								freeMacro(&head);
								return FATAL_ERROR;
							}
							newNode->comm = NULL;
							/*copy the name to the nodes name*/
							strcpy(newNode->name,token);
							
							/*check if the name is legal and print a massage if not*/
							if(checkLegalName(token) <=COM_ERROR)
							{
								fprintf(stderr,"in line %d, the name of the macro '%s' is a saved name of assembly \n",lineNum,token);
								error = !OK;
							}
							/*check if the name is legal and print a massage if not*/
							if(checkName(token))
							{
								fprintf(stderr,"In line %d , the name '%s' is ilegal, please chose another name \n",lineNum,token);
								error = !OK;
							}
							/*save the command in the comm section in the node and check if there was a critical error*/
							if((check = getComm(&(newNode->comm),f)) == FATAL_ERROR)
							{
								freeMacro(&head);
								return FATAL_ERROR;
							}
							/*check if there was an error and continue*/
							else if(check == ERROR)
							{
								error = ERROR;
							}
							

						
						}
						/*if we got here, it means the user used another name after the first name, so we print a massage and set the error flag*/
						else
						{
							fprintf(stderr,"in line : %d please define only one name, '%s' is wrong \n",lineNum,token);
							error = !OK;
							
						}
						
					}
				}
				/*continue with the token*/
				token = strtok(NULL,TOKEN_WHITE);
				
			}
			/*if the head of the list hasnt been initalized, set the new node to head and to current*/
			if(!head)
			{
				head = newNode;
				current = newNode;
			}
			/*else set the new node to the next of the current, and current to the next*/
			else
			{
				current->next = newNode;
				current = current->next;
			}
			current->next = NULL;
			
		}
		/*increase the line number*/
		lineNum++;
	
	}
	/*go to the start of the original file*/
	rewind(f);
	/*check if we got an error in the mcro definitions*/
	if(error)
	{
		/*free the mcro nodes*/
		freeMacro(&head);
		/*free the file name*/
		free(filename);
		/*close the file and set the pointer to NULL for the main function*/
		fclose(f);
		*ofile = NULL;
		return ERROR;
	}
	/*change the last char to m for the .am file and open the file*/
	filename[strlen(filename) - ERROR] = LAST_AFTER_AS;
	fmac = fopen(filename,"w");
	
	/*check if the file opened seccesfuly*/
	if(fmac == NULL)
	{
		fprintf(stderr,"couldnt open file for the pre assembler stage \n");
		return FATAL_ERROR;
	}
	/*set the current back to the head*/
	current = head;
	while(fgets(line,LINELENGTH,f))
	{
		/*check if there is a nacro flag*/
		
		if(strstr(line,MACRO))
		{
			/*loop until there is an endmacro flag*/
			while(!(strstr(line,END_MACRO)))
			{
				fgets(line,LINELENGTH,f);
			}
			continue;
			

		}
		/*loop in the linked list until the last node*/	
		while(current != NULL)
		{
			if(strstr(line,current->name))
			{
				/*print the name on the macro file*/
				fprintf(fmac,"%s",current->comm);
				/*break the loop so the next if will know there was a macro there*/
				break;
				
			}
				/*set the current to his next until the loop is over*/	
			current = current->next;
			
		}
		/*check if we got to the end of the node,if we are it means there are no macro declaration and we print the line as it is*/
		if(current == NULL)
		{
			fprintf(fmac,"%s",line);
		}
		current = head;
	
	}
	/*close the original file*/
	fclose(f);
	/*close the after macro file and open again for reading*/
	fclose(fmac);
	/*open the after macro file for reading*/
	fmac = fopen(filename,"r");
	if(fmac == NULL)
	{
		fprintf(stderr,"couldnt open file for the pre assembler stage \n");
		return FATAL_ERROR;
	}
	/*set the after macro file to the file that got as a parameter*/
	*ofile = fmac;
	/*free all the macro commands and the nodes*/
	freeMacro(&head);
	/*free the file name*/
	free(filename);
	return 0;
}
					
	
	
	
