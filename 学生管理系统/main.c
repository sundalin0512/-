#include <stdio.h>
#include <string.h>
#include "storageIO.h"
#include <stdlib.h>

int main()
{
	FILE *fp = CreateFile("1.bin", 1024*1024, NULL);
	char *szHello = "00000000000000000";
	int index = 0;


	AppendData(fp, szHello, strlen(szHello) + 1, &index);
	szHello = "11111111111111111";
	AppendData(fp, szHello, strlen(szHello) + 1, &index);
	szHello = "2222222222222222222222222222";
	AppendData(fp, szHello, strlen(szHello) + 1, &index);

	
	szHello = "33333333333333333333333";
	AppendData(fp, szHello, strlen(szHello) + 1, &index);
	szHello = "444444444444";
	AppendData(fp, szHello, strlen(szHello) + 1, &index);
	szHello = "555555555555555555555555555555555"; 
	AppendData(fp, szHello, strlen(szHello) + 1, &index);
	szHello = "66666666666666666666";
	AppendData(fp, szHello, strlen(szHello) + 1, &index);
	szHello = "7";
	AppendData(fp, szHello, strlen(szHello) + 1, &index);
	szHello = "8888888888888888888888888888888888888888888";
	AppendData(fp, szHello, strlen(szHello) + 1, &index);

	szHello = "999999999999999";
	AppendData(fp, szHello, strlen(szHello) + 1, &index);
	
	DeleteData(fp, 9);
	
	DeleteData(fp, 7);
	DeleteData(fp, 5);
	DeleteData(fp, 3);
	DeleteData(fp, 3);
	DeleteData(fp, 0);
	DeleteData(fp, 0);
	DeleteData(fp, 0);
	DeleteData(fp, 0);

	Defragment(fp);
	
	FileClose(fp);
	return 0;

}
