#pragma once

#include "repository.h"

#define MAX_COMMAND_SIZE 32
#define MAX_STRING_SIZE 64

typedef struct MaterialServices
{
	int index;
	DynamicArray* repoStack;
	MaterialRepo* materialRepo;
} MaterialServices;

MaterialServices* createMaterialServices(MaterialRepo* materialRepo);
void destroyMaterialServices(MaterialServices* materialServices);
void initMaterialRepo(MaterialServices* materialServices);

Material* getMaterial(MaterialServices* materialServices, int position);
DynamicArray* getExpired(MaterialServices* materialServices, int (*filterFunction)(Material*, char*), char* filter);
DynamicArray* getSortedAscending(MaterialServices* materialServices);
DynamicArray* getShort(MaterialServices* materialServices, int (*compareFunction)(Material*, Material*), char* filterSupplier, double filterQuantity);

int add(MaterialServices* materialServices, char* name, char* supplier, double quantity, int day, int month, int year);
int update(MaterialServices* materialServices, 
			char* name, char* supplier, int day, int month, int year, 
			char* newName, char* newSupplier, double newQuantity, int newDay, int newMonth, int newYear);
int rem(MaterialServices* materialServices, char* name, char* supplier, int day, int month, int year);

int undo(MaterialServices* materialServices);
int redo(MaterialServices* materialServices);

//Tests
void testMaterialServices();
