
#ifndef MAMAN14V10_STRHANDLE_H
#define MAMAN14V10_STRHANDLE_H
#include <string.h>
#include "dataStructures.h"
#include <stdio.h>
#include "reservedWord.h"
#include "stdlib.h"
#include <ctype.h>
#define maxLabelLen 32
#define capitalAascii 65
#define capitalZascii 90
#define lowCap_a_ascii 97
#define lowCap_z_ascii 122
#define digitsStart 48
#define digitsEnd 57
#define colonAscii 58
#define maxLineLen 81
#define maxInstOrCommLen 8
#define regLen 4
#define highest16bitNum +32767
#define lowest16bitNum -32768
#define plusAscii 43
#define minusAscii 45
#define imedNumMaxLen 7
int nonBlankInd(char *);
int isBlank(char *);
int isComment(char *);
int labelCharTest(char*,int);
int labelDigitsCheck(char*,int);
void isLabel(line *,label *,char *,int,int);
void labelTest(line*,label*,int,int,char*,int);
int firstArgIndWLabel(char *);
int firstArgIndNoLabel(char *);
int labelDuplicationCheck(label *,int,char *);
int labelColonCheck(char*,int);
void raiseLabelError(line*,int);
int lastCharColonChecker(char *);
void validFirstWord(line *,int,char*,int*,int*);
int reserved_Ind_WLabel(char*);
void ReservedRelevantFlagsWLabel(line *,label*,int, int,char *,int*,int*);
void reservedWithLabel(line *,label *,char *,int,int,int*,int*);
void firstWordRelevantFlags(line *,int ,char *,int*,int*);
void raiseInstructionFlag(line*,int,char*);
void raiseTypeFlags(line*,int,char*);
int validLabelArg(char*);
int argLabelCheck(char *);
int extArgDuplication(label *,int ,char *);
void extArgTest(line *,label *,int ,int,char *);
void opCodeCoder(line *,int l,char *);
void RTypeOpCoder(line *,int ,char *);
void ITypeOpCoder(line *,int ,char *);
void JTypeOpCoder(line *,int ,char *);
int reservedIndNoLabel(char*);
void checkAndEncodeRtypeArgs(line *,int,char *);
int doubleComma(char *);
int doubleDollar(char *);
int tripleNum(char*,int);
void ValidRegsSyntax(line *,int , int,char*);
void commandArgsTest(line *,int ,label *,int ,char *);
void checkRegOrderAndCode(line *,int,char*,int);
void codeRTypeRegs(line *,int,char *,int);
void codeValidRegArg(line *,int, char *,int);
void validImedCoder(line *,int,char*);
void raiseRegErrorFlags(line *,int);
int fillUpOpBuffer(line *,int,char* ,char* ,int,int,int,int,int);
void raiseExtraneousOperandFlags(line *,int);
void raiseMissingOperandFlags(line *,int);
void raiseAscizErrorFlags(line *,int);
void raiseDotDErrorFlags(line *,int );
void raiseImedFlag(line *,int );
void op1RegOp2ImedOp3Reg(line *,int,int,char *);
void codeOp1RegOp2RegOp3Label(line *,int ,label *,int ,char*,int);
void checkAndEncodeITypeArgs(line *,int ,label*,int,char*);
void checkStop(line *,int ,char *,int );
void checkAndEncodeJTypeArgs(line *,int ,label *,int,char* );
void checkAndCodeJmp(line *,int,label *,int,char *,int);
void checkJmpRegAndCode(line *,int, char *);
void codeJTypeValidLabel(line *,int,label*,int,char*);
void checkAndCodeLaAndCall(line *,int ,label *,int ,char *,int);
int fillUpInsArgBuffer(line *,int ,char *,char *,int ,int );
void handleEntryArg(line *,int ,char *);
void instructionsArgsTest(line *,int,char *,int*);
int valid_dot_d_argVal(line *,int ,char *);
void CodeDot_d(line *,int ,char *,int,int*);
void checkAndCode_d_dot_b(line *,int,char *,int);
void checkAndCode_dot_d(line *,int,char *,int,int,int);
void checkAndCode_d_dot_w(line *,int,char *,int);
void checkAndCodeAsciz(line *,int ,char *,int,int*);
void raiseAllocationError(line *,int);
void raiseLabelArgError(line *,int);


#define jmpOpcode 30
#define tabAscii 9
#define spaceAscii 32
#define semiColonAscii 59
#define parenthesesAscii 34
#define stopOpCode 63
#define regLen 4
#define dollarAscii 36
#define commaAscii 44
#define maxRegNumWending 3
#define stopOpCode 63
#define dwMaxArgLen 12
#define dhMaxArgLen 7
#define dbMaxArgLen 5

#define dbMin -128
#define dbMax 127
#define dhMax 32767
#define dhMin -32768
#define dwMax 2147483647
#define dwMin -2147483648
#define dwBites 4
#define dbBites 1
#define dhBites 2





#endif
