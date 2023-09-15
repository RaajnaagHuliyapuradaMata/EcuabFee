

#define FEE_30_SMALLSECTOR_IMPLEMENTATION_SOURCE

#include "Fee_30_SmallSector.hpp"
#include "Fee_30_SmallSector_Cbk.hpp"

#include "Fee_30_SmallSector_TaskManager.hpp"
#include "Fee_30_SmallSector_FlsCoordinator.hpp"

#include "Fee_30_SmallSector_PartitionHandler.hpp"
#include "Fee_30_SmallSector_BlockHandler.hpp"
#include "Fee_30_SmallSector_DatasetHandler.hpp"

#include "Fee_30_SmallSector_Layer1_Read.hpp"
#include "Fee_30_SmallSector_Layer1_Write.hpp"

#include "Fee_30_SmallSector_Layer2_WriteInstance.hpp"
#include "Fee_30_SmallSector_Layer2_DatasetEraser.hpp"
#include "Fee_30_SmallSector_Layer2_InstanceFinder.hpp"

#include "Fee_30_SmallSector_Layer3_ReadManagementBytes.hpp"

#if(   (FEE_30_SMALLSECTOR_SW_MAJOR_VERSION != (2u)) \
     || (FEE_30_SMALLSECTOR_SW_MINOR_VERSION != (0u)) \
     || (FEE_30_SMALLSECTOR_AR_RELEASE_MAJOR_VERSION != (4u)) \
     || (FEE_30_SMALLSECTOR_AR_RELEASE_MINOR_VERSION != (0u)))
# error "Version numbers of Fee_30_SmallSector.c and Fee_30_SmallSector.h are inconsistent!"
#endif

#if(   (FEE_30_SMALLSECTOR_CFG_MAJOR_VERSION != (2u)) \
     || (FEE_30_SMALLSECTOR_CFG_MINOR_VERSION != (0u)))
# error "Version numbers of Fee_30_SmallSector.c and Fee_30_SmallSector_Cfg.h are inconsistent!"
#endif

#if(   (FEE_30_SMALLSECTOR_CBK_MAJOR_VERSION != (2u)) \
     || (FEE_30_SMALLSECTOR_CBK_MINOR_VERSION != (0u)))
# error "Version numbers of Fee_30_SmallSector.c and Fee_30_SmallSector_Cbk.h are inconsistent!"
#endif

#define FEE_30_SMALLSECTOR_RESERVED_BLOCK_NUMBER_1             (0x0000u)
#define FEE_30_SMALLSECTOR_RESERVED_BLOCK_NUMBER_2             (0xFFFFu)

#if !defined (FEE_30_SMALLSECTOR_LOCAL)
#define FEE_30_SMALLSECTOR_LOCAL static
#endif

#if !defined (FEE_30_SMALLSECTOR_LOCAL_INLINE)
#define FEE_30_SMALLSECTOR_LOCAL_INLINE LOCAL_INLINE
#endif

typedef struct{
  MemIf_JobResultType JobResult;
  MemIf_StatusType Status;
}Fee_30_SmallSector_ComponentParameterType;

typedef enum{
  FEE_30_SMALLSECTOR_STATE_UNINIT = 0
   ,  FEE_30_SMALLSECTOR_STATE_IDLE
   ,  FEE_30_SMALLSECTOR_STATE_BUSY
}EFeeStateType;

typedef struct{
  boolean MainFunctionBusy;
  boolean JobRequested;
  boolean CancelRequested;
}Fee_30_SmallSector_InternalFlagsType;

#define FEE_30_SMALLSECTOR_START_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL VAR(Fee_30_SmallSector_ComponentParameterType, FEE_30_SMALLSECTOR_FAST_DATA) Fee_30_SmallSector_ComponentParameter = {MEMIF_JOB_FAILED, MEMIF_UNINIT};

FEE_30_SMALLSECTOR_LOCAL VAR(EFeeStateType, FEE_30_SMALLSECTOR_APPL_DATA) Fee_30_SmallSector_StateMachine = FEE_30_SMALLSECTOR_STATE_UNINIT;

#define FEE_30_SMALLSECTOR_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

#define FEE_30_SMALLSECTOR_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL VAR(Fee_30_SmallSector_UserJobParameterType, FEE_30_SMALLSECTOR_APPL_DATA) Fee_30_SmallSector_UserJobParameter;

FEE_30_SMALLSECTOR_LOCAL VAR(Fee_30_SmallSector_InternalFlagsType, FEE_30_SMALLSECTOR_APPL_DATA) Fee_30_SmallSector_InternalFlags;

#define FEE_30_SMALLSECTOR_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define FEE_30_SMALLSECTOR_START_SEC_CODE
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_SetFlagJobRequested(boolean value);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_SetFlagCancelRequested(boolean value);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_SetFlagMainFunctionBusy(boolean value);

FEE_30_SMALLSECTOR_LOCAL FUNC(MemIf_StatusType, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_GetLayer1Status(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(MemIf_JobResultType, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_GetLayer1JobResult(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_StartLayer1Job(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_SetUserParameter(uint16 BlockNumber
   ,                                                            uint16 BlockOffset
   ,                                                            Fee_30_SmallSector_DataPtr DataBufferPtr
   ,                                                            uint16 Length
   ,                                                            Fee_30_SmallSector_JobType JobType);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_SetJobParameter(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_StartJob(void);

FEE_30_SMALLSECTOR_LOCAL_INLINE FUNC(Std_ReturnType, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_CheckIdle(void);

#if(FEE_30_SMALLSECTOR_DEV_ERROR_DETECT == STD_ON)

FEE_30_SMALLSECTOR_LOCAL_INLINE FUNC(Std_ReturnType, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_DetChkInit(void);

FEE_30_SMALLSECTOR_LOCAL_INLINE FUNC(Std_ReturnType, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_DetChkBlockNumberOk(uint16 BlockNumber, uint8 ApiID);

FEE_30_SMALLSECTOR_LOCAL_INLINE FUNC(Std_ReturnType, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_DetChkDataPointerOk(
        P2CONST(void, AUTOMATIC, FEE_30_SMALLSECTOR_APPL_DATA) DataBufferPtr);

FEE_30_SMALLSECTOR_LOCAL_INLINE FUNC(Std_ReturnType, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_DetChkBlockOffsetOk(uint16 BlockNumber, uint16 BlockOffset);

FEE_30_SMALLSECTOR_LOCAL_INLINE FUNC(Std_ReturnType, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_DetChkBlockLengthOk(uint16 BlockNumber
   ,       uint16 Length, uint16 BlockOffset);
#endif

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_InitDataHandler(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_ProcessStateMachine(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_FinishJob(MemIf_JobResultType JobResult);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_SetFlagJobRequested(boolean value)
{
  Fee_30_SmallSector_InternalFlags.JobRequested = value;
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_SetFlagCancelRequested(boolean value)
{
  Fee_30_SmallSector_InternalFlags.CancelRequested = value;
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_SetFlagMainFunctionBusy(boolean value)
{
  Fee_30_SmallSector_InternalFlags.MainFunctionBusy = value;
}

FEE_30_SMALLSECTOR_LOCAL FUNC(MemIf_StatusType, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_GetLayer1Status(void){

  MemIf_StatusType retVal;

  switch(Fee_30_SmallSector_UserJobParameter.JobType)
  {
   case (FEE_30_SMALLSECTOR_READ_JOB):
      retVal = Fee_30_SmallSector_Rd_GetStatus();
      break;
    default:
      retVal = Fee_30_SmallSector_Wr_GetStatus();
      break;
  }
  return retVal;
}

FEE_30_SMALLSECTOR_LOCAL FUNC(MemIf_JobResultType, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_GetLayer1JobResult(void){

  MemIf_JobResultType retVal;

  switch(Fee_30_SmallSector_UserJobParameter.JobType)
  {
   case (FEE_30_SMALLSECTOR_READ_JOB):
      retVal = Fee_30_SmallSector_Rd_GetJobResult();
      break;
    default:
      retVal = Fee_30_SmallSector_Wr_GetJobResult();
      break;
  }
  return retVal;
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_StartLayer1Job(void){

  switch(Fee_30_SmallSector_UserJobParameter.JobType)
  {
   case (FEE_30_SMALLSECTOR_READ_JOB):
      (void) Fee_30_SmallSector_Rd_StartJob(Fee_30_SmallSector_UserJobParameter);
      break;
    default:
      (void) Fee_30_SmallSector_Wr_StartJob(Fee_30_SmallSector_UserJobParameter);
      break;
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_SetUserParameter(uint16 BlockNumber
   ,                                                           uint16 BlockOffset
   ,                                                           Fee_30_SmallSector_DataPtr DataBufferPtr
   ,                                                           uint16 Length
   ,                                                           Fee_30_SmallSector_JobType JobType)
{
  Fee_30_SmallSector_UserJobParameter.BlockNumber = BlockNumber;
  Fee_30_SmallSector_UserJobParameter.BlockOffset = BlockOffset;
  Fee_30_SmallSector_UserJobParameter.DataBufferPtr = DataBufferPtr;
  Fee_30_SmallSector_UserJobParameter.Length = Length;
  Fee_30_SmallSector_UserJobParameter.JobType = JobType;
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_SetJobParameter(void){
  Fee_30_SmallSector_ComponentParameter.Status = MEMIF_BUSY;
  Fee_30_SmallSector_ComponentParameter.JobResult = MEMIF_JOB_PENDING;

  Fee_30_SmallSector_SetFlagJobRequested(TRUE);
  Fee_30_SmallSector_SetFlagMainFunctionBusy(TRUE);
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_StartJob(void){

  Fee_30_SmallSector_Tm_Init();

  Fee_30_SmallSector_InitDataHandler();

  Fee_30_SmallSector_StartLayer1Job();

  Fee_30_SmallSector_StateMachine = FEE_30_SMALLSECTOR_STATE_BUSY;
}

FEE_30_SMALLSECTOR_LOCAL_INLINE FUNC(Std_ReturnType, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_CheckIdle(void){
  return (Std_ReturnType) ((Fee_30_SmallSector_ComponentParameter.Status == MEMIF_IDLE) ? E_OK : E_NOT_OK);
}

#if(FEE_30_SMALLSECTOR_DEV_ERROR_DETECT == STD_ON)

FEE_30_SMALLSECTOR_LOCAL_INLINE FUNC(Std_ReturnType, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_DetChkInit(void){
  return (Std_ReturnType) ((Fee_30_SmallSector_ComponentParameter.Status == MEMIF_UNINIT) ? E_NOT_OK : E_OK);
}

FEE_30_SMALLSECTOR_LOCAL_INLINE FUNC(Std_ReturnType, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_DetChkBlockNumberOk(uint16 BlockNumber, uint8 ApiID)
{
  Std_ReturnType retVal;

  if((BlockNumber == (uint16)FEE_30_SMALLSECTOR_RESERVED_BLOCK_NUMBER_1) ||
      (BlockNumber == (uint16)FEE_30_SMALLSECTOR_RESERVED_BLOCK_NUMBER_2))
  {
    retVal = E_NOT_OK;
  }

  else if((Fee_30_SmallSector_Bh_GetBlockIndex(BlockNumber) + 1u) > FEE_30_SMALLSECTOR_NUMBER_OF_BLOCKS)
  {
    retVal = E_NOT_OK;
  }
  else if(ApiID == (uint8) FEE_30_SMALLSECTOR_SID_ERASE_IMMEDIATE_BLOCK)
  {

    retVal = ( (Fee_30_SmallSector_Bh_IsImmediateData(BlockNumber) == FALSE) ? E_NOT_OK : E_OK);
  }

  else if(Fee_30_SmallSector_Bh_GetNrOfDatasets(BlockNumber) <= Fee_30_SmallSector_Bh_GetDatasetIndex(BlockNumber))
  {
    retVal = E_NOT_OK;
  }
  else{
    retVal = E_OK;
  }
  return retVal;
}

FEE_30_SMALLSECTOR_LOCAL_INLINE FUNC(Std_ReturnType, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_DetChkDataPointerOk(
        P2CONST(void, AUTOMATIC, FEE_30_SMALLSECTOR_APPL_DATA) DataBufferPtr)
{
  return (Std_ReturnType) ((DataBufferPtr == NULL_PTR) ? E_NOT_OK : E_OK);
}

FEE_30_SMALLSECTOR_LOCAL_INLINE FUNC(Std_ReturnType, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_DetChkBlockOffsetOk(uint16 BlockNumber, uint16 BlockOffset)
{
  return (Std_ReturnType) ((BlockOffset >= Fee_30_SmallSector_Bh_GetDataLength(BlockNumber)) ? E_NOT_OK : E_OK);
}

FEE_30_SMALLSECTOR_LOCAL_INLINE FUNC(Std_ReturnType, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_DetChkBlockLengthOk(uint16 BlockNumber
   ,       uint16 Length, uint16 BlockOffset)
{
  Std_ReturnType retVal;

  if(Length == 0u)
  {
    retVal = E_NOT_OK;
  }

  else if( Fee_30_SmallSector_Bh_GetDataLength(BlockNumber) < (Length + BlockOffset) )
  {
    retVal = E_NOT_OK;
  }
  else{
    retVal = E_OK;
  }

  return (Std_ReturnType) retVal;
}
#endif

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_InitDataHandler(void){
  Fee_30_SmallSector_Ph_Init(Fee_30_SmallSector_UserJobParameter.BlockNumber);
  Fee_30_SmallSector_Bh_Init(Fee_30_SmallSector_UserJobParameter.BlockNumber, Fee_30_SmallSector_Ph_GetPartitionStartAddress());
  Fee_30_SmallSector_Dh_Init(Fee_30_SmallSector_UserJobParameter.BlockNumber, Fee_30_SmallSector_Bh_GetDataLength(Fee_30_SmallSector_UserJobParameter.BlockNumber)
   ,  Fee_30_SmallSector_Bh_GetNrOfInstances(Fee_30_SmallSector_UserJobParameter.BlockNumber), Fee_30_SmallSector_Bh_GetBlockStartAddress(), Fee_30_SmallSector_Ph_GetPartitionAddressAlignment());
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_ProcessStateMachine(void){
  switch(Fee_30_SmallSector_StateMachine)
  {

   case FEE_30_SMALLSECTOR_STATE_IDLE:
      if(Fee_30_SmallSector_InternalFlags.CancelRequested == TRUE)
      {

        Fee_30_SmallSector_SetFlagCancelRequested(FALSE);
      }
      else{

        Fee_30_SmallSector_StartJob();
        Fee_30_SmallSector_SetFlagJobRequested(FALSE);
      }

      break;

   case FEE_30_SMALLSECTOR_STATE_BUSY:
      if(Fee_30_SmallSector_InternalFlags.CancelRequested == TRUE)
      {
        Fee_30_SmallSector_Tm_CancelTasks();

        if(Fee_30_SmallSector_InternalFlags.JobRequested == FALSE)
        {
          Fee_30_SmallSector_SetFlagMainFunctionBusy(FALSE);
        }

        Fee_30_SmallSector_StateMachine = FEE_30_SMALLSECTOR_STATE_IDLE;
        Fee_30_SmallSector_SetFlagCancelRequested(FALSE);
      }
      else{
        if(Fee_30_SmallSector_Fls_GetStatus() == MEMIF_IDLE)
        {

          Fee_30_SmallSector_Tm_ExecuteTask();

          if((Fee_30_SmallSector_GetLayer1Status() == MEMIF_IDLE) && (Fee_30_SmallSector_InternalFlags.CancelRequested == FALSE))
          {
            Fee_30_SmallSector_FinishJob(Fee_30_SmallSector_GetLayer1JobResult());
          }
        }
      }
      break;
    default:
      break;
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_FinishJob(MemIf_JobResultType JobResult)
{

  Fee_30_SmallSector_SetFlagMainFunctionBusy(FALSE);
  Fee_30_SmallSector_StateMachine = FEE_30_SMALLSECTOR_STATE_IDLE;

  Fee_30_SmallSector_ComponentParameter.JobResult = JobResult;
  Fee_30_SmallSector_ComponentParameter.Status = MEMIF_IDLE;

#if(FEE_30_SMALLSECTOR_NVM_POLLING_MODE == STD_OFF)
  if(JobResult == MEMIF_JOB_OK)
  {
    Fee_30_SmallSector_CbkJobEndNotification();
  }
  else{
    Fee_30_SmallSector_CbkJobErrorNotification();
  }
#endif
}

FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Init(void){
  Fee_30_SmallSector_ComponentParameter.JobResult = MEMIF_JOB_OK;
  Fee_30_SmallSector_ComponentParameter.Status = MEMIF_IDLE;

  Fee_30_SmallSector_StateMachine = FEE_30_SMALLSECTOR_STATE_IDLE;

  Fee_30_SmallSector_SetFlagMainFunctionBusy(FALSE);
  Fee_30_SmallSector_SetFlagJobRequested(FALSE);
  Fee_30_SmallSector_SetFlagCancelRequested(FALSE);

  Fee_30_SmallSector_Rd_Init();
  Fee_30_SmallSector_Wr_Init();

  Fee_30_SmallSector_If_Init();
  Fee_30_SmallSector_De_Init();
  Fee_30_SmallSector_WrInst_Init();

  Fee_30_SmallSector_RdMgmt_Init();
}

FUNC(Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Read(uint16 BlockNumber, uint16 BlockOffset
   ,   Fee_30_SmallSector_VarDataPtrType DataBufferPtr,uint16 Length)
{

  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FEE_30_SMALLSECTOR_E_NO_ERROR;

#if(FEE_30_SMALLSECTOR_DEV_ERROR_DETECT == STD_ON)

  if(Fee_30_SmallSector_DetChkInit() == E_NOT_OK)
  {
    errorId = FEE_30_SMALLSECTOR_E_UNINIT;
  }
  else

  if(Fee_30_SmallSector_DetChkBlockNumberOk(BlockNumber, FEE_30_SMALLSECTOR_SID_READ) == E_NOT_OK)
  {
    errorId = FEE_30_SMALLSECTOR_E_INVALID_BLOCK_NO;
  }
  else

  if(Fee_30_SmallSector_DetChkDataPointerOk(DataBufferPtr) == E_NOT_OK)
  {
    errorId = FEE_30_SMALLSECTOR_E_INVALID_DATA_POINTER;
  }
  else

  if(Fee_30_SmallSector_DetChkBlockOffsetOk(BlockNumber, BlockOffset) == E_NOT_OK)
  {
    errorId = FEE_30_SMALLSECTOR_E_INVALID_BLOCK_OFS;
  }
  else

  if(Fee_30_SmallSector_DetChkBlockLengthOk(BlockNumber, Length, BlockOffset) == E_NOT_OK)
  {
    errorId = FEE_30_SMALLSECTOR_E_INVALID_BLOCK_LEN;
  }
  else

#endif

  if(Fee_30_SmallSector_CheckIdle() == E_NOT_OK)
  {
    errorId = FEE_30_SMALLSECTOR_E_BUSY;
  }
  else{

    Fee_30_SmallSector_SetUserParameter(BlockNumber, BlockOffset, DataBufferPtr, Length, FEE_30_SMALLSECTOR_READ_JOB);
    Fee_30_SmallSector_SetJobParameter();

    retVal = E_OK;
  }

#if(FEE_30_SMALLSECTOR_DEV_ERROR_REPORT == STD_ON)
  if(errorId != FEE_30_SMALLSECTOR_E_NO_ERROR)
  {
    (void)Det_ReportError(FEE_30_SMALLSECTOR_MODULE_ID, FEE_30_SMALLSECTOR_INSTANCE_ID, FEE_30_SMALLSECTOR_SID_READ, errorId);
  }
#else
  FEE_DUMMY_STATEMENT(errorId);
#endif

  return retVal;
}

FUNC(Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Write(uint16 BlockNumber, Fee_30_SmallSector_VarDataPtrType DataBufferPtr)
{

  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FEE_30_SMALLSECTOR_E_NO_ERROR;

#if(FEE_30_SMALLSECTOR_DEV_ERROR_DETECT == STD_ON)

  if(Fee_30_SmallSector_DetChkInit() == E_NOT_OK)
  {
    errorId = FEE_30_SMALLSECTOR_E_UNINIT;
  }
  else

  if(Fee_30_SmallSector_DetChkBlockNumberOk(BlockNumber, FEE_30_SMALLSECTOR_SID_WRITE) == E_NOT_OK)
  {
    errorId = FEE_30_SMALLSECTOR_E_INVALID_BLOCK_NO;
  }
  else

  if(Fee_30_SmallSector_DetChkDataPointerOk(DataBufferPtr) == E_NOT_OK)
  {
    errorId = FEE_30_SMALLSECTOR_E_INVALID_DATA_POINTER;
  }
  else

#endif

  if(Fee_30_SmallSector_CheckIdle() == E_NOT_OK)
  {
    errorId = FEE_30_SMALLSECTOR_E_BUSY;
  }
  else{

    Fee_30_SmallSector_SetUserParameter(BlockNumber, 0x0000U, DataBufferPtr, Fee_30_SmallSector_Bh_GetDataLength(BlockNumber), FEE_30_SMALLSECTOR_WRITE_JOB);
    Fee_30_SmallSector_SetJobParameter();

    retVal = E_OK;
  }

#if(FEE_30_SMALLSECTOR_DEV_ERROR_REPORT == STD_ON)
  if(errorId != FEE_30_SMALLSECTOR_E_NO_ERROR)
  {
    (void)Det_ReportError(FEE_30_SMALLSECTOR_MODULE_ID, FEE_30_SMALLSECTOR_INSTANCE_ID, FEE_30_SMALLSECTOR_SID_WRITE, errorId);
  }
#else
  FEE_DUMMY_STATEMENT(errorId);
#endif

  return retVal;
}

FUNC(Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_InvalidateBlock(uint16 BlockNumber)
{

  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FEE_30_SMALLSECTOR_E_NO_ERROR;

#if(FEE_30_SMALLSECTOR_DEV_ERROR_DETECT == STD_ON)

  if(Fee_30_SmallSector_DetChkInit() == E_NOT_OK)
  {
    errorId = FEE_30_SMALLSECTOR_E_UNINIT;
  }
  else

  if(Fee_30_SmallSector_DetChkBlockNumberOk(BlockNumber, FEE_30_SMALLSECTOR_SID_INVALIDATE_BLOCK) == E_NOT_OK)
  {
    errorId = FEE_30_SMALLSECTOR_E_INVALID_BLOCK_NO;
  }
  else

#endif

  if(Fee_30_SmallSector_CheckIdle() == E_NOT_OK)
  {
    errorId = FEE_30_SMALLSECTOR_E_BUSY;
  }
  else{

    Fee_30_SmallSector_SetUserParameter(BlockNumber, 0x0000U, NULL_PTR, 0x0000U, FEE_30_SMALLSECTOR_INVALIDATE_JOB);
    Fee_30_SmallSector_SetJobParameter();

    retVal = E_OK;
  }

#if(FEE_30_SMALLSECTOR_DEV_ERROR_REPORT == STD_ON)
  if(errorId != FEE_30_SMALLSECTOR_E_NO_ERROR)
  {
    (void)Det_ReportError(FEE_30_SMALLSECTOR_MODULE_ID, FEE_30_SMALLSECTOR_INSTANCE_ID, FEE_30_SMALLSECTOR_SID_INVALIDATE_BLOCK, errorId);
  }
#else
  FEE_DUMMY_STATEMENT(errorId);
#endif

  return retVal;
}

FUNC(Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_EraseImmediateBlock(uint16 BlockNumber)
{

  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FEE_30_SMALLSECTOR_E_NO_ERROR;

#if(FEE_30_SMALLSECTOR_DEV_ERROR_DETECT == STD_ON)

  if(Fee_30_SmallSector_DetChkInit() == E_NOT_OK)
  {
    errorId = FEE_30_SMALLSECTOR_E_UNINIT;
  }
  else

  if(Fee_30_SmallSector_DetChkBlockNumberOk(BlockNumber, FEE_30_SMALLSECTOR_SID_ERASE_IMMEDIATE_BLOCK) == E_NOT_OK)
  {
    errorId = FEE_30_SMALLSECTOR_E_INVALID_BLOCK_NO;
  }
  else

#endif

  if(Fee_30_SmallSector_CheckIdle() == E_NOT_OK)
  {
    errorId = FEE_30_SMALLSECTOR_E_BUSY;
  }
  else{

    Fee_30_SmallSector_SetUserParameter(BlockNumber, 0x0000U, NULL_PTR, 0x0000U, FEE_30_SMALLSECTOR_ERASE_JOB);
    Fee_30_SmallSector_SetJobParameter();

    retVal = E_OK;
  }

#if(FEE_30_SMALLSECTOR_DEV_ERROR_REPORT == STD_ON)
  if(errorId != FEE_30_SMALLSECTOR_E_NO_ERROR)
  {
    (void)Det_ReportError(FEE_30_SMALLSECTOR_MODULE_ID, FEE_30_SMALLSECTOR_INSTANCE_ID, FEE_30_SMALLSECTOR_SID_ERASE_IMMEDIATE_BLOCK, errorId);
  }
#else
  FEE_DUMMY_STATEMENT(errorId);
#endif

  return retVal;
}

FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Cancel(void){

  uint8 errorId = FEE_30_SMALLSECTOR_E_NO_ERROR;

#if(FEE_30_SMALLSECTOR_DEV_ERROR_DETECT == STD_ON)

  if(Fee_30_SmallSector_DetChkInit() == E_NOT_OK)
  {
    errorId = FEE_30_SMALLSECTOR_E_UNINIT;
  }
  else

#endif

  if(Fee_30_SmallSector_CheckIdle() == E_OK)
  {
    errorId = FEE_30_SMALLSECTOR_E_INVALID_CANCEL;
  }
  else{

    Fee_30_SmallSector_SetFlagCancelRequested(TRUE);
    Fee_30_SmallSector_SetFlagJobRequested(FALSE);

    Fee_30_SmallSector_ComponentParameter.JobResult = MEMIF_JOB_CANCELED;
    Fee_30_SmallSector_ComponentParameter.Status = MEMIF_IDLE;
  }

#if(FEE_30_SMALLSECTOR_DEV_ERROR_REPORT == STD_ON)
  if(errorId != FEE_30_SMALLSECTOR_E_NO_ERROR)
  {
    (void)Det_ReportError(FEE_30_SMALLSECTOR_MODULE_ID, FEE_30_SMALLSECTOR_INSTANCE_ID, FEE_30_SMALLSECTOR_SID_CANCEL, errorId);
  }
#else
  FEE_DUMMY_STATEMENT(errorId);
#endif
}

FUNC(MemIf_StatusType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_GetStatus(void){
  return Fee_30_SmallSector_ComponentParameter.Status;
}

FUNC(MemIf_JobResultType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_GetJobResult(void){

  MemIf_JobResultType retVal;
  uint8 errorId = FEE_30_SMALLSECTOR_E_NO_ERROR;

#if(FEE_30_SMALLSECTOR_DEV_ERROR_DETECT == STD_ON)

  if(Fee_30_SmallSector_DetChkInit() == E_NOT_OK)
  {
    errorId = FEE_30_SMALLSECTOR_E_UNINIT;
    retVal = MEMIF_JOB_FAILED;
  }
  else

#endif
  {

    retVal = Fee_30_SmallSector_ComponentParameter.JobResult;

  }

#if(FEE_30_SMALLSECTOR_DEV_ERROR_REPORT == STD_ON)
  if(errorId != FEE_30_SMALLSECTOR_E_NO_ERROR)
  {
    (void)Det_ReportError(FEE_30_SMALLSECTOR_MODULE_ID, FEE_30_SMALLSECTOR_INSTANCE_ID, FEE_30_SMALLSECTOR_SID_GET_JOB_RESULT, errorId);
  }
#else
  FEE_DUMMY_STATEMENT(errorId);
#endif

  return retVal;
}

#if(FEE_30_SMALLSECTOR_VERSION_INFO_API == STD_ON)

FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, FEE_30_SMALLSECTOR_APPL_DATA) VersionInfoPtr)
{

  uint8 errorId = FEE_30_SMALLSECTOR_E_NO_ERROR;

# if(FEE_30_SMALLSECTOR_DEV_ERROR_DETECT == STD_ON)

  if(Fee_30_SmallSector_DetChkDataPointerOk((void *)VersionInfoPtr) == E_NOT_OK)
  {
    errorId = FEE_30_SMALLSECTOR_E_INVALID_DATA_POINTER;
  }
  else

# endif
  {

   VersionInfoPtr->vendorID = FEE_30_SMALLSECTOR_VENDOR_ID;
   VersionInfoPtr->moduleID = FEE_30_SMALLSECTOR_MODULE_ID;

   VersionInfoPtr->sw_major_version = FEE_30_SMALLSECTOR_SW_MAJOR_VERSION;
   VersionInfoPtr->sw_minor_version = FEE_30_SMALLSECTOR_SW_MINOR_VERSION;
   VersionInfoPtr->sw_patch_version = FEE_30_SMALLSECTOR_SW_PATCH_VERSION;

  }

#if(FEE_30_SMALLSECTOR_DEV_ERROR_REPORT == STD_ON)
  if(errorId != FEE_30_SMALLSECTOR_E_NO_ERROR)
  {
    (void)Det_ReportError(FEE_30_SMALLSECTOR_MODULE_ID, FEE_30_SMALLSECTOR_INSTANCE_ID, FEE_30_SMALLSECTOR_SID_GET_VERSION_INFO, errorId);
  }
#else
  FEE_DUMMY_STATEMENT(errorId);
#endif
}

#endif

FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_SetMode(MemIf_ModeType Mode)
{

  FEE_DUMMY_STATEMENT(Mode);
}

FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_SuspendWrites (void){

  Fee_30_SmallSector_Wr_SuspendWrites();
}

FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_ResumeWrites (void){

  Fee_30_SmallSector_Wr_ResumeWrites();
}

#if(FEE_30_SMALLSECTOR_FLS_POLLING_MODE == STD_OFF)

FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_JobEndNotification(void){
  Fee_30_SmallSector_Fls_SetCurrentFlsJobResult();
}

FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_JobErrorNotification(void){
  Fee_30_SmallSector_Fls_SetCurrentFlsJobResult();
}

#endif

FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_MainFunction (void){

  uint8 errorId = FEE_30_SMALLSECTOR_E_NO_ERROR;

#if(FEE_30_SMALLSECTOR_DEV_ERROR_DETECT == STD_ON)

  if(Fee_30_SmallSector_DetChkInit() == E_NOT_OK)
  {
    errorId = FEE_30_SMALLSECTOR_E_UNINIT;
  }
  else

#endif
  {

   if(Fee_30_SmallSector_InternalFlags.MainFunctionBusy == TRUE)
   {
        Fee_30_SmallSector_ProcessStateMachine();
   }
  }

#if(FEE_30_SMALLSECTOR_DEV_ERROR_REPORT == STD_ON)
  if(errorId != FEE_30_SMALLSECTOR_E_NO_ERROR)
  {
    (void)Det_ReportError(FEE_30_SMALLSECTOR_MODULE_ID, FEE_30_SMALLSECTOR_INSTANCE_ID, FEE_30_SMALLSECTOR_SID_MAINFUNCTION, errorId);
  }
#else
  FEE_DUMMY_STATEMENT(errorId);
#endif
}

FUNC (uint32, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_AlignValue(uint32 Value, uint16 Alignment)
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

#define FEE_30_SMALLSECTOR_STOP_SEC_CODE
#include "MemMap.hpp"

