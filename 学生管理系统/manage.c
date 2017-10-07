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
		case '1':     //打开/创建文件      
			OpenStudentFile();
			break;
		case '2':     //添加学生信息      
			AddStudentInfo();
			break;
		case '3':     //删除学生信息         
			DeleteStudentInfo();
			break;
		case '4':     //修改学生信息   
			ModifyStudentInfo();
			break;
		case '5':     //查询学生信息           
			QueryStudentInfo();
			break;
		case '6':     //统计学生信息         
			GradeStatistic();
			break;
		case '7':     //显示存储信息      
			ShowStorageInfo();
			break;
		case '8':     //碎片整理     
			DefragmentFile();
			break;
		case '9':     //显示所有学生信息
			ShowAllInfo();
			break;
		case '0':     //退出  
			Quit();
			break;
		}
	}
}

void ShowMainMenu()
{
	ClearScreen();
	PrintLine("                    欢迎使用学生管理系统                    ");
	PrintLine("----------------------------------------------------------");
	PrintLine("请选择需要使用的服务:                                       ");
	PrintLine("1： 打开/创建文件                                           ");
	PrintLine("2： 添加学生信息                                            ");
	PrintLine("3： 删除学生信息                                            ");
	PrintLine("4： 修改学生信息                                            ");
	PrintLine("5： 查询学生信息                                            ");
	PrintLine("6： 统计学生信息                                            ");
	PrintLine("7： 显示存储信息                                            ");
	PrintLine("8： 碎片整理                                               ");
	PrintLine("9： 显示所有学生信息                                        ");
	PrintLine("0： 退出                                                   ");
	PrintLine("----------------------------------------------------------");
}

// 0找不到，1找到
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
	PrintLine("1：打开文件");
	PrintLine("2：创建文件");
	PrintLine("0：返回");
	input = GetInputChar();
	if (input == '1')
	{
		PrintLine("请输入文件名:");
		GetInputString(fileName);
		if (IsFileExist(fileName))
		{
			strcpy_s(g_currentFileName, MAX_STR_LEN, fileName);
		}
		else
		{
			PrintLine("文件不存在");
		}
	}
	else if (input == '2')
	{
		PrintLine("请输入文件名:");
		GetInputString(fileName);
		PrintLine("文件大小（KB）:");
		size = GetInputInt();
		CreateStudentFile(fileName, size * 1024, NULL);
	}
	else if (input == '2')
	{
		return;
	}
	else
	{
		PrintLine("输入有误!");
	}
	GetInputChar();
}

void SetID(Student *student)
{
	char tmpStr[MAX_STR_LEN];
	PrintLine("学号:");
	GetInputString(tmpStr);
	student->ID = (char *)calloc(strnlen(tmpStr, MAX_STR_LEN) + 1, 1);
	strcpy(student->ID, tmpStr);
}

void SetName(Student *student)
{
	char tmpStr[MAX_STR_LEN];
	PrintLine("姓名:");
	GetInputString(tmpStr);
	student->name = (char *)calloc(strnlen(tmpStr, MAX_STR_LEN) + 1, 1);
	strcpy(student->name, tmpStr);
}

void SetSexual(Student *student)
{
	char tmpStr[MAX_STR_LEN];
	PrintLine("性别:");
	GetInputString(tmpStr);
	student->sexual = (char *)calloc(strnlen(tmpStr, MAX_STR_LEN) + 1, 1);
	strcpy(student->sexual, tmpStr);
}

void SetBirthday(Student *student)
{
	int tmpInt = 0;
	PrintLine("生日:");
	PrintLine("年：");
	tmpInt = GetInputInt();
	student->Birthday.year = tmpInt;
	PrintLine("月：");
	tmpInt = GetInputInt();
	student->Birthday.month = tmpInt;
	PrintLine("日：");
	tmpInt = GetInputInt();
	student->Birthday.day = tmpInt;
}

void SetCGrade(Student *student)
{
	char tmpStr[MAX_STR_LEN];
	PrintLine("C语言成绩:");
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
	PrintLine("要删除第几个学生:");
	tmpInt = GetInputInt();
	retValue = DeleteStudent(g_currentFileName, tmpInt);
	if (retValue == RET_SUCCESS)
	{
		PrintLine("删除成功");
	}
	else
	{
		PrintLine("出错啦");
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
	PrintLine("要修改第几个学生:");
	index = GetInputInt();
	retVal = GetStudent(g_currentFileName, index, &student);
	if (retVal != RET_SUCCESS)
	{
		PrintLine("404");
		return;
	}
	PrintLine("修改学号？ y/n");
	tmpChar = GetInputChar();
	if (tmpChar == 'Y' || tmpChar == 'y')
	{
		SetID(&student);
	}
	PrintLine("修改姓名？ y/n");
	tmpChar = GetInputChar();
	if (tmpChar == 'Y' || tmpChar == 'y')
	{
		SetName(&student);
	}
	PrintLine("修改性别？ y/n");
	tmpChar = GetInputChar();
	if (tmpChar == 'Y' || tmpChar == 'y')
	{
		SetSexual(&student);
	}
	PrintLine("修改生日？ y/n");
	tmpChar = GetInputChar();
	if (tmpChar == 'Y' || tmpChar == 'y')
	{
		SetBirthday(&student);
	}
	PrintLine("修改C语言成绩？ y/n");
	tmpChar = GetInputChar();
	if (tmpChar == 'Y' || tmpChar == 'y')
	{
		SetCGrade(&student);
	}
	retVal = ModifyStudent(g_currentFileName, index, &student);
	if (retVal == RET_SUCCESS)
	{
		PrintLine("修改成功");
	}
	else
	{
		PrintLine("出错啦");
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
	PrintLine("要查询的项目：");
	PrintLine("1: 学号");
	PrintLine("2: 姓名");
	PrintLine("3: 性别");
	PrintLine("4: 生日");
	PrintLine("5: C语言成绩");
	tmpChar = GetInputChar();
	if (tmpChar == '4')
	{
		PrintLine("1: 年");
		PrintLine("2: 月");
		PrintLine("3: 日");
		dayChoose = atoi(GetInputChar());
		if (dayChoose > 3 || dayChoose <= 0)
		{
			PrintLine("出错啦");
			return;
		}
		dayChoose -= 1;
		tmpInt = GetInputInt();
	}
	else
	{
		PrintLine("请输入要查询的内容：");
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
		PrintLine("碎片整理完成");
	}
	else
	{
		PrintLine("发生未知错误");
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
