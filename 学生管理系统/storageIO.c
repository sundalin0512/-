#include "storageIO.h"
#include <stdlib.h>
#include <string.h>

typedef struct File
{
	size_t fileSize;			// 0x00 文件大小
	size_t offsetBegin;			// 0x04 本文件开始位置
	int hasNextFile;			// 0x08 是否有下一个文件
	int hasPreFile;				// 0x0C 是否有上一个文件
	char nextFileName[0x20];	// 0x10 下一个文件相对目录
	char preFileName[0x20];		// 0x30 上一个文件相对目录
	int offsetUesd;				// 0x50 已用节点偏移
	int offsetUnused;			// 0x54 未用节点偏移
	int reserved[2];			// 0x58 保留
	void *data;					// 0x60 数据
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
/// 在指定位置写入指定长度数据
/// </summary>
/// <param name="stream">文件指针</param>
/// <param name="offsetFromFileStart">写入位置距离文件起始的字节数</param>
/// <param name="buffer">要写入的数据</param>
/// <param name="bytesToWrite">要写入的字节数</param>
/// <returns>成功写入的字节数</returns>
static size_t FileWrite(FILE *stream, const size_t offsetFromFileStart, const void *buffer, const size_t bytesToWrite);

/// <summary>
/// 在指定位置读取指定长度
/// </summary>
/// <param name="stream">文件指针</param>
/// <param name="offsetFromFileStart">读取位置距离文件起始的字节数</param>
/// <param name="buffer">保存读取的数据，需要有效指针</param>
/// <param name="bytesToRead">要读取的字节数</param>
/// <returns>成功读取的字节数</returns>
static size_t FileRead(FILE *stream, const size_t offsetFromFileStart, void *buffer, const size_t bytesToRead);

/// <summary>
/// 获取文件信息
/// </summary>
/// <param name="stream">文件指针</param>
/// <returns>文件信息</returns>
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
