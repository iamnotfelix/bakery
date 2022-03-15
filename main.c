
#include "ui.h"
#include "validation.h"
#include "dynamicArray.h"

#include <stdio.h>
#include <crtdbg.h>

int main()
{
	testDate();
	testMaterial();
	testMaterialRepo();
	testMaterialServices();
	testValidation();
	testDynamicArray();
	//_CrtDumpMemoryLeaks();
	printf("Test ran successfully!\n\n");

	MaterialRepo* materialRepo = createMaterialRepo(10);
	MaterialServices* materialServices = createMaterialServices(materialRepo);
	UI* ui = createUI(materialServices);

	start(ui);

	destroyUI(ui);

	_CrtDumpMemoryLeaks();

	//printf("Press anything to continue...\n");
	//int tmp = getchar();

	return 0;
}