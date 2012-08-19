#include "socketcan/socketcan.h"
#include "canopen/canopen.h"
#include "motor/motor.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdbool.h>


int main(void) {
	motor_init();
	motor_enable();
	sleep(1);

	printf("Run 50 [mm/sec]\n");
	vel_set_speed_right(50);
	vel_set_speed_left(50);
	sleep(5);

	printf("Halt!\n");
	motor_halt();
	sleep(5);

	printf("Run, but not enabled!\n");
	vel_set_speed(50, 0);
	sleep(5);

	printf("Run again, and rotate!\n");
	motor_enable();
	vel_set_speed(0, (2.0*3.14/10.0)*1000); // Speed [mm/sec], Rotation [mRad/sec]
	sleep(5);

	printf("Zero speed\n");
	vel_set_speed(0, 0);
	sleep(5);

	/*
	printf("Possition mode!\n");
	motor_setmode(Motor_mode_Position);
	pos_rotate_grad(180);
	*/

	motor_disable();
	motor_close();
	return 0;
}

