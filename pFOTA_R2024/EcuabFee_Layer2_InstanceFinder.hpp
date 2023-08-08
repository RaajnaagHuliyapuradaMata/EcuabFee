

#ifndef ECUABFEE_SUB_INSTANCE_FINDER_H
# define ECUABFEE_SUB_INSTANCE_FINDER_H

#include "EcuabFee.hpp"
#include "EcuabFee_InstanceHandler.hpp"

typedef enum{
  INSTANCE_FINDER_OK = 0,
  INSTANCE_FINDER_PENDING,
  INSTANCE_FINDER_CANCELED,
  INSTANCE_FINDER_FAILED,
  INSTANCE_FINDER_INCONSISTENT,
  INSTANCE_FINDER_INVALID,
  INSTANCE_FINDER_EP1_INCORRECT,
  INSTANCE_FINDER_EP2_INCORRECT
}EcuabFee_If_ResultType;

# define ECUABFEE_START_SEC_CODE
# include "MemMap.hpp"

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_If_Init(void);

FUNC(MemIf_StatusType, ECUABFEE_PUBLIC_CODE) EcuabFee_If_GetStatus(void);

FUNC(EcuabFee_If_ResultType, ECUABFEE_PUBLIC_CODE) EcuabFee_If_GetJobResult(void);

FUNC(Std_ReturnType, ECUABFEE_PUBLIC_CODE) EcuabFee_If_StartJob(EcuabFee_Ih_InstanceVarPointerType Instance);

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_If_Execute(void);

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_If_Cancel(void);

FUNC(MemIf_JobResultType, ECUABFEE_PUBLIC_CODE) EcuabFee_If_MapResult(EcuabFee_If_ResultType JobResult);

# define ECUABFEE_STOP_SEC_CODE
# include "MemMap.hpp"

#endif

