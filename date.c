
#include "date.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>


Date* createDate(int day, int month, int year)
{
	Date* date = (Date*)malloc(sizeof(Date));
	if (date == NULL)
		return NULL;
	date->day = day;
	date->month = month;
	date->year = year;
	return date;
}

void destroyDate(Date* date)
{
	if (date == NULL)
		return;
	free(date);
}

int getDay(const Date* date)
{
	if (date == NULL)
		return -1;
	return date->day;
}

int getMonth(const Date* date)
{
	if (date == NULL)
		return -1;

	return date->month;
}

int getYear(const Date* date)
{
	if (date == NULL)
		return -1;

	return date->year;
}

int equalDates(const Date* x, const Date* y)
{
	if (x == NULL || y == NULL)
		return -1;

	if (getDay(x) != getDay(y) || 
		getMonth(x) != getMonth(y) || 
		getYear(x) != getYear(y))
		return 0;
	
	return 1;
}

int isExpired(const Date* date)
{
	if (date == NULL)
		return -1;

	time_t t = time(NULL);
	struct tm time = *localtime(&t);
	//printf("%d/%d/%d\n", time.tm_mday, time.tm_mon + 1, time.tm_year + 1900);

	int currentDay = time.tm_mday, currentMonth = time.tm_mon + 1, currentYear = time.tm_year + 1900;
	int day = getDay(date), month = getMonth(date), year = getYear(date);

	if (year < currentYear)
		return 1;
	else if (year == currentYear)
	{
		if (month < currentMonth)
			return 1;
		else if (month == currentMonth)
			if (day < currentDay)
				return 1;
	}

	return 0;
}

Date* copyDate(Date* date)
{
	if (date == NULL)
		return NULL;

	Date* dateCopy = createDate(getDay(date), getMonth(date), getYear(date));

	return dateCopy;
}


//Tests


void testCreateDate()
{
	Date* testDate = createDate(1, 2, 3);

	assert(testDate != NULL);

	destroyDate(testDate);
}

void testDateGetters()
{
	Date* testDate = createDate(1, 2, 3);

	assert(getDay(testDate) == 1);
	assert(getMonth(testDate) == 2);
	assert(getYear(testDate) == 3);

	destroyDate(testDate);
}

void testEqualDates()
{
	Date* testDate1 = createDate(1, 2, 3);
	Date* testDate2 = createDate(2, 3, 4);
	Date* testDate3 = createDate(2, 3, 4);

	assert(equalDates(testDate1, testDate2) == 0);
	assert(equalDates(testDate1, testDate3) == 0);
	assert(equalDates(testDate2, testDate3) == 1);
	assert(equalDates(NULL, testDate3) == -1);

	destroyDate(testDate1);
	destroyDate(testDate2);
	destroyDate(testDate3);
}

void testIsExpired()
{
	time_t t = time(NULL);
	struct tm time = *localtime(&t);
	int currentDay = time.tm_mday, currentMonth = time.tm_mon + 1, currentYear = time.tm_year + 1900;

	Date* testDate1 = createDate(currentDay, currentMonth, currentYear);
	Date* testDate2 = createDate(currentDay-1, currentMonth, currentYear);
	Date* testDate3 = createDate(currentDay-1, currentMonth-1, currentYear);
	Date* testDate4 = createDate(currentDay, currentMonth, currentYear-1);

	assert(isExpired(testDate1) == 0);
	assert(isExpired(testDate2) == 1);
	assert(isExpired(testDate3) == 1);
	assert(isExpired(testDate4) == 1);

	destroyDate(testDate1);
	destroyDate(testDate2);
	destroyDate(testDate3);
	destroyDate(testDate4);
}

void testCopyDate()
{
	Date* testDate = createDate(1, 2, 3);

	assert(copyDate(NULL) == NULL);

	Date* copyOfDate = copyDate(testDate);
	assert(testDate != copyOfDate);

	assert(getDay(testDate) == getDay(copyOfDate));
	assert(getMonth(testDate) == getMonth(copyOfDate));
	assert(getYear(testDate) == getYear(copyOfDate));

	destroyDate(testDate);
	destroyDate(copyOfDate);
}

void testDate()
{
	testCreateDate();
	testDateGetters();
	testEqualDates();
	testIsExpired();
	testCopyDate();
}
