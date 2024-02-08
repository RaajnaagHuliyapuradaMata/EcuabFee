#ifndef EcuabFee_LAYER2_DATASET_ERASER_H
#define EcuabFee_LAYER2_DATASET_ERASER_H

#include "Fee_30_SmallSector.hpp"
#include "EcuabFee_DatasetHandler.hpp"
#include "EcuabFee_Layer2_InstanceFinder.hpp"

#define EcuabFee_START_SEC_CODE
#include "MemMap.hpp"
FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_De_Init(void);
FUNC(MemIf_StatusType, EcuabFee_PUBLIC_CODE) EcuabFee_De_GetStatus(void);
FUNC(MemIf_JobResultType, EcuabFee_PUBLIC_CODE) EcuabFee_De_GetJobResult(void);
FUNC(Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_De_StartJob(EcuabFee_If_ResultType InstanceFinderResult);
FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_De_Execute(void);
FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_De_Cancel(void);
#define EcuabFee_STOP_SEC_CODE
#include "MemMap.hpp"

#endif

