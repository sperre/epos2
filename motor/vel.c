#include "vel.h"
#include "motor.h"
#include "epos.h"

#include "socketcan/printd.h"
#include "canopen/canopen.h"



// Non-exported function
static int _vel_speed(int32_t vel, uint16_t nodeid) {
	int err = 0;
	const int32_t rpm = motor_mmsec_to_rpm(vel);
	Socketcan_t target_vel[2] = {
		{4, rpm},
		{2, Switch_On_And_Enable_Operation}
	};

	err = PDO_send(motor_pdo_fd, PDO_RX2_ID + nodeid, 2, target_vel);
	return err;
}


int vel_set_speed_left(int32_t vel) {
	return _vel_speed(-vel, MOTOR_EPOS_L_ID);
}


int vel_set_speed_right(int32_t vel) {
	return _vel_speed(vel, MOTOR_EPOS_R_ID);
}

int vel_set_speed(int32_t movement, int32_t rotation) {
	int err = 0;
	double w = rotation/1000.0; // Remove milli
	int32_t v_r = movement + w*MOTOR_WHEELBASE_RADIUS;
	int32_t v_l = movement - w*MOTOR_WHEELBASE_RADIUS;
	err |= vel_set_speed_right(v_r);
	err |= vel_set_speed_left(v_l);
	return err;
}


int vel_read(int32_t* pos_left, int32_t* vel_left, int32_t* pos_right,
             int32_t* vel_right, int timeout) {

	int err;
	int status = 0;

	my_can_frame f;
	err = PDO_read(motor_pdo_fd, &f, timeout);

	if(err != 0) {
		return err;
	}

	uint32_t enc, rpm;
	switch(f.id) {
		case(PDO_TX1_ID + MOTOR_EPOS_R_ID):
			status = (f.data[0]<<0) | (f.data[1]<<8);
			break;
		case(PDO_TX1_ID + MOTOR_EPOS_L_ID):
			status = (f.data[0]<<0) | (f.data[1]<<8);
			break;
		case(PDO_TX2_ID + MOTOR_EPOS_R_ID):
			enc = ((uint32_t)f.data[0]<<0) | ((uint32_t)f.data[1]<<8) | ((uint32_t)f.data[2]<<16) | ((uint32_t)f.data[3]<<24);
			rpm = ((uint32_t)f.data[4]<<0) | ((uint32_t)f.data[5]<<8) | ((uint32_t)f.data[6]<<16) | ((uint32_t)f.data[7]<<24);
			*pos_right = motor_enc_to_mm(enc);
			*vel_right = motor_rpm_to_mmsec(rpm);
			break;
		case(PDO_TX2_ID + MOTOR_EPOS_L_ID):
			enc = ((uint32_t)f.data[0]<<0) | ((uint32_t)f.data[1]<<8) | ((uint32_t)f.data[2]<<16) | ((uint32_t)f.data[3]<<24);
			rpm = ((uint32_t)f.data[4]<<0) | ((uint32_t)f.data[5]<<8) | ((uint32_t)f.data[6]<<16) | ((uint32_t)f.data[7]<<24);
			*pos_left = motor_enc_to_mm(-enc);
			*vel_left = motor_rpm_to_mmsec(-rpm);
			break;
		default:
			printd(LOG_WARN, "motor/vel.c recived unkown PDO pkg 0x%x\n", f.id);
			break;
	}

	if(status & 0x08) {
		// The epos reported an error
		printd(LOG_ERROR, "The epos %d reported an error! status=0x%x\n", f.id-PDO_TX1_ID, status);
		return -11;  // TODO: uniq error codes
	} else if(status & 0x80) {
		// The epos reported an warning
		printd(LOG_ERROR, "The epos %d reported an warning! status=0x%x\n", f.id-PDO_TX1_ID, status);
		return -12;
	}

	return 0;
}
