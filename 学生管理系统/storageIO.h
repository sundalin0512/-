#pragma once
#include <stdio.h>
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
/// 创建文件，返回文件指针
/// </summary>
/// <param name="fileName">要创建的文件名</param>
/// <param name="sizeBytes">要创建的文件字节数</param>
/// <param name="preFileName">前一个文件的文件名</param>
/// <returns>创建的文件指针，失败返回空指针</returns>
FILE *CreateFile(const char *fileName, const size_t sizeBytes, const char *preFileName);

/// <summary>
/// 打开已存在的文件，返回文件指针 
/// </summary>
/// <param name="fileName">要打开的文件名</param>
/// <returns>打开的文件指针，失败返回空指针</returns>
FILE *FileOpen(const char *fileName);

/// <summary>
/// 关闭文件
/// </summary>
/// <param name="stream">要关闭的文件指针</param>
void FileClose(FILE *stream);

/// <summary>
/// 获取文件长度
/// </summary>
/// <param name="stream">文件指针</param>
/// <returns>文件的字节数</returns>
size_t GetFileSize(FILE *stream);

/// <summary>
/// 获取所存储数据的条目数
/// </summary>
/// <param name="stream">文件指针</param>
/// <returns>所存储数据的条目数</returns>
size_t GetDataNumber(FILE *stream);

/// <summary>
/// 在指定索引处添加数据
/// </summary>
/// <param name="stream">文件指针</param>
/// <param name="index">索引</param>
/// <param name="buffer">要写入的数据</param>
/// <param name="bytesToInsert">要写入的字节数</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType InsertData(FILE *stream, const int index, const void* buffer, const size_t bytesToInsert);

/// <summary>
/// 在索引尾添加数据
/// </summary>
/// <param name="stream">文件指针</param>
/// <param name="buffer">要写入的数据</param>
/// <param name="bytesToInsert">要写入的字节数</param>
/// <param name="index">返回插入的索引</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType AppendData(FILE *stream, const void * buffer, const size_t bytesToInsert, int *index);

/// <summary>
/// 根据索引获取文件中的数据
/// </summary>
/// <param name="stream">文件指针</param>
/// <param name="buffer">获取的数据（传出参数）</param>
/// <param name="bytesHasGet">获取的数据的字节数（传出参数）</param>
/// <param name="index">要获取的数据的索引</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType GetData(FILE *stream, void **buffer, size_t *bytesHasGet, const int index);

/// <summary>
/// 删除指定索引的数据
/// </summary>
/// <param name="stream">文件指针</param>
/// <param name="index">要删除的索引</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType DeleteData(FILE *stream, const int index);

/// <summary>
/// 碎片整理
/// </summary>
/// <param name="stream">文件指针</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType Defragment(FILE *stream);

/// <summary>
/// 文件的使用信息
/// </summary>
/// <param name="stream">文件指针</param>
/// <param name="sizeList">返回每个使用空间的长度</param>
/// <param name="statusList">返回每个使用空间的状态 0：未使用 1：已使用</param>
/// <param name="listSize">返回List的长度</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType GetFileStatus(FILE *stream, int **sizeList, int **statusList, int *listSize);