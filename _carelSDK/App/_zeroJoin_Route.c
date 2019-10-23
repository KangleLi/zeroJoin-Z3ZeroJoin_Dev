#include "../common.h"


static _zeroJoin_RoutingHistoryTable_t _RHT_[10] = {0};
static _zeroJoin_RoutingSeqTable_t _RST_[10] = {0};

static void _zeroJoin_RoutingHistoryTableAdd(uint8_t *packet);
static void _zeroJoin_RoutingSeqTableAdd(uint8_t *packet);


EmberEventControl _zeroJoinRoute_EC;
void _zeroJoinRoute_EF(void){
	emberEventControlSetInactive(_zeroJoinRoute_EC);

	// ·�ɼ�¼��ά��
	for(uint8_t i = 0;i < 10; i++){
		if(_RHT_[i].enable == 0){
			continue;
		}

		if(++_RHT_[i].time >= _ZEROJOIN_ROUTING_HISTORY_RMTIME){
			_RHT_[i].enable = 0;
		}
	}

	// ·�����б�ά��&����
	for(uint8_t i = 0;i < 10; i++){

		if(_RST_[i].enable == 0){
			continue;
		}

		if(--_RST_[i].rmTime == 0){
			_RST_[i].enable = 0;
			mfglibSendPacket(_RST_[i].msg, 0);
		}
	}

	// DEBUG
	bool _empty_ = true;
	for(uint8_t i = 0;i < 10; i++){
		if(_RST_[i].enable > 0){
			_empty_ = false;
			break;
		}
	}
	if(!_empty_){
		DEBUG("____RoutingSeqTable____");
		for(uint8_t i = 0;i < 10; i++){
			if(_RST_[i].enable == 0){
				continue;
			}
			emberAfDebugPrint("## %d ##   ", i);
			for(uint8_t j=0;j<=_RST_[i].msg[0];j++){
				emberAfDebugPrint(" %X", _RST_[i].msg[j]);
			}
			emberAfDebugPrintln("   rmTime: %d", _RST_[i].rmTime);
		}
		emberAfDebugPrintln("\n\n");
	}

	emberEventControlSetDelayMS(_zeroJoinRoute_EC, 10);
}


/********************************************************************************
 * ������_zeroJoinRoute_Init
 * ���ܣ�·�ɹ��ܳ�ʼ��
 * ������-
 * ���أ�-
 */
void _zeroJoinRoute_Init(void){
	memset(_RST_, 0, sizeof(_RST_));
	memset(_RHT_, 0, sizeof(_RHT_));
	emberEventControlSetDelayMS(_zeroJoinRoute_EC, 0);
}


/********************************************************************************
 * ������_zeroJoinPacketCmp
 * ���ܣ�packet�Ƚ�
 * ������p1 -> packet1�� p2 -> packet2��
 * ���أ�0 - ����packet��ͬ�� -1 - ��ͬ
 */
int8_t _zeroJoinPacketCmp(uint8_t *p1, uint8_t *p2){

	if(p1[0] != p2[0]){
		return -1;
	}

	bool _eq_ = true;
	for(uint8_t i = 1;i <= p1[0]; i++){
		if(p1[i] != p2[i]){
			_eq_ = false;
			break;
		}
	}

	if(!_eq_){
		return -1;
	}

	return 0;
}


/********************************************************************************
 * ������_zeroJoin_RoutingHistoryTableAdd
 * ���ܣ�·�ɼ�¼������㷨
 * ������packet -> ֡�����
 * ���أ�-
 */
static void _zeroJoin_RoutingHistoryTableAdd(uint8_t *packet){

	bool _findIdle_ = false;
	uint16_t maxTime = 0;
	uint8_t maxTime_Index = 0;
	for(uint8_t i = 0;i < 10; i++){
		// �ҵ���λ���������
		if(_RHT_[i].enable == 0){
			_findIdle_ = true;
			// add
			_RHT_[i].enable = 1;
			_RHT_[i].time = 0;
			memcpy(_RHT_[i].msg, packet, 128);
			break;
		}

		if(_RHT_[i].time > maxTime){
			maxTime = _RHT_[i].time;
			maxTime_Index = i;
		}
	}

	// û���ҵ���λ,���滻�����ʱ����
	if(!_findIdle_){
		// add
		_RHT_[maxTime_Index].enable = 1;
		_RHT_[maxTime_Index].time = 0;
		memcpy(_RHT_[maxTime_Index].msg, packet, 128);
	}
}


/********************************************************************************
 * ������_zeroJoin_RoutingSeqTableAdd
 * ���ܣ�·�����б�����㷨
 * ������packet -> ֡�����
 * ���أ�-
 */
static void _zeroJoin_RoutingSeqTableAdd(uint8_t *packet){

	bool _findIdle_ = false;
	uint16_t maxTime = 0;
	uint8_t maxTime_Index = 0;
	for(uint8_t i = 0;i < 10; i++){
		// �ҵ���λ���������
		if(_RST_[i].enable == 0){
			_findIdle_ = true;
			// add
			_RST_[i].enable = 1;
			_RST_[i].rmTime = _getCountRandom(0, 10);
			memcpy(_RST_[i].msg, packet, 128);
			break;
		}

		if(_RST_[i].rmTime > maxTime){
			maxTime = _RST_[i].rmTime;
			maxTime_Index = i;
		}
	}

	// û���ҵ���λ,���滻�����ʱ����
	if(!_findIdle_){
		// add
		_RST_[maxTime_Index].enable = 1;
		_RST_[maxTime_Index].rmTime = _getCountRandom(0, 10);
		memcpy(_RST_[maxTime_Index].msg, packet, 128);
	}
}


/********************************************************************************
 * ������_zeroJoin_Routing
 * ���ܣ�packet·�ɴ���
 * ˵����packet���뱻���˹�
 * ������packet -> ֡�����
 * ���أ�-
 */
void _zeroJoin_Routing(uint8_t *packet){


	// ·�ɼ�¼�������ͬ��
	bool _eq_item_ = false;
	for(uint8_t i = 0;i < 10; i++){
		if(_RHT_[i].enable == 0){
			continue;
		}
		// ������ͬ��
		if(!_zeroJoinPacketCmp(_RHT_[i].msg, packet)){
			_eq_item_ = true;
			break;
		}
	}

	// �����Ѵ�����
	if(_eq_item_){
		return;
	}


	// ���·�ɼ�¼������
	_zeroJoin_RoutingHistoryTableAdd(packet);

	// ���·�����б�����
	_zeroJoin_RoutingSeqTableAdd(packet);
}





#if 0
// ·�����б�
static _zeroJoin_RoutingSeqTable_t _RST_[10] = {0};
// ·�ɼ�¼��
static _zeroJoin_RoutingHistoryTable_t _RHT_[10] = {0};


/********************************************************************************
 * ������_zeroJoinRoute_EF
 * ���ܣ�·��ά���¼�
 * ������-
 * ���أ�-
 */
EmberEventControl _zeroJoinRoute_EC;
void _zeroJoinRoute_EF(void){
	emberEventControlSetInactive(_zeroJoinRoute_EC);


	// ·�ɼ�¼��ά��
	for(uint8_t i = 0;i < 10; i++){
		if(_RHT_[i].enable == 0){
			continue;
		}

		if(++_RHT_[i].time >= _ZEROJOIN_LIMIT_RHT_TIME){
			_RHT_[i].enable = 0;
//			DEBUG("_routing history table --!");
		}
	}

	// ·�����б�ά��&����
	for(uint8_t i = 0;i < 10; i++){

		if(_RST_[i].enable == 0){
			continue;
		}

		if(--_RST_[i].rmTime == 0){
			_RST_[i].enable = 0;
			mfglibSendPacket(_RST_[i].msg, 0);
		}
	}

	// ts
	static uint16_t _cnt_ = 0;
	if(++_cnt_ >= 10){
		_cnt_ = 0;
		bool _has_val_ = false;
		for(uint8_t i = 0;i < 10; i++){
			if(_RST_[i].enable > 0){
				_has_val_ = true;
				continue;
			}
		}
		if(_has_val_){
			// print
			DEBUG("------seqTable------");
			for(uint8_t j = 0;j < 10; j++){
				if(_RST_[j].enable == 0){
					continue;
				}
				emberAfDebugPrintln("#[j]#");
				emberAfDebugPrint("msg: ");
				for(uint8_t m=0;m<=_RST_[j].msg[0];m++){
					emberAfDebugPrint(" %X", _RST_[j].msg[m]);
				}
				emberAfDebugPrintln("       emTime: %d", _RST_[j].rmTime);
			}
		}
	}
	//

	emberEventControlSetDelayMS(_zeroJoinRoute_EC, 10);
}


/********************************************************************************
 * ������_zeroJoinRoute_Init
 * ���ܣ�·�ɹ��ܳ�ʼ��
 * ������-
 * ���أ�-
 */
void _zeroJoinRoute_Init(void){
	memset(_RST_, 0, sizeof(_RST_));
	memset(_RHT_, 0, sizeof(_RHT_));
	emberEventControlSetDelayMS(_zeroJoinRoute_EC, 0);
}


/********************************************************************************
 * ������_zeroJoin_RoutingHistoryTableAdd
 * ���ܣ�·�ɼ�¼������㷨
 * ������packet -> ֡�����
 * ���أ�-
 */
static void _zeroJoin_RoutingHistoryTableAdd(uint8_t *packet){
	uint8_t *ptrPacket = packet + 5;		// point->Type
	uint8_t type = *ptrPacket++;
	uint8_t curHopCnt = *ptrPacket++;
	uint8_t lmtHopCnt = *ptrPacket++;		// point->eui64

	bool _findIdle_ = false;
	uint16_t maxTime = 0;
	uint8_t maxTime_Index = 0;
	for(uint8_t i = 0;i < 10; i++){
		// �ҵ���λ���������
		if(_RHT_[i].enable == 0){
			_findIdle_ = true;
			// add
			_RHT_[i].enable = 1;
			_RHT_[i].curHop = curHopCnt;
			_RHT_[i].lmtHop = lmtHopCnt;
			_RHT_[i].time = 0;
			_RHT_[i].type = type;
			memcpy(_RHT_[i].eui64, ptrPacket, EUI64_SIZE);
			break;
		}

		if(_RHT_[i].time > maxTime){
			maxTime = _RHT_[i].time;
			maxTime_Index = i;
		}
	}

	// û���ҵ���λ,���滻�����ʱ����
	if(!_findIdle_){
		// add
		_RHT_[maxTime_Index].enable = 1;
		_RHT_[maxTime_Index].curHop = curHopCnt;
		_RHT_[maxTime_Index].lmtHop = lmtHopCnt;
		_RHT_[maxTime_Index].time = 0;
		_RHT_[maxTime_Index].type = type;
		memcpy(_RHT_[maxTime_Index].eui64, ptrPacket, EUI64_SIZE);
	}

//	DEBUG("_routing history table ++!");
}


/********************************************************************************
 * ������_zeroJoin_RoutingSeqTableAdd
 * ���ܣ�·�����б�����㷨
 * ������packet -> ֡�����
 * ���أ�-
 */
static void _zeroJoin_RoutingSeqTableAdd(uint8_t *packet){
	uint8_t *ptrPacket = packet + 6;		// point->RouteInfo
	uint8_t curHopCnt = *ptrPacket++;
	uint8_t lmtHopCnt = *ptrPacket++;		// point->eui64
	curHopCnt = curHopCnt<lmtHopCnt?(curHopCnt+1):lmtHopCnt;	// ������һ
	EmberEUI64 _eui64_;
	memcpy(_eui64_, ptrPacket, EUI64_SIZE);

	// �ȶ�EUI & Type
	bool _eq_eui64_ = false;
	uint8_t _eq_eui64_index = 0;
	for(uint8_t i = 0;i < 10; i++){
		if(_RST_[i].enable == 0){
			continue;
		}
		_eq_eui64_ = true;
		for(uint8_t j = 0;j < EUI64_SIZE; j++){
			if(_RST_[i].msg[j+8] != _eui64_[j]){
				_eq_eui64_ = false;
				break;
			}
		}
		if(_eq_eui64_ && (_RST_[i].msg[5] == packet[5])){
			_eq_eui64_index = i;
			break;
		}else{
			_eq_eui64_ = false;
		}
	}

	// �ҵ���ͬEUI,�ȶ�����������
	if(_eq_eui64_){
		if(_RST_[_eq_eui64_index].msg[6] > curHopCnt){
			_RST_[_eq_eui64_index].msg[6] = curHopCnt;
			_RST_[_eq_eui64_index].msg[7] = lmtHopCnt;
			_RST_[_eq_eui64_index].rmTime = _getCountRandom(0, 10);
		}else{
			// ����
		}
	// û���ҵ�EUI,ֱ�����
	}else{
		bool finded = false;
		uint16_t max_rmTime = 0;
		uint8_t max_rmTime_Index = 0;
		for(uint8_t i = 0;i < 10; i++){
			// �ҵ���λ��ֱ�����
			if(_RST_[i].enable == 0){
				finded = true;
				_RST_[i].enable = 1;
				_RST_[i].rmTime = _getCountRandom(0, 10);
				memcpy(_RST_[i].msg, packet, 128);
				_RST_[i].msg[6] ++;	// ������һ
				break;
			}

			if(_RST_[i].rmTime > max_rmTime){
				max_rmTime = _RST_[i].rmTime;
				max_rmTime_Index = i;
			}
		}
		// û���ҵ������滻
		if(!finded){
			_RST_[max_rmTime_Index].enable = 1;
			_RST_[max_rmTime_Index].rmTime = _getCountRandom(0, 10);
			memcpy(_RST_[max_rmTime_Index].msg, packet, 128);
			_RST_[max_rmTime_Index].msg[6] ++;	// ������һ
		}
	}


}


/********************************************************************************
 * ������_zeroJoin_Routing
 * ���ܣ�packet·�ɴ���
 * ˵����	1.���������ﵽ���Ƶİ�
 * 			2.����·�ɼ�¼������ͬEui64��Type����ҵ�������������ȶԣ�
 * 				�滻��������������û�ҵ�������б�����ӣ�������㷨��
 * 			3.��2���ҵ���ͬeui����ҵ�ǰ������С������û���ҵ��������·�����б���Ӵ���
 * 			4.·�����б���ӣ�
 * 				4.1 ����·�����б�����ͬEui64��Type����ҵ�������������ȶԣ�����ѡ���Ը��£�
 * 					��û�ҵ�������б�����ӣ�������㷨��
 * ������packet -> ֡�����
 * ���أ�-
 */
void _zeroJoin_Routing(uint8_t *packet){

	uint8_t *ptrPacket = packet + 5;	// point->Type
	uint8_t type = *ptrPacket++;
	uint8_t curHopCnt = *ptrPacket++;
	uint8_t lmtHopCnt = *ptrPacket++;	// point->eui64

	// Hop Check
	if(curHopCnt >= lmtHopCnt){
		return;
	}

	EmberEUI64 _eui64_;
	memcpy(_eui64_, ptrPacket, EUI64_SIZE);

	bool _do_route_ = false;
	bool _eq_eui64 = false;
	uint8_t _eq_eui64_index = 0;

	// ����·�ɼ�¼��Ѱ����ͬeui64&type��
	for(uint8_t i = 0;i < 10; i++){
		if(_RHT_[i].enable == 0){
			continue;
		}

		_eq_eui64 = true;
		for(uint8_t j = 0;i < EUI64_SIZE; j++){
			if(_RHT_[i].eui64[j] != _eui64_[j]){
				_eq_eui64 = false;
				break;
			}
		}
		if(_eq_eui64 && (_RHT_[i].type == type)){
			_eq_eui64_index = i;
			break;
		}else{
			_eq_eui64 = false;
		}
	}

	if(_eq_eui64){
		// �ҵ���ӦEUI����ҵ�ǰ������С,�����滻����·��
		if(_RHT_[_eq_eui64_index].curHop > curHopCnt){
			_do_route_ = true;
			_RHT_[_eq_eui64_index].curHop = curHopCnt;			// �滻��
			_RHT_[_eq_eui64_index].time = 0;					// ���ô��ʱ��
//			DEBUG("[1] replace!");
		}
	}else{
		// û���ҵ����������·��
		_do_route_ = true;
//		DEBUG("[2] add!");
		_zeroJoin_RoutingHistoryTableAdd(packet);
	}

	// ·�����б����
	if(_do_route_){
//		DEBUG("[3] routing!");
		_zeroJoin_RoutingSeqTableAdd(packet);
	}

	curHopCnt++;
}
#endif










