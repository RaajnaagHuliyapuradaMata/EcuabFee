#include "EcuabFee_FblBlockInterface.hpp"
#include "fbl_hw.hpp"

static const uint8 aucSymmetricKey[32] = { 0xf6, 0x81, 0x0c, 0xe9, 0x9f, 0xc6, 0x7d, 0x89,
										   0x0d, 0x97, 0xad, 0xcb, 0x8b, 0x18, 0x7e, 0x81,
										   0xdf, 0x59, 0x06, 0x0d, 0xf4, 0xce, 0x4b, 0xd2,
										   0xf5, 0x61, 0x95, 0x58, 0x7d, 0x49, 0x87, 0x75 };

static const uint8 aucInitialVector[16] = { 0x48, 0x75, 0x33, 0x57, 0x86, 0x22, 0x47, 0xb1,
										   0x20, 0xed, 0xe3, 0x7e, 0x0e, 0x74, 0x72, 0x6a };

uint8 FEEFBL_GetProgAttemptsCounter(uint8* ucData){
  uint8 U8_Counter;

  for (U8_Counter=0; U8_Counter<(kEepSizeProgAttemptsCounter); U8_Counter++){
    ucData[U8_Counter] = 0x00;
  }

  ApplFblNvReadProgAttemptsCounter(&ucData[0]);

  return (kEepSizeProgAttemptsCounter);
}

uint8 FEEFBL_GetMaxProgAttemptsCounter(uint8* ucData){
  uint8 U8_Counter;

  for (U8_Counter=0; U8_Counter<(kEepSizeMaxProgAttemptsCounter); U8_Counter++){
    ucData[U8_Counter] = 0x00;
  }

  ApplFblNvReadMaxProgAttemptsCounter(&ucData[0]);

  return (kEepSizeMaxProgAttemptsCounter);
}

uint8 FEEFBL_GetApplicationSignature(uint8* ucData){
  uint8 U8_Counter;

  for (U8_Counter=0; U8_Counter<(kEepSizeApplicationSignature); U8_Counter++){
    ucData[U8_Counter] = 0x00;
  }

  ApplFblNvReadApplicationSignature(0, &ucData[0]);

  return (kEepSizeApplicationSignature);
}

uint8 FEEFBL_GetProgrammingDate(uint8* ucData, uint8 ucAppFbl){
  uint8 U8_Counter;

  for (U8_Counter=0; U8_Counter<(kEepSizeProgrammingDate); U8_Counter++){
    ucData[U8_Counter] = 0x00;
  }

  ApplFblNvReadProgrammingDate(&ucData[0]);

  return (kEepSizeProgrammingDate);
}

uint8 FEEFBL_GetTesterSerialNumber(uint8* ucData, uint8 ucAppFbl){
  uint8 U8_Counter;

  for (U8_Counter=0; U8_Counter<(kEepSizeTesterSerialNumber); U8_Counter++){
    ucData[U8_Counter] = 0x00;
  }

  ApplFblNvReadTesterSerialNumber(&ucData[0]);

  return (kEepSizeTesterSerialNumber);
}

#ifdef DEBUG_IGNORE_MESSAGE_AUTHENTICATION
uint8 FEEFBL_GetUdsBootKey(uint8* ucData){
	uint8 ucCounter;
	boolean bKeyInitialized = FALSE;

	
	ApplFblNvReadSecKey_UDSBOOTKEY(&ucData[0]);

	
	for (ucCounter = 0; ucCounter < kEepSizeSecKey_UDSBOOTKEY; ucCounter++)
	{
		if(ucData[ucCounter] != 0xff)
		{
			bKeyInitialized = TRUE;
		}
	}

	
	if(bKeyInitialized == FALSE)
	{
		MEMCPY(ucData, aucDefaultKey, kEepSizeSecKey_UDSBOOTKEY);
	}

	return (kEepSizeSecKey_UDSBOOTKEY);
}
#endif

#if(SEED_KEY_ALGORITHM==SEED_KEY_USE_HMAC_SHA256)

 uint8 FEEFBL_GetUdsAppKey(uint8* ucData)
 {
  uint8 ucCounter;
  boolean bKeyInitialized = FALSE;

  ApplFblNvReadSecKey_UDSAPPKEY(&ucData[0]);

  for (ucCounter = 0; ucCounter < kEepSizeSecKey_UDSAPPKEY; ucCounter++){
    if(ucData[ucCounter] != 0xff){
      bKeyInitialized = TRUE;
    }
  }

  if(bKeyInitialized == FALSE){
    MEMCPY(ucData, aucDefaultKey, kEepSizeSecKey_UDSAPPKEY);
  }

  return (kEepSizeSecKey_UDSAPPKEY);
 }

 uint8 FEEFBL_GetUdsMsgKey(uint8* ucData)
 {
  uint8 ucCounter;
  boolean bKeyInitialized = FALSE;

  ApplFblNvReadSecKey_UDSMSGKEY(&ucData[0]);

  for (ucCounter = 0; ucCounter < kEepSizeSecKey_UDSMSGKEY; ucCounter++){
    if(ucData[ucCounter] != 0xff){
      bKeyInitialized = TRUE;
    }
  }

  if(bKeyInitialized == FALSE){
    MEMCPY(ucData, aucDefaultKey, kEepSizeSecKey_UDSMSGKEY);
  }

  return (kEepSizeSecKey_UDSMSGKEY);
 }

uint8 FEEFBL_GetFaultFlag(uint8* ucData){
  uint8 U8_Counter;

  for (U8_Counter=0; U8_Counter<(kEepSizeFaultFlag); U8_Counter++){
    ucData[U8_Counter] = 0x00;
  }

  ApplFblNvReadFaultFlag(&ucData[0]);

  return (kEepSizeFaultFlag);
}

#elif(SEED_KEY_ALGORITHM==SEED_KEY_USE_AES256_CBC)

uint8 FEEFBL_GetSymmetricKey(uint8* ucData){
	uint8 ucCounter;
	boolean bKeyInitialized = FALSE;

	
	ApplFblNvReadSecKey_AES_SYMMETRIC_KEY(&ucData[0]);

	
	for (ucCounter = 0; ucCounter < kEepSizeSecKey_AES_SYMMETRIC_KEY; ucCounter++)
	{
		if(ucData[ucCounter] != 0xff)
		{
			bKeyInitialized = TRUE;
		}
	}

	
	if(bKeyInitialized == FALSE)
	{
		MEMCPY(ucData, aucSymmetricKey, sizeof(aucSymmetricKey));
	}

	return (kEepSizeSecKey_AES_SYMMETRIC_KEY);
}

uint8 FEEFBL_GetInitialVector(uint8* ucData){
	uint8 ucCounter;
	boolean bKeyInitialized = FALSE;

	
	ApplFblNvReadSecKey_AES_INITIAL_VECTOR(&ucData[0]);

	
	for (ucCounter = 0; ucCounter < kEepSizeSecKey_AES_INITIAL_VECTOR; ucCounter++)
	{
		if(ucData[ucCounter] != 0xff)
		{
			bKeyInitialized = TRUE;
		}
	}

	
	if(bKeyInitialized == FALSE)
	{
		MEMCPY(ucData, aucInitialVector, sizeof(aucInitialVector));
	}

	return (kEepSizeSecKey_AES_INITIAL_VECTOR);
}
#endif

#if(SEED_KEY_ALGORITHM==SEED_KEY_USE_HMAC_SHA256)

 uint8 FEEFBL_PutUdsAppKey(const uint8* ucData)
 {
  uint8 ucRetVal;
  ucRetVal = ApplFblNvWriteSecKey_UDSAPPKEY(&ucData[0]);
  return ucRetVal;
 }

 uint8 FEEFBL_PutUdsBootKey(const uint8* ucData)
 {
  uint8 ucRetVal;
  ucRetVal = ApplFblNvWriteSecKey_UDSBOOTKEY(&ucData[0]);
  return ucRetVal;
 }

 uint8 FEEFBL_PutUdsMsgKey(const uint8* ucData)
 {
  uint8 ucRetVal;
  ucRetVal = ApplFblNvWriteSecKey_UDSMSGKEY(&ucData[0]);
  return ucRetVal;
 }
#endif

 uint8 FEEFBL_PutAesSymmetricKey(const uint8* ucData)
 {
	 uint8 ucRetVal;
	 ucRetVal = ApplFblNvWriteSecKey_AES_SYMMETRIC_KEY(&ucData[0]);
	 return ucRetVal;
 }

 uint8 FEEFBL_PutAesInitialVector(const uint8* ucData)
 {
	 uint8 ucRetVal;
	 ucRetVal = ApplFblNvWriteSecKey_AES_INITIAL_VECTOR(&ucData[0]);
	 return ucRetVal;
 }

 uint8 FEEFBL_PutApplicationSignature(uint8* ucData)
 {
  uint8 ucRetVal;
  ucRetVal = ApplFblNvWriteApplicationSignature(0, &ucData[0]);
  return ucRetVal;
 }

 uint8 FEEFBL_PutFaultFlag(uint8* ucData)
 {
  uint8 ucRetVal;
  ucRetVal = ApplFblNvWriteFaultFlag(&ucData[0]);
  return ucRetVal;
 }
