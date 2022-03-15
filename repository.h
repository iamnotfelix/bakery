#pragma once

#include "material.h"
#include "dynamicArray.h"

typedef struct MaterialRepo
{
	DynamicArray* data;
} MaterialRepo;

MaterialRepo* createMaterialRepo(int capacity);
void destroyMaterialRepo(MaterialRepo* materialRepo);

int getSize(MaterialRepo* materialRepo);
int getMaterialPos(MaterialRepo* materialRepo, Material* material);
Material* getMaterialAtPos(MaterialRepo* materialRepo, int position);

int findMaterial(MaterialRepo* materialRepo, Material* material);
int addMaterial(MaterialRepo* materialRepo, Material* material);
int updateMaterial(MaterialRepo* materialRepo, Material* material, Material* updatedMaterial);
int removeMaterial(MaterialRepo* materialRepo, Material* material);

MaterialRepo* copyMaterialRepo(MaterialRepo* materialRepo);

//Tests
void testMaterialRepo();
