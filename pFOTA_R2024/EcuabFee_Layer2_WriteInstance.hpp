

#ifndef ECUABFEE_LAYER2_WRITE_INSTANCE_H
# define ECUABFEE_LAYER2_WRITE_INSTANCE_H

#include "EcuabFee.hpp"
#include "EcuabFee_InstanceHandler.hpp"

# define ECUABFEE_START_SEC_CODE
# include "MemMap.hpp"

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_WrInst_Init(void);

FUNC(MemIf_StatusType, ECUABFEE_PUBLIC_CODE) EcuabFee_WrInst_GetStatus(void);

FUNC(MemIf_JobResultType, ECUABFEE_PUBLIC_CODE) EcuabFee_WrInst_GetJobResult(void);

FUNC(Std_ReturnType, ECUABFEE_PUBLIC_CODE) EcuabFee_WrInst_StartJob(EcuabFee_Ih_InstanceType Instance, EcuabFee_UserJobParameterType UserJobParameter);

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_WrInst_Execute(void);

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_WrInst_Cancel(void);

# define ECUABFEE_STOP_SEC_CODE
# include "MemMap.hpp"

#endif

