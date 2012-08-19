#include "PDO.h"
#include "SDO.h"
#include "socketcan/printd.h"
#include <string.h>


int PDO_send(int fp, uint16_t pdo_id, uint8_t length, Socketcan_t data[]) {
	//uint16_t cob = 0x200 + id;
	return socketcan_write(fp, pdo_id, length, data);
}


int PDO_read(int fp, my_can_frame* pdo, int timeout) {
	// Don't touch callers struct unless we have to
	int err, func;
	my_can_frame f;

	err = socketcan_read(fp, &f, timeout);
	if(err != 0) {
		// Read error, or no data
		return err;
	}

	func = f.id & (~0x7f); // COB function code
	if(func == 0 || func == SDO_RX || func == SDO_TX) {
		// Not a PDO!
		printd(LOG_WARN, "PDO.c recived non pdo-pkg\n");
		return SOCKETCAN_ERROR;
	}

	// Copy data to return struct
	memcpy(pdo, &f, sizeof(f));
	return 0;
}
