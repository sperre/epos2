#include "socketcan.h"
#include "printd.h"

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include <linux/can.h>
#include <linux/can/raw.h>


/*
#ifndef AF_CAN
#define AF_CAN PF_CAN
#endif
*/


int socketcan_open(uint16_t filter[], uint16_t filtermask[], uint16_t num_filters) {
	int fd =  -1;
	// Create the socket
	fd = socket( PF_CAN, SOCK_RAW, CAN_RAW );
	if(fd  == -1) {
		printd(LOG_ERROR, "socketcan: Error opening socket\n");
		return fd;
	}

	// Locate the interface you wish to use
	struct ifreq ifr;
	strcpy(ifr.ifr_name, "can0");
	ioctl(fd, SIOCGIFINDEX, &ifr); // ifr.ifr_ifindex gets filled with that device's index

	// Set Filter for this conection
	struct can_filter rfilter[num_filters];
	for(int i=0; i<num_filters; i++) {
		rfilter[i].can_id   = filter[i];
		rfilter[i].can_mask = filtermask[i];
	}
	setsockopt(fd, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));
	
	// Select that CAN interface, and bind the socket to it.
	struct sockaddr_can addr;
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	bind(fd, (struct sockaddr*)&addr, sizeof(addr));

	// Set to non blocking
	fcntl(fd, F_SETFL, O_NONBLOCK);

	return fd;
}


void socketcan_close(int fd) {
	close(fd);
}


int socketcan_read(int fd, my_can_frame* frame, int timeout) {
	// Wait for data or timeout
	int bytes, t;
	struct can_frame f;
	struct pollfd p[1];

	p[0].fd = fd;
	p[0].events = POLLIN;

	t = poll(p, 1, timeout);

	// Try to read available data
	bytes = read(fd, &f, sizeof(f));

	if(bytes <= 0) {
		// Error, no bytes read
		frame->id = 0;
		frame->dlc = 0;
		if (bytes == 0) {
			return SOCKETCAN_TIMEOUT;
		}
		//printd(LOG_ERROR, "socketcan: Could not read data from CAN-bus\n");
		return SOCKETCAN_ERROR;
	}

	// Copy data
	frame->id = f.can_id;
	frame->dlc = f.can_dlc;
	memcpy(frame->data, f.data, sizeof(frame->data));
	return 0;
}


int socketcan_write(int fd, uint16_t id, uint8_t length, Socketcan_t data[]) {
	int bytes;
	uint8_t byte, n;
	struct can_frame frame;
	frame.can_id = id;

	byte = 0;
	for(int i=0; i<length; i++) {
		n = 0;
		while(n < data[i].size) {
			frame.data[byte] = (data[i].data >> 8*n);
			n++;
			byte++;
		}
	}

	frame.can_dlc = byte;

	bytes = write(fd, &frame, sizeof(frame));
	if(bytes < 0) {
		// Error, no data written
		printd(LOG_ERROR, "socketcan: Could not write data to CAN-bus\n");
		return SOCKETCAN_ERROR;
	}
	return 0;
}
