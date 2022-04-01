/******************************************************************************/
/* File   : Fee.cpp                                                           */
/* Author : NAGARAJA HM (c) since 1982. All rights reserved.                  */
/******************************************************************************/

/******************************************************************************/
/* #INCLUDES                                                                  */
/******************************************************************************/
#include "Module.hpp"
#include "infFee_EcuM.hpp"
#include "infFee_Dcm.hpp"
#include "infFee_SchM.hpp"

/******************************************************************************/
/* #DEFINES                                                                   */
/******************************************************************************/
#define FEE_AR_RELEASE_MAJOR_VERSION                                           4
#define FEE_AR_RELEASE_MINOR_VERSION                                           3

/******************************************************************************/
/* MACROS                                                                     */
/******************************************************************************/
#if(FEE_AR_RELEASE_MAJOR_VERSION != STD_AR_RELEASE_MAJOR_VERSION)
   #error "Incompatible FEE_AR_RELEASE_MAJOR_VERSION!"
#endif

#if(FEE_AR_RELEASE_MINOR_VERSION != STD_AR_RELEASE_MINOR_VERSION)
   #error "Incompatible FEE_AR_RELEASE_MINOR_VERSION!"
#endif

/******************************************************************************/
/* TYPEDEFS                                                                   */
/******************************************************************************/
class module_Fee:
      public abstract_module
{
   public:
      module_Fee(Std_TypeVersionInfo lVersionInfo) : abstract_module(lVersionInfo){
      }
      FUNC(void, FEE_CODE) InitFunction(
         CONSTP2CONST(CfgModule_TypeAbstract, FEE_CONFIG_DATA, FEE_APPL_CONST) lptrCfgModule
      );
      FUNC(void, FEE_CODE) DeInitFunction (void);
      FUNC(void, FEE_CODE) MainFunction   (void);
};

extern VAR(module_Fee, FEE_VAR) Fee;

/******************************************************************************/
/* CONSTS                                                                     */
/******************************************************************************/
CONSTP2VAR(infEcuMClient, FEE_VAR, FEE_CONST) gptrinfEcuMClient_Fee = &Fee;
CONSTP2VAR(infDcmClient,  FEE_VAR, FEE_CONST) gptrinfDcmClient_Fee  = &Fee;
CONSTP2VAR(infSchMClient, FEE_VAR, FEE_CONST) gptrinfSchMClient_Fee = &Fee;

/******************************************************************************/
/* PARAMS                                                                     */
/******************************************************************************/
#include "CfgFee.hpp"

/******************************************************************************/
/* OBJECTS                                                                    */
/******************************************************************************/
VAR(module_Fee, FEE_VAR) Fee(
   {
         0x0000
      ,  0xFFFF
      ,  0x01
      ,  '0'
      ,  '1'
      ,  '0'
   }
);

/******************************************************************************/
/* FUNCTIONS                                                                  */
/******************************************************************************/
FUNC(void, FEE_CODE) module_Fee::InitFunction(
   CONSTP2CONST(CfgModule_TypeAbstract, FEE_CONFIG_DATA, FEE_APPL_CONST) lptrCfgModule
){
#if(STD_ON == Fee_InitCheck)
   if(E_OK == IsInitDone){
#if(STD_ON == Fee_DevErrorDetect)
      Det_ReportError(
      );
#endif
   }
   else{
#endif
      if(NULL_PTR == lptrCfgModule){
#if(STD_ON == Fee_DevErrorDetect)
         Det_ReportError(
         );
#endif
      }
      else{
// check lptrCfgModule for memory faults
// use PBcfg_Fee as back-up configuration
      }
      IsInitDone = E_OK;
#if(STD_ON == Fee_InitCheck)
   }
#endif
}

FUNC(void, FEE_CODE) module_Fee::DeInitFunction(void){
#if(STD_ON == Fee_InitCheck)
   if(E_OK != IsInitDone){
#if(STD_ON == Fee_DevErrorDetect)
      Det_ReportError(
      );
#endif
   }
   else{
#endif
      IsInitDone = E_NOT_OK;
#if(STD_ON == Fee_InitCheck)
   }
#endif
}

FUNC(void, FEE_CODE) module_Fee::MainFunction(void){
#if(STD_ON == Fee_InitCheck)
   if(E_OK != IsInitDone){
#if(STD_ON == Fee_DevErrorDetect)
      Det_ReportError(
      );
#endif
   }
   else{
#endif
#if(STD_ON == Fee_InitCheck)
   }
#endif
}

class class_Fee_Unused{
   public:
      FUNC(void, FEE_CODE) SetMode                (void);
      FUNC(void, FEE_CODE) Read                   (void);
      FUNC(void, FEE_CODE) Write                  (void);
      FUNC(void, FEE_CODE) Cancel                 (void);
      FUNC(void, FEE_CODE) GetStatus              (void);
      FUNC(void, FEE_CODE) GetJobResult           (void);
      FUNC(void, FEE_CODE) InvalidateBlock        (void);
      FUNC(void, FEE_CODE) GetVersionInfo         (void);
      FUNC(void, FEE_CODE) EraseImmediateBlock    (void);
      FUNC(void, FEE_CODE) CbJobEndNotification   (void);
      FUNC(void, FEE_CODE) CbJobErrorNotification (void);
};

FUNC(void, FEE_CODE) class_Fee_Unused::SetMode(void){
}

FUNC(void, FEE_CODE) class_Fee_Unused::Read(void){
}

FUNC(void, FEE_CODE) class_Fee_Unused::Write(void){
}

FUNC(void, FEE_CODE) class_Fee_Unused::Cancel(void){
}

FUNC(void, FEE_CODE) class_Fee_Unused::GetStatus(void){
}

FUNC(void, FEE_CODE) class_Fee_Unused::GetJobResult(void){
}

FUNC(void, FEE_CODE) class_Fee_Unused::InvalidateBlock(void){
}

FUNC(void, FEE_CODE) class_Fee_Unused::EraseImmediateBlock(void){
}

FUNC(void, FEE_CODE) class_Fee_Unused::CbJobEndNotification(void){
}

FUNC(void, FEE_CODE) class_Fee_Unused::CbJobErrorNotification(void){
}

/******************************************************************************/
/* EOF                                                                        */
/******************************************************************************/

