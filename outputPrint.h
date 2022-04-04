#ifndef MAMAN14V10_OUTPUTPRINT_H
#define MAMAN14V10_OUTPUTPRINT_H
#include "strHandle.h"
#include "stdio.h"
#define maxInputFileEndingLen 4
#define DotObEndingLen 4
#define DotExtEndingLen 5
#define DotEntEndingLen 5
#define maxLineLen 81

void createDotObFile(line *,int ,label *,int ,int ,int,int*,char*);
void printRToOb(FILE*,line *,int ,long *);
void  printJToOb(FILE *,line *,int,long *);
void printIToOb(FILE*,line*,int,long*);
void printAscizToOB(FILE *,line *,int,long*,int*);
void printDotD(FILE *,line*,int ,long *,int*);
void createDotExt(line *,int,char*);
void createDotEnt(label *,int,char*);
void createOutput(line *,int,label *,int,int,int,int*,char*,int);
int entryTypeLabelScanner(label *,int);
int externTypeLinesScanner(line *,int);



#endif
