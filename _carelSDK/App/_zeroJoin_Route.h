
#ifndef CARELSDK_APP__ZEROJOIN_ROUTE_H_
#define CARELSDK_APP__ZEROJOIN_ROUTE_H_


#define _ZEROJOIN_ROUTING_HISTORY_RMTIME		100				// 1s


typedef struct {
	uint8_t msg[128];
	uint16_t time;
	uint8_t enable;
} _zeroJoin_RoutingHistoryTable_t;


typedef struct {
	uint8_t msg[128];
	uint16_t rmTime;
	uint8_t enable;
} _zeroJoin_RoutingSeqTable_t;



void _zeroJoinRoute_Init(void);
int8_t _zeroJoinPacketCmp(uint8_t *p1, uint8_t *p2);
void _zeroJoin_Routing(uint8_t *packet);



#if 0
#define _ZEROJOIN_LIMIT_RHT_TIME				80				// 单位10ms,路由记录表维护限制时间
#define _ZEROJOIN_LIMIT_RST_TIME				10				// 单位10ms,路由序列表发送随机限制时间


typedef struct {
	EmberEUI64 eui64;
	uint8_t type;
	uint8_t curHop;
	uint8_t lmtHop;
	uint16_t time;
	uint8_t enable;
} _zeroJoin_RoutingHistoryTable_t;


typedef struct {
	uint8_t msg[128];
	uint16_t rmTime;
	uint8_t enable;
} _zeroJoin_RoutingSeqTable_t;

void _zeroJoin_Routing(uint8_t *packet);
void _zeroJoinRoute_Init(void);
#endif

#endif /* CARELSDK_APP__ZEROJOIN_ROUTE_H_ */
