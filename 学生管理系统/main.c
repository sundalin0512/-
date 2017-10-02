#include <stdio.h>
#include <string.h>
#include "storageIO.h"
#include <stdlib.h>

int main()
{
	FILE *fp = CreateFile("1.bin", 1024*1024, NULL);
	char *szHello = "hello world";
	InsertData(fp, 0, szHello, strlen(szHello) + 1);
	system("pause");
	szHello = "lalala hahaha";
	InsertData(fp, 0, szHello, strlen(szHello) + 1);
	system("pause");
	DeleteData(fp, 0);
	system("pause");
	szHello = "1234";
	InsertData(fp, 0, szHello, strlen(szHello) + 1);
	system("pause");
	
	DeleteData(fp, 1);
	system("pause");
	DeleteData(fp, 0);
	system("pause");
	FileClose(fp);
	return 0;

}
