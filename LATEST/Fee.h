#pragma once

#include "Std_Types.h"
#include "Compiler_Cfg_Fee.h"

class class_Fee{
   public:
      FUNC(void, FEE_CODE) InitFunction           (void);
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
      FUNC(void, FEE_CODE) MainFunction           (void);
};

extern class_Fee Fee;

