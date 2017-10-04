#pragma once

#define MAX_STR_LEN 32

typedef struct Student
{
	char *ID;
	char *name;
	char *telephone;
	char *grade_C;
	struct
	{
		int year;
		int month;
		int day;
	}Birthday;
}Student;

typedef enum ReturnType
{
	RET_SUCCESS = 0,
	RET_ERROR,
	RET_NOSPACE,
	RET_ILLEGAL_INDEX,
	RET_NO_HEAP_SPACE
}ReturnType;