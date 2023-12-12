#ifndef EcuabFee_LAYER1_READ_H
#define EcuabFee_LAYER1_READ_H

#include "Fee_30_SmallSector.hpp"

#define EcuabFee_START_SEC_CODE
#include "MemMap.hpp"
FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_Rd_Init(void);
FUNC(MemIf_StatusType, EcuabFee_PUBLIC_CODE) EcuabFee_Rd_GetStatus(void);
FUNC(MemIf_JobResultType, EcuabFee_PUBLIC_CODE) EcuabFee_Rd_GetJobResult(void);
FUNC(Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Rd_StartJob(EcuabFee_UserJobParameterType EcuabFee_UserJobParameter);
FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_Rd_Execute(void);
FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_Rd_Cancel(void);
#define EcuabFee_STOP_SEC_CODE
#include "MemMap.hpp"

#endif

