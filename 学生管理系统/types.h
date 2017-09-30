#pragma once

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
}Student;