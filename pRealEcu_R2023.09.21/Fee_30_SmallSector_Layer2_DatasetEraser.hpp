#ifndef FEE_30_SMALLSECTOR_LAYER2_DATASET_ERASER_H
#define FEE_30_SMALLSECTOR_LAYER2_DATASET_ERASER_H

#include "Fee_30_SmallSector.hpp"
#include "Fee_30_SmallSector_DatasetHandler.hpp"
#include "Fee_30_SmallSector_Layer2_InstanceFinder.hpp"

#define FEE_30_SMALLSECTOR_START_SEC_CODE
#include "MemMap.hpp"
FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_De_Init(void);
FUNC(MemIf_StatusType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_De_GetStatus(void);
FUNC(MemIf_JobResultType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_De_GetJobResult(void);
FUNC(Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_De_StartJob(Fee_30_SmallSector_If_ResultType InstanceFinderResult);
FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_De_Execute(void);
FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_De_Cancel(void);
#define FEE_30_SMALLSECTOR_STOP_SEC_CODE
#include "MemMap.hpp"

#endif

