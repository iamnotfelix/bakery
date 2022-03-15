
#include "services.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>


MaterialServices* createMaterialServices(MaterialRepo* materialRepo)
{
	MaterialServices* materialServices = (MaterialServices*)malloc(sizeof(MaterialServices));

	if (materialServices == NULL)
		return NULL;

	materialServices->materialRepo = materialRepo;
	materialServices->index = 0;
	materialServices->repoStack = createDynamicArray(2, &destroyMaterialRepo);

	if (materialServices->repoStack == NULL)
		return NULL;

	int status = apd(materialServices->repoStack, materialServices->materialRepo);

	if (status == -1)
		return NULL;
	
	return materialServices;
}

void destroyMaterialServices(MaterialServices* materialServices)
{
	if (materialServices == NULL)
		return;

	destroyDynamicArray(materialServices->repoStack);
	free(materialServices);
}

void initMaterialRepo(MaterialServices* materialServices)
{
	if (materialServices == NULL)
		return;

	addMaterial(materialServices->materialRepo, createMaterial("Wheat flour", "WindMill", 10.5, createDate(24, 5, 2025)));
	addMaterial(materialServices->materialRepo, createMaterial("Sugar", "HomeGoods", 20, createDate(20, 6, 2024)));
	addMaterial(materialServices->materialRepo, createMaterial("Eggs", "JohnsFarm", 100, createDate(3, 4, 2022)));
	addMaterial(materialServices->materialRepo, createMaterial("Salt", "HomeGoods", 15, createDate(30, 10, 2030)));
	addMaterial(materialServices->materialRepo, createMaterial("Baking powder", "HomeGoods", 1.25, createDate(10, 11, 2021)));
	addMaterial(materialServices->materialRepo, createMaterial("Butter", "JohnsFarm", 10.5, createDate(7, 3, 2022)));
	addMaterial(materialServices->materialRepo, createMaterial("Cake flour", "CakesSupply", 15.3, createDate(10, 5, 2022)));
	addMaterial(materialServices->materialRepo, createMaterial("Pastry flour", "CakesSupply", 5, createDate(10, 5, 2022)));
	addMaterial(materialServices->materialRepo, createMaterial("Sprouted flour", "CakesSupply", 25, createDate(12, 3, 2022)));
	addMaterial(materialServices->materialRepo, createMaterial("Seeds mix", "HomeGoods", 33, createDate(3, 3, 2022)));

}

Material* getMaterial(MaterialServices* materialServices, int position)
{
	if (materialServices == NULL)
		return NULL;

	return getMaterialAtPos(materialServices->materialRepo, position);
}


DynamicArray* getExpired(MaterialServices* materialServices, int (*filterFunction)(Material*, char*), char* filter)
{
	DynamicArray* dArray = createDynamicArray(2, &destroyMaterial);
	
	for (int i = 0; i < getSize(materialServices->materialRepo); i++)
	{
		Material* material = getMaterialAtPos(materialServices->materialRepo, i);

		int expired = isExpired(getDate(material));
		int ok = filterFunction(material, filter);
		if (expired == 1 && ok == 1)
		{
			Material* materialCopy = copyMaterial(material);
			apd(dArray, materialCopy);
		}
	}

	return dArray;
}

void clearStack(MaterialServices* materialServices)
{
	for (int i = materialServices->index + 1; i < len(materialServices->repoStack); i++)
	{
		del(materialServices->repoStack, i);
	}
}

int setMaterialRepo(MaterialServices* materialServices)
{
	if (materialServices->index < len(materialServices->repoStack) - 1)
		clearStack(materialServices);

	MaterialRepo* materialRepoCopy = copyMaterialRepo(getElement(materialServices->repoStack, materialServices->index));

	if (materialRepoCopy == NULL)
		return -1;

	int status = apd(materialServices->repoStack, materialRepoCopy);

	if (status == -1)
		return -1;
	
	materialServices->materialRepo = materialRepoCopy;
	materialServices->index++;

	return 1;
}

int add(MaterialServices* materialServices, char* name, char* supplier, double quantity, int day, int month, int year)
{
	if (materialServices == NULL)
		return -1;

	Date* date = createDate(day, month, year);
	Material* material = createMaterial(name, supplier, quantity, date);

	if (material == NULL)
		return -1;

	int status = setMaterialRepo(materialServices);

	if (status == -1)
	{
		destroyMaterial(material);
		return -1;
	}

	status = addMaterial(materialServices->materialRepo, material);
	if (status == -1)
		destroyMaterial(material);

	return status;
}

int update(MaterialServices* materialServices, char* name, char* supplier, int day, int month, int year, char* newName, char* newSupplier, double newQuantity, int newDay, int newMonth, int newYear)
{
	if (materialServices == NULL)
		return -1;
	
	Date* date = createDate(day, month, year);
	Date* newDate = createDate(newDay, newMonth, newYear);

	Material* material = createMaterial(name, supplier, 0, date);
	Material* newMaterial = createMaterial(newName, newSupplier, newQuantity, newDate);

	if (material == NULL || newMaterial == NULL)
		return -1;

	int status = setMaterialRepo(materialServices);

	if (status == -1)
	{
		destroyMaterial(material);
		destroyMaterial(newMaterial);
		return -1;
	}

	status = updateMaterial(materialServices->materialRepo, material, newMaterial);

	destroyMaterial(material);
	if (status == -1) 
		destroyMaterial(newMaterial);

	return status;
}

int rem(MaterialServices* materialServices, char* name, char* supplier, int day, int month, int year)
{
	if (materialServices == NULL)
		return -1;

	Date* date = createDate(day, month, year);
	Material* material = createMaterial(name, supplier, 0, date);

	if (material == NULL)
		return -1;

	int status = setMaterialRepo(materialServices);

	if (status == -1)
	{
		destroyMaterial(material);
		return -1;
	}

	status = removeMaterial(materialServices->materialRepo, material);
	destroyMaterial(material);

	return status;
}

int undo(MaterialServices* materialServices)
{
	if (materialServices->index > 0)
	{ 
		materialServices->index--;
		materialServices->materialRepo = getElement(materialServices->repoStack, materialServices->index);
	}
	else
		return -1;
	
	return 1;
}

int redo(MaterialServices* materialServices)
{
	if (materialServices->index < len(materialServices->repoStack) - 1)
	{
		materialServices->index++;
		materialServices->materialRepo = getElement(materialServices->repoStack, materialServices->index);
	}
	else
		return -1;

	return 1;
}

DynamicArray* getSortedAscending(MaterialServices* materialServices)
{
	if (materialServices == NULL)
		return NULL;

	DynamicArray* dArray = createDynamicArray(10, &destroyMaterial);

	if (dArray == NULL)
		return NULL;

	for (int i = 0; i < getSize(materialServices->materialRepo); i++)
	{
		Material* material = getMaterialAtPos(materialServices->materialRepo, i);
		Material* materialCopy = copyMaterial(material);
		apd(dArray, materialCopy);
	}

	for (int i = 0; i < len(dArray) - 1; i++)
		for (int j = i + 1; j < len(dArray); j++)
		{
			Material* x = getElement(dArray, i);
			Material* y = getElement(dArray, j);

			if (strcmp(getName(x), getName(y)) > 0)
			{
				swap(dArray, i, j);
			}
		}
	return dArray;
}

DynamicArray* getShort(MaterialServices* materialServices, int (*compareFunction)(Material*, Material*), char* filterSupplier, double filterQuantity)
{ 
	if (materialServices == NULL)
		return NULL;

	DynamicArray* dArray = createDynamicArray(2, &destroyMaterial);

	if (dArray == NULL)
		return NULL;

	for (int i = 0; i < getSize(materialServices->materialRepo); i++)
	{
		Material* material = getMaterialAtPos(materialServices->materialRepo, i);
		if (getQuantity(material) < filterQuantity && strcmp(getSupplier(material), filterSupplier) == 0)
		{
			Material* materialCopy = copyMaterial(material);
			apd(dArray, materialCopy);
		}
	}

	sort(dArray, compareFunction);

	return dArray;
}


//Tests


void testCreateMaterialServices()
{
	MaterialRepo* materialRepo = createMaterialRepo(10);
	MaterialServices* materialServices = createMaterialServices(materialRepo);

	assert(materialRepo != NULL && materialServices != NULL);

	destroyMaterialServices(materialServices);
}

void testGetMaterial()
{
	MaterialRepo* materialRepo = createMaterialRepo(10);
	MaterialServices* materialServices = createMaterialServices(materialRepo);
	
	add(materialServices, "testName", "testSupplier", 12.34, 1, 2, 3);
	
	assert(getMaterial(materialServices, 1) == NULL);
	assert(getMaterial(materialServices, 0) != NULL);

	destroyMaterialServices(materialServices);
}

void testGetExpired()
{
	MaterialRepo* materialRepo = createMaterialRepo(10);
	MaterialServices* materialServices = createMaterialServices(materialRepo);

	add(materialServices, "testName1", "testSupplier", 1, 12, 3, 2022);
	add(materialServices, "otherName2", "testSupplier", 2, 12, 3, 2022);
	add(materialServices, "testName3", "otherSupplier", 2, 12, 3, 2022);
	add(materialServices, "testName4", "testSupplier", 3, 12, 3, 2022);
	add(materialServices, "otherName5", "testSupplier", 4, 12, 3, 2022);

	DynamicArray* dArray1 = getExpired(materialServices, &isLessThan, "3.5234");

	assert(len(dArray1) == 4);
	assert(getQuantity(getElement(dArray1, 0)) == 1);
	assert(getQuantity(getElement(dArray1, 1)) == 2);
	assert(getQuantity(getElement(dArray1, 2)) == 2);
	assert(getQuantity(getElement(dArray1, 3)) == 3);

	DynamicArray* dArray2 = getExpired(materialServices, &nameContains, "test");

	assert(len(dArray2) == 3);
	assert(getQuantity(getElement(dArray2, 0)) == 1);
	assert(getQuantity(getElement(dArray2, 1)) == 2);
	assert(getQuantity(getElement(dArray2, 2)) == 3);

	destroyDynamicArray(dArray1);
	destroyDynamicArray(dArray2);
	destroyMaterialServices(materialServices);
}

void testGetShort()
{
	MaterialRepo* materialRepo = createMaterialRepo(10);
	MaterialServices* materialServices = createMaterialServices(materialRepo);

	add(materialServices, "testName1", "testSupplier", 1, 1, 2, 3);
	add(materialServices, "testName2", "testSupplier", 2, 1, 2, 3);
	add(materialServices, "testName3", "otherSupplier", 2, 1, 2, 3);
	add(materialServices, "testName4", "testSupplier", 3, 1, 2, 3);
	add(materialServices, "testName5", "testSupplier", 4, 1, 2, 3);

	DynamicArray* dArray = getShort(materialServices, &less, "testSupplier", 3.5);

	assert(len(dArray) == 3);
	assert(getQuantity(getElement(dArray, 0)) == 1);
	assert(getQuantity(getElement(dArray, 1)) == 2);
	assert(getQuantity(getElement(dArray, 2)) == 3);

	destroyDynamicArray(dArray);
	destroyMaterialServices(materialServices);
}

void testAdd()
{
	MaterialRepo* materialRepo = createMaterialRepo(1);
	MaterialServices* materialServices = createMaterialServices(materialRepo);

	assert(add(NULL, "testName", "testSupplier", 12.34, 1, 2, 3) == -1);

	assert(add(materialServices, "testName", "testSupplier", 12.34, 1, 2, 3) == 1);
	assert(getSize(materialServices->materialRepo) == 1);

	assert(add(materialServices, "otherName", "otherSupplier", 23.45, 2, 3, 4) == 1);
	assert(getSize(materialServices->materialRepo) == 2);

	destroyMaterialServices(materialServices);
}

void testUpdate()
{
	MaterialRepo* materialRepo = createMaterialRepo(1);
	MaterialServices* materialServices = createMaterialServices(materialRepo);

	add(materialServices, "testName", "testSupplier", 12.34, 1, 2, 3);

	assert(update(NULL, "testName", "testSupplier", 1, 2, 3, "otherName", "otherSupplier", 69.420, 1, 2, 3) == -1);
	assert(update(materialServices, "anotherName", "anotherSupplier", 1, 2, 3, "otherName", "otherSupplier", 69.420, 3, 3, 3) == -1);
	assert(update(materialServices, "testName", "testSupplier", 1, 2, 3, "otherName", "otherSupplier", 69.420, 3, 3, 3) == 1);
	assert(strcmp(getName(getElement(materialServices->materialRepo->data, 0)), "otherName") == 0);
	assert(strcmp(getSupplier(getElement(materialServices->materialRepo->data, 0)), "otherSupplier") == 0);
	assert(getDay(getDate(getElement(materialServices->materialRepo->data, 0))) == 3);

	destroyMaterialServices(materialServices);
}

void testRem()
{
	MaterialRepo* materialRepo = createMaterialRepo(1);
	MaterialServices* materialServices = createMaterialServices(materialRepo);

	add(materialServices, "testName", "testSupplier", 12.34, 1, 2, 3);

	assert(rem(NULL, "testName", "testSupplier", 1, 2, 3) == -1);
	assert(rem(materialServices, "otherName", "otherSupplier", 1, 2, 3) == -1);
	assert(rem(materialServices, "testName", "testSupplier", 1, 2, 3) == 1);
	assert(getSize(materialServices->materialRepo) == 0);

	destroyMaterialServices(materialServices);
}

void testUndoRedo()
{
	MaterialRepo* materialRepo = createMaterialRepo(1);
	MaterialServices* materialServices = createMaterialServices(materialRepo);

	assert(len(materialServices->repoStack) == 1);
	assert(materialServices->materialRepo == getElement(materialServices->repoStack, 0));
	assert(undo(materialServices) == -1);
	assert(redo(materialServices) == -1);

	add(materialServices, "a", "a", 1, 1, 1, 1);
	assert(materialServices->index == 1);
	assert(materialServices->materialRepo == getElement(materialServices->repoStack, 1));
	assert(len(materialServices->repoStack) == 2);
	assert(undo(materialServices) == 1);
	assert(materialServices->index == 0);
	assert(redo(materialServices) == 1);
	assert(materialServices->index == 1);
	assert(redo(materialServices) == -1);

	update(materialServices, "a", "a", 1, 1, 1, "b", "b", 2, 2, 2, 2);
	assert(materialServices->index == 2);
	assert(materialServices->materialRepo == getElement(materialServices->repoStack, 2));
	assert(len(materialServices->repoStack) == 3);
	assert(undo(materialServices) == 1);
	assert(materialServices->index == 1);
	assert(redo(materialServices) == 1);
	assert(materialServices->index == 2);
	assert(redo(materialServices) == -1);
	
	assert(undo(materialServices) == 1);

	rem(materialServices, "a", "a", 1, 1, 1);
	assert(materialServices->index == 2);
	assert(materialServices->materialRepo == getElement(materialServices->repoStack, 2));
	assert(len(materialServices->repoStack) == 3);
	assert(undo(materialServices) == 1);
	assert(materialServices->index == 1);
	assert(redo(materialServices) == 1);
	assert(materialServices->index == 2);
	assert(redo(materialServices) == -1);

	destroyMaterialServices(materialServices);
}

void testMaterialServices()
{
	testCreateMaterialServices();
	testGetMaterial();
	testGetExpired();
	testGetShort();
	testAdd();
	testUpdate();
	testRem();
	testUndoRedo();
}
