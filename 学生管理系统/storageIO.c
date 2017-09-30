#include "storageIO.h"
#include <stdlib.h>
#include <string.h>

typedef struct File
{
	size_t fileSize;			// 0x00 �ļ���С
	size_t offsetBegin;			// 0x04 ���ļ���ʼλ��
	int hasNextFile;			// 0x08 �Ƿ�����һ���ļ�
	int hasPreFile;				// 0x0C �Ƿ�����һ���ļ�
	char nextFileName[0x20];	// 0x10 ��һ���ļ����Ŀ¼
	char preFileName[0x20];		// 0x30 ��һ���ļ����Ŀ¼
	int offsetUesd;				// 0x50 ���ýڵ�ƫ��
	int offsetUnused;			// 0x54 δ�ýڵ�ƫ��
	int reserved[2];			// 0x58 ����
	void *data;					// 0x60 ����
}File;

typedef struct TagList
{
	size_t size;
	struct
	{
		int offset;
		size_t size;
	}list[1];
}UsedList, UnusedList;

/// <summary>
/// ��ָ��λ��д��ָ����������
/// </summary>
/// <param name="stream">�ļ�ָ��</param>
/// <param name="offsetFromFileStart">д��λ�þ����ļ���ʼ���ֽ���</param>
/// <param name="buffer">Ҫд�������</param>
/// <param name="bytesToWrite">Ҫд����ֽ���</param>
/// <returns>�ɹ�д����ֽ���</returns>
static size_t FileWrite(FILE *stream, const size_t offsetFromFileStart, const void *buffer, const size_t bytesToWrite);

/// <summary>
/// ��ָ��λ�ö�ȡָ������
/// </summary>
/// <param name="stream">�ļ�ָ��</param>
/// <param name="offsetFromFileStart">��ȡλ�þ����ļ���ʼ���ֽ���</param>
/// <param name="buffer">�����ȡ�����ݣ���Ҫ��Чָ��</param>
/// <param name="bytesToRead">Ҫ��ȡ���ֽ���</param>
/// <returns>�ɹ���ȡ���ֽ���</returns>
static size_t FileRead(FILE *stream, const size_t offsetFromFileStart, void *buffer, const size_t bytesToRead);

/// <summary>
/// ��ȡ�ļ���Ϣ
/// </summary>
/// <param name="stream">�ļ�ָ��</param>
/// <returns>�ļ���Ϣ</returns>
static File *GetFileInfo(FILE *stream);

FILE* CreateFile(const char* fileName, const size_t sizeBytes, const char *preFileName)
{
	if (sizeBytes > 0x60)
	{
		FILE *fp = NULL;
		File file = {
			sizeBytes - 0x60,
			0x60,
			0,
			0,
			"",
			"",
			0x60,
			0x64,
			0,0,0
		};
		fopen_s(&fp, fileName, "wb+");
		if (fp == NULL)
		{
			return NULL;
		}
		if (preFileName != NULL)
		{
			File preFile = { 0 };
			FILE *fpPreFile = NULL;
			file.hasPreFile = 1;
			strcpy_s(file.preFileName, 0x20 - 1, preFileName);
			fpPreFile = FileOpen(preFileName);
			FileRead(fpPreFile, 0, &preFile, sizeof(preFile));
			preFile.hasNextFile = 1;
			strcpy_s(preFile.nextFileName, 0x20 - 1, fileName);
			FileWrite(fpPreFile, 0, &preFile, sizeof(preFile));
			FileClose(fpPreFile);
		}
		file.data = malloc(sizeBytes - 0x60);
		((int*)file.data)[0] = 0;
		((int*)file.data)[1] = 0;
		FileWrite(fp, 0, &file, 0x60);
		FileWrite(fp, 0x60, file.data, sizeBytes - 0x60);
		free(file.data);
		return fp;
	}
	else
	{
		return NULL;
	}
}

FILE *FileOpen(const char *fileName)
{
	FILE *fp = NULL;
	fopen_s(&fp, fileName, "rb+");
	return fp;
}

void FileClose(FILE* stream)
{
	if (stream != NULL)
	{
		fclose(stream);
	}
}

size_t GetFileSize(FILE* stream)
{
	size_t size = 0;
	FileRead(stream, 0, &size, sizeof(int));
	return size;
}

int InsertTag(FILE* stream, int index, void* buffer, size_t bytesToInsert)
{
	//File *file = GetFileInfo(stream);
	//UnusedList unusedList = { 0 };
	//FileRead(stream, file->offsetUnused, &unusedList, sizeof(int));
	//return 0;
}

int AppendTag(FILE* stream, void* buffer, size_t bytesToInsert)
{
}

int DeleteTag(FILE* stream, int index)
{
}

static size_t FileWrite(FILE *stream, const size_t offsetFromFileStart, const void *buffer, const size_t bytesToWrite)
{
	size_t bytesHasWrite = 0;
	fseek(stream, offsetFromFileStart, SEEK_SET);
	bytesHasWrite = fwrite(buffer, 1, bytesToWrite, stream);
	fflush(stream);
	return bytesHasWrite;
}

static size_t FileRead(FILE *stream, const size_t offsetFromFileStart, void *buffer, const size_t bytesToRead)
{
	size_t bytesHasRead = 0;
	fseek(stream, offsetFromFileStart, SEEK_SET);
	bytesHasRead = fread(buffer, 1, bytesToRead, stream);
	fflush(stream);
	return bytesHasRead;
}

static File *GetFileInfo(FILE *stream)
{
	File *file = (File*)malloc(sizeof(File));
	FileRead(stream, 0, file, sizeof(File));
	return file;
}
