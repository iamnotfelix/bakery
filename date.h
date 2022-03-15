#pragma once

#define EXPIRATION_DAY 1
#define EXPIRATION_MONTH 1
#define EXPIRATION_YEAR 2022

typedef struct Date
{
	int day, month, year;
} Date;

Date* createDate(int day, int month, int year);
void destroyDate(Date* date);

int getDay(const Date* date);
int getMonth(const Date* date);
int getYear(const Date* date);

int equalDates(const Date* x, const Date* y);
int isExpired(const Date* date);
Date* copyDate(Date* date);

//Tests
void testDate();
