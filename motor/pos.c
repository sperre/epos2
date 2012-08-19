#include "pos.h"
#include "motor.h"
#include "epos.h"

#include "canopen/PDO.h"

#include <inttypes.h>
#include <math.h>


void _pos_goto(int32_t pos_l, int32_t pos_r) {
	Socketcan_t target_left[2] = {
		{4, -pos_l*MOTOR_STEP_PER_MM}, // Motor is mounted "backwards"
		{2, Switch_On_And_Enable_Operation}
	};
	
	Socketcan_t target_right[2] = {
		{4, pos_r*MOTOR_STEP_PER_MM},
		{2, Switch_On_And_Enable_Operation}
	};

	PDO_send(motor_pdo_fd, PDO_TX1_ID+MOTOR_EPOS_L_ID, 2, target_left);
	PDO_send(motor_pdo_fd, PDO_TX1_ID+MOTOR_EPOS_R_ID, 2, target_right);
}

void pos_straight(int32_t length) {
	_pos_goto(length, length);
}

void pos_rotate_rad(double rad) {
	int32_t length = rad*MOTOR_WHEEL_RADIUS;
	_pos_goto(-length, length); // left goes backward in posetiv direction
}

void pos_rotate_grad(double grad) {
	pos_rotate_rad(2*M_PI*grad/360.0);
}


int ppos_read(int32_t* pos_left, int32_t* vel_left, int32_t* pos_right, int32_t* vel_right) {
	const int timeout = 1000;
	my_can_frame f;

	int ret = PDO_read(motor_pdo_fd, &f, timeout);
	switch(f.id) {
		case(PDO_RX2_ID + MOTOR_EPOS_R_ID):
			*pos_right = ((uint32_t)f.data[0]<<0) | ((uint32_t)f.data[1]<<8) | ((uint32_t)f.data[2]<<16) | ((uint32_t)f.data[3]<<24);
			*vel_right = ((uint32_t)f.data[4]<<0) | ((uint32_t)f.data[5]<<8) | ((uint32_t)f.data[6]<<16) | ((uint32_t)f.data[7]<<24);
			break;
		case(PDO_RX2_ID + MOTOR_EPOS_L_ID):
			*pos_left = -( ((uint32_t)f.data[0]<<0) | ((uint32_t)f.data[1]<<8) | ((uint32_t)f.data[2]<<16) | ((uint32_t)f.data[3]<<24) );
			*vel_left = -( ((uint32_t)f.data[4]<<0) | ((uint32_t)f.data[5]<<8) | ((uint32_t)f.data[6]<<16) | ((uint32_t)f.data[7]<<24) );
			break;
	}
	return ret;
}
 
