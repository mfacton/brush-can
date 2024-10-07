#include "command.h"
#include "can.h"
#include "inttypes.h"
#include "main.h"

extern FDCAN_HandleTypeDef hfdcan1;

static FDCAN_TxHeaderTypeDef tx_header;
static FDCAN_RxHeaderTypeDef rx_header;
static uint8_t rx_data[64];

//static const uint8_t data_sizes[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20,
//		24, 32, 48, 64 };

static inline void can_handle_message(void) {
	Command_Instruction(rx_header.Identifier, rx_data);
}

static inline void can_receive(void) {
	if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE,
			0) != HAL_OK) {
		Error_Handler();
	}
}

void Can_Init(void) {
	if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK) {
		Error_Handler();
	}

	can_receive();

	tx_header.IdType = FDCAN_STANDARD_ID;
	tx_header.TxFrameType = FDCAN_DATA_FRAME;
	tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	tx_header.BitRateSwitch = FDCAN_BRS_ON;
	tx_header.FDFormat = FDCAN_FD_CAN;
	tx_header.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	tx_header.MessageMarker = 0;
}

void Can_Transmit(uint16_t id, uint8_t *data, enum CanDataSize size) {
	tx_header.Identifier = id;
	tx_header.DataLength = size;
	if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &tx_header, data) != HAL_OK) {
		Error_Handler();
	}
}

void Can_Handler(void) {
	if (HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &rx_header,
			rx_data) != HAL_OK) {
		Error_Handler();
	}
	can_handle_message();
	can_receive();
}
