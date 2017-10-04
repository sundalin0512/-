#include "storage.h"


/// <summary>
/// 创建学生文档文件
/// </summary>
/// <param name="fileName">文件名</param>
/// <param name="fileSize">文件大小字节数</param>
/// <param name="preFileName">前一个文件的文件名</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType CreateStudentFile(char * fileName, size_t fileSize, char * preFileName)
{
	FILE *fp = CreateFile(fileName, fileSize, preFileName);
	if (fp == NULL)
	{
		return RET_ERROR;
	}
	FileClose(fp);
	return RET_SUCCESS;
}

/// <summary>
/// 添加学生信息到文件
/// </summary>
/// <param name="fileName">文件名</param>
/// <param name="index">存储位置</param>
/// <param name="student">要存的学生信息</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType AddStudent(char * fileName, int index, Student * student)
{
	char *buffer = NULL;
	size_t bufferSize = 0;
	size_t size[5] = { 0 };
	size_t bufferOffset = 0;
	ReturnType retValue = RET_SUCCESS;
	FILE* fp = FileOpen(fileName);
	if (fp == NULL)
	{
		return RET_ERROR;
	}
	size[0] = strnlen(student->ID, MAX_STR_LEN) + 1;
	size[1] = strnlen(student->name, MAX_STR_LEN) + 1;
	size[2] = strnlen(student->telephone, MAX_STR_LEN) + 1;
	size[3] = strnlen(student->grade_C, MAX_STR_LEN) + 1;
	size[4] = sizeof(student->Birthday);
	bufferSize += size[0] + sizeof(size_t);
	bufferSize += size[1] + sizeof(size_t);
	bufferSize += size[2] + sizeof(size_t);
	bufferSize += size[3] + sizeof(size_t);
	bufferSize += size[4] + sizeof(size_t);
	buffer = (char*)calloc(bufferSize, 1);
	if (buffer == NULL)
	{
		return RET_NO_HEAP_SPACE;
	}

	memcpy_s(buffer, sizeof(size_t), &size[0], sizeof(size_t));
	bufferOffset += sizeof(size_t);
	memcpy_s(buffer + bufferOffset, size[0], student->ID, size[0]);
	bufferOffset += size[0];

	memcpy_s(buffer + bufferOffset, sizeof(size_t), &size[1], sizeof(size_t));
	bufferOffset += sizeof(size_t);
	memcpy_s(buffer + bufferOffset, size[1], student->name, size[1]);
	bufferOffset += size[1];

	memcpy_s(buffer + bufferOffset, sizeof(size_t), &size[2], sizeof(size_t));
	bufferOffset += sizeof(size_t);
	memcpy_s(buffer + bufferOffset, size[2], student->telephone, size[2]);
	bufferOffset += size[2];

	memcpy_s(buffer + bufferOffset, sizeof(size_t), &size[3], sizeof(size_t));
	bufferOffset += sizeof(size_t);
	memcpy_s(buffer + bufferOffset, size[3], student->grade_C, size[3]);
	bufferOffset += size[3];

	memcpy_s(buffer + bufferOffset, sizeof(size_t), &size[4], sizeof(size_t));
	bufferOffset += sizeof(size_t);
	memcpy_s(buffer + bufferOffset, size[4], &student->Birthday, size[4]);
	bufferOffset += size[4];
	if(index >= 0)
	{
		retValue = InsertData(fp, index, buffer, bufferSize);
	}
	else
	{
		retValue = AppendData(fp, buffer, bufferSize, &index);
	}
	if (retValue != RET_SUCCESS)
	{
		free(buffer);
		return retValue;
	}
	FileClose(fp);
	return RET_SUCCESS;
}

/// <summary>
/// 添加学生信息到文件尾
/// </summary>
/// <param name="fileName">文件名</param>
/// <param name="student">学生信息</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType AppendStudent(char * fileName, Student * student)
{
	return AddStudent(fileName, -1, student);
}

/// <summary>
/// 获取文件中保存的学生信息的条目数
/// </summary>
/// <param name="fileName">文件名</param>
/// <returns>条目数</returns>
int GetStudentSize(char * fileName)
{
	int size = 0;
	FILE *fp = FileOpen(fileName);
	if (fp == NULL)
	{
		return -1;
	}
	size =  GetDataNumber(fp);
	FileClose(fp);
	return size;
}

/// <summary>
/// 获取学生信息
/// </summary>
/// <param name="fileName">文件名</param>
/// <param name="index">在文件中的顺序</param>
/// <param name="student">返回学生信息</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType GetStudent(char * fileName, int index, Student * student)
{
	char *buffer = NULL;
	size_t bufferSize = NULL;
	size_t size[5] = { 0 };
	size_t bufferOffset = 0;
	ReturnType retValue = RET_SUCCESS;
	FILE* fp = FileOpen(fileName);
	if (fp == NULL)
	{
		return RET_ERROR;
	}
	retValue = GetData(fp, (void**)&buffer, &bufferSize, index);
	if (retValue != RET_SUCCESS)
	{
		return retValue;
	}

	memcpy_s(&size[0], sizeof(size_t), buffer, sizeof(size_t));
	bufferOffset += sizeof(size_t);
	student->ID = (char*)calloc(size[0], 1);
	memcpy_s(student->ID, size[0], buffer + bufferOffset, size[0]);
	bufferOffset += size[0];

	memcpy_s(&size[1], sizeof(size_t), buffer + bufferOffset, sizeof(size_t));
	bufferOffset += sizeof(size_t);
	student->name = (char*)calloc(size[1], 1);
	memcpy_s(student->name, size[1], buffer + bufferOffset, size[1]);
	bufferOffset += size[1];

	memcpy_s(&size[2], sizeof(size_t), buffer + bufferOffset, sizeof(size_t));
	bufferOffset += sizeof(size_t);
	student->telephone = (char*)calloc(size[2], 1);
	memcpy_s(student->telephone, size[2], buffer + bufferOffset, size[2]);
	bufferOffset += size[2];

	memcpy_s(&size[3], sizeof(size_t), buffer + bufferOffset, sizeof(size_t));
	bufferOffset += sizeof(size_t);
	student->grade_C = (char*)calloc(size[3], 1);
	memcpy_s(student->grade_C, size[3], buffer + bufferOffset, size[3]);
	bufferOffset += size[3];

	memcpy_s(&size[4], sizeof(size_t), buffer + bufferOffset, sizeof(size_t));
	bufferOffset += sizeof(size_t);
	memcpy_s(&student->Birthday, size[4], buffer + bufferOffset, size[4]);
	bufferOffset += size[4];

	free(buffer);
	FileClose(fp);
	return RET_SUCCESS;
}

/// <summary>
/// 删除学生信息条目
/// </summary>
/// <param name="fileName">文件名</param>
/// <param name="index">要删除的条目的索引</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType DeleteStudent(char * fileName, int index)
{
	ReturnType retValue = RET_SUCCESS;
	FILE* fp = FileOpen(fileName);
	if (fp == NULL)
	{
		return RET_ERROR;
	}
	retValue = DeleteData(fp, index);
	FileClose(fp);
	return retValue;
}

/// <summary>
/// 修改学生信息
/// </summary>
/// <param name="fileName">文件名</param>
/// <param name="index">要修改的学生信息的索引</param>
/// <param name="newStudent">修改后的学生信息</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType ModifyStudent(char * fileName, int index, Student * newStudent)
{
	ReturnType retValue = RET_SUCCESS;
	retValue = DeleteStudent(fileName, index);
	if (retValue != RET_SUCCESS)
	{
		return retValue;
	}
	AddStudent(fileName, index, newStudent);
	return retValue;
}
