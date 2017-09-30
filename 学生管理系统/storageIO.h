#pragma once
#include <stdio.h>

/// <summary>
/// �����ļ��������ļ�ָ��
/// </summary>
/// <param name="fileName">Ҫ�������ļ���</param>
/// <param name="sizeBytes">Ҫ�������ļ��ֽ���</param>
/// <param name="preFileName">ǰһ���ļ����ļ���</param>
/// <returns>�������ļ�ָ�룬ʧ�ܷ��ؿ�ָ��</returns>
FILE *CreateFile(const char *fileName, const size_t sizeBytes, const char *preFileName);

/// <summary>
/// ���ļ��������ļ�ָ��
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
/// ��ָ���������������
/// </summary>
/// <param name="stream">�ļ�ָ��</param>
/// <param name="index">����</param>
/// <param name="buffer">Ҫд�������</param>
/// <param name="bytesToInsert">Ҫд����ֽ���</param>
/// <returns>���������ֵ��-1��ʾʧ��</returns>
int InsertTag(FILE *stream, int index, void *buffer, size_t bytesToInsert);

/// <summary>
/// ������β�������
/// </summary>
/// <param name="stream">�ļ�ָ��</param>
/// <param name="buffer">Ҫд�������</param>
/// <param name="bytesToInsert">Ҫд����ֽ���</param>
/// <returns>���������ֵ��-1��ʾʧ��</returns>
int AppendTag(FILE *stream, void *buffer, size_t bytesToInsert);

/// <summary>
/// ɾ��ָ������������
/// </summary>
/// <param name="stream">�ļ�ָ��</param>
/// <param name="index">Ҫɾ��������</param>
/// <returns>�ɹ���0��ʧ�ܣ�1</returns>
int DeleteTag(FILE *stream, int index);

