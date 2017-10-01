#include <stdio.h>
#include <string.h>
#include "storageIO.h"

int main()
{
	FILE *fp = CreateFile("1.bin", 1024, NULL);
	char *szHello = "hello world";
	InsertData(fp, 0, szHello, strlen(szHello) + 1);
	szHello = "lalala hahaha";
	InsertData(fp, 0, szHello, strlen(szHello) + 1);
	DeleteData(fp, 1);
	InsertData(fp, 0, szHello, strlen(szHello) + 1);
	DeleteData(fp, 0);
	DeleteData(fp, 0);
	FileClose(fp);
	return 0;

}
