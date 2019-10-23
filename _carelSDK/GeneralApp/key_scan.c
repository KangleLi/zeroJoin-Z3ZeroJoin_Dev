/*=======================================================================*\
* ------------------------------------------------------------------------|
* �ļ�		��_carelSDK/GeneralApp/keyscan.c
* ����		��
* ��������		��2018/11/15
* �����		��Carel.Li
* �޸�����		��2019/1
* �޸�����		���ܹ���д
* �汾		��v1.1
*-------------------------------------------------------------------------|
\*=======================================================================*/
#include "../common.h"
#include <stdlib.h>

// KEY�ṹ����
LIST(scan_keys);

EmberEventControl gAppKeyScan_EventControl;


/**************************************************************************|
 * ������	gKey_Add
 * ���ܣ�	���KEYԪ��
 * ���ڣ�	-
 * ������	num-ָ�����
 * 		func_get-״̬��ȡ�ӿ�
 * 		pressed_callback-���»ص��ӿ�
 * 		released_callback-�ͷŻص��ӿ�
 * 		pp_callback-��ѹ̬���ڻص��ӿ�
 * ���أ�	-
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
 * ������	getKeyItermPtr
 * ���ܣ�	-
 * ���ڣ�	-
 * ������	-
 * ���أ�	-
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
 * ������	gAppKeyScan_EventFunction
 * ���ܣ�	KEYɨ�账������
 * ���ڣ�	50ms
 * ������	-
 * ���أ�	-
 ***/
void gAppKeyScan_EventFunction(void){
	struct KEY_SCAN_t *keyItem;
	emberEventControlSetInactive(gAppKeyScan_EventControl);

	if(*scan_keys != NULL){
			for(keyItem = *scan_keys;;keyItem = keyItem->next){
				switch(keyItem->seq){
				// �ͷ�״̬
				case 0:
					keyItem->state = 0;
					// �ȴ�����
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
				// ����״̬
				case 1:
					keyItem->state = 1;
					if(++(keyItem->pcnt) >= 100000)
						keyItem->pcnt = 100000;
					// �ȴ��ͷ�
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
					// ���ڻص�
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
 * ������	startApp_Keyscan
 * ���ܣ�	��ʼ��Key��������Key����
 * ���ڣ�	-
 * ������	delay-������ʱ
 * ���أ�	-
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




