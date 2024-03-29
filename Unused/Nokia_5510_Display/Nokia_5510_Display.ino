#include <Nokia_LCD.h>
#include <Wire.h>

#define pot_in A0
int input;
byte current_num;
Nokia_LCD lcd(D8, D7, D6, D5, D4);  // Initialize Nokia 5510 display; adjust pins as needed

const long interval = 2000;  // Time interval for number acceptance
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
int place = 0;  // Place on the LCD display
byte password[4] = { 1, 2, 3, 4 };
byte enteredPassword[4];
byte temp_num = 20;
bool allowed_access = true;
bool locked = true;

const unsigned char SUS_logo[4032] = {
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x60, 0x60, 0x5F,
0x5F, 0x5E, 0x5E, 0x5E, 0x5E, 0x5F, 0x5E, 0x5E, 0x5E, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x60, 0x60, 0x60, 0x5F,
0x5F, 0x5F, 0x5F, 0x60, 0x5F, 0x5F, 0x5E, 0x5E, 0x5E, 0x5E,
0x5E, 0x5F, 0x5F, 0x5F, 0x5E, 0x5E, 0x5E, 0x5D, 0x5E, 0x5D,
0x5D, 0x5D, 0x5D, 0x5C, 0x5C, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5E, 0x5F, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E,
0x5E, 0x5E, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5E, 0x5E, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5E,
0x5E, 0x5E, 0x5E, 0x5E, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5E,
0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5D, 0x5C, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5E, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5D, 0x5C,
0x5D, 0x5D, 0x5D, 0x5D, 0x5D, 0x5E, 0x5E, 0x5E, 0x5F, 0x5F,
0x5F, 0x60, 0x5F, 0x5F, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E,
0x5E, 0x5D, 0x5D, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5E,
0x5E, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x60, 0x5F, 0x5E, 0x5D, 0x74, 0xAC, 0xCF,
0xE7, 0xF3, 0xFE, 0xFF, 0xFC, 0xED, 0xE2, 0xC9, 0xA0, 0x61,
0x5D, 0x5E, 0x5F, 0x5F, 0x60, 0x5F, 0x5F, 0x60, 0x5F, 0x5F,
0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5E, 0x5E, 0x5E, 0x5E, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x60, 0x5F, 0x5F, 0x5E, 0xA4,
0xDE, 0xFF, 0xFF, 0xF8, 0xE0, 0xCC, 0xC1, 0xBD, 0xC5, 0xCE,
0xE7, 0xFE, 0xFF, 0xF8, 0xCE, 0x86, 0x5E, 0x5E, 0x5F, 0x5F,
0x60, 0x60, 0x5F, 0x5F, 0x5F, 0x5F, 0x5E, 0x5E, 0x5E, 0x5E,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5E, 0x5E, 0x5E,
0x5E, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x60, 0x60, 0x5F,
0x5E, 0x9A, 0xEB, 0xFF, 0xF3, 0xBE, 0x79, 0x16, 0x56, 0x16,
0x15, 0x55, 0x16, 0x16, 0x1D, 0x3A, 0x96, 0xD4, 0xFD, 0xFF,
0xD3, 0x77, 0x5E, 0x5F, 0x60, 0x5F, 0x60, 0x60, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5D, 0x5E, 0x5E, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x60,
0x60, 0x61, 0x5F, 0x6A, 0xD0, 0xFF, 0xEE, 0xA3, 0x1C, 0x17,
0xA7, 0x2B, 0x35, 0x16, 0x16, 0x4D, 0x15, 0x16, 0x64, 0x17,
0x17, 0x17, 0x56, 0xC6, 0xFC, 0xF9, 0xAB, 0x5E, 0x5E, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5E, 0x5F, 0x5F, 0x5F, 0x5E, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5D, 0x5E, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x60, 0x61, 0x60, 0x5F, 0x80, 0xEA, 0xFE, 0xBD,
0x3F, 0x3D, 0x5E, 0x17, 0x4B, 0x19, 0xA4, 0xDD, 0xE6, 0xDE,
0xC4, 0x8B, 0x49, 0xAA, 0xA5, 0x7E, 0x1F, 0x16, 0x71, 0xE5,
0xFF, 0xCE, 0x5E, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x60, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5E,
0x5E, 0x5E, 0x5F, 0x5F, 0x5F, 0x5F, 0x60, 0x60, 0x5E, 0x80,
0xF2, 0xFA, 0x8D, 0x17, 0x7C, 0x87, 0x1C, 0x16, 0x43, 0xDE,
0xD2, 0x96, 0x81, 0x84, 0x8A, 0x8A, 0x68, 0x8D, 0xDC, 0xE4,
0xEB, 0xCA, 0x6C, 0x27, 0xC8, 0xFF, 0xD4, 0x5E, 0x5E, 0x5F,
0x5F, 0x60, 0x60, 0x5F, 0x60, 0x5F, 0x5E, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5F, 0x60,
0x60, 0x5F, 0x6F, 0xF0, 0xF8, 0x82, 0x16, 0x7F, 0xD9, 0xDE,
0x82, 0x17, 0xD6, 0xA4, 0x63, 0x85, 0x9F, 0x90, 0x65, 0x69,
0x87, 0x54, 0x8F, 0xC4, 0xC7, 0xF5, 0xEC, 0xC0, 0x32, 0xB1,
0xFF, 0xCF, 0x5D, 0x5E, 0x5F, 0x60, 0x60, 0x5F, 0x5F, 0x5F,
0x5E, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5E, 0x5E, 0x5F,
0x5E, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0xE0, 0xFA, 0x86, 0x17,
0x5A, 0x48, 0xBE, 0xCC, 0x4E, 0xA2, 0xAA, 0x85, 0xED, 0xF1,
0xDA, 0xC6, 0xC3, 0x7B, 0x51, 0x6B, 0x42, 0x9C, 0xA1, 0xD4,
0xD9, 0xE9, 0xE2, 0x49, 0xC3, 0xFF, 0xB0, 0x5E, 0x5E, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5E, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x60, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0xB9,
0xFF, 0xA5, 0x17, 0x96, 0xD8, 0x3F, 0x6C, 0x81, 0x17, 0xBE,
0x60, 0xF0, 0xFF, 0xFF, 0xE0, 0xC6, 0xC5, 0xAA, 0x65, 0x4E,
0x4D, 0x68, 0xA7, 0x7B, 0xBC, 0xD9, 0xFF, 0xE8, 0x43, 0xDC,
0xFB, 0x82, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5E, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5E, 0x77, 0xFA, 0xD7, 0x17, 0x17, 0x17, 0x7C, 0x4E,
0x16, 0x16, 0x17, 0xA6, 0x83, 0xF0, 0xFF, 0xFF, 0xD9, 0xC6,
0xC5, 0xB2, 0x7B, 0x3C, 0x56, 0x4A, 0x96, 0x9D, 0xDE, 0x9B,
0xEE, 0xF1, 0xDC, 0x57, 0xF7, 0xDD, 0x5E, 0x5E, 0x5F, 0x5F,
0x5F, 0x5E, 0x5E, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5E, 0x5F, 0x5F, 0x5E, 0x5D, 0xCF, 0xFA, 0x59, 0x63,
0x66, 0x17, 0x17, 0x41, 0x55, 0x16, 0x17, 0x8F, 0x84, 0xCB,
0xE2, 0xD8, 0xC6, 0xC5, 0xC5, 0xAD, 0x7C, 0x40, 0x55, 0x4A,
0x8A, 0xAB, 0xD5, 0xA9, 0x8D, 0xC0, 0xD4, 0xA7, 0xAE, 0xFF,
0x96, 0x5E, 0x5F, 0x5F, 0x5F, 0x5F, 0x5E, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5D, 0x73,
0xFB, 0xC5, 0x81, 0x9D, 0x8D, 0x91, 0x17, 0x17, 0x87, 0x3E,
0x16, 0x66, 0x64, 0xC5, 0xC5, 0xC4, 0xC4, 0xC4, 0xC4, 0x8D,
0x75, 0x46, 0x49, 0x54, 0x8A, 0xAB, 0xD6, 0x9E, 0x8B, 0x8B,
0xCD, 0xF4, 0x40, 0xF2, 0xDB, 0x5E, 0x5F, 0x5F, 0x5F, 0x5F,
0x5E, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5E, 0xBB, 0xFF, 0x61, 0x9C, 0x1D, 0x17, 0xA2,
0x17, 0x17, 0x3B, 0x16, 0x17, 0x68, 0x6E, 0x82, 0xB8, 0xC2,
0xC2, 0xBC, 0x94, 0x7B, 0x48, 0x5D, 0x34, 0x5E, 0x8A, 0xAA,
0xC5, 0xE1, 0xB7, 0xAD, 0xFC, 0xFA, 0x9F, 0xB9, 0xFE, 0x78,
0x5E, 0x5F, 0x5F, 0x5F, 0x5E, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x60, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0xE5, 0xE0, 0x1A,
0x7F, 0x98, 0xB0, 0x68, 0x17, 0x1F, 0xC9, 0x5D, 0x16, 0x9B,
0x59, 0x65, 0x54, 0x7B, 0x86, 0x78, 0x69, 0x43, 0x58, 0x40,
0x28, 0x4F, 0x8A, 0xAA, 0xBD, 0xCB, 0xEF, 0xFF, 0xE8, 0xCF,
0xD4, 0x61, 0xFF, 0xB4, 0x5E, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5E, 0x5E, 0x5E,
0x6B, 0xFF, 0xB8, 0xA8, 0xB4, 0xEC, 0xC6, 0xA0, 0x16, 0x17,
0x59, 0x16, 0x22, 0xCA, 0xA9, 0x48, 0x6D, 0x5D, 0x55, 0x4F,
0x50, 0x5B, 0x3A, 0x4D, 0x63, 0x4A, 0x63, 0xAA, 0xBD, 0xD3,
0xEE, 0xFC, 0xDD, 0xC3, 0xDF, 0x17, 0xEC, 0xDA, 0x5E, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5E,
0x5F, 0x5F, 0x5E, 0x5E, 0x97, 0xFF, 0x89, 0xAC, 0x9B, 0xF4,
0x82, 0xBC, 0xC0, 0x9B, 0x17, 0x53, 0xDE, 0xE3, 0x8F, 0x7F,
0x55, 0x3D, 0x47, 0x42, 0x32, 0x3F, 0x69, 0x7F, 0x89, 0xB5,
0xA4, 0x4B, 0xC0, 0xC4, 0xD1, 0xEA, 0xC5, 0xBE, 0xD3, 0x24,
0xCA, 0xF1, 0x5E, 0x5F, 0x5F, 0x5F, 0x5E, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5E, 0x5F, 0x5F, 0x5E, 0x5E, 0xB4, 0xFF,
0x5C, 0xAC, 0x9A, 0xF4, 0xC9, 0x3E, 0x4C, 0x99, 0xC4, 0xF6,
0xDD, 0x99, 0x8D, 0x8A, 0x7B, 0x79, 0x70, 0x6E, 0x76, 0x82,
0x8A, 0x8A, 0xBE, 0xEC, 0xC6, 0xA7, 0xA0, 0xC3, 0xC4, 0xD3,
0xD5, 0xAE, 0xC2, 0x40, 0xAC, 0xFF, 0x5D, 0x5E, 0x5F, 0x5E,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5E, 0x5E, 0x5E,
0x5E, 0x5F, 0xB8, 0xFF, 0x17, 0xAB, 0x9B, 0xEA, 0xA7, 0xA6,
0x38, 0x76, 0xB5, 0x91, 0x8B, 0x7D, 0xAF, 0x7C, 0xD7, 0xD0,
0xD2, 0xD3, 0xD5, 0x5E, 0x81, 0x80, 0xC2, 0x94, 0xB2, 0xD0,
0x81, 0xB5, 0xC2, 0xC5, 0xDA, 0x81, 0xBA, 0x1C, 0xA7, 0xFF,
0x77, 0x5E, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5E, 0x5E, 0x5E, 0xC2, 0xFF, 0x17, 0xA5,
0x9A, 0xE9, 0xB3, 0xA0, 0x51, 0x84, 0x8B, 0x8C, 0x88, 0x99,
0xA9, 0x91, 0xBD, 0xD6, 0xD6, 0xD5, 0xAE, 0x74, 0x5B, 0x71,
0x74, 0x7E, 0x9D, 0x8B, 0xD0, 0x6B, 0xC3, 0xC1, 0xA7, 0x6D,
0x9D, 0x16, 0xA3, 0xFF, 0x8E, 0x5E, 0x5E, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E,
0xBC, 0xFF, 0x17, 0x2B, 0x40, 0x57, 0x57, 0x43, 0x5F, 0x72,
0x7B, 0x77, 0x5F, 0xBA, 0xA9, 0x92, 0xC4, 0xAF, 0xAE, 0xD9,
0xB0, 0x77, 0x54, 0x6D, 0x3C, 0x6F, 0x7E, 0x8B, 0xA6, 0xA5,
0xAA, 0xA1, 0x77, 0x67, 0x54, 0x17, 0xA7, 0xFF, 0x82, 0x5E,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5E,
0x5E, 0x5E, 0x5E, 0x5E, 0xB6, 0xFF, 0x47, 0x16, 0x16, 0x17,
0x16, 0x1A, 0x42, 0x54, 0x56, 0x53, 0x3D, 0xCA, 0xA9, 0x92,
0xC4, 0xAF, 0x5A, 0xDC, 0xAF, 0x74, 0x54, 0x77, 0x59, 0x43,
0x61, 0x81, 0x8B, 0xC8, 0x56, 0x78, 0x67, 0x67, 0x36, 0x17,
0xAC, 0xFF, 0x5E, 0x5E, 0x5E, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5D, 0x5E, 0x5E, 0x5E, 0x5F, 0xA5, 0xFF,
0x7E, 0x6A, 0xB7, 0x3C, 0x16, 0x16, 0x16, 0x15, 0x16, 0x16,
0x17, 0xCB, 0xA9, 0x91, 0xC3, 0xFA, 0xF7, 0xFD, 0xAE, 0x74,
0x55, 0x7D, 0x64, 0x2E, 0x49, 0x69, 0x8B, 0xBD, 0x68, 0x79,
0x68, 0x5C, 0x17, 0x17, 0xCA, 0xF5, 0x5D, 0x5E, 0x5F, 0x5E,
0x60, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5E, 0x5E, 0x5E,
0x5F, 0x5F, 0x7F, 0xFF, 0xAA, 0x3A, 0x90, 0x16, 0x1F, 0x6A,
0x8C, 0x3E, 0x47, 0x53, 0x17, 0xCC, 0xAA, 0x91, 0xC2, 0xB9,
0xBC, 0xBD, 0x9A, 0x69, 0x56, 0x7D, 0x63, 0x31, 0x48, 0x79,
0x8B, 0xA7, 0x8C, 0x77, 0x65, 0x30, 0x17, 0x17, 0xE7, 0xE1,
0x5D, 0x5E, 0x5E, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x60, 0xF1, 0xD7, 0x17,
0x17, 0x17, 0x85, 0xB7, 0x53, 0x17, 0x16, 0x17, 0x17, 0xCB,
0xAA, 0x75, 0x7D, 0x5D, 0x57, 0x58, 0x5D, 0x42, 0x5F, 0x80,
0x62, 0x31, 0x80, 0x7C, 0x8A, 0x9B, 0x93, 0x6F, 0x45, 0x63,
0x33, 0x47, 0xFF, 0xC3, 0x5E, 0x5F, 0x5F, 0x5F, 0x60, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x60, 0x5F, 0x5F, 0x5F,
0x5F, 0xCE, 0xEA, 0x17, 0x17, 0x7D, 0xEB, 0xF7, 0x9E, 0x17,
0x17, 0x67, 0x17, 0xCC, 0xAA, 0x8B, 0x8B, 0x85, 0x7D, 0x7A,
0x78, 0x78, 0x7E, 0x8B, 0x62, 0x38, 0x71, 0x25, 0x80, 0x88,
0x4A, 0x46, 0x15, 0x7D, 0x52, 0x7D, 0xFA, 0x92, 0x5E, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x60,
0x60, 0x5F, 0x5F, 0x5F, 0x5F, 0x76, 0x46, 0x3A, 0x17, 0x17,
0x7C, 0xB0, 0x16, 0x58, 0x8C, 0x17, 0x17, 0xCB, 0xAA, 0x8B,
0x8B, 0x86, 0x75, 0x74, 0x81, 0x8B, 0x8B, 0x8B, 0x63, 0x31,
0x1A, 0x2D, 0x68, 0x59, 0x16, 0x39, 0xA3, 0x22, 0x17, 0x45,
0x62, 0x5E, 0x5F, 0x5F, 0x60, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x60, 0x5F, 0x5F, 0x5F,
0x61, 0x55, 0x17, 0x17, 0x15, 0x66, 0x16, 0x85, 0xAF, 0x16,
0x17, 0xCC, 0xA7, 0x8B, 0x88, 0x64, 0x47, 0x3A, 0x58, 0x7B,
0x8C, 0x8A, 0x67, 0x31, 0x61, 0x26, 0x3E, 0x1C, 0x17, 0x43,
0xA2, 0x37, 0x2C, 0x62, 0x61, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5E, 0x5F, 0x5F,
0x5F, 0x60, 0x60, 0x60, 0x5B, 0x26, 0x58, 0x5A, 0x5B, 0x5C,
0x5C, 0x5C, 0x5C, 0x5E, 0x4F, 0xC9, 0x9E, 0x88, 0x77, 0x56,
0x15, 0x59, 0x29, 0x63, 0x87, 0x88, 0x69, 0x31, 0x32, 0x56,
0x5A, 0x5A, 0x59, 0x59, 0x58, 0x58, 0x42, 0x40, 0x61, 0x60,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5E, 0x5E, 0x5E, 0x5F, 0x60, 0x60, 0x5F, 0x5F, 0x48,
0x48, 0x68, 0x9E, 0xB2, 0xBD, 0xC5, 0xCA, 0xCA, 0xAA, 0xC6,
0x97, 0x85, 0x72, 0x52, 0x14, 0xA0, 0x15, 0x60, 0x81, 0x84,
0x67, 0x37, 0xC8, 0xCB, 0xC9, 0xBE, 0xB6, 0xA8, 0x7C, 0x5A,
0x2E, 0x5C, 0x60, 0x60, 0x5F, 0x5F, 0x5F, 0x5F, 0x5E, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5E, 0x5E, 0x5E, 0x5F, 0x5F,
0x60, 0x60, 0x5F, 0x5F, 0x40, 0x6F, 0xBE, 0xCB, 0xCB, 0xCB,
0xCB, 0xCA, 0xAB, 0xC2, 0x8E, 0x84, 0x6F, 0x51, 0x14, 0xA1,
0x15, 0x61, 0x7C, 0x7F, 0x65, 0x3C, 0xCA, 0xCA, 0xCB, 0xCB,
0xCA, 0xC4, 0x96, 0x20, 0x59, 0x61, 0x60, 0x5F, 0x5F, 0x5F,
0x5F, 0x5E, 0x5E, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x60, 0x61, 0x59, 0x1D,
0x6D, 0x8B, 0x94, 0x87, 0x95, 0xA2, 0x91, 0xAD, 0x76, 0x6E,
0x60, 0x4A, 0x14, 0x92, 0x15, 0x59, 0x68, 0x6A, 0x5A, 0x35,
0xA2, 0x9B, 0x88, 0x99, 0x8D, 0x6A, 0x15, 0x44, 0x61, 0x61,
0x60, 0x60, 0x5F, 0x5F, 0x5F, 0x5F, 0x5E, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5E, 0x5E, 0x5F,
0x60, 0x4E, 0x75, 0xEA, 0xFF, 0xFF, 0xFF, 0xED, 0x94, 0x16,
0x17, 0xA2, 0xAB, 0x9A, 0x16, 0x16, 0x17, 0x17, 0x17, 0x17,
0xAA, 0xAB, 0x9A, 0x16, 0x16, 0x85, 0xEC, 0xFF, 0xFF, 0xFF,
0xE5, 0x7B, 0x3F, 0x5F, 0x61, 0x60, 0x5F, 0x60, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x60, 0x5F,
0x5F, 0x5E, 0x5E, 0x5F, 0x60, 0x48, 0xF2, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFB, 0x50, 0x17, 0xF3, 0xFF, 0xE7, 0x17, 0x17,
0x17, 0x17, 0x17, 0x17, 0xFF, 0xFF, 0xE7, 0x16, 0x54, 0xFA,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x16, 0x58, 0x60, 0x5F,
0x5F, 0x5F, 0x5F, 0x5E, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x60, 0x60, 0x5F, 0x5F, 0x5F, 0x5F, 0x86,
0xFF, 0xFF, 0xD5, 0x2C, 0x8A, 0xE6, 0xB4, 0x17, 0x17, 0xF6,
0xFF, 0xE7, 0x17, 0x17, 0x15, 0x16, 0x17, 0x17, 0xFF, 0xFF,
0xE7, 0x16, 0xA8, 0xFF, 0xFF, 0xBE, 0x34, 0x92, 0xEC, 0x93,
0x17, 0x58, 0x60, 0x5F, 0x5F, 0x5F, 0x5F, 0x5E, 0x5E, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5E, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x7B, 0xFF, 0xFF, 0xF2, 0xB8, 0x8E, 0x4A,
0x16, 0x17, 0x17, 0xF6, 0xFF, 0xE7, 0x17, 0x17, 0x16, 0x17,
0x17, 0x17, 0xFF, 0xFF, 0xE7, 0x17, 0x9C, 0xFF, 0xFF, 0xEA,
0xB4, 0x88, 0x27, 0x17, 0x17, 0x59, 0x60, 0x5F, 0x5E, 0x5F,
0x5E, 0x5D, 0x5E, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5E,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x31, 0xDB, 0xFF,
0xFF, 0xFF, 0xFF, 0xF7, 0xA3, 0x16, 0x17, 0xF2, 0xFF, 0xE7,
0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0xFF, 0xFF, 0xE7, 0x16,
0x3A, 0xEA, 0xFF, 0xFF, 0xFF, 0xFF, 0xF4, 0x8B, 0x17, 0x5A,
0x61, 0x60, 0x5F, 0x5F, 0x5F, 0x5E, 0x5E, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5E, 0x5E, 0x5E, 0x5E, 0x5F, 0x5F,
0x5F, 0x31, 0x2C, 0xB0, 0xDF, 0xFA, 0xFF, 0xFF, 0xFF, 0x73,
0x17, 0xE9, 0xFF, 0xFA, 0x48, 0x17, 0x17, 0x17, 0x17, 0x7D,
0xFF, 0xFF, 0xDB, 0x17, 0x17, 0x44, 0xBB, 0xE2, 0xFD, 0xFF,
0xFF, 0xFA, 0x52, 0x59, 0x61, 0x60, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5E,
0x5E, 0x5E, 0x5F, 0x5F, 0x5F, 0x31, 0xC6, 0x97, 0x16, 0x21,
0xCD, 0xFF, 0xFF, 0xAA, 0x17, 0xC9, 0xFF, 0xFF, 0xD2, 0x25,
0x16, 0x16, 0x41, 0xE6, 0xFF, 0xFF, 0xB0, 0x17, 0x2A, 0xD7,
0x6D, 0x15, 0x3E, 0xDE, 0xFF, 0xFF, 0x89, 0x59, 0x60, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x60, 0x5F, 0x5E, 0x5F, 0x5E, 0x5E, 0x5F, 0x5F, 0xA1,
0xFF, 0xFF, 0xE2, 0xCC, 0xF1, 0xFF, 0xFF, 0x84, 0x17, 0x63,
0xFA, 0xFF, 0xFF, 0xEE, 0xCC, 0xCE, 0xF6, 0xFF, 0xFF, 0xF0,
0x3D, 0x17, 0xBD, 0xFF, 0xFB, 0xD7, 0xCC, 0xF9, 0xFF, 0xFF,
0x6F, 0x5D, 0x60, 0x60, 0x5F, 0x5F, 0x60, 0x60, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x60, 0x5F, 0x5F, 0x5F, 0x5F,
0x5E, 0x5E, 0x5E, 0x7C, 0xE7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xCA, 0x16, 0x17, 0x16, 0x88, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xF2, 0x69, 0x16, 0x17, 0x66, 0xED, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xBD, 0x60, 0x60, 0x60, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5E, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5E, 0x5E, 0x5F, 0x5F, 0x5F, 0x60, 0xA6,
0xCF, 0xD2, 0xCE, 0x9D, 0x53, 0x44, 0x2E, 0x16, 0x16, 0x53,
0xBD, 0xE6, 0xFB, 0xFA, 0xE4, 0xB8, 0x42, 0x17, 0x17, 0x26,
0x3C, 0x5A, 0xAF, 0xD1, 0xD3, 0xCA, 0x96, 0x5F, 0x5F, 0x5F,
0x60, 0x5F, 0x5E, 0x5F, 0x5F, 0x5F, 0x5E, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5E, 0x5E, 0x5F, 0x5F, 0x5E, 0x5F, 0x60,
0x5F, 0x60, 0x60, 0x60, 0x5F, 0x5F, 0x5F, 0x5F, 0x60, 0x60,
0x60, 0x5E, 0x56, 0x4D, 0x46, 0x40, 0x3F, 0x3E, 0x3F, 0x42,
0x4C, 0x53, 0x5C, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5E, 0x5F, 0x5E, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5E, 0x5E, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x60, 0x60, 0x60, 0x5F, 0x5F, 0x5F,
0x5F, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x61, 0x61,
0x60, 0x5F, 0x5F, 0x60, 0x60, 0x60, 0x61, 0x61, 0x60, 0x60,
0x5F, 0x60, 0x60, 0x60, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5E,
0x5E, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5E, 0x5E, 0x5F, 0x5F, 0x5F, 0x5E, 0x5F, 0x5F, 0x5F,
0x60, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x60, 0x60, 0x60,
0x60, 0x61, 0x61, 0x60, 0x5F, 0x5F, 0x5F, 0x60, 0x60, 0x60,
0x61, 0x60, 0x60, 0x60, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x60, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E,
0x5D, 0x5D, 0x5E, 0x5E, 0x60, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x60, 0x5F, 0x5F, 0x5F, 0x5F, 0x60, 0x60, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5E, 0x5E, 0x5F, 0x5F, 0x5F, 0x5F, 0x5E, 0x5D,
0x5D, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5F, 0x5F, 0x5F,
0x5F, 0x5E, 0x5E, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
0x5F, 0x5F};
void setup() {
  Serial.begin(15200);
  pinMode(pot_in, INPUT_PULLUP);  // Initialize potentiometer

  // Initialize Nokia 5510 LCD display
  lcd.begin();
  lcd.setContrast(60);
  lcd.clear();
  
  // Display initial text
  lcd.setCursor(0, 0);
  lcd.print("Enter password");
}

void loop() {
  if (locked) {
    // Processing input from potentiometer
    input = analogRead(pot_in);
    current_num = (byte)map(input, 10, 1023, 0, 9);

    // Display current number
    lcd.setCursor(place * 15, 1);
    lcd.print(current_num);

    if (current_num != temp_num) {
      temp_num = current_num;
      currentMillis = millis();
      previousMillis = currentMillis;
    } else {
      currentMillis = millis();
    }

    // Timer
    if (currentMillis - previousMillis >= interval && current_num == temp_num) {
      previousMillis = currentMillis;
      //lcd.setCursor(place, 1);
      //lcd.print(current_num);
      enteredPassword[place] = current_num;
      place++;

      if (place >= 4) {
        allowed_access = true;
        for (int i = 0; i < place; i++) {
          if (password[i] != enteredPassword[i]) {
            allowed_access = false;
          }
        }

        if (allowed_access) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("DOOR IS OPEN!");  // Unlock
          delay(3000);
          lcd.draw(SUS_logo,
                   sizeof(SUS_logo) / sizeof(SUS_logo[0]),
                   true);
          delay(10000);
          locked = false;
        } else {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("ACCESS DENIED");  // Deny access
          delay(2000);
          place = 0;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Enter Password");
          temp_num = 15;
        }
      }
    }
  }
}
