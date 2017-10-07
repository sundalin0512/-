#pragma once

#include "types.h"

void ClearScreen();

void PrintLine(char *buffer);

char GetInputChar();

void GetInputString(char buffer[MAX_STR_LEN]);

int GetInputInt();

void PrintStudent(Student student, int index);

void ShowGrade(double totalGrade, double averageGrade, double maxGrade, double minGrade);

void ShowStudentFileStatus(int *sizeList, int *statusList, int listSize);

void PrintStudentHelpInfo();