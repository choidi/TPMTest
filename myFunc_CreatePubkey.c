/*
 * =====================================================================================
 *
 *       Filename:  myFunc_CreatePubkey.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年09月19日 23时09分46秒
 *       Revision:  none
 *       Compiler:  gcc
 *      
 *         Author:  choidi, choidichamy@gmail 
 *=====================================================================================
 */


#include <stdlib.h>
#include <stdio.h>
#include <openssl/rsa.h>
#include <openssl/err.h>

#include <tss/tss_error.h>
#include <tss/tss_defines.h>
#include <tss/tss_structs.h>
#include <tss/tss_typedef.h>
#include <tss/tspi.h>

TSS_RESULT MyFunc_CreatePubKey(RSA *rsa, int padding, TSS_HKEY *hKey){
  TSS_HCONTEXT hContext;
  TSS_RESULT result;
  UINT32 encScheme, sizeN, keySize;
  BYTE n[2048];

  switch(padding){
    case RSA_PKCS1_PADDING:
      encScheme = TSS_ES_RSAESPKCSV15;
      break;

    case RSA_PKCS1_OAEP_PADDING:
      encScheme = TSS_ES_RSAESOAEP_SHA1_MGF1;
      break;

    case RSA_NO_PADDING:
      encScheme = TSS_ES_NONE;
      break;

    default:
      return TSS_E_INTERNAL_ERROR;
      break;
  }

  if((keySize = get_tss_key_size(RSA_size(rsa) * 8 )) == 0){
    return TSS_E_BAD_PARAMETER;
  }

  result = Tspi_Context_CreateObject(hContext, TSS_OBJECT_TYPE_RSAKEY, TSS_KEY_TYPE_LEGACY | keySize, hKey);

  if(result != TSS_SUCCESS){
    LogError("Tspi_Context_CreateObject failed:%s", strresult(result));
    return result;
  }

  if((sizeN = BN_bn2bin(rsa->n, n)) <= 0){
    LogError("BN_bn2bin failed");
    ERR_print_errors_fp(stdout);
    Tspi_Context_CloseObject(hContext, *hKey);
    return TSS_E_FAIL;
  }

  result = Tspi_SetAttribData(*hKey, TSS_TSPATTRIB_KEY_BLOB, TSS_TSPATTRIB_KEYBLOB_PUBLIC_KEY, sizeN, n);

  if(result != TSS_SUCCESS){
    LogError("Tspi_SetAttribData failed:%s", strresult(result));
    Tspi_Context_CloseObject(hContext, *hKey);
    return result;
  }

  result = Tspi_SetAttribUnit32(*hKey, TSS_TSPATTRIB_KEY_INFO, TSS_TSPATTRIB_KEYINFO_ALGORITHM, TSS_ALG_RSA );

  if(result != TSS_SUCCESS){
    LogError("Tspi_SetAttribUnit32 failed:%s", strresult(result));
    Tspi_Context_CloseObject(hContext, *hKey);
    return result;
  }

  result = Tspi_SetAttribUnit32(*hKey, TSS_TSPATTRIB_RSAKEY_INFO, TSS_TSPATTRIB_KEYINFO_RSA_PRIMES, 2);
  if(result != TSS_SUCCESS){
    LogError("Tspi_SetAttribUnit32 failed:%s", strresult(result));
    Tspi_Context_CloseObject(hContext, *hKey);
    return result;
  }

  result = Tspi_SetAttribUnit32(*hKey, TSS_TSPATTRIB_KEY_INFO, TSS_TSPATTRIB_KEYINFO_ENCSCHEME, encScheme);

  if(result != TSS_SUCCESS){
    LogError("Tspi_Setattribunit32 failed:%s", strresult(result));
    Tspi_Context_CloseObject(hContext,*hKey);
    return result;
  }
  return TSS_SUCCESS;
}

int main(){

  RSA *rsa;
  int padding = 1;
  TSS_HKEY *hkey;

  MyFunc_CreatePubKey(rsa, padding, hkey);

  return 0;
}

