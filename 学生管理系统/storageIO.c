#include "storageIO.h"
#include <stdlib.h>
#include <string.h>

typedef struct FileInfo
{
	size_t fileSize;			// 0x00 �ļ���С
	size_t offsetBegin;			// 0x04 ���ļ���ʼλ��
	int hasNextFile;			// 0x08 �Ƿ�����һ���ļ�
	int hasPreFile;				// 0x0C �Ƿ�����һ���ļ�
	char nextFileName[0x20];	// 0x10 ��һ���ļ����Ŀ¼
	char preFileName[0x20];		// 0x30 ��һ���ļ����Ŀ¼
	size_t offsetUsed;			// 0x50 ���ýڵ�ƫ�ƣ���δ�ýڵ�ǰ
	size_t offsetUnused;		// 0x54 δ�ýڵ�ƫ�ƣ������ýڵ��
	int reserved[2];			// 0x58 ����
	//void *data;				// 0x60 ����
}FileInfo;

typedef struct TagList
{
	size_t size;	//list�ĳ���
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
static size_t FileWrite(FILE *stream, const size_t offsetFromFileStart, const void *buffer, const size_t bytesToWrite)
{
	size_t bytesHasWrite = 0;
	fseek(stream, offsetFromFileStart, SEEK_SET);
	bytesHasWrite = fwrite(buffer, 1, bytesToWrite, stream);
	fflush(stream);
	return bytesHasWrite;
}

/// <summary>
/// ��ָ��λ�ö�ȡָ������
/// </summary>
/// <param name="stream">�ļ�ָ��</param>
/// <param name="offsetFromFileStart">��ȡλ�þ����ļ���ʼ���ֽ���</param>
/// <param name="buffer">�����ȡ�����ݣ���Ҫ��Чָ��</param>
/// <param name="bytesToRead">Ҫ��ȡ���ֽ���</param>
/// <returns>�ɹ���ȡ���ֽ���</returns>
static size_t FileRead(FILE *stream, const size_t offsetFromFileStart, void *buffer, const size_t bytesToRead)
{
	size_t bytesHasRead = 0;
	fseek(stream, offsetFromFileStart, SEEK_SET);
	bytesHasRead = fread(buffer, 1, bytesToRead, stream);
	fflush(stream);
	return bytesHasRead;
}

/// <summary>
/// ��ȡ�ļ���Ϣ
/// </summary>
/// <param name="stream">�ļ�ָ��</param>
/// <returns>�ļ���Ϣ</returns>
static FileInfo *GetFileInfo(FILE *stream)
{
	FileInfo *file = (FileInfo*)calloc(sizeof(FileInfo), 1);
	FileRead(stream, 0, file, sizeof(FileInfo));
	return file;
}

/// <summary>
/// �����ļ��������ļ�ָ��
/// </summary>
/// <param name="fileName">Ҫ�������ļ���</param>
/// <param name="sizeBytes">Ҫ�������ļ��ֽ���</param>
/// <param name="preFileName">ǰһ���ļ����ļ���</param>
/// <returns>�������ļ�ָ�룬ʧ�ܷ��ؿ�ָ��</returns>
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
/// ���ļ��������ļ�ָ��
/// </summary>
/// <param name="fileName">Ҫ�򿪵��ļ���</param>
/// <returns>�򿪵��ļ�ָ�룬ʧ�ܷ��ؿ�ָ��</returns>
FILE *FileOpen(const char *fileName)
{
	FILE *fp = NULL;
	fopen_s(&fp, fileName, "rb+");
	return fp;
}

/// <summary>
/// �ر��ļ�
/// </summary>
/// <param name="stream">Ҫ�رյ��ļ�ָ��</param>
void FileClose(FILE *stream)
{
	if (stream != NULL)
	{
		fclose(stream);
	}
}

/// <summary>
/// ��ȡ�ļ�����
/// </summary>
/// <param name="stream">�ļ�ָ��</param>
/// <returns>�ļ����ֽ���</returns>
size_t GetFileSize(FILE *stream)
{
	size_t size = 0;
	FileRead(stream, 0, &size, sizeof(int));
	return size;
}

/// <summary>
/// Ѱ��ӵ���㹻�ռ��δ�ýڵ�
/// </summary>
/// <param name="size">��Ҫ���ֽ���</param>
/// <param name="tagList">Ҫ���ҵı�<typeparam name="UnusedList"/></param>
/// <returns>�սڵ���±꣨��С��0����-1��ʾû���ҵ�</returns>
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
/// ��ȡδ�ýڵ�
/// </summary>
/// <param name="stream">�ļ�ָ��</param>
/// <param name="fileinfo">�ļ�ͷ��Ϣ</param>
/// <param name="unusedList">����δ�ýڵ��б�</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType GetUnusedList(FILE *stream, const FileInfo *fileinfo, UnusedList **unusedList)
{
	size_t size = 0;
	FileRead(stream, fileinfo->offsetUnused, &size, sizeof(size_t));
	size = size*(sizeof(int) + sizeof(size_t)) + sizeof(size_t);
	*unusedList = (UnusedList*)calloc(size, 1);
	if (*unusedList == NULL)
	{
		return RET_NO_HEAP_SPACE;
	}
	FileRead(stream, fileinfo->offsetUnused, *unusedList, size);
	return RET_SUCCESS;
}

/// <summary>
/// ��ȡ���ýڵ�
/// </summary>
/// <param name="stream">�ļ�ָ��</param>
/// <param name="fileinfo">�ļ�ͷ��Ϣ</param>
/// <param name="usedList">�������ýڵ��б�</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType GetUsedList(FILE *stream, const FileInfo *fileinfo, UnusedList **usedList)
{
	size_t size = 0;
	FileRead(stream, fileinfo->offsetUsed, &size, sizeof(size_t));
	size = size*(sizeof(int) + sizeof(size_t)) + sizeof(size_t);
	*usedList = (UsedList*)calloc(size, 1);
	if (*usedList == NULL)
	{
		return RET_NO_HEAP_SPACE;
	}
	FileRead(stream, fileinfo->offsetUsed, *usedList, size);
	return RET_SUCCESS;
}

/// <summary>
/// ��ָ���������������
/// </summary>
/// <param name="stream">�ļ�ָ��</param>
/// <param name="index">����</param>
/// <param name="buffer">Ҫд�������</param>
/// <param name="bytesToInsert">Ҫд����ֽ���</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType InsertData(FILE *stream, const int index, const void* buffer, const size_t bytesToInsert)
{
	FileInfo *fileinfo = GetFileInfo(stream);
	size_t size = 0;
	UnusedList *unusedList = NULL;
	UsedList *usedList = NULL;
	int unusedPlaceIndex = 0;
	ReturnType retValue = RET_SUCCESS;

	//��ȡδ�ýڵ�
	retValue = GetUnusedList(stream, fileinfo, &unusedList);
	if (retValue != RET_SUCCESS)
	{
		return retValue;
	}
	//��ȡ���ýڵ�
	retValue = GetUsedList(stream, fileinfo, &usedList);
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
		// д�뵽�ļ�β
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
		// д����
		writePosition += FileWrite(stream, writePosition, buffer, bytesToInsert);
		{
			// TODO: 
			// д���ýڵ�
			size_t writeSize = index *(sizeof(int) + sizeof(size_t)) + sizeof(size_t);
			char *writeBuffer = (char*)usedList;
			usedList->size += 1;
			fileinfo->offsetUsed = writePosition;
			writePosition += FileWrite(stream, writePosition, usedList, writeSize);
			writeBuffer += writeSize;
			writeSize = (usedList->size - index - 1) *(sizeof(int) + sizeof(size_t));
			writePosition += FileWrite(stream, writePosition, &usedListPosition, sizeof(size_t));
			writePosition += FileWrite(stream, writePosition, &bytesToInsert, sizeof(size_t));
			writePosition += FileWrite(stream, writePosition, writeBuffer, writeSize);
		}
		// дδ�ýڵ�
		fileinfo->offsetUnused = writePosition;
		FileWrite(stream, writePosition, unusedList, unusedListSize);
		// �޸��ļ�ͷ
		FileWrite(stream, 0, fileinfo, sizeof(FileInfo));
		return RET_SUCCESS;
	}
	{
		// д�뵽δʹ�ÿռ�
		size_t writePosition = unusedList->list[unusedPlaceIndex].offset;
		size_t writeLength = bytesToInsert;
		size_t freeSpaceLength = unusedList->list[unusedPlaceIndex].size;
		size_t fileLength = GetFileSize(stream);
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
		// д����
		FileWrite(stream, writePosition, buffer, writeLength);
		// д���ýڵ�
		insertOffset = writePosition;
		writePosition = fileinfo->offsetUsed;
		usedList->size += 1;
		writePosition += FileWrite(stream, writePosition, usedList, usedListSize);
		writePosition += FileWrite(stream, writePosition, &insertOffset, sizeof(size_t));
		writePosition += FileWrite(stream, writePosition, &bytesToInsert, sizeof(size_t));
		// дδ�ýڵ�
		fileinfo->offsetUnused = writePosition;
		if (writeLength == freeSpaceLength)
		{
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
			unusedList->list[unusedPlaceIndex].offset += writeLength;
			FileWrite(stream, writePosition, unusedList, unusedListSize);
		}
		// �޸��ļ�ͷ
		FileWrite(stream, 0, fileinfo, sizeof(FileInfo));
		return RET_SUCCESS;
	}
}

/// <summary>
/// ������β�������
/// </summary>
/// <param name="stream">�ļ�ָ��</param>
/// <param name="buffer">Ҫд�������</param>
/// <param name="bytesToInsert">Ҫд����ֽ���</param>
/// <param name="index">���ز��������</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType AppendData(FILE *stream, const void * buffer, const size_t bytesToInsert, int * index)
{
	FileInfo *file = GetFileInfo(stream);
	size_t size = 0;
	FileRead(stream, file->offsetUsed, &size, sizeof(size_t));
	*index = size;
	return InsertData(stream, size, buffer, bytesToInsert);
}

/// <summary>
/// ɾ��ָ������������
/// </summary>
/// <param name="stream">�ļ�ָ��</param>
/// <param name="index">Ҫɾ��������</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType DeleteData(FILE *stream, const int index)
{
	FileInfo *fileinfo = GetFileInfo(stream);
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

	//��ȡδ�ýڵ�
	retValue = GetUnusedList(stream, fileinfo, &unusedList);
	if (retValue != RET_SUCCESS)
	{
		return retValue;
	}
	//��ȡ���ýڵ�
	retValue = GetUsedList(stream, fileinfo, &usedList);
	if (retValue != RET_SUCCESS)
	{
		return retValue;
	}
	if (index > usedList->size - 1)
	{
		return RET_ILLEGAL_INDEX;
	}

	// ����Ҫд���δ�ýڵ������
	unusedBlockOffset = usedList->list[index].offset;
	unusedBlockSize = usedList->list[index].size;
	// д���ýڵ�
	usedList->size -= 1;
	offset = index *(sizeof(int) + sizeof(size_t)) + sizeof(size_t);
	writePosition = fileinfo->offsetUsed;
	FileWrite(stream, writePosition, usedList, sizeof(size_t));
	writePosition = fileinfo->offsetUsed + offset;
	writeBuffer = (char*)usedList;
	writeBuffer += offset + sizeof(int) + sizeof(size_t);
	writeLength = usedList->size *(sizeof(int) + sizeof(size_t)) + sizeof(size_t) - offset;

	writePosition += FileWrite(stream, writePosition, writeBuffer, writeLength);
	// дδ�ýڵ�
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
		// ���Ժϲ��սڵ�
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
		fileinfo->offsetUnused = writePosition;
		writeLength = newUnusedList->size*(sizeof(int) + sizeof(size_t)) + sizeof(size_t);
		FileWrite(stream, writePosition, newUnusedList, writeLength);
	}
	FileWrite(stream, 0, fileinfo, sizeof(FileInfo));
	return RET_SUCCESS;
}

/// <summary>
/// ��Ƭ����
/// </summary>
/// <param name="stream">�ļ�ָ��</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType Defragment(FILE *stream)
{
	FileInfo *fileinfo = GetFileInfo(stream);
	UsedList *usedList = NULL;
	UsedList *ussdListCopy = NULL;
	ReturnType retValue = RET_SUCCESS;
	size_t writePosition = sizeof(FileInfo);
	int usedListSize = 0;
	int count = 0;

	//��ȡ���ýڵ�
	retValue = GetUsedList(stream, fileinfo, &usedList);
	if (retValue != RET_SUCCESS)
	{
		return retValue;
	}
	//����һ�����ýڵ�
	retValue = GetUsedList(stream, fileinfo, &ussdListCopy);
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
		FileRead(stream, minOffset, buffer, minSize);
		ussdListCopy->list[minIndex].offset = 0;
		usedList->list[minIndex].offset = writePosition;
		writePosition += FileWrite(stream, writePosition, buffer, minSize);
		free(buffer);
	}
	fileinfo->offsetUsed = writePosition;
	usedListSize = usedList->size*(sizeof(int) + sizeof(size_t)) + sizeof(size_t);
	writePosition += FileWrite(stream, writePosition, usedList, usedListSize);
	fileinfo->offsetUnused = writePosition;
	count = 0;
	FileWrite(stream, writePosition, &count, sizeof(size_t));

	FileWrite(stream, 0, fileinfo, sizeof(FileInfo));

	return RET_SUCCESS;
}