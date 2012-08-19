#include "NMT.h"
#include "canopen.h"
#include "socketcan/socketcan.h"


int NMT_change_state(int fd, uint8_t nodeid, enum NMT_transisions state) {

	Socketcan_t data[2];
	data[0].size = 1;
	data[0].data = state;
	data[1].size = 1;
	data[1].data = nodeid;

	return socketcan_write(fd, 0, 2, data);
	
}
