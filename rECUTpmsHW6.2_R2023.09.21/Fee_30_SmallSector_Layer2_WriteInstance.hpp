#ifndef EcuabFee_LAYER2_WRITE_INSTANCE_H
#define EcuabFee_LAYER2_WRITE_INSTANCE_H

#include "Fee_30_SmallSector.hpp"
#include "EcuabFee_InstanceHandler.hpp"

#define EcuabFee_START_SEC_CODE
#include "MemMap.hpp"
FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_WrInst_Init(void);
FUNC(MemIf_StatusType, EcuabFee_PUBLIC_CODE) EcuabFee_WrInst_GetStatus(void);
FUNC(MemIf_JobResultType, EcuabFee_PUBLIC_CODE) EcuabFee_WrInst_GetJobResult(void);
FUNC(Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_WrInst_StartJob(EcuabFee_Ih_InstanceType Instance, EcuabFee_UserJobParameterType UserJobParameter);
FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_WrInst_Execute(void);
FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_WrInst_Cancel(void);
#define EcuabFee_STOP_SEC_CODE
#include "MemMap.hpp"

#endif

