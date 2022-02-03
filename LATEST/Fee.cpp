/*****************************************************/
/* File   : Fee.cpp                                  */
/* Author : Naagraaj HM                              */
/*****************************************************/

/*****************************************************/
/* #INCLUDES                                         */
/*****************************************************/
#include "Fee.h"

#include "Fee_EcuM.h"

/*****************************************************/
/* #DEFINES                                          */
/*****************************************************/

/*****************************************************/
/* MACROS                                            */
/*****************************************************/

/*****************************************************/
/* TYPEDEFS                                          */
/*****************************************************/

/*****************************************************/
/* CONSTS                                            */
/*****************************************************/

/*****************************************************/
/* PARAMS                                            */
/*****************************************************/

/*****************************************************/
/* OBJECTS                                           */
/*****************************************************/
class_Fee Fee;
class_Fee_EcuM Fee_EcuM;
class_EcuM_Client *EcuM_Client_ptr_Fee = &Fee_EcuM;

/*****************************************************/
/* FUNCTIONS                                         */
/*****************************************************/
FUNC(void, FEE_CODE) class_Fee_EcuM::InitFunction(void){
}

FUNC(void, FEE_CODE) class_Fee::SetMode(void){
}

FUNC(void, FEE_CODE) class_Fee::Read(void){
}

FUNC(void, FEE_CODE) class_Fee::Write(void){
}

FUNC(void, FEE_CODE) class_Fee::Cancel(void){
}

FUNC(void, FEE_CODE) class_Fee::GetStatus(void){
}

FUNC(void, FEE_CODE) class_Fee::GetJobResult(void){
}

FUNC(void, FEE_CODE) class_Fee::InvalidateBlock(void){
}

FUNC(void, FEE_CODE) class_Fee::GetVersionInfo(void){
}

FUNC(void, FEE_CODE) class_Fee::EraseImmediateBlock(void){
}

FUNC(void, FEE_CODE) class_Fee::CbJobEndNotification(void){
}

FUNC(void, FEE_CODE) class_Fee::CbJobErrorNotification(void){
}

FUNC(void, FEE_CODE) class_Fee::MainFunction(void){
}

/*****************************************************/
/* EOF                                               */
/*****************************************************/

