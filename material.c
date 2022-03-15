
#include "material.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>


Material* createMaterial(char* name, char* supplier, double quantity, Date* date)
{
	Material* material = (Material*)malloc(sizeof(Material));

	if (material == NULL)
		return NULL;

	material->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
	
	if (material->name == NULL)
	{
		free(material);
		return NULL;
	}

	strcpy(material->name, name);

	material->supplier = (char*)malloc(sizeof(char) * (strlen(supplier) + 1));

	if (material->supplier == NULL)
	{
		free(material->name);
		free(material);
		return NULL;
	}

	strcpy(material->supplier, supplier);

	material->date = date;
	material->quantity = quantity;

	return material;
}

void destroyMaterial(Material* material)
{
	if (material == NULL)
		return;
	free(material->name);
	free(material->supplier);
	destroyDate(material->date);
	free(material);
}

const char* getName(Material* material)
{
	if (material == NULL)
		return NULL;

	return material->name;
}

const char* getSupplier(Material* material)
{
	if (material == NULL)
		return NULL;

	return material->supplier;
}

double getQuantity(Material* material)
{
	if (material == NULL)
		return -1;

	return material->quantity;
}

const Date* getDate(Material* material)
{
	if (material == NULL)
		return NULL;

	return material->date;
}

int equalMaterials(Material* x, Material* y)
{
	if (x == NULL || y == NULL)
		return -1;

	if (strcmp(getName(x), getName(y)) != 0)
		return 0;

	if (strcmp(getSupplier(x), getSupplier(y)) != 0)
		return 0;

	if (equalDates(getDate(x), getDate(y)) == 0)
		return 0;

	return 1;
}

int isLessThan(Material* material, char* quantity)
{
	if (material == NULL)
		return -1;

	double q = strtod(quantity, NULL);
	if (getQuantity(material) <	q)
		return 1;
	return 0;
}

int nameContains(Material* material, char* string)
{
	if (material == NULL)
		return -1;

	char* status = strstr(getName(material), string);
	if (status == NULL)
		return 0;
	return 1;
}

int less(Material* x, Material* y)
{
	if (x == NULL || y == NULL)
		return -1;

	double quantity1 = getQuantity(x);
	double quantity2 = getQuantity(y);

	if (quantity1 < quantity2)
		return 1;

	return 0;
}

int greater(Material* x, Material* y)
{
	if (x == NULL || y == NULL)
		return -1;

	double quantity1 = getQuantity(x);
	double quantity2 = getQuantity(y);

	if (quantity1 > quantity2)
		return 1;

	return 0;
}

Material* copyMaterial(Material* material)
{
	if (material == NULL)
		return NULL;

	Material* materialCopy = (Material*)malloc(sizeof(Material));

	if (materialCopy == NULL)
		return NULL;

	char* nameCopy = (char*)malloc(sizeof(char) * (strlen(getName(material)) + 1));
	if (nameCopy == NULL)
	{
		free(materialCopy);
		return NULL;
	}
	strcpy(nameCopy, getName(material));

	char* supplierCopy = (char*)malloc(sizeof(char) * (strlen(getSupplier(material)) + 1));
	if (supplierCopy == NULL)
	{
		free(nameCopy);
		free(materialCopy);
		return NULL;
	}
	strcpy(supplierCopy, getSupplier(material));

	Date* dateCopy = copyDate(material->date);
	if (dateCopy == NULL)
	{
		free(nameCopy);
		free(supplierCopy);
		free(materialCopy);
		return NULL;
	}

	materialCopy->name = nameCopy;
	materialCopy->supplier = supplierCopy;
	materialCopy->quantity = material->quantity;
	materialCopy->date = dateCopy;

	return materialCopy;
}


//Tests


void testCreateMaterial()
{
	Date* testDate = createDate(1, 2, 3);
	Material* testMaterial = createMaterial("testName", "testSupplier", 12.34, testDate);

	assert(testMaterial != NULL);

	destroyMaterial(testMaterial);
}

void testMaterialGetters()
{
	Date* testDate = createDate(1, 2, 3);
	Material* testMaterial = createMaterial("testName", "testSupplier", 12.34, testDate);

	assert(strcmp("testName", getName(testMaterial)) == 0);
	assert(strcmp("testSupplier", getSupplier(testMaterial)) == 0);
	assert(getQuantity(testMaterial) == 12.34);
	assert(getDate(testMaterial) == testDate);

	destroyMaterial(testMaterial);
}

void testEqualMaterials()
{
	Date* testDate1 = createDate(1, 2, 3);
	Material* testMaterial1 = createMaterial("testName", "testSupplier", 12.34, testDate1);

	Date* testDate2 = createDate(2, 3, 4);
	Material* testMaterial2 = createMaterial("testName2", "testSupplier2", 12.345, testDate2);

	Date* testDate3 = createDate(1, 2, 3);
	Material* testMaterial3 = createMaterial("testName", "testSupplier", 12.3, testDate3);

	assert(equalMaterials(testMaterial1, testMaterial2) == 0);
	assert(equalMaterials(testMaterial2, testMaterial3) == 0);
	assert(equalMaterials(testMaterial1, testMaterial3) == 1);
	assert(equalMaterials(NULL, testMaterial1) == -1);

	destroyMaterial(testMaterial1);
	destroyMaterial(testMaterial2);
	destroyMaterial(testMaterial3);
}

void testIsLessThan()
{
	Date* testDate = createDate(1, 2, 3);
	Material* testMaterial = createMaterial("testName", "testSupplier", 12.34, testDate);

	assert(isLessThan(NULL, "123") == -1);
	assert(isLessThan(testMaterial, "12.5") == 1);
	assert(isLessThan(testMaterial, "12.3") == 0);

	destroyMaterial(testMaterial);
}

void testNameContains()
{
	Date* testDate = createDate(1, 2, 3);
	Material* testMaterial = createMaterial("testName", "testSupplier", 12.34, testDate);

	assert(nameContains(NULL, "test") == -1);
	assert(nameContains(testMaterial, "test") == 1);
	assert(nameContains(testMaterial, "other") == 0);

	destroyMaterial(testMaterial);
}

void testLessGreater()
{
	Date* testDate1 = createDate(1, 2, 3);
	Material* testMaterial1 = createMaterial("testName", "testSupplier", 12.34, testDate1);

	Date* testDate2 = createDate(1, 2, 3);
	Material* testMaterial2 = createMaterial("testName", "testSupplier", 12.35, testDate2);

	assert(less(NULL, NULL) == -1);
	assert(greater(NULL, NULL) == -1);
	assert(less(testMaterial1, testMaterial2) == 1);
	assert(less(testMaterial2, testMaterial1) == 0);
	assert(greater(testMaterial1, testMaterial2) == 0);
	assert(greater(testMaterial2, testMaterial1) == 1);

	destroyMaterial(testMaterial1);
	destroyMaterial(testMaterial2);
}

void testCopyMaterial()
{
	Date* testDate = createDate(1, 2, 3);
	Material* testMaterial = createMaterial("testName", "testSupplier", 12.34, testDate);

	assert(copyMaterial(NULL) == NULL);

	Material* copyOfMaterial = copyMaterial(testMaterial);

	assert(testMaterial != copyOfMaterial);
	
	assert(equalMaterials(testMaterial, copyOfMaterial) == 1);
	assert(strcmp(getName(testMaterial), getName(copyOfMaterial)) == 0);
	assert(strcmp(getSupplier(testMaterial), getSupplier(copyOfMaterial)) == 0);
	assert(getQuantity(testMaterial) == getQuantity(copyOfMaterial));
	assert(equalDates(getDate(testMaterial), getDate(copyOfMaterial)) == 1);

	destroyMaterial(testMaterial);
	destroyMaterial(copyOfMaterial);
}

void testMaterial()
{
	testCreateMaterial();
	testMaterialGetters();
	testEqualMaterials();
	testIsLessThan();
	testNameContains();
	testLessGreater();
	testCopyMaterial();
}
