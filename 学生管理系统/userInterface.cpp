#include "userInterface.h"

#include <stdio.h>
#include <stdlib.h>

void ClearScreen()
{
	system("cls");
}

void PrintLine(char *buffer)
{
	printf(buffer);
	printf("\r\n");
}


char GetInputChar()
{
	char ch = '\0';
	setvbuf(stdin, NULL, _IONBF, 0);
	scanf("%c", &ch);
	return ch;
}

void GetInputString(char buffer[MAX_STR_LEN])
{
	setvbuf(stdin, NULL, _IONBF, 0);
	scanf_s("%s", buffer, MAX_STR_LEN);
}

int GetInputInt()
{
	int val = 0;
	setvbuf(stdin, NULL, _IONBF, 0);
	scanf("%d", &val);
	return val;
}

void PrintStudent(Student student, int index)
{
	printf("%d:\t%s\t%s\t%s\t%s\t%d %d %d\r\n", index, student.ID, student.name, student.sexual, student.grade_C, student.Birthday.year,
		   student.Birthday.month, student.Birthday.day);
}

void ShowGrade(double totalGrade, double averageGrade, double maxGrade, double minGrade)
{
	printf("最高分：\t%lf\r\n", maxGrade);
	printf("最低分：\t%lf\r\n", minGrade);
	printf("总成绩：\t%lf\r\n", totalGrade);
	printf("平均分：\t%lf\r\n", averageGrade);
}

void ShowStudentFileStatus(int * sizeList, int * statusList, int listSize)
{
	ClearScreen();
	for (int i = 0; i < listSize; i++)
	{
		for (int j = 0; j < sizeList[i]; j++)
		{
			printf("%c", statusList[i] == 1 ? 'U' : 'F');
		}
	}
}

void PrintStudentHelpInfo()
{
	printf("   \t学号\t姓名\t性别\tC成绩\t出生日期\r\n");
}