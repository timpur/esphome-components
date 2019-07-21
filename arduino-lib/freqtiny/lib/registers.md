# I2C Registers

| REG                    | BIT 7        | BIT 6        | BIT 5        | BIT 4        | BIT 3        | BIT 2        | BIT 1        | BIT 0        |
| ---------------------- | ------------ | ------------ | ------------ | ------------ | ------------ | ------------ | ------------ | ------------ |
| Identification (0x00)  | ID4(1)       | ID3(0)       | ID2(1)       | ID1(0)       | ID0(1)       | VER2(0)      | VER1(0)      | VER0(0)      |
| Address (0x01)         | ADDR7(0)     | ADDR6(0)     | ADDR5(0)     | ADDR4(0)     | ADDR3(0)     | ADDR2(0)     | ADDR1(0)     | ADDR0(0)     |
| Config (0x02)          | ----         | ----         | PWF2(0)      | PWF1(1)      | PWF0(0)      | INUP(1)      | PIN1(0)      | PIN0(1)      |
| Control (0x03)         | ----         | ----         | ----         | ----         | CALC(0)      | SAV(0)       | CMT(0)       | RST(0)       |
| Frequency A (0x04)     | FREQA7(0)    | FREQA6(0)    | FREQA5(0)    | FREQA4(0)    | FREQA3(0)    | FREQA2(0)    | FREQA1(0)    | FREQA0(0)    |
| Frequency B (0x05)     | FREQB7(0)    | FREQB6(0)    | FREQB5(0)    | FREQB4(0)    | FREQB3(0)    | FREQB2(0)    | FREQB1(0)    | FREQB0(0)    |
| Frequency C (0x06)     | FREQC7(0)    | FREQC6(0)    | FREQC5(0)    | FREQC4(0)    | FREQC3(0)    | FREQC2(0)    | FREQC1(0)    | FREQC0(0)    |
| Frequency D (0x07)     | FREQD7(0)    | FREQD6(0)    | FREQD5(0)    | FREQD4(0)    | FREQD3(0)    | FREQD2(0)    | FREQD1(0)    | FREQD0(0)    |
| PWM (0x08)             | PWM7(0)      | PWM6(0)      | PWM5(0)      | PWM4(0)      | PWM3(0)      | PWM2(0)      | PWM1(0)      | PWM0(0)      |
| Frequency MAX A (0x09) | FREQMAXA7(0) | FREQMAXA6(0) | FREQMAXA5(0) | FREQMAXA4(0) | FREQMAXA3(0) | FREQMAXA2(0) | FREQMAXA1(0) | FREQMAXA0(0) |
| Frequency MAX B (0x0A) | FREQMAXB7(0) | FREQMAXB6(0) | FREQMAXB5(0) | FREQMAXB4(0) | FREQMAXB3(0) | FREQMAXB2(0) | FREQMAXB1(0) | FREQMAXB0(0) |
| Frequency MAX C (0x0B) | FREQMAXC7(0) | FREQMAXC6(0) | FREQMAXC5(0) | FREQMAXC4(0) | FREQMAXC3(0) | FREQMAXC2(0) | FREQMAXC1(0) | FREQMAXC0(0) |
| Frequency MAX D (0x0C) | FREQMAXD7(0) | FREQMAXD6(0) | FREQMAXD5(0) | FREQMAXD4(0) | FREQMAXD3(0) | FREQMAXD2(0) | FREQMAXD1(0) | FREQMAXD0(0) |
| PWM MAX (0x0D)         | PWMMAX7(0)   | PWMMAX6(0)   | PWMMAX5(0)   | PWMMAX4(0)   | PWMMAX3(0)   | PWMMAX2(0)   | PWMMAX1(0)   | PWMMAX0(0)   |
| Frequency MIN A (0x0E) | FREQMINA7(0) | FREQMINA6(0) | FREQMINA5(0) | FREQMINA4(0) | FREQMINA3(0) | FREQMINA2(0) | FREQMINA1(0) | FREQMINA0(0) |
| Frequency MIN B (0x1F) | FREQMINB7(0) | FREQMINB6(0) | FREQMINB5(0) | FREQMINB4(0) | FREQMINB3(0) | FREQMINB2(0) | FREQMINB1(0) | FREQMINB0(0) |
| Frequency MIN C (0x10) | FREQMINC7(0) | FREQMINC6(0) | FREQMINC5(0) | FREQMINC4(0) | FREQMINC3(0) | FREQMINC2(0) | FREQMINC1(0) | FREQMINC0(0) |
| Frequency MIN D (0x11) | FREQMIND7(0) | FREQMIND6(0) | FREQMIND5(0) | FREQMIND4(0) | FREQMIND3(0) | FREQMIND2(0) | FREQMIND1(0) | FREQMIND0(0) |
| PWM MIN (0x12)         | PWMMIN7(0)   | PWMMIN6(0)   | PWMMIN5(0)   | PWMMIN4(0)   | PWMMIN3(0)   | PWMMIN2(0)   | PWMMIN1(0)   | PWMMIN0(0)   |

## Address

Requires save and power reset to take effect

## Config

## Pin Mapping

Bits: 0 - 1 Min:0 Max:3 Default: 1

0: Pin 5
1: Pin 3
2: Pin 4
3: Pin 1

## Input Pull up

Bits: 2 Min: 0 Max: 1 Default: 1

Enables pull up on the configured pin

## Pulse Width Filter

Bits:3 - 5 Min: 0 Max: 7 Default: 2

Ignore pulses under VALUE \* 50us, eg 2 \* 50us = 100us

## Control

1 = Reset to defaults
2 = Commit config to memory
4 = Save config to flash
8 = Calculate frequency data
