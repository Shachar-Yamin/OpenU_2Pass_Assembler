#include "reservedWord.h"
#include "dataStructures.h"




/* creating an array if reserved words that has a common dinominator in order to find a way to write less code and still  treat all of the possible senarios that my appeare in the input file */


char commands[27][5] = {"add","sub","and","or","nor","move","mvhi","mvlo","addi","subi","andi","ori","nori","bne","beq","blt","bgt","lb","sb","lw","sw","lh","sh","jmp","la","call","stop"};

/* these array's hold all commands that has a common dinominator - same number and type of arguments */
const char *three_operands_three_registers[] = {"add","sub","and","or","nor"};
int three_operands_three_registers_length;
three_operands_three_registers_length  = sizeof(three_operands_three_registers) / sizeof(three_operands_three_registers[0]);

const char *two_operands_two_registers[] = {"move","mvhi","mvlo"};
int two_operands_two_registers_length;
two_operands_two_registers_length = sizeof(two_operands_two_registers) / sizeof(two_operands_two_registers[0]);

const char *three_operands_op1reg_op2imed_op3reg[] = {"addi", "subi", "andi","ori", "nori","lb","sb","lw","sw","lh","sh"};
int three_operands_op1reg_op2imed_op3regLen = sizeof(three_operands_op1reg_op2imed_op3reg)/sizeof(three_operands_op1reg_op2imed_op3reg[0]);

const char *three_operands_op1reg_op2reg_op3label[] = {"beq", "bne", "blt","bgt"};
int three_operands_op1reg_op2reg_op3labelLen;
three_operands_op1reg_op2reg_op3labelLen = sizeof(three_operands_op1reg_op2reg_op3label)/sizeof(three_operands_op1reg_op2reg_op3label[0]);

/*end of array's hold all commands that has a common dinominator - same number and type of arguments */



/* these array divide all commands based on thier type */

const char *Rtype_commands[] = {"add","sub","and","or","nor","move","mvhi","mvlo"};
int rtypeLen;
rtypeLen = sizeof (Rtype_commands)/sizeof(Rtype_commands[0]);


const char *Itype_commands[] = {"addi","subi","andi","ori","nori","bne","beq","blt","bgt","lb","sb","lw","sw","lh","sh"};
int itypeLen;
itypeLen = sizeof (Itype_commands)/sizeof(Itype_commands[0]);


const char *Jtype_commands[] =  {"jmp","la","call","stop"};
int jtypeLen;
jtypeLen = sizeof (Jtype_commands)/sizeof(Jtype_commands[0]);

/* end of array's that divide all commands based on thier type */

const char *instructions[] = {".dh",".dw",".db",".asciz",".entry",".extern"};
int instruction_list_length;
instruction_list_length = sizeof(instructions) / sizeof(instructions[0]);

/* these array's hold commands that are divided by thier oppcode */
const char *zeroOpCode[]={"add","sub","and","or","nor"};
int zeroOpCodeLen;
zeroOpCodeLen = sizeof(zeroOpCode) / sizeof(zeroOpCode[0]);


const char *oneOpCode[] = {"move","mvhi","mvlo"};
int oneOpCodeLen;
oneOpCodeLen = sizeof(oneOpCode) / sizeof(oneOpCode[0]);

const char *indPlusTenOpcode[] = {"addi","subi","andi","ori","nori","bne","beq","blt","bgt","lb","sb","lw","sw","lh","sh"};
int indPlusTenOpcodeLen;
indPlusTenOpcodeLen = sizeof(indPlusTenOpcode) / sizeof(indPlusTenOpcode[0]);

const char *indPlusThirtyOpcode[] = {"jmp","la","call"};
int indPlusThirtyOpcodeLen;
indPlusThirtyOpcodeLen = sizeof(indPlusThirtyOpcode)/sizeof(indPlusThirtyOpcode[0]);

/* end of array's that  hold commands that are divided by thier oppcode */


/* these function's help to categorize a reserved word to a specific group that will give us data about her,
the function's categorize a reserved word opcode by searching the reserved words in the array that were mentioned above */ 

int isZeroOp(char *buffer){
    int ind;
    for (ind=0;ind<zeroOpCodeLen;ind++) {
        if(strcmp(buffer,zeroOpCode[ind])==0)
            return 1;
    }
    return 0;
}

int isOneOp(char *buffer){
    int ind;
    for(ind=0;ind<oneOpCodeLen;ind++){
        if(strcmp(buffer,oneOpCode[ind])==0)
            return 1;
    }
    return 0;
}
int isIndPlusTenOp(char *buffer){
    int ind;
    for(ind=0;ind<indPlusTenOpcodeLen;ind++){
        if(strcmp(buffer,indPlusTenOpcode[ind])==0)
            return ind+10;
    }
    return 0;
}
int isIndPlusThirty(char *buffer){
    int ind;
    for(ind=0;ind<indPlusThirtyOpcodeLen;ind++){
        if(strcmp(buffer,indPlusThirtyOpcode[ind])==0)
            return ind+30;
    }
    return 0;
}
/* end of function's that categorize a reserved word opcode */

int isStop(char *buffer){/* this int function check if the reserved word is a stop reserved and retiurn's 1 in case it is, zero if not  */
    if(strcmp(buffer,"stop")==0)
        return 1;
    else
        return 0;
}





int isCommand(char* buffer){/* this function checks if the reserved word is a command and returns 1 if it is, and zero if not */
    int i;
    for (i = 0;i<numOfCommands;i++) {
        if(strcmp(buffer,commands[i])==0)
            return 1;
    }
    return 0;
}

int isInstruction(char *buffer){/* this function checks if the reserved word is an instruction and returns 1 if it is, and zero if not */
    int i;
    for (i = 0;i<instruction_list_length;i++) {
        if(strcmp(buffer,instructions[i])==0)
            return 1;
    }
    return 0;
}
int isExt(char *buffer){/* this function checks if the reserved word is a .extern instruction and returns 1 if it is, and zero if not */
    if(strcmp(".extern",buffer)==0)
        return 1;
    return 0;
}

int isEnt(char *buffer){/* this function checks if the reserved word is a .entry instruction and returns 1 if it is, and zero if not */
    if(strcmp(".entry",buffer)==0)
        return 1;
    return 0;
}

int isRType(char *buffer){/* this function checks if the reserved word is a R type command and returns 1 if it is, and zero if not */
    int ind;
    for (ind=0;ind<rtypeLen;ind++){
        if(strcmp(buffer,Rtype_commands[ind])==0)
            return 1;
    }
    return 0;
}

int isIType(char *buffer){/* this function checks if the reserved word is an I type command and returns 1 if it is, and zero if not */
    int ind;
    for (ind = 0;ind<itypeLen; ind++){
        if(strcmp(buffer,Itype_commands[ind])==0)
            return 1;
    }
    return 0;
}
int isJType(char *buffer){/* this function checks if the reserved word is a J type command and returns 1 if it is, and zero if not */
    int ind;
    for (ind = 0;  ind<jtypeLen ;ind++) {
        if(strcmp(buffer,Jtype_commands[ind])==0)
            return 1;
    }
    return 0;
}



/* these function's check the if the reserved word need to get a spacific amount of args and type of args besed on 
thier appearnce in a spacific array, wich is one of one's mentioned above, they return a non zero if it is appears in the array and zero if not */
int isThreeRegArgsReturnFunct(char *buffer){
    int ind;
    for (ind = 0; ind < three_operands_three_registers_length;ind++) {
        if(strcmp(buffer,three_operands_three_registers[ind])==0)
            return ind+1;
    }
    return 0;
}

int isTwoRegArgsReturnFunct(char *buffer){
    int ind;
    for(ind=0;ind<two_operands_two_registers_length;ind++){
        if(strcmp(buffer,two_operands_two_registers[ind])==0)
            return ind+1;
    }
    return 0;

}

int is_op1reg_op2_imed_Op3Reg(char *buffer){
    int ind;
    for(ind=0;ind<three_operands_op1reg_op2imed_op3regLen;ind++){
        if (strcmp(buffer,three_operands_op1reg_op2imed_op3reg[ind])==0)
            return 1;
    }
    return 0;
}

int is_op1reg_op2reg_op3label(char *buffer){
    int ind;
    for(ind=0;ind<three_operands_op1reg_op2reg_op3labelLen;ind++){
        if (strcmp(buffer,three_operands_op1reg_op2reg_op3label[ind])==0)
            return 1;
    }
    return 0;
}

/* end of argument cetogrizing function's */





