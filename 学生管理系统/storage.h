#pragma once

#include <string.h>
#include <stdlib.h>

#include "types.h"
#include "storageIO.h"

/// <summary>
/// 创建学生文档文件
/// </summary>
/// <param name="fileName">文件名</param>
/// <param name="fileSize">文件大小字节数</param>
/// <param name="preFileName">前一个文件的文件名</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType CreateStudentFile(char *fileName, size_t fileSize, char *preFileName);

/// <summary>
/// 添加学生信息到文件
/// </summary>
/// <param name="fileName">文件名</param>
/// <param name="index">存储位置</param>
/// <param name="student">要存的学生信息</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType AddStudent(char *fileName, int index, Student *student);

/// <summary>
/// 添加学生信息到文件尾
/// </summary>
/// <param name="fileName">文件名</param>
/// <param name="student">学生信息</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType AppendStudent(char *fileName, Student *student);

/// <summary>
/// 获取文件中保存的学生信息的条目数
/// </summary>
/// <param name="fileName">文件名</param>
/// <returns>条目数</returns>
int GetStudentSize(char * fileName);

/// <summary>
/// 获取学生信息
/// </summary>
/// <param name="fileName">文件名</param>
/// <param name="index">在文件中的顺序</param>
/// <param name="student">返回学生信息</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType GetStudent(char *fileName, int index, Student *student);

/// <summary>
/// 删除学生信息条目
/// </summary>
/// <param name="fileName">文件名</param>
/// <param name="index">要删除的条目的索引</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType DeleteStudent(char *fileName, int index);

/// <summary>
/// 修改学生信息
/// </summary>
/// <param name="fileName">文件名</param>
/// <param name="index">要修改的学生信息的索引</param>
/// <param name="newStudent">修改后的学生信息</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType ModifyStudent(char *fileName, int index, Student *newStudent);