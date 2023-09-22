#ifndef FEE_30_SMALLSECTOR_PARTITION_HANDLER_H
#define FEE_30_SMALLSECTOR_PARTITION_HANDLER_H

#include "Fee_30_SmallSector.hpp"

#define FEE_30_SMALLSECTOR_START_SEC_CODE
#include "MemMap.hpp"
FUNC (void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ph_Init(uint16 BlockNumber);
FUNC (Fee_30_SmallSector_AddressType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ph_GetPartitionStartAddress(void);
FUNC (uint16, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ph_GetPartitionWriteAlignment(void);
FUNC (uint16, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ph_GetPartitionAddressAlignment(void);
FUNC (uint16, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ph_GetPartitionReadAlignment(void);
FUNC (Fee_30_SmallSector_FlsApiPtrType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ph_GetFlsApiPtr(void);
FUNC (uint8, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ph_GetFlsEraseValue(void);
FUNC (boolean, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ph_GetBlankCheckApi(void);
#define FEE_30_SMALLSECTOR_STOP_SEC_CODE
#include "MemMap.hpp"

#endif

