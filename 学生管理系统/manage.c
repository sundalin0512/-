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
	ClearScreem();
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



void OpenStudentFile()
{
	char fileName[MAX_STR_LEN] = { 0 };
	int size = 0;
	char input = '\0';
	PrintLine("1：打开文件");
	PrintLine("2：创建文件");
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
	else if (input == '1')
	{
		PrintLine("请输入文件名:");
		GetInputString(fileName);
		PrintLine("文件大小（KB）:");
		size = GetInputInt();
		CreateStudentFile(fileName, size * 1024, NULL);
	}
	else
	{
		PrintLine("输入有误!");
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
