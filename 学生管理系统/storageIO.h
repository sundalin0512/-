#pragma once
#include <stdio.h>
#include "types.h"

/// <summary>
/// 创建文件，返回文件指针
/// </summary>
/// <param name="fileName">要创建的文件名</param>
/// <param name="sizeBytes">要创建的文件字节数</param>
/// <param name="preFileName">前一个文件的文件名</param>
/// <returns>创建的文件指针，失败返回空指针</returns>
FILE *CreateFile(const char *fileName, const size_t sizeBytes, const char *preFileName);

/// <summary>
/// 打开文件，返回文件指针
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
ReturnType AppendData(FILE *stream, const void * buffer, const size_t bytesToInsert, int * index);

/// <summary>
/// 删除指定索引的数据
/// </summary>
/// <param name="stream">文件指针</param>
/// <param name="index">要删除的索引</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType DeleteData(FILE *stream, int index);

