/**
 * @file proj.c
 * @brief This file contains the implementation of the proj module.
 */

#include <lcom/lcf.h>
#include "device/mouse.h"
#include "device/keyboard.h"
#include "dispatcher/dispatcher.h"
#include "menu/menu.h"
#include "graphics/sprite.h"
#include <lcom/timer.h>

extern uint8_t keyboard_data,mouse_data,kbd_status_byte,mouse_status_byte,mouse_count;
struct packet mouse_packet;
bool discard_mouse_data,discard_keyboard_data,skip_print,game_running = true;
extern int timer_counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  //lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  //lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

/**
 * @brief Main game loop for the project.
 * 
 * This function initializes the game state and enters a loop that continues until the game state is KILL.
 * Within the loop, it receives messages and handles them based on their type and source.
 * It handles hardware interrupts for the mouse, keyboard, and timer, processing the respective data and updating the game state.
 * After the loop, it performs cleanup by unsubscribing from the interrupts and disabling the mouse data report.
 * 
 * @param argc The number of strings pointed to by argv
 * @param argv A pointer to an array of arguments
 * @return int Returns 0 upon successful execution
 */
int (proj_main_loop)(int argc, char **argv) {
  vg_init(0x115);
  int ipc_status;
  int r;
  uint8_t irq_set_mouse,irq_set_kbd, irq_set_timer;
  message msg;
  uint8_t bytes[2];
  uint8_t size = 1;
  bool is_make;

  kbc_issue_mouse_cmd(ENABLE_DATA_REPORT);
  m_kbc_subscribe_int(&irq_set_mouse);
  kbc_subscribe_int(&irq_set_kbd);
  timer_subscribe_int(&irq_set_timer);

  State state = init_game_state();

  while (state != KILL) {   //while(game_running)
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {                     /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {               
        case HARDWARE:                                   /* hardware interrupt notification */
          if (msg.m_notify.interrupts & irq_set_mouse) { /* subscribed interrupt */
            mouse_ih();
            if (discard_mouse_data == false) {
              mouse_packet_handler(&mouse_packet);
              
              if (mouse_count == 3) {
                mouse_state_handler(&mouse_packet);
                //mouse_print_packet(&mouse_packet);
                mouse_count = 0;
              }
            }
            discard_mouse_data = false;
          }
          if (msg.m_notify.interrupts & irq_set_timer) { /* subscribed interrupt */
            timer_int_handler(); // timer_counter++
            
            // timer 0 generates interrupts 60 per second by default
            //            30 fps
            if(timer_counter%2==0){
              game_state_handler();
            }
          }  
          if (msg.m_notify.interrupts & irq_set_kbd) { /* subscribed interrupt */
            kbc_ih();
            if (discard_keyboard_data == false){
              process_scancode(&is_make,&size,bytes);
              if (!skip_print){
                kbd_state_handler(is_make,size,bytes);
                //kbd_print_scancode(is_make,size,bytes);
                size = 1;
              }
            }
            discard_keyboard_data = false;
            timer_counter = 0;
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */
      }
    }
    else { /* received a standard message, not a notification */
           /* no standard messages expected: do nothing */
    }
    state = get_state();
  }
  // cleanup
  vg_exit();
  timer_unsubscribe_int();
  m_kbc_unsubscribe_int(&irq_set_mouse);
  kbc_unsubscribe_int(&irq_set_kbd);
  kbc_issue_mouse_cmd(DISABLE_DATA_REPORT);

return 0;
}
