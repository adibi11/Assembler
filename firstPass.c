#include "macroLib.h"
/*in this function we Will do the first pass on the assembley file*/
int firstPass(FILE *f,char *name)
{
	/*initalize a string to savethe line with fgets, a pointer to char to save what after the label and a char varaibel*/
	char line[LINELENGTH],*command,c;
	/*set error flags and checks, and line number*/
	int error = 0, lineNum = 1, i,checkVal;
	/*set varaible to 100*/
	int adr = FIRST_ADRESS;
	/*pointer to char to check the lines*/
	char *checkChar;
	/*define pointers for the label*/
	label *lHead = NULL,*lCurrent, *lNew = NULL;
	/*define pointers for externs*/
	label *eHead = NULL,*eCurrent = NULL, *eNew = NULL;
	/*define ponters for entries*/
	label *nHead = NULL, *nCurrent = NULL, *nNew =NULL;
	/*define varaible to save instruction line parameters*/
	cmdLine *node = NULL;
	label *labelInLine = NULL;
	/*define varaibles for the word array*/
	short *newArr = NULL,*insArr = NULL, amount = SIZE;
	int cnt = OK,mid;
	/*define varaible for instruction array */
	short *dataArr = NULL;
	int dataCnt = OK,dataAmount;
	/*define a linked list to save the data nodes*/
	label *dHead = NULL,*dCurrent;
	while(fgets(line,LINELENGTH,f))
	{	
		/*check if we got a whole line or it was to long*/
		if(line[strlen(line) - 1] != NEW_LINE)
		{
			/*if it is, print a massage, initalize the last char to new line to continue the check*/
			fprintf(stderr,"In file '%s' \n",name);
			fprintf(stderr,"In line %d , have more than 81 charachtas \n",lineNum);
			error = ERROR;
			line[strlen(line) - 1] = NEW_LINE;
			while((c = fgetc(f)) != NEW_LINE);
			
				
		}
		/*get the new line char addres*/
		if((checkChar = strchr(line,NEW_LINE)))
		{
			/*check if its an empty line*/
			if(checkBefore(checkChar,line) == OK)
			{
				/*if it is count the line and continue*/
				lineNum++;
				continue;
			}
		}
		/*check if there is a comment on the line*/
		if((checkChar = strchr(line,COMMENT)))
		{
			i = 0;
			/*loop until we get to the comment pointer and check if there is somthing written before*/
			while(&line[i] != checkChar)
			{
				if(line[i] != SPACE && line[i] != TAB)
				{
					fprintf(stderr,"In file '%s' \n",name);
					fprintf(stderr,"In line %d, a comment should be a full line \n", lineNum);
					error = ERROR;
					
					break;
				}
				i++;
			}
			lineNum++;
			continue;
		}
		/*check if there is a ':' , to get the label*/
		if((checkChar = strchr(line,LABEL_END)))
		{
			/*set the new label varaible to null and i to 0*/
			lNew= NULL;
			
			i = 0;
			/*check if there any white spaces before the label, if we do, skip them*/
			while ((line[i] == SPACE || line[i] == TAB) && &line[i] != checkChar)
			{
   				 i++;
			}
			
			/*check if the line starts with a comment*/
			if(&line[i] == checkChar)
			{
				fprintf(stderr,"In file '%s'\n",name);
				fprintf(stderr,"undifined ':' in line %d\n", lineNum);
				error = 1;
				continue;
			}
			/*ask for space from malloc to save the instruction after the label*/		
			command = (char *)malloc((strlen(checkChar) + 1) * sizeof(char));
			if(!command)
			{
				fprintf(stderr,"Not enogh space \n");
				freeAll(&lHead,&eHead,&nHead,&labelInLine,&dataArr,&insArr,&dHead);
				return FATAL_ERROR;
			}
			/*coppy the instrucrion after the ':' to the command pointer*/
			strcpy(command,checkChar + 1);
			/*set the ':' to null to continue manipulating the string*/
			*checkChar = '\0';
			/*check if the label is defined legal by charachters*/
			if(checkName(line + i))
			{
				fprintf(stderr,"In file '%s'\n",name);
				fprintf(stderr,"In line %d, there is an illegal label :' %s '\n",lineNum,line);
				error = ERROR;
			
			}
			/*check if the name is a savd name of assembly*/
			else if(checkLegalName(line + i) <= COM_ERROR)
			{
				fprintf(stderr,"In file '%s'\n",name);
				fprintf(stderr,"In line %d, the label ' %s ', is a saved name for assembley \n",lineNum, line);
				error = ERROR;
			}
			else
			{
				/*get a memmory for a new label from malloc*/
				lNew = (label *)malloc(sizeof(label));
				if(!lNew)
				{
					fprintf(stderr,"No spcae \n");
					freeAll(&lHead,&eHead,&nHead,&labelInLine,&dataArr,&insArr,&dHead);
					return FATAL_ERROR;
				}
				/*coppy the name of the label to the node*/
				strcpy(lNew->name, line + i);
				
			}
			/*make all the line string null*/
			terminate(line);
			/*coppy the command(the rest of the line after the label to the string*/
			strcpy(line,command);
			/*free the command pointer*/
			free(command);
		}
		/*get the new line char addres*/
		if((checkChar = strchr(line,NEW_LINE)))
		{
			/*check if its an empty line*/
			if(checkBefore(checkChar,line) == OK)
			{
				/*if it is count the line and continue*/
				lineNum++;
				if(lNew != NULL)
				{ 
					fprintf(stderr,"In file %s, the label %s, has no value\n",name,lNew->name);
					free(lNew);
					lNew = NULL;
				}
				continue;
			}
		}
		/*check if in the line there is .extern flag*/
		if((checkChar = strstr(line,EXT)))
		{
			/*check if there was a label in the line*/
			if(lNew != NULL)
			{
				/*if there was a label, print a massage and free the label*/
				printf("WARNING, the label ' %s ' is irelevant behind .extern \n", lNew->name);
				free(lNew);
				lNew = NULL;
			}
			/*initialize i to 0*/
			i = 0;
			/*check if there is chars before the .extern flag*/
			while(&line[i] != checkChar)
			{
				/*check if there is somthing beside white spaces before the flag*/
				if(line[i] != SPACE && line[i] != TAB)
				{
					/*print an error and initalize the error flag*/
					fprintf(stderr,"In file '%s' \n In line %d, there is a charachter before .extern \n",name,lineNum);
					error = ERROR;
					break;
				}
				i++;
			}
			/*get the labels and check them,if it return true, there was a problem in the extern definition*/
			if((checkVal=getLabels(name,checkChar,lineNum,&eNew,EXT)) == FATAL_ERROR)
			{	
				/*free the labels and initialize the error flag*/
				freeAll(&lHead,&eHead,&nHead,&labelInLine,&dataArr,&insArr,&dHead);
				return FATAL_ERROR;
			}
			else if(checkVal == ERROR)
			{
				error = ERROR;
			}
			else
			{
				/*check if the head is null*/
				if(eHead == NULL)
				{
					/*initalize the head and the current to the new node*/
					eHead =	eNew;
					eCurrent = eNew;
				}
				else
				{
					/*initalize the next node to the new*/
					eCurrent->next = eNew;
					/*initalize current to the new node*/
					eCurrent = eCurrent->next;
					
				}
			}
			
			/*check if the current is null*/
			if (eCurrent != NULL) 
			{
				/*loop until the end of the linked list, because the getLabels function can get more than one labels*/
   				while (eCurrent->next != NULL) 
   				{
        				eCurrent = eCurrent->next;
    				}
			}
			/*increase the line count and continue*/
			lineNum++;
			continue;	
		}
		/*if there is an .entry flag in line*/
		if((checkChar = strstr(line,ENT)))
		{
			/*check if there was a label before*/
			if(lNew != NULL)
			{
				/*print a watning and free the label*/
				printf("In file '%s'\n",name);
				printf("WARNING:The label %s is irrelevant before the entry flag\n",lNew->name);
				free(lNew);
				lNew = NULL;
			}
			/*check if there is somthing before the flag*/
			if(checkBefore(checkChar,line))
			{
				/*if it do, print an error and initialize the error flag*/
				fprintf(stderr,"In file '%s' \nIn line %d there is undifined char before the entry flag \n",name,lineNum);
				error = ERROR;
			}
			/*get the entry labels*/
			if((checkVal = getLabels(name,checkChar,lineNum,&nNew,ENT)) == FATAL_ERROR)
			{	
				/*free the labels and set the error flag*/
				freeAll(&lHead,&eHead,&nHead,&labelInLine,&dataArr,&insArr,&dHead);
				return FATAL_ERROR;
			}
			else if(checkVal == ERROR)
			{
				error = ERROR;
			}
			else
			{
				/*check if the head of the entries is null*/
				if(nHead == NULL)
				{
					/*set the current and the head to the new node*/
					nHead = nNew;
					nCurrent = nNew;
				}
				else
				{
					/*set the next of the current to the new node*/
					nCurrent->next = nNew;
					/*set the current to the new node*/
					nCurrent = nCurrent->next;
				}
				/*loop until the end of the linked list*/
				while(nCurrent->next !=NULL)
				{
					nCurrent = nCurrent->next;
				}
			}
			/*increase the line count and continue*/
			lineNum++;
			continue;
			
		}
		/*if there is a data instruction in line*/
		if((checkChar = strstr(line,DATA)))
		{
			/*check if there was a label*/
			if(lNew != NULL)
			{
				/*save the current index if the data array*/
				lNew->addres = dataCnt;
				/*check if there is a head for the linked list*/
				if(!dHead)
				{
					/*set the head and dCurrent to the new node*/
					dHead = lNew;
					dCurrent = lNew;
				}
				else
				{
					/*set the next of the current node to the new one*/
					dCurrent->next = lNew;
					/*set the current varaible to be the new node*/
					dCurrent = dCurrent->next;
					/*initialize the next node to NULL*/
					dCurrent->next = NULL;
				}
				lNew =NULL;
			}
			/*check if there is somthing before the .data*/
			if(checkBefore(checkChar,line))
			{
				/*if there is print a massage and set the error flag*/
				fprintf(stderr,"in file %s,\nIn line %d There is undifined char behind .data definition \n",name,lineNum);
				error = ERROR;
			}
			/*get the data from the related function and check if there is a kind of error*/
			if((checkVal = getIntData(checkChar,&dataArr,&dataCnt,&dataAmount,lineNum,name)) == FATAL_ERROR)
			{
				/*free all the pointer and return FAATAL_eRROR*/
				freeAll(&lHead,&eHead,&nHead,&labelInLine,&dataArr,&insArr,&dHead);
				return FATAL_ERROR;
			}
			/*set the error flag*/
			else if(checkVal == ERROR)
			{
				error = ERROR;
			}
			/*count the line number*/
			lineNum++;
			continue;
		}
		/*if there is a .string definition in the line*/
		if((checkChar = strstr(line,STRING)))
		{
			/*check if there is a char before*/
			if(checkBefore(checkChar,line))
			{
				fprintf(stderr,"in file %s,\nIn line %d There is undifined char behind .string definition \n",name,lineNum);
				error = ERROR;
			}
			/*check if there was a label*/
			if(lNew != NULL)
			{
				/*save the index of the data array*/
				lNew->addres = dataCnt;
				/*check if there is a head for the linked list*/
				if(!dHead)
				{
					/*set the head and dCurrent to the new node*/
					dHead = lNew;
					dCurrent = lNew;
				}
				else
				{
					/*set the next of the current node to the new one*/
					dCurrent->next = lNew;
					/*set the current varaible to be the new node*/
					dCurrent = dCurrent->next;
					/*initialize the next node to NULL*/
					dCurrent->next = NULL;
				}
				lNew =NULL;
			}
			/*get the data with the realted function and check types of error*/
			if((checkVal = getString(checkChar,&dataArr,&dataCnt,&dataAmount,lineNum,name)) == FATAL_ERROR)
			{
				/*free all the pointers and return FATAL_ERROR*/
				freeAll(&lHead,&eHead,&nHead,&labelInLine,&dataArr,&insArr,&dHead);
				return FATAL_ERROR;
			}
			/*set the error flag*/
			else if(checkVal == ERROR)
			{
				error = ERROR;
			}
			/*count the line*/
			lineNum++;
			continue;
		}
			
			
		/*check if there was a .extern or .entry after the label*/
		if(lNew != NULL)
		{
			lNew->addres = adr+cnt;
			/*check if there is a head for the linked list*/
			if(!lHead)
			{
				/*set the head and lCurrent to the new node*/
				lHead = lNew;
				lCurrent = lNew;
			}
			else
			{
				/*set the next of the current node to the new one*/
				lCurrent->next = lNew;
				/*set the current varaible to be the new node*/
				lCurrent = lCurrent->next;
				/*initialize the next node to NULL*/
				lCurrent->next = NULL;
					
			}
			lNew =NULL;
		}
		/*a function that gets the parameters of the instruction line and save it in  struct*/
		if((checkVal = getParam(line,&node,name,lineNum)) == OK)
		{
			/*check if this is the first instruction*/
			if(insArr == NULL)
			{
				/*get space from malloc*/
				insArr = (short *)malloc(amount * sizeof(short));
				if(insArr == NULL)
				{
					/*if there isnt space print a massage*/
					fprintf(stderr,"Not enough space\n");
					/*free the struct and all the pointers and return FATAL_ERROR*/
					freeCommLine(&node);
					freeAll(&lHead,&eHead,&nHead,&labelInLine,&dataArr,&insArr,&dHead);
					return FATAL_ERROR;
				}
				/*initalize all the indexes to 0*/
				makeZero(insArr,&cnt,amount);

			}
			/*save the cnt counter in mid varaible*/
			mid = cnt;
			/*check if we ar close to the and of the array*/
			if(ADD_CHECK(mid) >= amount)
			{
				/*ask for new space with realloc*/
				newArr = (short *)realloc(insArr,(amount = ENLARGE(amount)) * sizeof(short));
				if(newArr == NULL)
				{
					/*id there isnt space, print a massage */
					fprintf(stderr,"Not enough space\n");
					/*free the all the pointers and return fatal error*/
					freeCommLine(&node);
					freeAll(&lHead,&eHead,&nHead,&labelInLine,&dataArr,&insArr,&dHead);
					return FATAL_ERROR;
				}
				/*save the new addres from realloc*/
				insArr = newArr;
				/*make all the new indexes 0*/
				makeZero(insArr,&cnt,amount);

			}
			/*all of the functions that checks parameters are described in the header*/
			/*check if this is a command that gets no parameters*/
			/*checj for rts or stop*/
			if(CHECK_ZERO_PARAM(node->comNum))
			{
				/*check with the function realted to this function*/
				if(checkZeroParam(name,lineNum,insArr,&cnt,node) == ERROR)
				{
					/*set the error flag*/
					error = ERROR;
				}
			}
			/*check for the commands:not,clr,inc.dec.jmp,bne,red,jsr*/
			/*check the function with one parameter*/
			else if(CHECK_ONE_PARAM_1(node->comNum))
			{
				/*check with the realted function*/
				if((checkVal = checkOneParam1(name,lineNum,insArr,&cnt,node,&labelInLine))== FATAL_ERROR)
				{
					/*if we got fatal error, free all the nodes and return FATAL_ERROR*/
					freeCommLine(&node);
					freeAll(&lHead,&eHead,&nHead,&labelInLine,&dataArr,&insArr,&dHead);
					return FATAL_ERROR;
				}
				/*if we got an error, set the error flag*/
				else if(checkVal == ERROR)
				{
					error = ERROR;
				}
			}
			/*check for prn*/
			/*all of the functions until the block is over are checked the same way*/
			else if(node->comNum == PRN)
			{
				if((checkVal = checkOneParam2(name,lineNum,insArr,&cnt,node,&labelInLine))== FATAL_ERROR)
				{
					freeCommLine(&node);
					freeAll(&lHead,&eHead,&nHead,&labelInLine,&dataArr,&insArr,&dHead);
					return FATAL_ERROR;
				}
				else if(checkVal == ERROR)
				{
					error = ERROR;
				}
				
			}
			/*check for lea*/
			else if(node->comNum == LEA)
			{
				if((checkVal = checkTwoParam1(name,lineNum,insArr,&cnt,node,&labelInLine))== FATAL_ERROR)
				{

					freeCommLine(&node);
					freeAll(&lHead,&eHead,&nHead,&labelInLine,&dataArr,&insArr,&dHead);
					return FATAL_ERROR;
				}
				else if(checkVal == ERROR)
				{
					error = ERROR;
				}
				
				
			}
			/*check for add,sub,mov*/
			else if(CHECK_TWO_PARAM(node->comNum))
			{
				if((checkVal = checkTwoParam3(name,lineNum,insArr,&cnt,node,&labelInLine))== FATAL_ERROR)
				{

					freeCommLine(&node);
					freeAll(&lHead,&eHead,&nHead,&labelInLine,&dataArr,&insArr,&dHead);
					return FATAL_ERROR;
				}
				else if(checkVal == ERROR)
				{
					error = ERROR;
				}
				
			}		
		}
		/*check if there was a fatal problem*/
		else if(checkVal == FATAL_ERROR)
		{
			/*free the labels and return fatal_error*/
			freeAll(&lHead,&eHead,&nHead,&labelInLine,&dataArr,&insArr,&dHead);
			return FATAL_ERROR;
		}
		/*IF ITS NOT OK, and not FATAL_ERROR, set the error flag*/
		else
		{
			error = ERROR;
		}
		/*free the struct and set the pointer to null*/
		freeCommLine(&node);
		node = NULL;
		
		/*increase the line counter*/
		lineNum++;
	}
	/*set lCurrent to the first label*/
	lCurrent = lHead;
	/*set dCurrent to the first data label*/
	dCurrent = dHead;
	/*check if any of the are NULL*/
	if(dCurrent != NULL && lCurrent != NULL)
	{
		/*loop until the last label*/
		while(lCurrent->next != NULL)
		{
			lCurrent = lCurrent->next;
		}
		/*while we didnt reach the end of the data label list*/
		while(dCurrent != NULL)
		{
			/*set the last node of the labelsto the current of the data labels*/
			lCurrent->next = dCurrent;
			/*set dCurrent to the next*/
			dCurrent = dCurrent->next;
			/*set lCurrent to the next*/
			lCurrent = lCurrent->next;
			/*set the last node addres to its value + the last index in the instruction array + 100*/
			lCurrent->addres += (cnt + adr);
			
		}
		/*set dHead tp NULL*/
		dHead = NULL;
	
	}
	/*if there isnt labels but there is data instructions*/
	else if(lCurrent == NULL && dCurrent != NULL)
	{
		/*set dHead to lHead*/
		lHead = dHead;
		/*set the current varaible to the first node*/
		lCurrent = lHead;
		while(lCurrent != NULL)
		{
			/*set the address to the addres of the node + the last index if the instructin array + 100*/
			lCurrent->addres += (cnt+adr);
			lCurrent = lCurrent->next;
		}
		/*set dHead to NULL*/
		dHead = NULL;
	}
	lCurrent = lHead;
	/*connect the data array with the instruction array and check if there is error*/
	if(connect(&insArr,dataArr,cnt,dataCnt) == FATAL_ERROR)
	{	
		/*free all the pointer and return FATAL_eRROR*/
		freeAll(&lHead,&eHead,&nHead,&labelInLine,&dataArr,&insArr,&dHead);
		return FATAL_ERROR;
	}
	/*check if there is a label that defined twice*/
	if(checkDouble(lHead,name))
	{
		/*set the error flag*/
		error = ERROR;
	}
	/*check if all the entries exist in the label list, and none of the extern exist there*/
	if(checkExist(lHead,nHead,OK,name)||checkExist(lHead,eHead,ERROR,name))
	{
		/*set the error flag*/
		error = ERROR;
	}
	/*insert the in the right indexes the address of the labels in the array*/
	if((checkVal = insertIndex(&eHead,lHead,labelInLine,insArr,name)) == ERROR)
	{
		/*set the error flag*/
		error = ERROR;
	}
	/*if error*/
	else if(checkVal == FATAL_ERROR)
	{
		/*free all the pointers and return FATAL_ERROR*/
		freeAll(&lHead,&eHead,&nHead,&labelInLine,&dataArr,&insArr,&dHead);
		return FATAL_ERROR;
	}
	
	/*check if there was any error in the file*/
	if(error)
	{
		/*print a massage and return ERROR*/
		printf("Couldnt compile file %s \n",name);
		return ERROR;
	}
	/*check if there is externs*/
	if(eHead != NULL)
	{	
		/*use print labels to open .ext file and print the values, check if there was any errors*/
		if((printLabels(eHead,name,ERROR) == FATAL_ERROR))
		{	
			/*free all the pointers and return FATAL_ERROR*/
			freeAll(&lHead,&eHead,&nHead,&labelInLine,&dataArr,&insArr,&dHead);
			return FATAL_ERROR;
		}
	}
	/*check if there is entry labels*/
	if(nHead != NULL)
	{	
		/*use the function print labels to open .ent files and print the values, check if there was any error*/
		if(printLabels(nHead,name,OK) == FATAL_ERROR)
		{	
			/*free all the pointers and return FATAL_ERROR*/
			freeAll(&lHead,&eHead,&nHead,&labelInLine,&dataArr,&insArr,&dHead);
			return FATAL_ERROR;
		}
	}
	/*print and open the file by base64*/
	if(printOb(name,cnt,dataCnt,insArr))
	{
		/*free all the pointers and return FATAL_ERROR*/
		freeAll(&lHead,&eHead,&nHead,&labelInLine,&dataArr,&insArr,&dHead);
		return FATAL_ERROR;
	}
	/*free all the pointers*/
	freeAll(&lHead,&eHead,&nHead,&labelInLine,&dataArr,&insArr,&dHead);
	return OK;
}
			
			
			
			
			
			
			
			
			
			
