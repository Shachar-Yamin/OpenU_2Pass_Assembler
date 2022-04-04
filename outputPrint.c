#include "outputPrint.h"

/* this function will produce the final output by creating the needed output files based on the input */  
void createOutput(line *linePointer,int lineInd,label *labelPointer,int labelInd,int IC,int DC,int *numOfprintedBites,char *inputFileName,int inputFileLen){
    char currArgVCopy[maxLineLen];/* a pointer that will contain a copy of the current input file we got from argv, this copy will prevent checnges in the argv value that will effect programs efficiency */
    strcpy(currArgVCopy,inputFileName);/*  coping the current input file from argv to his copy buffer) */ 
    createDotObFile(linePointer,lineInd,labelPointer,labelInd,IC,DC,numOfprintedBites,currArgVCopy);/* calling the function that will create the .ob output file */
    createDotEnt(labelPointer,labelInd,currArgVCopy);/* calling the function that will create the .ent file if there is a need for one */
    createDotExt(linePointer,lineInd,currArgVCopy);/* calling the function that will create the .ext file if there is a need for one */
    



}



void createDotObFile(line *linePointer,int lineInd,label *labelPointer,int labelInd,int IC,int DC,int *numOfPrintedByte,char *inputFileName){/* this function creates the ps.ob output file */
    /* variable declartion */
    int numOfCodeBytes,numOfDataBytes,currLineInd;
    long address;
    char obEndingbuffer[DotObEndingLen];
    FILE *obPointer;
    /* creating the .ob File name buffer that will be based on the input file */
    strcpy(obEndingbuffer,".ob");
    obEndingbuffer[DotObEndingLen-1] ='\0';
    inputFileName[strlen(inputFileName)-(DotObEndingLen-1)]='\0';
    inputFileName = strcat(inputFileName,obEndingbuffer);  
    address =100;
    
    obPointer =fopen(inputFileName,"w");
    /* end of creating the .ob File name buffer */

    /* calculating and printing the amount of code and data bytes in the beggining of the .ob file */
    numOfCodeBytes = IC -100;
    numOfDataBytes = DC;
    fprintf(obPointer,"   %d    %d\n",numOfCodeBytes,numOfDataBytes);/* printing the amount of bytes that were used by commands and instructions in the beginning of the files */
    for (currLineInd=0;currLineInd<=lineInd;currLineInd++){/* a for loop that goes trough all commands and print's them */
        /* giving the command line the right printing function based on her specific type */
        if(linePointer[currLineInd].RtypeCommand==TRUE){
            printRToOb(obPointer,linePointer,currLineInd,&address);
            
        }
        else if(linePointer[currLineInd].JtypeCommand==TRUE)
            printJToOb(obPointer,linePointer,currLineInd,&address);
        else if(linePointer[currLineInd].ItypeCommand==TRUE)
            printIToOb(obPointer,linePointer,currLineInd,&address);
    }
    for (currLineInd=0;currLineInd<=lineInd;currLineInd++){/* a for loop that goes trough directives only and print's their data image based on their type */
        if(linePointer[currLineInd].is_asciz==TRUE){
            printAscizToOB(obPointer,linePointer,currLineInd,&address,numOfPrintedByte);
        }
        else if(linePointer[currLineInd].is_dw==TRUE||linePointer[currLineInd].is_db==TRUE||linePointer[currLineInd].is_dh==TRUE){
            printDotD(obPointer,linePointer,currLineInd,&address,numOfPrintedByte);
        }
    }
}



void printRToOb(FILE *obPointer,line *linePointer,int currLineInd,long *address){
    unsigned int the32bitVariable;/* a 32 bit varible that will be fiiled the 32 bit R struct bitfield */
    unsigned int tempVal;
    int ind;
    the32bitVariable=0;
    /* filling the 32 bit varible, most segnificant get filled is entered by moving the right amount of bytes left,after the that  the next most segnificant is being filled the same way*/
    tempVal = linePointer[currLineInd].R.opcode;
    the32bitVariable |= (tempVal<<26); /* we need that the opcode 6 bites will be the most significant bites */
    tempVal = linePointer[currLineInd].R.rs;
    the32bitVariable |= (tempVal<<21);
    tempVal= linePointer[currLineInd].R.rt;
    the32bitVariable |= (tempVal<<16);
    tempVal = linePointer[currLineInd].R.rd;
    the32bitVariable |= (tempVal<<11);
    tempVal = linePointer[currLineInd].R.funct;
    the32bitVariable |= (tempVal<<6);
    tempVal = linePointer[currLineInd].R.notused;
    the32bitVariable |= (tempVal);
    fprintf(obPointer,"%04ld ",*address);/* printing the address first */
    for(ind=0;ind<4;ind++) {
        fprintf(obPointer,"%X",(the32bitVariable & 0xF0)>> 4); /* printing high 4 bits (nibble) masked with 0xF0 in order to clear the lower bibble  */
        fprintf(obPointer,"%X ",the32bitVariable & 0x0F); /* printing lowe 4 bits and a space after them in order to create a separation (nibble) masked with 0xF0 in order to clear the higher bibble */
       /* 1 byte was printed, shifting right 8 bits is needed in order to get to the next byte */
        the32bitVariable >>= 8;
    }
    fprintf(obPointer,"\n");
    *address+=4;/* increasing address by 4 bytes */

}


void  printJToOb(FILE *obPointer,line *linePointer,int currLineInd,long *address){
    unsigned int the32bitVariable;/* a 32 bit varible that will be fiiled the 32 bit J struct bitfield */
    unsigned int tempVal;
    int ind;
    the32bitVariable=0;    /* filling the 32 bit varible, most segnificant get filled is entered by moving the right amount of bytes left,after the that  the next most segnificant is being filled the same way*/
    tempVal = linePointer[currLineInd].J.opcode;
    the32bitVariable |= (tempVal<<26);
    tempVal = linePointer[currLineInd].J.reg;
    the32bitVariable |= (tempVal<<25);
    tempVal = linePointer[currLineInd].J.address;
    the32bitVariable |= (tempVal);
    fprintf(obPointer,"%04ld ",*address);/* printing the address first */


    for(ind=0;ind<4;ind++) {
        fprintf(obPointer,"%X",(the32bitVariable & 0xF0) >> 4); /* high nibble masked with 0xF0 in order to clear the low nibble  */
        fprintf(obPointer,"%X ",the32bitVariable & 0x0F); /* low nibble  with 0x0F in order to clear the high nibble */
        /* 1 byte was printed, shifting right 8 bit is needed in order to get to the next byte */
        the32bitVariable >>= 8;/* moving to the neext 8 bites by shifting right */
    }
    fprintf(obPointer,"\n");
    *address+=4;/* increasing address by 4 bytes */
}


/* needed fix*/
void printIToOb(FILE *obPointer,line *linePointer,int currLineInd,long *address){
    unsigned int the16bitUnsignedVariable;/* a 16 unsigned bit varible that will be fiiled the unsigned 16 bit  of the 32 bit J struct bitfield */
    signed int the16BitSignedTempVAL;/* a 16 signed bit varible that will be fiiled the signed 16 bit  of the 16 bit J struct bitfield */
    unsigned int tempUnsignedVal;
    signed int tempSignedTempVal;
    int ind;
    the16bitUnsignedVariable=0, the16BitSignedTempVAL=0;
    /* unsigned part - filling the 16 bit varible, most segnificant filled is entered by moving the right amount of bytes left,after the that  the next most segnificant is being filled the same way*/
    tempUnsignedVal = linePointer[currLineInd].I.opcode;
    the16bitUnsignedVariable |= (tempUnsignedVal<<10);
    tempUnsignedVal = linePointer[currLineInd].I.rs;
    the16bitUnsignedVariable |= (tempUnsignedVal<<5);

    tempUnsignedVal = linePointer[currLineInd].I.rt;
    the16bitUnsignedVariable |= (tempUnsignedVal);
    /* signed part  immed bifield is 16 bite long and the varible is 16 bites, no need to move bits on order to make these 16 bits segnificant */
    tempSignedTempVal = linePointer[currLineInd].I.immed;
    the16BitSignedTempVAL |= (tempSignedTempVal);

    fprintf(obPointer,"%04ld ",*address);/* printing the address first */

    /* printing the 16 signed bits first */
    for (ind=0;ind<2;ind++) {
        fprintf(obPointer,"%X",(the16BitSignedTempVAL & 0xF0) >> 4); /* high nibble masked with 0xF0 in order to clear the low nibble  */
        fprintf(obPointer,"%X ",the16BitSignedTempVAL & 0x0F); /* low nibble  with 0x0F in order to clear the high nibble */
        /* 1 byte was printed, shifting right 8 bit is needed in order to get to the next byte */
        the16BitSignedTempVAL >>= 8;/* moving to the neext 8 bites by shifting right */
    }

    /* printing the 16 unsigned bits after */
    for (ind=0;ind<2;ind++) {
        fprintf(obPointer,"%X",(the16bitUnsignedVariable & 0xF0) >> 4); /* high nibble masked with 0xF0 in order to clear the low nibble  */
        fprintf(obPointer,"%X ",the16bitUnsignedVariable & 0x0F); /* low nibble  with 0x0F in order to clear the high nibble */
        /* 1 byte was printed, shifting right 8 bit is needed in order to get to the next byte */
        the16bitUnsignedVariable >>= 8;/* moving to the neext 8 bites by shifting right */
    }
    fprintf(obPointer,"\n");
    *address+=4;/* increasing address by 4 bytes */


}


void printAscizToOB(FILE *obPointer,line *linePointer,int currLineInd,long *address,int *numOfPrintedByte){/* this function goes true the asciz type instruction line and print the data it contains */
    int ind;
    if(*numOfPrintedByte==0){
        fprintf(obPointer,"%04ld ",*address);/* printing the address  before the new line starts */ 
    }
    for (ind=1;ind<=linePointer[currLineInd].ascizDataImage.biteSize;ind++){
        *numOfPrintedByte+=1;
        fprintf(obPointer,"%X",(linePointer[currLineInd].ascizDataImage.stringArray[ind-1]>>4) & 0x0F);/* high nibble, masking it with 0x0F */
        fprintf(obPointer,"%X ",linePointer[currLineInd].ascizDataImage.stringArray[ind-1] & 0x0F);/* low nibble, masking it with 0x0F */
        if(*numOfPrintedByte%4==0){
            *address+=4;
            fprintf(obPointer,"\n");/*a new line is needed after printing 4 bites */
            fprintf(obPointer,"%04ld ",*address);/* printing the address  before the new line starts */ 
        }
    }
    /* freeing dynamic memory after it was used */
    free(linePointer[currLineInd].ascizDataImage.stringArray);
}

void printDotD(FILE *obPointer,line *linePointer,int currLineInd,long *address,int *numOfPrintedByte){/* this function print's all .d lines argument in hex */
        int ind,argumentBiteSize;
        if(*numOfPrintedByte==0){
        fprintf(obPointer,"%04ld ",*address);/* printing the address  before the new line starts */ 
        }
        for (ind=1;ind<=linePointer[currLineInd].dataImage.biteSize;ind++){
            *numOfPrintedByte+=1;
            fprintf(obPointer,"%X",(linePointer[currLineInd].dataImage.argumentsArray[ind-1]>>4) & 0x0F);/* high nibble, masking it with 0x0F */
            fprintf(obPointer,"%X ",linePointer[currLineInd].dataImage.argumentsArray[ind-1] & 0x0F);/* low nibble, masking it with 0x0F */
            if(*numOfPrintedByte%4==0){
                *address+=4;/* encreasing address value  by 4 */
                fprintf(obPointer,"\n");/*a new line is needed after printing 4 bites */
                fprintf(obPointer,"%04ld ",*address);/* printing the address  before the new line starts */
            }
        }
        /* freeing dynamic memory after it was used */
        free(linePointer[currLineInd].dataImage.argumentsArray);
}















void createDotExt(line *linePointer,int lineInd,char *inputFileName){/* this function creates the ps.ext outputFile only if there are valid .extern instruction lines in the input file */
    int currLineInd,currExtAddressInd;/* creating index variables for the iterations */
    FILE *extPointer;/* creating a file type pointer for the ext file */
    char extEndingbuffer[DotExtEndingLen];
    if(!(externTypeLinesScanner(linePointer,lineInd)))
        return;
    strcpy(extEndingbuffer,".ext");
    extEndingbuffer[DotExtEndingLen-1] = '\0';
    inputFileName[(strlen(inputFileName))-(DotExtEndingLen-1)] = '\0';
    inputFileName = strcat(inputFileName,extEndingbuffer);
    extPointer = fopen(inputFileName,"w");/* creating the ps.ext file */
    for (currLineInd=0;currLineInd<=lineInd;currLineInd++) {/* looping true the valid lines in order to check if there are valid .extern lines */
        if (linePointer[currLineInd].isExtern==TRUE){
            
            for(currExtAddressInd=0;currExtAddressInd<linePointer[currLineInd].ext.addressInd;currExtAddressInd++){/* looping trough the external addresses array - each external label can hold more than one address it was used in */
                fprintf(extPointer,"%s ",linePointer[currLineInd].ext.extArg);/* printing the external label */
                fprintf(extPointer,"%04d\n",linePointer[currLineInd].ext.extAddress[currExtAddressInd]);/* printing the address of the line that used the external label from the external addresses dynamic array */
            }
            /* freeing dynamic memory after it was used */
            free(linePointer[currLineInd].ext.extArg);
            free(linePointer[currLineInd].ext.extAddress);
        }
    }
    
}

int externTypeLinesScanner(line *linePointer,int lineInd){
    int currLineInd;
    for(currLineInd=0;currLineInd<=lineInd;currLineInd++){
        if (linePointer[currLineInd].isExtern==TRUE)
            return 1;
    }
    return 0;
}

void createDotEnt(label *labelPointer,int labelInd,char *inputFileName){/* this function creates the ps.ent outputFile only if there are valid .entry instruction lines in the input file */
    int currLabelInd;/* creating index variable for the iteration */
    FILE *entPointer;/* creating a file type pointer for the .ent file */
    char entEndingbuffer[DotEntEndingLen];
    if(!(entryTypeLabelScanner(labelPointer,labelInd)))/* calling this function in order to realize if if a .ent file is needed */
        return;
    strcpy(entEndingbuffer,".ent");
   
    entEndingbuffer[DotEntEndingLen-1] = '\0';
    inputFileName[strlen(inputFileName)-(DotEntEndingLen-2)] = '\0';
    inputFileName = strcat(inputFileName,entEndingbuffer);
    entPointer = fopen(inputFileName,"w");
    for (currLabelInd=0;currLabelInd<=labelInd;currLabelInd++){/* scanning the label's table in order to find valid entry type label's */
        if (labelPointer[currLabelInd].isEnt==TRUE){  
            fprintf(entPointer,"%s ",labelPointer[currLabelInd].labelBuff);/* printing the .entry label */
            if (labelPointer[currLabelInd].isCode==TRUE){
                fprintf(entPointer,"%04d\n",labelPointer[currLabelInd].ic);/* printing the address of the .entry label based on her other type */
            }
            else if (labelPointer[currLabelInd].isData==TRUE){
                fprintf(entPointer,"%04d\n",labelPointer[currLabelInd].dc);/* printing the address of the .entry label based on her other type */
            }
        }
    }
}

int entryTypeLabelScanner(label *labelPointer,int labelInd){/* this function help us know if a .ent file is needed */
    int currLabelInd;/* creating index variable for the iteration */
        for (currLabelInd=0;currLabelInd<=labelInd;currLabelInd++){/* scanning the label's table in order to find valid entry type label's that will make us to open a .ent output file */
            if(labelPointer[currLabelInd].isEnt==TRUE){
                return 1;
            }/* end of if */
        }/* end of for */
        return 0;
}/* end of entryTypeLabelScanner*/
















