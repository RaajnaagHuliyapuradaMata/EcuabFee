#ifndef EcuabFee_LAYER1_WRITE_H
#define EcuabFee_LAYER1_WRITE_H

#include "Fee_30_SmallSector.hpp"

#define EcuabFee_START_SEC_CODE
#include "MemMap.hpp"
FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_Wr_Init(void);
FUNC(MemIf_StatusType, EcuabFee_PUBLIC_CODE) EcuabFee_Wr_GetStatus(void);
FUNC(MemIf_JobResultType, EcuabFee_PUBLIC_CODE) EcuabFee_Wr_GetJobResult(void);
FUNC(Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Wr_StartJob(EcuabFee_UserJobParameterType EcuabFee_UserJobParameter);
FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_Wr_Execute(void);
FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_Wr_Cancel(void);
FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_Wr_SuspendWrites(void);
FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_Wr_ResumeWrites(void);
#define EcuabFee_STOP_SEC_CODE
#include "MemMap.hpp"

#endif

