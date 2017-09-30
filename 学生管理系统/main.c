#include <stdio.h>
#include <string.h>
#include "storageIO.h"

int main()
{
	FILE *fp = CreateFile("1.bin", 1024, NULL);
	char *szHello = "hello world";
	InsertTag(fp, 0, szHello, strlen(szHello));
	FileClose(fp);
	return 0;

}
