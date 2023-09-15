

#define ECUABFEE_IMPLEMENTATION_SOURCE

#include "EcuabFee.hpp"
#include "EcuabFee_FlsCoordinator.hpp"
#include "EcuabFee_PartitionHandler.hpp"

#ifndef ECUABFEE_LOCAL
#define ECUABFEE_LOCAL static
#endif

#if(ECUABFEE_FLS_POLLING_MODE == STD_OFF)
#define ECUABFEE_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

ECUABFEE_LOCAL VAR(MemIf_JobResultType, ECUABFEE_APPL_DATA) EcuabFee_Fls_GlobalJobResult;

#define ECUABFEE_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"
#endif

#define ECUABFEE_START_SEC_CODE
#include "MemMap.hpp"

ECUABFEE_LOCAL FUNC(MemIf_JobResultType, ECUABFEE_PRIVATE_CODE) EcuabFee_Fls_GetJobResultFromFlash(void);

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_Fls_SetResultPending(void);

#define ECUABFEE_STOP_SEC_CODE
#include "MemMap.hpp"

#define ECUABFEE_START_SEC_CODE
#include "MemMap.hpp"

ECUABFEE_LOCAL FUNC(MemIf_JobResultType, ECUABFEE_PRIVATE_CODE) EcuabFee_Fls_GetJobResultFromFlash(void){
  return EcuabFee_Ph_GetFlsApiPtr()->GetJobResult();
}

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_Fls_SetResultPending(void){
#if(ECUABFEE_FLS_POLLING_MODE == STD_OFF)
  EcuabFee_Fls_GlobalJobResult = MEMIF_JOB_PENDING;
#endif
}

#define ECUABFEE_STOP_SEC_CODE
#include "MemMap.hpp"

#define ECUABFEE_START_SEC_CODE
#include "MemMap.hpp"

FUNC (Std_ReturnType, ECUABFEE_PUBLIC_CODE) EcuabFee_Fls_Read(EcuabFee_AddressType Address
   ,   EcuabFee_DataPtr BufferPtr, uint16 Length){
  EcuabFee_Fls_SetResultPending();
  return EcuabFee_Ph_GetFlsApiPtr()->Read(Address, BufferPtr, Length);
}

FUNC (Std_ReturnType, ECUABFEE_PUBLIC_CODE) EcuabFee_Fls_Write(EcuabFee_AddressType Address
   ,   EcuabFee_ConstDataPtr BufferPtr, uint16 Length){
  EcuabFee_Fls_SetResultPending();
  return EcuabFee_Ph_GetFlsApiPtr()->Write(Address, BufferPtr, Length);
}

FUNC (Std_ReturnType, ECUABFEE_PUBLIC_CODE) EcuabFee_Fls_Compare(EcuabFee_AddressType Address
   ,   EcuabFee_ConstDataPtr BufferPtr, uint16 Length){
  EcuabFee_Fls_SetResultPending();
  return EcuabFee_Ph_GetFlsApiPtr()->Compare(Address, BufferPtr, Length);
}

FUNC (Std_ReturnType, ECUABFEE_PUBLIC_CODE) EcuabFee_Fls_Erase(EcuabFee_AddressType Address, uint16 Length){
  EcuabFee_Fls_SetResultPending();
  return EcuabFee_Ph_GetFlsApiPtr()->Erase(Address, Length);
}

FUNC (Std_ReturnType, ECUABFEE_PUBLIC_CODE) EcuabFee_Fls_BlankCheck(EcuabFee_AddressType Address, uint16 Length){
  EcuabFee_Fls_SetResultPending();
  return EcuabFee_Ph_GetFlsApiPtr()->BlankCheck(Address, Length);
}

#if(ECUABFEE_FLS_POLLING_MODE == STD_OFF)

FUNC (void, ECUABFEE_PUBLIC_CODE) EcuabFee_Fls_SetCurrentFlsJobResult(void){
  EcuabFee_Fls_GlobalJobResult = EcuabFee_Fls_GetJobResultFromFlash();
}
#endif

FUNC (MemIf_JobResultType, ECUABFEE_PUBLIC_CODE) EcuabFee_Fls_GetJobResult(void){
#if(ECUABFEE_FLS_POLLING_MODE == STD_ON)

  return EcuabFee_Fls_GetJobResultFromFlash();

#else

  return EcuabFee_Fls_GlobalJobResult;

#endif
}

FUNC (MemIf_StatusType, ECUABFEE_PUBLIC_CODE) EcuabFee_Fls_GetStatus(void){
  return EcuabFee_Ph_GetFlsApiPtr()->GetStatus();
}

#define ECUABFEE_STOP_SEC_CODE
#include "MemMap.hpp"

