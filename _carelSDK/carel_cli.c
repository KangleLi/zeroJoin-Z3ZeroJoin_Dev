#include "common.h"
// u - uint8_t
// b - string
// v - uint16_t

extern uint8_t _st_rxtx;	// 0-idle, 1-rx, 2-tx
extern uint16_t _id_rxtx;
extern uint8_t _ch_rxtx;
extern int8_t _pw_rxtx;
extern uint16_t _total_tx_packets;
extern uint16_t _cnt_tx_packets;
extern uint16_t _cnt_rx_packets;


void carelTest_RXHandler(void){

	if(_st_rxtx == 1){
		_debug("rx is running! please stop Rx.");
		return;
	}
	else if(_st_rxtx == 2){
		_debug("tx is running! please stop Tx.");
		return;
	}

	_id_rxtx = (uint16_t)emberUnsignedCommandArgument(0);
	_ch_rxtx = (uint8_t)emberUnsignedCommandArgument(1);
	_pw_rxtx = (int8_t)emberSignedCommandArgument(2);

	// 设置信道
	EmberStatus status = mfglibSetChannel(_ch_rxtx);
	if(status){
		_debug("channel error.");
		return;
	}

	// 设置功率
	status = mfglibSetPower(0, _pw_rxtx);
	if(status){
		_debug("power error.");
		return;
	}

	_st_rxtx = 1;

	_debug("____________Rx!id: 0x%2X, ch: %d,  power: %d", _id_rxtx, _ch_rxtx, _pw_rxtx);
	_debug("");
}

void carelTest_TXHandler(void){

	if(_st_rxtx == 1){
		_debug("rx is running! please stop Rx.");
		return;
	}
	else if(_st_rxtx == 2){
		_debug("tx is running! please stop Tx.");
		return;
	}

	_id_rxtx = (uint16_t)emberUnsignedCommandArgument(0);
	_ch_rxtx = (uint8_t)emberUnsignedCommandArgument(1);
	_pw_rxtx = (int8_t)emberSignedCommandArgument(2);
	_total_tx_packets = (uint16_t)emberUnsignedCommandArgument(3);

	// 设置信道
	EmberStatus status = mfglibSetChannel(_ch_rxtx);
	if(status){
		_debug("channel error.");
		return;
	}

	// 设置功率
	status = mfglibSetPower(0, _pw_rxtx);
	if(status){
		_debug("power error.");
		return;
	}

	// 启动event
	_st_rxtx = 2;
	_cnt_tx_packets = 0;
	_cnt_rx_packets = 0;
	extern EmberEventControl _rxtx_test_EC;
	emberEventControlSetDelayMS(_rxtx_test_EC, 10);

	_debug("____________Tx!id: 0x%2X, ch: %d,  power: %d, num: %d",\
			_id_rxtx, _ch_rxtx, _pw_rxtx, _total_tx_packets);
	_debug("");
}

void carelTest_StopHandler(void){

	if(_st_rxtx == 1){
		_st_rxtx = 0;
		_debug("Rx stoped.");
		return;
	}
	else if(_st_rxtx == 2){
		_st_rxtx = 0;
		_debug("Tx stoped.");
	}
	else{
		_st_rxtx = 0;
		_debug("not running.");
		return;
	}

	extern EmberEventControl _rxtx_test_EC;
	emberEventControlSetInactive(_rxtx_test_EC);

	extern void _rxtx_printLosRate(void);
	_rxtx_printLosRate();
}

static const char * const carelTestRxArg[] = {
  "id.",
  "channel.",
  "power.",
  NULL
};

static const char * const carelTestTxArg[] = {
  "id.",
  "channel.",
  "power.",
  "the total of packets.",
  NULL
};

static EmberCommandEntry emberCommandCarelRxTxTest[] = {
  emberCommandEntryActionWithDetails("r", carelTest_RXHandler, "vus", "start the test in Rx Mode.", carelTestRxArg),
  emberCommandEntryActionWithDetails("t", carelTest_TXHandler, "vusv", "start the test in Tx Mode.", carelTestTxArg),
  emberCommandEntryActionWithDetails("stop", carelTest_StopHandler, "", "stop testing.", NULL),
  emberCommandEntryTerminator(),
};


EmberCommandEntry emberAfCustomCommands[] = {
	emberCommandEntrySubMenu("rxtx", emberCommandCarelRxTxTest, ""),
	emberCommandEntryTerminator(),
};






