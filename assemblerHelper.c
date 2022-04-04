#include "assemblerHelper.h"
#include "string.h"
#include <stdio.h>


int validInputFile(char *inputFileName) {/* this int type  function checks if the input file from argv has a valid .as ending */
    char buffer[maxFileEndingLen];
    char *currFileEnding;
    int inputFileLen;
    strcpy(buffer,".as");
    buffer[maxFileEndingLen-1] = '\0';
    currFileEnding = strchr(inputFileName,pointAscii);
    if (currFileEnding){
        if (strcmp(currFileEnding,buffer)==0)/* comparing the file ending with a valid one */
            return 1;/* return 1 - "true" if the program ending  is valid */
    }
    return 0;/* return 0 - "flase" if the program ending is invalid */


}/* end of valid input function */

void errorSpecifier(line *linePointer,int lineInd,int lineNum){/* a void type function that checks if an error flag is raised, if it is it print's a specific error messege */
    if (linePointer[lineInd]._isError == TRUE) { /* declaring error found while the last line was parsed */
        if (linePointer[lineInd].invalid_reserved== TRUE)
            printf("invalid command or instruction in line #%d.\n", lineNum);
        else if (linePointer[lineInd].invalid_label == TRUE)
            printf("invalid label in line #%d.\n", lineNum);
        else if(linePointer[lineInd].invalid_register==TRUE)
            printf("invalid register argument in line #%d.\n",lineNum);
        else if (linePointer[lineInd].invalid_labelArg==TRUE)
            printf("invalid label argument in line #%d.\n",lineNum);
        else if(linePointer[lineInd].invalid_immed==TRUE)
            printf("invalid immed in line #%d\n",lineNum);
        else if(linePointer[lineInd].invalid_asciz_stringArg==TRUE)
            printf("invalid .asciz argument- the argument is not a valid string in line #%d.\n",lineNum);
        else if (linePointer[lineInd].invalid_rang_dotD_arg==TRUE)
            printf("invalid .d instruction argument- the amount of bites the argument takes is out of range in line #%d.\n",lineNum);
        else if (linePointer[lineInd].undefined_label_arg==TRUE)
            printf("The label that was given as argument is not defined in line #%d.\n",lineNum);
        else if (linePointer[lineInd].allocationError==TRUE)
            printf("no available memory for a dynamic data structure reallocation in line #%d.\n",lineNum);
        else if (linePointer[lineInd].missing_Operand==TRUE)
            printf("Missing operand, the command in line #%d received less operands than needed.\n",lineNum);
        else if(linePointer[lineInd].extraneous_Operand==TRUE)
            printf("extraneous operand, the command in line #%d received more operands than needed.\n",lineNum);
        
    }
   

}



void executePasses(char *argv[],int argc){/* a void type function that execute the assembler passes */
    /* declaring all the assembler's neccesery varibles */
    int validFlag,lineNum,lineInd,labelInd,argcInd,numOfLabelCell,ignoredLinesNum,secondPassInd;
    int IC,DC;/* code and data memory trackers */
    int numOfPrintedDataBites;/* an int varible that will help to keep the form of the .ob output file */ 
    int inputFileLen;
    int label_intialize_flag;
    /* delaring the file pointer, and both of the pointer's that willpoint the dynamic array that will contain  all line's and label's infomation */
    line *linePointer;
    label *labelPointer;
    FILE *currentFilePointer;
    char buffer[maxLineLen];/* a buffer that will contain each line of the entern input file by order */
    validFlag = 0;/* this flag tells us if there was at least one valid .as file */
    





        for (argcInd =1;argcInd<argc;argcInd++){/* a loop that goes tru all arguments stating from argc=1 in order to get only the input files */
            
            numOfPrintedDataBites = 0;
            IC =100;
            DC = 0;
            lineNum =1,lineInd=0,ignoredLinesNum=0;
            labelInd = 0;
            
            if(!(validInputFile(argv[argcInd]))){/*if the file is not valid continue to the next one */
                printf("Unreadble file, check the entered file format.\n");
                continue;
            }
            currentFilePointer = fopen(argv[argcInd],"r");/* open current file for reading */

            while(fgets(buffer,maxLineLen,currentFilePointer)!=0){/* while the current input file isn't fully readed */
                label_intialize_flag=0;/* this flag will help us to know if there is a need in initializing the label's dynamic array new cell */
                if(isComment(buffer)|| isBlank(buffer)){/* these function's check if the entered line is a comment or a blank line. if they are, they need to be ignored */
                    ignoredLinesNum++;/* count the line's who didnt got into the first and second passe's in order to track the real line number */
                    continue;/*continue to the next line in the input file */
                }
                if(lineNum==1){/* it means no space was located for the dynamic array */
                    linePointer =(line*) calloc(1,sizeof(line));
                    labelPointer = (label*)calloc(1,sizeof(label));
                }
                else{/*linePointer is already pointing a dynamic memory, realloc the linePointer in order to handle the current new line */
                    linePointer = (line*) realloc(linePointer,lineNum*sizeof(line));
                }

                if(lineNum>1){/* if its not the first line */
                    if(linePointer[lineInd-1].validLabel==TRUE){/* if a valid label was found, there is a need to extend the label dynamic array in order to handle the next valid label */
                        label_intialize_flag=1;/* turn on the label initialize flag */
                        labelInd++;/* increase index */
                        labelPointer = (label*) realloc(labelPointer,(labelInd+1)*sizeof(label));/* extend the label array based on the num of label we got so far */
                    }   
                }
                
                initialize_line_structures(linePointer,lineInd);/* initalizing all of line structure struct's and flag's */
                if(label_intialize_flag){/* if a nwe label cell was created in the label's dynamic array */
                    initialize_label_structures(labelPointer,labelInd);/* initalizing all of label's structure  flag's */
                }
                firstPass(linePointer,labelPointer,buffer,lineInd,labelInd,&IC,&DC);/* this function execute the first pass on the current line */
                errorSpecifier(linePointer,lineInd,lineNum+ignoredLinesNum);/* calling error specifier in order to get an output in case of an error in the current line */
                /* increasing lineNum and line index varibles in order to handle the new line */
                lineNum++;
                lineInd++;                
            }/* end of while */
            if(!(errorsScanner(linePointer,lineInd,ignoredLinesNum))){/* calling error scanner in order to see if there is a point of getting to the second pass */
                /* it means no error's were found during the first pass */
                updateLabelDC(labelPointer,labelInd,&IC);/* update all data type labels dc by adding ICF to it */ 
                secondPass(linePointer,lineInd,labelPointer,labelInd,ignoredLinesNum);/* execute the second pass */
            }
            if(!(errorsScanner(linePointer,lineInd,ignoredLinesNum))){/* calling error scanner, if an error was found during the first or second pass, there is no need in creating the output file */
                createOutput(linePointer,lineInd,labelPointer,labelInd,IC,DC,&numOfPrintedDataBites,argv[argcInd],strlen(argv[argcInd]));
            }
            free(linePointer);/* freeing linePointer, all internal struct's sent to free during the first and second pass */ 
            free(labelPointer);/* freeing labelPointer */
        }/*end of for */ 
}/* end of execution */









        
            
