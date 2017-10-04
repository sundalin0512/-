#include "userInterface.h"

#include <stdio.h>
#include <stdlib.h>

void ClearScreem()
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
