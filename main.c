#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ti/getcsc.h>
#include <ti/screen.h>

#define INPUT_SIZE 17
#define RESP_SIZE 80
#define MAX_POWERS 5

/* Function prototypes */
void calculatePowers(int value, int base, char *powers);
int onesComplement(int value, int bitLength);
int twosComplement(int value, int bitLength);

/* Validation functions */
int validateBinary(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] != '0' && str[i] != '1')
            return 0;
    }
    return 1;
}

int validateHex(const char *str) {
    int start = 0;
    if (strncmp(str, "0x", 2) == 0) {
        start = 2;
    }
    for (int i = start; str[i] != '\0'; i++) {
        char c = toupper(str[i]);
        if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F')))
            return 0;
    }
    return 1;
}

int validateDecimal(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i]))
            return 0;
    }
    return 1;
}

/* Calculate powers used in representation */
void calculatePowers(int value, int base, char *powers) {
    int powerCount = 0;
    strcpy(powers, "Powers: ");

    if (value == 0) {
        strcat(powers, "0");
        return;
    }

    int tempValue = value;
    int position = 0;

    while (tempValue > 0 && powerCount < MAX_POWERS) {
        if (tempValue & 1) {
            char temp[20];
            sprintf(temp, "%d^%d + ", base, position);
            strcat(powers, temp);
            powerCount++;
        }
        tempValue >>= 1;
        position++;
    }

    // Remove trailing " + "
    size_t len = strlen(powers);
    if (len > 8) {
        powers[len - 3] = '\0';
    }
}

/* Conversion functions */
void decimalToBinary(int decimal, char *result) {
    char temp[17];
    int idx = 0;

    if (decimal == 0) {
        strcpy(result, "0");
        return;
    }

    while (decimal > 0 && idx < 16) {
        temp[idx++] = (decimal & 1) ? '1' : '0';
        decimal >>= 1;
    }

    int j = 0;
    for (int i = idx - 1; i >= 0; i--) {
        result[j++] = temp[i];
    }
    result[j] = '\0';
}

void decimalToHex(int decimal, char *result) {
    sprintf(result, "0x%X", decimal);
}

int binaryToDecimal(const char *binary) {
    int decimal = 0;
    for (int i = 0; binary[i] != '\0'; i++) {
        decimal = decimal * 2 + (binary[i] - '0');
    }
    return decimal;
}

int hexToDecimal(const char *hex) {
    int decimal = 0;
    int start = 0;
    if (strncmp(hex, "0x", 2) == 0) {
        start = 2;
    }
    for (int i = start; hex[i] != '\0'; i++) {
        char c = toupper(hex[i]);
        int value = (c >= 'A') ? (c - 'A' + 10) : (c - '0');
        decimal = decimal * 16 + value;
    }
    return decimal;
}

/* Complement functions */
int onesComplement(int value, int bitLength) {
    return (~value) & ((1 << bitLength) - 1);
}

int twosComplement(int value, int bitLength) {
    return (~value + 1) & ((1 << bitLength) - 1);
}

/* Display functions */
void displayMenu(void) {
    os_ClrHome();
    os_PutStrFull("Number Base Converter");
    os_NewLine();
    os_PutStrFull("1: Binary to Others");
    os_NewLine();
    os_PutStrFull("2: Hex to Others");
    os_NewLine();
    os_PutStrFull("3: Decimal to Others");
    os_NewLine();
    os_PutStrFull("4: Exit");
    os_NewLine();
    os_NewLine();
    os_PutStrFull("Choose (1-4)");
}

void displayResults(int decimal, int bitLength, int isSignedNegative) {
    char binary[17];
    char hex[17];
    char powers[RESP_SIZE];
    char response[RESP_SIZE];

    // Page 1: Basic conversions
    os_ClrHome();
    sprintf(response, "Unsigned: %d", decimal);
    os_PutStrFull(response);
    os_NewLine();

    decimalToBinary(decimal, binary);
    sprintf(response, "Binary: %s", binary);
    os_PutStrFull(response);
    os_NewLine();

    decimalToHex(decimal, hex);
    sprintf(response, "Hex: %s", hex);
    os_PutStrFull(response);
    os_NewLine();

    calculatePowers(decimal, 2, powers);
    os_PutStrFull(powers);
    os_NewLine();
    os_NewLine();
    os_PutStrFull("Next page: any key...");
    while (!os_GetCSC())
        ;

    os_ClrHome();
    int signedValue = isSignedNegative ? decimal - (1 << (bitLength - 1)) : decimal;
    sprintf(response, "Signed: %d", signedValue);
    os_PutStrFull(response);
    os_NewLine();

    int onesComp = onesComplement(decimal, bitLength);
    sprintf(response, "1's Comp: %d", onesComp);
    os_PutStrFull(response);
    os_NewLine();
    calculatePowers(onesComp, 2, powers);
    os_PutStrFull(powers);
    os_NewLine();

    int twosComp = twosComplement(decimal, bitLength);
    sprintf(response, "2's Comp: %d", twosComp);
    os_PutStrFull(response);
    os_NewLine();
    calculatePowers(twosComp, 2, powers);
    os_PutStrFull(powers);
    os_NewLine();

    os_NewLine();
    os_PutStrFull("Menu: any key...");
    while (!os_GetCSC())
        ;
}

void showError(const char *message) {
    os_ClrHome();
    os_PutStrFull(message);
    os_NewLine();
    os_PutStrFull("Any key to continue...");
    while (!os_GetCSC())
        ;
}

int main(void) {
    char input[INPUT_SIZE];
    int decimal;
    uint8_t key;

    displayMenu();

    while (1) {
        key = os_GetCSC();

        if (key == sk_1 || key == sk_2 || key == sk_3 || key == sk_4) {
            if (key == sk_4) {
                os_ClrHome();
                os_PutStrFull("Goodbye!");
                while (!os_GetCSC())
                    ;
                break;
            }

            os_ClrHome();
            if (key == sk_1) {
                os_PutStrFull("Enter binary: ");
                os_GetStringInput("", input, INPUT_SIZE);

                if (!validateBinary(input)) {
                    showError("Invalid binary!");
                    displayMenu();
                    continue;
                }
                decimal = binaryToDecimal(input);
                displayResults(decimal, strlen(input), input[0] == '1');
            } else if (key == sk_2) {
                os_PutStrFull("Enter hex (0x): ");
                os_GetStringInput("", input, INPUT_SIZE);

                if (!validateHex(input)) {
                    showError("Invalid hex!");
                    displayMenu();
                    continue;
                }
                decimal = hexToDecimal(input);
                displayResults(decimal, (strlen(input)) * 4, 0);
            } else if (key == sk_3) {
                os_PutStrFull("Enter decimal: ");
                os_GetStringInput("", input, INPUT_SIZE);

                if (!validateDecimal(input)) {
                    showError("Invalid decimal!");
                    displayMenu();
                    continue;
                }
                decimal = atoi(input);
                int bitLength = 0;
                int temp = decimal;
                while (temp > 0) {
                    temp >>= 1;
                    bitLength++;
                }
                displayResults(decimal, bitLength ? bitLength : 1, 0);
            }

            displayMenu();
        }
    }

    return 0;
}
