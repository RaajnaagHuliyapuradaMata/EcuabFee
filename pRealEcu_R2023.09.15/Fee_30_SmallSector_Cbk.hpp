

#ifndef FEE_30_SMALLSECTOR_CBK_H
#define FEE_30_SMALLSECTOR_CBK_H

#define FEE_30_SMALLSECTOR_CBK_MAJOR_VERSION    (2)
#define FEE_30_SMALLSECTOR_CBK_MINOR_VERSION    (0)
#define FEE_30_SMALLSECTOR_CBK_PATCH_VERSION    (0)

#define FEE_30_SMALLSECTOR_START_SEC_CODE
#include "MemMap.hpp"

FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_JobEndNotification(void);

FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_JobErrorNotification(void);

#define FEE_30_SMALLSECTOR_STOP_SEC_CODE
#include "MemMap.hpp"

#endif

