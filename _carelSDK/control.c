#include "common.h"


//void vcReadAttribute(uint16_t nodeId, uint8_t ep, uint16_t clusterId, uint16_t attributeId, uint8_t mfg)
//{
//	uint8_t cmd[64];
//	uint8_t len;
//
//	if (mfg) {
//		memset(cmd, 0, 64);
//		len = sprintf(cmd, "zcl mfg-code 0x%02x\r\n", EMBER_AF_MANUFACTURER_CODE);
//		emberProcessCommandString(cmd, len);
//	}
//
//	memset(cmd, 0, 64);
//	len = sprintf(cmd, "zcl global read 0x%x 0x%x\r\n", clusterId, attributeId);
//	emberProcessCommandString(cmd, len);
//
//	memset(cmd, 0, 64);
//	len = sprintf(cmd, "send 0x%x 0x%x 0x%x\r\n", nodeId, 1, ep);
//	emberProcessCommandString(cmd, len);
//}
//


void _save2token_attrRx(uint8_t mode, uint8_t ch, int8_t power, uint16_t header){
	// mode
	emberAfWriteManufacturerSpecificServerAttribute(1, 0xFC81, 0x0000, 0x1254, (uint8_t*)&mode, 0x20);
	// ch
	emberAfWriteManufacturerSpecificServerAttribute(1, 0xFC81, 0x0001, 0x1254, (uint8_t*)&ch, 0x20);
	// power
	emberAfWriteManufacturerSpecificServerAttribute(1, 0xFC81, 0x0002, 0x1254, (uint8_t*)&power, 0x28);
	// header
	emberAfWriteManufacturerSpecificServerAttribute(1, 0xFC81, 0x0003, 0x1254, (uint8_t*)&header, 0x21);
}


void _get4token_attrRx(uint8_t *mode, uint8_t *ch, int8_t *power, uint16_t *header){
	// mode
	emberAfWriteManufacturerSpecificServerAttribute(1, 0xFC81, 0x0000, 0x1254, (uint8_t*)&mode, 0x20);
	// ch
	emberAfWriteManufacturerSpecificServerAttribute(1, 0xFC81, 0x0001, 0x1254, (uint8_t*)&ch, 0x20);
	// power
	emberAfWriteManufacturerSpecificServerAttribute(1, 0xFC81, 0x0002, 0x1254, (uint8_t*)&power, 0x28);
	// header
	emberAfWriteManufacturerSpecificServerAttribute(1, 0xFC81, 0x0003, 0x1254, (uint8_t*)&header, 0x21);
}



