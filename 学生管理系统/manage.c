#include "manage.h"

#include "storage.h"
#include "userInterface.h"

static char g_currentFileName[MAX_STR_LEN];

int MainLoop()
{
	while (1)
	{
		char input = '\0';
		ShowMainMenu();
		input = GetInputChar();
		switch (input)
		{
		case '1':     //��/�����ļ�      
			OpenStudentFile();
			break;
		case '2':     //���ѧ����Ϣ      
			AddStudentInfo();
			break;
		case '3':     //ɾ��ѧ����Ϣ         
			DeleteStudentInfo();
			break;
		case '4':     //�޸�ѧ����Ϣ   
			ModifyStudentInfo();
			break;
		case '5':     //��ѯѧ����Ϣ           
			QueryStudentInfo();
			break;
		case '6':     //ͳ��ѧ����Ϣ         
			GradeStatistic();
			break;
		case '7':     //��ʾ�洢��Ϣ      
			ShowStorageInfo();
			break;
		case '8':     //��Ƭ����     
			DefragmentFile();
			break;
		case '9':     //��ʾ����ѧ����Ϣ
			ShowAllInfo();
			break;
		case '0':     //�˳�  
			Quit();
			break;
		}
	}
}

void ShowMainMenu()
{
	ClearScreem();
	PrintLine("                    ��ӭʹ��ѧ������ϵͳ                    ");
	PrintLine("----------------------------------------------------------");
	PrintLine("��ѡ����Ҫʹ�õķ���:                                       ");
	PrintLine("1�� ��/�����ļ�                                           ");
	PrintLine("2�� ���ѧ����Ϣ                                            ");
	PrintLine("3�� ɾ��ѧ����Ϣ                                            ");
	PrintLine("4�� �޸�ѧ����Ϣ                                            ");
	PrintLine("5�� ��ѯѧ����Ϣ                                            ");
	PrintLine("6�� ͳ��ѧ����Ϣ                                            ");
	PrintLine("7�� ��ʾ�洢��Ϣ                                            ");
	PrintLine("8�� ��Ƭ����                                               ");
	PrintLine("9�� ��ʾ����ѧ����Ϣ                                        ");
	PrintLine("0�� �˳�                                                   ");
	PrintLine("----------------------------------------------------------");
}



void OpenStudentFile()
{
	char fileName[MAX_STR_LEN] = { 0 };
	int size = 0;
	char input = '\0';
	PrintLine("1�����ļ�");
	PrintLine("2�������ļ�");
	input = GetInputChar();
	if (input == '1')
	{
		PrintLine("�������ļ���:");
		GetInputString(fileName);
		if (IsFileExist(fileName))
		{
			strcpy_s(g_currentFileName, MAX_STR_LEN, fileName);
		}
		else
		{
			PrintLine("�ļ�������");
		}
	}
	else if (input == '1')
	{
		PrintLine("�������ļ���:");
		GetInputString(fileName);
		PrintLine("�ļ���С��KB��:");
		size = GetInputInt();
		CreateStudentFile(fileName, size * 1024, NULL);
	}
	else
	{
		PrintLine("��������!");
	}
	GetInputChar();
}

void AddStudentInfo()
{
	
}

void DeleteStudentInfo()
{
}

void ModifyStudentInfo()
{
}

void QueryStudentInfo()
{
}

void GradeStatistic()
{
}

void ShowStorageInfo()
{
}

void DefragmentFile()
{
}

void ShowAllInfo()
{
}

void Quit()
{
}
