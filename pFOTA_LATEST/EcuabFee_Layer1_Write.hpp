

#ifndef ECUABFEE_LAYER1_WRITE_H
#define ECUABFEE_LAYER1_WRITE_H

#include "EcuabFee.hpp"

#define ECUABFEE_START_SEC_CODE
#include "MemMap.hpp"

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_Wr_Init(void);

FUNC(MemIf_StatusType, ECUABFEE_PUBLIC_CODE) EcuabFee_Wr_GetStatus(void);

FUNC(MemIf_JobResultType, ECUABFEE_PUBLIC_CODE) EcuabFee_Wr_GetJobResult(void);

FUNC(Std_ReturnType, ECUABFEE_PUBLIC_CODE) EcuabFee_Wr_StartJob(EcuabFee_UserJobParameterType EcuabFee_UserJobParameter);

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_Wr_Execute(void);

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_Wr_Cancel(void);

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_Wr_SuspendWrites(void);

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_Wr_ResumeWrites(void);

#define ECUABFEE_STOP_SEC_CODE
#include "MemMap.hpp"

#endif

