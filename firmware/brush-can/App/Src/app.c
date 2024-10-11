#include "app.h"
#include "string.h"
#include "encoder.h"
#include "drv8876.h"
#include "pid.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim8;
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim17;

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;

extern FDCAN_HandleTypeDef hfdcan1;

// 8000 steps per second at full speed
// 20khz is good
static struct Encoder_Handle encoder1 = {.aPort = ECA1_GPIO_Port, .aPin = ECA1_Pin, .bPort = ECB1_GPIO_Port, .bPin = ECB1_Pin, .dir = 1};
static struct Encoder_Handle encoder2 = {.aPort = ECA2_GPIO_Port, .aPin = ECA2_Pin, .bPort = ECB2_GPIO_Port, .bPin = ECB2_Pin, .dir = 1};
static struct Encoder_Handle encoder3 = {.aPort = ECA3_GPIO_Port, .aPin = ECA3_Pin, .bPort = ECB3_GPIO_Port, .bPin = ECB3_Pin, .dir = 1};
static struct Encoder_Handle* encoders[3] = {&encoder1, &encoder2, &encoder3};

static struct Drv8876_Handle m1 = {.aTim = &htim2, .aChan = TIM_CHANNEL_4, .bTim = &htim2, .bChan = TIM_CHANNEL_3, .cTim = &htim2, .cChan = TIM_CHANNEL_1, .max = 100, .dir = 1};
static struct Drv8876_Handle m2 = {.aTim = &htim4, .aChan = TIM_CHANNEL_2, .bTim = &htim4, .bChan = TIM_CHANNEL_1, .cTim = &htim3, .cChan = TIM_CHANNEL_2, .max = 100, .dir = 1};
static struct Drv8876_Handle m3 = {.aTim = &htim3, .aChan = TIM_CHANNEL_1, .bTim = &htim2, .bChan = TIM_CHANNEL_2, .cTim = &htim8, .cChan = TIM_CHANNEL_1, .max = 100, .dir = 1};
static struct Drv8876_Handle* motors[3] = {&m1, &m2, &m3};

static struct Pid_Handle pid1 = {.kp = 0, .ki = 0, .kd = 0};
static struct Pid_Handle pid2 = {.kp = 0, .ki = 0, .kd = 0};
static struct Pid_Handle pid3 = {.kp = 0, .ki = 0, .kd = 0};
static struct Pid_Handle* pids[3] = {&pid1, &pid2, &pid3};

static uint16_t adc1Data[2] = {0};
static uint16_t adc2Data[2] = {0};
// static uint16_t* adcs[3] = {adc1Data, adc1Data+1, adc2Data};

static const uint16_t startId = 256;
static FDCAN_TxHeaderTypeDef txHeader = {
	.IdType = FDCAN_STANDARD_ID,
	.TxFrameType = FDCAN_DATA_FRAME,
	.ErrorStateIndicator = FDCAN_ESI_PASSIVE,
	.BitRateSwitch = FDCAN_BRS_OFF,
	.FDFormat = FDCAN_CLASSIC_CAN,
	.TxEventFifoControl = FDCAN_NO_TX_EVENTS,
	.MessageMarker = 0,
};
static FDCAN_RxHeaderTypeDef rxHeader;
static uint8_t rxData[8];

static FDCAN_FilterTypeDef filter = {
	.IdType = FDCAN_STANDARD_ID,
	.FilterIndex = 0,
	.FilterType = FDCAN_FILTER_RANGE,
	.FilterConfig = FDCAN_FILTER_TO_RXFIFO0,
	.FilterID1 = startId,
	.FilterID2 = startId+2,
};

static void app_parse_frame(void) {
	const uint16_t frameId = rxHeader.Identifier;
	const uint16_t motor = frameId-startId;
	if (rxHeader.RxFrameType == FDCAN_REMOTE_FRAME) {
		// request current
		txHeader.Identifier = frameId;
		txHeader.DataLength = FDCAN_DLC_BYTES_4;
		HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &txHeader, (uint8_t*)&(encoders[motor]->pos));

		// txHeader.DataLength = FDCAN_DLC_BYTES_2;
		// HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &txHeader, (uint8_t*)(adc2Data+1));
	}else{
		float val;
		memcpy((uint8_t*)&val, rxData+1, 4);
		// command
		switch (rxData[0]) {
		case 0: // current limit
			Drv8876_SetCurrent(motors[motor], (uint8_t)val);
			break;
		case 1: // target
			pids[motor]->target = val;
			break;
		case 2: // P
			pids[motor]->kp = val;
			break;
		case 3: // I
			pids[motor]->ki = val;
			break;
		case 4: // D
			pids[motor]->kd = val;
			break;
		case 5:
			encoders[motor]->pos = val;
			break;
		case 6:
			motors[motor]->dir = val>0;
			encoders[motor]->dir = val>0;
			encoders[motor]->pos = -encoders[motor]->pos;
			break;
		}
	}
}

void App_Init(void) {
    Encoder_Init(&encoder1);
    Encoder_Init(&encoder2);
    Encoder_Init(&encoder3);

    Drv8876_Init(&m1);
    Drv8876_Init(&m2);
    Drv8876_Init(&m3);

	Pid_Init(&pid1);
	Pid_Init(&pid2);
	Pid_Init(&pid3);

	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc1Data, 2);
	HAL_ADC_Start_DMA(&hadc2, (uint32_t*)adc2Data, 2);

	HAL_FDCAN_ConfigGlobalFilter(&hfdcan1, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_REJECT_REMOTE);
	HAL_FDCAN_ConfigFilter(&hfdcan1, &filter);

    HAL_FDCAN_Start(&hfdcan1);
	HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);

	HAL_TIM_Base_Start_IT(&htim16);
	HAL_TIM_Base_Start_IT(&htim17);
}

void App_Update(void) {
	// do nothing
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {
	if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET) {
		HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rxHeader, rxData);

		app_parse_frame();

		HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim == &htim17) {
		Encoder_Update(&encoder1);
		Encoder_Update(&encoder2);
		Encoder_Update(&encoder3);
	} else {
		const float vm1 = Pid_Update(&pid1, (float)encoder1.pos);
		const float vm2 = Pid_Update(&pid2, (float)encoder2.pos);
		const float vm3 = Pid_Update(&pid3, (float)encoder3.pos);

    	Drv8876_SetVoltage(&m1, vm1);
    	Drv8876_SetVoltage(&m2, vm2);
    	Drv8876_SetVoltage(&m3, vm3);
	}
}