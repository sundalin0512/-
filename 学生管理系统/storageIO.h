#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

typedef struct FileInfo
{
	size_t fileSize;			// 0x00 文件大小
	size_t offsetBegin;			// 0x04 本文件开始位置
	int hasNextFile;			// 0x08 是否有下一个文件
	int hasPreFile;				// 0x0C 是否有上一个文件
	char nextFileName[0x20];	// 0x10 下一个文件相对目录
	char preFileName[0x20];		// 0x30 上一个文件相对目录
	size_t offsetUsed;			// 0x50 已用节点偏移，在未用节点前
	size_t offsetUnused;		// 0x54 未用节点偏移，在已用节点后
	int reserved[6];			// 0x58 保留
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

class File
{
private:
	FileInfo fileinfo;
	FILE *stream;
public:
	/// <summary>
	/// 在指定位置写入指定长度数据
	/// </summary>
	/// <param name="offsetFromFileStart">写入位置距离文件起始的字节数</param>
	/// <param name="buffer">要写入的数据</param>
	/// <param name="bytesToWrite">要写入的字节数</param>
	/// <returns>成功写入的字节数</returns>
	size_t FileWrite(const size_t offsetFromFileStart, const void *buffer, const size_t bytesToWrite)
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
	/// <param name="offsetFromFileStart">读取位置距离文件起始的字节数</param>
	/// <param name="buffer">保存读取的数据，需要有效指针</param>
	/// <param name="bytesToRead">要读取的字节数</param>
	/// <returns>成功读取的字节数</returns>
	size_t FileRead(const size_t offsetFromFileStart, void *buffer, const size_t bytesToRead)
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
	/// <returns>文件信息</returns>
	void GetFileInfo()
	{
		FileRead(0, &fileinfo, sizeof(FileInfo));
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
		if (sizeBytes > sizeof(FileInfo)+8)
		{
			fileinfo = {
				sizeBytes,
				sizeof(FileInfo),
				0,
				0,
				"",
				"",
				sizeof(FileInfo),
				sizeof(FileInfo)+4,
				0,0
			};
			fopen_s(&stream, fileName, "wb+");
			if (stream == NULL)
			{
				return NULL;
			}
			if (preFileName != NULL)
			{
				FileInfo filePre = { 0 };
				FILE *streamPreFile = NULL;
				File preFile;
				preFile.fileinfo.hasPreFile = 1;
				strcpy_s(preFile.fileinfo.preFileName, 0x20 - 1, preFileName);
				preFile.FileOpen(preFileName);
				preFile.FileRead(0, &preFile.fileinfo, sizeof(preFile.fileinfo));
				filePre.hasNextFile = 1;
				strcpy_s(filePre.nextFileName, 0x20 - 1, fileName);
				preFile.FileWrite(0, &preFile.fileinfo, sizeof(preFile.fileinfo));
				preFile.FileClose();
			}
			void *data = calloc(sizeBytes - sizeof(FileInfo), 1);
			if (data == NULL)
			{
				FileClose();
				return NULL;
			}
			((int*)data)[0] = 0;
			((int*)data)[1] = 0;
			FileWrite(0, &fileinfo, sizeof(FileInfo));
			FileWrite(sizeof(FileInfo), data, sizeBytes - sizeof(FileInfo));
			free(data);
			return stream;
		}
		else
		{
			return NULL;
		}
	}

	/// <summary>
	/// 打开已存在的文件，返回文件指针
	/// </summary>
	/// <param name="fileName">要打开的文件名</param>
	/// <returns>打开的文件指针，失败返回空指针</returns>
	FILE *FileOpen(const char *fileName)
	{
		fopen_s(&stream, fileName, "rb+");
		return stream;
	}

	/// <summary>
	/// 关闭文件
	/// </summary>
	void FileClose()
	{
		if (stream != NULL)
		{
			fclose(stream);
		}
	}

	/// <summary>
	/// 获取文件长度
	/// </summary>
	/// <returns>文件的字节数</returns>
	size_t GetFileSize()
	{
		size_t size = 0;
		FileRead(0, &size, sizeof(int));
		return size;
	}

	/// <summary>
	/// 获取所存储数据的条目数
	/// </summary>
	/// <returns>所存储数据的条目数</returns>
	size_t GetDataNumber()
	{
		size_t size = 0;
		GetFileInfo();
		FileRead(fileinfo.offsetUsed, &size, sizeof(size_t));
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
			if (size <= tagList->list[i].size)
			{
				return i;
			}
		}
		return -1;
	}

	/// <summary>
	/// 读取未用节点
	/// </summary>
	/// <param name="fileinfo">文件头信息</param>
	/// <param name="unusedList">返回未用节点列表</param>
	/// <returns> ReturnType <see cref="ReturnType"/> </returns>
	ReturnType GetUnusedList(UnusedList **unusedList)
	{
		size_t size = 0;
		FileRead(fileinfo.offsetUnused, &size, sizeof(size_t));
		size = size*(sizeof(int) + sizeof(size_t)) + sizeof(size_t);
		*unusedList = (UnusedList*)calloc(size, 1);
		if (*unusedList == NULL)
		{
			return RET_NO_HEAP_SPACE;
		}
		FileRead(fileinfo.offsetUnused, *unusedList, size);
		return RET_SUCCESS;
	}

	/// <summary>
	/// 读取已用节点
	/// </summary>
	/// <param name="fileinfo">文件头信息</param>
	/// <param name="usedList">返回已用节点列表</param>
	/// <returns> ReturnType <see cref="ReturnType"/> </returns>
	ReturnType GetUsedList(UnusedList **usedList)
	{
		size_t size = 0;
		FileRead(fileinfo.offsetUsed, &size, sizeof(size_t));
		size = size*(sizeof(int) + sizeof(size_t)) + sizeof(size_t);
		*usedList = (UsedList*)calloc(size, 1);
		if (*usedList == NULL)
		{
			return RET_NO_HEAP_SPACE;
		}
		FileRead(fileinfo.offsetUsed, *usedList, size);
		return RET_SUCCESS;
	}

	/// <summary>
	/// 在指定索引处添加数据
	/// </summary>
	/// <param name="index">索引</param>
	/// <param name="buffer">要写入的数据</param>
	/// <param name="bytesToInsert">要写入的字节数</param>
	/// <returns> ReturnType <see cref="ReturnType"/> </returns>
	ReturnType InsertData(const int index, const void *buffer, const size_t bytesToInsert)
	{
		GetFileInfo();
		size_t size = 0;
		UnusedList *unusedList = NULL;
		UsedList *usedList = NULL;
		int unusedPlaceIndex = 0;
		ReturnType retValue = RET_SUCCESS;

		//读取未用节点
		retValue = GetUnusedList(&unusedList);
		if (retValue != RET_SUCCESS)
		{
			return retValue;
		}
		//读取已用节点
		retValue = GetUsedList(&usedList);
		if (retValue != RET_SUCCESS)
		{
			return retValue;
		}

		size = bytesToInsert;
		if (index > usedList->size)
		{
			return RET_ILLEGAL_INDEX;
		}
		unusedPlaceIndex = FindEmptyPlace(bytesToInsert, unusedList);
		if (unusedPlaceIndex == -1)
		{
			// 写入到文件尾
			size_t writePosition = fileinfo.offsetUsed;
			size_t usedListPosition = fileinfo.offsetUsed;
			size_t fileLength = GetFileSize();
			size_t unusedListSize = unusedList->size*(sizeof(int) + sizeof(size_t)) + sizeof(size_t);
			size_t usedListSize = usedList->size*(sizeof(int) + sizeof(size_t)) + sizeof(size_t);
			size_t additionalSize = sizeof(int) + sizeof(size_t);

			if (fileLength < writePosition + bytesToInsert + unusedListSize + usedListSize + additionalSize)
			{
				return RET_NOSPACE;
			}
			// 写数据
			writePosition += FileWrite(writePosition, buffer, bytesToInsert);
			{
				// 写已用节点
				size_t writeSize = index *(sizeof(int) + sizeof(size_t)) + sizeof(size_t);
				char *writeBuffer = (char*)usedList;
				usedList->size += 1;
				fileinfo.offsetUsed = writePosition;
				writePosition += FileWrite(writePosition, usedList, writeSize);
				writeBuffer += writeSize;
				writeSize = (usedList->size - index - 1) *(sizeof(int) + sizeof(size_t));
				writePosition += FileWrite(writePosition, &usedListPosition, sizeof(size_t));
				writePosition += FileWrite(writePosition, &bytesToInsert, sizeof(size_t));
				writePosition += FileWrite(writePosition, writeBuffer, writeSize);
			}
			// 写未用节点
			fileinfo.offsetUnused = writePosition;
			FileWrite(writePosition, unusedList, unusedListSize);
			// 修改文件头
			FileWrite(0, &fileinfo, sizeof(FileInfo));
			return RET_SUCCESS;
		}
		{
			// 写入到未使用空间
			size_t writePosition = unusedList->list[unusedPlaceIndex].offset;
			size_t writeLength = bytesToInsert;
			size_t freeSpaceLength = unusedList->list[unusedPlaceIndex].size;
			size_t fileLength = GetFileSize();
			size_t unusedListSize = unusedList->size*(sizeof(int) + sizeof(size_t)) + sizeof(size_t);
			size_t usedListSize = usedList->size*(sizeof(int) + sizeof(size_t)) + sizeof(size_t);
			size_t additionalSize = sizeof(int) + sizeof(size_t);
			size_t insertOffset = 0;

			if (writeLength == freeSpaceLength)
			{
				additionalSize = 0;
			}
			if (fileLength < writePosition + bytesToInsert + unusedListSize + usedListSize + additionalSize)
			{
				return RET_NOSPACE;
			}
			// 写数据
			FileWrite(writePosition, buffer, writeLength);
			// 写已用节点
			{
				size_t writeSize = index *(sizeof(int) + sizeof(size_t)) + sizeof(size_t);
				char *writeBuffer = (char*)usedList;
				size_t usedListPosition = fileinfo.offsetUsed;
				insertOffset = writePosition;
				writePosition = fileinfo.offsetUsed;
				usedList->size += 1;
				writePosition += FileWrite(writePosition, usedList, writeSize);
				writeBuffer += writeSize;
				writeSize = (usedList->size - index - 1) *(sizeof(int) + sizeof(size_t));
				writePosition += FileWrite(writePosition, &insertOffset, sizeof(size_t));
				writePosition += FileWrite(writePosition, &bytesToInsert, sizeof(size_t));
				writePosition += FileWrite(writePosition, writeBuffer, writeSize);
				//insertOffset = writePosition;
				//writePosition = fileinfo.offsetUsed;
				//usedList->size += 1;
				//writePosition += FileWrite(stream, writePosition, usedList, usedListSize);
				//writePosition += FileWrite(stream, writePosition, &insertOffset, sizeof(size_t));
				//writePosition += FileWrite(stream, writePosition, &bytesToInsert, sizeof(size_t));
			}
			// 写未用节点
			fileinfo.offsetUnused = writePosition;
			if (writeLength == freeSpaceLength)
			{
				char *nextWriteBuffer = (char*)unusedList;
				writeLength = unusedPlaceIndex*(sizeof(int) + sizeof(size_t)) + sizeof(size_t);
				writePosition += FileWrite(writePosition, unusedList, writeLength);
				writeLength = unusedListSize - writeLength - (sizeof(int) + sizeof(size_t));
				nextWriteBuffer += writeLength + sizeof(int) + sizeof(size_t);
				FileWrite(writePosition, nextWriteBuffer, writeLength);
			}
			else
			{
				unusedList->list[unusedPlaceIndex].size -= writeLength;
				unusedList->list[unusedPlaceIndex].offset += writeLength;
				FileWrite(writePosition, unusedList, unusedListSize);
			}
			// 修改文件头
			FileWrite(0, &fileinfo, sizeof(FileInfo));
			return RET_SUCCESS;
		}
	}

	/// <summary>
	/// 在索引尾添加数据
	/// </summary>
	/// <param name="buffer">要写入的数据</param>
	/// <param name="bytesToInsert">要写入的字节数</param>
	/// <param name="index">返回插入的索引</param>
	/// <returns> ReturnType <see cref="ReturnType"/> </returns>
	ReturnType AppendData(const void *buffer, const size_t bytesToInsert, int *index)
	{
		GetFileInfo();
		size_t size = 0;
		FileRead(fileinfo.offsetUsed, &size, sizeof(size_t));
		*index = size;
		return InsertData(size, buffer, bytesToInsert);
	}

	/// <summary>
	/// 根据索引获取文件中的数据
	/// </summary>
	/// <param name="buffer">获取的数据（传出参数）</param>
	/// <param name="bytesHasGet">获取的数据的字节数（传出参数）</param>
	/// <param name="index">要获取的数据的索引</param>
	/// <returns> ReturnType <see cref="ReturnType"/> </returns>
	ReturnType GetData(void **buffer, size_t *bytesHasGet, const int index)
	{
		GetFileInfo();
		UsedList *usedList = NULL;
		ReturnType retValue = RET_SUCCESS;
		size_t size = 0;
		size_t offset = 0;
		retValue = GetUsedList(&usedList);
		if (index >= usedList->size)
		{
			return RET_ILLEGAL_INDEX;
		}
		size = usedList->list[index].size;
		offset = usedList->list[index].offset;
		*buffer = calloc(size, 1);
		*bytesHasGet = FileRead(offset, *buffer, size);
		return RET_SUCCESS;
	}

	/// <summary>
	/// 删除指定索引的数据
	/// </summary>
	/// <param name="index">要删除的索引</param>
	/// <returns> ReturnType <see cref="ReturnType"/> </returns>
	ReturnType DeleteData(const int index)
	{
		GetFileInfo();
		size_t size = 0;
		UnusedList *unusedList = NULL;
		UsedList *usedList = NULL;
		size_t writePosition = 0;
		size_t unusedBlockOffset = 0;
		size_t unusedBlockSize = 0;
		char *writeBuffer = NULL;
		int offset = 0;
		int unusedListInsertIndex = 0;
		ReturnType retValue = RET_SUCCESS;
		size_t writeLength = 0;

		//读取未用节点
		retValue = GetUnusedList(&unusedList);
		if (retValue != RET_SUCCESS)
		{
			return retValue;
		}
		//读取已用节点
		retValue = GetUsedList(&usedList);
		if (retValue != RET_SUCCESS)
		{
			return retValue;
		}
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
		writePosition = fileinfo.offsetUsed;
		FileWrite(writePosition, usedList, sizeof(size_t));
		writePosition = fileinfo.offsetUsed + offset;
		writeBuffer = (char*)usedList;
		writeBuffer += offset + sizeof(int) + sizeof(size_t);
		writeLength = usedList->size *(sizeof(int) + sizeof(size_t)) + sizeof(size_t) - offset;

		writePosition += FileWrite(writePosition, writeBuffer, writeLength);
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
		{
			// 尝试合并空节点
			UnusedList *newUnusedList = (UnusedList*)calloc((unusedList->size + 1)*(sizeof(int) + sizeof(size_t)) + sizeof(size_t), 1);
			int count = 0;
			if (newUnusedList == NULL)
			{
				return RET_NO_HEAP_SPACE;
			}
			for (int i = 0; i <= unusedList->size; i++)
			{
				if (i != unusedListInsertIndex && i < unusedList->size)
				{
					newUnusedList->list[count].offset = unusedList->list[i].offset;
					newUnusedList->list[count].size = unusedList->list[i].size;
					count++;
					continue;
				}
				if (i != 0 && newUnusedList->list[count - 1].offset + newUnusedList->list[count - 1].size == unusedBlockOffset)
				{
					newUnusedList->list[count - 1].size += unusedBlockSize;
				}
				else if (i == unusedListInsertIndex)
				{
					newUnusedList->list[count].offset = unusedBlockOffset;
					newUnusedList->list[count].size = unusedBlockSize;
					count++;
				}
				if (i < unusedList->size)
					if (newUnusedList->list[count - 1].offset + newUnusedList->list[count - 1].size == unusedList->list[i].offset)
					{
						newUnusedList->list[count - 1].size += unusedList->list[i].size;
					}
					else
					{
						newUnusedList->list[count].offset = unusedList->list[i].offset;
						newUnusedList->list[count].size = unusedList->list[i].size;
						count++;
					}
			}
			newUnusedList->size = count;
			fileinfo.offsetUnused = writePosition;
			writeLength = newUnusedList->size*(sizeof(int) + sizeof(size_t)) + sizeof(size_t);
			FileWrite(writePosition, newUnusedList, writeLength);
		}
		FileWrite(0, &fileinfo, sizeof(FileInfo));
		return RET_SUCCESS;
	}

	/// <summary>
	/// 碎片整理
	/// </summary>
	/// <returns> ReturnType <see cref="ReturnType"/> </returns>
	ReturnType Defragment()
	{
		GetFileInfo();
		UsedList *usedList = NULL;
		UsedList *ussdListCopy = NULL;
		ReturnType retValue = RET_SUCCESS;
		size_t writePosition = sizeof(FileInfo);
		int usedListSize = 0;
		int count = 0;

		//读取已用节点
		retValue = GetUsedList(&usedList);
		if (retValue != RET_SUCCESS)
		{
			return retValue;
		}
		//拷贝一份已用节点
		retValue = GetUsedList(&ussdListCopy);
		if (retValue != RET_SUCCESS)
		{
			return retValue;
		}

		for (count = 0; count < ussdListCopy->size; count++)
		{
			size_t minOffset = 0;
			size_t minSize = 0;
			size_t minIndex = 0;
			void *buffer = NULL;
			for (size_t i = 0; i < ussdListCopy->size; i++)
			{
				if (ussdListCopy->list[i].offset != 0)
				{
					if (minOffset == 0)
					{
						minOffset = ussdListCopy->list[i].offset;
						minSize = ussdListCopy->list[i].size;
						minIndex = i;
					}
					else if (minOffset > ussdListCopy->list[i].offset)
					{
						minOffset = ussdListCopy->list[i].offset;
						minSize = ussdListCopy->list[i].size;
						minIndex = i;
					}
				}
			}
			buffer = calloc(minSize, 1);
			if (buffer == NULL)
			{
				return RET_NO_HEAP_SPACE;
			}
			FileRead(minOffset, buffer, minSize);
			ussdListCopy->list[minIndex].offset = 0;
			usedList->list[minIndex].offset = writePosition;
			writePosition += FileWrite(writePosition, buffer, minSize);
			free(buffer);
		}
		fileinfo.offsetUsed = writePosition;
		usedListSize = usedList->size*(sizeof(int) + sizeof(size_t)) + sizeof(size_t);
		writePosition += FileWrite(writePosition, usedList, usedListSize);
		fileinfo.offsetUnused = writePosition;
		count = 0;
		FileWrite(writePosition, &count, sizeof(size_t));

		FileWrite(0, &fileinfo, sizeof(FileInfo));

		return RET_SUCCESS;
	}

	/// <summary>
	/// 文件的使用信息
	/// </summary>
	/// <param name="sizeList">返回每个使用空间的长度</param>
	/// <param name="statusList">返回每个使用空间的状态 0：未使用 1：已使用</param>
	/// <param name="listSize">返回List的长度</param>
	/// <returns> ReturnType <see cref="ReturnType"/> </returns>
	ReturnType GetFileStatus(int ** sizeList, int ** statusList, int * listSize)
	{
		UnusedList *unusedList;
		UsedList *usedList;
		GetFileInfo();
		GetUnusedList(&unusedList);
		GetUsedList(&usedList);
		*listSize = unusedList->size * 2 + 1;
		*sizeList = (int*)calloc(*listSize, sizeof(int));
		if (*sizeList == NULL)
		{
			return RET_NO_HEAP_SPACE;
		}
		*statusList = (int*)calloc(*listSize, sizeof(int));
		if (*statusList == NULL)
		{
			return RET_NO_HEAP_SPACE;
		}
		if (unusedList->size > 0)
		{
			(*sizeList)[0] = unusedList->list[0].offset - fileinfo.offsetBegin;
			(*statusList)[0] = 1;
			for (int i = 1; i < unusedList->size; i++)
			{
				(*sizeList)[i * 2 - 1] = unusedList->list[i - 1].size;
				(*statusList)[i * 2 - 1] = 0;
				(*sizeList)[i * 2] = unusedList->list[i].offset - unusedList->list[i - 1].offset - unusedList->list[i - 1].size;
				(*statusList)[i * 2] = 1;
			}

			(*sizeList)[unusedList->size * 2 - 1] = unusedList->list[unusedList->size - 1].size;
			(*statusList)[unusedList->size * 2 - 1] = 0;
			(*sizeList)[unusedList->size * 2] = fileinfo.offsetUsed - unusedList->list[unusedList->size - 1].offset - unusedList->list[unusedList->size - 1].size;
			(*statusList)[unusedList->size * 2] = 1;
		}
		else if (usedList->size >0)
		{
			(*sizeList)[0] = fileinfo.offsetUsed - fileinfo.offsetBegin;
			(*statusList)[0] = 1;
		}
		else
		{
			(*sizeList)[0] = 0;
			(*statusList)[0] = 0;
		}
		return RET_SUCCESS;
	}

};
