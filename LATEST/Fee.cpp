/*****************************************************/
/* File   : Fee.cpp                                  */
/* Author : Naagraaj HM                              */
/*****************************************************/

/*****************************************************/
/* #INCLUDES                                         */
/*****************************************************/
#include "module.h"
#include "infFee_EcuM.h"
#include "infFee_SchM.h"
#include "Fee_Unused.h"

/*****************************************************/
/* #DEFINES                                          */
/*****************************************************/

/*****************************************************/
/* MACROS                                            */
/*****************************************************/

/*****************************************************/
/* TYPEDEFS                                          */
/*****************************************************/
class module_Fee:
      public abstract_module
{
   public:
      FUNC(void, FEE_CODE) InitFunction   (void);
      FUNC(void, FEE_CODE) DeInitFunction (void);
      FUNC(void, FEE_CODE) GetVersionInfo (void);
      FUNC(void, FEE_CODE) MainFunction   (void);
};

/*****************************************************/
/* CONSTS                                            */
/*****************************************************/

/*****************************************************/
/* PARAMS                                            */
/*****************************************************/

/*****************************************************/
/* OBJECTS                                           */
/*****************************************************/
module_Fee     Fee;
infEcuMClient* gptrinfEcuMClient_Fee = &Fee;
infDcmClient*  gptrinfDcmClient_Fee  = &Fee;
infSchMClient* gptrinfSchMClient_Fee = &Fee;

/*****************************************************/
/* FUNCTIONS                                         */
/*****************************************************/
FUNC(void, FEE_CODE) module_Fee::InitFunction(void){
}

FUNC(void, FEE_CODE) module_Fee::DeInitFunction(void){
}

FUNC(void, FEE_CODE) module_Fee::GetVersionInfo(void){
}

FUNC(void, FEE_CODE) module_Fee::MainFunction(void){
}

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

FUNC(void, FEE_CODE) class_Fee_Unused::GetVersionInfo(void){
}

FUNC(void, FEE_CODE) class_Fee_Unused::EraseImmediateBlock(void){
}

FUNC(void, FEE_CODE) class_Fee_Unused::CbJobEndNotification(void){
}

FUNC(void, FEE_CODE) class_Fee_Unused::CbJobErrorNotification(void){
}

/*****************************************************/
/* EOF                                               */
/*****************************************************/

