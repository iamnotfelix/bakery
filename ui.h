#pragma once

#include "services.h"

typedef struct UI
{
	MaterialServices* materialServices;
} UI;

UI* createUI(MaterialServices* materialServices);
void destroyUI(UI* ui);
void start(UI* ui);
