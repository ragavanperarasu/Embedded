#include <nxp/iolpc2148.h>

// LCD Pins
#define rs IO0PIN_bit.P0_8
#define en IO0PIN_bit.P0_11

// LCD Commands
unsigned char commandarray[5] = {0x38, 0x01, 0x06, 0x0c, 0x80};

// Delay function
void delay(unsigned int x) {
    unsigned int i;
    for (i = 0; i < x; i++);
}

// LCD Initialization
void lcd_init() {
    int i;
    for (i = 0; i < 5; i++) {
        IO1PIN = commandarray[i] << 16;
        rs = 0;
        en = 1;
        delay(4095);
        en = 0;
    }
}

// LCD Command
void lcd_cmd(unsigned char cmd) {
    IO1PIN = cmd << 16;
    rs = 0;
    en = 1;
    delay(4095);
    en = 0;
}

// LCD Print string
void lcd_print(char *str) {
    while (*str != '\0') {
        IO1PIN = (*str) << 16;
        rs = 1;
        en = 1;
        delay(500);
        en = 0;
        str++;
    }
}

// Integer to string (itoa)
void intToStr(int num, char *str) {
    int i = 0, j, rem;
    char temp[10];
    int isNegative = 0;

    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    if (num < 0) {
        isNegative = 1;
        num = -num;
    }

    while (num != 0) {
        rem = num % 10;
        temp[i++] = rem + '0';
        num /= 10;
    }

    if (isNegative) {
        str[0] = '-';
        for (j = 0; j < i; j++) {
            str[j + 1] = temp[i - j - 1];
        }
        str[i + 1] = '\0';
    } else {
        for (j = 0; j < i; j++) {
            str[j] = temp[i - j - 1];
        }
        str[i] = '\0';
    }
}

// Keypad Scan Function
char get_key() {
    int row_vals[] = {0x000E0000, 0x000D0000, 0x000B0000, 0x00070000};
    char key_map[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '=', 'D'}
    };

    int row, col;

    for (row = 0; row < 4; row++) {
        IO0PIN = row_vals[row]; // set one row low at a time
        delay(1000);
        int col_val = IO0PIN & 0x00F00000;
        if (col_val != 0x00F00000) {
            for (col = 0; col < 4; col++) {
                if ((col_val & (1 << (20 + col))) == 0) {
                    while ((IO0PIN & 0x00F00000) != 0x00F00000); // wait for key release
                    return key_map[row][col];
                }
            }
        }
    }
    return 0;
}

void main() {
    int num1 = 0, num2 = 0;
    char key;
    char buffer[20];
    int input_stage = 1; // 1: input num1, 2: input num2

    // GPIO and LCD Setup
    PINSEL0 = 0x00000000;
    PINSEL2 = 0x00000000;
    IO0DIR |= 0x000F0000; // P0.16 to P0.19 as row output
    IO0DIR &= ~0x00F00000; // P0.20 to P0.23 as column input
    IO0DIR_bit.P0_8 = 1;  // RS as output
    IO0DIR_bit.P0_11 = 1; // EN as output
    IO1DIR = 0x00FF0000;  // LCD data pins on P1.16–P1.23 as output

    lcd_init();
    lcd_cmd(0x80);
    lcd_print("Enter Num1:");

    while (1) {
        key = get_key();

        if (key >= '0' && key <= '9') {
            if (input_stage == 1) {
                num1 = num1 * 10 + (key - '0');
                lcd_cmd(0xC0);
                lcd_print("Num1: ");
                intToStr(num1, buffer);
                lcd_print(buffer);
            } else if (input_stage == 2) {
                num2 = num2 * 10 + (key - '0');
                lcd_cmd(0xC0);
                lcd_print("Num2: ");
                intToStr(num2, buffer);
                lcd_print(buffer);
            }
        } else if (key == '=') {
            if (input_stage == 1) {
                input_stage = 2;
                lcd_cmd(0x01);
                delay(10000);
                lcd_cmd(0x80);
                lcd_print("Enter Num2:");
            }
        } else if (key == 'D') {
            // Calculate subtraction and display
            int result = num1 - num2;
            lcd_cmd(0x01);
            delay(10000);
            lcd_cmd(0x80);
            lcd_print("Result = ");
            intToStr(result, buffer);
            lcd_print(buffer);
        } else if (key == 'C') {
            // Reset all
            num1 = 0;
            num2 = 0;
            input_stage = 1;
            lcd_cmd(0x01);
            delay(10000);
            lcd_cmd(0x80);
            lcd_print("Enter Num1:");
        }

        delay(100000); // debounce
    }
}
