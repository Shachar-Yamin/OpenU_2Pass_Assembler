#include "strHandle.h"

int nonBlankInd(char *buffer){/* this function initialize an ind variable and returns the first nonBlank char ind */
    int firstLetterInd;
    for (firstLetterInd = 0; isspace(buffer[firstLetterInd]); firstLetterInd++) {
        /* looping through all tabs and spaces that my come in the beginning in order to point the first nonBlank char ind*/}
    return firstLetterInd;

}

int isBlank(char *buffer){/* this int function checks if the line is a blank line */
    int i;
    for (i=0;buffer[i]!='\n';i++) {
        if (isspace(buffer[i])==0)
            return 0;/* return 0 in case its not */
    }
    return 1;/* return's 1 if it is balnk */
}

int isComment(char *buffer) {/* this int function checks if the line is a comment line */
    int i;
    for (i = 0; buffer[i] != '\n'; i++) {
        if (isspace(buffer[i])==0){
            if (buffer[i] == semiColonAscii)
                return 1;/* return's 1 if it is a comment */
            else
                return 0;/* return 0 in case its not */
        }
    }
    return 0;/* return 0 in case its not */
}


/* start Label analyze function */
void isLabel(line *linePointer,label *labelPointer,char *buffer,int lineInd,int labelInd){/* this function checks if there is an option for a label in current line */
    char labelBuffer[maxLabelLen]; /* a constant array that will get the string we suspect to be a label */
    int firstLetterInd,labelBufInd;
    for (firstLetterInd = 0; isspace(buffer[firstLetterInd]); firstLetterInd++) {
        /* looping through all tabs and spaces that my come in the beginning in order to point the first nonBlank char ind*/}

    for (labelBufInd=0;isspace(buffer[firstLetterInd])==0 && buffer[firstLetterInd]!='\n';labelBufInd++) {
        if (firstLetterInd > firstLetterInd + maxLabelLen) {/* an if statement that checks if label length is bigger than 31*/
            raiseLabelError(linePointer, lineInd);
            return; /* rethink about an exit strategy */
        }
        labelBuffer[labelBufInd] = buffer[firstLetterInd++];
    }
    labelBuffer[labelBufInd] = '\0';/* declaring string ending in order to prevent garbage value appearance */

    labelTest(linePointer,labelPointer,lineInd,labelInd,labelBuffer,labelBufInd);/* check if the label is valid and raise relevant flag */
    if(linePointer[lineInd]._isError==TRUE)
        return; /* return to assembler.c an error message will be printed */

    labelBuffer[labelBufInd-1] = '\0';/* deleting the colon in order to check if the label is a reserved word */
    if(isCommand(labelBuffer)|| isInstruction(labelBuffer)){
        raiseLabelError(linePointer,lineInd);
        return;
    }
    linePointer[lineInd].validLabel=TRUE;
    strcpy(labelPointer[labelInd].labelBuff,labelBuffer);/* coping the label to the labelBuffer inside the label struct */
}/* end of isLabel */

void labelTest(line * linePointer,label *labelPointer,int lineInd,int labelInd,char*labelBuffer,int labelBufInd){/* this function check if the label is valid 
and raise an error flag in case its not */

    if(labelDuplicationCheck(labelPointer,labelInd,labelBuffer) || labelColonCheck(labelBuffer,labelBufInd)==0){/*an if statement that checks if the current label is already exist and if the label contains a colon */
        raiseLabelError(linePointer,lineInd);
        return;/* return to isLabel*/
    }
    for (labelBufInd= 0;labelBuffer[labelBufInd]!='\0';labelBufInd++){
        if(labelBufInd==0){
            if(!(labelCharTest(labelBuffer,labelBufInd))) {/* label must start with a letter */
                raiseLabelError(linePointer,lineInd);
            }
        }
        if(!(labelCharTest(labelBuffer,labelBufInd) || labelDigitsCheck(labelBuffer,labelBufInd))){
            raiseLabelError(linePointer,lineInd);
        }
    }
}


int labelCharTest(char* buffer,int i){/* this function check if current label char is valid by her ascii*/
    if ((buffer[i] >= lowCap_a_ascii && buffer[i] <= lowCap_z_ascii) ||(buffer[i] >= capitalAascii && buffer[i] <= capitalZascii))
        return 1;
    else
        return 0;
}
int labelDigitsCheck(char* buffer,int i){/* this function check if current label digit is valid by her ascii */
    if(buffer[i]>=digitsStart && buffer[i]<=digitsEnd)
        return 1;
    else if (buffer[i]==colonAscii) {
        if (buffer[i+1]=='\0')
            return 1;
        else
            return 0;
    }
    else
        return 0;
}

int labelDuplicationCheck(label *labelPointer,int labelInd,char *givenLabel){/* this function check if the label was already defined */
    int ind;
    for (ind=0; ind<=labelInd;ind++) {
        if (strcmp(labelPointer[ind].labelBuff, givenLabel) == 0) {
            if (labelPointer[ind].isExt == TRUE || labelPointer[ind].isEnt == TRUE)
                return 0;
            else {
                return 1;
            }
        }
    }
    return 0;
}

int labelColonCheck(char* buffer,int labelLengh){/* this function check if label has colon in the end */
    if(buffer[labelLengh-1]==colonAscii)
        return 1;
    return 0;

}

void raiseLabelError(line* linePointer,int lineInd){/* this function raise a label error in case one was found */
    linePointer[lineInd]._isError=TRUE;
    linePointer[lineInd].invalid_label=TRUE;
}

/* end of label function's */



/* function's that checks if the user chose to enter a line without a label in the beginning */
void validFirstWord(line *linePointer,int lineInd,char * buffer,int *IC,int *DC) {
    int firstLetterInd;
    char firstWordBuff[maxLabelLen]; /* it might be a label */
    int firstWordInd;
    for (firstLetterInd = 0; isspace(buffer[firstLetterInd]); firstLetterInd++) {
        /* looping through all tabs and spaces that may come in the beginning in order to point the first nonBlank char ind*/}

    firstWordInd=0;
    for(firstLetterInd; isspace(buffer[firstLetterInd])==0 && firstLetterInd<maxLineLen && buffer[firstLetterInd]!='\n'; firstLetterInd++) {
        if (firstWordInd > maxLabelLen) {/* an if statement that checks if label length is bigger than 31*/
            raiseLabelError(linePointer, lineInd);
            return; /* rethink about an exit strategy */
        }
        firstWordBuff[firstWordInd++] = buffer[firstLetterInd];
    }
    firstWordBuff[firstWordInd] = '\0';/* putting a string ending char in the end of the string in order to prevent garbage chars appearance */


    firstWordRelevantFlags(linePointer,lineInd,firstWordBuff,IC,DC);/* raising all relevant flag based on the first word given */
    if (linePointer[lineInd].isEntry==TRUE){
        return;/* the assembler will take care of .entry instruction in the second pass */

    }
    if(linePointer[lineInd].command==TRUE){
        opCodeCoder(linePointer,lineInd,firstWordBuff);/* coding the relevant opcode according to the command reserved word */
    }

}


int lastCharColonChecker(char *buffer){/* a function that checks if the first word has a colon, if their is, its suspected as a label */
    int i;
    for (i = 0;buffer[i]!='\0';i++) {
        if(buffer[i+1]=='\0'){
            if(buffer[i]==colonAscii)
                return 1;
        }
    }
    return 0;
}
void firstWordRelevantFlags(line *linePointer,int lineInd,char *buffer,int *IC,int*DC){/* a function that raise first the Word Relevant Flags */
    if (isCommand(buffer)) {
        linePointer[lineInd].command = TRUE;
        linePointer[lineInd].noLabel = TRUE;
        linePointer[lineInd].IC = *IC;/* giving the line her final address */
        *IC+=4;/* extending IC by four in order to track the next command address */
        raiseTypeFlags(linePointer,lineInd,buffer);/* checks if the command are type J,R or I and raise relevant flag */
    }
    else if (isInstruction(buffer)){
        if(isEnt(buffer))
            linePointer[lineInd].isEntry=TRUE;
        else if(isExt(buffer))
            linePointer[lineInd].isExtern=TRUE;
        else if (linePointer[lineInd].isExtern==FALSE&& linePointer[lineInd].isEntry==FALSE){
            linePointer[lineInd].instruction = TRUE;
            linePointer[lineInd].DC = *DC;/* giving the instruction line her temporary address that get an updated in the second pass */
            raiseInstructionFlag(linePointer,lineInd,buffer);
            linePointer[lineInd].noLabel = TRUE;
        }
    }
    else if(lastCharColonChecker(buffer)==0){/* checking if the first word was meant to be used as a label by checking if it contains a colon in end */
        linePointer[lineInd].invalid_reserved=TRUE;
        linePointer[lineInd]._isError=TRUE;
    }
}

void raiseInstructionFlag(line *linePointer,int lineInd,char *buffer){/* this function will help to detect the exact type of instruction */
    if (strcmp(buffer,".asciz")==0)
        linePointer[lineInd].is_asciz=TRUE;
    else if(strcmp(buffer,".dh")==0)
        linePointer[lineInd].is_dh=TRUE;
    else if(strcmp(buffer,".dw")==0)
        linePointer[lineInd].is_dw=TRUE;
    else if (strcmp(buffer,".db")==0)
        linePointer[lineInd].is_db=TRUE;
}

/* end of function's that checks if the user chose to enter a line without a label in the beginning




/*the start of getting command or instruction from a line that contains a label functions*/


void reservedWithLabel(line *linePointer,label *labelPointer,char *buffer,int lineInd,int labelInd,int* IC,int*DC){/* this function is responsible of checking the reserved word in line without a label */
    char commOrInstBuff[maxInstOrCommLen];/* the maximum length of a valid command or an instruction is 7 plus a string ending char ('\0') */
    int commOrInstInd;
    int newBuffInd;
    int startingInd;
    commOrInstInd = reserved_Ind_WLabel(buffer);/* getting the command or instruction stating Ind */

    newBuffInd =0;
    startingInd = commOrInstInd;
    for (commOrInstInd;buffer[commOrInstInd]!='\n' && isspace(buffer[commOrInstInd])==0;commOrInstInd++) {
        if(newBuffInd>maxInstOrCommLen){
            linePointer[lineInd].invalid_reserved=TRUE;
            linePointer[lineInd]._isError=TRUE;
            return;
        }
        commOrInstBuff[newBuffInd++] = buffer[commOrInstInd];
    }
    commOrInstBuff[newBuffInd]='\0';/*in order to prevent garbage chars in the end of the string we want to check */


    /* raising the relevant flags based on the word that was founded */
    ReservedRelevantFlagsWLabel(linePointer,labelPointer,lineInd,labelInd,commOrInstBuff,IC,DC);
    if(linePointer[lineInd].command==TRUE){
        opCodeCoder(linePointer,lineInd,commOrInstBuff);/* coding the relevant opcode according to the command reserved word */
    }
}

int reserved_Ind_WLabel(char *buffer){/* this function returns command or instruction starting index */
    int afterLabelInd;
    for (afterLabelInd=0;buffer[afterLabelInd]!='\n' && buffer[afterLabelInd]!=colonAscii;afterLabelInd++) {
    }/* creating a for loop that will stop when it will reach the label's colon */
    afterLabelInd+=2; /* in order to skip the colon that the loop stopped on plus the needed space after the label */

    for (afterLabelInd; isspace(buffer[afterLabelInd]) && buffer[afterLabelInd]!='\n';afterLabelInd++) {
        /* this loop skips all tabs and spaces that might appear between the label and to command/instruction */
    }
    return afterLabelInd;
}
void ReservedRelevantFlagsWLabel(line *linePointer,label *labelPointer,int lineInd,int labelInd,char *buffer,int *IC,int *DC) {/* a function that checks if the word is reserved and raise a relevant flag */
    if (isCommand(buffer)) {/* if a command is found raise relevant flags */
        linePointer[lineInd].command = TRUE;
        labelPointer[labelInd].isCode = TRUE;
        raiseTypeFlags(linePointer,lineInd,buffer);
        linePointer[lineInd].IC = *IC;
        labelPointer[labelInd].ic = *IC;
        *IC+=4;/* extending ic value by four in order to track the next line address*/
    } else if (isExt(buffer)) {/* if extern raise flag */
        linePointer[lineInd].validLabel=FALSE;/* turning of the valid label flag by iving it the value "false" because an extern label should be ignored */
        linePointer[lineInd].isExtern = TRUE;
        linePointer[lineInd].ignoreLabel=TRUE;/* turning on this flag in order to prevent the labels dynamic array from extending */
        strcpy(labelPointer[labelInd].labelBuff,"");/* there is no need in keeping an extern instruction label*/

    } else if (isEnt(buffer)) {/* if enter enter raise flag */
        linePointer[lineInd].validLabel=FALSE;/* turning of the valid label flag by iving it the value "false" because an entry label should be ignored */
        linePointer[lineInd].isEntry = TRUE;
        linePointer[lineInd].ignoreLabel=TRUE;/* turning on this flag in order to prevent the labels dynamic array from extending */
        strcpy(labelPointer[labelInd].labelBuff,"");/* there is no need in keeping an entry instruction label*/
    } else if (isInstruction(buffer)) {/* if an instruction is found raise relevant flags */
        raiseInstructionFlag(linePointer,lineInd,buffer);/* this function will detect the exact type of instruction */
        linePointer[lineInd].DC = *DC;/* giving the line her unfinished address (will get updated in the second pass */
        labelPointer[labelInd].dc= *DC;/* giving the label her unfinished address (will get updated in the second pass */
        linePointer[lineInd].instruction = TRUE;
        labelPointer[labelInd].isData = TRUE;

    } else {/* not a reserved word */
        linePointer[lineInd]._isError = TRUE;
        linePointer[lineInd].invalid_reserved = TRUE;
    }
}

/* this function raise the relevant type flag (I,J,R) when a line holds a valid command reserved word */
void raiseTypeFlags(line* linePointer,int lineInd,char *buffer){
    if(isRType(buffer))
        linePointer[lineInd].RtypeCommand=TRUE;
    else if(isIType(buffer))
        linePointer[lineInd].ItypeCommand=TRUE;
    else if(isJType(buffer))
        linePointer[lineInd].JtypeCommand=TRUE;
}/* end of raiseTypeFlags */


/* opcode functions- these function's will code the opcode of every command that has one in her bitfield based on her kind */

void RTypeOpCoder(line *linePointer,int lineInd,char *buffer){
    int op;
    if(isZeroOp(buffer)){
        linePointer[lineInd].R.opcode=0;
    }
    else if(isOneOp(buffer)) {
        linePointer[lineInd].R.opcode = 1;
    }


}
void JTypeOpCoder(line *linePointer,int lineInd,char *buffer){
    int op;

    if(isIndPlusThirty(buffer)){
        op = isIndPlusThirty(buffer);
        linePointer[lineInd].J.opcode=op;
    } else if(isStop(buffer)){
        linePointer[lineInd].J.opcode=stopOpCode;
    }

}

void ITypeOpCoder(line *linePointer,int lineInd,char *buffer){
    int op;
    op = isIndPlusTenOp(buffer);
    linePointer[lineInd].I.opcode=op;
}

void opCodeCoder(line *linePointer,int lineInd,char *buffer){
    if(linePointer[lineInd].RtypeCommand==TRUE){
        RTypeOpCoder(linePointer,lineInd,buffer);
    }
    else if(linePointer[lineInd].JtypeCommand==TRUE){
        JTypeOpCoder(linePointer,lineInd,buffer);
    }
    else if(linePointer[lineInd].ItypeCommand==TRUE){
        ITypeOpCoder(linePointer,lineInd,buffer);
    }
}



/* end of opcode functions */


/* valid arguments functions */
int argLabelCheck(char *buffer){/* a function that checks if label that was given as am argument is valid */
    int bufferInd;
    for (bufferInd = 0; buffer[bufferInd] != '\0'; bufferInd++) {
        if(buffer[bufferInd]==colonAscii){
            return 0;
        }
        if (bufferInd == 0) {
            if (!(labelCharTest(buffer, bufferInd))) {/* label must start with a letter */
                return 0;
            }
        }
        if (!(labelCharTest(buffer, bufferInd) || labelDigitsCheck(buffer, bufferInd))) {
            return 0;
        }
    }
    return 1;

}




int firstArgIndWLabel(char *buffer){
    int firstArgInd;
    firstArgInd = reserved_Ind_WLabel(buffer);/* giving the address the index of the first reserved word after passing the label and the blank chars */
    for (firstArgInd;isspace(buffer[firstArgInd])== 0 && buffer[firstArgInd]!='\n';firstArgInd++) {
        /* looping thru the reserved word char until it reaches a blank char */
    }
    for(firstArgInd;isspace(buffer[firstArgInd])&&buffer[firstArgInd]!='\n';firstArgInd++){
        /*looping thru all blank chars between the reserved word and the arg */
    }
    return firstArgInd;
}

int firstArgIndNoLabel(char *buffer) {
    int firstArgInd;
    for (firstArgInd = 0; isspace(buffer[firstArgInd]) && buffer[firstArgInd] != '\n'; firstArgInd++) {
        /* looping through all tabs and spaces that may come in the beginning in order to point the first nonBlank char ind*/}
    for (firstArgInd; isspace(buffer[firstArgInd])==0 && buffer[firstArgInd] != '\n'; firstArgInd++) {
        /*looping trough all chars of the reserved word */}
    for (firstArgInd; isspace(buffer[firstArgInd]) &&buffer[firstArgInd] != '\n'; firstArgInd++) {
        /* one more time looping through all tabs and spaces that come after the reserved word in order to point the first arg*/}
    return firstArgInd;
}


/* command argFunctions */

int iterateUntilComma(line *linePointer,int lineInd,char *buffer,int ind){/* this function checks if there are more non blank chars between the operand and the comma */
    for (ind;buffer[ind]!=commaAscii;ind++){
        raiseExtraneousOperandFlags(linePointer,lineInd);
    }
    return ind;

}

void iterateUntilEnd(line *linePointer,int lineInd,char *buffer,int ind){/* this function checks if there are more non blank chars between the operand and the string ending char */
    for (ind;buffer[ind]!='\0';ind++){
        if ((isspace(buffer[ind])==0) && buffer[ind]!='\n') {
            raiseExtraneousOperandFlags(linePointer, lineInd);
            return;
        }
    }
}


void raiseExtraneousOperandFlags(line *linePointer,int lineInd){/* a function that was made to raise the extraneous_operand flag in case of entering operands needed */
    linePointer[lineInd].extraneous_Operand=TRUE;
    linePointer[lineInd]._isError=TRUE;
}
void raiseMissingOperandFlags(line *linePointer,int lineInd){
    linePointer[lineInd].missing_Operand=TRUE;
    linePointer[lineInd]._isError =TRUE;
}


int reservedIndNoLabel(char *buffer){/* a function that get the reserved word index when there is no label */
    int firstLetterInd;
    for (firstLetterInd = 0;isspace(buffer[firstLetterInd]) && buffer[firstLetterInd]!='\n'; firstLetterInd++) {
        /* looping through all tabs and spaces that my come in the beginning in order to point the first nonBlank char ind*/}
    return firstLetterInd;
}


void raiseCommandsArgFlags(line *linePointer,int lineInd,char *buffer){/* this function raise all command's argument flag's in order to handle multiple numner of commands in one function */
    /* getting the reserved command in order to find out how many args it gets */
    char commandBuffer[maxInstOrCommLen];
    int reservedInd,commBuffInd;
    commBuffInd=0;
    if(linePointer[lineInd].validLabel==TRUE){
        reservedInd = reserved_Ind_WLabel(buffer);
    } else {
        reservedInd = reservedIndNoLabel(buffer);
    }
    for(reservedInd;isspace(buffer[reservedInd])==0&& buffer[reservedInd]!='\0';reservedInd++){
        /* this loop goes true all non blank char from the beginning of the command in order to copy ot to the new buffer */
        commandBuffer[commBuffInd++] = buffer[reservedInd];}
    commandBuffer[commBuffInd] = '\0';
    if(isThreeRegArgsReturnFunct(commandBuffer)) {/* this function check if the command is one of the commands that get three registers and return's her opcode in case of a non zero return value */
        linePointer[lineInd].three_operands_three_reg= TRUE;
        linePointer[lineInd].R.funct= isThreeRegArgsReturnFunct(commandBuffer);/* all commands that get three registers are type R commands */
    }
    else if (isTwoRegArgsReturnFunct(commandBuffer)){
        linePointer[lineInd].two_operands_two_registers=TRUE;
        linePointer[lineInd].R.funct= isTwoRegArgsReturnFunct(commandBuffer);/* all commands that get two registers are type R commands */
    }
    else if (is_op1reg_op2_imed_Op3Reg(commandBuffer))
        linePointer[lineInd].three_operands_op1reg_op2imed_op3reg=TRUE;
    else if (is_op1reg_op2reg_op3label(commandBuffer))
        linePointer[lineInd].three_operands_op1reg_op2reg_op3label=TRUE;
    else if(strcmp(commandBuffer,"jmp")==0)
        linePointer[lineInd].one_operand_label_or_reg=TRUE;
    else if (strcmp(commandBuffer,"la")==0|| strcmp(commandBuffer,"call")==0)
        linePointer[lineInd].one_operand_label=TRUE;
}

void commandArgsTest(line *linePointer,int lineInd,label *labelPointer,int labelInd,char *buffer){/* this function check all command line's arguments and codes them
if an error was found, an error flag will rise as well */
/* calling a specific function based on her type */
    raiseCommandsArgFlags(linePointer,lineInd,buffer);
    if(linePointer[lineInd].RtypeCommand==TRUE)
        checkAndEncodeRtypeArgs(linePointer,lineInd,buffer);
    else if (linePointer[lineInd].ItypeCommand==TRUE){
        checkAndEncodeITypeArgs(linePointer,lineInd,labelPointer,lineInd,buffer);
    }
    else if (linePointer[lineInd].JtypeCommand==TRUE){
        checkAndEncodeJTypeArgs(linePointer,lineInd,labelPointer,labelInd,buffer);
    }
}/* end commandArgsTest  function */


void checkAndEncodeRtypeArgs(line *linePointer,int lineInd,char *buffer){/* this function check the R TYPE COMMANDS REGISTERS */
    int firstArgInd;
    if (linePointer[lineInd].validLabel==TRUE){
        firstArgInd = firstArgIndWLabel(buffer);
    }
    else {
        firstArgInd= firstArgIndNoLabel(buffer);
    }
    ValidRegsSyntax(linePointer,lineInd,firstArgInd,buffer);
    

}


void ValidRegsSyntax(line *linePointer,int lineInd, int firstArgInd,char *buffer) {/*   this function checks if the register syntax is valid */
    int firstArgIndLocation, argsBufferInd;
    firstArgIndLocation = firstArgInd;
    if (tripleNum(buffer,firstArgInd)) {/* a function that checks if there are more the two numbers in a row in the registers */
        raiseRegErrorFlags(linePointer, lineInd);
        return;
    }
    checkRegOrderAndCode(linePointer, lineInd, buffer, firstArgInd);
}




void checkRegOrderAndCode(line *linePointer,int lineInd, char *buffer,int firstArgInd){/* this function tracks the registers operands syntax by flags and raise an error in case of a syntax error */
    char numBuff[3];
    int nextNumFlag,nextDollarFlag,nextCommaFlag,nextBlank;/* declaring flags that will help to check if args are in right order */
    int numOfReg;
    nextNumFlag=0,nextDollarFlag=1,nextCommaFlag=0,nextBlank=0,numOfReg=0;
    for (firstArgInd; buffer[firstArgInd] != '\n';firstArgInd++){/* a for loop that will go trough all arguments and if they are valid registers */
        if (buffer[firstArgInd] == dollarAscii|| firstArgInd==0) {
            if (nextDollarFlag == 0) {
                raiseRegErrorFlags(linePointer, lineInd);
                return;
            }
            nextNumFlag = 1, nextCommaFlag = 0, nextDollarFlag = 0, nextBlank = 0;/* checking if the registers syntax is valid by flags,
            if the flag worth a zero and it meets  a char it represents, declare an error */ 
        }
        else if (isspace(buffer[firstArgInd])) {
            if (nextBlank == 0) {
                raiseRegErrorFlags(linePointer, lineInd);
                return;
            }

        }else if (isdigit(buffer[firstArgInd])) {
            if (nextNumFlag == 0) {
                raiseRegErrorFlags(linePointer, lineInd);
                return;
            }
            numBuff[0] = buffer[firstArgInd];/* loading the num into the num buffe r*/
            if (firstArgInd < (strlen(buffer) - 1) && isdigit(buffer[firstArgInd + 1])) {
                numBuff[1] = buffer[firstArgInd + 1];
                firstArgInd++;/* in order to not iterate the same char twice */
                numBuff[2] = '\0';
            } else/* in case the reg is a single char num */
                numBuff[1] = '\0';
            numOfReg++, nextNumFlag = 1, nextCommaFlag = 1, nextDollarFlag = 0, nextBlank = 1;/* the appearance of more than two numbers have been checked in other function */
            codeRTypeRegs(linePointer, lineInd, numBuff, numOfReg);
        } else if (buffer[firstArgInd] == commaAscii) {
            if (nextCommaFlag == 0) {
                raiseRegErrorFlags(linePointer, lineInd);
                return;
            }
            nextNumFlag = 0, nextCommaFlag = 0, nextDollarFlag = 1, nextBlank = 1;
        }
        else {/*if a char is not on a dollar,digit or a comma the char is not valid */
            raiseRegErrorFlags(linePointer, lineInd);
            return;
        }
    }
    /* checking if there is a valid num of arguments */
    if ( linePointer[lineInd].R.opcode==0 && numOfReg>3){/* in case that 0 opcode R type command got more than three arguments */
        raiseExtraneousOperandFlags(linePointer,lineInd);/* raise a flag that declare an extraneous operand error */
        return;
    }
    else if(linePointer[lineInd].R.opcode==0 && numOfReg<3){/* in case that 0 opcode R type command got less than three arguments */
        raiseMissingOperandFlags(linePointer,lineInd);/* raise a flag that declare a missing operand error */
        return;
    }
    if(linePointer[lineInd].R.opcode==1 && numOfReg>2){/* in case that 1 opcode R type command got more than three arguments */
        raiseExtraneousOperandFlags(linePointer,lineInd);/* raise a flag that declare an extraneous operand error */
        return;
    }
    else if (linePointer[lineInd].R.opcode==1 && numOfReg<2){/* in case that 1 opcode R type command got less than two arguments */
        raiseMissingOperandFlags(linePointer,lineInd);/* raise a flag that declare a missing operand error */
        return;
    }
}


void codeRTypeRegs(line *linePointer,int lineInd,char *buffer,int numOfReg){
    int bufferNumValue,ind;
    bufferNumValue = atoi(buffer);
    if (bufferNumValue<0 || bufferNumValue>31){
        raiseRegErrorFlags(linePointer,lineInd);
        return;
    }
    if (linePointer[lineInd].R.opcode==0){
        if (numOfReg==1){
            linePointer[lineInd].R.rs=bufferNumValue;
        }
        else if(numOfReg==2){
            linePointer[lineInd].R.rt=bufferNumValue;
        }
        else if(numOfReg==3){
            linePointer[lineInd].R.rd=bufferNumValue;
        }
        else {/* there are more then three args */
            raiseExtraneousOperandFlags(linePointer,lineInd);
            return;
        }
    }
    else if(linePointer[lineInd].R.opcode==1){
        if (numOfReg==1)
            linePointer[lineInd].R.rs=bufferNumValue;
        else if(numOfReg==2)
            linePointer[lineInd].R.rd=bufferNumValue;
        else {/* there are more then two args */
            raiseExtraneousOperandFlags(linePointer,lineInd);
            return;
        }
    }
    strcpy(buffer, "");/* clearing up the current buffer content in order to get the next reg value */
}









int tripleNum(char *buffer,int firstArgInd){/* a function that checks if there are more then two numbers in a row after the dollar sign in the arguments buffer */
    int tripleNumFlag;
    tripleNumFlag=0;
    for (firstArgInd; buffer[firstArgInd]!='\n';firstArgInd++){
        if (isdigit(buffer[firstArgInd])){
            tripleNumFlag++;
            if (tripleNumFlag==3) {
                return 1;
            }
        }
        else if (isspace(buffer[firstArgInd])==0){
            tripleNumFlag=0;
        }
    }
    return 0;
}

void raiseRegErrorFlags(line *linePointer,int lineInd){/* a function that raise error if an invalid register is found */
    linePointer[lineInd].invalid_register=TRUE;
    linePointer[lineInd]._isError=TRUE;
}




/* valid I-type args functions */

void codeItypeValidLabel(line *linePointer,int lineInd,label *labelPointer,int labelInd,char *buffer){/* this function codes a valid label argument inside a J type command that gets one */ 
    int ind,codedImedFlag;
    codedImedFlag=0;
    if (argLabelCheck(buffer)==0){
        raiseLabelArgError(linePointer,lineInd);
        return;
    }
    for (ind=0;ind<=labelInd;ind++){
        if (strcmp(buffer,labelPointer[ind].labelBuff)==0){
            if (labelPointer[ind].isCode==TRUE){
                linePointer[lineInd].I.immed = (labelPointer[ind].ic- linePointer[lineInd].IC);
                codedImedFlag=1;
            }
        }
    }
    if (codedImedFlag==0) {
        /* if a valid equal label wasnt found, raise a missed label flag , and copy the label argument in order to search her easily in the second pass */
        linePointer[lineInd].i_TypeMissedLabel = TRUE;/* this flag will help to detect the uncoded field in the second pass */
        linePointer[lineInd].I.missedILabel = (char*) calloc(maxLabelLen, sizeof(char));/* this line is creating a dynamic array that will contain the the missed in label for the second pass */
        strcpy(linePointer[lineInd].I.missedILabel,buffer);/* copying the label argument into the dynamic buffer */
    }
}/* end of code Itype valid label function */


void checkAndEncodeITypeArgs(line *linePointer,int lineInd,label *labelPointer,int labelInd,char *buffer){/* this functions checks I-type commands arguments and codes them if they are valid */
    int firstArgInd;
    if (linePointer[lineInd].validLabel==TRUE){
        firstArgInd = firstArgIndWLabel(buffer);
    }
    else if(linePointer[lineInd].validLabel==FALSE){
        firstArgInd= firstArgIndNoLabel(buffer);
    }
    if (linePointer[lineInd].I.opcode<15 || linePointer[lineInd].I.opcode > 18){/*an if statement that checks if a function is not a conditional split command */
        op1RegOp2ImedOp3Reg(linePointer,lineInd,firstArgInd,buffer);/* this function check the validity of the I types command that has an opcode that's lower than 15 and higher than 18 */
    }
    else{/* if the i type command is not a conditional split command such as (beq, bne, blt, bgt) */
        codeOp1RegOp2RegOp3Label(linePointer,lineInd,labelPointer,labelInd,buffer,firstArgInd);
    }
}




void op1RegOp2ImedOp3Reg(line *linePointer,int lineInd,int firstArgInd,char *buffer){/* this function codes the function the I type functions that gets the gets register,imed,register as arguments */
    char op1Buff[regLen], op2Buff[imedNumMaxLen], op3Buff[regLen];
    firstArgInd = fillUpOpBuffer(linePointer,lineInd,buffer,op1Buff,regLen,firstArgInd,1,0,0); /* this function copies the current index argument into a compatible buffer */
    if (firstArgInd==-1)
        return;
    codeValidRegArg(linePointer,lineInd,op1Buff,1);/* this function checks if the register is valid and encodes the register operand */
    firstArgInd= iterateUntilComma(linePointer,lineInd,buffer,firstArgInd);/* this function passes all blank chars until it meets a comma */
    firstArgInd++;/* in order to pass the comma */
    firstArgInd = fillUpOpBuffer(linePointer,lineInd,buffer,op2Buff,imedNumMaxLen,firstArgInd,0,1,0);/* this function copies the current index argument into a compatible buffer */
    if (firstArgInd==-1)/* it means an error has occurred */
        return;
    validImedCoder(linePointer,lineInd,op2Buff);
    firstArgInd= iterateUntilComma(linePointer,lineInd,buffer,firstArgInd);
    firstArgInd++;/* in order to pass the comma */;
    firstArgInd = fillUpOpBuffer(linePointer,lineInd,buffer,op3Buff,regLen,firstArgInd,1,0,0);/* this function copies the current index argument into a compatible buffer */
    if (firstArgInd==-1)/* it means an error has occurred */
        return;
    codeValidRegArg(linePointer,lineInd,op3Buff,3);/* this function checks if the register is valid and encodes the register */
    iterateUntilEnd(linePointer,lineInd,buffer,firstArgInd);/* this function checks if there are more parameters than needed */
}

int fillUpOpBuffer(line *linePointer,int lineInd,char* buffer,char* opBuffer,int bufferLen,int firstArgInd,int isReg,int isImed,int isLabel){/* this function fillUp a buffer with the reg content */
    int nonBlankCounter, bufferInd, stoppedOnComma;
    stoppedOnComma = 0, bufferInd = 0,nonBlankCounter = 0;
    for (firstArgInd;buffer[firstArgInd]==tabAscii||buffer[firstArgInd]==spaceAscii;firstArgInd++);/*looping tru blank chars that may appear after comma */
    for (firstArgInd; buffer[firstArgInd] != '\n' && (buffer[firstArgInd]!=tabAscii&& buffer[firstArgInd]!=spaceAscii); firstArgInd++) {/* a for loop that fills the first arg buffer */
        /* this for loop copies the arguments into another in order to make it easier to check 
        and stops when it reachs a comma or a new line */
        if (buffer[firstArgInd] == commaAscii) {
            stoppedOnComma = 1;
            break;}
        if (nonBlankCounter > regLen - 1&& isReg==1) {
            raiseRegErrorFlags(linePointer, lineInd);
            return -1;
        }
        else if(isImed && nonBlankCounter>imedNumMaxLen-1){
            raiseImedFlag(linePointer,lineInd);
            return -1;
        } else if (isLabel && nonBlankCounter>maxLabelLen-1){
            raiseLabelArgError(linePointer,lineInd);
            return -1;
        }
        nonBlankCounter++;
        opBuffer[bufferInd++] = buffer[firstArgInd];
    }
    if (nonBlankCounter==0){/* if there is no argument to fill up the buffer with*/
        raiseMissingOperandFlags(linePointer,lineInd);
        return -1;
    }
    opBuffer[bufferInd] = '\0';
    return firstArgInd;/* this function return the updated index in order to make arguments tracking easier */
}

void codeValidRegArg(line *linePointer,int lineInd, char *buffer,int numOfArg){/* this function check if an arg is valid and code it */
    char regNumBuff[maxRegNumWending];
    int regNumVal;
    if(buffer[0]!=dollarAscii){
        raiseRegErrorFlags(linePointer,lineInd);
        return;
    }
    if (isdigit(buffer[1])){
        regNumBuff[0] = buffer[1];
        if(isdigit(buffer[2])){/* in case the reg number is grater than 9 */
            regNumBuff[1] = buffer[2];
            regNumBuff[2] = '\0';
        } else
            regNumBuff[1] = '\0';
    }
    else {
        raiseRegErrorFlags(linePointer,lineInd);
        return;
    }
    regNumVal = atoi(regNumBuff);/* converting the string into a num */
    if(regNumVal<0 || regNumVal>31){
        raiseRegErrorFlags(linePointer,lineInd);
        return;
    } else if (numOfArg==1) {
        linePointer[lineInd].I.rs = regNumVal;/* code the valid the valid reg*/
    } else
        linePointer[lineInd].I.rt = regNumVal;/* code the valid the valid reg*/
}

void validImedCoder(line *linePointer,int lineInd,char *buffer){/* this function code the valid imed and raise an error flag if imed isn't valid */
    int ind,imedIntVal;
    for (ind= 0;buffer[ind]!='\0';ind++){
        if (isdigit(buffer[ind]==0)){
            if (ind==0 &&(buffer[ind]==plusAscii || buffer[ind]==minusAscii))
                continue;
            else {
                raiseImedFlag(linePointer,lineInd);
                return;
            }
        }
    }
    imedIntVal = atoi(buffer);
    if(imedIntVal>highest16bitNum || imedIntVal<lowest16bitNum){/* checking if the imed operand can be store inside a 16bit variable */
        raiseImedFlag(linePointer,lineInd);
        return;
    }
    linePointer[lineInd].I.immed = imedIntVal;
}

void raiseImedFlag(line *linePointer,int lineInd){/* this function raises an imed operand error flags */
    linePointer[lineInd]._isError=TRUE;
    linePointer[lineInd].invalid_immed=TRUE;
}


void codeOp1RegOp2RegOp3Label(line *linePointer,int lineInd,label *labelPointer,int labelInd,char *buffer,int firstArgInd){/* a function that checks the beq, bne, blt, bgt i-type commands operands and codes them if they are valid */
    char op1Buff[regLen], op2Buff[regLen], op3Buff[maxLabelLen];
    firstArgInd = fillUpOpBuffer(linePointer,lineInd,buffer,op1Buff,regLen,firstArgInd,1,0,0); /* this function copies the current index argument into a compatible buffer */
    if (firstArgInd==-1)
        return;
    codeValidRegArg(linePointer,lineInd,op1Buff,1);/* this function checks if the register is valid and encodes the register operand */
    firstArgInd= iterateUntilComma(linePointer,lineInd,buffer,firstArgInd);/* this function passes all blank chars until it meets a comma */
    firstArgInd++;/* in order to pass the comma */
    firstArgInd = fillUpOpBuffer(linePointer,lineInd,buffer,op2Buff,imedNumMaxLen,firstArgInd,1,0,0);/* this function copies the current index argument into a compatible buffer */
    if (firstArgInd==-1)/* it means an error has occurred */
        return;
    codeValidRegArg(linePointer,lineInd,op2Buff,2);
    firstArgInd= iterateUntilComma(linePointer,lineInd,buffer,firstArgInd);
    firstArgInd++;/* in order to pass the comma */;
    firstArgInd = fillUpOpBuffer(linePointer,lineInd,buffer,op3Buff,maxLabelLen,firstArgInd,0,0,1);/* this function copies the current index argument into a compatible buffer */
    if (firstArgInd==-1)/* it means an error has occurred */
        return;
    
    codeItypeValidLabel(linePointer,lineInd,labelPointer,labelInd,op3Buff);/* this function checks if the register is valid and encodes the register */
    if (linePointer[lineInd]._isError==TRUE)
        return;
    
    iterateUntilEnd(linePointer,lineInd,buffer,firstArgInd);/* this function checks if there are more parameters than needed */
}
/* end of I-type args functions */

/* j-type commands arg functions */

void checkAndEncodeJTypeArgs(line *linePointer,int lineInd,label *labelPointer,int labelInd,char* buffer){/* this function checks all J-type commands args and codes them */
    int firstArgInd;
    if (linePointer[lineInd].validLabel==TRUE){/* in case the command has a label */
        firstArgInd = firstArgIndWLabel(buffer);
    } else{/* in case the command does not have a label */
        firstArgInd = firstArgIndNoLabel(buffer);
    }

    if (linePointer[lineInd].J.opcode==stopOpCode) {
        checkStop(linePointer, lineInd, buffer, firstArgInd);
    }
    else if (linePointer[lineInd].J.opcode==jmpOpcode){/* it means the function can get a label or an register (only one of them) as an argument */
        checkAndCodeJmp(linePointer,lineInd,labelPointer,labelInd,buffer,firstArgInd);
    }
    else{
        checkAndCodeLaAndCall(linePointer,lineInd,labelPointer,labelInd,buffer,firstArgInd);
    }

}

void checkStop(line *linePointer,int lineInd,char *buffer,int firstArgInd){
    iterateUntilEnd(linePointer,lineInd,buffer,firstArgInd);/* in case of a valid stop command the only non-blank char that comes  after stop is a  new line char,this function check's it */
    linePointer[lineInd].J.reg=0;
    linePointer[lineInd].J.address=0;
    /* the opcode was coded in other function */
}


void checkAndCodeJmp(line *linePointer,int lineInd,label *labelPointer,int labelInd,char *buffer,int firstArgInd){/* a function that checks jmp arg and codes it if its valid */
    int isReg;
    isReg =0;
    char labelBuff[maxLabelLen],regBuff[regLen];
    if (buffer[firstArgInd]==dollarAscii){
        firstArgInd = fillUpOpBuffer(linePointer,lineInd,buffer,regBuff,regLen,firstArgInd,1,0,0);/* a function that returns a buffer that contains the argument only */
        if (firstArgInd==-1){/* it means the arg is to long and cant be valid */
            raiseRegErrorFlags(linePointer,lineInd);/* raising an invalid register error */
            return;
        }
        checkJmpRegAndCode(linePointer,lineInd,regBuff);/*there is a suspected register as an argument check if he's valid and code in case it is */
    }
    else {
        firstArgInd = fillUpOpBuffer(linePointer, lineInd, buffer, labelBuff, maxLabelLen, firstArgInd, 0, 0, 1);
        if (firstArgInd==-1){/* it means the arg is to long and cant be valid */
            raiseLabelArgError(linePointer,lineInd);/* raising an invalid argument error */
            return;
        }
        codeJTypeValidLabel(linePointer,lineInd,labelPointer,labelInd,labelBuff);
    }
    iterateUntilEnd(linePointer,lineInd,buffer,firstArgInd);/* a function that checks if they are more arguments than needed (one argument in this case) */
}
void checkJmpRegAndCode(line *linePointer,int lineInd, char *buffer) {/* this function check if an arg is valid and code it */
    char regNumBuff[maxRegNumWending];
    int regNumVal;
    if (buffer[0] != dollarAscii) {
        raiseRegErrorFlags(linePointer, lineInd);
        return;
    }
    if (isdigit(buffer[1])) {
        regNumBuff[0] = buffer[1];
        if (isdigit(buffer[2])) {/* in case the reg number is grater than 9 */
            regNumBuff[1] = buffer[2];
            regNumBuff[2] = '\0';
        } else
            regNumBuff[1] = '\0';
    } else {
        raiseRegErrorFlags(linePointer, lineInd);
        return;
    }
    regNumVal = atoi(regNumBuff);/* converting the string into a num */
    if (regNumVal < 0 || regNumVal > 31) {
        raiseRegErrorFlags(linePointer, lineInd);
        return;
    }
    linePointer[lineInd].J.reg = 1;
    linePointer[lineInd].J.address = regNumVal;
}/* end of checkJmpRegAndCode */

void codeJTypeValidLabel(line *linePointer,int lineInd,label *labelPointer,int labelInd,char *buffer){/* this function codes a J type command label argument */ 
    int ind,codedImedFlag;
    codedImedFlag=0;
    if (argLabelCheck(buffer)==0){/* if the label argument isnt valid raise an error falg */
        raiseLabelArgError(linePointer,lineInd);
        return;
    }
    for (ind=0;ind<=labelInd;ind++){/* loop trough the labe array and check if an equal label was found */
        if (strcmp(buffer,labelPointer[ind].labelBuff)==0){
            if (labelPointer[ind].isCode==TRUE){
                linePointer[lineInd].J.address = labelPointer[ind].ic;
                codedImedFlag=1;
            }
            else if (labelPointer[ind].isExt==TRUE && labelPointer[labelInd].ignoreLabel==FALSE){
                linePointer[lineInd].J.address =0;
                codedImedFlag=1;
            }
        }
    }
    if (codedImedFlag==0)/* in case nothing was coded in the first pass */
        linePointer[lineInd].j_TypeMissedLabel=TRUE;/* this flag will help to detect the uncoded field in the second pass */
    linePointer[lineInd].J.missedJLabel = (char*) calloc(maxLabelLen,sizeof(char ));/* this line is creating a dynamic array that will contain the the missed in label for the second pass */
    strcpy(linePointer[lineInd].J.missedJLabel,buffer);
}


void checkAndCodeLaAndCall(line *linePointer,int lineInd,label *labelPointer,int labelInd,char *buffer,int firstArgInd){/* a function that checks la and call j-type commands and codes their arguments */
    char labelArgBuff[maxLabelLen]; /* a buffer that will contain the label in order to check her syntax */
    firstArgInd = fillUpOpBuffer(linePointer,lineInd,buffer,labelArgBuff,maxLabelLen,firstArgInd,0,0,1);
    if (firstArgInd==-1){
        raiseLabelArgError(linePointer,lineInd);
        return;
    }
    codeJTypeValidLabel(linePointer,lineInd,labelPointer,labelInd,labelArgBuff);
    iterateUntilEnd(linePointer,lineInd,buffer,firstArgInd);/* a function that checks if there are extronus arguments in these J-type commands */
}














/* j-type commands arg functions end */



/* valid extern functions */



void extArgCheckWLabel(line *linePointer,label *labelPointer,int lineInd,int labelInd,char *buffer){/* checking an extern line with a label */
    char argBuffer[maxLabelLen];/* extern argument needs to be a valid label */
    int argInd, argBufferInd;
    argInd = firstArgIndWLabel(buffer);
    argBufferInd=0;
    for (argInd;isspace(buffer[argInd])==0 && buffer[argInd] != '\n'; argInd++) {
        argBuffer[argBufferInd++] = buffer[argInd];
    }
    argBuffer[argBufferInd] = '\0';
    iterateUntilEnd(linePointer,lineInd,buffer,argInd);/* this function checks if there is a non blank char that comes after .extern label argument */

    if(argLabelCheck(argBuffer)==0){
        raiseLabelArgError(linePointer, lineInd);
        return;
    }
    if(extArgDuplication(labelPointer,labelInd,argBuffer)) {
        raiseLabelArgError(linePointer, lineInd);
        return;
    }

    linePointer[lineInd].validLabel = FALSE;/* giving the valid label flag a false value in order the program will ignore the extern label and wont create an extra cell in the label array */
    linePointer[lineInd].ext.extArg = (char*) calloc(maxLabelLen,sizeof(char ));
    strcpy(linePointer[lineInd].ext.extArg,argBuffer);


}

void extArgCheckNoLabel(line *linePointer,label *labelPointer,int lineInd,int labelInd,char *buffer){/* checking an extern line without a label */
    char argBuffer[maxLabelLen];/* extern argument needs to be a valid label */
    int argInd, argBufferInd;
    argInd = firstArgIndNoLabel(buffer);
    argBufferInd=0;
    for (argInd;isspace(buffer[argInd])==0 &&buffer[argInd] != '\n'; argInd++) {
        argBuffer[argBufferInd++] = buffer[argInd];
    }
    argBuffer[argBufferInd] = '\0';
    argInd++;
    iterateUntilEnd(linePointer,lineInd,buffer,argInd);/* this function checks if there is a non blank char that comes after .extern label argument */
    if(argLabelCheck(argBuffer)==0){/* if the label arg s not a valid label raise an error flag */
        raiseLabelArgError(linePointer, lineInd);
        return;
    }
    if(extArgDuplication(labelPointer,labelInd,argBuffer)) {
        raiseLabelArgError(linePointer, lineInd);
        return;
    }
    linePointer[lineInd].ext.extArg = (char*) calloc(maxLabelLen,sizeof(char ));
    strcpy(linePointer[lineInd].ext.extArg,argBuffer);

}


int extArgDuplication(label *labelPointer,int labelInd,char *buffer){
    int ind;
    for (ind=0;ind<labelInd;ind++){
        if (strcmp(labelPointer[ind].labelBuff,buffer)==0){
            if (labelPointer[ind].isExt==FALSE && labelPointer[labelInd].ignoreLabel==FALSE){/* if the extern arg has been given to a non extern reserved word and this label is not declared as an ignored label return an error */
                return 1;/* there is an error label duplication */
            }

        }

    }
    return 0;
}


void extArgTest(line *linePointer,label *labelPointer,int lineInd,int labelInd,char *buffer) {/* this function check .extern argument */
    if (linePointer[lineInd].validLabel == TRUE) {
        extArgCheckWLabel(linePointer, labelPointer, lineInd, labelInd, buffer);
    } else if (linePointer[lineInd].validLabel == FALSE) {
        extArgCheckNoLabel(linePointer, labelPointer, lineInd, labelInd, buffer);
    }
}


/* end of valid extern functions */


/* entry label saving functions */

void handleEntryArg(line *linePointer,int lineInd,char *buffer){/* this function copies entry's argument in order to check if its valid in the second pass */
    int firstArgInd,labelBuffInd,nonBlankCounter;
    char labelBuff[maxLabelLen];
    labelBuffInd=0,nonBlankCounter=0;
    if(linePointer[lineInd].validLabel==TRUE)/* the index value of the first argument changes in case of a label */
        firstArgInd = firstArgIndWLabel(buffer);
    else
        firstArgInd = firstArgIndNoLabel(buffer);
    for (firstArgInd;buffer[firstArgInd]!='\n' && isspace(buffer[firstArgInd])==0;firstArgInd++){/* looping from the begging of the argument until the buffer end's or until it gets a blank char */
        if (labelBuffInd>maxLabelLen){/* if th argument length is bigger than the max size of a label it must be an invalid one */
            raiseLabelArgError(linePointer,lineInd);
            return;
        }
        nonBlankCounter++;
        labelBuff[labelBuffInd++] = buffer[firstArgInd];
    }
    if (nonBlankCounter==0){/* it means that there wasn't an argument after the entry reserved word */
        raiseMissingOperandFlags(linePointer,lineInd);/* raise the missing argument flag in order to print a valid error input */
        return;
    }
    iterateUntilEnd(linePointer,lineInd,buffer,firstArgInd);/* a function that checks if there is another extraneous argument, if there is - it raises a relevant flag */
    labelBuff[labelBuffInd] = '\0';
    linePointer[lineInd].entryArgument = (char*) calloc(maxLabelLen,sizeof(char ));/* initializing dynamic memory in order to save the entry argument for a smoother experience in the second pass */
    strcpy(linePointer[lineInd].entryArgument,labelBuff);
}

/* end of entry label saving functions */

/* instructions args functions */

void raiseAscizErrorFlags(line *linePointer,int lineInd){/* this function raise a  error flags in case of invalid .asciz char argument */
    linePointer[lineInd].invalid_asciz_stringArg=TRUE;
    linePointer[lineInd]._isError=TRUE;
}

void raiseDotDErrorFlags(line *linePointer,int lineInd){/* this function raise a error flags in case of invalid .d argument */
    linePointer[lineInd].invalid_rang_dotD_arg=TRUE;
    linePointer[lineInd]._isError=TRUE;
}


void instructionsArgsTest(line *linePointer,int lineInd,char *buffer,int *DC){/* these function handle the instruction's arguments besed on their kind */
    int firstArgInd;
    if (linePointer[lineInd].validLabel==TRUE)/* the first argument index value depends whether the line contains a label or not */
        firstArgInd = firstArgIndWLabel(buffer);
    else
        firstArgInd = firstArgIndNoLabel(buffer);
/* giving the instruction the right function */
    if (linePointer[lineInd].is_asciz){
        checkAndCodeAsciz(linePointer,lineInd,buffer,firstArgInd,DC);
    } else
        CodeDot_d(linePointer,lineInd,buffer,firstArgInd,DC);
}


void checkAndCodeAsciz(line *linePointer,int lineInd,char *buffer,int firstArgInd,int *DC){/* this function check and code the argument in an asciz type instruction line */
    int numOfChars;
    numOfChars=0;
    if (buffer[firstArgInd]!=parenthesesAscii) {/* valid .asciz argument must start with parentheses */
        raiseAscizErrorFlags(linePointer, lineInd);
        return;
    }
    firstArgInd++;
    for (firstArgInd;buffer[firstArgInd]!=parenthesesAscii && buffer[firstArgInd]!='\n';firstArgInd++){/* looping the argument until a new line char of prenthesses */
        if (isprint(buffer[firstArgInd])){
            numOfChars++;
            if(numOfChars==1) {
                linePointer[lineInd].ascizDataImage.stringArray = calloc(numOfChars,sizeof(char ));
                linePointer[lineInd].ascizDataImage.stringArray[numOfChars-1] = buffer[firstArgInd];
                linePointer[lineInd].ascizDataImage.biteSize+=1;
            }
            else{
                linePointer[lineInd].ascizDataImage.stringArray= realloc(linePointer[lineInd].ascizDataImage.stringArray,numOfChars);
                if (!(linePointer[lineInd].ascizDataImage.stringArray)){/* in case of an allocation error raise relevant flags and return */
                    raiseAllocationError(linePointer,lineInd);
                    free(linePointer[lineInd].ascizDataImage.stringArray);
                    return;
                }
                linePointer[lineInd].ascizDataImage.stringArray[numOfChars-1] = buffer[firstArgInd];
                linePointer[lineInd].ascizDataImage.biteSize+=1;/* increasing the bite size in order to track the arguments array */
            }
        }
        else{/* printable chars are the only ones allowed */
            raiseAscizErrorFlags(linePointer,lineInd);
            free(linePointer[lineInd].ascizDataImage.stringArray);
            return;}
    }
    if (buffer[firstArgInd]==parenthesesAscii){/* it means the argument ended in a valid way */
        numOfChars++;
        linePointer[lineInd].ascizDataImage.stringArray= realloc(linePointer[lineInd].ascizDataImage.stringArray,numOfChars);/* extending the array in order to put a string ending char in the end */
        if (!(linePointer[lineInd].ascizDataImage.stringArray)){
            raiseAllocationError(linePointer,lineInd);
            free(linePointer[lineInd].ascizDataImage.stringArray);
            return;
        }
        linePointer[lineInd].ascizDataImage.stringArray[numOfChars-1] = '\0';/* a null char is needed in strings */
        linePointer[lineInd].ascizDataImage.biteSize+=1;/* increasing the bite size in order to track the arguments array */
        *DC+=linePointer[lineInd].ascizDataImage.biteSize;/* extending dc size by adding the amount of bites the asciz instruction takes in order to track the next instruction address */
    
    }
    else{
        raiseAscizErrorFlags(linePointer,lineInd);
        free(linePointer[lineInd].ascizDataImage.stringArray);
    }
    firstArgInd++;/* in to pass the comma it stopped on */
iterateUntilEnd(linePointer,lineInd,buffer,firstArgInd);
}

void CodeDot_d(line *linePointer,int lineInd,char *buffer,int firstArgInd,int* DC){/* this function get a .d instruction and points it to the right function */
    if (linePointer[lineInd].is_db==TRUE)
        checkAndCode_dot_d(linePointer,lineInd,buffer,firstArgInd,dbMaxArgLen,dbBites);
    else if (linePointer[lineInd].is_dh==TRUE)
        checkAndCode_dot_d(linePointer,lineInd,buffer,firstArgInd,dhMaxArgLen,dhBites);
    else if (linePointer[lineInd].is_dw==TRUE)
        checkAndCode_dot_d(linePointer,lineInd,buffer,firstArgInd,dwMaxArgLen,dwBites);
    *DC+=linePointer[lineInd].dataImage.biteSize;/* extending dc by adding the amount of bites given as arguments in order to track the next line address */
          
}








void checkAndCode_dot_d(line *linePointer,int lineInd,char *buffer,int firstArgInd,int argLen,int numOfBites) {/* this function check all .d directives argument's and copies them into the data imae if they are valid */
    int argVal, numOfArgs,argIndStart;
    char argBuffer[dwMaxArgLen]; /* initializing the argBuffer to the size of the highest argument the instruction can get*/
    numOfArgs = 0,argIndStart=0;
    while (firstArgInd > 0) {/* -2 means that all of the line was checked, -1 means that an error has occurred */
        firstArgInd = fillUpInsArgBuffer(linePointer, lineInd, buffer, argBuffer,argLen, firstArgInd);
        
        if (firstArgInd == -1)/* -1 means that an error has occurred */
            return;
        argVal = atoi(argBuffer);
        if (valid_dot_d_argVal(linePointer, lineInd, argBuffer) == 0){/* checking if the num of bites each argument can get is able to hold the argument value */
            raiseDotDErrorFlags(linePointer, lineInd);/* raising a dot d directive "argument out of range" flag */
            free(linePointer[lineInd].dataImage.argumentsArray);
            return;
        }
        numOfArgs++;/* increasing the number of arguments by one */
        if (numOfArgs==1){
            linePointer[lineInd].dataImage.argumentsArray = (char*)calloc(numOfBites,sizeof(char));
            memcpy(&linePointer[lineInd].dataImage.argumentsArray[argIndStart],&argVal,numOfBites);/* coping argument checked value into argument array ina specific num of bites*/
            linePointer[lineInd].dataImage.biteSize+=numOfBites;/* increasing bite size in order to track the num of bites we got from arguments */
            argIndStart+=numOfBites;/* increasing the bite index in order to track the arguments array */
            
        }
        else{/* when the pointer was allready initilized with memory, realloc for more */
            linePointer[lineInd].dataImage.argumentsArray = (char*) realloc(linePointer[lineInd].dataImage.argumentsArray,numOfArgs*numOfBites*sizeof(char));
            if (!(linePointer[lineInd].dataImage.argumentsArray)){/* if there no enough space for memory reallocation */
                raiseAllocationError(linePointer,lineInd);/* raise allocation error flag */
                free(linePointer[lineInd].dataImage.argumentsArray);
                return;
            }
            memcpy(&linePointer[lineInd].dataImage.argumentsArray[argIndStart],&argVal,numOfBites);
            linePointer[lineInd].dataImage.biteSize+=numOfBites;/* increasing bite size in order to track the num of bites we got from arguments */
            argIndStart+=numOfBites;/* increasing the bite index in order to track the arguments array */
        }
        strcpy(argBuffer,"");/* this line cleans up the buffer before it gets the new argument */
        

    }
    if(numOfArgs==0){
        raiseMissingOperandFlags(linePointer,lineInd);
        free(linePointer[lineInd].dataImage.argumentsArray);
    }
}









int fillUpInsArgBuffer(line *linePointer,int lineInd,char *buffer,char *argBuffer,int maxLen,int firstArgInd) {/* this int function get an ind varible and an buffer pointer and 
filling buffer with the argument until it reaches a new line or a comma, and return's the updates index */
    int blankFlag,argBufferInd,argLen,curArgStart;
    blankFlag = 0, argBufferInd = 0,curArgStart = firstArgInd;
    argLen =0;/* a variable that is meant to track the arg length */
    for (firstArgInd;buffer[firstArgInd]!='\n' && isspace(buffer[firstArgInd]);firstArgInd++);/* a loop that was created in order to pass all blank chars that may appear after comma */

    for (firstArgInd; buffer[firstArgInd] != '\n' && buffer[firstArgInd] != commaAscii; firstArgInd++) {/* a loop that checks if the argument is valid and fills up his buffer */
        if (isspace(buffer[firstArgInd])) {
            blankFlag = 1;/* "turning on" the blank char flag */
            continue;/* there is no point in getting the blank chars inside the argument buffer */
        }
        if (blankFlag == 1 && isspace(buffer[firstArgInd])==0) {
            raiseExtraneousOperandFlags(linePointer,lineInd);/* blank chars are not allowed between argument numbers */
            return -1;
        }
        if (isdigit(buffer[firstArgInd]==0)){
            if (!(firstArgInd==curArgStart && (buffer[firstArgInd]==plusAscii || buffer[firstArgInd]==minusAscii))){/* excluding the minus and the plus signs that may can in the beginning */
                raiseDotDErrorFlags(linePointer,lineInd);/* blank chars are not allowed between argument numbers */
                return -1;
            }
        }
        if(argLen==maxLen){/* ensuring that we dont get an argument that cant be valid by his length */
            raiseDotDErrorFlags(linePointer,lineInd);;/* blank chars are not allowed between argument numbers */
            return -1;
        }
        argBuffer[argBufferInd++] = buffer[firstArgInd];
        argLen++;
    }
    argBuffer[argBufferInd] = '\0';
    if (buffer[firstArgInd]==commaAscii){
        firstArgInd++;/* in order to pass the comma */
    }
    else if (buffer[firstArgInd]=='\n'){
        return -2;/* it means that there are no more arguments to check */
    }
    return firstArgInd;
}/* end of fillUpInsArgBuffer */

int valid_dot_d_argVal(line *linePointer,int lineInd,char *buffer){/* this int function checks if the argument that was given to a .d instruction is valid 
based on her type */
    int argVal;
    argVal = atoi(buffer);
    if (linePointer[lineInd].is_db==TRUE){
        if (argVal<dbMin|| argVal>dbMax)
            return 0;
        else
            return 1;
    }
    else if (linePointer[lineInd].is_dh==TRUE){
        if (argVal<dhMin|| argVal>dhMax)
            return 0;
        else
            return 1;
    }
    else if(linePointer[lineInd].is_dw==TRUE){
        if (argVal<dwMin|| argVal>dwMax)
            return 0;
        else
            return 1;
    }
}/* end of valid .d argument value function */

void raiseAllocationError(line *linePinter,int lineInd){/* this function raise a memory allocation error flag */
    linePinter[lineInd].allocationError=TRUE;
    linePinter[lineInd]._isError=TRUE;
}

void raiseLabelArgError(line *linePointer,int lineInd){/* this function raise an invalid label argument error flag */
    linePointer[lineInd].invalid_labelArg=TRUE;
    linePointer[lineInd]._isError=TRUE;
}