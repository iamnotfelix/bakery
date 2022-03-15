#pragma once

#include "date.h"

typedef struct Material
{
	char* name;
	char* supplier;
	double quantity;
	Date* date;
} Material;

Material* createMaterial(char* name, char* supplier, double quantity, Date* date);
void destroyMaterial(Material* material);

const char* getName(Material* material);
const char* getSupplier(Material* material);
double getQuantity(Material* material);
const Date* getDate(Material* material);

int equalMaterials(Material* x, Material* y);
int isLessThan(Material* material, char* quantity);
int nameContains(Material* material, char* string);
int less(Material* x, Material* y);
int greater(Material* x, Material* y);

Material* copyMaterial(Material* material);

//Tests
void testMaterial();
