#include "../common.h"
#include <stdlib.h>


static _zeroJoin_GwCmdParse_t _GCP_ = {0};
static _zeroJoin_State_t _devState = _ZEROJOIN_ST_IDLE;
static uint8_t _zeroJoinFrame_UpCmdAck[128] = {0};
static _zeroJoin_GwCmd_t _zeroJoinFrame_DownCmd[128] = {0};		// 网关专用
static int8_t _zeroJoinGwCmdFrameParse(uint8_t *packet);
static int8_t _zeroJoin_SetStackToken(void);


static void _zeroJoinRxPacketCallBack(uint8_t *packet, uint8_t linkQuality, int8_t rssi);
static void _zeroDevInfoFramingAck(uint8_t *dat, uint8_t status);
static void _zeroDevInfoFramingCmd(uint8_t *dat, _zeroJoin_GwCmd_t *gwCmd);

EmberEventControl _zeroJoinTx_EC;
void _zeroJoinTx_EF(void){
	emberEventControlSetInactive(_zeroJoinTx_EC);
	uint8_t enabled = 1;
	static uint8_t _seq_ = 0;
	switch(_seq_){
	// 启动MFG-LIB
	case 0:
		halCommonSetToken(TOKEN_MFG_LIB_ENABLED, &enabled);
		mfglibStart(_zeroJoinRxPacketCallBack);
		mfglibSetOptions(1);											// CSMA Enabled
		mfglibSetChannel(_ZEROJOIN_CHANNEL);
		mfglibSetPower(0, _ZEROJOIN_POWER);
		_devState = _ZEROJOIN_ST_RUN_RX;
		_seq_ ++;
		break;

	// 等待RX收到有效值
	case 1:
		// to Write StackToken
		if(_devState == _ZEROJOIN_ST_RUN_RX_GETGWINFO){
			if(_zeroJoin_SetStackToken() == 0){
				// LED Off
				gLed_Inactive(1);
				gLed_SetStillState(1, 0);
				_devState = _ZEROJOIN_ST_RUN_SEARCHING_GW;
				// 写入Token
				uint8_t _wrBuf_ = 1;
				emberAfWriteManufacturerSpecificServerAttribute(1, 0xFC80, 0x0000, 0x1254, &_wrBuf_, 0x20);
				// 重启
				halInternalSysReset(RESET_SOFTWARE_REBOOT);
				for(;;);
				break;
			}
		}
#if 0
		// 网关测试
		{
			static uint16_t cnt_gwts = 0;
			if(++cnt_gwts >= 300){
				cnt_gwts = 0;
				_zeroJoin_GwCmd_t gwCmd = {
						.devEui64 = {0x81 ,0xEE ,0xDB ,0xFE ,0xFF ,0x5E ,0xCF ,0xD0},
						.gwEui64 = {0x8A ,0x31 ,0xF7 ,0xFE ,0xFF ,0x5E ,0xCF ,0xD0},
						.nodeId = 0x1234,
						.channel = 17,
						.nwkKey = {0xDF ,0xAA  ,0x8F ,0x58 ,0x59 ,0xF6 ,0x96 ,0xDC ,0xCE ,0x8B ,0xD1 ,0x36 ,0x52 ,0x11 ,0x59 ,0x06},
						.nwkKeySeq = 0x00,
						.panId = 0x7777,
				};
				_zeroDevInfoFramingCmd(_zeroJoinFrame_DownCmd, &gwCmd);
				mfglibSendPacket(_zeroJoinFrame_DownCmd, 0);
			}
		}
#endif
		break;



	default:
		break;
	}

	emberEventControlSetDelayMS(_zeroJoinTx_EC, 10);
}

static void _zeroJoinRxPacketCallBack(uint8_t *packet,
                            uint8_t linkQuality,
                            int8_t rssi){
	if(_devState != _ZEROJOIN_ST_RUN_RX){
		return;
	}

	uint8_t *ptrPacket = packet;

	// check: length
	if(*ptrPacket++ < _ZEROJOIN_PACKET_LENGTH_LIMIT){
		return;
	}

	// check: head
	if(*ptrPacket++ != 0x19){
		return;
	}
	if(*ptrPacket++ != 0x95){
		return;
	}
	if(*ptrPacket++ != 0x01){
		return;
	}
	if(*ptrPacket++ != 0x02){
		return;
	}


	_zeroJoin_Type_t _type_ = *ptrPacket++;
	switch(_type_){
	// Gateway DOWN-CMD
	case _ZEROJOIN_TYPE_DOWN_CMD:

		// TODO print
		emberAfDebugPrint("------get GWCMD: ");
		for(uint8_t i=0;i<=packet[0];i++){
			emberAfDebugPrint(" %X", packet[i]);
		}
		emberAfDebugPrintln("\n\n");
		//

		// Check Length
		if(packet[0] != _ZEROJOIN_PACKET_LENGTH_CMD){
			break;
		}

		// Check devEui64
		{
			ptrPacket += 2;
			EmberEUI64 _devEui64_;
			memcpy(_devEui64_, ptrPacket, EUI64_SIZE);
			EmberEUI64 _localEui64_;
			emberAfGetEui64(_localEui64_);
			bool _eq_eui64_ = true;
			for(uint8_t i = 0;i < EUI64_SIZE; i++){
				if(_devEui64_[i] != _localEui64_[i]){
					_eq_eui64_ = false;
					break;
				}
			}
			// Eui64 相同, 进行消息解析
			if(_eq_eui64_){
				// TODO 消息解析
//				DEBUG("_____cmd parse:");

				int8_t ret = _zeroJoinGwCmdFrameParse(packet);

				// Ack
				_zeroDevInfoFramingAck(_zeroJoinFrame_UpCmdAck, ret==0?0x00:0x01);
				mfglibSendPacket(_zeroJoinFrame_UpCmdAck, 0);
				DEBUG("_____ack!");

				// 解析成功，准备切回正常模式
				if(!ret){
					mfglibEnd();
					uint8_t enabled = 0;
					halCommonSetToken(TOKEN_MFG_LIB_ENABLED, &enabled);
					_devState = _ZEROJOIN_ST_RUN_RX_GETGWINFO;
				}

				//

			// Eui64 不同, 进行消息转发
			}else{
				DEBUG("_____routing cmd!");
				_zeroJoin_Routing(packet);
			}
		}

		//
		break;

	// Device UP-CMDACK
	case _ZEROJOIN_TYPE_UP_CMD_ACK:

		// Check Length
		if(packet[0] != _ZEROJOIN_PACKET_LENGTH_CMD_ACK){
			break;
		}

		{
			ptrPacket += 3;
			EmberEUI64 _eui64_;
			memcpy(_eui64_, ptrPacket, EUI64_SIZE);
			EmberEUI64 _localEui64_;
			emberAfGetEui64(_localEui64_);
			bool _eq_eui64_ = true;
			for(uint8_t i = 0;i < EUI64_SIZE; i++){
				if(_eui64_[i] != _localEui64_[i]){
					_eq_eui64_ = false;
					break;
				}
			}
			// Eui64 相同, 丢弃
			if(_eq_eui64_){
			// 不同，则转发
			}else{
				DEBUG("_____routing ack!");
				_zeroJoin_Routing(packet);
			}
		}

		break;

	default:
		break;
	}

	// Routing
//	_zeroJoin_Routing(packet);

}






/********************************************************************************
 * 函数：_startZeroJoin
 * 功能：启动工程入网模式，指示灯进入4Hz闪烁，并预先离网，其后启动zeroJoin事件
 * 参数：-
 * 返回：-
 */
void _startZeroJoin(void){
	emberLeaveNetwork();
	gLed_Active(1, 5, 0xFFFFFFFF);	// 4Hz
	emberEventControlSetDelayMS(_zeroJoinTx_EC, 100);
}


/********************************************************************************
 * 函数：_zeroJoinGwCmdFrameParse
 * 功能：网关CMD解析
 * 说明：调用前请过滤
 * 参数：packet - .
 * 返回：0 - 解析成功， -1 - 失败
 */
static int8_t _zeroJoinGwCmdFrameParse(uint8_t *packet){

	_GCP_.enable = 0;
	uint8_t *ptrDat = packet + 16;			// PTR -> pandId_L
	_GCP_.PandId = (uint16_t)ptrDat[0] | ((uint16_t)ptrDat[1] << 8);
	if(_GCP_.PandId == 0x0000){
		return -1;
	}
	ptrDat += 2;
	_GCP_.NodeId = (uint16_t)ptrDat[0] | ((uint16_t)ptrDat[1] << 8);
	if(_GCP_.NodeId == 0x0000){
		return -1;
	}
	ptrDat += 2;
	_GCP_.channel = *ptrDat++;
	memcpy(_GCP_.gwEUI64, ptrDat, 8);
	ptrDat += 8;
	memcpy(_GCP_.nwkKey, ptrDat, 16);
	ptrDat += 16;
	_GCP_.NWKKeySeq = *ptrDat;
	_GCP_.enable = 1;

	return 0;
}



/********************************************************************************
 * 函数：_zeroDevInfoFramingAck
 * 功能：设备上行ACK组帧
 * 说明：帧格式
 * 			FrameLength  +  Head  +  Type  +  Reserved  +  Status  +  EUI64  +  CRC16
 * 			FrameLength	: [18]				    							1-byte
 * 			Head 		: [0x19 0x95 0x01 0x02]								4-bytes
 * 			Type		: [0x00]											1-byte
 * 			Reserved	: ...												2-bytes
 * 			Status		: [.]												1-byte
 * 			EUI64		: [........]										8-bytes
 * 			CRC16		: [..]												2-bytes
 * 参数：dat -> 帧缓冲池
 * 返回：-
 */
static void _zeroDevInfoFramingAck(uint8_t *dat, uint8_t status){

	uint8_t *ptrDat = dat + 1;
	memset(dat, 0, 128);
	uint8_t bufLen = 0;

	// Head
	ptrDat[0] = 0x19;
	ptrDat[1] = 0x95;
	ptrDat[2] = 0x01;
	ptrDat[3] = 0x02;
	ptrDat += 4;
	bufLen += 4;

	// Type
	*ptrDat++ = _ZEROJOIN_TYPE_UP_CMD_ACK;
	bufLen += 1;

	// Reserved
	ptrDat += 2;
	bufLen += 2;

	// Status
	*ptrDat++ = status;
	bufLen += 1;

	// EUI64
	EmberEUI64 _rui64_ = {0};
	emberAfGetEui64(_rui64_);
	memcpy(ptrDat, _rui64_, EUI64_SIZE);
	ptrDat += EUI64_SIZE;
	bufLen += EUI64_SIZE;

	// CRC16
	*ptrDat++ = 0;
	*ptrDat = 0;
	bufLen += 2;

	// Length
	dat[0] = bufLen;
}



/********************************************************************************
 * 函数：_zeroDevInfoFramingCmd
 * 功能：设备下行CMD组帧
 * 说明：帧格式
 * 			FrameLength  +  Head  +  Type  +  Reserved  + \
 * 				 devEUI64  +  PandId  +  NodeId  +  Channel  +  gwEUI64(extPanId)  +  NWKKey  +  NWKKeySeq  +  CRC16
 * 			FrameLength	: [47]				    							1-byte
 * 			Head 		: [0x19 0x95 0x01 0x02]								4-bytes
 * 			Type		: [0x01]											1-byte
 * 			Reserved	: ...												2-bytes
 * 			devEUI64	: [........]										8-bytes
 * 			PandId		: [..]												2-bytes
 * 			NodeId		: [..]												2-bytes
 * 			Channel		: [.]												1-byte
 * 			gwEUI64		: [........]										8-bytes
 *			NWKKey		: [................]								16-bytes
 *			NWKKeySeq	: [.]												1-bytes
 * 			CRC16		: [..]												2-bytes
 * 参数：dat -> 帧缓冲池
 * 返回：-
 */
static void _zeroDevInfoFramingCmd(uint8_t *dat, _zeroJoin_GwCmd_t *gwCmd){

	uint8_t *ptrDat = dat + 1;
	memset(dat, 0, 128);
	uint8_t bufLen = 0;

	// Head
	ptrDat[0] = 0x19;
	ptrDat[1] = 0x95;
	ptrDat[2] = 0x01;
	ptrDat[3] = 0x02;
	ptrDat += 4;
	bufLen += 4;

	// Type
	*ptrDat++ = _ZEROJOIN_TYPE_DOWN_CMD;
	bufLen += 1;

	// Reserved
	ptrDat += 2;
	bufLen += 2;

	// devEUI64
	memcpy(ptrDat, gwCmd->devEui64, EUI64_SIZE);
	ptrDat += EUI64_SIZE;
	bufLen += EUI64_SIZE;

	// pandId
	*ptrDat++ = (uint8_t)(gwCmd->panId & 0xFF);
	*ptrDat++ = (uint8_t)((gwCmd->panId >> 8) & 0xFF);
	bufLen += 2;

	// nodeId
	*ptrDat++ = (uint8_t)(gwCmd->nodeId & 0xFF);
	*ptrDat++ = (uint8_t)((gwCmd->nodeId >> 8) & 0xFF);
	bufLen += 2;

	// Channel
	*ptrDat++ = gwCmd->channel;
	bufLen += 1;

	// gwEUI64(extPanId)
	memcpy(ptrDat, gwCmd->gwEui64, EUI64_SIZE);
	ptrDat += EUI64_SIZE;
	bufLen += EUI64_SIZE;

	// NWKKey
	memcpy(ptrDat, gwCmd->nwkKey, 16);
	ptrDat += 16;
	bufLen += 16;

	// NWKKeySeq
	*ptrDat++ = gwCmd->nwkKeySeq;
	bufLen += 1;

	// CRC16
	*ptrDat++ = 0;
	*ptrDat = 0;
	bufLen += 2;

	// Length
	dat[0] = bufLen;
}


uint16_t _getCountRandom(uint16_t fixed, uint16_t variable){
	uint16_t _buf_ = 0;
	_buf_ += fixed;
	_buf_ += (halCommonGetRandom() % variable) + 1;
	return _buf_;
}



/********************************************************************************
 * 函数：_zeroJoin_SetStackToken
 * 功能：强制写Stack Token
 * 参数：-
 * 返回：-
 */
static int8_t _zeroJoin_SetStackToken(void){

	if(_GCP_.enable == 0){
		return -1;
	}

	// _StackNodeData_
	tokTypeStackNodeData _StackNodeData_ = {0};
	memcpy(_StackNodeData_.extendedPanId, _GCP_.gwEUI64, 8);
	_StackNodeData_.nodeType = 0x02;
	_StackNodeData_.panId = _GCP_.PandId;
	_StackNodeData_.radioFreqChannel = _GCP_.channel;
	_StackNodeData_.radioTxPower = EMBER_AF_PLUGIN_NETWORK_STEERING_RADIO_TX_POWER;
	_StackNodeData_.stackProfile = 0x0002;
	_StackNodeData_.zigbeeNodeId = _GCP_.NodeId;

	_stkTokenSet_StackNodeData(&_StackNodeData_);

	// keys
	tokTypeStackKeys _Keys_ = {0};
	_Keys_.activeKeySeqNum = _GCP_.NWKKeySeq;
	memcpy(_Keys_.networkKey, _GCP_.nwkKey, 16);

	_stkTokenSet_StackKeys(&_Keys_);

	// TrustCenter
	tokTypeStackTrustCenter _TrustCenter_ = {
		.mode = 12388,
	};
	_stkTokenSet_TrustCenter(&_TrustCenter_);

	// NetworkManagement
	tokTypeStackNetworkManagement _NetworkManagement_ = {
		.activeChannels = 0x07FFF800,
		.managerNodeId = 0x0000,
		.updateId = 0x00
	};
	_stkTokenSet_NetworkManagement(&_NetworkManagement_);

	// StackParentInfo
	tokTypeStackParentInfo _StackParentInfo_ = {0};
	_StackParentInfo_.parentNodeId = 0x0000;
	memcpy(_StackParentInfo_.parentEui, _GCP_.gwEUI64, 8);

	_stkTokenSet_ParentInfo(&_StackParentInfo_);


	return 0;

}


/********************************************************************************
 * 函数：_zeroJoin_PrintStackToken
 * 功能：打印Stack Token
 * 参数：-
 * 返回：-
 */
void _zeroJoin_PrintStackToken(void){
	tokTypeStackNodeData _StackNodeData_;
	tokTypeStackKeys _keys_;
	tokTypeStackTrustCenter _TrustCenter_;
	tokTypeStackNetworkManagement _NetworkManagement_;
	tokTypeStackParentInfo _StackParentInfo_;
	tokTypeStackNonceCounter _NonceCounter_;

	// NodeData
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

	// Keys
	_stkTokenGet_StackKeys(&_keys_);
	emberAfDebugPrint("- networkKey:");
	for(uint8_t i=0;i<16;i++){
		emberAfDebugPrint(" %X", _keys_.networkKey[i]);
	}
	emberAfDebugPrintln("");
	emberAfDebugPrintln("- activeKeySeqNum: %d", _keys_.activeKeySeqNum);
	emberAfDebugPrintln("\n\n");

	// TrustCenter
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

	// NetworkManagement
	_stkTokenGet_NetworkManagement(&_NetworkManagement_);
	emberAfDebugPrintln("- activeChannels: 	0x%4X", _NetworkManagement_.activeChannels);
	emberAfDebugPrintln("- managerNodeId: 	0x%2X", _NetworkManagement_.managerNodeId);
	emberAfDebugPrintln("- updateId:		0x%X", _NetworkManagement_.updateId);
	emberAfDebugPrintln("\n\n");

	// ParentInfo
	_stkTokenGet_ParentInfo(&_StackParentInfo_);
	emberAfDebugPrint("- parentEui:");
	for(uint8_t i=0;i<8;i++){
		emberAfDebugPrint(" %X", _StackParentInfo_.parentEui[i]);
	}
	emberAfDebugPrintln("\n- parentNodeId:	0x%2X", _StackParentInfo_.parentNodeId);
	emberAfDebugPrintln("\n\n");

	// NonceCounter
	_stkTokenGet_NonceCounter(&_NonceCounter_);
	emberAfDebugPrintln("- NonceCounter:	%d", _NonceCounter_);
	emberAfDebugPrintln("\n\n");
}






















