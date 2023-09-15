

#ifndef ECUABFEE_LAYER1_READ_H
#define ECUABFEE_LAYER1_READ_H

#include "EcuabFee.hpp"

#define ECUABFEE_START_SEC_CODE
#include "MemMap.hpp"

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_Rd_Init(void);

FUNC(MemIf_StatusType, ECUABFEE_PUBLIC_CODE) EcuabFee_Rd_GetStatus(void);

FUNC(MemIf_JobResultType, ECUABFEE_PUBLIC_CODE) EcuabFee_Rd_GetJobResult(void);

FUNC(Std_ReturnType, ECUABFEE_PUBLIC_CODE) EcuabFee_Rd_StartJob(EcuabFee_UserJobParameterType EcuabFee_UserJobParameter);

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_Rd_Execute(void);

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_Rd_Cancel(void);

#define ECUABFEE_STOP_SEC_CODE
#include "MemMap.hpp"

#endif

