#include "stdafx.h"
#include "CppUnitTest.h"
extern "C" {
#include "storageIO.h"
#include "storage.h"
	extern size_t FileRead(FILE *stream, const size_t offsetFromFileStart, void *buffer, const size_t bytesToRead);
	extern size_t FileWrite(FILE *stream, const size_t offsetFromFileStart, const void *buffer, const size_t bytesToWrite);
	//extern ReturnType AddStudent(char *fileName, int index, Student *student);
}
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(StorageIOTest)
	{
	public:
		FILE *fp;
		StorageIOTest()
		{
			fp = CreateFile("test.bin", 1024 * 1024, NULL);
		}
		~StorageIOTest()
		{
			FileClose(fp);
		}
		TEST_METHOD(AppendData_Test1)
		{

			char *szHello = "00000000000000000";
			int index = 0;
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			Assert::AreEqual(index, 0);
		}

		TEST_METHOD(AppendData_Test2)
		{
			char *szHello = "00000000000000000";
			int index = 0;
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			Assert::AreEqual(index, 0);
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			Assert::AreEqual(index, 1);
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			Assert::AreEqual(index, 2);
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			Assert::AreEqual(index, 3);
		}

		TEST_METHOD(InsertData_Test1)
		{
			InsertData(fp, 0, "00000000", 9);
			InsertData(fp, 0, "11111111", 9);

			char szTest[] = "00000000""\0""11111111";
			char *buffer = new char[sizeof(szTest)];
			FileRead(fp, 0x60, buffer, sizeof(szTest));
			for (int i = 0; i < sizeof(szTest); i++)
			{
				Assert::AreEqual(szTest[i], buffer[i]);
			}
			FileInfo fileinfo;
			FileRead(fp, 0, &fileinfo, sizeof(fileinfo));
			Assert::AreEqual(size_t(0x60 + 18), fileinfo.offsetUsed);
			Assert::AreEqual(size_t(0x60 + 18 + 20), fileinfo.offsetUnused);
		}

		TEST_METHOD(DeleteData_Test1)
		{
			InsertData(fp, 0, "00000000", 9);
			InsertData(fp, 0, "11111111", 9);
			DeleteData(fp, 1);
			InsertData(fp, 0, "00000000", 9);

			char szTest[] = "00000000""\0""11111111";
			char *buffer = new char[sizeof(szTest)];
			FileRead(fp, 0x60, buffer, sizeof(szTest));
			for (int i = 0; i < sizeof(szTest); i++)
			{
				Assert::AreEqual(szTest[i], buffer[i]);
			}
			FileInfo fileinfo;
			FileRead(fp, 0, &fileinfo, sizeof(fileinfo));
			Assert::AreEqual(size_t(0x60 + 18), fileinfo.offsetUsed);
			Assert::AreEqual(size_t(0x60 + 18 + 20), fileinfo.offsetUnused);
		}

		TEST_METHOD(Defragment_Test1)
		{
			char *szHello = "00000000000000000";
			int index = 0;
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "11111111111111111";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "2222222222222222222222222222";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);


			szHello = "33333333333333333333333";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "444444444444";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "555555555555555555555555555555555";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "66666666666666666666";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "7";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "8888888888888888888888888888888888888888888";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);

			szHello = "999999999999999";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);

			DeleteData(fp, 9);

			DeleteData(fp, 7);
			DeleteData(fp, 5);
			DeleteData(fp, 3);
			DeleteData(fp, 3);
			DeleteData(fp, 0);
			DeleteData(fp, 0);
			DeleteData(fp, 0);
			DeleteData(fp, 0);
			DeleteData(fp, 0);

			Defragment(fp);
			int buffer[2];
			FileRead(fp, 0x60, buffer, 2 * sizeof(size_t));
			Assert::AreEqual(0, buffer[1]);
			Assert::AreEqual(0, buffer[0]);

		}

		TEST_METHOD(Defragment_Test2)
		{
			char *szHello = "00000000000000000";
			int index = 0;
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "11111111111111111";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "2222222222222222222222222222";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);


			szHello = "33333333333333333333333";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "444444444444";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "555555555555555555555555555555555";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "66666666666666666666";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "7";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "8888888888888888888888888888888888888888888";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);

			szHello = "999999999999999";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);

			DeleteData(fp, 9);

			DeleteData(fp, 7);
			DeleteData(fp, 5);
			DeleteData(fp, 3);
			DeleteData(fp, 3);

			Defragment(fp);
			char szTest[] = "00000000000000000\00011111111111111111\0002222222222222222222222222222\00066666666666666666666\0008888888888888888888888888888888888888888888";
			char *buffer = new char[sizeof(szTest)];
			FileRead(fp, 0x60, buffer, sizeof(szTest));
			for (int i = 0; i < sizeof(szTest); i++)
			{
				Assert::AreEqual(szTest[i], buffer[i]);
			}
			FileInfo fileinfo;
			FileRead(fp, 0, &fileinfo, sizeof(fileinfo));
			Assert::AreEqual(size_t(0x60 + 130), fileinfo.offsetUsed);
			Assert::AreEqual(size_t(0x60 + 130 + 44), fileinfo.offsetUnused);
		}

		TEST_METHOD(GetData_Test1)
		{
			char *szHello[10];
			szHello[0] = "00000000000000000";
			int index = 0;
			AppendData(fp, szHello[0], strlen(szHello[0]) + 1, &index);
			szHello[1] = "11111111111111111";
			AppendData(fp, szHello[1], strlen(szHello[1]) + 1, &index);
			szHello[2] = "2222222222222222222222222222";
			AppendData(fp, szHello[2], strlen(szHello[2]) + 1, &index);


			szHello[3] = "33333333333333333333333";
			AppendData(fp, szHello[3], strlen(szHello[3]) + 1, &index);
			szHello[4] = "444444444444";
			AppendData(fp, szHello[4], strlen(szHello[4]) + 1, &index);
			szHello[5] = "555555555555555555555555555555555";
			AppendData(fp, szHello[5], strlen(szHello[5]) + 1, &index);
			szHello[6] = "66666666666666666666";
			AppendData(fp, szHello[6], strlen(szHello[6]) + 1, &index);
			szHello[7] = "7";
			AppendData(fp, szHello[7], strlen(szHello[7]) + 1, &index);
			szHello[8] = "8888888888888888888888888888888888888888888";
			AppendData(fp, szHello[8], strlen(szHello[8]) + 1, &index);

			szHello[9] = "999999999999999";
			AppendData(fp, szHello[9], strlen(szHello[9]) + 1, &index);

			char *data = nullptr;
			size_t size;
			for (int j = 0; j < 10; j++)
			{
				GetData(fp, (void**)&data, &size, j);
				for (int i = 0; i < size; i++)
				{
					Assert::AreEqual(szHello[j][i], data[i]);
				}
				free(data);
			}
		}

		TEST_METHOD(GetData_Test2)
		{
			char *szHello[10];
			szHello[0] = "00000000000000000";
			int index = 0;
			AppendData(fp, szHello[0], strlen(szHello[0]) + 1, &index);
			szHello[1] = "11111111111111111";
			AppendData(fp, szHello[1], strlen(szHello[1]) + 1, &index);
			szHello[2] = "2222222222222222222222222222";
			AppendData(fp, szHello[2], strlen(szHello[2]) + 1, &index);


			szHello[3] = "33333333333333333333333";
			AppendData(fp, szHello[3], strlen(szHello[3]) + 1, &index);
			szHello[4] = "444444444444";
			AppendData(fp, szHello[4], strlen(szHello[4]) + 1, &index);
			szHello[5] = "555555555555555555555555555555555";
			AppendData(fp, szHello[5], strlen(szHello[5]) + 1, &index);
			szHello[6] = "66666666666666666666";
			AppendData(fp, szHello[6], strlen(szHello[6]) + 1, &index);
			szHello[7] = "7";
			AppendData(fp, szHello[7], strlen(szHello[7]) + 1, &index);
			szHello[8] = "8888888888888888888888888888888888888888888";
			AppendData(fp, szHello[8], strlen(szHello[8]) + 1, &index);

			szHello[9] = "999999999999999";
			AppendData(fp, szHello[9], strlen(szHello[9]) + 1, &index);

			DeleteData(fp, 9);

			DeleteData(fp, 7);
			DeleteData(fp, 5);
			DeleteData(fp, 3);
			DeleteData(fp, 3);

			char *data = nullptr;
			size_t size;
			int count = 0;
			for (int j = 0; j < 10; j++)
			{
				if (j == 3 || j == 4 || j == 5 || j == 7 || j == 9)
					continue;
				GetData(fp, (void**)&data, &size, count);
				count++;
				for (int i = 0; i < size; i++)
				{
					Assert::AreEqual(szHello[j][i], data[i]);
				}
				free(data);
			}
		}

		TEST_METHOD(GetData_Test3)
		{
			char *szHello[10];
			szHello[0] = "00000000000000000";
			int index = 0;
			AppendData(fp, szHello[0], strlen(szHello[0]) + 1, &index);
			szHello[1] = "11111111111111111";
			AppendData(fp, szHello[1], strlen(szHello[1]) + 1, &index);
			szHello[2] = "2222222222222222222222222222";
			AppendData(fp, szHello[2], strlen(szHello[2]) + 1, &index);


			szHello[3] = "33333333333333333333333";
			AppendData(fp, szHello[3], strlen(szHello[3]) + 1, &index);
			szHello[4] = "444444444444";
			AppendData(fp, szHello[4], strlen(szHello[4]) + 1, &index);
			szHello[5] = "555555555555555555555555555555555";
			AppendData(fp, szHello[5], strlen(szHello[5]) + 1, &index);
			szHello[6] = "66666666666666666666";
			AppendData(fp, szHello[6], strlen(szHello[6]) + 1, &index);
			szHello[7] = "7";
			AppendData(fp, szHello[7], strlen(szHello[7]) + 1, &index);
			szHello[8] = "8888888888888888888888888888888888888888888";
			AppendData(fp, szHello[8], strlen(szHello[8]) + 1, &index);

			szHello[9] = "999999999999999";
			AppendData(fp, szHello[9], strlen(szHello[9]) + 1, &index);

			DeleteData(fp, 9);

			DeleteData(fp, 7);
			DeleteData(fp, 5);
			DeleteData(fp, 3);
			DeleteData(fp, 3);
			Defragment(fp);
			char *data = nullptr;
			size_t size;
			int count = 0;
			for (int j = 0; j < 10; j++)
			{
				if (j == 3 || j == 4 || j == 5 || j == 7 || j == 9)
					continue;
				GetData(fp, (void**)&data, &size, count);
				count++;
				for (int i = 0; i < size; i++)
				{
					Assert::AreEqual(szHello[j][i], data[i]);
				}
				free(data);
			}
		}

		TEST_METHOD(GetDataNumber_Test1)
		{
			InsertData(fp, 0, "00000000", 9);
			Assert::AreEqual(1, (int)GetDataNumber(fp));
			InsertData(fp, 0, "00000000", 9);
			Assert::AreEqual(2, (int)GetDataNumber(fp));
			InsertData(fp, 0, "00000000", 9);
			Assert::AreEqual(3, (int)GetDataNumber(fp));
			InsertData(fp, 0, "00000000", 9);
			Assert::AreEqual(4, (int)GetDataNumber(fp));
			InsertData(fp, 0, "00000000", 9);
			Assert::AreEqual(5, (int)GetDataNumber(fp));
			InsertData(fp, 0, "00000000", 9);
			Assert::AreEqual(6, (int)GetDataNumber(fp));
			InsertData(fp, 0, "00000000", 9);
			Assert::AreEqual(7, (int)GetDataNumber(fp));
			DeleteData(fp, 1);
			Assert::AreEqual(6, (int)GetDataNumber(fp));
			DeleteData(fp, 1);
			Assert::AreEqual(5, (int)GetDataNumber(fp));
			DeleteData(fp, 1);
			Assert::AreEqual(4, (int)GetDataNumber(fp));
			DeleteData(fp, 1);
			Assert::AreEqual(3, (int)GetDataNumber(fp));
			DeleteData(fp, 1);
			Assert::AreEqual(2, (int)GetDataNumber(fp));
			DeleteData(fp, 1);
			Assert::AreEqual(1, (int)GetDataNumber(fp));
			DeleteData(fp, 0);
			Assert::AreEqual(0, (int)GetDataNumber(fp));
		}

		TEST_METHOD(GetFileStatus_Test1)
		{
			char *szHello = "00000000000000000";
			int index = 0;
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			DeleteData(fp, 1);
			int size[3] = { 18,18,0 };
			int used[3] = { 1,0,1 };
			int *sizeList;
			int *statusList;
			int ListSize;
			GetFileStatus(fp, &sizeList, &statusList, &ListSize);
			Assert::AreEqual(3, ListSize);
			for (int i = 0; i < 3; i++)
			{
				Assert::AreEqual(size[i], sizeList[i]);
				Assert::AreEqual(used[i], statusList[i]);
			}
		}

		TEST_METHOD(GetFileStatus_Test2)
		{
			char *szHello = "00000000000000000";
			int index = 0;
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			int size[3] = { 18 };
			int used[3] = { 1 };
			int *sizeList;
			int *statusList;
			int ListSize;
			GetFileStatus(fp, &sizeList, &statusList, &ListSize);
			Assert::AreEqual(1, ListSize);
			for (int i = 0; i < 1; i++)
			{
				Assert::AreEqual(size[i], sizeList[i]);
				Assert::AreEqual(used[i], statusList[i]);
			}
		}
		TEST_METHOD(GetFileStatus_Test3)
		{
			char *szHello = "00000000000000000";
			int index = 0;
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "11111111111111111";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "2222222222222222222222222222";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);


			szHello = "33333333333333333333333";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "444444444444";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "555555555555555555555555555555555";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "66666666666666666666";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "7";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "8888888888888888888888888888888888888888888";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);

			szHello = "999999999999999";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);

			DeleteData(fp, 9);

			DeleteData(fp, 7);
			DeleteData(fp, 5);
			DeleteData(fp, 3);
			DeleteData(fp, 3);

			//Defragment(fp);
			int size[7] = { 65, 71, 21, 2, 44, 16, 0 };
			int used[7] = { 1 ,0, 1, 0, 1, 0, 1 };
			int *sizeList;
			int *statusList;
			int ListSize;
			GetFileStatus(fp, &sizeList, &statusList, &ListSize);
			Assert::AreEqual(7, ListSize);
			for (int i = 0; i < 7; i++)
			{
				Assert::AreEqual(size[i], sizeList[i]);
				Assert::AreEqual(used[i], statusList[i]);
			}
		}

		TEST_METHOD(GetFileStatus_Test4)
		{
			char *szHello = "00000000000000000";
			int index = 0;
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "11111111111111111";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "2222222222222222222222222222";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);


			szHello = "33333333333333333333333";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "444444444444";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "555555555555555555555555555555555";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "66666666666666666666";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "7";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "8888888888888888888888888888888888888888888";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);

			szHello = "999999999999999";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);

			DeleteData(fp, 9);

			DeleteData(fp, 7);
			DeleteData(fp, 5);
			DeleteData(fp, 3);
			DeleteData(fp, 3);
			DeleteData(fp, 0);
			DeleteData(fp, 0);
			DeleteData(fp, 0);
			DeleteData(fp, 0);
			DeleteData(fp, 0);

			//Defragment(fp);
			int size[3] = { 0, 65 + 71 + 21 + 2 + 44 + 16,0 };
			int used[3] = { 1 ,0, 1 };
			int *sizeList;
			int *statusList;
			int ListSize;
			GetFileStatus(fp, &sizeList, &statusList, &ListSize);
			Assert::AreEqual(3, ListSize);
			for (int i = 0; i < 3; i++)
			{
				Assert::AreEqual(size[i], sizeList[i]);
				Assert::AreEqual(used[i], statusList[i]);
			}
		}

		TEST_METHOD(GetFileStatus_Test5)
		{
			char *szHello = "00000000000000000";
			int index = 0;
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "11111111111111111";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "2222222222222222222222222222";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);


			szHello = "33333333333333333333333";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "444444444444";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "555555555555555555555555555555555";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "66666666666666666666";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "7";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);
			szHello = "8888888888888888888888888888888888888888888";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);

			szHello = "999999999999999";
			AppendData(fp, szHello, strlen(szHello) + 1, &index);

			DeleteData(fp, 9);

			DeleteData(fp, 7);
			DeleteData(fp, 5);
			DeleteData(fp, 3);
			DeleteData(fp, 3);
			DeleteData(fp, 0);
			DeleteData(fp, 0);
			DeleteData(fp, 0);
			DeleteData(fp, 0);
			DeleteData(fp, 0);

			Defragment(fp);
			int size[1] = { 0};
			int used[1] = { 0};
			int *sizeList;
			int *statusList;
			int ListSize;
			GetFileStatus(fp, &sizeList, &statusList, &ListSize);
			Assert::AreEqual(1, ListSize);
			for (int i = 0; i < 1; i++)
			{
				Assert::AreEqual(size[i], sizeList[i]);
				Assert::AreEqual(used[i], statusList[i]);
			}
		}

	};

	TEST_CLASS(StorageTest)
	{
	public:
		Student student[10];
		char *fileName;
		StorageTest()
		{
			student[0].ID = "2013084117";
			student[0].name = "sundalin";
			student[0].telephone = "18877776666";
			student[0].grade_C = "90";
			student[0].Birthday = { 1993,5,12 };

			student[1].ID = "2013081234";
			student[1].name = "sssdalin";
			student[1].telephone = "18822223333";
			student[1].grade_C = "60";
			student[1].Birthday = { 1995,2,18 };
		}
		~StorageTest()
		{

		}
		TEST_METHOD(AddStudent_Test1)
		{
			fileName = "AddStudent_Test1.bin";
			FILE *fp = CreateFile(fileName, 1000, nullptr);
			FileClose(fp);
			AddStudent(fileName, 0, &student[0]);

			fp = FileOpen(fileName);
			char *buffer;
			size_t size;
			GetData(fp, (void**)&buffer, &size, 0);

			Assert::AreEqual(11, ((int*)buffer)[0]);
			Assert::AreEqual(0x43, (int)size);
		}

		TEST_METHOD(AddStudent_Test2)
		{
			fileName = "AddStudent_Test2.bin";
			FILE *fp = CreateFile(fileName, 1000, nullptr);
			FileClose(fp);
			AddStudent(fileName, 0, &student[0]);
			AddStudent(fileName, 1, &student[0]);

			fp = FileOpen(fileName);
			char *buffer;
			size_t size;
			GetData(fp, (void**)&buffer, &size, 0);

			Assert::AreEqual(11, ((int*)buffer)[0]);
			Assert::AreEqual(0x43, (int)size);

			free(buffer);
			GetData(fp, (void**)&buffer, &size, 1);

			Assert::AreEqual(11, ((int*)buffer)[0]);
			Assert::AreEqual(0x43, (int)size);
			free(buffer);
		}

		TEST_METHOD(GetStudent_Test1)
		{
			fileName = "GetStudent_Test1.bin";
			FILE *fp = CreateFile(fileName, 1000, nullptr);
			FileClose(fp);
			AddStudent(fileName, 0, &student[1]);
			AddStudent(fileName, 0, &student[0]);

			Student tmpStudent;
			GetStudent(fileName, 0, &tmpStudent);
			for (int i = 0; i < strlen(student[0].ID) + 1; i++)
			{
				Assert::AreEqual(student[0].ID[i], tmpStudent.ID[i]);
			}
			for (int i = 0; i < strlen(student[0].name) + 1; i++)
			{
				Assert::AreEqual(student[0].name[i], tmpStudent.name[i]);
			}
			for (int i = 0; i < strlen(student[0].telephone) + 1; i++)
			{
				Assert::AreEqual(student[0].telephone[i], tmpStudent.telephone[i]);
			}
			for (int i = 0; i < strlen(student[0].grade_C) + 1; i++)
			{
				Assert::AreEqual(student[0].grade_C[i], tmpStudent.grade_C[i]);
			}
			Assert::AreEqual(student[0].Birthday.year, tmpStudent.Birthday.year);
			Assert::AreEqual(student[0].Birthday.month, tmpStudent.Birthday.month);
			Assert::AreEqual(student[0].Birthday.day, tmpStudent.Birthday.day);
		}

		TEST_METHOD(GetStudent_Test2)
		{
			fileName = "GetStudent_Test2.bin";
			FILE *fp = CreateFile(fileName, 1000, nullptr);
			FileClose(fp);
			AddStudent(fileName, 0, &student[0]);
			AddStudent(fileName, 0, &student[0]);

			Student tmpStudent;
			GetStudent(fileName, 1, &tmpStudent);
			for (int i = 0; i < strlen(student[0].ID) + 1; i++)
			{
				Assert::AreEqual(student[0].ID[i], tmpStudent.ID[i]);
			}
			for (int i = 0; i < strlen(student[0].name) + 1; i++)
			{
				Assert::AreEqual(student[0].name[i], tmpStudent.name[i]);
			}
			for (int i = 0; i < strlen(student[0].telephone) + 1; i++)
			{
				Assert::AreEqual(student[0].telephone[i], tmpStudent.telephone[i]);
			}
			for (int i = 0; i < strlen(student[0].grade_C) + 1; i++)
			{
				Assert::AreEqual(student[0].grade_C[i], tmpStudent.grade_C[i]);
			}
			Assert::AreEqual(student[0].Birthday.year, tmpStudent.Birthday.year);
			Assert::AreEqual(student[0].Birthday.month, tmpStudent.Birthday.month);
			Assert::AreEqual(student[0].Birthday.day, tmpStudent.Birthday.day);
		}

		TEST_METHOD(GetStudent_Test3)
		{
			fileName = "GetStudent_Test3.bin";
			FILE *fp = CreateFile(fileName, 1000, nullptr);
			FileClose(fp);
			AddStudent(fileName, 0, &student[0]);

			Assert::AreEqual((int)RET_ILLEGAL_INDEX, (int)AddStudent(fileName, 10, &student[0]));
		}

		TEST_METHOD(DeleteStudent_Test1)
		{
			fileName = "DeleteStudent_Test1.bin";
			FILE *fp = CreateFile(fileName, 1000, nullptr);
			FileClose(fp);
			AddStudent(fileName, 0, &student[1]);
			AddStudent(fileName, 1, &student[0]);
			DeleteStudent(fileName, 0);
			Student tmpStudent;
			GetStudent(fileName, 0, &tmpStudent);
			for (int i = 0; i < strlen(student[0].ID) + 1; i++)
			{
				Assert::AreEqual(student[0].ID[i], tmpStudent.ID[i]);
			}
			for (int i = 0; i < strlen(student[0].name) + 1; i++)
			{
				Assert::AreEqual(student[0].name[i], tmpStudent.name[i]);
			}
			for (int i = 0; i < strlen(student[0].telephone) + 1; i++)
			{
				Assert::AreEqual(student[0].telephone[i], tmpStudent.telephone[i]);
			}
			for (int i = 0; i < strlen(student[0].grade_C) + 1; i++)
			{
				Assert::AreEqual(student[0].grade_C[i], tmpStudent.grade_C[i]);
			}
		}

		TEST_METHOD(ModifyStudent_Test1)
		{
			fileName = "ModifyStudent_Test1.bin";
			FILE *fp = CreateFile(fileName, 1000, nullptr);
			FileClose(fp);
			AddStudent(fileName, 0, &student[1]);
			AddStudent(fileName, 1, &student[1]);
			AddStudent(fileName, 1, &student[1]);
			ModifyStudent(fileName, 2, &student[0]);
			Student tmpStudent;
			GetStudent(fileName, 2, &tmpStudent);
			for (int i = 0; i < strlen(student[0].ID) + 1; i++)
			{
				Assert::AreEqual(student[0].ID[i], tmpStudent.ID[i]);
			}
			for (int i = 0; i < strlen(student[0].name) + 1; i++)
			{
				Assert::AreEqual(student[0].name[i], tmpStudent.name[i]);
			}
			for (int i = 0; i < strlen(student[0].telephone) + 1; i++)
			{
				Assert::AreEqual(student[0].telephone[i], tmpStudent.telephone[i]);
			}
			for (int i = 0; i < strlen(student[0].grade_C) + 1; i++)
			{
				Assert::AreEqual(student[0].grade_C[i], tmpStudent.grade_C[i]);
			}
		}

		TEST_METHOD(AppendStudent_Test1)
		{
			fileName = "AppendStudent_Test1.bin";
			FILE *fp = CreateFile(fileName, 1000, nullptr);
			FileClose(fp);
			AppendStudent(fileName, &student[1]);
			AppendStudent(fileName, &student[1]);
			AppendStudent(fileName, &student[0]);
			AppendStudent(fileName, &student[1]);
			AppendStudent(fileName, &student[0]);
			AppendStudent(fileName, &student[0]);
			AppendStudent(fileName, &student[1]);
			AppendStudent(fileName, &student[0]);
			DeleteStudent(fileName, 0);
			DeleteStudent(fileName, 0);
			DeleteStudent(fileName, 0);
			DeleteStudent(fileName, 0);
			DeleteStudent(fileName, 0);
			DeleteStudent(fileName, 0);


			Student tmpStudent;
			GetStudent(fileName, 1, &tmpStudent);
			for (int i = 0; i < strlen(student[0].ID) + 1; i++)
			{
				Assert::AreEqual(student[0].ID[i], tmpStudent.ID[i]);
			}
			for (int i = 0; i < strlen(student[0].name) + 1; i++)
			{
				Assert::AreEqual(student[0].name[i], tmpStudent.name[i]);
			}
			for (int i = 0; i < strlen(student[0].telephone) + 1; i++)
			{
				Assert::AreEqual(student[0].telephone[i], tmpStudent.telephone[i]);
			}
			for (int i = 0; i < strlen(student[0].grade_C) + 1; i++)
			{
				Assert::AreEqual(student[0].grade_C[i], tmpStudent.grade_C[i]);
			}
		}

		TEST_METHOD(GetStudentSize_Test1)
		{
			fileName = "GetStudentSize_Test1.bin";
			FILE *fp = CreateFile(fileName, 1000, nullptr);
			FileClose(fp);
			AppendStudent(fileName, &student[1]);
			Assert::AreEqual(1, (int)GetStudentSize("GetStudentSize_Test1.bin"));
			AppendStudent(fileName, &student[1]);
			Assert::AreEqual(2, (int)GetStudentSize("GetStudentSize_Test1.bin"));
			AppendStudent(fileName, &student[0]);
			Assert::AreEqual(3, (int)GetStudentSize("GetStudentSize_Test1.bin"));
			AppendStudent(fileName, &student[1]);
			Assert::AreEqual(4, (int)GetStudentSize("GetStudentSize_Test1.bin"));
			AppendStudent(fileName, &student[0]);
			Assert::AreEqual(5, (int)GetStudentSize("GetStudentSize_Test1.bin"));
			AppendStudent(fileName, &student[0]);
			Assert::AreEqual(6, (int)GetStudentSize("GetStudentSize_Test1.bin"));
			AppendStudent(fileName, &student[1]);
			Assert::AreEqual(7, (int)GetStudentSize("GetStudentSize_Test1.bin"));
			AppendStudent(fileName, &student[0]);
			Assert::AreEqual(8, (int)GetStudentSize("GetStudentSize_Test1.bin"));

			DeleteStudent(fileName, 0);
			Assert::AreEqual(7, (int)GetStudentSize("GetStudentSize_Test1.bin"));
			DeleteStudent(fileName, 0);
			Assert::AreEqual(6, (int)GetStudentSize("GetStudentSize_Test1.bin"));
			DeleteStudent(fileName, 0);
			Assert::AreEqual(5, (int)GetStudentSize("GetStudentSize_Test1.bin"));
			DeleteStudent(fileName, 0);
			Assert::AreEqual(4, (int)GetStudentSize("GetStudentSize_Test1.bin"));
			DeleteStudent(fileName, 0);
			Assert::AreEqual(3, (int)GetStudentSize("GetStudentSize_Test1.bin"));
			DeleteStudent(fileName, 0);
			Assert::AreEqual(2, (int)GetStudentSize("GetStudentSize_Test1.bin"));
			DeleteStudent(fileName, 0);
			Assert::AreEqual(1, (int)GetStudentSize("GetStudentSize_Test1.bin"));
			DeleteStudent(fileName, 0);
			Assert::AreEqual(0, (int)GetStudentSize("GetStudentSize_Test1.bin"));
		}
	};
}