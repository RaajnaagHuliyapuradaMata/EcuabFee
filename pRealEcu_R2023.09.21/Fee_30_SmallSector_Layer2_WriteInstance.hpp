#ifndef FEE_30_SMALLSECTOR_LAYER2_WRITE_INSTANCE_H
#define FEE_30_SMALLSECTOR_LAYER2_WRITE_INSTANCE_H

#include "Fee_30_SmallSector.hpp"
#include "Fee_30_SmallSector_InstanceHandler.hpp"

#define FEE_30_SMALLSECTOR_START_SEC_CODE
#include "MemMap.hpp"
FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_WrInst_Init(void);
FUNC(MemIf_StatusType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_WrInst_GetStatus(void);
FUNC(MemIf_JobResultType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_WrInst_GetJobResult(void);
FUNC(Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_WrInst_StartJob(Fee_30_SmallSector_Ih_InstanceType Instance, Fee_30_SmallSector_UserJobParameterType UserJobParameter);
FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_WrInst_Execute(void);
FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_WrInst_Cancel(void);
#define FEE_30_SMALLSECTOR_STOP_SEC_CODE
#include "MemMap.hpp"

#endif

