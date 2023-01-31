/******************************************************************************/
/* File   : EcuabFee.cpp                                                      */
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

/******************************************************************************/
/* MACROS                                                                     */
/******************************************************************************/

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
VAR(module_EcuabFee, ECUABFEE_VAR) EcuabFee;

/******************************************************************************/
/* FUNCTIONS                                                                  */
/******************************************************************************/
FUNC(void, ECUABFEE_CODE) module_EcuabFee::InitFunction(
      CONSTP2CONST(ConstModule_TypeAbstract, ECUABFEE_CONST,       ECUABFEE_APPL_CONST) lptrNvMBlocksRomModule
   ,  CONSTP2CONST(CfgModule_TypeAbstract,   ECUABFEE_CONFIG_DATA, ECUABFEE_APPL_CONST) lptrCfgModule
){
#if(STD_ON == EcuabFee_InitCheck)
   if(
         E_OK
      != IsInitDone
   ){
#endif
      if(
            (NULL_PTR != lptrNvMBlocksRomModule)
         && (NULL_PTR != lptrCfgModule)
      ){
         lptrNvMBlocksRom = lptrNvMBlocksRomModule;
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
         ,  ECUABFEE_E_UNINIT
      );
#endif
   }
#endif
}

FUNC(void, ECUABFEE_CODE) module_EcuabFee::DeInitFunction(
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
         ,  ECUABFEE_E_UNINIT
      );
#endif
   }
#endif
}

FUNC(void, ECUABFEE_CODE) module_EcuabFee::MainFunction(
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
         ,  ECUABFEE_E_UNINIT
      );
#endif
   }
#endif
}

FUNC(void, ECUABFEE_CODE) module_EcuabFee::SetMode(
   void
){
}

FUNC(void, ECUABFEE_CODE) module_EcuabFee::Read(
   void
){
}

FUNC(void, ECUABFEE_CODE) module_EcuabFee::Write(
   void
){
}

FUNC(void, ECUABFEE_CODE) module_EcuabFee::McalCancel(
   void
){
}

FUNC(void, ECUABFEE_CODE) module_EcuabFee::GetStatus(
   void
){
}

FUNC(void, ECUABFEE_CODE) module_EcuabFee::GetJobResult(
   void
){
}

FUNC(void, ECUABFEE_CODE) module_EcuabFee::InvalidateBlock(
   void
){
}

FUNC(void, ECUABFEE_CODE) module_EcuabFee::EraseImmediateBlock(
   void
){
}

FUNC(void, ECUABFEE_CODE) module_EcuabFee::CbJobEndNotification(
   void
){
}

FUNC(void, ECUABFEE_CODE) module_EcuabFee::CbJobErrorNotification(
   void
){
}

/******************************************************************************/
/* EOF                                                                        */
/******************************************************************************/

