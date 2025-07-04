#include "macroLib.h"
/*macros to insert the type of ARE in the first 2 bits*/
#define REALLOCATE 2
#define EXTERNAL 1
/*in this file we will initalize functions that check the validity of the labels*/
/*check if som of the nodes exist twice in the array*/
int checkDouble(label *head,char *name)
{
	/*set label varaibles for the loop*/
	label *he = head,*current,*checkName;
	/*set count and an error flag*/
	int count = OK,error = OK;
	/*set current to the head of the file*/
	current = he;
	/*loop until the last node*/
	while(current !=NULL)
	{
		/*set checkName to the current node*/
		checkName = current;
		/*set current to the head*/
		current = he;
		while(current != NULL)
		{
			/*check if some of the nodes got the same name*/
			if(strcmp(current->name,checkName->name) == OK)
			{
				/*count the node*/
				count++;
			}
			/*check if it counted more than once*/
			if(count > ERROR)
			{
				/*print a massage*/
				fprintf(stderr,"In file %s\n You cant initialize a label more than 1: %s\n",name,checkName->name);
				/*set the erro flag and break the loop*/
				error = ERROR;
				break;
			}
			/*set current to his next*/
			current = current->next;
		}
		/*check if the current is not null, if not set the current to the next*/
		if(current != NULL)
		{
			current = current->next;
		}
		else
		{
			/*set the current to the next if checkName*/
			current = checkName->next;
		}
		/*set count to 0*/
		count = OK;
	}
	/*return the error flag*/
	return error;
}
/*this function gets the label list, and can get eaither the extern and the entry list and check if they exist*/
/*in this function we relay on the function get double, that a label couldnt apeer twice*/
int checkExist(label *head1,label *head2,int sign,char *name)
{
	/*set vatraibles to the label parameters*/
	label *he1 = head1, *curr1;
	/*set varaibles to the entry\extern parameters*/
	label *curr2 = head2;
	/*set a counter and error flag*/
	int cnt = OK,error = OK;
	/*while we didnt reach the end of the entry\extern list*/
	while(curr2 != NULL)
	{
		/*set curr1 to the head of the labels list*/
		curr1 = he1;
		/*loop until the last label*/
		while(curr1 != NULL)
		{
			/*compare the names*/
			if(strcmp(curr2->name,curr1->name)==OK)
			{
				/*save the addres, if it is an extern it shouldnt get here and it is an error*/
				curr2->addres = curr1->addres;
				/*count the node*/
				cnt++;
			}
			/*go to the next*/
			curr1 = curr1->next;
		}
		/*check if it is an entry*/
		if(sign == OK)
		{
			/*check if there isnt a label like this*/
			if(cnt < ERROR)
			{
				/*print a massage and set the error flag*/
				fprintf(stderr,"IN file %s\nAn entry label should be exist in the file: %s\n",name,curr2->name);
				error = ERROR;
			}
		}
		else
		{
			/*check if the label exist in the label list*/
			if(cnt >= ERROR)
			{
				/*print a massage and set the error flag*/
				fprintf(stderr,"In file %s\nAn extern cant be a label in the file:%s\n",name,curr2->name);
				error = ERROR;
			}
		}
		/*set the counter to 0*/
		cnt = OK;
		/*go to the next node*/
		curr2= curr2->next;
	}
	/*return the error flag*/
	return error;

}



/*in this function , we check if the name got as parameter is an extern or a label and we initiliaze the instruction array*/
int insertIndex(label **le,label *la,label *InLine,short *arr,char *name)
{
	/*set varaibles for the labeles*/
	label *eCurrent = *le, *lCurrent = la, *eHead = *le
	/*set varaibles for new extern nodes*/;
	label *head = NULL,*current,*newNode;
	/*set an error flag*/
	int error = OK;
	/*while loop until the last parameter from the lines*/
	while(InLine != NULL)
	{
		/*set eCurrent to the first extern*/
		eCurrent = eHead;
		/*set lCurrent to the first label*/
		lCurrent = la;
		/*loop until the last extern*/
		while(eCurrent != NULL)
		{
			/*compare the name of the nodes*/
			if(strcmp(InLine->name,eCurrent->name)== OK)
			{
				/*insert in the first 2 bits 01*/
				arr[InLine->addres] |= EXTERNAL;
				/*check if the head is NULL*/
				if(head == NULL)
				{
					/*get new location for the head*/
					newNode = (label *)malloc(sizeof(label));
					/*check success*/
					if(newNode == NULL)
					{
						/*print a massage and return FATAL_eRROR*/
						fprintf(stderr,"Not enough space \n");
						return FATAL_ERROR;
					}
					/*set the head and the current to the newNode*/
					head = newNode;
					current = newNode;
				}
				else
				{
					/*get space from malloc to a newNode*/
					newNode = (label *)malloc(sizeof(label));
					/*check success*/
					if(newNode == NULL)
					{
						/*print a massage and return FATAL_ERROR*/
						fprintf(stderr,"Not enough space \n");
						return FATAL_ERROR;
					}
					/*set the next of the current to the newNode*/
					current->next = newNode;
					/*set the current to his next*/
					current = current->next;
				}
				/*set the enxt of the current to NULL*/
				current->next = NULL;
				/*coppy the parameter to the new node*/
				strcpy(current->name,eCurrent->name);
				/*save the address index if the parameter and add 100*/
				current->addres = InLine->addres + FIRST_ADRESS;
				break;
			}
			/*move to the next extern node*/
			eCurrent = eCurrent->next;
		}
		/*loop until the last node*/
		while(lCurrent != NULL)
		{
			/*compare the names*/
			if(strcmp(InLine->name,lCurrent->name) == OK)
			{
				/*set in the first 2 bits 10*/
				arr[InLine->addres] |= REALLOCATE;
				/*insert the address in the right index to the array*/
				arr[InLine->addres] |= MOVE_DEST(lCurrent->addres);
				break;
			}
			lCurrent = lCurrent->next;
		}
		/*if bothe of the linkes lists are null*/
		if(lCurrent == NULL && eCurrent == NULL)
		{
			/*print a massage and set the error flag*/
			fprintf(stderr,"In file %s\nThe label %s didnt found\n",name,InLine->name);
			error = ERROR;
		}
		/*move to the next parameter*/
		InLine = InLine->next;
	}
	/*check if we created new extern list*/
	if(head != NULL)
	{
		/*free the old one*/
		freeLabels(le);
		/*initialize the new one to le*/
		*le = head;
	}
	/*return the error flag*/
	return error;
}

