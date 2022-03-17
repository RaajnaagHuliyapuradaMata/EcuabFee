/******************************************************************************/
/* File   : Fee.cpp                                                           */
/* Author : NAGARAJA HM (c) since 1982. All rights reserved.                  */
/******************************************************************************/

/******************************************************************************/
/* #INCLUDES                                                                  */
/******************************************************************************/
#include "infFee_Version.h"

#include "module.h"
#include "infFee_EcuM.h"
#include "infFee_Dcm.h"
#include "infFee_SchM.h"

/******************************************************************************/
/* #DEFINES                                                                   */
/******************************************************************************/

/******************************************************************************/
/* MACROS                                                                     */
/******************************************************************************/

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
}

FUNC(void, FEE_CODE) module_Fee::DeInitFunction(void){
}

FUNC(void, FEE_CODE) module_Fee::GetVersionInfo(void){
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

