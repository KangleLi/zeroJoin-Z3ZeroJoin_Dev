
#ifndef CARELSDK_APP__ZEROJOIN_H_
#define CARELSDK_APP__ZEROJOIN_H_



#define _ZEROJOIN_CHANNEL						18
#define _ZEROJOIN_POWER							3
#define _ZEROJOIN_PACKET_LENGTH_LIMIT			15		// 综合包长度限制最小值
#define _ZEROJOIN_PACKET_LENGTH_CMD				47		// Down-GWCMD Length
#define _ZEROJOIN_PACKET_LENGTH_CMD_ACK			18		// Up-CMDAck Length


typedef struct {
	uint16_t PandId;
	uint16_t NodeId;
	uint8_t channel;
	EmberEUI64 gwEUI64;
	uint8_t nwkKey[16];
	uint8_t NWKKeySeq;
	uint8_t enable;
} _zeroJoin_GwCmdParse_t;


typedef enum {
	_ZEROJOIN_ST_IDLE,
	_ZEROJOIN_ST_RUN_RX,
	_ZEROJOIN_ST_RUN_RX_GETGWINFO,
	_ZEROJOIN_ST_RUN_SEARCHING_GW,
} _zeroJoin_State_t;


typedef enum {
	_ZEROJOIN_TYPE_DOWN_CMD,
	_ZEROJOIN_TYPE_UP_CMD_ACK,
} _zeroJoin_Type_t;

// 网关专用
typedef struct {
	EmberEUI64 devEui64;
	uint16_t panId;
	uint16_t nodeId;
	uint8_t channel;
	EmberEUI64 gwEui64;
	uint8_t nwkKey[16];
	uint8_t nwkKeySeq;
} _zeroJoin_GwCmd_t;

extern void _startZeroJoin(void);
extern void _zeroJoin_PrintStackToken(void);
extern uint16_t _getCountRandom(uint16_t fixed, uint16_t variable);

#endif /* CARELSDK_APP__ZEROJOIN_H_ */
