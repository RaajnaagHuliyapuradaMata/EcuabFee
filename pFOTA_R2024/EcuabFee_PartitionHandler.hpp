

#ifndef ECUABFEE_PARTITION_HANDLER_H
# define ECUABFEE_PARTITION_HANDLER_H

# include "EcuabFee.hpp"

# define ECUABFEE_START_SEC_CODE
# include "MemMap.hpp"

FUNC (void, ECUABFEE_PUBLIC_CODE) EcuabFee_Ph_Init(uint16 BlockNumber);

FUNC (EcuabFee_AddressType, ECUABFEE_PUBLIC_CODE) EcuabFee_Ph_GetPartitionStartAddress(void);

FUNC (uint16, ECUABFEE_PUBLIC_CODE) EcuabFee_Ph_GetPartitionWriteAlignment(void);

FUNC (uint16, ECUABFEE_PUBLIC_CODE) EcuabFee_Ph_GetPartitionAddressAlignment(void);

FUNC (uint16, ECUABFEE_PUBLIC_CODE) EcuabFee_Ph_GetPartitionReadAlignment(void);

FUNC (EcuabFee_FlsApiPtrType, ECUABFEE_PUBLIC_CODE) EcuabFee_Ph_GetFlsApiPtr(void);

FUNC (uint8, ECUABFEE_PUBLIC_CODE) EcuabFee_Ph_GetFlsEraseValue(void);

FUNC (boolean, ECUABFEE_PUBLIC_CODE) EcuabFee_Ph_GetBlankCheckApi(void);

# define ECUABFEE_STOP_SEC_CODE
# include "MemMap.hpp"

#endif

