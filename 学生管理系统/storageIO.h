#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

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

class File
{
private:
	FileInfo fileinfo;
	FILE *stream;
public:
	/// <summary>
	/// ��ָ��λ��д��ָ����������
	/// </summary>
	/// <param name="offsetFromFileStart">д��λ�þ����ļ���ʼ���ֽ���</param>
	/// <param name="buffer">Ҫд�������</param>
	/// <param name="bytesToWrite">Ҫд����ֽ���</param>
	/// <returns>�ɹ�д����ֽ���</returns>
	size_t FileWrite(const size_t offsetFromFileStart, const void *buffer, const size_t bytesToWrite)
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
	/// <param name="offsetFromFileStart">��ȡλ�þ����ļ���ʼ���ֽ���</param>
	/// <param name="buffer">�����ȡ�����ݣ���Ҫ��Чָ��</param>
	/// <param name="bytesToRead">Ҫ��ȡ���ֽ���</param>
	/// <returns>�ɹ���ȡ���ֽ���</returns>
	size_t FileRead(const size_t offsetFromFileStart, void *buffer, const size_t bytesToRead)
	{
		size_t bytesHasRead = 0;
		fseek(stream, offsetFromFileStart, SEEK_SET);
		bytesHasRead = fread(buffer, 1, bytesToRead, stream);
		fflush(stream);
		return bytesHasRead;
	}

private:
	/// <summary>
	/// ��ȡ�ļ���Ϣ
	/// </summary>
	/// <returns>�ļ���Ϣ</returns>
	void GetFileInfo()
	{
		FileRead(0, &fileinfo, sizeof(FileInfo));
	}

public:
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
			fileinfo = {
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
			void *data = calloc(sizeBytes - 0x60, 1);
			if (data == NULL)
			{
				FileClose();
				return NULL;
			}
			((int*)data)[0] = 0;
			((int*)data)[1] = 0;
			FileWrite(0, &fileinfo, 0x60);
			FileWrite(0x60, data, sizeBytes - 0x60);
			free(data);
			return stream;
		}
		else
		{
			return NULL;
		}
	}

	/// <summary>
	/// ���Ѵ��ڵ��ļ��������ļ�ָ��
	/// </summary>
	/// <param name="fileName">Ҫ�򿪵��ļ���</param>
	/// <returns>�򿪵��ļ�ָ�룬ʧ�ܷ��ؿ�ָ��</returns>
	FILE *FileOpen(const char *fileName)
	{
		fopen_s(&stream, fileName, "rb+");
		return stream;
	}

	/// <summary>
	/// �ر��ļ�
	/// </summary>
	void FileClose()
	{
		if (stream != NULL)
		{
			fclose(stream);
		}
	}

	/// <summary>
	/// ��ȡ�ļ�����
	/// </summary>
	/// <returns>�ļ����ֽ���</returns>
	size_t GetFileSize()
	{
		size_t size = 0;
		FileRead(0, &size, sizeof(int));
		return size;
	}

	/// <summary>
	/// ��ȡ���洢���ݵ���Ŀ��
	/// </summary>
	/// <returns>���洢���ݵ���Ŀ��</returns>
	size_t GetDataNumber()
	{
		size_t size = 0;
		GetFileInfo();
		FileRead(fileinfo.offsetUsed, &size, sizeof(size_t));
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
	/// <param name="fileinfo">�ļ�ͷ��Ϣ</param>
	/// <param name="unusedList">����δ�ýڵ��б�</param>
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
	/// ��ȡ���ýڵ�
	/// </summary>
	/// <param name="fileinfo">�ļ�ͷ��Ϣ</param>
	/// <param name="usedList">�������ýڵ��б�</param>
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
	/// ��ָ���������������
	/// </summary>
	/// <param name="index">����</param>
	/// <param name="buffer">Ҫд�������</param>
	/// <param name="bytesToInsert">Ҫд����ֽ���</param>
	/// <returns> ReturnType <see cref="ReturnType"/> </returns>
	ReturnType InsertData(const int index, const void *buffer, const size_t bytesToInsert)
	{
		GetFileInfo();
		size_t size = 0;
		UnusedList *unusedList = NULL;
		UsedList *usedList = NULL;
		int unusedPlaceIndex = 0;
		ReturnType retValue = RET_SUCCESS;

		//��ȡδ�ýڵ�
		retValue = GetUnusedList(&unusedList);
		if (retValue != RET_SUCCESS)
		{
			return retValue;
		}
		//��ȡ���ýڵ�
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
			// д�뵽�ļ�β
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
			// д����
			writePosition += FileWrite(writePosition, buffer, bytesToInsert);
			{
				// д���ýڵ�
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
			// дδ�ýڵ�
			fileinfo.offsetUnused = writePosition;
			FileWrite(writePosition, unusedList, unusedListSize);
			// �޸��ļ�ͷ
			FileWrite(0, &fileinfo, sizeof(FileInfo));
			return RET_SUCCESS;
		}
		{
			// д�뵽δʹ�ÿռ�
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
			// д����
			FileWrite(writePosition, buffer, writeLength);
			// д���ýڵ�
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
			// дδ�ýڵ�
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
			// �޸��ļ�ͷ
			FileWrite(0, &fileinfo, sizeof(FileInfo));
			return RET_SUCCESS;
		}
	}

	/// <summary>
	/// ������β�������
	/// </summary>
	/// <param name="buffer">Ҫд�������</param>
	/// <param name="bytesToInsert">Ҫд����ֽ���</param>
	/// <param name="index">���ز��������</param>
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
	/// ����������ȡ�ļ��е�����
	/// </summary>
	/// <param name="buffer">��ȡ�����ݣ�����������</param>
	/// <param name="bytesHasGet">��ȡ�����ݵ��ֽ���������������</param>
	/// <param name="index">Ҫ��ȡ�����ݵ�����</param>
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
	/// ɾ��ָ������������
	/// </summary>
	/// <param name="index">Ҫɾ��������</param>
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

		//��ȡδ�ýڵ�
		retValue = GetUnusedList(&unusedList);
		if (retValue != RET_SUCCESS)
		{
			return retValue;
		}
		//��ȡ���ýڵ�
		retValue = GetUsedList(&usedList);
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
		writePosition = fileinfo.offsetUsed;
		FileWrite(writePosition, usedList, sizeof(size_t));
		writePosition = fileinfo.offsetUsed + offset;
		writeBuffer = (char*)usedList;
		writeBuffer += offset + sizeof(int) + sizeof(size_t);
		writeLength = usedList->size *(sizeof(int) + sizeof(size_t)) + sizeof(size_t) - offset;

		writePosition += FileWrite(writePosition, writeBuffer, writeLength);
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
			fileinfo.offsetUnused = writePosition;
			writeLength = newUnusedList->size*(sizeof(int) + sizeof(size_t)) + sizeof(size_t);
			FileWrite(writePosition, newUnusedList, writeLength);
		}
		FileWrite(0, &fileinfo, sizeof(FileInfo));
		return RET_SUCCESS;
	}

	/// <summary>
	/// ��Ƭ����
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

		//��ȡ���ýڵ�
		retValue = GetUsedList(&usedList);
		if (retValue != RET_SUCCESS)
		{
			return retValue;
		}
		//����һ�����ýڵ�
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
	/// �ļ���ʹ����Ϣ
	/// </summary>
	/// <param name="sizeList">����ÿ��ʹ�ÿռ�ĳ���</param>
	/// <param name="statusList">����ÿ��ʹ�ÿռ��״̬ 0��δʹ�� 1����ʹ��</param>
	/// <param name="listSize">����List�ĳ���</param>
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
