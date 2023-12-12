#ifndef EcuabFee_PARTITION_HANDLER_H
#define EcuabFee_PARTITION_HANDLER_H

#include "Fee_30_SmallSector.hpp"

#define EcuabFee_START_SEC_CODE
#include "MemMap.hpp"
FUNC (void, EcuabFee_PUBLIC_CODE) EcuabFee_Ph_Init(uint16 BlockNumber);
FUNC (EcuabFee_AddressType, EcuabFee_PUBLIC_CODE) EcuabFee_Ph_GetPartitionStartAddress(void);
FUNC (uint16, EcuabFee_PUBLIC_CODE) EcuabFee_Ph_GetPartitionWriteAlignment(void);
FUNC (uint16, EcuabFee_PUBLIC_CODE) EcuabFee_Ph_GetPartitionAddressAlignment(void);
FUNC (uint16, EcuabFee_PUBLIC_CODE) EcuabFee_Ph_GetPartitionReadAlignment(void);
FUNC (EcuabFee_FlsApiPtrType, EcuabFee_PUBLIC_CODE) EcuabFee_Ph_GetFlsApiPtr(void);
FUNC (uint8, EcuabFee_PUBLIC_CODE) EcuabFee_Ph_GetFlsEraseValue(void);
FUNC (boolean, EcuabFee_PUBLIC_CODE) EcuabFee_Ph_GetBlankCheckApi(void);
#define EcuabFee_STOP_SEC_CODE
#include "MemMap.hpp"

#endif

