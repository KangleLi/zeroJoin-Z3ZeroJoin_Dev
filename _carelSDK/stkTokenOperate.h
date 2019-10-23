
#ifndef CARELSDK_STKTOKENOPERATE_H_
#define CARELSDK_STKTOKENOPERATE_H_



extern void _stkTokenGet_StackNodeData(tokTypeStackNodeData *dat);
extern void _stkTokenSet_StackNodeData(tokTypeStackNodeData *dat);


extern void _stkTokenGet_StackKeys(tokTypeStackKeys *dat);
extern void _stkTokenSet_StackKeys(tokTypeStackKeys *dat);

extern void _stkTokenGet_TrustCenter(tokTypeStackTrustCenter *dat);
extern void _stkTokenSet_TrustCenter(tokTypeStackTrustCenter *dat);

extern void _stkTokenGet_NetworkManagement(tokTypeStackNetworkManagement *dat);
extern void _stkTokenSet_NetworkManagement(tokTypeStackNetworkManagement *dat);

extern void _stkTokenGet_ParentInfo(tokTypeStackParentInfo *dat);
extern void _stkTokenSet_ParentInfo(tokTypeStackParentInfo *dat);

extern void _stkTokenGet_MultiPhyNwkInfo(tokTypeStackMultiPhyNwkInfo *dat);
extern void _stkTokenSet_MultiPhyNwkInfo(tokTypeStackMultiPhyNwkInfo *dat);

extern void _stkTokenGet_NonceCounter(tokTypeStackNonceCounter *dat);
extern void _stkTokenSet_NonceCounter(tokTypeStackNonceCounter *dat);








#endif /* CARELSDK_STKTOKENOPERATE_H_ */
