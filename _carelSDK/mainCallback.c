
#include "./common.h"


bool startup = true;
static bool _getGwResp_ = false;


// 应用层消息处理
boolean emberAfPreMessageReceivedCallback(EmberAfIncomingMessage* incomingMessage){
	if(incomingMessage->source == 0x0000){
		_getGwResp_ = true;
	}
	return false;
}

// ZDO消息处理
boolean emberAfPreZDOMessageReceivedCallback(EmberNodeId emberNodeId,
                                          EmberApsFrame* apsFrame,
                                          int8u* message,
                                          int16u length){
	switch(apsFrame->clusterId){
	case SIMPLE_DESCRIPTOR_RESPONSE:
		break;
	case MATCH_DESCRIPTORS_RESPONSE:
		break;
	case END_DEVICE_BIND_RESPONSE:
		break;
	case END_DEVICE_ANNOUNCE:
		break;
	case IEEE_ADDRESS_RESPONSE:
		break;
	case ACTIVE_ENDPOINTS_RESPONSE:
		break;
	case NODE_DESCRIPTOR_RESPONSE:
		break;
	default:
		break;
	}

	return false;
}



void emberAfMainTickCallback(void){
	static bool _f_startup = false;
	if(!_f_startup){
		_f_startup = true;

		startApp_Keyscan(0);
		startApp_LedBlink(0);

		// 注册KeyLed
		gLed_Add(1, LED1_Handler, 0);
		gKey_Add(1, KEY1_Get, KEY1_pdCB, KEY1_rdCB, KEY1_ppCB);

		// 点亮LED
		gLed_SetStillState(1, 1);

		// Routing
		_zeroJoinRoute_Init();

		// Debug
		EmberEUI64 _rui64_ = {0};
		emberAfGetEui64(_rui64_);
		emberAfDebugPrint("EUI64: ");
		emberAfDebugPrint(" %X", _rui64_[0]);
		emberAfDebugPrint(" %X", _rui64_[1]);
		emberAfDebugPrint(" %X", _rui64_[2]);
		emberAfDebugPrint(" %X", _rui64_[3]);
		emberAfDebugPrint(" %X", _rui64_[4]);
		emberAfDebugPrint(" %X", _rui64_[5]);
		emberAfDebugPrint(" %X", _rui64_[6]);
		emberAfDebugPrint(" %X", _rui64_[7]);
		_zeroJoin_PrintStackToken();

		// 启动初始化任务
//		extern EmberEventControl _carel_MainInit_EC;
//		emberEventControlSetDelayMS(_carel_MainInit_EC, 1000);

	}
}

// nothing
void emberAfMainInitCallback(void){
	_DrvInitKeyLed();
}




EmberEventControl _carel_MainInit_EC;
void _carel_MainInit_EF(void){
	emberEventControlSetInactive(_carel_MainInit_EC);

	static uint16_t cnt1 = 0;
	static uint16_t cnt2 = 0;
	uint8_t _rdBuf_;
	static uint8_t _seq_ = 0;

	switch(_seq_){
	// 模式判断
	case 0:
		emberAfReadManufacturerSpecificServerAttribute(1, 0xFC80, 0x0000, 0x1254, &_rdBuf_, 1);
		if(_rdBuf_ == 1){
			// LED 1Hz
			gLed_Active(1, 20, 0xFFFFFFFF);
			_seq_ ++;
		}else{
			_seq_ += 2;
		}
		// 写入Token
		{
			uint8_t _wrBuf_ = 0;
			emberAfWriteManufacturerSpecificServerAttribute(1, 0xFC80, 0x0000, 0x1254, &_wrBuf_, 0x20);
		}
		break;

	// 5min的时间去请求网关的链路
	case 1:
		// 检测是否收到网关消息包
		if(_getGwResp_){
			cnt1 = 0;
			cnt2 = 0;
			_seq_ ++;
			break;
		}

		// 每隔10s请求一次网关IEEE
		if(++cnt1 >= 100){
			cnt1 = 0;
			emberAfFindIeeeAddress(0x0000, NULL);
		}

		// 5min限制时间
		if(++cnt2 >= 3000){
			cnt1 = 0;
			cnt2 = 0;
			emberLeaveNetwork();
			_seq_ ++;
			break;
		}

		break;

	case 2:
		// 判断网络状态
		if(emberAfNetworkState() == EMBER_NO_NETWORK){
			// 未入网进入工程模式
			_startZeroJoin();

		}else{
			// 网络连通，进入正常模式 ， LED On
			gLed_Inactive(1);
			gLed_SetStillState(1, 1);
			startup = false;
			emberEventControlSetInactive(_carel_MainInit_EC);
		}

		return;

	default:
		break;
	}



	emberEventControlSetDelayMS(_carel_MainInit_EC, 100);

}


void emberAfOnOffClusterServerAttributeChangedCallback(int8u endpoint,
                                                       EmberAfAttributeId attributeId){
	if(attributeId != 0x0000){
		return;
	}
	uint8_t onoff = 0;
	if(!emberAfReadServerAttribute(endpoint, 6, 0, &onoff, 1)){
		gLed_SetStillState(endpoint, onoff);
	}
}
