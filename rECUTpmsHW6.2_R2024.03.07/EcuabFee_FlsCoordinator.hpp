#pragma once

#include "Types_EcuabIfMem.hpp"

FUNC(Std_ReturnType,      ECUABFEE_PUBLIC_CODE) EcuabFee_Fls_Read                   (EcuabFee_AddressType Address, EcuabFee_DataPtr      BufferPtr, uint16 Length);
FUNC(Std_ReturnType,      ECUABFEE_PUBLIC_CODE) EcuabFee_Fls_Write                  (EcuabFee_AddressType Address, EcuabFee_ConstDataPtr BufferPtr, uint16 Length);
FUNC(Std_ReturnType,      ECUABFEE_PUBLIC_CODE) EcuabFee_Fls_Compare                (EcuabFee_AddressType Address, EcuabFee_ConstDataPtr BufferPtr, uint16 Length);
FUNC(Std_ReturnType,      ECUABFEE_PUBLIC_CODE) EcuabFee_Fls_Erase                  (EcuabFee_AddressType Address, uint16 Length);
FUNC(Std_ReturnType,      ECUABFEE_PUBLIC_CODE) EcuabFee_Fls_BlankCheck             (EcuabFee_AddressType Address, uint16 Length);
FUNC(void,                ECUABFEE_PUBLIC_CODE) EcuabFee_Fls_SetCurrentFlsJobResult (void);
FUNC(MemIf_JobResultType, ECUABFEE_PUBLIC_CODE) EcuabFee_Fls_GetJobResult           (void);
FUNC(MemIf_StatusType,    ECUABFEE_PUBLIC_CODE) EcuabFee_Fls_GetStatus              (void);
