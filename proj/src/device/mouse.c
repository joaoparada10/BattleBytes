/**
 * @file mouse.c
 * @brief Mouse interrupt handling and packet processing.
 */

#include "mouse.h"
#include "keyboard.h"
#include <lcom/lcf.h>


int mouse_hook_id = 5; /**< @brief Hook ID for mouse interrupts */
uint8_t mouse_status_byte; /**< @brief Status byte from the mouse controller */
uint8_t mouse_data; /**< @brief Data read from the mouse */
bool discard_mouse_data; /**< @brief Flag to discard erroneous mouse data */
uint8_t mouse_count = 0; /**< @brief Counter for mouse packets */
int x_sum = 0; /**< @brief Sum of X movements */
int y_sum = 0; /**< @brief Sum of Y movements */
MouseState currentState = START; /**< @brief Current state of the mouse state machine */

/**
 * @brief Subscribes mouse interrupts.
 * 
 * @param bit_no Pointer to the bit number to be set.
 * @return 0 on success, non-zero otherwise.
 */

int(m_kbc_subscribe_int)(uint8_t *bit_no) {
  *bit_no = BIT(mouse_hook_id);
  sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
  return 0;
}

/**
 * @brief Unsubscribes mouse interrupts.
 * 
 * @return 0 on success, non-zero otherwise.
 */

int(m_kbc_unsubscribe_int)() {
  sys_irqrmpolicy(&mouse_hook_id);
  return 0;
}

/**
 * @brief Checks if the mouse status byte is valid.
 * 
 * @param mouse_status_byte Status byte to be checked.
 * @return True if valid, false otherwise.
 */

bool(is_valid_m_st)(uint8_t mouse_status_byte) {
  if (mouse_status_byte & KBC_ST_PAR_ERR) {
    printf("parity error \n");
    return false;
  }
  if (mouse_status_byte & KBC_ST_TO_ERR) {
    printf("timeout error \n");
    return false;
  }
  if (!(mouse_status_byte & KBC_ST_AUX)) {
    printf("mouse_data is coming from keyboard error \n");
    return false;
  }
  return true;
}

/**
 * @brief Reads a byte from the KBC output buffer for the mouse.
 * 
 * @return 0 on success, non-zero otherwise.
 */

int(m_read_obf_byte)() {
  uint8_t attempts = 10;
  while (attempts > 0) {
    if (util_sys_inb(KBC_ST_REG, &mouse_status_byte) != 0) {
      printf("error reading status \n");
      return -1;
    };
    if (mouse_status_byte & KBC_ST_OBF) {
      if (util_sys_inb(KBC_OUT_BUF, &mouse_data) != 0) {
        printf("error reading kbc output buffer \n");
        return -1;
      };
      if (!is_valid_m_st(mouse_status_byte)) {
        printf("invalid status \n");
        return -1;
      }
      return 0;
    }
    tickdelay(micros_to_ticks(DELAY_US));
    attempts--;
  }
  printf("timed out! \n");
  return 1;
}

/**
 * @brief Mouse interrupt handler.
 */

void(mouse_ih)() {
  if (m_read_obf_byte() != 0) {
    discard_mouse_data = true;
    printf("error reading reading kbc obf byte");
  }
}

/**
 * @brief Handles mouse packets.
 * 
 * @param pp Pointer to the packet structure to be filled.
 * @return 0 on success, non-zero otherwise.
 */

int(mouse_packet_handler)(struct packet *pp) {
  switch (mouse_count) {
    case 0:
      if (!(mouse_data & BIT(3))) {
        printf("KBC is not syncd with the mouse, byte discarded. \n");
        discard_mouse_data = true;
        break;
      }
      else {
        pp->bytes[0] = mouse_data;
        pp->lb = (mouse_data & LEFT_BUTTON);
        pp->rb = (mouse_data & RIGHT_BUTTON);
        pp->mb = (mouse_data & MIDDLE_BUTTON);
        pp->x_ov = (mouse_data & MOUSE_X_OVFL);
        pp->y_ov = (mouse_data & MOUSE_Y_OVFL);
      }
      mouse_count++;
      break;
    case 1:
      pp->bytes[1] = mouse_data;
      if (pp->bytes[0] & MSB_X_DELTA) {
        pp->delta_x = 0xFF00;
        pp->delta_x |= mouse_data;
      }
      else {
        pp->delta_x &= 0x0000;
        pp->delta_x |= mouse_data;
      }
      mouse_count++;
      break;
    case 2:
      pp->bytes[2] = mouse_data;
      if (pp->bytes[0] & MSB_Y_DELTA) {
        pp->delta_y = 0xFF00;
        pp->delta_y |= mouse_data;
      }
      else {
        pp->delta_y &= 0x0000;
        pp->delta_y |= mouse_data;
      }
      mouse_count++;
      break;

    default:
      mouse_count = 0;
      break;
  }

  return 0;
}

/**
 * @brief Issues a command to the mouse.
 * 
 * @param cmd Command to be issued.
 * @return 0 on success, non-zero otherwise.
 */

int(kbc_issue_mouse_cmd)(uint8_t cmd) {
  kbc_issue_cmd(KBC_CMD_REG, WRITE_BYTE_MOUSE_CMD);
  kbc_issue_cmd(KBC_IN_BUF, cmd);
  m_read_obf_byte();
  if (mouse_data == ACK_BYTE)
    return 0;
  else
    return -1;
}

/**
 * @brief Mouse state machine to handle gestures.
 * 
 * @param pp Pointer to the packet structure.
 * @param currentState Pointer to the current state of the mouse.
 * @param x_len Minimum x length for gesture detection.
 * @param tolerance Tolerance for gesture detection.
 */

void(mouse_state_machine)(struct packet *pp, MouseState *currentState, uint8_t x_len, uint8_t tolerance) {
  switch (*currentState) {
    case START:
      if (pp->lb && !(pp->rb || pp->mb)) {
        state_change(DRAWING1);
      }
      break;
    case DRAWING1:
      if (pp->lb && !(pp->rb || pp->mb)) {
        state_maintain(pp);
        if (x_sum < 0) x_sum = 0;
        if (y_sum < 0) y_sum = 0;
        if (((pp->delta_x < 0) && (abs(pp->delta_x) > tolerance)) ||
            ((pp->delta_y < 0) && (abs(pp->delta_y) > tolerance)))
          state_change(START);
      }
      else if (!pp->rb && !(pp->rb || pp->mb)){
        if ((x_sum >= x_len) && (y_sum > x_sum)) {
          state_change(PEAK);
        }
        else state_change(START);
      }
      break;
    case PEAK:
      if (pp->lb || pp->mb || 
          (abs(pp->delta_x) > tolerance) ||
          (abs(pp->delta_y) > tolerance)) 
          state_change(START);
      else if (pp->rb) state_change(DRAWING2);      
      break;
    case DRAWING2:
      if (pp->rb && !(pp->lb || pp->mb)) {
        state_maintain(pp);
        if (x_sum < 0) x_sum = 0;
        if (y_sum > 0) y_sum = 0;
        if (((pp->delta_x < 0) && (abs(pp->delta_x) > tolerance)) ||
            ((pp->delta_y > 0) && (abs(pp->delta_y) > tolerance)))
          state_change(START);
      }
      else if (!pp->rb && !(pp->rb || pp->mb)){
        if ((x_sum >= x_len) && (-y_sum > x_sum)) {
          state_change(COMPLETE);
        }
        else state_change(START);
      }
      break;
    default:
      break;
  }
}

/**
 * @brief Changes the state of the mouse state machine.
 * 
 * @param newState New state to be set.
 */

void(state_change)(MouseState newState) {
  x_sum = 0;
  y_sum = 0;
  currentState = newState;
}

/**
 * @brief Maintains the current state of the mouse state machine.
 * 
 * @param pp Pointer to the packet structure.
 */

void(state_maintain)(struct packet *pp) {
  x_sum += pp->delta_x;
  y_sum += pp->delta_y;
}
