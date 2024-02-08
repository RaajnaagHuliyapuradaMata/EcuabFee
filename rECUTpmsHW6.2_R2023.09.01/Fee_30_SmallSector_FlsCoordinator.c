#define EcuabFee_IMPLEMENTATION_SOURCE

#include "Fee_30_SmallSector.hpp"
#include "EcuabFee_FlsCoordinator.hpp"
#include "EcuabFee_PartitionHandler.hpp"

#ifndef EcuabFee_LOCAL
#define EcuabFee_LOCAL static
#endif

#if(EcuabFee_FLS_POLLING_MODE == STD_OFF)
#define EcuabFee_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

EcuabFee_LOCAL VAR(MemIf_JobResultType, EcuabFee_APPL_DATA) EcuabFee_Fls_GlobalJobResult;

#define EcuabFee_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"
#endif

#define EcuabFee_START_SEC_CODE
#include "MemMap.hpp"

EcuabFee_LOCAL FUNC(MemIf_JobResultType, EcuabFee_PRIVATE_CODE) EcuabFee_Fls_GetJobResultFromFlash(void);

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_Fls_SetResultPending(void);

#define EcuabFee_STOP_SEC_CODE
#include "MemMap.hpp"

#define EcuabFee_START_SEC_CODE
#include "MemMap.hpp"

EcuabFee_LOCAL FUNC(MemIf_JobResultType, EcuabFee_PRIVATE_CODE) EcuabFee_Fls_GetJobResultFromFlash(void){
  return EcuabFee_Ph_GetFlsApiPtr()->GetJobResult();
}

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_Fls_SetResultPending(void){
#if(EcuabFee_FLS_POLLING_MODE == STD_OFF)
  EcuabFee_Fls_GlobalJobResult = MEMIF_JOB_PENDING;
#endif
}

#define EcuabFee_STOP_SEC_CODE
#include "MemMap.hpp"

#define EcuabFee_START_SEC_CODE
#include "MemMap.hpp"

FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Fls_Read(EcuabFee_AddressType Address
   ,   EcuabFee_DataPtr BufferPtr, uint16 Length)
{
  EcuabFee_Fls_SetResultPending();
  return EcuabFee_Ph_GetFlsApiPtr()->Read(Address, BufferPtr, Length);
}

FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Fls_Write(EcuabFee_AddressType Address
   ,   EcuabFee_ConstDataPtr BufferPtr, uint16 Length)
{
  EcuabFee_Fls_SetResultPending();
  return EcuabFee_Ph_GetFlsApiPtr()->Write(Address, BufferPtr, Length);
}

FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Fls_Compare(EcuabFee_AddressType Address
   ,   EcuabFee_ConstDataPtr BufferPtr, uint16 Length)
{
  EcuabFee_Fls_SetResultPending();
  return EcuabFee_Ph_GetFlsApiPtr()->Compare(Address, BufferPtr, Length);
}

FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Fls_Erase(EcuabFee_AddressType Address, uint16 Length)
{
  EcuabFee_Fls_SetResultPending();
  return EcuabFee_Ph_GetFlsApiPtr()->Erase(Address, Length);
}

FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Fls_BlankCheck(EcuabFee_AddressType Address, uint16 Length)
{
  EcuabFee_Fls_SetResultPending();
  return EcuabFee_Ph_GetFlsApiPtr()->BlankCheck(Address, Length);
}

#if(EcuabFee_FLS_POLLING_MODE == STD_OFF)

FUNC (void, EcuabFee_PUBLIC_CODE) EcuabFee_Fls_SetCurrentFlsJobResult(void){
  EcuabFee_Fls_GlobalJobResult = EcuabFee_Fls_GetJobResultFromFlash();
}
#endif

FUNC (MemIf_JobResultType, EcuabFee_PUBLIC_CODE) EcuabFee_Fls_GetJobResult(void){
#if(EcuabFee_FLS_POLLING_MODE == STD_ON)

  return EcuabFee_Fls_GetJobResultFromFlash();

#else

  return EcuabFee_Fls_GlobalJobResult;

#endif
}

FUNC (MemIf_StatusType, EcuabFee_PUBLIC_CODE) EcuabFee_Fls_GetStatus(void){
  return EcuabFee_Ph_GetFlsApiPtr()->GetStatus();
}

#define EcuabFee_STOP_SEC_CODE
#include "MemMap.hpp"

