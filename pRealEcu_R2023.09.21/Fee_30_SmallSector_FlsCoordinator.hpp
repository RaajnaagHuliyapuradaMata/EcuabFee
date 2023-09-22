

#ifndef FEE_30_SMALLSECTOR_FLS_COORDINATOR_H
#define FEE_30_SMALLSECTOR_FLS_COORDINATOR_H

#include "MemIf_Types.hpp"

#define FEE_30_SMALLSECTOR_START_SEC_CODE
#include "MemMap.hpp"

FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Fls_Read(Fee_30_SmallSector_AddressType Address
   ,   Fee_30_SmallSector_DataPtr BufferPtr, uint16 Length);

FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Fls_Write(Fee_30_SmallSector_AddressType Address
   ,   Fee_30_SmallSector_ConstDataPtr BufferPtr, uint16 Length);

FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Fls_Compare(Fee_30_SmallSector_AddressType Address
   ,   Fee_30_SmallSector_ConstDataPtr BufferPtr, uint16 Length);

FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Fls_Erase(Fee_30_SmallSector_AddressType Address, uint16 Length);

FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Fls_BlankCheck(Fee_30_SmallSector_AddressType Address, uint16 Length);

FUNC (void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Fls_SetCurrentFlsJobResult(void);

FUNC (MemIf_JobResultType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Fls_GetJobResult(void);

FUNC (MemIf_StatusType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Fls_GetStatus(void);

#define FEE_30_SMALLSECTOR_STOP_SEC_CODE
#include "MemMap.hpp"

#endif

