
#include "services.h"
#include "ui.h"
#include "validation.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

UI* createUI(MaterialServices* materialServices)
{
	UI* ui = (UI*)malloc(sizeof(UI));

	if (ui == NULL)
		return NULL;

	ui->materialServices = materialServices;

	return ui;
}

void destroyUI(UI* ui)
{
	if (ui == NULL)
		return;

	destroyMaterialServices(ui->materialServices);
	free(ui);
}

void printMenu()
{
	printf("Menu:\n");
	printf("add\tAdd a material.\n");
	printf("delete\tDelete a material.\n");
	printf("update\tUpdate a material.\n");
	printf("list\tList all available materials.\n\n");
	printf("expired\tGet all expired materials.\n");
	printf("short\tGet materials that are short on quantity.\n\n");
	printf("sort\tPrint materials sorted by name.\n\n");
	printf("undo\tUndo an operation.\n");
	printf("redo\tRedo an operation.\n");
	printf("help\tShow this menu.\n");
	printf("exit\tExit the application.\n");
	printf("\nEnter a command:\n");
}

void printExpiredMenu()
{
	printf("Choose one of the option:\n");
	printf("1\tGet expired materials containing a given string in their name.\n");
	printf("2\tGet expired materials having a quantity less than a given one.\n");
}

void printShortMenu()
{
	printf("Choose one of the option:\n");
	printf("1\tSorted in ascending order by the quantity.\n");
	printf("2\tSorted in descending order by the quantity.\n");
}

void printMaterials(DynamicArray* dArray)
{
	if (len(dArray) == 0)
	{
		printf("No materials to be displayed!\n");
		return;
	}

	printf("%-3s %20s %20s %20s %30s\n", "NR", "NAME", "SUPPLIER", "QUANTITY", "EXPIRATION_DATE");
	for (int i = 0; i < len(dArray); i++)
	{
		Material* m = getElement(dArray, i);
		const Date* d = getDate(m);
		printf("%-3d %20s %20s %*.4lf %20d/%d/%d\n", i + 1, getName(m), getSupplier(m), 20, getQuantity(m), getDay(d), getMonth(d), getYear(d));
	}
}

int getCommand(char* command)
{
	printf(">>>");
	int status = scanf("%63s", command);
	int c;  while ((c = getchar()) != '\n' && c != EOF) {}
	if (status == 0)
		return -1;
	return 1;
}

int getNameInput(char *name)
{
	int x = 0;
	while (x == 0)
	{
		printf("Enter name: ");
		x = scanf("%63[^\n]s", name);
		int c;  while ((c = getchar()) != '\n' && c != EOF) {}
		if (x == 0)
			printf("Enter a valid name!\n");
	}
	return 1;
}

int getSupplierInput(char* supplier)
{
	int x = 0;
	while (x == 0)
	{
		printf("Enter supplier: ");
		x = scanf("%63[^\n]s", supplier);
		int c;  while ((c = getchar()) != '\n' && c != EOF) {}
		if (x == 0)
			printf("Enter a valid supplier!\n");
	}
	return 1;
}

int getQuantityInput(double* quantity)
{
	int x = 0;
	while (x == 0)
	{
		char q[MAX_STRING_SIZE] = { 0 };

		printf("Enter quantity: ");
		x = scanf("%63s", &q);
		int c;  while ((c = getchar()) != '\n' && c != EOF) {}

		*quantity = atof(q);

		if (x == 0 || *quantity == 0)
		{
			x = 0;
			printf("Enter a valid quantity!\n");
		}
	}
	return 1;
}

int getDayInput(int* day)
{
	int x = 0;
	while (x == 0)
	{
		char d[3] = { 0 };

		printf("Enter expiration day: ");
		x = scanf("%2s", &d);
		int c;  while ((c = getchar()) != '\n' && c != EOF) {}

		*day = atoi(d);
		if (x == 0 || *day == 0)
		{
			x = 0;
			printf("Enter a valid day!\n");
		}
	}
	return 1;
}

int getMonthInput(int* month)
{
	int x = 0;
	while (x == 0)
	{
		char m[3] = { 0 };

		printf("Enter expiration month: ");
		x = scanf("%2s", &m);
		int c;  while ((c = getchar()) != '\n' && c != EOF) {}

		*month = atoi(m);
		if (x == 0 || *month == 0)
		{
			x = 0;
			printf("Enter a valid month!\n");
		}
	}
	return 1;
}

int getYearInput(int* year)
{
	int x = 0;
	while (x == 0)
	{
		char y[5] = { 0 };

		printf("Enter expiration year: ");
		x = scanf("%4s", &y);
		int c;  while ((c = getchar()) != '\n' && c != EOF) {}

		*year = atoi(y);
		if (x == 0 || *year == 0)
		{
			x = 0;
			printf("Enter a valid year!\n");
		}
	}
	return 1;
}

int getDateInput(int* day, int* month, int* year)
{
	while (1)
	{
		getDayInput(day);
		getMonthInput(month);
		getYearInput(year);
		if (validateDate(*day, *month, *year))
			break;
		printf("Enter a valid date!\n");
	}
	return 1;
}

int getString(char* s)
{
	int status = 0;

	while (status == 0)
	{
		printf("Enter a string for the filter (or 'none', no filter): ");
		
		status = scanf("%63[^\n]s", s);
		int c;  while ((c = getchar()) != '\n' && c != EOF) {}

		if (status == 0)
			printf("Enter a valid string!\n");
	}
	return 1;
}


int addHandler(UI* ui)
{
	char name[MAX_STRING_SIZE] = { 0 };
	char supplier[MAX_STRING_SIZE] = { 0 };
	double quantity = 0;
	int day = 0, month = 0, year = 0;

	getNameInput(name);
	getSupplierInput(supplier);
	getQuantityInput(&quantity);
	getDateInput(&day, &month, &year);

	return add(ui->materialServices, name, supplier, quantity, day, month, year);
}

int deleteHandler(UI* ui)
{
	char name[MAX_STRING_SIZE] = { 0 };
	char supplier[MAX_STRING_SIZE] = { 0 };
	int day = 0, month = 0, year = 0;

	getNameInput(name);
	getSupplierInput(supplier);
	getDateInput(&day, &month, &year);
	
	return rem(ui->materialServices, name, supplier, day, month, year);
}

int updateHandler(UI* ui)
{
	char name[MAX_STRING_SIZE] = { 0 };
	char supplier[MAX_STRING_SIZE] = { 0 };
	int day = 0, month = 0, year = 0;

	char newName[MAX_STRING_SIZE] = { 0 };
	char newSupplier[MAX_STRING_SIZE] = { 0 };
	double newQuantity = 0;
	int newDay = 0, newMonth = 0, newYear = 0;

	printf("Enter the material you want to change...\n");
	getNameInput(name);
	getSupplierInput(supplier);
	getDateInput(&day, &month, &year);

	printf("Enter new data...\n");
	getNameInput(newName);
	getSupplierInput(newSupplier);
	getQuantityInput(&newQuantity);
	getDateInput(&newDay, &newMonth, &newYear);

	return update(ui->materialServices, name, supplier, day, month, year, 
										newName, newSupplier, newQuantity, newDay, newMonth, newYear);
}

int getExpiredHandler(UI* ui)
{	
	char s[MAX_STRING_SIZE] = { 0 };
	int (*filterFunction)(Material*, char*);

	printExpiredMenu();
	while (1)
	{
		char command[MAX_COMMAND_SIZE] = { 0 };

		int status = getCommand(command);
		if (status == -1)
			return -1;
		if (strcmp(command, "1") == 0)
		{
			status = getString(s);
			if (status == -1)
				return -1;

			filterFunction = &nameContains;
			break;
		}
		else if (strcmp(command, "2") == 0)
		{
			double q = 0;

			status = getQuantityInput(&q);
			if (status == -1)
				return -1;

			snprintf(s, MAX_STRING_SIZE - 1, "%lf", q);
			
			filterFunction = &isLessThan;
			break;
		}
		else
			printf("Invalid option!\n");
	}

	DynamicArray* expired;
	if (strcmp(s, "none") == 0)
		expired = getExpired(ui->materialServices, filterFunction, "");
	else
		expired = getExpired(ui->materialServices, filterFunction, s);

	if (expired == NULL)
		return -1;

	printMaterials(expired);

	destroyDynamicArray(expired);
	return 1;
}

int sortHandler(UI* ui)
{
	DynamicArray* sorted = getSortedAscending(ui->materialServices);

	if (sorted == NULL)
		return -1;

	printMaterials(sorted);

	destroyDynamicArray(sorted);
	return 1;
}

int getShortHandler(UI* ui)
{
	char filterSupplier[MAX_STRING_SIZE] = { 0 };
	double filterQuantity = 0;
	int (*compareFunction)(Material*, Material*);
	
	getSupplierInput(filterSupplier);
	getQuantityInput(&filterQuantity);

	printShortMenu();
	while (1)
	{
		char command[MAX_COMMAND_SIZE] = { 0 };

		int status = getCommand(command);
		if (status == -1)
			return -1;
		if (strcmp(command, "1") == 0)
		{
			compareFunction = &less;
			break;
		}
		else if (strcmp(command, "2") == 0)
		{
			compareFunction = &greater;
			break;
		}
		else
			printf("Invalid option!\n");
	}

	DynamicArray* shortMaterial = getShort(ui->materialServices, compareFunction, filterSupplier, filterQuantity);

	if (shortMaterial == NULL)
		return -1;

	printMaterials(shortMaterial);

	destroyDynamicArray(shortMaterial);
	return 1;
}

void commandHandler(UI* ui)
{
	while (1)
	{
		char command[MAX_COMMAND_SIZE];
		int status = getCommand(command);

		command[strcspn(command, "\r\n")] = 0; //remove any unwanted invisible characters
		if (status == 1)
		{
			if (strcmp(command, "exit") == 0)
				//exit(0);
				break;
			else if (strcmp(command, "help") == 0)
				printMenu();
			else if (strcmp(command, "add") == 0)
			{
				status = addHandler(ui);
				if (status == 1)
					printf("Added successfully!\n");
				else
					printf("An error occured while trying to add the material, try again!\n");
			}
			else if (strcmp(command, "delete") == 0)
			{
				status = deleteHandler(ui);
				if (status == 1)
					printf("Deleted successfully!\n");
				else
					printf("An error occured while trying to delete the material, try again!\n");
			}
			else if (strcmp(command, "update") == 0)
			{
				status = updateHandler(ui);
				if (status == 1)
					printf("Updated successfully!\n");
				else
					printf("An error occured while trying to update the material, try again!\n");
			}
			else if (strcmp(command, "list") == 0)
			{
				printMaterials(ui->materialServices->materialRepo->data);
			}
			else if (strcmp(command, "expired") == 0)
			{
				status = getExpiredHandler(ui);
				if (status == -1)
					printf("An error occured while trying to retrieve expired materials!\n");
			}
			else if (strcmp(command, "sort") == 0)
			{
				status = sortHandler(ui);
				if (status == -1)
					printf("Something went wrong!\n");
			}
			else if (strcmp(command, "short") == 0)
			{
				status = getShortHandler(ui);
				if (status == -1)
					printf("Something went wrong!\n");
			}
			else if (strcmp(command, "undo") == 0)
			{
				status = undo(ui->materialServices);
				if (status == 1)
					printf("Undone successfully!\n");
				else
					printf("No operations to undo!\n");
			}
			else if (strcmp(command, "redo") == 0)
			{
				status = redo(ui->materialServices);
				if (status == 1)
					printf("Undone successfully!\n");
				else
					printf("No operations to redo!\n");
			}
			else
				printf("Invalid command!\n");
		}
	}
}

void start(UI* ui)
{
	initMaterialRepo(ui->materialServices);
	printMenu();
	commandHandler(ui);
}

// TODO: change atoi and atof to strtol and strtod
