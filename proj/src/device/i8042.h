#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define DELAY_US    20000

#define KEYBOARD_IRQ 1
#define MOUSE_IRQ 12

#define KBC_ST_REG 0x64
#define KBC_CMD_REG 0x64
#define KBC_OUT_BUF 0x60
#define KBC_IN_BUF 0x60

#define KBC_ST_PAR_ERR BIT(7)
#define KBC_ST_TO_ERR BIT(6)
#define KBC_ST_AUX BIT(5)
#define KBC_ST_IBF BIT(1)
#define KBC_ST_OBF BIT(0)

#define ESC_BREAK_CODE 0x81
#define SPACEBAR_MAKE_CODE 0x39
#define SPACEBAR_BREAK_CODE 0xb9
#define ARROW_UP_MAKE_CODE 0x48
#define ARROW_UP_BREAK_CODE 0xc8
#define ARROW_DOWN_MAKE_CODE 0x50
#define ARROW_DOWN_BREAK_CODE 0xd0
#define A_MAKE_CODE 0x1e
#define A_BREAK_CODE 0x9e
#define B_MAKE_CODE 0x30
#define B_BREAK_CODE 0xb0
#define C_MAKE_CODE 0x2e
#define C_BREAK_CODE 0xae
#define D_MAKE_CODE 0x20
#define D_BREAK_CODE 0xa0
#define E_MAKE_CODE 0x12
#define E_BREAK_CODE 0X92
#define F_MAKE_CODE 0x21
#define F_BREAK_CODE 0xa1
#define G_MAKE_CODE 0x22
#define G_BREAK_CODE 0xa2
#define H_MAKE_CODE 0x23
#define H_BREAK_CODE 0xa3
#define I_MAKE_CODE 0x17
#define I_BREAK_CODE 0x97
#define J_MAKE_CODE 0x24
#define J_BREAK_CODE 0xa4
#define K_MAKE_CODE 0x25
#define K_BREAK_CODE 0xa5
#define L_MAKE_CODE 0x26
#define L_BREAK_CODE 0xa6
#define M_MAKE_CODE 0x32
#define M_BREAK_CODE 0xb2
#define N_MAKE_CODE 0x31
#define N_BREAK_CODE 0xb1
#define O_MAKE_CODE 0x18
#define O_BREAK_CODE 0x98
#define P_MAKE_CODE 0x19
#define P_BREAK_CODE 0x99
#define Q_MAKE_CODE 0x10
#define Q_BEEAK_CODE 0x90
#define R_MAKE_CODE 0x13
#define R_BREAK_CODE 093 
#define S_MAKE_CODE 0x1f
#define S_BREAK_CODE 0x9f
#define T_MAKE_CODE 0x14
#define T_BREAK_CODE 0x94
#define U_MAKE_CODE 0x16
#define U_BREAK_CODE 0x96
#define V_MAKE_CODE 0x2f
#define V_BREAK_CODE 0xaf
#define W_MAKE_CODE 0x11
#define W_BREAK_CODE 0x91
#define X_MAKE_CODE 0x2d
#define X_BREAK_CODE 0xad
#define Y_MAKE_CODE 0x15
#define Y_BREAK_CODE 0x95
#define Z_MAKE_CODE 0x2c
#define Z_BREAK_CODE 0xac

#define SCANCODE_PREFIX 0xE0
#define BREAK_BIT BIT(7)

#define KBC_READ_CMD_BYTE 0x20
#define KBC_WRITE_CMD_BYTE 0x60
#define DISABLE_MOUSE_CMD 0xA7
#define ENABLE_MOUSE_CMD 0xA8
#define MOUSE_INTERFACE_CMD 0xA9
#define WRITE_BYTE_MOUSE_CMD 0xD4


#define ACK_BYTE 0xFA
#define NACK_BYTE 0xFE
#define ERROR_BYTE 0xFC

#define MOUSE_RESET 0xFF
#define MOUSE_RESEND 0xFE
#define SET_DEFAULTS 0xF6
#define DISABLE_DATA_REPORT 0xF5
#define ENABLE_DATA_REPORT 0xF4
#define SET_SAMPLE_RATE 0xF3
#define SET_REMOTE_MODE 0xF0
#define READ_DATA 0xEB
#define SET_STREAM_MODE 0xEA
#define STATUS_REQUEST 0xE9
#define SET_RESOLUTION 0xE8
#define SET_SCALING_2_1 0xE7
#define SET_SCALING_1_1 0xE6

#define LEFT_BUTTON BIT(0)
#define RIGHT_BUTTON BIT(1)
#define MIDDLE_BUTTON BIT(2)
#define MSB_X_DELTA BIT(4)
#define MSB_Y_DELTA BIT(5)
#define MOUSE_X_OVFL BIT(6)
#define MOUSE_Y_OVFL BIT(7)

#define NEGATE_VALUE BIT(8)

#endif
