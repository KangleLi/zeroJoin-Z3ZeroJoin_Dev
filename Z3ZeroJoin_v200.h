// This file is generated by Simplicity Studio.  Please do not edit manually.
//
//

// Enclosing macro to prevent multiple inclusion
#ifndef SILABS_ZNET_CONFIG
#define SILABS_ZNET_CONFIG


/**** Included Header Section ****/



// Networks
#define EM_AF_GENERATED_NETWORK_TYPES { \
  EM_AF_NETWORK_TYPE_ZIGBEE_PRO, /* Primary */ \
}
#define EM_AF_GENERATED_ZIGBEE_PRO_NETWORKS { \
  { \
    /* Primary */ \
    ZA_ROUTER, \
    EMBER_AF_SECURITY_PROFILE_Z3, \
  }, \
}
#define EM_AF_GENERATED_NETWORK_STRINGS  \
  "Primary (pro)", \
/**** ZCL Section ****/
#define ZA_PROMPT "Z3ZeroJoin_v200"
#define ZCL_USING_BASIC_CLUSTER_SERVER
#define ZCL_USING_RXTX_CLUSTER_SERVER
/**** Optional Attributes ****/
#define ZCL_USING_BASIC_CLUSTER_MODEL_IDENTIFIER_ATTRIBUTE 
#define ZCL_USING_RXTX_CLUSTER_ATTRIBUTE_MODE_ID_ATTRIBUTE 
#define ZCL_USING_RXTX_CLUSTER_ATTRIBUTE_CHANNEL_ID_ATTRIBUTE 
#define ZCL_USING_RXTX_CLUSTER_ATTRIBUTE_POWER_ID_ATTRIBUTE 
#define EMBER_AF_MANUFACTURER_CODE 0x1254
#define EMBER_AF_DEFAULT_RESPONSE_POLICY_ALWAYS

/**** Cluster endpoint counts ****/
#define EMBER_AF_BASIC_CLUSTER_SERVER_ENDPOINT_COUNT (1)
#define EMBER_AF_RXTX_CLUSTER_SERVER_ENDPOINT_COUNT (1)

/**** Cluster Endpoint Summaries ****/
#define EMBER_AF_MAX_SERVER_CLUSTER_COUNT (2)
#define EMBER_AF_MAX_CLIENT_CLUSTER_COUNT (0)
#define EMBER_AF_MAX_TOTAL_CLUSTER_COUNT (2)

/**** CLI Section ****/
#define EMBER_AF_GENERATE_CLI
#define EMBER_AF_ENABLE_CUSTOM_COMMANDS
#define EMBER_COMMAND_INTEPRETER_HAS_DESCRIPTION_FIELD

/**** Security Section ****/
#define EMBER_AF_HAS_SECURITY_PROFILE_Z3

/**** Network Section ****/
#define EMBER_SUPPORTED_NETWORKS (1)
#define EMBER_AF_NETWORK_INDEX_PRIMARY (0)
#define EMBER_AF_DEFAULT_NETWORK_INDEX EMBER_AF_NETWORK_INDEX_PRIMARY
#define EMBER_AF_HAS_ROUTER_NETWORK
#define EMBER_AF_HAS_RX_ON_WHEN_IDLE_NETWORK
#define EMBER_AF_TX_POWER_MODE EMBER_TX_POWER_MODE_USE_TOKEN

/**** Callback Section ****/
#define EMBER_CALLBACK_MAIN_INIT
#define EMBER_CALLBACK_MAIN_TICK
#define EMBER_CALLBACK_PRE_MSG
#define EMBER_CALLBACK_PRE_ZDO
#define EMBER_CALLBACK_UNUSED_PAN_ID_FOUND
#define EMBER_CALLBACK_SCAN_ERROR
#define EMBER_CALLBACK_FIND_UNUSED_PAN_ID_AND_FORM
#define EMBER_CALLBACK_START_SEARCH_FOR_JOINABLE_NETWORK
#define EMBER_CALLBACK_GET_FORM_AND_JOIN_EXTENDED_PAN_ID
#define EMBER_CALLBACK_SET_FORM_AND_JOIN_EXTENDED_PAN_ID
#define EMBER_CALLBACK_ENERGY_SCAN_RESULT
#define EMBER_CALLBACK_SCAN_COMPLETE
#define EMBER_CALLBACK_NETWORK_FOUND
#define EMBER_CALLBACK_MAC_FILTER_MATCH_MESSAGE
#define EMBER_APPLICATION_HAS_MAC_FILTER_MATCH_MESSAGE_HANDLER
#define EMBER_CALLBACK_EZSP_MAC_FILTER_MATCH_MESSAGE
#define EZSP_APPLICATION_HAS_MAC_FILTER_MATCH_HANDLER
#define EMBER_CALLBACK_INTERPAN_SEND_MESSAGE
#define EMBER_CALLBACK_COUNTER_HANDLER
#define EMBER_APPLICATION_HAS_COUNTER_HANDLER
#define EMBER_CALLBACK_EZSP_COUNTER_ROLLOVER_HANDLER
#define EZSP_APPLICATION_HAS_COUNTER_ROLLOVER_HANDLER
#define EMBER_CALLBACK_BASIC_CLUSTER_RESET_TO_FACTORY_DEFAULTS
/**** Debug printing section ****/

// Global switch
#define EMBER_AF_PRINT_ENABLE
// Individual areas
#define EMBER_AF_PRINT_CORE 0x0001
#define EMBER_AF_PRINT_DEBUG 0x0002
#define EMBER_AF_PRINT_BITS { 0x03 }
#define EMBER_AF_PRINT_NAMES { \
  "Core",\
  "Debug",\
  NULL\
}
#define EMBER_AF_PRINT_NAME_NUMBER 2


#define EMBER_AF_SUPPORT_COMMAND_DISCOVERY


// Generated plugin macros

// Use this macro to check if ADC plugin is included
#define EMBER_AF_PLUGIN_ADC

// Use this macro to check if AES-CMAC plugin is included
#define EMBER_AF_PLUGIN_AES_CMAC

// Use this macro to check if Antenna Stub plugin is included
#define EMBER_AF_PLUGIN_ANTENNA_STUB

// Use this macro to check if Basic Server Cluster plugin is included
#define EMBER_AF_PLUGIN_BASIC

// Use this macro to check if Binding Table Library plugin is included
#define EMBER_AF_PLUGIN_BINDING_TABLE_LIBRARY
// User options for plugin Binding Table Library
#define EMBER_BINDING_TABLE_SIZE 3

// Use this macro to check if CCM* Encryption plugin is included
#define EMBER_AF_PLUGIN_CCM_ENCRYPTION
// User options for plugin CCM* Encryption
#define EMBER_AF_PLUGIN_CCM_ENCRYPTION_SOFTWARE_CCM
#define USE_SOFTWARE_CCM

// Use this macro to check if Radio Coexistence Stub plugin is included
#define EMBER_AF_PLUGIN_COEXISTENCE_STUB

// Use this macro to check if Counters plugin is included
#define EMBER_AF_PLUGIN_COUNTERS
// User options for plugin Counters

// Use this macro to check if Ember Minimal Printf plugin is included
#define EMBER_AF_PLUGIN_EMBER_MINIMAL_PRINTF

// Use this macro to check if Form and Join Library plugin is included
#define EMBER_AF_PLUGIN_FORM_AND_JOIN

// Use this macro to check if HAL Library plugin is included
#define EMBER_AF_PLUGIN_HAL_LIBRARY

// Use this macro to check if Install Code Library plugin is included
#define EMBER_AF_PLUGIN_INSTALL_CODE_LIBRARY

// Use this macro to check if Interpan plugin is included
#define EMBER_AF_PLUGIN_INTERPAN
// User options for plugin Interpan
#define EMBER_AF_PLUGIN_INTERPAN_DELIVER_TO PRIMARY_ENDPOINT
#define EMBER_AF_PLUGIN_INTERPAN_DELIVER_TO_SPECIFIED_ENDPOINT_VALUE 1

// Use this macro to check if Manufacturing Library plugin is included
#define EMBER_AF_PLUGIN_MANUFACTURING_LIBRARY

// Use this macro to check if Manufacturing Library CLI plugin is included
#define EMBER_AF_PLUGIN_MANUFACTURING_LIBRARY_CLI
#define EZSP_APPLICATION_HAS_MFGLIB_HANDLER

// Use this macro to check if mbed TLS plugin is included
#define EMBER_AF_PLUGIN_MBEDTLS
// User options for plugin mbed TLS
#define EMBER_AF_PLUGIN_MBEDTLS_CONF_DEVICE_ACCELERATION
#define EMBER_AF_PLUGIN_MBEDTLS_CONF_DEVICE_ACCELERATION_APP

// Use this macro to check if Network Find plugin is included
#define EMBER_AF_PLUGIN_NETWORK_FIND
#define EMBER_AF_DISABLE_FORM_AND_JOIN_TICK
// User options for plugin Network Find
#define EMBER_AF_PLUGIN_NETWORK_FIND_CHANNEL_MASK 0x0318C800
#define EMBER_AF_PLUGIN_NETWORK_FIND_CUT_OFF_VALUE -48
#define EMBER_AF_PLUGIN_NETWORK_FIND_RADIO_TX_POWER 3
#define EMBER_AF_PLUGIN_NETWORK_FIND_EXTENDED_PAN_ID { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
#define EMBER_AF_PLUGIN_NETWORK_FIND_DURATION 5
#define EMBER_AF_PLUGIN_NETWORK_FIND_JOINABLE_SCAN_TIMEOUT_MINUTES 1

// Use this macro to check if Network Steering plugin is included
#define EMBER_AF_PLUGIN_NETWORK_STEERING
// User options for plugin Network Steering
#define EMBER_AF_PLUGIN_NETWORK_STEERING_CHANNEL_MASK 0x0318C800
#define EMBER_AF_PLUGIN_NETWORK_STEERING_RADIO_TX_POWER 19
#define EMBER_AF_PLUGIN_NETWORK_STEERING_SCAN_DURATION 5
#define EMBER_AF_PLUGIN_NETWORK_STEERING_COMMISSIONING_TIME_S 180

// Use this macro to check if RAIL Library plugin is included
#define EMBER_AF_PLUGIN_RAIL_LIBRARY
// User options for plugin RAIL Library
#define EMBER_AF_PLUGIN_RAIL_LIBRARY_RAILPHYDEF 1

// Use this macro to check if Scan Dispatch plugin is included
#define EMBER_AF_PLUGIN_SCAN_DISPATCH
// User options for plugin Scan Dispatch
#define EMBER_AF_PLUGIN_SCAN_DISPATCH_SCAN_QUEUE_SIZE 10

// Use this macro to check if Security Core Library plugin is included
#define EMBER_AF_PLUGIN_SECURITY_LIBRARY_CORE
// User options for plugin Security Core Library
#define EMBER_TRANSIENT_KEY_TIMEOUT_S 300

// Use this macro to check if Serial plugin is included
#define EMBER_AF_PLUGIN_SERIAL

// Use this macro to check if Simulated EEPROM version 1 Library plugin is included
#define EMBER_AF_PLUGIN_SIM_EEPROM1
// User options for plugin Simulated EEPROM version 1 Library

// Use this macro to check if Simple Main plugin is included
#define EMBER_AF_PLUGIN_SIMPLE_MAIN

// Use this macro to check if Strong Random plugin is included
#define EMBER_AF_PLUGIN_STRONG_RANDOM
// User options for plugin Strong Random
#define EMBER_AF_PLUGIN_STRONG_RANDOM_RADIO_PRNG
#define USE_RADIO_API_FOR_TRNG

// Use this macro to check if Update TC Link Key plugin is included
#define EMBER_AF_PLUGIN_UPDATE_TC_LINK_KEY
// User options for plugin Update TC Link Key
#define EMBER_AF_PLUGIN_UPDATE_TC_LINK_KEY_MAX_ATTEMPTS 3

// Use this macro to check if ZCL Framework Core plugin is included
#define EMBER_AF_PLUGIN_ZCL_FRAMEWORK_CORE
// User options for plugin ZCL Framework Core
#define EMBER_AF_PLUGIN_ZCL_FRAMEWORK_CORE_CLI_ENABLED
#define ZA_CLI_FULL

// Use this macro to check if ZigBee PRO Stack Library plugin is included
#define EMBER_AF_PLUGIN_ZIGBEE_PRO_LIBRARY
// User options for plugin ZigBee PRO Stack Library
#define EMBER_MAX_END_DEVICE_CHILDREN 6
#define EMBER_PACKET_BUFFER_COUNT 75
#define EMBER_END_DEVICE_KEEP_ALIVE_SUPPORT_MODE EMBER_KEEP_ALIVE_SUPPORT_ALL
#define EMBER_END_DEVICE_POLL_TIMEOUT MINUTES_256
#define EMBER_END_DEVICE_POLL_TIMEOUT_SHIFT 6
#define EMBER_LINK_POWER_DELTA_INTERVAL 300
#define EMBER_APS_UNICAST_MESSAGE_COUNT 10
#define EMBER_BROADCAST_TABLE_SIZE 15
#define EMBER_NEIGHBOR_TABLE_SIZE 16

// Use this macro to check if ZigBee Light Link Library plugin is included
#define EMBER_AF_PLUGIN_ZLL_LIBRARY
// User options for plugin ZigBee Light Link Library
#define EMBER_ZLL_GROUP_ADDRESSES 0
#define EMBER_ZLL_RSSI_THRESHOLD -40


// Generated API headers

// API adc-cortexm3 from ADC plugin
#define EMBER_AF_API_ADC_CORTEXM3 "../../../soft/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.6/platform/base/hal/plugin/adc/adc-cortexm3.h"

// API adc from ADC plugin
#define EMBER_AF_API_ADC "../../../soft/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.6/platform/base/hal/plugin/adc/adc.h"

// API aes-cmac from AES-CMAC plugin
#define EMBER_AF_API_AES_CMAC "../../../soft/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.6/protocol/zigbee/app/framework/plugin/aes-cmac/aes-cmac.h"

// API antenna from Antenna Stub plugin
#define EMBER_AF_API_ANTENNA "../../../soft/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.6/platform/base/hal/plugin/antenna/antenna.h"

// API coexistence from Radio Coexistence Stub plugin
#define EMBER_AF_API_COEXISTENCE "../../../soft/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.6/platform/radio/rail_lib/plugin/coexistence/protocol/ieee802154/coexistence-802154.h"

// API network-steering from Network Steering plugin
#define EMBER_AF_API_NETWORK_STEERING "../../../soft/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.6/protocol/zigbee/app/framework/plugin/network-steering/network-steering.h"

// API rail-library from RAIL Library plugin
#define EMBER_AF_API_RAIL_LIBRARY "../../../soft/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.6/platform/radio/rail_lib/common/rail.h"

// API scan-dispatch from Scan Dispatch plugin
#define EMBER_AF_API_SCAN_DISPATCH "../../../soft/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.6/protocol/zigbee/app/framework/plugin/scan-dispatch/scan-dispatch.h"

// API serial from Serial plugin
#define EMBER_AF_API_SERIAL "../../../soft/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.6/platform/base/hal/plugin/serial/serial.h"

// API update-tc-link-key from Update TC Link Key plugin
#define EMBER_AF_API_UPDATE_TC_LINK_KEY "../../../soft/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.6/protocol/zigbee/app/framework/plugin/update-tc-link-key/update-tc-link-key.h"

// API command-interpreter2 from ZCL Framework Core plugin
#define EMBER_AF_API_COMMAND_INTERPRETER2 "../../../soft/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.6/protocol/zigbee/app/util/serial/command-interpreter2.h"


// Custom macros
#ifdef COMMISSIONING_STATUS_LED
#undef COMMISSIONING_STATUS_LED
#endif
#define COMMISSIONING_STATUS_LED BOARDLED2

#ifdef ON_OFF_LIGHT_LED
#undef ON_OFF_LIGHT_LED
#endif
#define ON_OFF_LIGHT_LED BOARDLED2

#ifdef LED_BLINK_PERIOD_MS
#undef LED_BLINK_PERIOD_MS
#endif
#define LED_BLINK_PERIOD_MS 2000



#endif // SILABS_ZNET_CONFIG
