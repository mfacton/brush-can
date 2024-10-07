#include "app.h"
#include "encoder.h"

extern TIM_HandleTypeDef htim2;

struct Encoder_Handle encoder = {.htim = &htim2};

void App_Init(void) {
    Encoder_Init(&encoder);
}

void App_Update(void) {
    
}