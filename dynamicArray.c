
#include "dynamicArray.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>


DynamicArray* createDynamicArray(int capacity, void (*destroyFunction)(void*))
{
	DynamicArray* dArray = (DynamicArray*)malloc(sizeof(DynamicArray));

	if (dArray == NULL)
		return NULL;

	dArray->data = (void**)malloc(sizeof(void*) * capacity);
	dArray->destroyFunction = destroyFunction;
	dArray->capacity = capacity;
	dArray->size = 0;

	return dArray;
}

void destroyDynamicArray(DynamicArray* dArray)
{
	if (dArray == NULL)
		return;
	if (dArray->destroyFunction != NULL)
		for (int i = 0; i < dArray->size; i++)
			dArray->destroyFunction(dArray->data[i]);
	free(dArray->data);
	free(dArray);
}

int len(DynamicArray* dArray)
{
	if (dArray == NULL)
		return -1;

	return dArray->size;
}

void* getElement(DynamicArray* dArray, int position)
{
	if (dArray == NULL)
		return NULL;

	if (position >= len(dArray) || position < 0)
		return NULL;

	return dArray->data[position];
}

int resize(DynamicArray* dArray)
{
	dArray->capacity *= 2;
	void** data = (void**)malloc(sizeof(void*) * dArray->capacity);

	if (data == NULL)
		return -1;

	memcpy(data, dArray->data, sizeof(void*) * dArray->size);
	free(dArray->data);
	dArray->data = data;

	return 1;
}

int apd(DynamicArray* dArray, void* element)
{
	if (dArray == NULL || element == NULL)
		return -1;

	if (dArray->capacity == dArray->size)
	{
		int status = resize(dArray);
		if (status == -1)
			return -1;
	}

	dArray->data[dArray->size] = element;
	dArray->size++;
	return 1;
}

int del(DynamicArray* dArray, int position)
{
	if (dArray == NULL)
		return -1;

	if (position < 0 || position >= dArray->size)
		return -1;

	void* aux = dArray->data[position];
	for (int i = position; i < dArray->size - 1; i++)
		dArray->data[i] = dArray->data[i + 1];

	dArray->size--;
	dArray->destroyFunction(aux);

	return 1;
}

int upd(DynamicArray* dArray, int position, void* newValues)
{
	if (dArray == NULL || newValues == NULL)
		return -1;
	
	if (position < 0 || position >= dArray->size)
		return -1;

	void* aux = dArray->data[position];
	dArray->data[position] = newValues;
	dArray->destroyFunction(aux);

	return 1;
}

int swap(DynamicArray* dArray, int x, int y)
{
	if (x < 0 || x >= len(dArray) || y < 0 || y >= len(dArray))
		return -1;

	void* aux = dArray->data[x];
	dArray->data[x] = dArray->data[y];
	dArray->data[y] = aux;

	return 1;
}

int sort(DynamicArray* dArray, int(compareFunction(void*, void*)))
{
	if (dArray == NULL || compareFunction == NULL)
		return -1;

	for (int i = 0; i < len(dArray)-1; i++)
		for (int j = i + 1; j < len(dArray); j++)
		{
			void* x = getElement(dArray, i);
			void* y = getElement(dArray, j);
			if (compareFunction(x, y) == 0)
				swap(dArray, i, j);
		}
	return 1;
}


//Tests

void testCreateDynamicArray()
{
	DynamicArray* testArray= createDynamicArray(10, NULL);

	assert(testArray != NULL);

	destroyDynamicArray(testArray);
}

void testDynamicArrayLen()
{
	DynamicArray* testArray = createDynamicArray(10, &destroyDynamicArray);

	assert(len(NULL) == -1);
	assert(len(testArray) == 0);
	
	DynamicArray* element = createDynamicArray(10, NULL);

	apd(testArray, element);
	assert(len(testArray) == 1);

	destroyDynamicArray(testArray);
}

void testDynamicArrayGetElement()
{
	DynamicArray* testArray = createDynamicArray(10, &destroyDynamicArray);
	DynamicArray* element = createDynamicArray(10, NULL);

	assert(getElement(NULL, 0) == NULL);
	assert(getElement(testArray, 1) == NULL);
	assert(getElement(testArray, -1) == NULL);

	apd(testArray, element);
	assert(getElement(testArray, 0) == element);
	
	destroyDynamicArray(testArray);
}

void testDynamicArrayApd()
{
	DynamicArray* testArray = createDynamicArray(10, &destroyDynamicArray);

	assert(apd(NULL, NULL) == -1);
	assert(apd(testArray, NULL) == -1);

	DynamicArray* element1 = createDynamicArray(10, NULL);
	DynamicArray* element2 = createDynamicArray(10, NULL);
	
	assert(apd(testArray, element1) == 1);
	assert(len(testArray) == 1);

	assert(apd(testArray, element2) == 1);
	assert(len(testArray) == 2);
	
	destroyDynamicArray(testArray);
}

void testDynamicArrayUpd()
{
	DynamicArray* testArray = createDynamicArray(10, &destroyDynamicArray);

	assert(upd(NULL, 0, NULL) == -1);
	assert(upd(testArray, 0, NULL) == -1);

	DynamicArray* element1 = createDynamicArray(10, NULL);
	DynamicArray* element2 = createDynamicArray(10, NULL);
	
	apd(testArray, element1);

	assert(upd(testArray, 0, element2) == 1);
	assert(getElement(testArray, 0) == element2);

	destroyDynamicArray(testArray);
}

void testDynamicArrayDel()
{
	DynamicArray* testArray = createDynamicArray(10, &destroyDynamicArray);

	assert(del(NULL, 0) == -1);
	assert(del(testArray, 0) == -1);

	DynamicArray* element1 = createDynamicArray(10, NULL);
	DynamicArray* element2 = createDynamicArray(10, NULL);

	apd(testArray, element1);
	apd(testArray, element2);
	
	assert(del(testArray, 1) == 1);
	assert(len(testArray) == 1);
	assert(del(testArray, 0) == 1);
	assert(len(testArray) == 0);

	destroyDynamicArray(testArray);
}

void testSwap()
{
	DynamicArray* testArray = createDynamicArray(10, &destroyDynamicArray);
	
	assert(swap(testArray, 0, 1) == -1);
	
	DynamicArray* element1 = createDynamicArray(10, NULL);
	DynamicArray* element2 = createDynamicArray(10, NULL);

	apd(testArray, element1);
	apd(testArray, element2);

	assert(swap(testArray, 0, 1) == 1);
	assert(getElement(testArray, 0) == element2);
	assert(getElement(testArray, 1) == element1);

	destroyDynamicArray(testArray);
}

int compareInts(int* x, int* y)
{
	if (x == NULL || y == NULL)
		return -1;

	if (*x > *y)
		return 0;

	return 1;
}

void testSort()
{
	DynamicArray* testArray = createDynamicArray(10, &free);

	int* element1 = (int*)malloc(sizeof(int));
	int* element2 = (int*)malloc(sizeof(int));
	
	if(element1 != NULL)
		*element1 = 1;
	if(element2 != NULL)
		*element2 = 2;

	apd(testArray, element2);
	apd(testArray, element1);

	sort(testArray, &compareInts);

	assert(getElement(testArray, 0) == element1);
	assert(getElement(testArray, 1) == element2);

	destroyDynamicArray(testArray);
}

void testDynamicArray()
{
	testCreateDynamicArray();
	testDynamicArrayLen();
	testDynamicArrayGetElement();
	testDynamicArrayApd();
	testDynamicArrayUpd();
	testDynamicArrayDel();
	testSwap();
	testSort();
}
