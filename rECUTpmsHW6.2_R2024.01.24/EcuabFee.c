#include "Std_Types.hpp"

#define ECUABFEE_IMPLEMENTATION_SOURCE

#include "EcuabFee.hpp"
#include "EcuabFee_Cbk.hpp"
#include "EcuabFee_TaskManager.hpp"
#include "EcuabFee_FlsCoordinator.hpp"
#include "EcuabFee_PartitionHandler.hpp"
#include "EcuabFee_BlockHandler.hpp"
#include "EcuabFee_DatasetHandler.hpp"
#include "EcuabFee_Layer1_Read.hpp"
#include "EcuabFee_Layer1_Write.hpp"
#include "EcuabFee_Layer2_WriteInstance.hpp"
#include "EcuabFee_Layer2_DatasetEraser.hpp"
#include "EcuabFee_Layer2_InstanceFinder.hpp"

#include "EcuabFee_Layer3_ReadManagementBytes.hpp"

#if(   (ECUABFEE_SW_MAJOR_VERSION != (2u)) \
     || (ECUABFEE_SW_MINOR_VERSION != (0u)) \
     || (ECUABFEE_AR_RELEASE_MAJOR_VERSION != (4u)) \
     || (ECUABFEE_AR_RELEASE_MINOR_VERSION != (0u)))
# error "Version numbers of EcuabFee.c and EcuabFee.h are inconsistent!"
#endif

#if(   (ECUABFEE_CFG_MAJOR_VERSION != (2u)) \
     || (ECUABFEE_CFG_MINOR_VERSION != (0u)))
# error "Version numbers of EcuabFee.c and EcuabFee_Cfg.h are inconsistent!"
#endif

#if(   (ECUABFEE_CBK_MAJOR_VERSION != (2u)) \
     || (ECUABFEE_CBK_MINOR_VERSION != (0u)))
# error "Version numbers of EcuabFee.c and EcuabFee_Cbk.h are inconsistent!"
#endif

#define ECUABFEE_RESERVED_BLOCK_NUMBER_1             (0x0000u)
#define ECUABFEE_RESERVED_BLOCK_NUMBER_2             (0xFFFFu)

#if !defined (ECUABFEE_LOCAL)
#define ECUABFEE_LOCAL static
#endif

#if !defined (ECUABFEE_LOCAL_INLINE)
#define ECUABFEE_LOCAL_INLINE LOCAL_INLINE
#endif

typedef struct{
  MemIf_JobResultType JobResult;
  MemIf_StatusType Status;
}EcuabFee_ComponentParameterType;

typedef enum{
  ECUABFEE_STATE_UNINIT = 0
   ,  ECUABFEE_STATE_IDLE
   ,  ECUABFEE_STATE_BUSY
}EFeeStateType;

typedef struct{
  boolean MainFunctionBusy;
  boolean JobRequested;
  boolean CancelRequested;
}EcuabFee_InternalFlagsType;

#define ECUABFEE_START_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

ECUABFEE_LOCAL VAR(EcuabFee_ComponentParameterType, ECUABFEE_FAST_DATA) EcuabFee_ComponentParameter = {MEMIF_JOB_FAILED, MEMIF_UNINIT};

ECUABFEE_LOCAL VAR(EFeeStateType, ECUABFEE_APPL_DATA) EcuabFee_StateMachine = ECUABFEE_STATE_UNINIT;

#define ECUABFEE_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

#define ECUABFEE_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

ECUABFEE_LOCAL VAR(EcuabFee_UserJobParameterType, ECUABFEE_APPL_DATA) EcuabFee_UserJobParameter;

ECUABFEE_LOCAL VAR(EcuabFee_InternalFlagsType, ECUABFEE_APPL_DATA) EcuabFee_InternalFlags;

#define ECUABFEE_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define ECUABFEE_START_SEC_CODE
#include "MemMap.hpp"

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_SetFlagJobRequested(boolean value);

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_SetFlagCancelRequested(boolean value);

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_SetFlagMainFunctionBusy(boolean value);

ECUABFEE_LOCAL FUNC(MemIf_StatusType, ECUABFEE_PRIVATE_CODE) EcuabFee_GetLayer1Status(void);

ECUABFEE_LOCAL FUNC(MemIf_JobResultType, ECUABFEE_PRIVATE_CODE) EcuabFee_GetLayer1JobResult(void);

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_StartLayer1Job(void);

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_SetUserParameter(uint16 BlockNumber
   ,     uint16 BlockOffset
   ,     EcuabFee_DataPtr DataBufferPtr
   ,     uint16 Length
   ,     EcuabFee_JobType JobType);

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_SetJobParameter(void);

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_StartJob(void);

ECUABFEE_LOCAL_INLINE FUNC(Std_ReturnType, ECUABFEE_PRIVATE_CODE) EcuabFee_CheckIdle(void);

#if(ECUABFEE_DEV_ERROR_DETECT == STD_ON)

ECUABFEE_LOCAL_INLINE FUNC(Std_ReturnType, ECUABFEE_PRIVATE_CODE) EcuabFee_DetChkInit(void);

ECUABFEE_LOCAL_INLINE FUNC(Std_ReturnType, ECUABFEE_PRIVATE_CODE) EcuabFee_DetChkBlockNumberOk(uint16 BlockNumber, uint8 ApiID);

ECUABFEE_LOCAL_INLINE FUNC(Std_ReturnType, ECUABFEE_PRIVATE_CODE) EcuabFee_DetChkDataPointerOk(
        P2CONST(void, AUTOMATIC, ECUABFEE_APPL_DATA) DataBufferPtr);

ECUABFEE_LOCAL_INLINE FUNC(Std_ReturnType, ECUABFEE_PRIVATE_CODE) EcuabFee_DetChkBlockOffsetOk(uint16 BlockNumber, uint16 BlockOffset);

ECUABFEE_LOCAL_INLINE FUNC(Std_ReturnType, ECUABFEE_PRIVATE_CODE) EcuabFee_DetChkBlockLengthOk(uint16 BlockNumber
   ,     uint16 Length, uint16 BlockOffset);
#endif

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_InitDataHandler(void);

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_ProcessStateMachine(void);

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_FinishJob(MemIf_JobResultType JobResult);

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_SetFlagJobRequested(boolean value)
{
  EcuabFee_InternalFlags.JobRequested = value;
}

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_SetFlagCancelRequested(boolean value)
{
  EcuabFee_InternalFlags.CancelRequested = value;
}

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_SetFlagMainFunctionBusy(boolean value)
{
  EcuabFee_InternalFlags.MainFunctionBusy = value;
}

ECUABFEE_LOCAL FUNC(MemIf_StatusType, ECUABFEE_PRIVATE_CODE) EcuabFee_GetLayer1Status(void){

  MemIf_StatusType retVal;

  switch(EcuabFee_UserJobParameter.JobType)
  {
   case (ECUABFEE_READ_JOB):
      retVal = EcuabFee_Rd_GetStatus();
      break;
    default:
      retVal = EcuabFee_Wr_GetStatus();
      break;
  }
  return retVal;
}

ECUABFEE_LOCAL FUNC(MemIf_JobResultType, ECUABFEE_PRIVATE_CODE) EcuabFee_GetLayer1JobResult(void){

  MemIf_JobResultType retVal;

  switch(EcuabFee_UserJobParameter.JobType)
  {
   case (ECUABFEE_READ_JOB):
      retVal = EcuabFee_Rd_GetJobResult();
      break;
    default:
      retVal = EcuabFee_Wr_GetJobResult();
      break;
  }
  return retVal;
}

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_StartLayer1Job(void){

  switch(EcuabFee_UserJobParameter.JobType)
  {
   case (ECUABFEE_READ_JOB):
      (void) EcuabFee_Rd_StartJob(EcuabFee_UserJobParameter);
      break;
    default:
      (void) EcuabFee_Wr_StartJob(EcuabFee_UserJobParameter);
      break;
  }
}

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_SetUserParameter(uint16 BlockNumber
   ,     uint16 BlockOffset
   ,     EcuabFee_DataPtr DataBufferPtr
   ,     uint16 Length
   ,     EcuabFee_JobType JobType)
{
  EcuabFee_UserJobParameter.BlockNumber = BlockNumber;
  EcuabFee_UserJobParameter.BlockOffset = BlockOffset;
  EcuabFee_UserJobParameter.DataBufferPtr = DataBufferPtr;
  EcuabFee_UserJobParameter.Length = Length;
  EcuabFee_UserJobParameter.JobType = JobType;
}

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_SetJobParameter(void){
  EcuabFee_ComponentParameter.Status = MEMIF_BUSY;
  EcuabFee_ComponentParameter.JobResult = MEMIF_JOB_PENDING;

  EcuabFee_SetFlagJobRequested(TRUE);
  EcuabFee_SetFlagMainFunctionBusy(TRUE);
}

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_StartJob(void){

  EcuabFee_Tm_Init();

  EcuabFee_InitDataHandler();

  EcuabFee_StartLayer1Job();

  EcuabFee_StateMachine = ECUABFEE_STATE_BUSY;
}

ECUABFEE_LOCAL_INLINE FUNC(Std_ReturnType, ECUABFEE_PRIVATE_CODE) EcuabFee_CheckIdle(void){
  return (Std_ReturnType) ((EcuabFee_ComponentParameter.Status == MEMIF_IDLE) ? E_OK : E_NOT_OK);
}

#if(ECUABFEE_DEV_ERROR_DETECT == STD_ON)

ECUABFEE_LOCAL_INLINE FUNC(Std_ReturnType, ECUABFEE_PRIVATE_CODE) EcuabFee_DetChkInit(void){
  return (Std_ReturnType) ((EcuabFee_ComponentParameter.Status == MEMIF_UNINIT) ? E_NOT_OK : E_OK);
}

ECUABFEE_LOCAL_INLINE FUNC(Std_ReturnType, ECUABFEE_PRIVATE_CODE) EcuabFee_DetChkBlockNumberOk(uint16 BlockNumber, uint8 ApiID)
{
  Std_ReturnType retVal;

  if((BlockNumber == (uint16)ECUABFEE_RESERVED_BLOCK_NUMBER_1) ||
      (BlockNumber == (uint16)ECUABFEE_RESERVED_BLOCK_NUMBER_2))
  {
    retVal = E_NOT_OK;
  }

  else if((EcuabFee_Bh_GetBlockIndex(BlockNumber) + 1u) > ECUABFEE_NUMBER_OF_BLOCKS)
  {
    retVal = E_NOT_OK;
  }
  else if(ApiID == (uint8) ECUABFEE_SID_ERASE_IMMEDIATE_BLOCK)
  {

    retVal = ( (EcuabFee_Bh_IsImmediateData(BlockNumber) == FALSE) ? E_NOT_OK : E_OK);
  }

  else if(EcuabFee_Bh_GetNrOfDatasets(BlockNumber) <= EcuabFee_Bh_GetDatasetIndex(BlockNumber))
  {
    retVal = E_NOT_OK;
  }
  else{
    retVal = E_OK;
  }
  return retVal;
}

ECUABFEE_LOCAL_INLINE FUNC(Std_ReturnType, ECUABFEE_PRIVATE_CODE) EcuabFee_DetChkDataPointerOk(
        P2CONST(void, AUTOMATIC, ECUABFEE_APPL_DATA) DataBufferPtr)
{
  return (Std_ReturnType) ((DataBufferPtr == NULL_PTR) ? E_NOT_OK : E_OK);
}

ECUABFEE_LOCAL_INLINE FUNC(Std_ReturnType, ECUABFEE_PRIVATE_CODE) EcuabFee_DetChkBlockOffsetOk(uint16 BlockNumber, uint16 BlockOffset)
{
  return (Std_ReturnType) ((BlockOffset >= EcuabFee_Bh_GetDataLength(BlockNumber)) ? E_NOT_OK : E_OK);
}

ECUABFEE_LOCAL_INLINE FUNC(Std_ReturnType, ECUABFEE_PRIVATE_CODE) EcuabFee_DetChkBlockLengthOk(uint16 BlockNumber
   ,     uint16 Length, uint16 BlockOffset)
{
  Std_ReturnType retVal;

  if(Length == 0u)
  {
    retVal = E_NOT_OK;
  }

  else if( EcuabFee_Bh_GetDataLength(BlockNumber) < (Length + BlockOffset) )
  {
    retVal = E_NOT_OK;
  }
  else{
    retVal = E_OK;
  }

  return (Std_ReturnType) retVal;
}
#endif

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_InitDataHandler(void){
  EcuabFee_Ph_Init(EcuabFee_UserJobParameter.BlockNumber);
  EcuabFee_Bh_Init(EcuabFee_UserJobParameter.BlockNumber, EcuabFee_Ph_GetPartitionStartAddress());
  EcuabFee_Dh_Init(EcuabFee_UserJobParameter.BlockNumber, EcuabFee_Bh_GetDataLength(EcuabFee_UserJobParameter.BlockNumber)
   ,  EcuabFee_Bh_GetNrOfInstances(EcuabFee_UserJobParameter.BlockNumber), EcuabFee_Bh_GetBlockStartAddress(), EcuabFee_Ph_GetPartitionAddressAlignment());
}

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_ProcessStateMachine(void){
  switch(EcuabFee_StateMachine)
  {

   case ECUABFEE_STATE_IDLE:
      if(EcuabFee_InternalFlags.CancelRequested == TRUE)
      {

        EcuabFee_SetFlagCancelRequested(FALSE);
      }
      else{

        EcuabFee_StartJob();
        EcuabFee_SetFlagJobRequested(FALSE);
      }

      break;

   case ECUABFEE_STATE_BUSY:
      if(EcuabFee_InternalFlags.CancelRequested == TRUE)
      {
        EcuabFee_Tm_CancelTasks();

        if(EcuabFee_InternalFlags.JobRequested == FALSE)
        {
          EcuabFee_SetFlagMainFunctionBusy(FALSE);
        }

        EcuabFee_StateMachine = ECUABFEE_STATE_IDLE;
        EcuabFee_SetFlagCancelRequested(FALSE);
      }
      else{
        if(EcuabFee_Fls_GetStatus() == MEMIF_IDLE)
        {

          EcuabFee_Tm_ExecuteTask();

          if((EcuabFee_GetLayer1Status() == MEMIF_IDLE) && (EcuabFee_InternalFlags.CancelRequested == FALSE))
          {
            EcuabFee_FinishJob(EcuabFee_GetLayer1JobResult());
          }
        }
      }
      break;
    default:
      break;
  }
}

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_FinishJob(MemIf_JobResultType JobResult)
{

  EcuabFee_SetFlagMainFunctionBusy(FALSE);
  EcuabFee_StateMachine = ECUABFEE_STATE_IDLE;

  EcuabFee_ComponentParameter.JobResult = JobResult;
  EcuabFee_ComponentParameter.Status = MEMIF_IDLE;

#if(ECUABFEE_NVM_POLLING_MODE == STD_OFF)
  if(JobResult == MEMIF_JOB_OK)
  {
    EcuabFee_CbkJobEndNotification();
  }
  else{
    EcuabFee_CbkJobErrorNotification();
  }
#endif
}

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_Init(void){
  EcuabFee_ComponentParameter.JobResult = MEMIF_JOB_OK;
  EcuabFee_ComponentParameter.Status = MEMIF_IDLE;

  EcuabFee_StateMachine = ECUABFEE_STATE_IDLE;

  EcuabFee_SetFlagMainFunctionBusy(FALSE);
  EcuabFee_SetFlagJobRequested(FALSE);
  EcuabFee_SetFlagCancelRequested(FALSE);

  EcuabFee_Rd_Init();
  EcuabFee_Wr_Init();

  EcuabFee_If_Init();
  EcuabFee_De_Init();
  EcuabFee_WrInst_Init();

  EcuabFee_RdMgmt_Init();
}

FUNC(Std_ReturnType, ECUABFEE_PUBLIC_CODE) EcuabFee_Read(uint16 BlockNumber, uint16 BlockOffset
   ,   EcuabFee_VarDataPtrType DataBufferPtr,uint16 Length)
{

  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = ECUABFEE_E_NO_ERROR;

#if(ECUABFEE_DEV_ERROR_DETECT == STD_ON)

  if(EcuabFee_DetChkInit() == E_NOT_OK)
  {
    errorId = ECUABFEE_E_UNINIT;
  }
  else

  if(EcuabFee_DetChkBlockNumberOk(BlockNumber, ECUABFEE_SID_READ) == E_NOT_OK)
  {
    errorId = ECUABFEE_E_INVALID_BLOCK_NO;
  }
  else

  if(EcuabFee_DetChkDataPointerOk(DataBufferPtr) == E_NOT_OK)
  {
    errorId = ECUABFEE_E_INVALID_DATA_POINTER;
  }
  else

  if(EcuabFee_DetChkBlockOffsetOk(BlockNumber, BlockOffset) == E_NOT_OK)
  {
    errorId = ECUABFEE_E_INVALID_BLOCK_OFS;
  }
  else

  if(EcuabFee_DetChkBlockLengthOk(BlockNumber, Length, BlockOffset) == E_NOT_OK)
  {
    errorId = ECUABFEE_E_INVALID_BLOCK_LEN;
  }
  else

#endif

  if(EcuabFee_CheckIdle() == E_NOT_OK)
  {
    errorId = ECUABFEE_E_BUSY;
  }
  else{

    EcuabFee_SetUserParameter(BlockNumber, BlockOffset, DataBufferPtr, Length, ECUABFEE_READ_JOB);
    EcuabFee_SetJobParameter();

    retVal = E_OK;
  }

#if(ECUABFEE_DEV_ERROR_REPORT == STD_ON)
  if(errorId != ECUABFEE_E_NO_ERROR)
  {
    (void)Det_ReportError(ECUABFEE_MODULE_ID, ECUABFEE_INSTANCE_ID, ECUABFEE_SID_READ, errorId);
  }
#else
  FEE_DUMMY_STATEMENT(errorId);
#endif

  return retVal;
}

FUNC(Std_ReturnType, ECUABFEE_PUBLIC_CODE) EcuabFee_Write(uint16 BlockNumber, EcuabFee_VarDataPtrType DataBufferPtr)
{

  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = ECUABFEE_E_NO_ERROR;

#if(ECUABFEE_DEV_ERROR_DETECT == STD_ON)

  if(EcuabFee_DetChkInit() == E_NOT_OK)
  {
    errorId = ECUABFEE_E_UNINIT;
  }
  else

  if(EcuabFee_DetChkBlockNumberOk(BlockNumber, ECUABFEE_SID_WRITE) == E_NOT_OK)
  {
    errorId = ECUABFEE_E_INVALID_BLOCK_NO;
  }
  else

  if(EcuabFee_DetChkDataPointerOk(DataBufferPtr) == E_NOT_OK)
  {
    errorId = ECUABFEE_E_INVALID_DATA_POINTER;
  }
  else

#endif

  if(EcuabFee_CheckIdle() == E_NOT_OK)
  {
    errorId = ECUABFEE_E_BUSY;
  }
  else{

    EcuabFee_SetUserParameter(BlockNumber, 0x0000U, DataBufferPtr, EcuabFee_Bh_GetDataLength(BlockNumber), ECUABFEE_WRITE_JOB);
    EcuabFee_SetJobParameter();

    retVal = E_OK;
  }

#if(ECUABFEE_DEV_ERROR_REPORT == STD_ON)
  if(errorId != ECUABFEE_E_NO_ERROR)
  {
    (void)Det_ReportError(ECUABFEE_MODULE_ID, ECUABFEE_INSTANCE_ID, ECUABFEE_SID_WRITE, errorId);
  }
#else
  FEE_DUMMY_STATEMENT(errorId);
#endif

  return retVal;
}

FUNC(Std_ReturnType, ECUABFEE_PUBLIC_CODE) EcuabFee_InvalidateBlock(uint16 BlockNumber)
{

  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = ECUABFEE_E_NO_ERROR;

#if(ECUABFEE_DEV_ERROR_DETECT == STD_ON)

  if(EcuabFee_DetChkInit() == E_NOT_OK)
  {
    errorId = ECUABFEE_E_UNINIT;
  }
  else

  if(EcuabFee_DetChkBlockNumberOk(BlockNumber, ECUABFEE_SID_INVALIDATE_BLOCK) == E_NOT_OK)
  {
    errorId = ECUABFEE_E_INVALID_BLOCK_NO;
  }
  else

#endif

  if(EcuabFee_CheckIdle() == E_NOT_OK)
  {
    errorId = ECUABFEE_E_BUSY;
  }
  else{

    EcuabFee_SetUserParameter(BlockNumber, 0x0000U, NULL_PTR, 0x0000U, ECUABFEE_INVALIDATE_JOB);
    EcuabFee_SetJobParameter();

    retVal = E_OK;
  }

#if(ECUABFEE_DEV_ERROR_REPORT == STD_ON)
  if(errorId != ECUABFEE_E_NO_ERROR)
  {
    (void)Det_ReportError(ECUABFEE_MODULE_ID, ECUABFEE_INSTANCE_ID, ECUABFEE_SID_INVALIDATE_BLOCK, errorId);
  }
#else
  FEE_DUMMY_STATEMENT(errorId);
#endif

  return retVal;
}

FUNC(Std_ReturnType, ECUABFEE_PUBLIC_CODE) EcuabFee_EraseImmediateBlock(uint16 BlockNumber)
{

  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = ECUABFEE_E_NO_ERROR;

#if(ECUABFEE_DEV_ERROR_DETECT == STD_ON)

  if(EcuabFee_DetChkInit() == E_NOT_OK)
  {
    errorId = ECUABFEE_E_UNINIT;
  }
  else

  if(EcuabFee_DetChkBlockNumberOk(BlockNumber, ECUABFEE_SID_ERASE_IMMEDIATE_BLOCK) == E_NOT_OK)
  {
    errorId = ECUABFEE_E_INVALID_BLOCK_NO;
  }
  else

#endif

  if(EcuabFee_CheckIdle() == E_NOT_OK)
  {
    errorId = ECUABFEE_E_BUSY;
  }
  else{

    EcuabFee_SetUserParameter(BlockNumber, 0x0000U, NULL_PTR, 0x0000U, ECUABFEE_ERASE_JOB);
    EcuabFee_SetJobParameter();

    retVal = E_OK;
  }

#if(ECUABFEE_DEV_ERROR_REPORT == STD_ON)
  if(errorId != ECUABFEE_E_NO_ERROR)
  {
    (void)Det_ReportError(ECUABFEE_MODULE_ID, ECUABFEE_INSTANCE_ID, ECUABFEE_SID_ERASE_IMMEDIATE_BLOCK, errorId);
  }
#else
  FEE_DUMMY_STATEMENT(errorId);
#endif

  return retVal;
}

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_Cancel(void){

  uint8 errorId = ECUABFEE_E_NO_ERROR;

#if(ECUABFEE_DEV_ERROR_DETECT == STD_ON)

  if(EcuabFee_DetChkInit() == E_NOT_OK)
  {
    errorId = ECUABFEE_E_UNINIT;
  }
  else

#endif

  if(EcuabFee_CheckIdle() == E_OK)
  {
    errorId = ECUABFEE_E_INVALID_CANCEL;
  }
  else{

    EcuabFee_SetFlagCancelRequested(TRUE);
    EcuabFee_SetFlagJobRequested(FALSE);

    EcuabFee_ComponentParameter.JobResult = MEMIF_JOB_CANCELED;
    EcuabFee_ComponentParameter.Status = MEMIF_IDLE;
  }

#if(ECUABFEE_DEV_ERROR_REPORT == STD_ON)
  if(errorId != ECUABFEE_E_NO_ERROR)
  {
    (void)Det_ReportError(ECUABFEE_MODULE_ID, ECUABFEE_INSTANCE_ID, ECUABFEE_SID_CANCEL, errorId);
  }
#else
  FEE_DUMMY_STATEMENT(errorId);
#endif
}

FUNC(MemIf_StatusType, ECUABFEE_PUBLIC_CODE) EcuabFee_GetStatus(void){
  return EcuabFee_ComponentParameter.Status;
}

FUNC(MemIf_JobResultType, ECUABFEE_PUBLIC_CODE) EcuabFee_GetJobResult(void){

  MemIf_JobResultType retVal;
  uint8 errorId = ECUABFEE_E_NO_ERROR;

#if(ECUABFEE_DEV_ERROR_DETECT == STD_ON)

  if(EcuabFee_DetChkInit() == E_NOT_OK)
  {
    errorId = ECUABFEE_E_UNINIT;
    retVal = MEMIF_JOB_FAILED;
  }
  else

#endif
  {

    retVal = EcuabFee_ComponentParameter.JobResult;

  }

#if(ECUABFEE_DEV_ERROR_REPORT == STD_ON)
  if(errorId != ECUABFEE_E_NO_ERROR)
  {
    (void)Det_ReportError(ECUABFEE_MODULE_ID, ECUABFEE_INSTANCE_ID, ECUABFEE_SID_GET_JOB_RESULT, errorId);
  }
#else
  FEE_DUMMY_STATEMENT(errorId);
#endif

  return retVal;
}

#if(ECUABFEE_VERSION_INFO_API == STD_ON)

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, ECUABFEE_APPL_DATA) VersionInfoPtr)
{

  uint8 errorId = ECUABFEE_E_NO_ERROR;

# if(ECUABFEE_DEV_ERROR_DETECT == STD_ON)

  if(EcuabFee_DetChkDataPointerOk((void *)VersionInfoPtr) == E_NOT_OK)
  {
    errorId = ECUABFEE_E_INVALID_DATA_POINTER;
  }
  else

# endif
  {

   VersionInfoPtr->vendorID = ECUABFEE_VENDOR_ID;
   VersionInfoPtr->moduleID = ECUABFEE_MODULE_ID;

   VersionInfoPtr->sw_major_version = ECUABFEE_SW_MAJOR_VERSION;
   VersionInfoPtr->sw_minor_version = ECUABFEE_SW_MINOR_VERSION;
   VersionInfoPtr->sw_patch_version = ECUABFEE_SW_PATCH_VERSION;

  }

#if(ECUABFEE_DEV_ERROR_REPORT == STD_ON)
  if(errorId != ECUABFEE_E_NO_ERROR)
  {
    (void)Det_ReportError(ECUABFEE_MODULE_ID, ECUABFEE_INSTANCE_ID, ECUABFEE_SID_GET_VERSION_INFO, errorId);
  }
#else
  FEE_DUMMY_STATEMENT(errorId);
#endif
}

#endif

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_SetMode(MemIf_ModeType Mode)
{

  FEE_DUMMY_STATEMENT(Mode);
}

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_SuspendWrites (void){

  EcuabFee_Wr_SuspendWrites();
}

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_ResumeWrites (void){

  EcuabFee_Wr_ResumeWrites();
}

#if(ECUABFEE_FLS_POLLING_MODE == STD_OFF)

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_JobEndNotification(void){
  EcuabFee_Fls_SetCurrentFlsJobResult();
}

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_JobErrorNotification(void){
  EcuabFee_Fls_SetCurrentFlsJobResult();
}

#endif

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_MainFunction (void){

  uint8 errorId = ECUABFEE_E_NO_ERROR;

#if(ECUABFEE_DEV_ERROR_DETECT == STD_ON)

  if(EcuabFee_DetChkInit() == E_NOT_OK)
  {
    errorId = ECUABFEE_E_UNINIT;
  }
  else

#endif
  {

   if(EcuabFee_InternalFlags.MainFunctionBusy == TRUE)
   {
        EcuabFee_ProcessStateMachine();
   }
  }

#if(ECUABFEE_DEV_ERROR_REPORT == STD_ON)
  if(errorId != ECUABFEE_E_NO_ERROR)
  {
    (void)Det_ReportError(ECUABFEE_MODULE_ID, ECUABFEE_INSTANCE_ID, ECUABFEE_SID_MAINFUNCTION, errorId);
  }
#else
  FEE_DUMMY_STATEMENT(errorId);
#endif
}

FUNC (uint32, ECUABFEE_PUBLIC_CODE) EcuabFee_AlignValue(uint32 Value, uint16 Alignment)
{
  uint32 retVal;

  if((Value % Alignment) == 0)
  {
    retVal = Value;
  }
  else{

    retVal = ((Value / Alignment) + 1u) * Alignment;
  }

  return retVal;
}

#define ECUABFEE_STOP_SEC_CODE
#include "MemMap.hpp"

