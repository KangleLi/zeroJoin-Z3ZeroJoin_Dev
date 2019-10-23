#include "./common.h"



//extern uint8_t status;
//extern uint16_t nodeId;
//extern uint16_t fwVer;
//extern uint8_t rssi;
//extern uint8_t lqi;
//extern uint8_t capability;
//extern uint8_t macId[8];
//extern uint8_t epNum;
//extern uint16_t fwVer;
//extern uint8_t modelId[32];
//extern uint8_t modelIdLen;
//extern uint8_t ep[6];
//extern uint16_t leaveNodeId;
//
//// 构建过程控制
//EmberEventControl _carel_buildingDevice_EC;
//void _carel_buildingDevice_EF(void){
//	emberEventControlSetInactive(_carel_buildingDevice_EC);
//
//	switch(status){
//	// 读active endpoint
//	case 1:
//		_debug("__[1] read active endpoint!");
//		{
//			uint8_t cmd[64];
//			uint8_t len;
//			memset(cmd, 0, 64);
//			len = sprintf(cmd, "zdo active 0x%x\r\n", nodeId);
//			emberProcessCommandString(cmd, len);
//		}
//		status = 2;
//		break;
//	// 读modelId
//	case 2:
//		_debug("__[3] read modelId!");
//		vcReadAttribute(nodeId, 1, ZCL_BASIC_CLUSTER_ID, ZCL_MODEL_IDENTIFIER_ATTRIBUTE_ID, 0);
//		status = 3;
//		break;
//	// 读fwVer
//	case 3:
//		_debug("__[5] read fwVer!");
//		{
//			uint8_t cmd[64];
//			uint8_t len;
//
//			memset(cmd, 0, 64);
//			len =  sprintf(cmd, "zcl mfg-code 0x%02x\r\n", EMBER_AF_MANUFACTURER_CODE);
//			emberProcessCommandString(cmd, len);
//
//			memset(cmd, 0, 64);
//			len = sprintf(cmd, "zcl simon read 0x%x 0x%x\r\n", 0x0005, 0x00FB);
//			emberProcessCommandString(cmd, len);
//
//			memset(cmd, 0, 64);
//			len = sprintf(cmd, "send 0x%x 0x%x 0x%x\r\n", nodeId, 1, 1);
//			emberProcessCommandString(cmd, len);
//
//			//
//			status = 4;
//		}
//		break;
//	// end
//	case 4:
//		{
//			char buffer[128];
//			uint8_t i = 0;
//			uint8_t j;
//			memset(buffer, 0, 128);
//			i = sprintf(buffer + i, "node-info ");
//			i += sprintf(buffer + i, "0x%04X ", fwVer);
//			i += sprintf(buffer + i, "0x%04X ", nodeId);
//			i += sprintf(buffer + i, "%02X%02X%02X%02X%02X%02X%02X%02X ",\
//					macId[7], macId[6], macId[5], macId[4], macId[3], macId[2], macId[1], macId[0]);
//			i += sprintf(buffer + i, "0x%02X ", modelIdLen);
//			if (modelIdLen > 0) {
//				i += sprintf(buffer + i, "%s ", modelId);
//			}
//			i += sprintf(buffer + i, "0x%02X ", rssi);
//			i += sprintf(buffer + i, "0x%02X", epNum);
//			for (j = 0; j < epNum; j++) {
//				i += sprintf(buffer + i, " 0x%02X", ep[j]);
//			}
//			status = 0;
//			emberAfCorePrintln("%s\r\n", buffer);
//			emberAfCorePrintln(" \r\n");
//			leaveNodeId = nodeId;
//			// 下一步，禁止入网
//			extern EmberEventControl _carel_buildingDevice_EC;
//			emberEventControlSetActive(_carel_buildingDevice_EC);
//		}
//		break;
//
//	// 关闭入网许可
//	case 0:
//		_debug("__[7] closing the network!");
//		emberAfPluginNetworkCreatorSecurityCloseNetwork();
//		break;
//
//	default:
//		status = 0;
//		break;
//	}
//
//
//}

#if 0
bool startup = true;

// 上电1s初始化
EmberEventControl _carel_MainInit_EC;
extern void _zeroJoin_SetStackToken(void);
//extern void carelRxHandler(uint8_t *packet, uint8_t linkQuality, int8_t rssi);
void _carel_MainInit_EF(void){
	emberEventControlSetInactive(_carel_MainInit_EC);

	tokTypeStackNetworkManagement _NetworkManagement_;
	tokTypeStackTrustCenter _TrustCenter_;
	tokTypeStackKeys _keys_;
	tokTypeStackNodeData _StackNodeData_;
	static uint8_t seq_mainInit = 0;
//	uint8_t enabled = 1;
	switch(seq_mainInit){
	case 0:
		// 点亮LED
		gLed_SetStillState(1, 1);
#if 0
		// 使能mfglib
		halCommonSetToken(TOKEN_MFG_LIB_ENABLED, &enabled);
		// 启动mfglib
//		emberAfMfglibStart(true);
		mfglibStart(carelRxHandler);
		// 开启csma
		mfglibSetOptions(1);

#endif
		seq_mainInit ++;


		// 2s后进去下一序列
		emberEventControlSetDelayMS(_carel_MainInit_EC, 2000);
		break;

	case 1:
		// TS
//		_zeroJoin_SetStackToken();

		// get
		_stkTokenGet_StackNodeData(&_StackNodeData_);
		emberAfDebugPrintln("- pandid: 			0x%2X", _StackNodeData_.panId);
		emberAfDebugPrintln("- radioTxPower: 	0x%2X", _StackNodeData_.radioTxPower);
		emberAfDebugPrintln("- radioFreqChannel:0x%2X", _StackNodeData_.radioFreqChannel);
		emberAfDebugPrintln("- stackProfile: 	0x%2X", _StackNodeData_.stackProfile);

		emberAfDebugPrintln("- nodeType: 		0x%2X", _StackNodeData_.nodeType);
		emberAfDebugPrintln("- zigbeeNodeId: 	0x%2X", _StackNodeData_.zigbeeNodeId);
		emberAfDebugPrint("- extendedPanId:");
		for(uint8_t i=0;i<8;i++){
			emberAfDebugPrint(" %X", _StackNodeData_.extendedPanId[i]);
		}
		emberAfDebugPrintln("\n\n");

		// set ts
		_StackNodeData_.nodeType = 2;
		_stkTokenSet_StackNodeData(&_StackNodeData_);


		// get key
		memset(&_keys_, 0, sizeof(_keys_));
		_stkTokenGet_StackKeys(&_keys_);
		emberAfDebugPrint("- networkKey:");
		for(uint8_t i=0;i<16;i++){
			emberAfDebugPrint(" %X", _keys_.networkKey[i]);
		}
		emberAfDebugPrintln("");
		emberAfDebugPrintln("- activeKeySeqNum: %d", _keys_.activeKeySeqNum);
		emberAfDebugPrintln("\n\n");

		// get
		_stkTokenGet_TrustCenter(&_TrustCenter_);
		emberAfDebugPrintln("- mode: %d", _TrustCenter_.mode);
		emberAfDebugPrint("- eui64:");
		for(uint8_t i=0;i<8;i++){
			emberAfDebugPrint(" %X", _TrustCenter_.eui64);
		}
		emberAfDebugPrintln("\n- key:");
		for(uint8_t i=0;i<16;i++){
			emberAfDebugPrint(" %X", _TrustCenter_.key);
		}
		emberAfDebugPrintln("\n\n");

		// get
		_stkTokenGet_NetworkManagement(&_NetworkManagement_);
		emberAfDebugPrintln("- activeChannels: 	0x%4X", _NetworkManagement_.activeChannels);
		emberAfDebugPrintln("- managerNodeId: 	0x%2X", _NetworkManagement_.managerNodeId);
		emberAfDebugPrintln("- updateId:		0x%X", _NetworkManagement_.updateId);
		emberAfDebugPrintln("\n\n");

		// get
		tokTypeStackParentInfo _StackParentInfo_;
		memset(&_StackParentInfo_, 0, sizeof(_StackParentInfo_));
		_stkTokenGet_ParentInfo(&_StackParentInfo_);
		emberAfDebugPrint("- parentEui:");
		for(uint8_t i=0;i<8;i++){
			emberAfDebugPrint(" %X", _StackParentInfo_.parentEui[i]);
		}
		emberAfDebugPrintln("\n- parentNodeId:	0x%2X", _StackParentInfo_.parentNodeId);
		emberAfDebugPrintln("\n\n");
#if 0
		// get
		tokTypeStackMultiPhyNwkInfo _MultiPhyNwkInfo_;
		_stkTokenGet_MultiPhyNwkInfo(&_MultiPhyNwkInfo_);
		emberAfDebugPrintln("- radioTxPower: 		0x%X", _MultiPhyNwkInfo_.radioTxPower);
		emberAfDebugPrintln("- radioFreqChannel:	0x%X", _MultiPhyNwkInfo_.radioFreqChannel);
		emberAfDebugPrintln("- pg28ChannelMask:		0x%4X", _MultiPhyNwkInfo_.pg28ChannelMask);
		emberAfDebugPrintln("- pg29ChannelMask: 	0x%4X", _MultiPhyNwkInfo_.pg29ChannelMask);
		emberAfDebugPrintln("- pg30ChannelMask: 	0x%4X", _MultiPhyNwkInfo_.pg30ChannelMask);
		emberAfDebugPrintln("- pg31ChannelMask: 	0x%4X", _MultiPhyNwkInfo_.pg31ChannelMask);
		emberAfDebugPrintln("- optionsMask: 		0x%X", _MultiPhyNwkInfo_.optionsMask);
		emberAfDebugPrintln("\n\n");
#endif
		// get NonceCounter
		tokTypeStackNonceCounter _NonceCounter_;
		_stkTokenGet_NonceCounter(&_NonceCounter_);
		emberAfDebugPrintln("- NonceCounter:	%d", _NonceCounter_);
		emberAfDebugPrintln("\n\n");

		startup = false;
		_debug("_____end");

		break;
	default:
		break;
	}


}
#endif
