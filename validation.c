
#include "validation.h"

#include <assert.h>


int validateDate(int day, int month, int year)
{
	int days[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if (year < 1900)
		return 0;

	if (month > 12 || month < 1)
		return 0;
	int limit = days[month];
	if (month == 2 && year % 4 == 0)
		limit++;
	
	if (day > limit || day < 1)
		return 0;
	
	return 1;
}

void testValidation()
{
	assert(validateDate(0, 0, 0) == 0);
	assert(validateDate(0, 1, 1900) == 0);
	assert(validateDate(12, 12, 1900) == 1);
	assert(validateDate(29, 2, 2000) == 1);
	assert(validateDate(1, 1, 1900) == 1);
	assert(validateDate(31, 1, 1900) == 1);
}
