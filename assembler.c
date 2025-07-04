
#include "macroLib.h"
int main(int argc, char *argv[])
{
	/*initalize a file pointer*/
	FILE *f = NULL;
	/*initalize i for the loop by 1*/
	int i = ERROR;
	/*varaible to check the output*/
	int check;
	/*check if there is files in the commandline*/
	if(argc == i)
	{
		/*print a massage and return OK*/
		printf("Please insert assembley file \n");
		return OK;
	}
	/*loop until the and of files*/
	while(i < argc)
	{
		/*check and insert the nacros, open a .am file and set it to the f pointer*/
		if((check = afterMacro(&f,argv[i])) == FATAL_ERROR)
		{
			/*if there is critical eissue*/
			return ERROR;
		}
		/*if there was any problem, dont continue checking the file*/
		if(check == ERROR)
		{
			/*close the files*/
			fclose(f);
			/*increase i for the next file and continue*/
			i++;
			continue;
		}
		/*get the value from the first pass function(all the other work and passes also in the first pass)*/
		check = firstPass(f,argv[i]);
		/*if there was a critical eissue*/
		if(check == FATAL_ERROR)
		{
			/*print a massage close the file and return ERROR*/
			printf("couldnt compile the file %s \n",argv[i]);
			fclose(f);
			return ERROR;
		}
		/*close the file*/
		fclose(f);
		/*print a massage about the compilatain*/
		printf("File %s compiled successfully\n",argv[i]);
		/*increase i for the next file*/
		i++;
		
	}
	return OK;
}
