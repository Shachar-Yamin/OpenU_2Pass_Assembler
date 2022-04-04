#include "dataStructures.h"

void initialize_line_structures(line *linePointer,int lineInd){/* this function initilize's the line type struct flags and internal struct's 
    linePointer[lineInd].IC=0;
    linePointer[lineInd].DC=0;
    linePointer[lineInd].entryArgument =0;


    /* initializing R struct */

    linePointer[lineInd].R.opcode = 0;
    linePointer[lineInd].R.rs = 0;
    linePointer[lineInd].R.rt = 0;
    linePointer[lineInd].R.rd = 0;
    linePointer[lineInd].R.funct = 0;
    linePointer[lineInd].R.notused = 0;
    /* end of initializing R struct */

    /* initializing I struct */
    linePointer[lineInd].I.opcode = 0;
    linePointer[lineInd].I.rs = 0;
    linePointer[lineInd].I.rt = 0;
    linePointer[lineInd].I.immed = 0;
    linePointer[lineInd].I.missedILabel = 0;

    /* end of initializing I struct */

    /* initializing J struct */
    linePointer[lineInd].J.opcode=0;
    linePointer[lineInd].J.reg=0;
    linePointer[lineInd].J.address=0;
    linePointer[lineInd].J.missedJLabel=0;
    /* end of initializing J struct */

    /* initializing ext struct */
    linePointer[lineInd].ext.extArg=0;
    linePointer[lineInd].ext.extAddress=0;
    linePointer[lineInd].ext.addressInd=0;
    /* end of initializing ext struct */


    /* initializing .d data Image struct */
    linePointer[lineInd].dataImage.argumentsArray=0;
    linePointer[lineInd].dataImage.biteSize=0;
    /* end of initializing .d data Image struct */

    /* initializing .asciz data Image struct */
    linePointer[lineInd].ascizDataImage.stringArray=0;
    linePointer[lineInd].ascizDataImage.biteSize=0;
    linePointer[lineInd].ascizDataImage.dc=0;
    /* end of initializing .asciz data Image struct */



    /* initializing flags*/

        linePointer[lineInd].validLabel=FALSE;
        linePointer[lineInd].noLabel=FALSE;
        linePointer[lineInd].command=FALSE;
        linePointer[lineInd].instruction=FALSE;


        linePointer[lineInd].RtypeCommand=FALSE;
        linePointer[lineInd].ItypeCommand=FALSE;
        linePointer[lineInd].JtypeCommand=FALSE;
        linePointer[lineInd]._isError=FALSE;
        /* error flags based by error type's*/
    linePointer[lineInd].invalid_labelArg=FALSE;
    linePointer[lineInd].invalid_immed=FALSE;
    linePointer[lineInd].undefined_label_arg=FALSE;
    linePointer[lineInd].allocationError=FALSE;
    linePointer[lineInd].unrecognized_directive=FALSE;
    linePointer[lineInd].allocationError=FALSE;
    linePointer[lineInd].undefined_label_arg=FALSE;
    linePointer[lineInd].extraneous_Operand=FALSE;
    linePointer[lineInd].missing_Operand=FALSE;
    linePointer[lineInd].extraneous_Operand=FALSE;
    linePointer[lineInd].invalid_register=FALSE;
    linePointer[lineInd].invalid_reserved=FALSE;
    linePointer[lineInd].invalid_label=FALSE;
    linePointer[lineInd].undefined_label_arg=FALSE;
    linePointer[lineInd].allocationError=FALSE;
    linePointer[lineInd].ignoreLabel=FALSE;
    linePointer[lineInd]. i_TypeMissedLabel=FALSE;
    linePointer[lineInd].j_TypeMissedLabel=FALSE;
    linePointer[lineInd].unrecognized_directive=FALSE;
    linePointer[lineInd].invalid_asciz_stringArg=FALSE;

    linePointer[lineInd].three_operands_three_reg=FALSE;
    linePointer[lineInd].two_operands_two_registers=FALSE;
    linePointer[lineInd].three_operands_op1reg_op2imed_op3reg=FALSE;
    linePointer[lineInd].three_operands_op1reg_op2reg_op3label=FALSE;
    linePointer[lineInd].one_operand_label_or_reg=FALSE;
    linePointer[lineInd].one_operand_label=FALSE;



    linePointer[lineInd].isEntry=FALSE;
    linePointer[lineInd].isExtern=FALSE;

        /* instructions flags */

    linePointer[lineInd].is_dh=FALSE;
    linePointer[lineInd].is_dw=FALSE;
    linePointer[lineInd].is_db=FALSE;
    linePointer[lineInd].is_asciz=FALSE;

}/* end of line struct initialize  function  */

void initialize_label_structures(label *labelPointer,int labelInd){ /* this function will itialize the new label array when a new cell is created */
    strcpy(labelPointer[labelInd].labelBuff,"");/* empting the label buff */
    labelPointer[labelInd].ic=0;
    labelPointer[labelInd].dc=0;
    labelPointer[labelInd].isCode=FALSE;
    labelPointer[labelInd].isData=FALSE;
    labelPointer[labelInd].isExt=FALSE;
    labelPointer[labelInd].isEnt=FALSE;
    labelPointer[labelInd].ignoreLabel=FALSE;
}/* end of label struct initialize  function  */