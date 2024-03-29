// This file is generated by Simplicity Studio.  Please do not edit manually.
//
//

// This file contains the tokens for attributes stored in flash


// Identifier tags for tokens
// Creator for attribute: model identifier, singleton.
#define CREATOR_MODEL_IDENTIFIER_SINGLETON 0xB000
#define NVM3KEY_MODEL_IDENTIFIER_SINGLETON ( NVM3KEY_DOMAIN_ZIGBEE | 0xB000 )
// Creator for attribute: on/off, endpoint: 1
#define CREATOR_ON_OFF_1 0xB001
#define NVM3KEY_ON_OFF_1 ( NVM3KEY_DOMAIN_ZIGBEE | 0xB001 )
// Creator for attribute: Mode, endpoint: 1
#define CREATOR_ATTRIBUTE_MODE_ID_1 0xB003
#define NVM3KEY_ATTRIBUTE_MODE_ID_1 ( NVM3KEY_DOMAIN_ZIGBEE | 0xB003 )
// Creator for attribute: Channel, endpoint: 1
#define CREATOR_ATTRIBUTE_CHANNEL_ID_1 0xB004
#define NVM3KEY_ATTRIBUTE_CHANNEL_ID_1 ( NVM3KEY_DOMAIN_ZIGBEE | 0xB004 )
// Creator for attribute: Power, endpoint: 1
#define CREATOR_ATTRIBUTE_POWER_ID_1 0xB005
#define NVM3KEY_ATTRIBUTE_POWER_ID_1 ( NVM3KEY_DOMAIN_ZIGBEE | 0xB005 )
// Creator for attribute: Header, endpoint: 1
#define CREATOR_ATTRIBUTE_HEADER_ID_1 0xB006
#define NVM3KEY_ATTRIBUTE_HEADER_ID_1 ( NVM3KEY_DOMAIN_ZIGBEE | 0xB006 )


// Types for the tokens
#ifdef DEFINETYPES
typedef uint8_t  tokType_on_off;
typedef uint8_t  tokType_current_level;
typedef uint16_t  tokType_start_up_color_temperature_mireds;
typedef int16_t  tokType_local_temperature;
typedef uint8_t  tokType_model_identifier[33];
typedef uint8_t  tokType_attribute_mode_id;
typedef uint8_t  tokType_attribute_channel_id;
typedef int8_t  tokType_attribute_power_id;
typedef uint16_t  tokType_attribute_header_id;
#endif // DEFINETYPES


// Actual token definitions
#ifdef DEFINETOKENS
DEFINE_BASIC_TOKEN(MODEL_IDENTIFIER_SINGLETON, tokType_model_identifier, {9,'Z','e','r','o','D','e','v','_','1',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0})
DEFINE_BASIC_TOKEN(ON_OFF_1, tokType_on_off, 0x00)
DEFINE_BASIC_TOKEN(ATTRIBUTE_MODE_ID_1, tokType_attribute_mode_id, 0x00)
DEFINE_BASIC_TOKEN(ATTRIBUTE_CHANNEL_ID_1, tokType_attribute_channel_id, 0x00)
DEFINE_BASIC_TOKEN(ATTRIBUTE_POWER_ID_1, tokType_attribute_power_id, 0x00)
DEFINE_BASIC_TOKEN(ATTRIBUTE_HEADER_ID_1, tokType_attribute_header_id, 0x0000)
#endif // DEFINETOKENS


// Macro snippet that loads all the attributes from tokens
#define GENERATED_TOKEN_LOADER(endpoint) do {\
  uint8_t ptr[33]; \
  uint8_t curNetwork = emberGetCurrentNetwork(); \
  uint8_t epNetwork; \
  halCommonGetToken((tokType_model_identifier *)ptr, TOKEN_MODEL_IDENTIFIER_SINGLETON); \
  emberAfWriteServerAttribute(1, ZCL_BASIC_CLUSTER_ID, ZCL_MODEL_IDENTIFIER_ATTRIBUTE_ID, (uint8_t*)ptr, ZCL_CHAR_STRING_ATTRIBUTE_TYPE); \
  epNetwork = emberAfNetworkIndexFromEndpoint(1); \
  if((endpoint) == 1 || ((endpoint) == EMBER_BROADCAST_ENDPOINT && epNetwork == curNetwork)) { \
    halCommonGetToken((tokType_on_off *)ptr, TOKEN_ON_OFF_1); \
    emberAfWriteServerAttribute(1, ZCL_ON_OFF_CLUSTER_ID, ZCL_ON_OFF_ATTRIBUTE_ID, (uint8_t*)ptr, ZCL_BOOLEAN_ATTRIBUTE_TYPE); \
    halCommonGetToken((tokType_attribute_mode_id *)ptr, TOKEN_ATTRIBUTE_MODE_ID_1); \
    emberAfWriteManufacturerSpecificServerAttribute(1, ZCL_RXTX_CLUSTER_ID, ZCL_ATTRIBUTE_MODE_ID_ATTRIBUTE_ID, 0x1254, (uint8_t*)ptr, ZCL_INT8U_ATTRIBUTE_TYPE); \
    halCommonGetToken((tokType_attribute_channel_id *)ptr, TOKEN_ATTRIBUTE_CHANNEL_ID_1); \
    emberAfWriteManufacturerSpecificServerAttribute(1, ZCL_RXTX_CLUSTER_ID, ZCL_ATTRIBUTE_CHANNEL_ID_ATTRIBUTE_ID, 0x1254, (uint8_t*)ptr, ZCL_INT8U_ATTRIBUTE_TYPE); \
    halCommonGetToken((tokType_attribute_power_id *)ptr, TOKEN_ATTRIBUTE_POWER_ID_1); \
    emberAfWriteManufacturerSpecificServerAttribute(1, ZCL_RXTX_CLUSTER_ID, ZCL_ATTRIBUTE_POWER_ID_ATTRIBUTE_ID, 0x1254, (uint8_t*)ptr, ZCL_INT8S_ATTRIBUTE_TYPE); \
    halCommonGetToken((tokType_attribute_header_id *)ptr, TOKEN_ATTRIBUTE_HEADER_ID_1); \
    emberAfWriteManufacturerSpecificServerAttribute(1, ZCL_RXTX_CLUSTER_ID, ZCL_ATTRIBUTE_HEADER_ID_ATTRIBUTE_ID, 0x1254, (uint8_t*)ptr, ZCL_INT16U_ATTRIBUTE_TYPE); \
  } \
} while(false)


// Macro snippet that saves the attribute to token
#define GENERATED_TOKEN_SAVER do {\
  uint8_t allZeroData[33]; \
  MEMSET(allZeroData, 0, 33); \
  if ( data == NULL ) data = allZeroData; \
  if ( clusterId == 0x00 ) { \
    if ( metadata->attributeId == 0x0005 && 0x0000 == emberAfGetMfgCode(metadata) &&!emberAfAttributeIsClient(metadata) ) \
      halCommonSetToken(TOKEN_MODEL_IDENTIFIER_SINGLETON, data); \
  }\
  if ( endpoint == 1 ) { \
    if ( clusterId == 0x06 ) { \
      if ( metadata->attributeId == 0x0000 && 0x0000 == emberAfGetMfgCode(metadata) &&!emberAfAttributeIsClient(metadata) ) \
        halCommonSetToken(TOKEN_ON_OFF_1, data); \
    } else if ( clusterId == 0xFC80 ) { \
      if ( metadata->attributeId == 0x0000 && 0x1254 == emberAfGetMfgCode(metadata) &&!emberAfAttributeIsClient(metadata) ) \
        halCommonSetToken(TOKEN_ATTRIBUTE_MODE_ID_1, data); \
      if ( metadata->attributeId == 0x0001 && 0x1254 == emberAfGetMfgCode(metadata) &&!emberAfAttributeIsClient(metadata) ) \
        halCommonSetToken(TOKEN_ATTRIBUTE_CHANNEL_ID_1, data); \
      if ( metadata->attributeId == 0x0002 && 0x1254 == emberAfGetMfgCode(metadata) &&!emberAfAttributeIsClient(metadata) ) \
        halCommonSetToken(TOKEN_ATTRIBUTE_POWER_ID_1, data); \
      if ( metadata->attributeId == 0x0003 && 0x1254 == emberAfGetMfgCode(metadata) &&!emberAfAttributeIsClient(metadata) ) \
        halCommonSetToken(TOKEN_ATTRIBUTE_HEADER_ID_1, data); \
    } \
  } \
} while(false)


