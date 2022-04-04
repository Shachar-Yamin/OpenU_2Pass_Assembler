#include "secondPassHelper.h"

int errorsScanner(line *linePointer,int currlLineInd,int ignoredLines){/* this int  function search for errors across the current file, if there are, it return's 1 if not it return's 0.*/
    int ind,errorFlag;
    errorFlag=0;
    for (ind = 0; ind <=currlLineInd ; ind++){/* looping trough all cells that was created for lines in the lines array */
        if (linePointer[ind]._isError==TRUE){
            return 1;
        }
    }
    return 0;

    }/* end of errorScanner */


int is_definedLabel(label *labelPointer,int curLabelInd,char *labelArgBuffer) {/* an int  function that checks if a label argument is defined as a valid label if it is, it returns 1, else it returns 0 */
    if ((strcmp(labelPointer[curLabelInd].labelBuff, labelArgBuffer) == 0) && labelPointer[curLabelInd].isExt!=TRUE) {
        return 1;
    }
    else
        return 0;
}/* end of is_definedLabel */


void updateLabelDC(label *labelPointer,int labelInd,int *ICF){/* this function updates all .data labels address value by adding the amount of bites the commands holds (ICF) */
    int ind;
    for (ind=0;ind<=labelInd;ind++) {
        if (labelPointer[ind].isData==TRUE){
            labelPointer[ind].dc+=*ICF;
        }
    }
}/* end of updateLabelDC */


void handleEntry(line *linePointer,int curLineInd,label *labelPointer,int labelInd,int ignoreLinesNum) {/* this function raise an entry flag if the .entry argument equal to a valid label */
    int currLabelInd, definedFlag;
    definedFlag = 0;
    for (currLabelInd = 0; currLabelInd <= labelInd; currLabelInd++) {/* scanning the label's dynamic array */
        if (is_definedLabel(labelPointer, currLabelInd, linePointer[curLineInd].entryArgument)) {/* this function check if the .enrty argument is a valid label */
            labelPointer[currLabelInd].isEnt = TRUE;/* if it is, raise the entry flag in order it will be identified in the printing time */
            definedFlag = 1;
            free(linePointer[curLineInd].entryArgument);/* free the entry arg pointer */
            
        }
    }
    if (definedFlag ==0) {/* if there wasn't any defined label that equals the one that was given as an argument raise a relevant error flag */
        raiseLabelArgError(linePointer, curLineInd);
        errorSpecifier(linePointer,curLineInd,ignoreLinesNum+curLineInd+1);/* raise and print an error meesege */
        free(linePointer[curLineInd].entryArgument);/* free the entry arg pointer */
    }
}/* end of handle entry */



void scanForEntrys(line *linePointer,int lineInd,label *labelPointer,int labelInd,int ignoredLinesNum){/* this function search for .entry lines and takes care of them as part of the  second pass */
    int currLineInd;
    for (currLineInd=0;currLineInd<=lineInd;currLineInd++){
        if(linePointer[currLineInd].isEntry==TRUE){
            handleEntry(linePointer,currLineInd,labelPointer,labelInd,ignoredLinesNum);/* calling handle entry in order to check the entry argument */
        }
    }
}/* end of scan for entry's */

/* function that are made to handle the I and J type line's that has a missed Label argument from the first pass */

void codeJTypeMissedLabel(line *linePointer,int currLineInd,label *labelPointer,int lineInd,int labelInd,int ignoredLinesNum){/* this void function check if the label arg that 
wasnt found in the first pass is a valid argument and code it if it is, raise an error if not*/

    int currLabelInd,missedFlag;/* defining variable's that wil track the looping trough the label array and help us tell if an identical label was found */
    missedFlag =0;/* creating a flag that will help us notice if the label argument  have been defined already and raise an error in case its needed */
    for (currLabelInd=0;currLabelInd<=labelInd;currLabelInd++) {/*looping trough the label array to first find out if the missed label arg is a data or a code type valid label */
        if (!(strcmp(linePointer[currLineInd].J.missedJLabel, labelPointer[currLabelInd].labelBuff))) {
            if (labelPointer[currLabelInd].isCode!= FALSE) {
                linePointer[currLineInd].J.address = labelPointer[currLabelInd].ic;
                missedFlag = 1;
            }
            else if (labelPointer[currLabelInd].isData != FALSE) {
                missedFlag = 1;
                linePointer[currLineInd].J.address = labelPointer[currLabelInd].dc;
                
            }
        }
    }
        if (missedFlag==0) {/* if the missed label wasn't found at the label array search in the extern array */
            codeJTypeExternMissed(linePointer,currLineInd,lineInd,&missedFlag); /* calling a function that checks if a label is an out source extern argument */

            
        }

    if (missedFlag==0){/* if the label that was given as an argument is not defined */
        raiseLabelArgError(linePointer,currLineInd);/* it means the un-founded label arg from the first pass doesnt exist and this situation requires a rising of an error flag */
        errorSpecifier(linePointer,currLineInd,currLineInd+ignoredLinesNum+1);/* calling the error Specifier in order to get the specific input that is needed for the problem */
    }
    free(linePointer[currLineInd].J.missedJLabel);
}/* end of  J type label */


void codeJTypeExternMissed(line *linePointer,int currLineInd,int lineInd,int *missedFlag){/* a function that checks if the missed label is an outsource extern argument */
    int lineLoopInd; /* a variable that will help track the looping trough the line index and search for an extern label args that will match the missed label */
    for (lineLoopInd = 0; lineLoopInd <= lineInd; lineLoopInd++){/* search true the saved valid extern label args that are saved inside the line array */
        if (linePointer[lineLoopInd].isExtern != FALSE) {
            if (strcmp(linePointer[lineLoopInd].ext.extArg, linePointer[currLineInd].J.missedJLabel) == 0) {
                if(linePointer[lineLoopInd].ext.addressInd==0){/* it means the current external label is in use for the first time */
                    linePointer[lineLoopInd].ext.extAddress = calloc(1,sizeof(int));
                    linePointer[lineLoopInd].ext.extAddress[0] = linePointer[currLineInd].IC;
                    linePointer[currLineInd].J.address = 0;
                    linePointer[lineLoopInd].ext.addressInd++;
                    *missedFlag =1;/*updating the missed flag value with a pointer in order to chenge the value of the the varible that his value was sent*/
                    
                }
                else{/* it means there is already one line that uses the outer label arg and the address dynamic array is have been defined */
                    linePointer[lineLoopInd].ext.extAddress = realloc(linePointer[lineLoopInd].ext.extAddress,++linePointer[lineLoopInd].ext.addressInd);
                    if (!(linePointer[lineLoopInd].ext.extAddress)){
                        raiseAllocationError(linePointer,lineInd);/* raise an error flag in case of allocation error in order to prevent segmentation fault */
                        free(linePointer[lineLoopInd].ext.extArg);
                        free(linePointer[lineLoopInd].ext.extAddress);

                    }
                    else {

                        linePointer[lineLoopInd].ext.extAddress[linePointer[lineLoopInd].ext.addressInd -1] = linePointer[currLineInd].IC;
                        linePointer[currLineInd].J.address = 0;
                        *missedFlag =1;/*updating the missed flag value with a pointer in order to chenge the value of the the varible that his value was sent*/
                    }
                }/* end of else */
            }/* and of strcmp if */
        }/* end of if */
    }/*end of for loop */
}/* end of codeJTypeExternMissed */







void codeITypeMissedLabel(line *linePointer,int currLineInd,label *labelPointer,int lineInd,int labelInd,int ignoredLinesNum ){/* a function that was made to handle the I type lines that has a missedLabel flag turned on from the first pass */
    int lineLoopInd,labelLoopInd;
    int definedFlag;/* a variable that will help us track if the missed Label arg is defined and raise an error flag in case it didnt */
    definedFlag=0;
    for(lineLoopInd=0;lineLoopInd<=lineInd;lineLoopInd++){/* a for loop that searches if the  I missed label is an external label and raise a valid error flag if it is */
        if(linePointer[lineLoopInd].isExtern!=FALSE){
            if(strcmp(linePointer[currLineInd].I.missedILabel,linePointer[lineLoopInd].ext.extArg)==0){
                raiseLabelArgError(linePointer,currLineInd);
                return;
            }/* end of if statement */
        }/* end of if */
    }/* end of for loop */
    for (labelLoopInd=0;labelLoopInd<=labelInd;labelLoopInd++){/* this for loop scan the label's array */

        if (!(strcmp(linePointer[currLineInd].I.missedILabel,labelPointer[labelLoopInd].labelBuff))){
            
        
            if (labelPointer[labelLoopInd].isData!=FALSE){/* if the equal label is a data type label */
                definedFlag = 1;
                linePointer[currLineInd].I.immed = (labelPointer[labelLoopInd].dc - linePointer[currLineInd].IC);/* code the label arg insinde the line immed field */
            }/* end of if data */

            if (labelPointer[labelLoopInd].isCode!=FALSE){/* if the equal label is a data type label */
                definedFlag = 1;
                linePointer[currLineInd].I.immed = (labelPointer[labelLoopInd].ic - linePointer[currLineInd].IC);/* code the label arg insinde the line immed field */
            }/* end of if code */
        }
        /* end of if strcmp */
    }/* end of for */
    if(definedFlag==0){/* if an equal label wasnt found */
        
        raiseLabelArgError(linePointer,currLineInd);
        errorSpecifier(linePointer,currLineInd,currLineInd+ignoredLinesNum+1);/* raise an error flag and print an error messege */
    }
    free(linePointer[currLineInd].I.missedILabel);
}/* end of code I type missed label */


void searchMissedLabel(line *linePointer,int lineInd,label *labelPointer,int labelInd,int ignoredLinesNum){
    int currLineInd;/* a variable that will help track the current line we want to send to the functions */

    for (currLineInd = 0; currLineInd<=lineInd;currLineInd++){
        if (linePointer[currLineInd].j_TypeMissedLabel==TRUE){
            codeJTypeMissedLabel(linePointer,currLineInd,labelPointer,lineInd,labelInd,ignoredLinesNum);/* call the function that will help handle all j type lines  that has a missed arg label */
        }/* end of if jMissed */
        else if (linePointer[currLineInd].i_TypeMissedLabel==TRUE){
            codeITypeMissedLabel(linePointer,currLineInd,labelPointer,lineInd,labelInd,ignoredLinesNum);/* call the function that will help handle all j type lines  that has a missed arg label */
        }/* end if if iMissed */
    }/* end of for */
}/* end of searchMissedLabel */












void secondPass(line *linePointer,int lineInd,label *labelPointer,int labelInd,int ignoredLinesNum){/* this function will contain all second pass function's and execute them */
    scanForEntrys(linePointer,lineInd,labelPointer,labelInd,ignoredLinesNum);/* handle the .entry instruction's that were ignored in the first pass */
    searchMissedLabel(linePointer,lineInd,labelPointer,labelInd,ignoredLinesNum);/* search for the missed labels who were given as arguments in the type I and J command's that haven't been checked im the first pass */














}
