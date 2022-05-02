#pragma once
/******************************************************************************/
/* File   : Fee_core.hpp                                                      */
/* Author : NAGARAJA HM (c) since 1982. All rights reserved.                  */
/******************************************************************************/

/******************************************************************************/
/* #INCLUDES                                                                  */
/******************************************************************************/
#include "CompilerCfg_Fee.hpp"

/******************************************************************************/
/* #DEFINES                                                                   */
/******************************************************************************/
#define FEE_CORE_FUNCTIONALITIES                                               \
              FUNC(void, FEE_CODE) SetMode                (void);              \
              FUNC(void, FEE_CODE) Read                   (void);              \
              FUNC(void, FEE_CODE) Write                  (void);              \
              FUNC(void, FEE_CODE) Cancel                 (void);              \
              FUNC(void, FEE_CODE) GetStatus              (void);              \
              FUNC(void, FEE_CODE) GetJobResult           (void);              \
              FUNC(void, FEE_CODE) InvalidateBlock        (void);              \
              FUNC(void, FEE_CODE) EraseImmediateBlock    (void);              \
              FUNC(void, FEE_CODE) CbJobEndNotification   (void);              \
              FUNC(void, FEE_CODE) CbJobErrorNotification (void);              \

#define FEE_CORE_FUNCTIONALITIES_VIRTUAL                                       \
      virtual FUNC(void, FEE_CODE) SetMode                (void) = 0;          \
      virtual FUNC(void, FEE_CODE) Read                   (void) = 0;          \
      virtual FUNC(void, FEE_CODE) Write                  (void) = 0;          \
      virtual FUNC(void, FEE_CODE) Cancel                 (void) = 0;          \
      virtual FUNC(void, FEE_CODE) GetStatus              (void) = 0;          \
      virtual FUNC(void, FEE_CODE) GetJobResult           (void) = 0;          \
      virtual FUNC(void, FEE_CODE) InvalidateBlock        (void) = 0;          \
      virtual FUNC(void, FEE_CODE) EraseImmediateBlock    (void) = 0;          \
      virtual FUNC(void, FEE_CODE) CbJobEndNotification   (void) = 0;          \
      virtual FUNC(void, FEE_CODE) CbJobErrorNotification (void) = 0;          \

/******************************************************************************/
/* MACROS                                                                     */
/******************************************************************************/

/******************************************************************************/
/* TYPEDEFS                                                                   */
/******************************************************************************/
class class_Fee_Functionality{
   public:
      FEE_CORE_FUNCTIONALITIES_VIRTUAL
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

