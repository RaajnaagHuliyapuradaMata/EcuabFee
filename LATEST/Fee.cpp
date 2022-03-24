/******************************************************************************/
/* File   : Fee.cpp                                                           */
/* Author : NAGARAJA HM (c) since 1982. All rights reserved.                  */
/******************************************************************************/

/******************************************************************************/
/* #INCLUDES                                                                  */
/******************************************************************************/
#include "module.h"
#include "CfgFee.h"
#include "infFee_EcuM.h"
#include "infFee_Dcm.h"
#include "infFee_SchM.h"

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
      FUNC(void, FEE_CODE) InitFunction   (void);
      FUNC(void, FEE_CODE) DeInitFunction (void);
      FUNC(void, FEE_CODE) GetVersionInfo (void);
      FUNC(void, FEE_CODE) MainFunction   (void);

   private:
      CONST(Std_TypeVersionInfo, FEE_CONST) VersionInfo = {
            0x0000
         ,  0xFFFF
         ,  0x01
         ,  '0'
         ,  '1'
         ,  '0'
      };
};

/******************************************************************************/
/* CONSTS                                                                     */
/******************************************************************************/

/******************************************************************************/
/* PARAMS                                                                     */
/******************************************************************************/

/******************************************************************************/
/* OBJECTS                                                                    */
/******************************************************************************/
VAR(module_Fee, FEE_VAR) Fee;
CONSTP2VAR(infEcuMClient, FEE_VAR, FEE_CONST) gptrinfEcuMClient_Fee = &Fee;
CONSTP2VAR(infDcmClient,  FEE_VAR, FEE_CONST) gptrinfDcmClient_Fee  = &Fee;
CONSTP2VAR(infSchMClient, FEE_VAR, FEE_CONST) gptrinfSchMClient_Fee = &Fee;

/******************************************************************************/
/* FUNCTIONS                                                                  */
/******************************************************************************/
FUNC(void, FEE_CODE) module_Fee::InitFunction(void){
   Fee.IsInitDone = E_OK;
}

FUNC(void, FEE_CODE) module_Fee::DeInitFunction(void){
   Fee.IsInitDone = E_NOT_OK;
}

FUNC(void, FEE_CODE) module_Fee::GetVersionInfo(void){
#if(STD_ON == Fee_DevErrorDetect)
//TBD: API parameter check
   Det_ReportError(
   );
#endif
}

FUNC(void, FEE_CODE) module_Fee::MainFunction(void){
}

#include "Fee_Unused.h"

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

