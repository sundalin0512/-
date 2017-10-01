#include "storageIO.h"
#include <stdlib.h>
#include <string.h>

typedef struct FileInfo
{
	size_t fileSize;			// 0x00 文件大小
	size_t offsetBegin;			// 0x04 本文件开始位置
	int hasNextFile;			// 0x08 是否有下一个文件
	int hasPreFile;				// 0x0C 是否有上一个文件
	char nextFileName[0x20];	// 0x10 下一个文件相对目录
	char preFileName[0x20];		// 0x30 上一个文件相对目录
	size_t offsetUsed;				// 0x50 已用节点偏移，在未用节点前
	size_t offsetUnused;			// 0x54 未用节点偏移，在已用节点后
	int reserved[2];			// 0x58 保留
	//void *data;				// 0x60 数据
}FileInfo;

typedef struct TagList
{
	size_t size;	//list的长度
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
static size_t FileWrite(FILE *stream, const size_t offsetFromFileStart, const void *buffer, const size_t bytesToWrite)
{
	size_t bytesHasWrite = 0;
	fseek(stream, offsetFromFileStart, SEEK_SET);
	bytesHasWrite = fwrite(buffer, 1, bytesToWrite, stream);
	fflush(stream);
	return bytesHasWrite;
}

/// <summary>
/// 在指定位置读取指定长度
/// </summary>
/// <param name="stream">文件指针</param>
/// <param name="offsetFromFileStart">读取位置距离文件起始的字节数</param>
/// <param name="buffer">保存读取的数据，需要有效指针</param>
/// <param name="bytesToRead">要读取的字节数</param>
/// <returns>成功读取的字节数</returns>
static size_t FileRead(FILE *stream, const size_t offsetFromFileStart, void *buffer, const size_t bytesToRead)
{
	size_t bytesHasRead = 0;
	fseek(stream, offsetFromFileStart, SEEK_SET);
	bytesHasRead = fread(buffer, 1, bytesToRead, stream);
	fflush(stream);
	return bytesHasRead;
}

/// <summary>
/// 获取文件信息
/// </summary>
/// <param name="stream">文件指针</param>
/// <returns>文件信息</returns>
static FileInfo *GetFileInfo(FILE *stream)
{
	FileInfo *file = (FileInfo*)calloc(sizeof(FileInfo), 1);
	FileRead(stream, 0, file, sizeof(FileInfo));
	return file;
}

/// <summary>
/// 创建文件，返回文件指针
/// </summary>
/// <param name="fileName">要创建的文件名</param>
/// <param name="sizeBytes">要创建的文件字节数</param>
/// <param name="preFileName">前一个文件的文件名</param>
/// <returns>创建的文件指针，失败返回空指针</returns>
FILE *CreateFile(const char *fileName, const size_t sizeBytes, const char *preFileName)
{
	if (sizeBytes > 0x68)
	{
		FILE *fp = NULL;
		FileInfo file = {
			sizeBytes,
			0x60,
			0,
			0,
			"",
			"",
			0x60,
			0x64,
			0,0
		};
		fopen_s(&fp, fileName, "wb+");
		if (fp == NULL)
		{
			return NULL;
		}
		if (preFileName != NULL)
		{
			FileInfo filePre = { 0 };
			FILE *fpPreFile = NULL;
			file.hasPreFile = 1;
			strcpy_s(file.preFileName, 0x20 - 1, preFileName);
			fpPreFile = FileOpen(preFileName);
			FileRead(fpPreFile, 0, &filePre, sizeof(filePre));
			filePre.hasNextFile = 1;
			strcpy_s(filePre.nextFileName, 0x20 - 1, fileName);
			FileWrite(fpPreFile, 0, &filePre, sizeof(filePre));
			FileClose(fpPreFile);
		}
		void *data = calloc(sizeBytes - 0x60, 1);
		if (data == NULL)
		{
			FileClose(fp);
			return NULL;
		}
		((int*)data)[0] = 0;
		((int*)data)[1] = 0;
		FileWrite(fp, 0, &file, 0x60);
		FileWrite(fp, 0x60, data, sizeBytes - 0x60);
		free(data);
		return fp;
	}
	else
	{
		return NULL;
	}
}

/// <summary>
/// 打开文件，返回文件指针
/// </summary>
/// <param name="fileName">要打开的文件名</param>
/// <returns>打开的文件指针，失败返回空指针</returns>
FILE *FileOpen(const char *fileName)
{
	FILE *fp = NULL;
	fopen_s(&fp, fileName, "rb+");
	return fp;
}

/// <summary>
/// 关闭文件
/// </summary>
/// <param name="stream">要关闭的文件指针</param>
void FileClose(FILE *stream)
{
	if (stream != NULL)
	{
		fclose(stream);
	}
}

/// <summary>
/// 获取文件长度
/// </summary>
/// <param name="stream">文件指针</param>
/// <returns>文件的字节数</returns>
size_t GetFileSize(FILE *stream)
{
	size_t size = 0;
	FileRead(stream, 0, &size, sizeof(int));
	return size;
}

/// <summary>
/// 寻找拥有足够空间的未用节点
/// </summary>
/// <param name="size">需要的字节数</param>
/// <param name="tagList">要查找的表<typeparam name="UnusedList"/></param>
/// <returns>空节点的下标（不小于0），-1表示没有找到</returns>
int FindEmptyPlace(const size_t size, UnusedList *tagList)
{
	int i = 0;
	for (i = 0; i < tagList->size; i++)
	{
		if (size < tagList->list[i].size)
		{
			return i;
		}
	}
	return -1;
}

/// <summary>
/// 在指定索引处添加数据
/// </summary>
/// <param name="stream">文件指针</param>
/// <param name="index">索引</param>
/// <param name="buffer">要写入的数据</param>
/// <param name="bytesToInsert">要写入的字节数</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType InsertData(FILE *stream, const int index, const void* buffer, const size_t bytesToInsert)
{
	FileInfo *fileinfo = GetFileInfo(stream);
	size_t size = 0;
	UnusedList *unusedList = NULL;
	UsedList *usedList = NULL;
	int unusedPlaceIndex = 0;

#pragma region refactor
	//读取未用节点
	FileRead(stream, fileinfo->offsetUnused, &size, sizeof(size_t));
	size = size*(sizeof(int) + sizeof(size_t)) + sizeof(size_t);
	unusedList = (UnusedList*)calloc(size, 1);
	if (unusedList == NULL)
	{
		return RET_NO_HEAP_SPACE;
	}
	FileRead(stream, fileinfo->offsetUnused, unusedList, size);

	//读取已用节点
	FileRead(stream, fileinfo->offsetUsed, &size, sizeof(size_t));
	size = size*(sizeof(int) + sizeof(size_t)) + sizeof(size_t);
	usedList = (UsedList*)calloc(size, 1);
	if (usedList == NULL)
	{
		return RET_NO_HEAP_SPACE;
	}
	FileRead(stream, fileinfo->offsetUsed, usedList, size);
#pragma endregion

	size = bytesToInsert;
	if (index > usedList->size)
	{
		return RET_ILLEGAL_INDEX;
	}
	unusedPlaceIndex = FindEmptyPlace(bytesToInsert, unusedList);
	if (unusedPlaceIndex == -1)
	{
		// 写入到文件尾
		size_t writePosition = fileinfo->offsetUsed;
		size_t usedListPosition = fileinfo->offsetUsed;
		size_t fileLength = GetFileSize(stream);
		size_t unusedListSize = unusedList->size*(sizeof(int) + sizeof(size_t)) + sizeof(size_t);
		size_t usedListSize = usedList->size*(sizeof(int) + sizeof(size_t)) + sizeof(size_t);
		size_t additionalSize = sizeof(int) + sizeof(size_t);

		if (fileLength < writePosition + bytesToInsert + unusedListSize + usedListSize + additionalSize)
		{
			return RET_NOSPACE;
		}
		// 写数据
		writePosition += FileWrite(stream, writePosition, buffer, bytesToInsert);
		// 写已用节点
		usedList->size += 1;
		fileinfo->offsetUsed = writePosition;
		writePosition += FileWrite(stream, writePosition, usedList, usedListSize);
		writePosition += FileWrite(stream, writePosition, &usedListPosition, sizeof(size_t));
		writePosition += FileWrite(stream, writePosition, &bytesToInsert, sizeof(size_t));
		// 写未用节点
		fileinfo->offsetUnused = writePosition;
		FileWrite(stream, writePosition, unusedList, unusedListSize);
		// 修改文件头
		FileWrite(stream, 0, fileinfo, sizeof(FileInfo));
		return RET_SUCCESS;
	}
	{
		// 写入到未使用空间
		size_t writePosition = unusedList->list[unusedPlaceIndex].offset;
		size_t writeLength = bytesToInsert;
		size_t freeSpaceLength = unusedList->list[unusedPlaceIndex].size;
		size_t fileLength = GetFileSize(stream);
		size_t unusedListSize = unusedList->size*(sizeof(int) + sizeof(size_t)) + sizeof(size_t);
		size_t usedListSize = usedList->size*(sizeof(int) + sizeof(size_t)) + sizeof(size_t);
		size_t additionalSize = sizeof(int) + sizeof(size_t);

		if (writeLength == freeSpaceLength)
		{
			additionalSize = 0;
		}
		if (fileLength < writePosition + bytesToInsert + unusedListSize + usedListSize + additionalSize)
		{
			return RET_NOSPACE;
		}
		// 写数据
		FileWrite(stream, writePosition, buffer, writeLength);
		// 写已用节点
		writePosition = fileinfo->offsetUsed;
		usedList->size += 1;
		fileinfo->offsetUsed = writePosition;
		writePosition += FileWrite(stream, writePosition, usedList, usedListSize);
		writePosition += FileWrite(stream, writePosition, &fileinfo->offsetUsed, sizeof(size_t));
		writePosition += FileWrite(stream, writePosition, &bytesToInsert, sizeof(size_t));
		// 写未用节点
		fileinfo->offsetUnused = writePosition;
		if (writeLength == freeSpaceLength)
		{
			size_t remaindSize = 0;
			char *nextWriteBuffer = (char*)unusedList;
			writeLength = unusedPlaceIndex*(sizeof(int) + sizeof(size_t)) + sizeof(size_t);
			writePosition += FileWrite(stream, writePosition, unusedList, writeLength);
			writeLength = unusedListSize - writeLength - (sizeof(int) + sizeof(size_t));
			nextWriteBuffer += writeLength + sizeof(int) + sizeof(size_t);
			FileWrite(stream, writePosition, nextWriteBuffer, writeLength);
		}
		else
		{
			unusedList->list[unusedPlaceIndex].size -= writeLength;
			FileWrite(stream, writePosition, unusedList, unusedListSize);
		}
		// 修改文件头
		FileWrite(stream, 0, fileinfo, sizeof(FileInfo));
		return RET_SUCCESS;
	}
}

/// <summary>
/// 在索引尾添加数据
/// </summary>
/// <param name="stream">文件指针</param>
/// <param name="buffer">要写入的数据</param>
/// <param name="bytesToInsert">要写入的字节数</param>
/// <param name="index">返回插入的索引</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType AppendData(FILE *stream, const void * buffer, const size_t bytesToInsert, int * index)
{
	FileInfo *file = GetFileInfo(stream);
	size_t size = 0;
	FileRead(stream, file->offsetUnused, &size, sizeof(size_t));
	*index = size;
	return InsertData(stream, size, buffer, bytesToInsert);
}

/// <summary>
/// 删除指定索引的数据
/// </summary>
/// <param name="stream">文件指针</param>
/// <param name="index">要删除的索引</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType DeleteData(FILE *stream, int index)
{
	FileInfo *fileinfo = GetFileInfo(stream);
	size_t size = 0;
	UnusedList *unusedList = NULL;
	UsedList *usedList = NULL;
	size_t writePosition = 0;
	size_t unusedListSize = 0;
	size_t usedListSize = 0;
	size_t unusedBlockOffset = 0;
	size_t unusedBlockSize = 0;
	char *writeBuffer = NULL;
	int offset = 0;
	int unusedListInsertIndex = 0;
	size_t writeLength = 0;
#pragma region refactor2
	//读取未用节点
	FileRead(stream, fileinfo->offsetUnused, &size, sizeof(size_t));
	size = size*(sizeof(int) + sizeof(size_t)) + sizeof(size_t);
	unusedList = (UnusedList*)calloc(size, 1);
	if (unusedList == NULL)
	{
		return RET_NO_HEAP_SPACE;
	}
	FileRead(stream, fileinfo->offsetUnused, unusedList, size);

	//读取已用节点
	FileRead(stream, fileinfo->offsetUsed, &size, sizeof(size_t));
	size = size*(sizeof(int) + sizeof(size_t)) + sizeof(size_t);
	usedList = (UsedList*)calloc(size, 1);
	if (usedList == NULL)
	{
		return RET_NO_HEAP_SPACE;
	}
	FileRead(stream, fileinfo->offsetUsed, usedList, size);
#pragma endregion
	if (index > usedList->size - 1)
	{
		return RET_ILLEGAL_INDEX;
	}

	// 保存要写入的未用节点的数据
	unusedBlockOffset = usedList->list[index].offset;
	unusedBlockSize = usedList->list[index].size;
	// 写已用节点
	usedList->size -= 1;
	offset = index *(sizeof(int) + sizeof(size_t)) + sizeof(size_t);
	writePosition = fileinfo->offsetUsed;
	FileWrite(stream, writePosition, usedList, sizeof(size_t));
	writePosition = fileinfo->offsetUsed + offset;
	writeBuffer = (char*)usedList;
	writeBuffer += offset + sizeof(int) + sizeof(size_t);
	writeLength = usedList->size *(sizeof(int) + sizeof(size_t)) + sizeof(size_t) - offset;
	
	writePosition += FileWrite(stream, writePosition, writeBuffer, writeLength);
	// 写未用节点
	for (unusedListInsertIndex = 0; unusedListInsertIndex < unusedList->size; unusedListInsertIndex++)
	{
		if (unusedList->list[unusedListInsertIndex].offset > unusedBlockOffset)
		{
			break;
		}
	}
	if (unusedListInsertIndex > unusedList->size && unusedList->size != 0)
	{
		return RET_ERROR;
	}
	fileinfo->offsetUnused = writePosition;
	unusedListSize = unusedList->size;
	unusedList->size += 1;
	writeLength = unusedListInsertIndex*(sizeof(int) + sizeof(size_t)) + sizeof(size_t);
	writeBuffer = (char*)unusedList;
	writePosition += FileWrite(stream, writePosition, writeBuffer, writeLength);
	writeBuffer += writeLength;
	writePosition += FileWrite(stream, writePosition, &unusedBlockOffset, sizeof(size_t));
	writePosition += FileWrite(stream, writePosition, &unusedBlockSize, sizeof(size_t));
	writeLength = (unusedListSize - unusedListInsertIndex)*(sizeof(int) + sizeof(size_t));
	FileWrite(stream, writePosition, writeBuffer, writeLength);
	// 修改文件头
	FileWrite(stream, 0, fileinfo, sizeof(FileInfo));
	return RET_SUCCESS;
	return RET_SUCCESS;
}

