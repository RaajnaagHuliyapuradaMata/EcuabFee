/******************************************************************************/
/* File   : EcuabFee.cpp                                                           */
/* Author : NAGARAJA HM (c) since 1982. All rights reserved.                  */
/******************************************************************************/

/******************************************************************************/
/* #INCLUDES                                                                  */
/******************************************************************************/
#include "Module.hpp"
#include "EcuabFee.hpp"
#include "infEcuabFee_Imp.hpp"

/******************************************************************************/
/* #DEFINES                                                                   */
/******************************************************************************/
#define FEE_AR_RELEASE_VERSION_MAJOR                                           4
#define FEE_AR_RELEASE_VERSION_MINOR                                           3

/******************************************************************************/
/* MACROS                                                                     */
/******************************************************************************/
#if(FEE_AR_RELEASE_VERSION_MAJOR != STD_AR_RELEASE_VERSION_MAJOR)
   #error "Incompatible FEE_AR_RELEASE_VERSION_MAJOR!"
#endif

#if(FEE_AR_RELEASE_VERSION_MINOR != STD_AR_RELEASE_VERSION_MINOR)
   #error "Incompatible FEE_AR_RELEASE_VERSION_MINOR!"
#endif

/******************************************************************************/
/* TYPEDEFS                                                                   */
/******************************************************************************/

/******************************************************************************/
/* CONSTS                                                                     */
/******************************************************************************/

/******************************************************************************/
/* PARAMS                                                                     */
/******************************************************************************/

/******************************************************************************/
/* OBJECTS                                                                    */
/******************************************************************************/
VAR(module_EcuabFee, FEE_VAR) EcuabFee;

/******************************************************************************/
/* FUNCTIONS                                                                  */
/******************************************************************************/
FUNC(void, FEE_CODE) module_EcuabFee::InitFunction(
      CONSTP2CONST(ConstModule_TypeAbstract, FEE_CONST,       FEE_APPL_CONST) lptrConstModule
   ,  CONSTP2CONST(CfgModule_TypeAbstract,   FEE_CONFIG_DATA, FEE_APPL_CONST) lptrCfgModule
){
#if(STD_ON == EcuabFee_InitCheck)
   if(
         E_OK
      != IsInitDone
   ){
#endif
      if(
            (NULL_PTR != lptrConstModule)
         && (NULL_PTR != lptrCfgModule)
      ){
         lptrConst = (const ConstEcuabFee_Type*)lptrConstModule;
         lptrCfg   = lptrCfgModule;
      }
      else{
#if(STD_ON == EcuabFee_DevErrorDetect)
         ServiceDet_ReportError(
               0 //TBD: IdModule
            ,  0 //TBD: IdInstance
            ,  0 //TBD: IdApi
            ,  0 //TBD: IdError
         );
#endif
      }
#if(STD_ON == EcuabFee_InitCheck)
      IsInitDone = E_OK;
   }
   else{
#if(STD_ON == EcuabFee_DevErrorDetect)
      ServiceDet_ReportError(
            0 //TBD: IdModule
         ,  0 //TBD: IdInstance
         ,  0 //TBD: IdApi
         ,  FEE_E_UNINIT
      );
#endif
   }
#endif
}

FUNC(void, FEE_CODE) module_EcuabFee::DeInitFunction(
   void
){
#if(STD_ON == EcuabFee_InitCheck)
   if(
         E_OK
      == IsInitDone
   ){
#endif
#if(STD_ON == EcuabFee_InitCheck)
      IsInitDone = E_NOT_OK;
   }
   else{
#if(STD_ON == EcuabFee_DevErrorDetect)
      ServiceDet_ReportError(
            0 //TBD: IdModule
         ,  0 //TBD: IdInstance
         ,  0 //TBD: IdApi
         ,  FEE_E_UNINIT
      );
#endif
   }
#endif
}

FUNC(void, FEE_CODE) module_EcuabFee::MainFunction(
   void
){
#if(STD_ON == EcuabFee_InitCheck)
   if(
         E_OK
      == IsInitDone
   ){
#endif
#if(STD_ON == EcuabFee_InitCheck)
   }
   else{
#if(STD_ON == EcuabFee_DevErrorDetect)
      ServiceDet_ReportError(
            0 //TBD: IdModule
         ,  0 //TBD: IdInstance
         ,  0 //TBD: IdApi
         ,  FEE_E_UNINIT
      );
#endif
   }
#endif
}

FUNC(void, FEE_CODE) module_EcuabFee::SetMode(
   void
){
}

FUNC(void, FEE_CODE) module_EcuabFee::Read(
   void
){
}

FUNC(void, FEE_CODE) module_EcuabFee::Write(
   void
){
}

FUNC(void, FEE_CODE) module_EcuabFee::McalCancel(
   void
){
}

FUNC(void, FEE_CODE) module_EcuabFee::GetStatus(
   void
){
}

FUNC(void, FEE_CODE) module_EcuabFee::GetJobResult(
   void
){
}

FUNC(void, FEE_CODE) module_EcuabFee::InvalidateBlock(
   void
){
}

FUNC(void, FEE_CODE) module_EcuabFee::EraseImmediateBlock(
   void
){
}

FUNC(void, FEE_CODE) module_EcuabFee::CbJobEndNotification(
   void
){
}

FUNC(void, FEE_CODE) module_EcuabFee::CbJobErrorNotification(
   void
){
}

/******************************************************************************/
/* EOF                                                                        */
/******************************************************************************/

