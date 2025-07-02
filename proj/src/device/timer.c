/**
 * @file timer.c
 * @brief Timer interrupt handling and configuration.
 */

#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int timer0_hook_id = 0; /**< @brief Hook ID for timer interrupts */
int timer_counter = 0; /**< @brief Counter for timer interrupts */

/**
 * @brief Sets the frequency of a timer.
 * 
 * @param timer Timer to be configured.
 * @param freq Desired frequency.
 * @return 0 on success, non-zero otherwise.
 */

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  u_int32_t minFreq = TIMER_FREQ / 65535;  // 65535 is the counter max
  if (freq < minFreq || freq > TIMER_FREQ) return 1;

  uint8_t counterMSB;
  uint8_t counterLSB;
  uint16_t counterValue = TIMER_FREQ / freq;  // we get the correct counter value according to the desired frequency
  util_get_MSB(counterValue, &counterMSB);  // split it into two
  util_get_LSB(counterValue, &counterLSB);

  uint8_t st;
  timer_get_conf(timer, &st); // loads the timer config to st
  st = st | TIMER_LSB_MSB;  // sets bits 4 and 5 to 1
  st = st & 0x3F; // sets bits 6 and 7 to 0
  st = st | (timer << 6); // sets the bits of the desired timer
  sys_outb(TIMER_CTRL,st);  // sends the control word to the control register
  sys_outb(TIMER_0+ timer,counterLSB);  // now we can set the timer counter to the desired value
  return sys_outb(TIMER_0 + timer,counterMSB);  // lsb first then msb
}

/**
 * @brief Subscribes timer interrupts.
 * 
 * @param bit_no Pointer to the bit number to be set.
 * @return 0 on success, non-zero otherwise.
 */

int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = BIT(timer0_hook_id);

  sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE,&timer0_hook_id);

  return 0;
}

/**
 * @brief Unsubscribes timer interrupts.
 * 
 * @return 0 on success, non-zero otherwise.
 */

int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&timer0_hook_id);
}

/**
 * @brief Timer interrupt handler.
 */

void (timer_int_handler)() {
  timer_counter++;
}

/**
 * @brief Gets the configuration of a timer.
 * 
 * @param timer Timer to be read.
 * @param st Pointer to store the status byte.
 * @return 0 on success, non-zero otherwise.
 */

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  uint8_t rbCommand= (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer)); // 111xxx0
  sys_outb(TIMER_CTRL,rbCommand); // sends readback command to the control register
  // after this sys_outb command, the corresponding timer contains its status on its 6 LSbits

  return util_sys_inb(TIMER_0+timer,st);  // reads the status into the st address
}

/**
 * @brief Displays the configuration of a timer.
 * 
 * @param timer Timer to be displayed.
 * @param st Status byte of the timer.
 * @param field Field of the timer configuration to be displayed.
 * @return 0 on success, non-zero otherwise.
 */


int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  
  union timer_status_field_val config;

  if (field == tsf_all) // if we want the entire configuration in hex we can simply
  {                     // pass the value st to config.byte
    config.byte = st;
  }

  else if (field == tsf_initial){ // in this case we just care to know the initialization mode
    if ((st & TIMER_LSB_MSB) == TIMER_LSB_MSB) config.in_mode = MSB_after_LSB;  // xx11xxxx
    else if ((st & TIMER_MSB) == TIMER_MSB) config.in_mode = MSB_only; // xx10xxxx
    else if ((st & TIMER_LSB) == TIMER_LSB) config.in_mode = LSB_only;  // xx01xxxx
    else config.in_mode = INVAL_val;  // xx00xxxx
  }

  else if (field == tsf_mode){  // in this case we want to know the counting mode
    st = (st >> 1) & 0x07;  // this sets st to binary values of 0000 to 0111 (0-7)
    // so that we can directly map the value of st to the corresponding counting mode

    if(st == 6) config.count_mode = 2;  // since st == 0110(6) == 0010(2) we have to this
    else if(st == 7) config.count_mode = 3; // and this st == 0111(7) == 0011(3)
    else config.count_mode = st;
  }

  else if (field == tsf_base){  // in this case we just want to know if we are counting in binary(0) or bcd(1)
    config.bcd = st;
  }

  else return 1;  
  
  return timer_print_config(timer,field,config);
}

