
#ifndef CARELSDK_COMMON_H_
#define CARELSDK_COMMON_H_

#include "app/framework/include/af.h"
#include "list.h"
#include "include/mfglib.h"
#include <stdint.h>
#include "_carelSDK/crc16.h"

#include "control.h"
#include "stkTokenOperate.h"

#include "GeneralApp/key_scan.h"
#include "GeneralApp/led_blink.h"

#include "Drv/keyLed.h"

#include "App/framing.h"
#include "App/_zeroJoin.h"
#include "App/_zeroJoin_Route.h"

#if 0
#define _debug(...) { \
	emberAfDebugPrint("[--debug--]file: %s, func: %s, line: %d\n# ", __FILE__, __func__, __LINE__); \
	emberAfDebugPrintln(__VA_ARGS__); \
}
#endif

#define _debug	emberAfDebugPrintln
#define DEBUG	emberAfDebugPrintln

extern bool startup;

#endif /* CARELSDK_COMMON_H_ */
