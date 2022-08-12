#pragma once
/******************************************************************************/
/* File   : EcuabFee_core.hpp                                                      */
/* Author : NAGARAJA HM (c) since 1982. All rights reserved.                  */
/******************************************************************************/

/******************************************************************************/
/* #INCLUDES                                                                  */
/******************************************************************************/
#include "CompilerCfg_EcuabFee.hpp"

/******************************************************************************/
/* #DEFINES                                                                   */
/******************************************************************************/
#define ECUABFEE_CORE_FUNCTIONALITIES                                               \
              FUNC(void, ECUABFEE_CODE) SetMode                (void);              \
              FUNC(void, ECUABFEE_CODE) Read                   (void);              \
              FUNC(void, ECUABFEE_CODE) Write                  (void);              \
              FUNC(void, ECUABFEE_CODE) McalCancel                 (void);              \
              FUNC(void, ECUABFEE_CODE) GetStatus              (void);              \
              FUNC(void, ECUABFEE_CODE) GetJobResult           (void);              \
              FUNC(void, ECUABFEE_CODE) InvalidateBlock        (void);              \
              FUNC(void, ECUABFEE_CODE) EraseImmediateBlock    (void);              \
              FUNC(void, ECUABFEE_CODE) CbJobEndNotification   (void);              \
              FUNC(void, ECUABFEE_CODE) CbJobErrorNotification (void);              \

#define ECUABFEE_CORE_FUNCTIONALITIES_VIRTUAL                                       \
      virtual FUNC(void, ECUABFEE_CODE) SetMode                (void) = 0;          \
      virtual FUNC(void, ECUABFEE_CODE) Read                   (void) = 0;          \
      virtual FUNC(void, ECUABFEE_CODE) Write                  (void) = 0;          \
      virtual FUNC(void, ECUABFEE_CODE) McalCancel                 (void) = 0;          \
      virtual FUNC(void, ECUABFEE_CODE) GetStatus              (void) = 0;          \
      virtual FUNC(void, ECUABFEE_CODE) GetJobResult           (void) = 0;          \
      virtual FUNC(void, ECUABFEE_CODE) InvalidateBlock        (void) = 0;          \
      virtual FUNC(void, ECUABFEE_CODE) EraseImmediateBlock    (void) = 0;          \
      virtual FUNC(void, ECUABFEE_CODE) CbJobEndNotification   (void) = 0;          \
      virtual FUNC(void, ECUABFEE_CODE) CbJobErrorNotification (void) = 0;          \

/******************************************************************************/
/* MACROS                                                                     */
/******************************************************************************/

/******************************************************************************/
/* TYPEDEFS                                                                   */
/******************************************************************************/
class class_EcuabFee_Functionality{
   public:
      ECUABFEE_CORE_FUNCTIONALITIES_VIRTUAL
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

/******************************************************************************/
/* FUNCTIONS                                                                  */
/******************************************************************************/

/******************************************************************************/
/* EOF                                                                        */
/******************************************************************************/

