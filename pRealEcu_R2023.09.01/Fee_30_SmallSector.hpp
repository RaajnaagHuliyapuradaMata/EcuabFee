#ifndef EcuabFee_H
#define EcuabFee_H

#include "Std_Types.hpp"
#include "MemIf_Types.hpp"
#include "EcuabFee_Cfg.hpp"

#define EcuabFee_VENDOR_ID                    (1u)
#define EcuabFee_MODULE_ID                    (21u)
#define EcuabFee_INSTANCE_ID                  (0u)
#define EcuabFee_AR_RELEASE_MAJOR_VERSION     (4u)
#define EcuabFee_AR_RELEASE_MINOR_VERSION     (0u)
#define EcuabFee_AR_RELEASE_REVISION_VERSION  (3u)
#define EcuabFee_SW_MAJOR_VERSION             (2u)
#define EcuabFee_SW_MINOR_VERSION             (0u)
#define EcuabFee_SW_PATCH_VERSION             (0u)
#define EcuabFee_E_NO_ERROR                   (0x00u)
#define EcuabFee_E_UNINIT                     (0x01u)
#define EcuabFee_E_INVALID_BLOCK_NO           (0x02u)
#define EcuabFee_E_INVALID_BLOCK_OFS          (0x03u)
#define EcuabFee_E_INVALID_DATA_POINTER       (0x04u)
#define EcuabFee_E_INVALID_BLOCK_LEN          (0x05u)
#define EcuabFee_E_BUSY                       (0x06u)
#define EcuabFee_E_BUSY_INTERNAL              (0x07u)
#define EcuabFee_E_INVALID_CANCEL             (0x08u)
#define EcuabFee_E_PARAM_MODE                 (0x0Au)
#define EcuabFee_SID_INIT                     (0x00u)
#define EcuabFee_SID_SET_MODE                 (0x01u)
#define EcuabFee_SID_READ                     (0x02u)
#define EcuabFee_SID_WRITE                    (0x03u)
#define EcuabFee_SID_CANCEL                   (0x04u)
#define EcuabFee_SID_GET_STATUS               (0x05u)
#define EcuabFee_SID_GET_JOB_RESULT           (0x06u)
#define EcuabFee_SID_INVALIDATE_BLOCK         (0x07u)
#define EcuabFee_SID_GET_VERSION_INFO         (0x08u)
#define EcuabFee_SID_ERASE_IMMEDIATE_BLOCK    (0x09u)
#define EcuabFee_SID_JOB_END_NOTIFICATION     (0x10u)
#define EcuabFee_SID_JOB_ERROR_NOTIFICATION   (0x11u)
#define EcuabFee_SID_MAINFUNCTION             (0x12u)

#define EcuabFee_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"
typedef P2VAR(uint8, AUTOMATIC, EcuabFee_APPL_DATA) EcuabFee_VarDataPtrType;

typedef enum{
  EcuabFee_READ_JOB = 0
   ,  EcuabFee_WRITE_JOB
   ,  EcuabFee_INVALIDATE_JOB
   ,  EcuabFee_ERASE_JOB
}EcuabFee_JobType;

typedef enum{
  EcuabFee_PATTERN_INIT = 0
   ,  EcuabFee_PATTERN_BLANK
   ,  EcuabFee_PATTERN_VALID
   ,  EcuabFee_PATTERN_INCONSISTENT
}EcuabFee_StaticPatternState;

typedef struct{
  uint16 BlockNumber;
  uint16 BlockOffset;
  uint16 Length;
  EcuabFee_VarDataPtrType DataBufferPtr;
  EcuabFee_JobType JobType;
}EcuabFee_UserJobParameterType;
#define EcuabFee_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define EcuabFee_START_SEC_CODE
#include "MemMap.hpp"
FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_Init (void);
FUNC(Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Read (uint16 BlockNumber, uint16 BlockOffset
   ,   EcuabFee_VarDataPtrType DataBufferPtr, uint16 Length);

FUNC(Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Write (uint16 BlockNumber, EcuabFee_VarDataPtrType DataBufferPtr);
FUNC(Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_InvalidateBlock (uint16 BlockNumber);
FUNC(Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_EraseImmediateBlock (uint16 BlockNumber);
FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_Cancel (void);
FUNC(MemIf_StatusType, EcuabFee_PUBLIC_CODE) EcuabFee_GetStatus (void);
FUNC(MemIf_JobResultType, EcuabFee_PUBLIC_CODE) EcuabFee_GetJobResult (void);
FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_GetVersionInfo (P2VAR(Std_VersionInfoType, AUTOMATIC, EcuabFee_APPL_DATA) VersionInfoPtr);
FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_SetMode (MemIf_ModeType Mode);
FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_SuspendWrites (void);
FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_ResumeWrites (void);
FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_MainFunction (void);
#define EcuabFee_STOP_SEC_CODE
#include "MemMap.hpp"

# ifdef EcuabFee_IMPLEMENTATION_SOURCE
#  define EcuabFee_START_SEC_CODE
#  include "MemMap.hpp"
FUNC (uint32, EcuabFee_PUBLIC_CODE) EcuabFee_AlignValue(uint32 Value, uint16 Alignment);
#  define EcuabFee_STOP_SEC_CODE
#  include "MemMap.hpp"
# endif

#endif

