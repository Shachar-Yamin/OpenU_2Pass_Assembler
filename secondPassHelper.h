
#ifndef MAMAN14V10_SECONDPASSHELPER_H
#define MAMAN14V10_SECONDPASSHELPER_H

#include "assemblerHelper.h"
#include "strHandle.h"
int errorsScanner(line *,int,int);
void updateLabelDC(label *,int ,int *);
int is_definedLabel(label *,int ,char *);
void handleEntry(line *,int ,label *,int,int);
void scanForEntrys(line *,int ,label *,int,int );
void codeJTypeMissedLabel(line *,int c,label *,int,int,int);
void codeJTypeExternMissed(line *,int,int,int*);
void codeITypeMissedLabel(line*,int,label*,int,int,int);
void searchMissedLabel(line *,int,label*,int,int);
void secondPass(line*,int,label*,int,int);
int twoEqualWords(char*,char*);


#endif
