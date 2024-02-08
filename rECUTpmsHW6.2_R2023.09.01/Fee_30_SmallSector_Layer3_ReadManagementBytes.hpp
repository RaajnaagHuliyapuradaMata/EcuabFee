#ifndef EcuabFee_LAYER3_READ_MANAGEMENT_BYTES_H
#define EcuabFee_LAYER3_READ_MANAGEMENT_BYTES_H

#include "Fee_30_SmallSector.hpp"
#include "EcuabFee_InstanceHandler.hpp"

#define EcuabFee_START_SEC_CODE
#include "MemMap.hpp"
FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_RdMgmt_Init(void);
FUNC(MemIf_StatusType, EcuabFee_PUBLIC_CODE) EcuabFee_RdMgmt_GetStatus(void);
FUNC(MemIf_JobResultType, EcuabFee_PUBLIC_CODE) EcuabFee_RdMgmt_GetJobResult(void);
FUNC(Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_RdMgmt_StartJob(EcuabFee_Ih_InstanceVarPointerType Instance);
FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_RdMgmt_Execute(void);
FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_RdMgmt_Cancel(void);
#define EcuabFee_STOP_SEC_CODE
#include "MemMap.hpp"

#endif

