#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define TPM_TAG_RQU_COMMAND 193
#define TPM_TAG_RQU_AUTH1_COMMAND 194
#define TPM_ORD_SHA1Start 160
#define TPM_ORD_SHA1Complete 162
#define TPM_ORD_GetRandom 70

/*********************first run "modprobe tpmd_dev""tpmd -f -d" and if there is a tcsd ,you cannot run it **********/

int main(int argc, char **argv)
{
    unsigned int i,j,fd;
    int res,ret;
    unsigned char buf[256];
    
    int buf_size = sizeof(buf);
    unsigned char random_cmd[] = {0, TPM_TAG_RQU_COMMAND,
                 0, 0, 0, 14,
                 0, 0, 0, TPM_ORD_GetRandom,
                0, 0, 0, 8};//70 means TPM_ORD_GetRandom

    unsigned char tpm_sha1start[]={0,TPM_TAG_RQU_COMMAND,
                0,0,0,10,
                0,0,0,TPM_ORD_SHA1Start};
        
    unsigned char tpm_sha1complete[]={0,TPM_TAG_RQU_COMMAND,
                0,0,0,78,0,0,0,TPM_ORD_SHA1Complete,
                0,0,0,64,
                1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,
                           33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64};
    

    fd = open("/dev/tpm0",O_RDWR);

    if(fd < 0){
        printf("Error: Open() failed: (%04x)\n ", fd);
        return -1;
    }    

    printf("sizeof(random_cmd): %d\n", sizeof(random_cmd));
    printf("data in random_cmd: ");
       for(i = 0; i < sizeof(random_cmd); i++)
                printf("%02x", random_cmd[i]);
       printf("\n");

    res = write(fd, random_cmd, sizeof(random_cmd));
    printf("res:%d\n sizeof_random_cmd:%d", res, sizeof(random_cmd));    

    if(res != sizeof(random_cmd)){
        printf("Error: write random command failed: (%04x)\n ", res);
        close(fd);
        return -1;
    }

    buf_size = 256;
       ret = read(fd, &buf, buf_size);

        printf("ret of read random tpm0: %d\n",ret);
        printf("read tpm0 random data: ");
        for(i = 0; i < ret; i++){
                printf("%02x ",buf[i] );
        }
        printf("\n");
    

    
    buf_size = 256;//buf_size > 10

    printf("sizeof(tpm_sha1start): %d\n", sizeof(tpm_sha1start));
    printf("data in tpm_sha1start: ");
       for(i = 0; i < sizeof(tpm_sha1start); i++)
                printf("%02x", tpm_sha1start[i]);
       printf("\n");
    res =write(fd, tpm_sha1start, sizeof(tpm_sha1start));
    
    if(res != sizeof(tpm_sha1start)){
        printf("Error: write tpm_sha1start failed: (%04x)\n ", res);
        close(fd);
        return -1;
    }
    
    buf_size = 256;
       ret = read(fd, &buf, buf_size);

        printf("ret of read tpm0 after tpm_sha1start : %d\n",ret);
        printf("read tpm0 tpm_sha1start data: ");
        for(i = 0; i < ret; i++){
                printf("%02x ",buf[i] );
        }
        printf("\n");


    buf_size = 256;//buf_size > 10

    printf("sizeof(tpm_sha1complete): %d\n", sizeof(tpm_sha1complete));
    printf("data in tpm_sha1complete: ");
       for(i = 0; i < sizeof(tpm_sha1complete); i++)
                printf("%02x", tpm_sha1complete[i]);
       printf("\n");


    res =write(fd, tpm_sha1complete, sizeof(tpm_sha1complete));
    
    if(res != sizeof(tpm_sha1complete)){
        printf("Error: write tpm_sha1complete failed: (%04x)\n ", res);
        close(fd);
        return -1;
    }
    
    buf_size = 256;
       ret = read(fd, &buf, buf_size);

        printf("ret of read tpm0 after tpm_sha1complete : %d\n",ret);
        printf("read tpm0 data after tpm_sha1complete : ");
        for(i = 0; i < ret; i++){
                printf("%02x ",buf[i] );
        }
        printf("\n");    
    
    close(fd);
    

    return 0;
}
