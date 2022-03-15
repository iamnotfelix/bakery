#pragma once


typedef struct DynamicArray
{
	int size, capacity;
	void** data;
	void (*destroyFunction)(void*);
} DynamicArray;

/*
	Creates a dynamic array.
	capacity - the initial capacity of the dynamic array
	destroyFunction - a pointer to the destroy function of the pointers that the dynamic array stores
	Returns a pointer to the new dynamic array.
*/
DynamicArray* createDynamicArray(int capacity, void (*destroyFunction)(void*));

/*
	Deallocates the memory ocuppied by the dynamic array and its contents.
	dArray - pointer to the dynamic array
*/
void destroyDynamicArray(DynamicArray* dArray);


/*
	Gets the size(length) of the dynamic array.
	dArray - pointer to the dynamic array
	Returns the size of the dunamic array, or if the pointer to the dynamic array is NULL it returns -1.
*/
int len(DynamicArray* dArray);

/*
	Gets the element from the given position in the dynamic array.
	dArray - pointer to the dynamic array
	Returns a pointer to the element from the given position or NULL if the position or the pointer to the dynamic array are not valid.
*/
void* getElement(DynamicArray* dArray, int position);

/*
	Gets the size(length) of the dynamic array.
	dArray - pointer to the dynamic array
*/
int apd(DynamicArray* dArray, void* element);
int upd(DynamicArray* dArray, int position, void* newValues);
int del(DynamicArray* dArray, int position);

int swap(DynamicArray* dArray, int x, int y);
int sort(DynamicArray* dArray, int(compareFunction(void*, void*)));

//Tests
void testDynamicArray();