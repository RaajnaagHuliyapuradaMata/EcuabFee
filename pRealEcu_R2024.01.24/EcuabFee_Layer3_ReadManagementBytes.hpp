#ifndef ECUABFEE_LAYER3_READ_MANAGEMENT_BYTES_H
#define ECUABFEE_LAYER3_READ_MANAGEMENT_BYTES_H

#include "EcuabFee.hpp"
#include "EcuabFee_InstanceHandler.hpp"

#define ECUABFEE_START_SEC_CODE
#include "MemMap.hpp"
FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_RdMgmt_Init(void);
FUNC(MemIf_StatusType, ECUABFEE_PUBLIC_CODE) EcuabFee_RdMgmt_GetStatus(void);
FUNC(MemIf_JobResultType, ECUABFEE_PUBLIC_CODE) EcuabFee_RdMgmt_GetJobResult(void);
FUNC(Std_ReturnType, ECUABFEE_PUBLIC_CODE) EcuabFee_RdMgmt_StartJob(EcuabFee_Ih_InstanceVarPointerType Instance);
FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_RdMgmt_Execute(void);
FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_RdMgmt_Cancel(void);
#define ECUABFEE_STOP_SEC_CODE
#include "MemMap.hpp"

#endif

