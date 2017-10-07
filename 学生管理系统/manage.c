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
	ClearScreen();
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

// 0�Ҳ�����1�ҵ�
int KMPSearch(char *szStr, char *szMatchStr)
{
	int iIndex = 0;
	int  i = 0;
	int j = 0;

	for (i = 0; i < (int)strnlen(szStr, MAX_STR_LEN) - (int)strnlen(szMatchStr, MAX_STR_LEN) + 1; i++)
	{
		iIndex = i;
		if (szStr[i] == szMatchStr[j])
		{
			do
			{
				i++;
				j++;
			} while (j != strnlen(szMatchStr, MAX_STR_LEN) && szStr[i] == szMatchStr[j]);
			if (j == strnlen(szMatchStr, MAX_STR_LEN))
			{
				return 1;
			}
			else
			{
				i = iIndex + 1;
				j = 0;
			}
		}
	}

	return 0;
}


void OpenStudentFile()
{
	char fileName[MAX_STR_LEN] = { 0 };
	int size = 0;
	char input = '\0';
	ClearScreen();
	PrintLine("1�����ļ�");
	PrintLine("2�������ļ�");
	PrintLine("0������");
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
	else if (input == '2')
	{
		PrintLine("�������ļ���:");
		GetInputString(fileName);
		PrintLine("�ļ���С��KB��:");
		size = GetInputInt();
		CreateStudentFile(fileName, size * 1024, NULL);
	}
	else if (input == '2')
	{
		return;
	}
	else
	{
		PrintLine("��������!");
	}
	GetInputChar();
}

void SetID(Student *student)
{
	char tmpStr[MAX_STR_LEN];
	PrintLine("ѧ��:");
	GetInputString(tmpStr);
	student->ID = (char *)calloc(strnlen(tmpStr, MAX_STR_LEN) + 1, 1);
	strcpy(student->ID, tmpStr);
}

void SetName(Student *student)
{
	char tmpStr[MAX_STR_LEN];
	PrintLine("����:");
	GetInputString(tmpStr);
	student->name = (char *)calloc(strnlen(tmpStr, MAX_STR_LEN) + 1, 1);
	strcpy(student->name, tmpStr);
}

void SetSexual(Student *student)
{
	char tmpStr[MAX_STR_LEN];
	PrintLine("�Ա�:");
	GetInputString(tmpStr);
	student->sexual = (char *)calloc(strnlen(tmpStr, MAX_STR_LEN) + 1, 1);
	strcpy(student->sexual, tmpStr);
}

void SetBirthday(Student *student)
{
	int tmpInt = 0;
	PrintLine("����:");
	PrintLine("�꣺");
	tmpInt = GetInputInt();
	student->Birthday.year = tmpInt;
	PrintLine("�£�");
	tmpInt = GetInputInt();
	student->Birthday.month = tmpInt;
	PrintLine("�գ�");
	tmpInt = GetInputInt();
	student->Birthday.day = tmpInt;
}

void SetCGrade(Student *student)
{
	char tmpStr[MAX_STR_LEN];
	PrintLine("C���Գɼ�:");
	GetInputString(tmpStr);
	student->grade_C = (char *)calloc(strnlen(tmpStr, MAX_STR_LEN) + 1, 1);
	strcpy(student->grade_C, tmpStr);
}
void AddStudentInfo()
{
	Student student = { 0 };

	ClearScreen();
	SetID(&student);
	SetName(&student);
	SetSexual(&student);
	SetBirthday(&student);
	SetCGrade(&student);

	AppendStudent(g_currentFileName, &student);

	GetInputChar();
}

void DeleteStudentInfo()
{
	int tmpInt = 0;
	ReturnType retValue = RET_SUCCESS;
	ClearScreen();
	PrintLine("Ҫɾ���ڼ���ѧ��:");
	tmpInt = GetInputInt();
	retValue = DeleteStudent(g_currentFileName, tmpInt);
	if (retValue == RET_SUCCESS)
	{
		PrintLine("ɾ���ɹ�");
	}
	else
	{
		PrintLine("������");
		PrintLine("404 Not Found");
	}
	GetInputChar();
}

void ModifyStudentInfo()
{
	ReturnType retVal = RET_SUCCESS;
	char tmpStr[MAX_STR_LEN];
	int tmpInt = 0;
	int index = 0;
	char tmpChar = '\0';
	Student student = { 0 };
	ClearScreen();
	PrintLine("Ҫ�޸ĵڼ���ѧ��:");
	index = GetInputInt();
	retVal = GetStudent(g_currentFileName, index, &student);
	if (retVal != RET_SUCCESS)
	{
		PrintLine("404");
		return;
	}
	PrintLine("�޸�ѧ�ţ� y/n");
	tmpChar = GetInputChar();
	if (tmpChar == 'Y' || tmpChar == 'y')
	{
		SetID(&student);
	}
	PrintLine("�޸������� y/n");
	tmpChar = GetInputChar();
	if (tmpChar == 'Y' || tmpChar == 'y')
	{
		SetName(&student);
	}
	PrintLine("�޸��Ա� y/n");
	tmpChar = GetInputChar();
	if (tmpChar == 'Y' || tmpChar == 'y')
	{
		SetSexual(&student);
	}
	PrintLine("�޸����գ� y/n");
	tmpChar = GetInputChar();
	if (tmpChar == 'Y' || tmpChar == 'y')
	{
		SetBirthday(&student);
	}
	PrintLine("�޸�C���Գɼ��� y/n");
	tmpChar = GetInputChar();
	if (tmpChar == 'Y' || tmpChar == 'y')
	{
		SetCGrade(&student);
	}
	retVal = ModifyStudent(g_currentFileName, index, &student);
	if (retVal == RET_SUCCESS)
	{
		PrintLine("�޸ĳɹ�");
	}
	else
	{
		PrintLine("������");
	}
	GetInputChar();

}

void QueryStudentInfo()
{
	char tmpChar = '\0';
	char tmpStr[MAX_STR_LEN] = "";
	int tmpInt = 0;
	int dayChoose = 0;
	int studentSize = GetStudentSize(g_currentFileName);
	Student student;
	ClearScreen();
	PrintLine("Ҫ��ѯ����Ŀ��");
	PrintLine("1: ѧ��");
	PrintLine("2: ����");
	PrintLine("3: �Ա�");
	PrintLine("4: ����");
	PrintLine("5: C���Գɼ�");
	tmpChar = GetInputChar();
	if (tmpChar == '4')
	{
		PrintLine("1: ��");
		PrintLine("2: ��");
		PrintLine("3: ��");
		dayChoose = atoi(GetInputChar());
		if (dayChoose > 3 || dayChoose <= 0)
		{
			PrintLine("������");
			return;
		}
		dayChoose -= 1;
		tmpInt = GetInputInt();
	}
	else
	{
		PrintLine("������Ҫ��ѯ�����ݣ�");
		GetInputString(tmpStr);
	}
	PrintStudentHelpInfo();
	for (int i = 0; i < studentSize; i++)
	{

		GetStudent(g_currentFileName, i, &student);
		switch (tmpChar)
		{
		case '1':
			if (KMPSearch(student.ID, tmpStr))
			{
				PrintStudent(student, i);
			}
			break;
		case '2':
			if (KMPSearch(student.name, tmpStr))
			{
				PrintStudent(student, i);
			}
			break;
		case '3':
			if (KMPSearch(student.sexual, tmpStr))
			{
				PrintStudent(student, i);
			}
			break;
		case '4':
			if (student.Birthday.data[dayChoose] == tmpInt)
			{
				PrintStudent(student, i);
			}
			break;
		case '5':
			if (KMPSearch(student.grade_C, tmpStr))
			{
				PrintStudent(student, i);
			}
			break;
		}
	}

	GetInputChar();
}

void GradeStatistic()
{
	int studentSize = GetStudentSize(g_currentFileName);
	double totalGrade = 0.0;
	double averageGrade = 0.0;
	double maxGrade = 0.0;
	double minGrade = 100.0;
	Student student;
	for (int i = 0; i < studentSize; i++)
	{
		double grade = 0.0;
		GetStudent(g_currentFileName, i, &student);
		grade = atof(student.grade_C);
		if (maxGrade < grade)
		{
			maxGrade = grade;
		}
		if (minGrade > grade)
		{
			minGrade = grade;
		}
		totalGrade += grade;
	}
	averageGrade = totalGrade / (double)studentSize;
	ShowGrade(totalGrade, averageGrade, maxGrade, minGrade);
	GetInputChar();
}

void ShowStorageInfo()
{
	int *sizeList = NULL;
	int *statusList = NULL;
	int *listSize = 0;
	GetStudentFileStatus(g_currentFileName, &sizeList, &statusList, &listSize);
	ShowStudentFileStatus(sizeList, statusList, listSize);
	GetInputChar();
}

void DefragmentFile()
{
	if (FileDefragment(g_currentFileName) == RET_SUCCESS)
	{
		PrintLine("��Ƭ�������");
	}
	else
	{
		PrintLine("����δ֪����");
	}
	GetInputChar();
}

void ShowAllInfo()
{
	int studentSize = GetStudentSize(g_currentFileName);
	Student student;
	PrintStudentHelpInfo();
	for (int i = 0; i < studentSize; i++)
	{
		GetStudent(g_currentFileName, i, &student);
		PrintStudent(student, i);
	}
	GetInputChar();
}

void Quit()
{
	exit(0);
}
