
#ifndef CARELSDK_PROJECT_DIMSW_CH1_2_PORTS_H_
#define CARELSDK_PROJECT_DIMSW_CH1_2_PORTS_H_

// ����ӳ��
#define _DrvInitKeyLed()		{ \
	extern void KEY1_HalInit(void);	\
	extern void LED1_HalInit(void);	\
	KEY1_HalInit();	\
	LED1_HalInit();	\
}


void LED1_Handler(uint8_t newState);
uint8_t KEY1_Get(void);
void KEY1_pdCB(void);
void KEY1_rdCB(uint32_t tim);
void KEY1_ppCB(uint32_t tim);


#endif /* CARELSDK_PROJECT_DIMSW_CH1_2_PORTS_H_ */
