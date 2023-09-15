#ifndef FEE_FBL_BLOCK_INTERFACE_X_H
#define FEE_FBL_BLOCK_INTERFACE_X_H

#include "CfgSwcServiceNvM.hpp"
#include "fbl_inc.hpp"

extern uint8 FEEFBL_GetProgAttemptsCounter(uint8* ucData);
extern uint8 FEEFBL_GetMaxProgAttemptsCounter(uint8* ucData);
extern uint8 FEEFBL_GetUdsBootKey(uint8* ucData);
#if(SEED_KEY_ALGORITHM==SEED_KEY_USE_HMAC_SHA256)
extern uint8 FEEFBL_GetUdsAppKey(uint8* ucData);
extern uint8 FEEFBL_GetUdsMsgKey(uint8* ucData);
#elif(SEED_KEY_ALGORITHM==SEED_KEY_USE_AES256_CBC)

extern uint8 FEEFBL_GetInitialVector(uint8* ucData);
extern uint8 FEEFBL_GetSymmetricKey(uint8* ucData);
#endif
extern uint8 FEEFBL_GetProgrammingDate(uint8* ucData, uint8 ucAppFbl);
extern uint8 FEEFBL_GetTesterSerialNumber(uint8* ucData, uint8 ucAppFbl);
extern uint8 FEEFBL_GetFaultFlag(uint8* ucData);
extern uint8 FEEFBL_PutUdsAppKey(const uint8* ucData);
extern uint8 FEEFBL_PutUdsBootKey(const uint8* ucData);
extern uint8 FEEFBL_PutUdsMsgKey(const uint8* ucData);
extern uint8 FEEFBL_PutFaultFlag(uint8* ucData);
extern uint8 FEEFBL_PutAesInitialVector(const uint8* ucData);
extern uint8 FEEFBL_PutAesSymmetricKey(const uint8* ucData);

#endif
