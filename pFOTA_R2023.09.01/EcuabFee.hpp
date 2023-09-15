#ifndef ECUABFEE_H
#define ECUABFEE_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "Std_Types.hpp"
#include "Types_EcuabMemIf.hpp"
#include "CfgEcuabFee.hpp"
#define ECUABFEE_VENDOR_ID                    (30u)
#define ECUABFEE_MODULE_ID                    (21u)
#define ECUABFEE_INSTANCE_ID                  (0u)
#define ECUABFEE_AR_RELEASE_MAJOR_VERSION     (4u)
#define ECUABFEE_AR_RELEASE_MINOR_VERSION     (0u)
#define ECUABFEE_AR_RELEASE_REVISION_VERSION  (3u)
#define ECUABFEE_SW_MAJOR_VERSION             (2u)
#define ECUABFEE_SW_MINOR_VERSION             (0u)
#define ECUABFEE_SW_PATCH_VERSION             (0u)
#define ECUABFEE_E_NO_ERROR                   (0x00u)
#define ECUABFEE_E_UNINIT                     (0x01u)
#define ECUABFEE_E_INVALID_BLOCK_NO           (0x02u)
#define ECUABFEE_E_INVALID_BLOCK_OFS          (0x03u)
#define ECUABFEE_E_INVALID_DATA_POINTER       (0x04u)
#define ECUABFEE_E_INVALID_BLOCK_LEN          (0x05u)
#define ECUABFEE_E_BUSY                       (0x06u)
#define ECUABFEE_E_BUSY_INTERNAL              (0x07u)
#define ECUABFEE_E_INVALID_CANCEL             (0x08u)
#define ECUABFEE_E_PARAM_MODE                 (0x0Au)
#define ECUABFEE_SID_INIT                     (0x00u)
#define ECUABFEE_SID_SET_MODE                 (0x01u)
#define ECUABFEE_SID_READ                     (0x02u)
#define ECUABFEE_SID_WRITE                    (0x03u)
#define ECUABFEE_SID_CANCEL                   (0x04u)
#define ECUABFEE_SID_GET_STATUS               (0x05u)
#define ECUABFEE_SID_GET_JOB_RESULT           (0x06u)
#define ECUABFEE_SID_INVALIDATE_BLOCK         (0x07u)
#define ECUABFEE_SID_GET_VERSION_INFO         (0x08u)
#define ECUABFEE_SID_ERASE_IMMEDIATE_BLOCK    (0x09u)
#define ECUABFEE_SID_JOB_END_NOTIFICATION     (0x10u)
#define ECUABFEE_SID_JOB_ERROR_NOTIFICATION   (0x11u)
#define ECUABFEE_SID_MAINFUNCTION             (0x12u)

#define ECUABFEE_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

typedef P2VAR(uint8, AUTOMATIC, ECUABFEE_APPL_DATA) EcuabFee_VarDataPtrType;

typedef enum{
  ECUABFEE_READ_JOB = 0
   ,  ECUABFEE_WRITE_JOB
   ,  ECUABFEE_INVALIDATE_JOB
   ,  ECUABFEE_ERASE_JOB
}EcuabFee_JobType;

typedef enum{
  ECUABFEE_PATTERN_INIT = 0
   ,  ECUABFEE_PATTERN_BLANK
   ,  ECUABFEE_PATTERN_VALID
   ,  ECUABFEE_PATTERN_INCONSISTENT
}EcuabFee_StaticPatternState;

typedef struct{
  uint16 BlockNumber;
  uint16 BlockOffset;
  uint16 Length;
  EcuabFee_VarDataPtrType DataBufferPtr;
  EcuabFee_JobType JobType;
}EcuabFee_UserJobParameterType;

#define ECUABFEE_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define ECUABFEE_START_SEC_CODE
#include "MemMap.hpp"

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_Init (void);

FUNC(Std_ReturnType, ECUABFEE_PUBLIC_CODE) EcuabFee_Read (uint16 BlockNumber, uint16 BlockOffset
   ,   EcuabFee_VarDataPtrType DataBufferPtr, uint16 Length);

FUNC(Std_ReturnType, ECUABFEE_PUBLIC_CODE) EcuabFee_Write (uint16 BlockNumber, EcuabFee_VarDataPtrType DataBufferPtr);
FUNC(Std_ReturnType, ECUABFEE_PUBLIC_CODE) EcuabFee_InvalidateBlock (uint16 BlockNumber);
FUNC(Std_ReturnType, ECUABFEE_PUBLIC_CODE) EcuabFee_EraseImmediateBlock (uint16 BlockNumber);
FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_Cancel (void);
FUNC(MemIf_StatusType, ECUABFEE_PUBLIC_CODE) EcuabFee_GetStatus (void);
FUNC(MemIf_JobResultType, ECUABFEE_PUBLIC_CODE) EcuabFee_GetJobResult (void);
FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_GetVersionInfo (P2VAR(Std_VersionInfoType, AUTOMATIC, ECUABFEE_APPL_DATA) VersionInfoPtr);
FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_SetMode (MemIf_ModeType Mode);
FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_SuspendWrites (void);
FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_ResumeWrites (void);
FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_MainFunction (void);

#define ECUABFEE_STOP_SEC_CODE
#include "MemMap.hpp"

#ifdef ECUABFEE_IMPLEMENTATION_SOURCE
#define ECUABFEE_START_SEC_CODE
#include "MemMap.hpp"
FUNC (uint32, ECUABFEE_PUBLIC_CODE) EcuabFee_AlignValue(uint32 Value, uint16 Alignment);
#define ECUABFEE_STOP_SEC_CODE
#include "MemMap.hpp"
#endif

#ifdef __cplusplus
}
#endif
#endif

