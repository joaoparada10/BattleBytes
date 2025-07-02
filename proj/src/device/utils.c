/**
 * @file utils.c
 * @brief Utility functions for the project.
 */


#include <lcom/lcf.h>
#include <stdint.h>

/**
 * @brief Extracts the least significant byte (LSB) from a 16-bit value.
 * 
 * @param val 16-bit value to be processed.
 * @param lsb Pointer to store the LSB.
 * @return 0 on success, non-zero otherwise.
 */

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL) return 1;
  *lsb = (uint8_t) val;
  return 0; 
}

/**
 * @brief Extracts the most significant byte (MSB) from a 16-bit value.
 * 
 * @param val 16-bit value to be processed.
 * @param msb Pointer to store the MSB.
 * @return 0 on success, non-zero otherwise.
 */

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL) return 1;
  *msb = (uint8_t) (val >> 8);
  return 0;
}

/**
 * @brief Reads a byte from a specified port.
 * 
 * @param port Port to be read.
 * @param value Pointer to store the read value.
 * @return 0 on success, non-zero otherwise.
 */

int (util_sys_inb)(int port, uint8_t *value) {
  if(value == NULL) return 1;
  uint32_t v2;
  int ret = sys_inb(port,&v2);
  *value = (uint8_t) v2;
  return ret;
}
