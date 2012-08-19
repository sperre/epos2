#ifndef SDO_H
#define SDO_H

#include "socketcan/socketcan.h"
#include <inttypes.h>

// Used internally in SDO.c
#define SDO_RESPONSE_ERROR     0x80
#define SDO_RESPONSE_WRITE_OK  0x60
#define SDO_RESPONSE_READ_OK   0x40


enum SDO_ID {
	SDO_TX = 0x580, /* +node id */
	SDO_RX = 0x600  /* +node id */
};

/** \struct SDO_data Structure holding all data needed to send an SDO object */
typedef struct {
	uint16_t nodeid;  //!< Node id to send data to
	uint16_t index;   //!< Index in Object dictionary
	uint8_t subindex; //!< Subindex in Object dictionary
	Socketcan_t data; //!< Data and type info
} SDO_data;

/**
 * Sends an SDO_data stuct on the can-bus.
 * \return  0 on success, -1 on error, -2 on timeout
 **/
int SDO_write(int fd, const SDO_data* d);


/**
 * Sends an SDO acknowledgement package in return to frame f
 * \return 0 on success
 */
int SDO_acknowledge(int fd, const my_can_frame* f);
#endif
