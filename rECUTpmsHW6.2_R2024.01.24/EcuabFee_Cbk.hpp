

#ifndef ECUABFEE_CBK_H
#define ECUABFEE_CBK_H

#define ECUABFEE_CBK_MAJOR_VERSION    (2)
#define ECUABFEE_CBK_MINOR_VERSION    (0)
#define ECUABFEE_CBK_PATCH_VERSION    (0)

#define ECUABFEE_START_SEC_CODE
#include "MemMap.hpp"

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_JobEndNotification(void);

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_JobErrorNotification(void);

#define ECUABFEE_STOP_SEC_CODE
#include "MemMap.hpp"

#endif

