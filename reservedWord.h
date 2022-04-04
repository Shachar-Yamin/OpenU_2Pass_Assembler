#ifndef MAMAN14V10_RESERVEDWORDS_H
#define MAMAN14V10_RESERVEDWORDS_H

#include <string.h>

int isInstruction(char *);
int isCommand(char* );
int isEnt(char *);
int isExt(char *);
int isRType(char*);
int isIType(char*);
int isJType(char*);
int isZeroOp(char *);
int isOneOp(char*);
int isIndPlusTenOp(char *);
int isIndPlusThirty(char*);
int isStop(char *);
int isThreeRegArgsReturnFunct(char*);
int isTwoRegArgsReturnFunct(char *);
int is_op1reg_op2_imed_Op3Reg(char *);
int is_op1reg_op2reg_op3label(char *);

#define numOfCommands 27
#define maxCommandLen 5








#endif
