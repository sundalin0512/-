#pragma once

typedef struct Student
{
	char ID[32];
	char name[32];
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
	char telephone[32];
	char grade_C[32];
}Student;