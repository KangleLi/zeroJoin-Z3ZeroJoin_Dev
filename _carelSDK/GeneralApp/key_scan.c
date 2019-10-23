/*=======================================================================*\
* ------------------------------------------------------------------------|
* 文件		：_carelSDK/GeneralApp/keyscan.c
* 描述		：
* 创建日期		：2018/11/15
* 设计者		：Carel.Li
* 修改日期		：2019/1
* 修改内容		：架构重写
* 版本		：v1.1
*-------------------------------------------------------------------------|
\*=======================================================================*/
#include "../common.h"
#include <stdlib.h>

// KEY结构链表
LIST(scan_keys);

EmberEventControl gAppKeyScan_EventControl;


/**************************************************************************|
 * 函数：	gKey_Add
 * 功能：	添加KEY元素
 * 周期：	-
 * 参数：	num-指定编号
 * 		func_get-状态获取接口
 * 		pressed_callback-按下回调接口
 * 		released_callback-释放回调接口
 * 		pp_callback-按压态周期回调接口
 * 返回：	-
 ***/
void gKey_Add(uint8_t num, void *func_get, void *pressed_callback, \
		      void *released_callback, void *pp_callback){
	struct KEY_SCAN_t *itermKey = malloc(sizeof(struct KEY_SCAN_t));
	itermKey->num = num;
	itermKey->pcnt = 0;
	itermKey->state = 0;
	itermKey->seq = 0;
	itermKey->comcnt = 0;
	itermKey->func_get = (func_get_f)func_get;
	itermKey->pressed_callback = (pressed_callback_f)pressed_callback;
	itermKey->released_callback = (released_callback_f)released_callback;
	itermKey->pp_callback = (pp_callback_f)pp_callback;

	list_add(scan_keys, itermKey);
}

/**************************************************************************|
 * 函数：	getKeyItermPtr
 * 功能：	-
 * 周期：	-
 * 参数：	-
 * 返回：	-
 ***/
struct KEY_SCAN_t *getKeyItermPtr(uint8_t keyNum){
	struct KEY_SCAN_t *itermPtr;
	for(itermPtr = *scan_keys;;itermPtr=itermPtr->next){
		if(itermPtr->num == keyNum){
			return itermPtr;
		}

		if(itermPtr->next == NULL){
			break;
		}
	}
	return NULL;
}


/**************************************************************************|
 * 函数：	gAppKeyScan_EventFunction
 * 功能：	KEY扫描处理任务
 * 周期：	50ms
 * 参数：	-
 * 返回：	-
 ***/
void gAppKeyScan_EventFunction(void){
	struct KEY_SCAN_t *keyItem;
	emberEventControlSetInactive(gAppKeyScan_EventControl);

	if(*scan_keys != NULL){
			for(keyItem = *scan_keys;;keyItem = keyItem->next){
				switch(keyItem->seq){
				// 释放状态
				case 0:
					keyItem->state = 0;
					// 等待按下
					if(keyItem->func_get()){
						if(++(keyItem->comcnt) >= DELAY_KEY_AS){
							keyItem->comcnt = 0;
							keyItem->pressed_callback();
							keyItem->seq ++;
							break;
						}
					}
					else{
						keyItem->comcnt = 0;
					}
					break;
				// 按下状态
				case 1:
					keyItem->state = 1;
					if(++(keyItem->pcnt) >= 100000)
						keyItem->pcnt = 100000;
					// 等待释放
					if(!keyItem->func_get()){
						if(++(keyItem->comcnt) >= DELAY_KEY_AS){
							keyItem->comcnt = 0;
							keyItem->released_callback(keyItem->pcnt * PERIOD_KEY_SCAN);
							keyItem->pcnt = 0;
							keyItem->seq --;
							break;
						}
					}
					else{
						keyItem->comcnt = 0;
					}
					// 周期回调
					keyItem->pp_callback(keyItem->pcnt * PERIOD_KEY_SCAN);
					break;
				default:
					keyItem->seq = 0;
					keyItem->pcnt = 0;
					keyItem->comcnt = 0;
					break;
				}


				if(keyItem->next == NULL){
					break;
				}
			}
	}

	emberEventControlSetDelayMS(gAppKeyScan_EventControl,PERIOD_KEY_SCAN);
}


/**************************************************************************|
 * 函数：	startApp_Keyscan
 * 功能：	初始化Key链表，启动Key任务
 * 周期：	-
 * 参数：	delay-启动延时
 * 返回：	-
 ***/
void startApp_Keyscan(uint32_t delay){
	list_init(scan_keys);
	if(delay == 0){
		emberEventControlSetActive(gAppKeyScan_EventControl);
	}
	else{
		emberEventControlSetDelayMS(gAppKeyScan_EventControl,delay);
	}
}

void stopApp_Keyscan(void){
	emberEventControlSetInactive(gAppKeyScan_EventControl);
}




