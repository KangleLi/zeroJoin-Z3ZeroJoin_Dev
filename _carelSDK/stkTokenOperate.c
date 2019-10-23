#include "common.h"

// nodeData
void _stkTokenGet_StackNodeData(tokTypeStackNodeData *dat){
	halCommonGetToken(dat, TOKEN_STACK_NODE_DATA);
}

void _stkTokenSet_StackNodeData(tokTypeStackNodeData *dat){
	halCommonSetToken(TOKEN_STACK_NODE_DATA, dat);
}

// Keys
void _stkTokenGet_StackKeys(tokTypeStackKeys *dat){
	halCommonGetToken(dat, TOKEN_STACK_KEYS);
}

void _stkTokenSet_StackKeys(tokTypeStackKeys *dat){
	halCommonSetToken(TOKEN_STACK_KEYS, dat);
}

// TrustCenter
// mode : 12388
void _stkTokenGet_TrustCenter(tokTypeStackTrustCenter *dat){
	halCommonGetToken(dat, TOKEN_STACK_TRUST_CENTER);
}

void _stkTokenSet_TrustCenter(tokTypeStackTrustCenter *dat){
	halCommonSetToken(TOKEN_STACK_TRUST_CENTER, dat);
}

// NetworkManagement
// activeChannels : 0x07FFF800
// managerNodeId: 	0x0000
void _stkTokenGet_NetworkManagement(tokTypeStackNetworkManagement *dat){
	halCommonGetToken(dat, TOKEN_STACK_NETWORK_MANAGEMENT);
}

void _stkTokenSet_NetworkManagement(tokTypeStackNetworkManagement *dat){
	halCommonSetToken(TOKEN_STACK_NETWORK_MANAGEMENT, dat);
}

// ParentInfo
// parentEui : --- coor
// parentNodeId : --- coor
void _stkTokenGet_ParentInfo(tokTypeStackParentInfo *dat){
	halCommonGetToken(dat, TOKEN_STACK_PARENT_INFO);
}

void _stkTokenSet_ParentInfo(tokTypeStackParentInfo *dat){
	halCommonSetToken(TOKEN_STACK_PARENT_INFO, dat);
}

// MultiPhyNwkInfo
void _stkTokenGet_MultiPhyNwkInfo(tokTypeStackMultiPhyNwkInfo *dat){
	halCommonGetToken(dat, TOKEN_STACK_MULTI_PHY_NWK_INFO);
}

void _stkTokenSet_MultiPhyNwkInfo(tokTypeStackMultiPhyNwkInfo *dat){
	halCommonSetToken(TOKEN_STACK_MULTI_PHY_NWK_INFO, dat);
}

// NonceCounter
void _stkTokenGet_NonceCounter(tokTypeStackNonceCounter *dat){
	halCommonGetToken(dat, TOKEN_STACK_NONCE_COUNTER);
}

void _stkTokenSet_NonceCounter(tokTypeStackNonceCounter *dat){
	halCommonSetToken(TOKEN_STACK_NONCE_COUNTER, dat);
}



