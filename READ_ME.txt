aouthur: Adi Ben Israel
	 David Hodush

The purpose of the project is to transform an assembly file , or number of files to a muchine code in base 64.
There are three main stages
1.We organize and search the macro to get the file ready to compile. After that the macro definitions are deleted from the file.
2.We compile the file, we check the syntax and save the data in an array to parse later.
3.We write the data to the files .ob, .ent, .ext.

There can be 2 types of data after the macro stage:
1. A line that contains data like strings, numbers
2. A line that contains commands, labels, and other instructions.
In the first pass, we go threw the code and collect all of the data, trying to find errors in the syntax and the data. We save the data in arrays and linked lists, and if there is an error we dont open the files .ob, .ent, .ext.
In the second pass, we write the data to the files .ob, .ent, .ext.

we use the header: macroLib.h to all of the c files
In the project we check for variuos things
.data:
	we check if there is a spare comma or to much, if there is to much we dont collect the other data in the line
	we check the data definition and how many times it written, if the .data was written with an error like: .dataaa than the program will identify it as an error.
	we check if the number is not containing charchters.
	we check if the number is larger than 12 bits
	
.string:
	we check the same thing as the .data on the .string definition, by times written or syntax errors
	we check if the string starts and ends in '"'
	we check the amount of commas compared to the amount of strings
.entry\.extern:
	we check the same things on the defnition of the instruction
	In the project, it says we can only take one label each time, but in the explantion of the first pass it says we can take more than 1 label
	se we collect all the label no matter the quantity
	we check if the label is legall by the chars and if its a saved name of assembley
commands:
	for each commands we check if the command exist in the list of commands, if the parameters are leggal
	some assemptions:
	we assume that if the number in the line is not a digit,or larger than 10 bits the user thought of it as a label and we treat it that way
	we assume that if the parameter is not a register the user thought of it as label.
mcro:
	we assume that in every mcro defnition there is a endmcro def, and we check if one of the mcros was written in the end of the file
	we assume that every macro is written before it is called in the program , so at first we collect the data from the mcros and than print it in another loop
	In the forum, they mentioned we can assume that and the only things we shold check is the macro definitions, if they are written well, and chars after and 		before the saved names of assembly.
	we assume in the first pass that there is not mcro or endmcro names and we dont search for them, they should be deleted in the afterMacro function.
	
We get as much data ass we can to keep up with the checks, and we save it in arrays or linked lists, if there is some kind of error we dont open files, the .ent,.ext and .ob opens only if the file passed the compillation. 
any other command that will be written with or without a label that dosent identify with the saved names of assembly will be counted as an unidentified command and will be counted as an error.
most of the data gets collected in the first pass, so the last functions are written in another file called secondPass but is used in the first pass function, also the opening of the files goes from their. because we dont want the main to remember any of our data and we want to let only functions deal with it.

