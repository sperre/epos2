#ifndef SOCKETCAN_H
#define SOCKETCAN_H

#include <inttypes.h>


#define SOCKETCAN_ERROR   -1
#define SOCKETCAN_TIMEOUT -2

typedef struct {
	uint32_t size; //!< in bytes (1=unit8_t, 2=uint16_t, 3=unit24_t, 4=uint32_t)
	uint32_t data;
} Socketcan_t;

typedef struct {
	uint16_t id;
	uint8_t dlc;
	uint8_t data[8];
} my_can_frame;


/**
 * Opens the connection to the CAN-bus.
 *
 * \param filter        array of accepted CANid's
 * \param filtermask    bitwise and-ed with the corresponding filter in the
 *                      'filter' array
 * \ param num_filters  length of 'filter' and 'filtermask' arrays
 * \return              file pointer on success, -1 on error
 *
 */
int socketcan_open(uint16_t filter[], uint16_t filtermask[], uint16_t num_filters);


/** Closes the connection to the CAN-bus */
void socketcan_close(int fp);


/**
 * Checks if there is data to be read from the CAN-bus
 * \param frame    where to save the data
 * \param timeout  milliseconds, -1 equals no timeout
 * \return              0 on success, -1 on error, -2 on timeout
 */
int socketcan_read(int fp, my_can_frame* frame, int timeout);


/**
 * Writes the data to the CAN-bus
 * \return              0 on success, -1 on error
 */
int socketcan_write(int fp, uint16_t id, uint8_t length, Socketcan_t data[]);

#endif
