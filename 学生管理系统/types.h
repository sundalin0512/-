#pragma once

#define MAX_STR_LEN 32

enum ModifyFlag
{
	ModifyFlag_Id = 0x01,
	ModifyFlag_name = 0x20,
	ModifyFlag_telephone = 0x40,
	ModifyFlag_gradeC = 0x80,
	ModifyFlag_birthday = 0x10
};

typedef struct Student
{
	char *ID;
	char *name;
	char *telephone;
	char *grade_C;
	//union Birthday
	//{
		struct
		{
			int year;
			int month;
			int day;
		}Birthday;
	//	int data[3];
	//};
	//int modifyFlag;
}Student;

typedef enum ReturnType
{
	RET_SUCCESS = 0,
	RET_ERROR,
	RET_NOSPACE,
	RET_ILLEGAL_INDEX,
	RET_NO_HEAP_SPACE
}ReturnType;