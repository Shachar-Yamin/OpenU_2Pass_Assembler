
#ifndef MAMAN14V10_ASSEMBLERHELPER_H
#define MAMAN14V10_ASSEMBLERHELPER_H
#include "dataStructures.h"
#include <stdio.h>
#include "string.h"
#include "stdlib.h"
#include "assemblerHelper.h"
#include "strHandle.h"
#include "firstPass.h"
#include "secondPassHelper.h"
#include "outputPrint.h"
void executePasses(char**,int);
#define tabAscii 9
#define spaceAscii 32
#define maxLineLen 81
#define maxFileEndingLen 4
#define pointAscii 46
int validInputFile(char *);
void errorSpecifier(line*,int,int);
#endif
