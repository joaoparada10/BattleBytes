/**
 * @file keyboard.c
 * @brief Keyboard interrupt handling and KBC communication.
 */


#include <lcom/lcf.h>
#include "keyboard.h"

int hook_id = 1; /**< @brief Hook ID for keyboard interrupts */
uint8_t keyboard_data; /**< @brief Data read from the keyboard */
uint8_t kbd_status_byte; /**< @brief Status byte from the keyboard controller */
bool discard_keyboard_data = false; /**< @brief Flag to discard erroneous keyboard data */
bool skip_print = false; /**< @brief Flag to skip printing of scancodes */

/**
 * @brief Subscribes keyboard interrupts.
 * 
 * @param bit_no Pointer to the bit number to be set.
 * @return 0 on success, non-zero otherwise.
 */


int (kbc_subscribe_int)(uint8_t *bit_no){
    *bit_no = BIT(hook_id);
    sys_irqsetpolicy(KEYBOARD_IRQ,IRQ_REENABLE | IRQ_EXCLUSIVE,&hook_id);
    return 0;
}

/**
 * @brief Unsubscribes keyboard interrupts.
 * 
 * @return 0 on success, non-zero otherwise.
 */


int (kbc_unsubscribe_int)(){
    sys_irqrmpolicy(&hook_id);
    return 0;
}

/**
 * @brief Issues a command to the KBC.
 * 
 * @param p Port to issue the command to.
 * @param cmd Command to be issued.
 * @return 0 on success, non-zero otherwise.
 */

int (kbc_issue_cmd)(uint8_t p, uint8_t cmd){
    uint8_t attempts = 10;
    while( attempts > 0 ) {
        util_sys_inb(KBC_ST_REG, &kbd_status_byte); /* assuming it returns OK */
        /* loop while 8042 input buffer is not empty */
        if( (kbd_status_byte & KBC_ST_IBF) == 0 ) {
            sys_outb(p, cmd); 
            return 0;
        }
        tickdelay(micros_to_ticks(DELAY_US));
        attempts--;
    }
    return -1;
}

/**
 * @brief Reads a command byte from the KBC.
 * 
 * @return Command byte.
 */

uint8_t (kbc_read_cmd)(){
    uint8_t cmd;
    kbc_issue_cmd(KBC_CMD_REG,KBC_READ_CMD_BYTE);
    util_sys_inb(KBC_IN_BUF,&cmd);
    return cmd;
}

/**
 * @brief Checks if the keyboard status byte is valid.
 * 
 * @param kbd_status_byte Status byte to be checked.
 * @return True if valid, false otherwise.
 */

bool is_valid_kb_st(uint8_t kbd_status_byte){
    if (kbd_status_byte & KBC_ST_PAR_ERR){
        printf("parity error \n");
        return false;
    }
    if (kbd_status_byte & KBC_ST_TO_ERR){
        printf("timeout error \n");
        return false;
    }
    if (kbd_status_byte & KBC_ST_AUX){
        printf("mouse keyboard_data error \n");
        return false;
    }
    return true;
}

/**
 * @brief Reads a value from the KBC output buffer.
 * 
 * @return 0 on success, non-zero otherwise.
 */

int (kbc_read_value)(){
    uint8_t attempts = 10;
    while( attempts > 0 ) {
        if(util_sys_inb(KBC_ST_REG, &kbd_status_byte) != 0){
            printf("error reading status \n");
            return -1;
        };
        if( kbd_status_byte & KBC_ST_OBF ) {
            if (util_sys_inb(KBC_OUT_BUF, &keyboard_data) != 0){
                printf("error reading kbc output buffer \n");
                return -1;
            };
            if (!is_valid_kb_st(kbd_status_byte)){
                printf("invalid status \n");
                return -1;
            }
            return 0;
        }
        tickdelay(micros_to_ticks(DELAY_US));
        attempts--;
    }
    printf ("timed out! \n");
    return 1;
}

/**
 * @brief Keyboard interrupt handler.
 */

void (kbc_ih)(){
    if (kbc_read_value() != 0){
        printf("error reading kbc obf byte");
        discard_keyboard_data = true;
    };
}

/**
 * @brief Processes scancodes.
 * 
 * @param is_make Pointer to a boolean that indicates if it is a make code.
 * @param size Pointer to the size of the scancode.
 * @param bytes Pointer to the array of scancode bytes.
 */

void process_scancode(bool* is_make,uint8_t* size, uint8_t* bytes){
    if (keyboard_data == SCANCODE_PREFIX){ // this means the scancode will have 2 bytes
        bytes[0] = keyboard_data;    // set the 1st byte to 0xE0
        skip_print = true;
        *size = 2;
    }
    else {    // process just 1 byte
        if(*size == 2){
            bytes[1] = keyboard_data;  // set the 2nd byte to the value read by the ih
            if (keyboard_data & BREAK_BIT) // if the msb bit is 1 its a breakcode
                *is_make = false;
            else 
                *is_make = true;
            skip_print = false;
        }
        else{
            bytes[0] = keyboard_data;
            
            if (keyboard_data & BREAK_BIT)
                *is_make = false;
            else 
                *is_make = true;
            }
    }
}
