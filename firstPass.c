#include "firstPass.h"


void firstPass(line *linePointer,label *labelPointer,char *buffer,int lineInd,int labelInd,int *IC,int *DC){
    
    validFirstWord(linePointer,lineInd,buffer,IC,DC);/* first, we check if the user entered a line that starts with a label or a line that starts with an instruction\command */
    if(linePointer[lineInd].isEntry==TRUE) {
        handleEntryArg(linePointer,lineInd,buffer);/* this function creates a dynamic buffer and copies the argument to it in order it will get checked in the second pass */

        return;/* line's that contain .entry instruction's will be taken care in the second pass */
    }
    if(linePointer[lineInd]._isError==TRUE) {

        return;/* if the first word is not a command or an instruction and it doesnt have a colon at her end there is a syntax error,return to main */
    }
    if(linePointer[lineInd].command==FALSE && linePointer[lineInd].instruction==FALSE && linePointer[lineInd].isExtern==FALSE && linePointer[lineInd].isEntry==FALSE) {/* if the first word is not a command or an instruction we need to check if it has a valid label */
        isLabel(linePointer, labelPointer, buffer, lineInd,
                labelInd); /* a function that checks if there is a valid label */

        if (linePointer[lineInd].validLabel == TRUE && linePointer[lineInd]._isError ==
                                                       FALSE) {/* if the label is valid, check if there is a reserved word after */
            reservedWithLabel(linePointer, labelPointer, buffer, lineInd, labelInd, IC, DC);
            if (linePointer[lineInd]._isError == TRUE || linePointer[lineInd].isEntry ==
                                                         TRUE) {/* if the word is not reserved (or entry- who gets treated in the second pass) */

                return;/* return to assembler.c and print error */
            }
        } else/* if the label is invalid */{

            return;/* return to main, the error will be printed there */
        }
    }
    /* line's argument test function based on the line rserved word type */
    if(linePointer[lineInd].isExtern==TRUE){
        extArgTest(linePointer,labelPointer,lineInd,labelInd,buffer);
    }
    else if(linePointer[lineInd].command==TRUE){
        commandArgsTest(linePointer,lineInd,labelPointer,labelInd,buffer);
    }
    else if (linePointer[lineInd].instruction==TRUE){
        instructionsArgsTest(linePointer,lineInd,buffer,DC);
    }

}/* end of first pass */





