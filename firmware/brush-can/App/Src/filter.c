//#include "filter.h"
//#include "main.h"
//
//extern FMAC_HandleTypeDef hfmac;
//
//float filter_coef[FILTER_ORDER] = {1.0/FILTER_ORDER, 1.0/FILTER_ORDER, 1.0/FILTER_ORDER, 1.0/FILTER_ORDER, 1.0/FILTER_ORDER};
//
//void Filter_Init(void) {
//	if (HAL_FMAC_FilterConfig(&hfmac, filter_coef, FILTER_ORDER, FILTER_ORDER, 0, 0) != HAL_OK) {
//		Error_Handler();
//	}
//}
//
//void Filter_Load(uint16_t* data, uint8_t len) {
//
//}
//
//void Filter_Handler(void) {
//
//}
