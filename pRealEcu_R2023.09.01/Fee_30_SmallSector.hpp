#ifndef FEE_30_SMALLSECTOR_H
#define FEE_30_SMALLSECTOR_H

#include "Std_Types.hpp"
#include "MemIf_Types.hpp"
#include "Fee_30_SmallSector_Cfg.hpp"

#define FEE_30_SMALLSECTOR_VENDOR_ID                    (30u)
#define FEE_30_SMALLSECTOR_MODULE_ID                    (21u)
#define FEE_30_SMALLSECTOR_INSTANCE_ID                  (0u)
#define FEE_30_SMALLSECTOR_AR_RELEASE_MAJOR_VERSION     (4u)
#define FEE_30_SMALLSECTOR_AR_RELEASE_MINOR_VERSION     (0u)
#define FEE_30_SMALLSECTOR_AR_RELEASE_REVISION_VERSION  (3u)
#define FEE_30_SMALLSECTOR_SW_MAJOR_VERSION             (2u)
#define FEE_30_SMALLSECTOR_SW_MINOR_VERSION             (0u)
#define FEE_30_SMALLSECTOR_SW_PATCH_VERSION             (0u)
#define FEE_30_SMALLSECTOR_E_NO_ERROR                   (0x00u)
#define FEE_30_SMALLSECTOR_E_UNINIT                     (0x01u)
#define FEE_30_SMALLSECTOR_E_INVALID_BLOCK_NO           (0x02u)
#define FEE_30_SMALLSECTOR_E_INVALID_BLOCK_OFS          (0x03u)
#define FEE_30_SMALLSECTOR_E_INVALID_DATA_POINTER       (0x04u)
#define FEE_30_SMALLSECTOR_E_INVALID_BLOCK_LEN          (0x05u)
#define FEE_30_SMALLSECTOR_E_BUSY                       (0x06u)
#define FEE_30_SMALLSECTOR_E_BUSY_INTERNAL              (0x07u)
#define FEE_30_SMALLSECTOR_E_INVALID_CANCEL             (0x08u)
#define FEE_30_SMALLSECTOR_E_PARAM_MODE                 (0x0Au)
#define FEE_30_SMALLSECTOR_SID_INIT                     (0x00u)
#define FEE_30_SMALLSECTOR_SID_SET_MODE                 (0x01u)
#define FEE_30_SMALLSECTOR_SID_READ                     (0x02u)
#define FEE_30_SMALLSECTOR_SID_WRITE                    (0x03u)
#define FEE_30_SMALLSECTOR_SID_CANCEL                   (0x04u)
#define FEE_30_SMALLSECTOR_SID_GET_STATUS               (0x05u)
#define FEE_30_SMALLSECTOR_SID_GET_JOB_RESULT           (0x06u)
#define FEE_30_SMALLSECTOR_SID_INVALIDATE_BLOCK         (0x07u)
#define FEE_30_SMALLSECTOR_SID_GET_VERSION_INFO         (0x08u)
#define FEE_30_SMALLSECTOR_SID_ERASE_IMMEDIATE_BLOCK    (0x09u)
#define FEE_30_SMALLSECTOR_SID_JOB_END_NOTIFICATION     (0x10u)
#define FEE_30_SMALLSECTOR_SID_JOB_ERROR_NOTIFICATION   (0x11u)
#define FEE_30_SMALLSECTOR_SID_MAINFUNCTION             (0x12u)

#define FEE_30_SMALLSECTOR_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"
typedef P2VAR(uint8, AUTOMATIC, FEE_30_SMALLSECTOR_APPL_DATA) Fee_30_SmallSector_VarDataPtrType;

typedef enum{
  FEE_30_SMALLSECTOR_READ_JOB = 0
   ,  FEE_30_SMALLSECTOR_WRITE_JOB
   ,  FEE_30_SMALLSECTOR_INVALIDATE_JOB
   ,  FEE_30_SMALLSECTOR_ERASE_JOB
}Fee_30_SmallSector_JobType;

typedef enum{
  FEE_30_SMALLSECTOR_PATTERN_INIT = 0
   ,  FEE_30_SMALLSECTOR_PATTERN_BLANK
   ,  FEE_30_SMALLSECTOR_PATTERN_VALID
   ,  FEE_30_SMALLSECTOR_PATTERN_INCONSISTENT
}Fee_30_SmallSector_StaticPatternState;

typedef struct{
  uint16 BlockNumber;
  uint16 BlockOffset;
  uint16 Length;
  Fee_30_SmallSector_VarDataPtrType DataBufferPtr;
  Fee_30_SmallSector_JobType JobType;
}Fee_30_SmallSector_UserJobParameterType;
#define FEE_30_SMALLSECTOR_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define FEE_30_SMALLSECTOR_START_SEC_CODE
#include "MemMap.hpp"
FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Init (void);
FUNC(Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Read (uint16 BlockNumber, uint16 BlockOffset
   ,   Fee_30_SmallSector_VarDataPtrType DataBufferPtr, uint16 Length);

FUNC(Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Write (uint16 BlockNumber, Fee_30_SmallSector_VarDataPtrType DataBufferPtr);
FUNC(Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_InvalidateBlock (uint16 BlockNumber);
FUNC(Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_EraseImmediateBlock (uint16 BlockNumber);
FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Cancel (void);
FUNC(MemIf_StatusType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_GetStatus (void);
FUNC(MemIf_JobResultType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_GetJobResult (void);
FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_GetVersionInfo (P2VAR(Std_VersionInfoType, AUTOMATIC, FEE_30_SMALLSECTOR_APPL_DATA) VersionInfoPtr);
FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_SetMode (MemIf_ModeType Mode);
FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_SuspendWrites (void);
FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_ResumeWrites (void);
FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_MainFunction (void);
#define FEE_30_SMALLSECTOR_STOP_SEC_CODE
#include "MemMap.hpp"

# ifdef FEE_30_SMALLSECTOR_IMPLEMENTATION_SOURCE
#  define FEE_30_SMALLSECTOR_START_SEC_CODE
#  include "MemMap.hpp"
FUNC (uint32, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_AlignValue(uint32 Value, uint16 Alignment);
#  define FEE_30_SMALLSECTOR_STOP_SEC_CODE
#  include "MemMap.hpp"
# endif

#endif

