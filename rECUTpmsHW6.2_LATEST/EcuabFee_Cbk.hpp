

#ifndef EcuabFee_CBK_H
#define EcuabFee_CBK_H

#define EcuabFee_CBK_MAJOR_VERSION    (2)
#define EcuabFee_CBK_MINOR_VERSION    (0)
#define EcuabFee_CBK_PATCH_VERSION    (0)

#define EcuabFee_START_SEC_CODE
#include "MemMap.hpp"

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_JobEndNotification(void);

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_JobErrorNotification(void);

#define EcuabFee_STOP_SEC_CODE
#include "MemMap.hpp"

#endif

