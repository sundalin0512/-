#include "stdafx.h"
#include "CppUnitTest.h"
extern "C" {
#include "../学生管理系统/storageIO.h"
	extern size_t FileRead(FILE *stream, const size_t offsetFromFileStart, void *buffer, const size_t bytesToRead);
	extern size_t FileWrite(FILE *stream, const size_t offsetFromFileStart, const void *buffer, const size_t bytesToWrite);
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
	};

}