assembler: assembler.o afterMacro.o macroHelp.o checkLegal.o getData.o checkParam.o secondPass.o openFiles.o firstPass.o 
	gcc -ansi -pedantic -Wall assembler.o afterMacro.o macroHelp.o firstPass.o checkLegal.o getData.o openFiles.o checkParam.o secondPass.o -lm -o assembler 

assembler.o: assembler.c macroLib.h
	gcc -c -pedantic -ansi -Wall assembler.c -o assembler.o

afterMacro.o: afterMacro.c macroLib.h
	gcc -c -pedantic -ansi -Wall afterMacro.c -o afterMacro.o

macroHelp.o: macroHelp.c macroLib.h
	gcc -c -pedantic -ansi -Wall macroHelp.c -o macroHelp.o

checkLegal.o: checkLegal.c macroLib.h
	gcc -c -ansi -pedantic -Wall checkLegal.c -o checkLegal.o 

getData.o: getData.c macroLib.h
	gcc -c -pedantic -ansi -Wall getData.c -lm -o getData.o

firstPass.o: firstPass.c macroLib.h
	gcc -c -pedantic -ansi -Wall firstPass.c -o firstPass.o
	
checkParam.o: checkParam.c macroLib.h
	gcc -c -pedantic -ansi -Wall checkParam.c -o checkParam.o
secondPass.o:secondPass.c macroLib.h
	gcc -c -pedantic -ansi -Wall secondPass.c -o secondPass.o
openFiles.o: openFiles.c macroLib.h
	gcc -c -pedantic -ansi -Wall openFiles.c -o openFiles.o

