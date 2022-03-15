
#include "repository.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>


MaterialRepo* createMaterialRepo(int capacity)
{
	MaterialRepo* materialRepo = (MaterialRepo*)malloc(sizeof(MaterialRepo));

	if (materialRepo == NULL)
		return NULL;

	materialRepo->data = createDynamicArray(capacity, &destroyMaterial);

	return materialRepo;
}

void destroyMaterialRepo(MaterialRepo* materialRepo)
{
	if (materialRepo == NULL)
		return;

	destroyDynamicArray(materialRepo->data);
	free(materialRepo);
}

int getSize(MaterialRepo* materialRepo)
{
	if (materialRepo == NULL)
		return -1;

	return len(materialRepo->data);
}

int getMaterialPos(MaterialRepo* materialRepo, Material* material)
{
	if (materialRepo == NULL || material == NULL)
		return -1;

	for (int i = 0; i < len(materialRepo->data); i++)
	{
		Material* tmpMaterial = getElement(materialRepo->data, i);

		if (equalMaterials(tmpMaterial, material) == 1)
			return i;
	}
	return -1;
}

Material* getMaterialAtPos(MaterialRepo* materialRepo, int position)
{
	if (materialRepo == NULL)
		return NULL;
	if (position < 0 || position > getSize(materialRepo) - 1)
		return NULL;

	return getElement(materialRepo->data, position);
}

int findMaterial(MaterialRepo* materialRepo, Material* material)
{
	if (materialRepo == NULL || material == NULL)
		return -1;

	for (int i = 0; i < len(materialRepo->data); i++)
	{
		Material* tmpMaterial = getElement(materialRepo->data, i);
		if (equalMaterials(tmpMaterial, material) == 1)
			return 1;
	}
	return 0;
}

int addMaterial(MaterialRepo* materialRepo, Material* material)
{
	if (materialRepo == NULL || material == NULL)
		return -1;

	int materialPosition = getMaterialPos(materialRepo, material);
	if (materialPosition != -1)
	{
		Material* tmpMaterial = getElement(materialRepo->data, materialPosition);
		material->quantity += getQuantity(tmpMaterial);
		return upd(materialRepo->data, materialPosition, material);
	}

	return apd(materialRepo->data, material);
}

int updateMaterial(MaterialRepo* materialRepo, Material* material, Material* updatedMaterial)
{
	if (materialRepo == NULL || material == NULL || updatedMaterial == NULL)
		return -1;

	int materialPosition = getMaterialPos(materialRepo, material);
	if (materialPosition == -1)
		return -1;

	return upd(materialRepo->data, materialPosition, updatedMaterial);
}

int removeMaterial(MaterialRepo* materialRepo, Material* material)
{
	if (materialRepo == NULL || material == NULL)
		return -1;

	int materialPosition = getMaterialPos(materialRepo, material);
	if (materialPosition == -1)
		return -1;

	return del(materialRepo->data, materialPosition);
}

MaterialRepo* copyMaterialRepo(MaterialRepo* materialRepo)
{
	if (materialRepo == NULL)
		return NULL;

	int capacity;
	if (getSize(materialRepo) == 0)
		capacity = 2;
	else
		capacity = getSize(materialRepo);

	MaterialRepo* materialRepoCopy = createMaterialRepo(capacity);

	for (int i = 0; i < getSize(materialRepo); i++)
	{
		Material* material = getMaterialAtPos(materialRepo, i);
		Material* materialCopy = copyMaterial(material);
		addMaterial(materialRepoCopy, materialCopy);
	}

	return materialRepoCopy;
}


//Tests


void testCreateMaterialRepo()
{
	MaterialRepo* testMaterialRepo = createMaterialRepo(10);

	assert(testMaterialRepo != NULL);

	destroyMaterialRepo(testMaterialRepo);
}

void testMaterialRepoGetters()
{
	MaterialRepo* testMaterialRepo = createMaterialRepo(10);

	assert(getSize(NULL) == -1);
	assert(getSize(testMaterialRepo) == 0);

	Date* testDate1 = createDate(1, 2, 3);
	Date* testDate2 = createDate(3, 2, 1);

	Material* testMaterial1 = createMaterial("testName", "testSupplier", 12.34, testDate1);
	Material* testMaterial2 = createMaterial("ohterName", "otherSupplier", 12.34, testDate2);

	addMaterial(testMaterialRepo, testMaterial1);

	assert(getMaterialPos(NULL, NULL) == -1);
	assert(getMaterialPos(testMaterialRepo, testMaterial2) == -1);
	assert(getSize(testMaterialRepo) == 1);

	addMaterial(testMaterialRepo, testMaterial2);

	assert(getMaterialPos(testMaterialRepo, testMaterial1) == 0);
	assert(getMaterialPos(testMaterialRepo, testMaterial2) == 1);

	assert(getMaterialAtPos(testMaterialRepo, 0) == testMaterial1);
	assert(getMaterialAtPos(testMaterialRepo, 1) == testMaterial2);

	//destroyMaterial(testMaterial2);
	destroyMaterialRepo(testMaterialRepo);

}

void testFindMaterial()
{
	MaterialRepo* testMaterialRepo = createMaterialRepo(10);

	Date* testDate1 = createDate(1, 2, 3);
	Date* testDate2 = createDate(3, 2, 1);

	Material* testMaterial1 = createMaterial("testName", "testSupplier", 12.34, testDate1);
	Material* testMaterial2 = createMaterial("ohterName", "otherSupplier", 12.34, testDate2);

	addMaterial(testMaterialRepo, testMaterial1);

	assert(findMaterial(testMaterialRepo, NULL) == -1);
	assert(findMaterial(testMaterialRepo, testMaterial1) == 1);
	assert(findMaterial(testMaterialRepo, testMaterial2) == 0);

	destroyMaterial(testMaterial2);
	destroyMaterialRepo(testMaterialRepo);
}

void testAddMaterial()
{
	MaterialRepo* testMaterialRepo = createMaterialRepo(1);

	Date* testDate1 = createDate(1, 2, 3);
	Date* testDate2 = createDate(3, 2, 1);
	Date* testDate3 = createDate(3, 2, 1);

	Material* testMaterial1 = createMaterial("testName", "testSupplier", 12.34, testDate1);
	Material* testMaterial2 = createMaterial("ohterName", "otherSupplier", 12.34, testDate2);
	Material* testMaterial3 = createMaterial("ohterName", "otherSupplier", 7.66, testDate3);

	assert(addMaterial(NULL, testMaterial1) == -1);
	assert(getSize(testMaterialRepo) == 0);

	assert(addMaterial(testMaterialRepo, testMaterial1) == 1);
	assert(getSize(testMaterialRepo) == 1);

	assert(addMaterial(testMaterialRepo, testMaterial2) == 1);
	assert(getSize(testMaterialRepo) == 2);

	assert(addMaterial(testMaterialRepo, testMaterial3) == 1);
	assert(getSize(testMaterialRepo) == 2);
	assert(getQuantity(testMaterial3) == (double)20);

	destroyMaterialRepo(testMaterialRepo);
}

void testUpdateMaterial()
{
	MaterialRepo* testMaterialRepo = createMaterialRepo(1);

	Date* testDate1 = createDate(1, 2, 3);
	Date* testDate2 = createDate(3, 2, 1);
	Date* testDate3 = createDate(3, 2, 1);

	Material* testMaterial1 = createMaterial("testName", "testSupplier", 12.34, testDate1);
	Material* testMaterial2 = createMaterial("ohterName", "otherSupplier", 12.34, testDate2);
	Material* testMaterial3 = createMaterial("anohterName", "anotherSupplier", 7.66, testDate3);

	addMaterial(testMaterialRepo, testMaterial1);

	assert(updateMaterial(testMaterialRepo, testMaterial1, NULL) == -1);
	assert(updateMaterial(testMaterialRepo, testMaterial1, testMaterial3) == 1);
	assert(updateMaterial(testMaterialRepo, testMaterial3, testMaterial2) == 1);
	
	destroyMaterialRepo(testMaterialRepo);
}

void testRemoveMaterial()
{
	MaterialRepo* testMaterialRepo = createMaterialRepo(1);

	Date* testDate1 = createDate(1, 2, 3);
	Date* testDate2 = createDate(3, 2, 1);
	Date* testDate3 = createDate(3, 2, 1);
	Date* testDate4 = createDate(4, 3, 2);

	Material* testMaterial1 = createMaterial("testName", "testSupplier", 12.34, testDate1);
	Material* testMaterial2 = createMaterial("ohterName", "otherSupplier", 23.45, testDate2);
	Material* testMaterial3 = createMaterial("anohterName", "anotherSupplier", 7.66, testDate3);
	Material* testMaterial4 = createMaterial("asdfName", "asdfSupplier", 34.56, testDate4);

	addMaterial(testMaterialRepo, testMaterial1);
	addMaterial(testMaterialRepo, testMaterial2);
	addMaterial(testMaterialRepo, testMaterial3);
	
	assert(removeMaterial(testMaterialRepo, NULL) == -1);

	assert(removeMaterial(testMaterialRepo, testMaterial1) == 1);
	assert(getSize(testMaterialRepo) == 2);


	assert(removeMaterial(testMaterialRepo, testMaterial2) == 1);
	assert(getSize(testMaterialRepo) == 1);

	assert(removeMaterial(testMaterialRepo, testMaterial4) == -1);
	assert(getSize(testMaterialRepo) == 1);

	assert(removeMaterial(testMaterialRepo, testMaterial3) == 1);
	assert(getSize(testMaterialRepo) == 0);

	destroyMaterial(testMaterial4);
	destroyMaterialRepo(testMaterialRepo);
}

void testCopyMaterialRepo()
{
	MaterialRepo* testMaterialRepo = createMaterialRepo(1);

	assert(copyMaterialRepo(NULL) == NULL);

	Date* testDate1 = createDate(1, 2, 3);
	Date* testDate2 = createDate(3, 2, 1);
	Date* testDate3 = createDate(3, 2, 1);

	Material* testMaterial1 = createMaterial("testName", "testSupplier", 12.34, testDate1);
	Material* testMaterial2 = createMaterial("ohterName", "otherSupplier", 23.45, testDate2);
	Material* testMaterial3 = createMaterial("anohterName", "anotherSupplier", 7.66, testDate3);
	
	addMaterial(testMaterialRepo, testMaterial1);
	addMaterial(testMaterialRepo, testMaterial2);
	addMaterial(testMaterialRepo, testMaterial3);

	MaterialRepo* materialRepoCopy = copyMaterialRepo(testMaterialRepo);

	assert(materialRepoCopy != NULL);
	assert(getSize(testMaterialRepo) == getSize(materialRepoCopy));
	assert(equalMaterials(getMaterialAtPos(testMaterialRepo, 0), getMaterialAtPos(materialRepoCopy, 0)) == 1);
	assert(equalMaterials(getMaterialAtPos(testMaterialRepo, 1), getMaterialAtPos(materialRepoCopy, 1)) == 1);
	assert(equalMaterials(getMaterialAtPos(testMaterialRepo, 2), getMaterialAtPos(materialRepoCopy, 2)) == 1);

	destroyMaterialRepo(testMaterialRepo);
	destroyMaterialRepo(materialRepoCopy);
}

void testMaterialRepo()
{
	testCreateMaterialRepo();
	testMaterialRepoGetters();
	testFindMaterial();
	testAddMaterial();
	testUpdateMaterial();
	testRemoveMaterial();
	testCopyMaterialRepo();
}
