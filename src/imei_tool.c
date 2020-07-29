/*
 * The International Mobile Equipment Identity (IMEI) is a number, 
 * usually unique, to identify 3GPP and iDEN mobile phones, as well as 
 * some satellite phones.
 *
 * Qualcomm stored IMEI info in NV 550 NV_UE_IMEI_I.
 *
 * It is a tool to learn Qualcomm IMEI encoding in NV XML file format.
 *
 *     imei_tool /d "8,58,135,66,24,6,0,84,120"
 *     imei_tool /e 378248160004587
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char IMEI[16] = {0};
    unsigned int imei[9] = {0};
    int count = 0;
    int index = 0;

    if (argc != 3) {
        printf("Invalid input.\n");
        printf("\n");
        printf("Below is the example:\n");
        printf("\timei_tool /d \"8,58,135,66,24,6,0,84,120\"\n");
        printf("\timei_tool /e 378248160004587\n");
        printf("\n");
        return 1;
    } else {
        char *imeistring = NULL;
        int length = 0;
        char *delim = ",";
        char *pch;
        unsigned int temp = 0;

        if (strcmp(argv[1], "/d")==0) {
            imeistring = (char *) malloc (sizeof(char) * (strlen(argv[2]) + 1));
            strcpy(imeistring, argv[2]);

            pch = strtok(imeistring, delim);
            while (pch != NULL && count < 9)
            {
                temp = (unsigned int) atoi(pch);            
                imei[count] = temp;
                count++;
                pch = strtok (NULL, delim);
            }

            if(imeistring) free(imeistring);

            /* The first byte of NV is the length of IMEI BCDs */
            if(imei[0] != 8) {
                printf("Invalid input.\n");
                return 1;
            }

            temp = imei[1];
            if ((temp & 0x0f) != 0xA) {
                // IMEI identifier is always 0xA
                printf("Invalid input.\n");
                return 1;
            } else {
                // For IMEI this field is coded using BCD coding.
                IMEI[0] = ((temp & 0xf0 ) >> 4) + '0';
            }

            for (count = 2; count < 9; count++) {
                temp = imei[count];
                
                IMEI[(count - 1) *  2] = ((temp & 0xf0 ) >> 4) + '0';
                IMEI[(count - 1) * 2 - 1] = (temp & 0x0f) + '0';
            }

            printf("Decoding NV XML item value to IMEI: \n");
            printf("%s\n", IMEI);
        } else if (strcmp(argv[1], "/e")==0) {
            int length = 0;

            snprintf(IMEI, 16, "%s", argv[2]);
            printf("Encoding IMEI %s to NV XML format: \n", IMEI);

            imei[0] = 0x8;    
            imei[1] = (((IMEI[0] - '0') & 0x0F) << 4) + 0x0A;

            length = strlen(IMEI);
            for (count = 1, index = 2; count < length; count = count + 2, index++) {
                imei[index] = (((IMEI[count + 1] - '0') & 0x0F) << 4) + 
                              ((IMEI[count] - '0') & 0x0F);
            }

            printf("Dec value: %d,%d,%d,%d,%d,%d,%d,%d,%d\n", 
                imei[0], imei[1], imei[2], imei[3], imei[4], 
                imei[5], imei[6], imei[7], imei[8]);

            printf("Hex value: 0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X\n", 
                imei[0], imei[1], imei[2], imei[3], imei[4], 
                imei[5], imei[6], imei[7], imei[8]);                
        }
    }

    return 0;
}
