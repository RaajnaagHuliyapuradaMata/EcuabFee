

#ifndef ECUABFEE_LAYER2_DATASET_ERASER_H
# define ECUABFEE_LAYER2_DATASET_ERASER_H

#include "EcuabFee.hpp"
#include "EcuabFee_DatasetHandler.hpp"
#include "EcuabFee_Layer2_InstanceFinder.hpp"

# define ECUABFEE_START_SEC_CODE
# include "MemMap.hpp"

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_De_Init(void);

FUNC(MemIf_StatusType, ECUABFEE_PUBLIC_CODE) EcuabFee_De_GetStatus(void);

FUNC(MemIf_JobResultType, ECUABFEE_PUBLIC_CODE) EcuabFee_De_GetJobResult(void);

FUNC(Std_ReturnType, ECUABFEE_PUBLIC_CODE) EcuabFee_De_StartJob(EcuabFee_If_ResultType InstanceFinderResult);

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_De_Execute(void);

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_De_Cancel(void);

# define ECUABFEE_STOP_SEC_CODE
# include "MemMap.hpp"

#endif

