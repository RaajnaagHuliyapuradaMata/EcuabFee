#define FEE_30_SMALLSECTOR_IMPLEMENTATION_SOURCE

#include "Fee_30_SmallSector.hpp"
#include "Fee_30_SmallSector_FlsCoordinator.hpp"
#include "Fee_30_SmallSector_PartitionHandler.hpp"

#ifndef FEE_30_SMALLSECTOR_LOCAL
#define FEE_30_SMALLSECTOR_LOCAL static
#endif

#if(FEE_30_SMALLSECTOR_FLS_POLLING_MODE == STD_OFF)
#define FEE_30_SMALLSECTOR_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL VAR(MemIf_JobResultType, FEE_30_SMALLSECTOR_APPL_DATA) Fee_30_SmallSector_Fls_GlobalJobResult;

#define FEE_30_SMALLSECTOR_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"
#endif

#define FEE_30_SMALLSECTOR_START_SEC_CODE
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL FUNC(MemIf_JobResultType, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Fls_GetJobResultFromFlash(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Fls_SetResultPending(void);

#define FEE_30_SMALLSECTOR_STOP_SEC_CODE
#include "MemMap.hpp"

#define FEE_30_SMALLSECTOR_START_SEC_CODE
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL FUNC(MemIf_JobResultType, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Fls_GetJobResultFromFlash(void){
  return Fee_30_SmallSector_Ph_GetFlsApiPtr()->GetJobResult();
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Fls_SetResultPending(void){
#if(FEE_30_SMALLSECTOR_FLS_POLLING_MODE == STD_OFF)
  Fee_30_SmallSector_Fls_GlobalJobResult = MEMIF_JOB_PENDING;
#endif
}

#define FEE_30_SMALLSECTOR_STOP_SEC_CODE
#include "MemMap.hpp"

#define FEE_30_SMALLSECTOR_START_SEC_CODE
#include "MemMap.hpp"

FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Fls_Read(Fee_30_SmallSector_AddressType Address
   ,   Fee_30_SmallSector_DataPtr BufferPtr, uint16 Length)
{
  Fee_30_SmallSector_Fls_SetResultPending();
  return Fee_30_SmallSector_Ph_GetFlsApiPtr()->Read(Address, BufferPtr, Length);
}

FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Fls_Write(Fee_30_SmallSector_AddressType Address
   ,   Fee_30_SmallSector_ConstDataPtr BufferPtr, uint16 Length)
{
  Fee_30_SmallSector_Fls_SetResultPending();
  return Fee_30_SmallSector_Ph_GetFlsApiPtr()->Write(Address, BufferPtr, Length);
}

FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Fls_Compare(Fee_30_SmallSector_AddressType Address
   ,   Fee_30_SmallSector_ConstDataPtr BufferPtr, uint16 Length)
{
  Fee_30_SmallSector_Fls_SetResultPending();
  return Fee_30_SmallSector_Ph_GetFlsApiPtr()->Compare(Address, BufferPtr, Length);
}

FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Fls_Erase(Fee_30_SmallSector_AddressType Address, uint16 Length)
{
  Fee_30_SmallSector_Fls_SetResultPending();
  return Fee_30_SmallSector_Ph_GetFlsApiPtr()->Erase(Address, Length);
}

FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Fls_BlankCheck(Fee_30_SmallSector_AddressType Address, uint16 Length)
{
  Fee_30_SmallSector_Fls_SetResultPending();
  return Fee_30_SmallSector_Ph_GetFlsApiPtr()->BlankCheck(Address, Length);
}

#if(FEE_30_SMALLSECTOR_FLS_POLLING_MODE == STD_OFF)

FUNC (void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Fls_SetCurrentFlsJobResult(void){
  Fee_30_SmallSector_Fls_GlobalJobResult = Fee_30_SmallSector_Fls_GetJobResultFromFlash();
}
#endif

FUNC (MemIf_JobResultType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Fls_GetJobResult(void){
#if(FEE_30_SMALLSECTOR_FLS_POLLING_MODE == STD_ON)

  return Fee_30_SmallSector_Fls_GetJobResultFromFlash();

#else

  return Fee_30_SmallSector_Fls_GlobalJobResult;

#endif
}

FUNC (MemIf_StatusType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Fls_GetStatus(void){
  return Fee_30_SmallSector_Ph_GetFlsApiPtr()->GetStatus();
}

#define FEE_30_SMALLSECTOR_STOP_SEC_CODE
#include "MemMap.hpp"

