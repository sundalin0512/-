#pragma once

#include <string.h>
#include <stdlib.h>

#include "types.h"
#include "storageIO.h"

class StudentFile
{
private:
	File file;
	char *m_fileName;
public:
	StudentFile()
	{
		m_fileName = nullptr;
	}
	~StudentFile()
	{
		if (m_fileName != nullptr)
		{
			delete m_fileName;
		}
	}
	/// <summary>
	/// ����ѧ���ĵ��ļ�
	/// </summary>
	/// <param name="fileName">�ļ���</param>
	/// <param name="fileSize">�ļ���С�ֽ���</param>
	/// <param name="preFileName">ǰһ���ļ����ļ���</param>
	/// <returns> ReturnType <see cref="ReturnType"/> </returns>
	ReturnType CreateStudentFile(char * fileName, size_t fileSize, char * preFileName)
	{
		FILE *fp = file.CreateFile(fileName, fileSize, preFileName);
		if (fp == NULL)
		{
			return RET_ERROR;
		}
		file.FileClose();
		return RET_SUCCESS;
	}
	
	/// <summary>
	/// ���Ĵ洢�ļ�·��
	/// </summary>
	/// <param name="fileName">�ļ�·��</param>
	/// <returns> ReturnType <see cref="ReturnType"/> </returns>
	ReturnType SetStudentFile(char * fileName)
	{
		if (fileName != nullptr)
		{
			delete m_fileName;
		}
		m_fileName = new char[strlen(fileName) + 1];
		strcpy(this->m_fileName, fileName);
		FILE *fp = file.FileOpen(m_fileName);
		if (fp == NULL)
		{
			return RET_ERROR;
		}
		file.FileClose();
		return RET_SUCCESS;
	}

	/// <summary>
	///	��ѯ�ļ��Ƿ����
	/// </summary>
	/// <param name="fileName">�ļ���</param>
	/// <returns>
	/// ����,���� <c>1</c> ������,���� <c>0</c>
	/// </returns>
	bool IsFileExist(char * fileName)
	{
		FILE *fp = file.FileOpen(fileName);
		bool ret = false;
		if (fp != NULL)
		{
			ret = true;
			file.FileClose();
		}

		return ret;
	}

	/// <summary>
	/// ���ѧ����Ϣ���ļ�
	/// </summary>
	/// <param name="fileName">�ļ���</param>
	/// <param name="index">�洢λ��</param>
	/// <param name="student">Ҫ���ѧ����Ϣ</param>
	/// <returns> ReturnType <see cref="ReturnType"/> </returns>
	ReturnType AddStudent(int index, Student * student)
	{
		char *buffer = NULL;
		size_t bufferSize = 0;
		size_t size[5] = { 0 };
		size_t bufferOffset = 0;
		ReturnType retValue = RET_SUCCESS;
		FILE* fp = file.FileOpen(m_fileName);
		if (fp == NULL)
		{
			return RET_ERROR;
		}
		size[0] = strnlen(student->ID, MAX_STR_LEN) + 1;
		size[1] = strnlen(student->name, MAX_STR_LEN) + 1;
		size[2] = strnlen(student->sexual, MAX_STR_LEN) + 1;
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
		memcpy_s(buffer + bufferOffset, size[2], student->sexual, size[2]);
		bufferOffset += size[2];

		memcpy_s(buffer + bufferOffset, sizeof(size_t), &size[3], sizeof(size_t));
		bufferOffset += sizeof(size_t);
		memcpy_s(buffer + bufferOffset, size[3], student->grade_C, size[3]);
		bufferOffset += size[3];

		memcpy_s(buffer + bufferOffset, sizeof(size_t), &size[4], sizeof(size_t));
		bufferOffset += sizeof(size_t);
		memcpy_s(buffer + bufferOffset, size[4], &student->Birthday, size[4]);
		bufferOffset += size[4];
		if (index >= 0)
		{
			retValue = file.InsertData(index, buffer, bufferSize);
		}
		else
		{
			retValue = file.AppendData(buffer, bufferSize, &index);
		}
		if (retValue != RET_SUCCESS)
		{
			free(buffer);
			return retValue;
		}
		file.FileClose();
		return RET_SUCCESS;
	}

	/// <summary>
	/// ���ѧ����Ϣ���ļ�β
	/// </summary>
	/// <param name="fileName">�ļ���</param>
	/// <param name="student">ѧ����Ϣ</param>
	/// <returns> ReturnType <see cref="ReturnType"/> </returns>
	ReturnType AppendStudent(Student * student)
	{
		return AddStudent(-1, student);
	}

	/// <summary>
	/// ��ȡ�ļ��б����ѧ����Ϣ����Ŀ��
	/// </summary>
	/// <param name="fileName">�ļ���</param>
	/// <returns>��Ŀ��</returns>
	int GetStudentSize()
	{
		int size = 0;
		FILE *fp = file.FileOpen(m_fileName);
		if (fp == NULL)
		{
			return -1;
		}
		size = file.GetDataNumber();
		file.FileClose();
		return size;
	}

	/// <summary>
	/// ��ȡѧ����Ϣ
	/// </summary>
	/// <param name="fileName">�ļ���</param>
	/// <param name="index">���ļ��е�˳��</param>
	/// <param name="student">����ѧ����Ϣ</param>
	/// <returns> ReturnType <see cref="ReturnType"/> </returns>
	ReturnType GetStudent(int index, Student * student)
	{
		char *buffer = NULL;
		size_t bufferSize = 0;
		size_t size[5] = { 0 };
		size_t bufferOffset = 0;
		ReturnType retValue = RET_SUCCESS;
		FILE* fp = file.FileOpen(m_fileName);
		if (fp == NULL)
		{
			return RET_ERROR;
		}
		retValue = file.GetData((void**)&buffer, &bufferSize, index);
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
		student->sexual = (char*)calloc(size[2], 1);
		memcpy_s(student->sexual, size[2], buffer + bufferOffset, size[2]);
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
		file.FileClose();
		return RET_SUCCESS;
	}

	/// <summary>
	/// ɾ��ѧ����Ϣ��Ŀ
	/// </summary>
	/// <param name="fileName">�ļ���</param>
	/// <param name="index">Ҫɾ������Ŀ������</param>
	/// <returns> ReturnType <see cref="ReturnType"/> </returns>
	ReturnType DeleteStudent(int index)
	{
		ReturnType retValue = RET_SUCCESS;
		FILE* fp = file.FileOpen(m_fileName);
		if (fp == NULL)
		{
			return RET_ERROR;
		}
		retValue = file.DeleteData(index);
		file.FileClose();
		return retValue;
	}

	/// <summary>
	/// �޸�ѧ����Ϣ
	/// </summary>
	/// <param name="fileName">�ļ���</param>
	/// <param name="index">Ҫ�޸ĵ�ѧ����Ϣ������</param>
	/// <param name="newStudent">�޸ĺ��ѧ����Ϣ</param>
	/// <returns> ReturnType <see cref="ReturnType"/> </returns>
	ReturnType ModifyStudent(int index, Student * newStudent)
	{
		ReturnType retValue = RET_SUCCESS;
		retValue = DeleteStudent(index);
		if (retValue != RET_SUCCESS)
		{
			return retValue;
		}
		AddStudent(index, newStudent);
		return retValue;
	}

	/// <summary>
	/// �ļ���ʹ����Ϣ
	/// </summary>
	/// <param name="fileName">�ļ���</param>
	/// <param name="sizeList">����ÿ��ʹ�ÿռ�ĳ���</param>
	/// <param name="statusList">����ÿ��ʹ�ÿռ��״̬ 0��δʹ�� 1����ʹ��</param>
	/// <param name="listSize">����List�ĳ���</param>
	/// <returns> ReturnType <see cref="ReturnType"/> </returns>
	ReturnType GetStudentFileStatus(int ** sizeList, int ** statusList, int * listSize)
	{
		ReturnType retVal = RET_SUCCESS;
		FILE *fp = file.FileOpen(m_fileName);
		retVal = file.GetFileStatus(sizeList, statusList, listSize);
		file.FileClose();
		return retVal;
	}

	/// <summary>
	/// ��Ƭ����
	/// </summary>
	/// <param name="fileName">�ļ���</param>
	/// <returns> ReturnType <see cref="ReturnType"/> </returns>
	ReturnType FileDefragment()
	{
		ReturnType retVal = RET_SUCCESS;
		FILE *fp = file.FileOpen(m_fileName);
		retVal = file.Defragment();
		file.FileClose();
		return retVal;
	}

};
