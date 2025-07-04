#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*define the length of the line*/
#define LINELENGTH 81
/*size and enlarge to get the macro command*/
#define SIZE 10
#define ENLARGE(X) (X) + 10
#define ADD_CHECK(X) (X)+2
/*end macro to check where the macro ends*/
#define END_MACRO "endmcro"
/*the size of the endmcro flag*/
#define END_MACRO_SIZE 7
/*error flags*/
#define OK 0
#define ERROR 1
#define FATAL_ERROR 2
/*label should be in size 31*/
#define LABEL_SIZE 31
/*define chars for checks*/
#define NEW_LINE '\n'
#define COMMENT ';'
#define TAB '\t'
#define SPACE ' '
#define COMMA ','
#define LABEL_END ':'
#define NEGA '-'
#define POS '+'
#define ST_SYM '"'
/*define a string for strtok*/
#define TOKEN_WHITE " \n\t"
/*define an error symbol for the command checks*/
#define COM_ERROR 19
#define LAST_COM 15
/*define first imagenary addres*/
#define FIRST_ADRESS 100
/*define instruction strings*/
#define EXT ".extern"
#define ENT ".entry"
#define DATA ".data"
#define STRING ".string"
/*define the size if an .extern size*/
#define EXT_SIZE 7
/*define a string to use strtok with whitespaces and commas*/
#define TOK_LINE " \t\n,"
#define CHANGE_SIGN -1
/*define finals for the assembley commands check*/
#define REG_SIZE 3
#define EMPTY 0
#define REG_FLAG 5
#define LABEL_FLAG 3
#define DIGIT_FLAG 1
#define REG_ERROR 8
#define REG_FATAL 9
#define ZE '0'
/*checj for rts or stop*/
#define CHECK_ZERO_PARAM(X) ((X) == 15 || (X) == 14)?1:0
/*check for the commands:not,clr,inc.dec.jmp,bne,red,jsr*/
#define CHECK_ONE_PARAM_1(X) ((X) == 4 || (X) == 5 || (X) == 13 || ((X)<=11 && (X)>= 7))?1:0
/*check for prn*/
#define PRN 12
/*check for lea*/
#define LEA 6
/*check for cmp*/
#define CMP 1
/*check for mov, add sub*/
#define CHECK_TWO_PARAM(X)	((X)==0||(X)== 2|| (X) == 3)?1:0

/*macros to move the binary vakues to put in a certain index in the array*/
#define MOVE_COM(X) (X)<<5
#define MOVE_DEST(X) (X)<<2
#define MOVE_ORG(X) (X)<<9
#define FIRST_REG(X) (X)<<7
/*define a struct for the macro nodes*/
typedef struct Lines{
	/*a pointer to save the command between the mcro and endmcro*/
	char *comm;
	/*next*/
	struct Lines *next;
	/*char array in size of 81*/
	char name[LINELENGTH];
}node;
/*define a linked list for the Labels*/
typedef struct Labels{
	/*address of the label, for some of the data it represent the index and than converted to an address*/
	int addres;
	/*next*/
	struct Labels *next;
	/*char array in size of 31*/
	char name[LABEL_SIZE];
}label;

/*a struct to save the data from each command line*/
typedef struct CommandLine{
	/*the number of the command*/
	int comNum;
	/*the first parameter after the command*/
	char *param1;
	/*the sign of the parameter, could be a label, a register or a digit*/
	int sign1;
	/*the value of the parameter, if its a digit or a register*/
	int value1;
	/*same for parameter 2*/
	char *param2;
	int sign2;
	int value2;
	int cnt;
}cmdLine;

/**
  gets a linked list of labels and print it on files, an extern or entry
  @param *head the head of the list
  @param *name the name of the file
  @param sign a isng to check if its an entry or extern
  @return FATAL_ERROR if there is problem with file or memory and OK if not
 
 
  */
int printLabels(label *head,char *name,int sign);
/**
  gets to linked lists and check if they exist or not
  @param *head1 the main labels list
  @param *head2 an extern or entry lists
  @param sign a sign to check if its a entry or extern
  @param name the name of the file to print the error
  @return ERROR if an extern exist in the main label or an entry dont exist in the main label, else OK
  */
int checkExist(label *head1,label *head2,int sign,char *name);
/**
  check if a label is written twice
  @param *head the head of the linked list
  @param *name the name of the file to print the error
*/
int checkDouble(label *head,char *name);
/**
  gets the linked list of the labels and the externs and the instruction array and insert
  the values in he right indexes, creates a new linked lists with the extern indexes and names
  @param **le pointer to pointer to the extern linked list
  @param *la the head of the label linked list
  @param *InLine the linked list of the labels got as parameters
  @param *arr the instruction array
  @param *name the name of the file
  @return FATAL_ERROR if there is problem with location, else OK
*/
int insertIndex(label **le,label *la,label *InLine,short *arr,char *name);
/**
  create an .ob file and print the values by base64 method
  @param *name the name of the file
  @param sizeI the size of the instruction only
  @param sizeD the size of the data onle
  @param *arr the array of instructions
*/
int printOb(char *name,int sizeI,int sizeD, short *arr);
/**
  gets a number with 6 bits and print return the char by base64
  @param value the value to check
  @return the char
*/
char base64char(int value);
/**
 * Print the binary representation of a short integer.
 * 
 * @param num The short integer to be printed in binary.
 */
void printBinary(short num);

/**
 * Print an array of short integers in their binary representation.
 * 
 * @param arr The array of short integers.
 * @param size The size of the array.
 */
void printShortArrayAsBinary(const short* arr, int size);

/**
 * Perform the first pass on the assembly file.
 * 
 * @param f A pointer to the input file.
 * @param name The name of the file.
 * @return Returns OK if successful, ERROR if there's an error in the assembly code, or FATAL_ERROR for critical issues.
 */
int firstPass(FILE *f, char *name);

/**
 * Handle macro definitions and output the modified file.
 * 
 * @param ofile A pointer to a file pointer to output the modified file.
 * @param fname The name of the input file.
 * @return Returns OK if successful, ERROR if there's an error, or FATAL_ERROR for critical issues.
 */
int afterMacro(FILE **ofile, char *fname);

/**
 * Check if a name consists of legal characters (letters or digits).
 * 
 * @param name The name to be checked.
 * @return Returns OK if the name is legal, ERROR otherwise.
 */
int checkName(char *name);

/**
 * Check if a name is not a save keyword of assembly.
 * 
 * @param name The name to be checked.
 * @return Returns OK if the name is legal, ERROR otherwise.
 */
int checkLegalName(char *name);

/**
 * Get the macro command from a file.
 * 
 * @param c A pointer to a pointer to characters to store the macro definition.
 * @param f A file pointer to read from.
 * @return Returns OK if successful, ERROR if there's an error and FATAL_ERROR for crtical essuies.
 */
int getComm(char **c, FILE *f);

/**
 * Free a linked list of macro nodes.
 * 
 * @param head A pointer to a pointer to the head of the linked list.
 */
int freeMacro(node **head);

/**
 * Check if there is any non-whitespace character in a certain line.
 * 
 * @param f A file pointer to read characters from.
 * @return Returns OK if only whitespace is found, ERROR if non-whitespace is encountered.
 */
int checkForWhite(FILE *f);

/**
 * Check if there is anything before a certain pointer in an array exept white spaces.
 * 
 * @param c A pointer to an array.
 * @param str A pointer to check against.
 * @return Returns 1 if there is something before the pointer, 0 otherwise.
 */
int checkBefore(char *c, char *str);

/**
 * Count the number of commas in a string.
 * 
 * @param str The string to count commas in.
 * @return The number of commas found in the string.
 */
int countCommas(char *str);

/**
 * Initialize all characters in a string to null.
 * 
 * @param line The string to be terminated.
 */
void terminate(char *line);

/**
 * Free the memory used by a cmdLine structure.
 * 
 * @param cm A pointer to a pointer to the cmdLine structure.
 */
int freeCommLine(cmdLine **cm);

/**
 * Get labels from extern or entry definitions in the assembly code.
 * 
 * @param name The name of the file.
 * @param line The line containing the label definitions.
 * @param lineNum The line number.
 * @param la A pointer to a pointer to the head of the labels linked list.
 * @param instr The kind of instruction (entry or extern).
 * @return Returns OK if successful, ERROR if there's an error.
 */
int getLabels(char *name, char *line, int lineNum, label **la, char *instr);

/**
 * Get decimal numbers from a .data definition and insert them into a short array.
 * 
 * @param line The line containing the .data definition.
 * @param arr A pointer to a pointer to the short array to insert numbers into.
 * @param size The current size of the array.
 * @param amount The amount of space of the array.
 * @param lineNum The line number.
 * @param name The name of the file.
 * @return Returns OK if successful, ERROR if there's an error and FATAL_ERROR for critical essuies.
 */
int getIntData(char *line, short **arr, int *size, int *amount, int lineNum, char *name);

/**
 * Get string data from a .string definition and insert it into a short array.
 * 
 * @param line The line containing the .string definition.
 * @param arr A pointer to a pointer to the short array to insert data into.
 * @param cnt A pointer to the current count of inserted characters.
 * @param amount The amount of space in the array.
 * @param lineNum The line number.
 * @param name The name of the file.
 * @return Returns OK if successful, ERROR if there's an error and FATAL_ERROR for critical essuies.
 */
int getString(char *line, short **arr, int *cnt, int *amount, int lineNum, char *name);

/**
 * Free all allocated memory used by various data structures.
 * 
 * @param la A pointer to a pointer to the head of the labels linked list.
 * @param ex A pointer to a pointer to the head of the extern labels linked list.
 * @param ent A pointer to a pointer to the head of the entry labels linked list.
 * @param wa A pointer to a pointer to the head of the parameter labels linked list.
 * @param comArr A pointer to a pointer to the array of command data.
 * @param dataArr A pointer to a pointer to the array of data definitions.
 * @param da A pointer to a pointer to the head of linked list of of data labels.
 * @return Returns OK.
 */
int freeAll(label **la, label **ex, label **ent, label **wa, short **comArr, short **dataArr, label **da);

/**
 * Free a linked list of labels.
 * 
 * @param head A pointer to a pointer to the head of the linked list.
 */
void freeLabels(label **head);

/**
 * Check if a token represents a valid register.
 * 
 * @param token The token to be checked.
 * @param lineNum The line number.
 * @param name The name of the file.
 * @return Returns OK if the token is a valid register, REG_ERROR if this is not supposed to be a register and REG_FATAL if its a register
 	with a problem.
 */
int checkRegister(char *token, int lineNum, char *name);

/**
 * Get parameters from a line and populate a cmdLine structure.
 * 
 * @param line The line to extract parameters from.
 * @param cm A pointer to a pointer to the cmdLine structure.
 * @param name The name of the file.
 * @param lineNum The line number.
 * @return Returns OK if successful, ERROR if there's an error.
 */
int getParam(char *line, cmdLine **cm, char *name, int lineNum);

/**
 * Set all elements of an array to zero.
 * 
 * @param array The array to be modified.
 * @param index A pointer to the current index within the array.
 * @param amount The amount of elements to set to zero.
 */
void makeZero(short *array, int *index, int amount);


/**
 * Concatenate two arrays of short integers.
 * 
 * @param arr1 A pointer to a pointer to the first array.
 * @param arr2 A pointer to the second array.
 * @param size1 The size of the first array.
 * @param size2 The size of the second array.
 * @return Returns OK if successful, ERROR if there's an error,and FATAL_ERROR if there wasnt space.
 */
int connect(short **arr1, short *arr2, int size1, int size2);

/**
 * Insert symbols bitwise into an array at a specified index.
 * 
 * @param arr The array to insert into.
 * @param index The index at which to insert.
 * @param address The address to insert.
 * @param send The send instruction flag.
 * @param com The command number.
 */
void insertSym(short *arr, int index, int address, int send, int com);

/**
 * Check the validity of an assembly command with zero parameters.
 * 
 * @param name The name of the command.
 * @param lineNum The line number.
 * @param array A pointer to the array of command data.
 * @param cnt A pointer to the current count of command data.
 * @param instr A pointer to the cmdLine structure representing the instruction.
 * @return Returns OK if successful, ERROR if there's an error.
 */
int checkZeroParam(char *name, int lineNum, short *array, int *cnt, cmdLine *instr);

/**
 * Check the validity of an assembly command with one parameter (type 1).
 * 
 * @param name The name of the command.
 * @param lineNum The line number.
 * @param array A pointer to the array of command data.
 * @param cnt A pointer to the current count of command data.
 * @param instr A pointer to the cmdLine structure representing the instruction.
 * @param la A pointer to a pointer to the head of the labels linked list.
 * @return Returns OK if successful, ERROR if there's an error.
 */
int checkOneParam1(char *name, int lineNum, short *array, int *cnt, cmdLine *instr, label **la);

/**
 * Check the validity of an assembly command with one parameter (type 2).
 * 
 * @param name The name of the command.
 * @param lineNum The line number.
 * @param array A pointer to the array of command data.
 * @param cnt A pointer to the current count of command data.
 * @param instr A pointer to the cmdLine structure representing the instruction.
 * @param la A pointer to a pointer to the head of the labels linked list.
 * @return Returns OK if successful, ERROR if there's an error.
 */
int checkOneParam2(char *name, int lineNum, short *array, int *cnt, cmdLine *instr, label **la);

/**
 * Check the validity of an assembly command with two parameters (type 1).
 * 
 * @param name The name of the command.
 * @param lineNum The line number.
 * @param array A pointer to the array of command data.
 * @param cnt A pointer to the current count of command data.
 * @param instr A pointer to the cmdLine structure representing the instruction.
 * @param la A pointer to a pointer to the head of the labels linked list.
 * @return Returns OK if successful, ERROR if there's an error.
 */
int checkTwoParam1(char *name, int lineNum, short *array, int *cnt, cmdLine *instr, label **la);

/**
 * Check the validity of an assembly command with two parameters (type 2).
 * 
 * @param name The name of the command.
 * @param lineNum The line number.
 * @param array A pointer to the array of command data.
 * @param cnt A pointer to the current count of command data.
 * @param instr A pointer to the cmdLine structure representing the instruction.
 * @param la A pointer to a pointer to the head of the labels linked list.
 * @return Returns OK if successful, ERROR if there's an error.
 */
int checkTwoParam2(char *name, int lineNum, short *array, int *cnt, cmdLine *instr, label **la);

/**
 * Check the validity of an assembly command with two parameters (type 3).
 * 
 * @param name The name of the command.
 * @param lineNum The line number.
 * @param array A pointer to the array of command data.
 * @param cnt A pointer to the current count of command data.
 * @param instr A pointer to the cmdLine structure representing the instruction.
 * @param la A pointer to a pointer to the head of the labels linked list.
 * @return Returns OK if successful, ERROR if there's an error.
 */
int checkTwoParam3(char *name, int lineNum, short *array, int *cnt, cmdLine *instr, label **la);

