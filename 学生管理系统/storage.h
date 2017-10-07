#pragma once

#include <string.h>
#include <stdlib.h>

#include "types.h"

/// <summary>
/// ����ѧ���ĵ��ļ�
/// </summary>
/// <param name="fileName">�ļ���</param>
/// <param name="fileSize">�ļ���С�ֽ���</param>
/// <param name="preFileName">ǰһ���ļ����ļ���</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType CreateStudentFile(char *fileName, size_t fileSize, char *preFileName);

/// <summary>
///	��ѯ�ļ��Ƿ����
/// </summary>
/// <param name="fileName">�ļ���</param>
/// <returns>
/// ����,���� <c>1</c> ������,���� <c>0</c>
/// </returns>
int IsFileExist(char *fileName);

/// <summary>
/// ���ѧ����Ϣ���ļ�
/// </summary>
/// <param name="fileName">�ļ���</param>
/// <param name="index">�洢λ��</param>
/// <param name="student">Ҫ���ѧ����Ϣ</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType AddStudent(char *fileName, int index, Student *student);

/// <summary>
/// ���ѧ����Ϣ���ļ�β
/// </summary>
/// <param name="fileName">�ļ���</param>
/// <param name="student">ѧ����Ϣ</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType AppendStudent(char *fileName, Student *student);

/// <summary>
/// ��ȡ�ļ��б����ѧ����Ϣ����Ŀ��
/// </summary>
/// <param name="fileName">�ļ���</param>
/// <returns>��Ŀ��</returns>
int GetStudentSize(char * fileName);

/// <summary>
/// ��ȡѧ����Ϣ
/// </summary>
/// <param name="fileName">�ļ���</param>
/// <param name="index">���ļ��е�˳��</param>
/// <param name="student">����ѧ����Ϣ</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType GetStudent(char *fileName, int index, Student *student);

/// <summary>
/// ɾ��ѧ����Ϣ��Ŀ
/// </summary>
/// <param name="fileName">�ļ���</param>
/// <param name="index">Ҫɾ������Ŀ������</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType DeleteStudent(char *fileName, int index);

/// <summary>
/// �޸�ѧ����Ϣ
/// </summary>
/// <param name="fileName">�ļ���</param>
/// <param name="index">Ҫ�޸ĵ�ѧ����Ϣ������</param>
/// <param name="newStudent">�޸ĺ��ѧ����Ϣ</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType ModifyStudent(char *fileName, int index, Student *newStudent);

/// <summary>
/// �ļ���ʹ����Ϣ
/// </summary>
/// <param name="fileName">�ļ���</param>
/// <param name="sizeList">����ÿ��ʹ�ÿռ�ĳ���</param>
/// <param name="statusList">����ÿ��ʹ�ÿռ��״̬ 0��δʹ�� 1����ʹ��</param>
/// <param name="listSize">����List�ĳ���</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType GetStudentFileStatus(char *fileName, int **sizeList, int **statusList, int *listSize);

/// <summary>
/// ��Ƭ����
/// </summary>
/// <param name="fileName">�ļ���</param>
/// <returns> ReturnType <see cref="ReturnType"/> </returns>
ReturnType FileDefragment(char *fileName);