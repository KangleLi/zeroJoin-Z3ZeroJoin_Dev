#include "common.h"

#define _LENGTH_BUFFER			64


static void _fillBuffer_(uint8_t* buff, uint8_t length, bool random);

uint8_t _st_rxtx = 0;	// 0-idle, 1-rx, 2-tx
uint16_t _id_rxtx = 0;
uint8_t _ch_rxtx = 0;
int8_t _pw_rxtx = 0;
uint16_t _total_tx_packets = 0;
uint16_t _cnt_tx_packets = 0;
uint16_t _cnt_rx_packets = 0;
uint8_t _rt_rssi = 0;
uint8_t _rt_linkQuality = 0;

// 打印当前丢包率
void _rxtx_printLosRate(void){
	uint16_t lost_packet = _cnt_tx_packets>_cnt_rx_packets?(_cnt_tx_packets-_cnt_rx_packets):0;
	uint16_t rate = (lost_packet*1000)/_cnt_tx_packets;
	_debug("--- Tx: %d, Rx: %d, LosRate: %d.%d%%, rssi: %d", \
				_cnt_tx_packets, _cnt_rx_packets, rate/10, rate%10, _rt_rssi);
}

EmberEventControl _rxtx_test_EC;
extern void carelTest_StopHandler(void);
void _rxtx_test_EF(void){
	emberEventControlSetInactive(_rxtx_test_EC);
	// 1(长度字节) + (_LENGTH_BUFFER-3)(内容) + 2(CRC)
	uint8_t sendBuff[_LENGTH_BUFFER];
	static uint16_t cnt_com1 = 0;
	switch(_st_rxtx){
	// rx
	case 1:
		break;

	// tx
	case 2:
		// 打印丢包率（0.5s周期）
		if(++cnt_com1 >= 50){
			cnt_com1 = 0;
			_rxtx_printLosRate();
		}

		// 发包
		_fillBuffer_(sendBuff, _LENGTH_BUFFER-1, true);					// 填充（第一字节填充为长度,此长度不包含第一字节，但包含2字节CRC）
		sendBuff[1] = (uint8_t)(_id_rxtx & 0xFF);						// ID, 2bytes
		sendBuff[2] = (uint8_t)((_id_rxtx >> 8) & 0xFF);
		sendBuff[3] = 0xFE;												// flag:Tx, 1byte
		sendBuff[4] = (uint8_t)((_cnt_tx_packets + 1) & 0xFF);			// seqNum, 2bytes
		sendBuff[5] = (uint8_t)(((_cnt_tx_packets + 1) >> 8) & 0xFF);

		if(!mfglibSendPacket(sendBuff, 0)){
			_cnt_tx_packets ++;
			if(_total_tx_packets == 0){
				// 持续发包
			}
			else{
				// 发包结束
				if(--_total_tx_packets == 0){
					carelTest_StopHandler();
					return;
				}
			}
		}

		break;

	default:
		_st_rxtx = 0;
		break;
	}

	emberEventControlSetDelayMS(_rxtx_test_EC, 10);
}


// 收包处理
void carelRxHandler(uint8_t *packet,
                            uint8_t linkQuality,
                            int8_t rssi){
	uint8_t sendPacket[126] = {0};
	MEMCOPY(sendPacket, packet, packet[0]+1);
	static uint16_t tscnt = 0;
	uint8_t len = packet[0];
	if(len < 6){
		return;
	}
	// Id校验
	uint16_t id = (uint16_t)packet[1] | ((uint16_t)packet[2] << 8);
	if(id != _id_rxtx){
		return ;
	}

	switch(_st_rxtx){
	// idle
	case 0:
		break;
	// rx (收到包后,改变标志位并转发)
	case 1:
		if(tscnt>0){
			tscnt --;
		}
//		if(!tscnt){
//			_debug("get, len:%d", len);
//			for(uint8_t i=0;i<_LENGTH_BUFFER;i++){
//				emberAfDebugPrint("%X ", packet[i]);
//			}
//			_debug("---");
//			tscnt = 100;
//		}
		// 校验：标志位
		if(sendPacket[3] != 0xFE){
//			_debug("__0000:%X", sendPacket[3]);
			return;
		}
		// 处理：改变标志位
		{
			sendPacket[3] = 0xEF;
		}

		mfglibSendPacket(sendPacket, 0);

		break;
	// tx (收到包后，统计丢包率)
	case 2:
		// 长度校验
		if(len != (_LENGTH_BUFFER-1)){
//			_debug("__0000:%X", len);
			return;
		}
		// 校验标志位
		if(packet[3] != 0xEF){
//			_debug("__1111:%X", packet[3]);
			return;
		}

		_cnt_rx_packets++;
		_rt_rssi = rssi;
		_rt_linkQuality = linkQuality;

		break;
	default:
		break;
	}
}


static void _fillBuffer_(uint8_t* buff, uint8_t length, bool random)
{
  uint8_t i;
  // length byte does not include itself. If the user asks for 10
  // bytes of packet this means 1 byte length, 7 bytes, and 2 bytes CRC
  // this example will have a length byte of 9, but 10 bytes will show
  // up on the receive side
  buff[0] = length;

  for (i = 1; i < length; i += 2) {
    // Two buffer elements per iteration to use both random bytes.
    if (random) {
      uint16_t randomNumber = emberGetPseudoRandomNumber();
      buff[i] = (uint8_t)(randomNumber & 0xFF);
      buff[i + 1] = (uint8_t)((randomNumber >> 8)) & 0xFF;
    } else {
      // Test pattern is ascending integers starting from 1.
      buff[i] = i;
      buff[i + 1] = i + 1;
    }
  }
}
