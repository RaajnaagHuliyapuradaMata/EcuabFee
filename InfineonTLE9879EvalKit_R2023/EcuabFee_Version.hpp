#pragma once
/******************************************************************************/
/* File   : EcuabFee_Version.hpp                                              */
/* Author : NAGARAJA HM (c) since 1982. All rights reserved.                  */
/******************************************************************************/

/******************************************************************************/
/* #DEFINES                                                                   */
/******************************************************************************/
#define ECUABFEE_AR_RELEASE_VERSION_MAJOR                                      4
#define ECUABFEE_AR_RELEASE_VERSION_MINOR                                      3

/******************************************************************************/
/* MACROS                                                                     */
/******************************************************************************/
#if(ECUABFEE_AR_RELEASE_VERSION_MAJOR != STD_AR_RELEASE_VERSION_MAJOR)
   #error "Incompatible ECUABFEE_AR_RELEASE_VERSION_MAJOR!"
#endif

#if(ECUABFEE_AR_RELEASE_VERSION_MINOR != STD_AR_RELEASE_VERSION_MINOR)
   #error "Incompatible ECUABFEE_AR_RELEASE_VERSION_MINOR!"
#endif

/******************************************************************************/
/* EOF                                                                        */
/******************************************************************************/

