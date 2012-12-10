#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<fcntl.h>

#define  TPMMAX 4096

unsigned int TPM_Transmit(unsigned char *blob){
	int tpmfp;
	int len;
	unsigned int size;
	
	tpmfp = open("/dev/tpm", O_RDWR);
	size = (unsigned int *)&blob[2];
	len = write(tpmfp, blob, size);
	len = read(tpmfp, blob, TPMMAX);
	close(tpmfp);
	return ((unsigned int *)&blob[6]);
}

unsigned int TPM_Reset(){
	unsigned char blob[TPMMAX]={0,193,0,0,0,10,0,0,0,90};
	return TPM_Transmit(blob);
}

unsigned int TPM_GetCapability_Version(){
	unsigned char blob[TPMMAX] = {0, 193, 0, 0, 0, 18, 0, 0, 0, 101, 0, 0, 0, 6, 0, 0, 0, 0};
	return (TPM_Transmit(blob));
}
void test(){
	int tpmfp;

	
   if((tpmfp = open("/dev/tpm0", O_RDWR)) < 0){
	printf("tpmfp:%d\n", tpmfp);
	printf("Unable to open /dev/tpm0\n");
	return;
   }

   close(tpmfp);
}

void test_tpm_reset(){
	int ret;
	if((ret = TPM_Reset())){
		exit(-2);
	}
}

/*void test_tpm_pcrread(){
	int ret;
	if((ret = TPM_PcrRead(0L, pcrvalue))){
		printf("TPM Pcr read failed, error %s\n", TPM_GetErrMsg(ret));
		exit(-3);
	}
}
*/

int main(){
	//test();
	//test_tpm_reset();
	printf("\nTPM version is ========\n%u\n", TPM_GetCapability_Version());
	return 0;
}
