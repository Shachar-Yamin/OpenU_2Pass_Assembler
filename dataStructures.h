#ifndef MAMAN14V10_DATASTRUCTURES_H
#define MAMAN14V10_DATASTRUCTURES_H
#define MaxLabelSize 32 /* it say's that the max length of label is 31 + the '\0' string ending char */
#include "string.h"


typedef enum
{
    FALSE,
    TRUE
} bool;



typedef struct line{
    int IC;
    int DC;
    int address;


    char *entryArgument;


    struct R {
        unsigned long  opcode:6;
        unsigned long  rs:5;
        unsigned long  rt:5;
        unsigned long  rd:5;
        unsigned long  funct:5;
        unsigned long  notused:6;
    }R;

    struct I{
        unsigned long  opcode:6;
        unsigned long rs:5;
        unsigned long rt:5;
        signed long  immed:16;
        char *missedILabel;

    }I;

    struct J{
        unsigned long  opcode:6;
        unsigned long reg:1;
        unsigned long  address:25;
        char *missedJLabel;
    }J;

    struct ext{
        char *extArg;
        int *extAddress;
        int addressInd;

    }ext;



        struct dot_d_dataImage {
           char *argumentsArray;
            int biteSize;
        } dataImage;




    struct ascizDataImage{
        char *stringArray;
        int biteSize;
        int dc;
    }ascizDataImage;
















    /*flags*/

    bool validLabel;

    bool noLabel;
    bool command;
    bool instruction;


    bool RtypeCommand;
    bool ItypeCommand;
    bool JtypeCommand;



    /* error flags based by error type's*/
    bool _isError;
    bool invalid_reserved;
    bool invalid_label;
    bool invalid_register;
    bool invalid_labelArg;
    bool invalid_immed;
    bool invalid_asciz_stringArg;
    bool invalid_rang_dotD_arg;
    bool undefined_label_arg;
    bool allocationError;
    bool missing_Operand;
    bool extraneous_Operand;

    /* end of error flags */



    bool ignoreLabel;
    bool i_TypeMissedLabel;
    bool j_TypeMissedLabel;
    bool unrecognized_directive;
    bool three_operands_three_reg;
    bool two_operands_two_registers;
    bool three_operands_op1reg_op2imed_op3reg;
    bool three_operands_op1reg_op2reg_op3label;
    bool one_operand_label_or_reg;
    bool one_operand_label;


    bool isEntry;
    bool isExtern;

    /* instructions flags */

    bool is_dh;
    bool is_dw;
    bool is_db;
    bool is_asciz;


}line;

typedef struct label{
    char labelBuff[MaxLabelSize];
    int ic;
    int dc;
    bool isCode;
    bool isData;
    bool isExt;
    bool isEnt;
    bool ignoreLabel;
}label;

void initialize_line_structures(line *,int);
void initialize_label_structures(label*,int);




#endif
