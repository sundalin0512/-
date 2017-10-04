#pragma once
#include <stdio.h>
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

/// <summary>
/// �����ļ��������ļ�ָ��
/// </summary>
/// <param name="fileName">Ҫ�������ļ���</param>
/// <param name="sizeBytes">Ҫ�������ļ��ֽ���</param>
/// <param name="preFileName">ǰһ���ļ����ļ���</param>
/// <returns>�������ļ�ָ�룬ʧ�ܷ��ؿ�ָ��</returns>
FILE *CreateFile(const char *fileName, const size_t sizeBytes, const char *preFileName);

/// <summary>
/// ���Ѵ��ڵ��ļ��������ļ�ָ�� 
/// </summary>
/// <param name="fileName">Ҫ�򿪵��ļ���</param>
/// <returns>�򿪵��ļ�ָ�룬ʧ�ܷ��ؿ�ָ��</returns>
FILE *FileOpen(const char *fileName);

/// <summary>
/// �ر��ļ�
/// </summary>
/// <param name="stream">Ҫ�رյ��ļ�ָ��</param>
void FileClose(FILE *stream);

/// <summary>
/// ��ȡ�ļ�����
/// </summary>
/// <param name="stream">�ļ�ָ��</param>
/// <returns>�ļ����ֽ���</returns>
size_t GetFileSize(FILE *stream);

/// <summary>
/// ��ȡ���洢���ݵ���Ŀ��
/// </summary>
/// <param name="stream">�ļ�ָ��</param>
/// <returns>���洢���ݵ���Ŀ��</returns>
size_t GetDataNumber(FILE *stream);

/// <summary>
/// ��ָ���������������
/// </summary>
/// <param name="stream">�ļ�ָ��</param>
/// <param name="index">����</param>
/// <param name="buffer">Ҫд�������</param>
/// <param name="bytesToInsert">Ҫд����ֽ���</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType InsertData(FILE *stream, const int index, const void* buffer, const size_t bytesToInsert);

/// <summary>
/// ������β�������
/// </summary>
/// <param name="stream">�ļ�ָ��</param>
/// <param name="buffer">Ҫд�������</param>
/// <param name="bytesToInsert">Ҫд����ֽ���</param>
/// <param name="index">���ز��������</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType AppendData(FILE *stream, const void * buffer, const size_t bytesToInsert, int *index);

/// <summary>
/// ����������ȡ�ļ��е�����
/// </summary>
/// <param name="stream">�ļ�ָ��</param>
/// <param name="buffer">��ȡ�����ݣ�����������</param>
/// <param name="bytesHasGet">��ȡ�����ݵ��ֽ���������������</param>
/// <param name="index">Ҫ��ȡ�����ݵ�����</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType GetData(FILE *stream, void **buffer, size_t *bytesHasGet, const int index);

/// <summary>
/// ɾ��ָ������������
/// </summary>
/// <param name="stream">�ļ�ָ��</param>
/// <param name="index">Ҫɾ��������</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType DeleteData(FILE *stream, const int index);

/// <summary>
/// ��Ƭ����
/// </summary>
/// <param name="stream">�ļ�ָ��</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType Defragment(FILE *stream);

/// <summary>
/// �ļ���ʹ����Ϣ
/// </summary>
/// <param name="stream">�ļ�ָ��</param>
/// <param name="sizeList">����ÿ��ʹ�ÿռ�ĳ���</param>
/// <param name="statusList">����ÿ��ʹ�ÿռ��״̬ 0��δʹ�� 1����ʹ��</param>
/// <param name="listSize">����List�ĳ���</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType GetFileStatus(FILE *stream, int **sizeList, int **statusList, int *listSize);