/******************************************************************************/
/* File   : Fee.cpp                                                           */
/* Author : NAGARAJA HM (c) since 1982. All rights reserved.                  */
/******************************************************************************/

/******************************************************************************/
/* #INCLUDES                                                                  */
/******************************************************************************/
#include "Module.hpp"
#include "Fee.hpp"
#include "infFee_Imp.hpp"

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
CONSTP2VAR(infEcuMClient, FEE_VAR, FEE_CONST) gptrinfEcuMClient_Fee = &Fee;

/******************************************************************************/
/* PARAMS                                                                     */
/******************************************************************************/

/******************************************************************************/
/* OBJECTS                                                                    */
/******************************************************************************/
VAR(module_Fee, FEE_VAR) Fee;

/******************************************************************************/
/* FUNCTIONS                                                                  */
/******************************************************************************/
FUNC(void, FEE_CODE) module_Fee::InitFunction(
   CONSTP2CONST(CfgModule_TypeAbstract, FEE_CONFIG_DATA, FEE_APPL_CONST) lptrCfgModule
){
#if(STD_ON == Fee_InitCheck)
   if(
         E_OK
      != IsInitDone
   ){
#endif
      if(NULL_PTR != lptrCfgModule){
         lptrCfg = lptrCfgModule;
      }
      else{
#if(STD_ON == Fee_DevErrorDetect)
         Det_ReportError(
               0 //TBD: IdModule
            ,  0 //TBD: IdInstance
            ,  0 //TBD: IdApi
            ,  0 //TBD: IdError
         );
#endif
      }
#if(STD_ON == Fee_InitCheck)
      IsInitDone = E_OK;
   }
   else{
#if(STD_ON == Fee_DevErrorDetect)
      Det_ReportError(
            0 //TBD: IdModule
         ,  0 //TBD: IdInstance
         ,  0 //TBD: IdApi
         ,  FEE_E_UNINIT
      );
#endif
   }
#endif
}

FUNC(void, FEE_CODE) module_Fee::DeInitFunction(
   void
){
#if(STD_ON == Fee_InitCheck)
   if(
         E_OK
      == IsInitDone
   ){
#endif
#if(STD_ON == Fee_InitCheck)
      IsInitDone = E_NOT_OK;
   }
   else{
#if(STD_ON == Fee_DevErrorDetect)
      Det_ReportError(
            0 //TBD: IdModule
         ,  0 //TBD: IdInstance
         ,  0 //TBD: IdApi
         ,  FEE_E_UNINIT
      );
#endif
   }
#endif
}

FUNC(void, FEE_CODE) module_Fee::MainFunction(
   void
){
#if(STD_ON == Fee_InitCheck)
   if(
         E_OK
      == IsInitDone
   ){
#endif
#if(STD_ON == Fee_InitCheck)
   }
   else{
#if(STD_ON == Fee_DevErrorDetect)
      Det_ReportError(
            0 //TBD: IdModule
         ,  0 //TBD: IdInstance
         ,  0 //TBD: IdApi
         ,  FEE_E_UNINIT
      );
#endif
   }
#endif
}

FUNC(void, FEE_CODE) module_Fee::SetMode(
   void
){
}

FUNC(void, FEE_CODE) module_Fee::Read(
   void
){
}

FUNC(void, FEE_CODE) module_Fee::Write(
   void
){
}

FUNC(void, FEE_CODE) module_Fee::Cancel(
   void
){
}

FUNC(void, FEE_CODE) module_Fee::GetStatus(
   void
){
}

FUNC(void, FEE_CODE) module_Fee::GetJobResult(
   void
){
}

FUNC(void, FEE_CODE) module_Fee::InvalidateBlock(
   void
){
}

FUNC(void, FEE_CODE) module_Fee::EraseImmediateBlock(
   void
){
}

FUNC(void, FEE_CODE) module_Fee::CbJobEndNotification(
   void
){
}

FUNC(void, FEE_CODE) module_Fee::CbJobErrorNotification(
   void
){
}

/******************************************************************************/
/* EOF                                                                        */
/******************************************************************************/

