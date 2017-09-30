#pragma once

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
	union Birthday
	{
		struct
		{
			int year;
			int month;
			int day;
		};
		int data[3];
	};
	int modifyFlag;
}Student;