#include "motor.h"
#include "epos.h"

#include "canopen/canopen.h"

/*
uint8_t epos_Error_Register(uint16_t node_id) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x1001;
	d.subindex = 0x00;
	d.data.size = SDO_type_uint8;
	SDO_read(motor_cfg_fd, &d);
	return *(d.data.uint8_p);
}
*/


int epos_Receive_PDO_n_Parameter(uint16_t node_id, uint8_t n, uint32_t cob) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x1400 + n-1;
	d.subindex = 0x01;
	d.data.size = 4;
	d.data.data = cob;
	return SDO_write(motor_cfg_fd, &d);
}


int epos_Receive_PDO_n_Mapping(uint16_t node_id,  uint8_t n, uint8_t num_objects, Epos_pdo_mapping* objects) {
	int err = 0;

	// Set number of objects to zero
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x1600 + n-1;
	d.subindex = 0x00;
	d.data.size = 1;
	d.data.data = 0;

	err = SDO_write(motor_cfg_fd, &d);
	if (err != 0) {
		return err;
	}

	// Write objects
	d.data.size = 4;
	for(int i=0; i<num_objects; i++) {
		Epos_pdo_mapping obj = objects[i];

		d.subindex = i+1;
		d.data.data = ((uint32_t)obj.index << 16) | (obj.subindex<<8) | (obj.length);
		err = SDO_write(motor_cfg_fd, &d);
		if (err != 0) {
			return err;
		}
	}

	// Set Correct number of objects
	d.subindex = 0x00;
	d.data.size = 1;
	d.data.data = num_objects;
	return SDO_write(motor_cfg_fd, &d);
}


int epos_Transmit_PDO_n_Parameter(uint16_t node_id, uint8_t n, uint32_t cob) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x1800 + n-1;
	d.subindex = 0x01;
	d.data.size = 4;
	d.data.data = cob;
	return SDO_write(motor_cfg_fd, &d);
}


int epos_Transmit_PDO_n_Mapping(uint16_t node_id, uint8_t n, uint8_t num_objects, Epos_pdo_mapping* objects) {
	int err = 0;

	// Set number of objects to zero
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x1A00 + n-1;
	d.subindex = 0x00;
	d.data.size = 1;
	d.data.data = 0;

	err = SDO_write(motor_cfg_fd, &d);
	if (err != 0) {
		return err;
	}

	// Write objects
	d.data.size = 4;
	for(int i=0; i<num_objects; i++) {
		Epos_pdo_mapping obj = objects[i];

		d.subindex = i+1;
		d.data.data = ((uint32_t)obj.index << 16) | (obj.subindex<<8) | (obj.length);
		err = SDO_write(motor_cfg_fd, &d);
		if (err != 0) {
			return err;
		}
	}

	// Set Correct number of objects
	d.subindex = 0x00;
	d.data.size = 1;
	d.data.data = num_objects;

	return SDO_write(motor_cfg_fd, &d);
}


int epos_Miscellaneous_Configuration(uint16_t node_id, uint16_t value) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x2008;
	d.subindex = 0x00;
	d.data.size = 2;
	d.data.data = value;

	return SDO_write(motor_cfg_fd, &d);
}


int epos_Controlword(uint16_t node_id, enum Epos_ctrl ctrl) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x6040;
	d.subindex = 0x00;
	d.data.size = 2;
	d.data.data = ctrl;

	return SDO_write(motor_cfg_fd, &d);
}


int epos_Position_Mode_Setting_Value(uint16_t node_id, int32_t value) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x2062;
	d.subindex = 0x00;
	d.data.size= 4;
	d.data.data = value;

	return SDO_write(motor_cfg_fd, &d);
}


int epos_Modes_of_Operation(uint16_t node_id, enum Epos_mode mode) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x6060;
	d.subindex = 0x00;
	d.data.size = 1;
	d.data.data = mode;

	return SDO_write(motor_cfg_fd, &d);
}


int epos_Position_Window(uint16_t node_id, uint32_t enc) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x6067;
	d.subindex = 0x00;
	d.data.size = 4;
	d.data.data = enc;

	return SDO_write(motor_cfg_fd, &d);
}


int epos_Position_Window_Time(uint16_t node_id, uint32_t ms) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x6068;
	d.subindex = 0x00;
	d.data.size = 4;
	d.data.data = ms;

	return SDO_write(motor_cfg_fd, &d);
}


int epos_Target_Position(uint16_t node_id, int32_t enc) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x607A;
	d.subindex = 0x00;
	d.data.size = 4;
	d.data.data = enc;

	return SDO_write(motor_cfg_fd, &d);
}


int epos_Software_Position_Limit(uint16_t node_id, int32_t min, int32_t max) {
	int err = 0;

	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x607D;
	d.subindex = 0x01;
	d.data.size = 4;
	d.data.data = min;

	err = SDO_write(motor_cfg_fd, &d);
	if (err != 0) {
		return err;
	}

	d.subindex = 0x02;
	d.data.data = max;

	return SDO_write(motor_cfg_fd, &d);
}


int epos_Maximal_Profile_Velocity(uint16_t node_id, uint32_t value) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x607F;
	d.subindex = 0x00;
	d.data.size = 4;
	d.data.data = value;

	return SDO_write(motor_cfg_fd, &d);
}


int epos_Profile_Velocity(uint16_t node_id, uint32_t value) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x6081;
	d.subindex = 0x00;
	d.data.size = 4;
	d.data.data = value;

	return SDO_write(motor_cfg_fd, &d);
}


int epos_Profile_Acceleration(uint16_t node_id, uint32_t value) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x6083;
	d.subindex = 0x00;
	d.data.size = 4;
	d.data.data = value;

	return SDO_write(motor_cfg_fd, &d);
}


int epos_Profile_Deceleration(uint16_t node_id, uint32_t value) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x6084;
	d.subindex = 0x00;
	d.data.size = 4;
	d.data.data = value;

	return SDO_write(motor_cfg_fd, &d);
}


int epos_Quickstop_Deceleration(uint16_t node_id, uint32_t value) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x6085;
	d.subindex = 0x00;
	d.data.size = 4;
	d.data.data = value;

	return SDO_write(motor_cfg_fd, &d);
}


int epos_Motion_Profile_Type(uint16_t node_id, enum Epos_Profile_Type mode) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x6086;
	d.subindex = 0x00;
	d.data.size = 2;
	d.data.data = mode;

	return SDO_write(motor_cfg_fd, &d);
}


int epos_Max_Acceleration(uint16_t node_id, uint32_t value) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x60C5;
	d.subindex = 0x00;
	d.data.size = 4;
	d.data.data = value;

	return SDO_write(motor_cfg_fd, &d);
}
