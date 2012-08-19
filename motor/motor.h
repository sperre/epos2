#ifndef LIBMOTOR_H
#define LIBMOTOR_H

#include "pos.h"
#include "vel.h"
#include <inttypes.h>


/*** CAN-bus spesific ***/
#define MOTOR_PC_ID     0x01
#define MOTOR_EPOS_L_ID 0x02
#define MOTOR_EPOS_R_ID 0x03


/*** Robot specific ***/
// One motor step converted to traveled distance [mm] (meassured)
#define MOTOR_STEP_PER_MM        4
#define MOTOR_WHEEL_RADIUS      32      /* [mm] */
#define MOTOR_WHEELBASE_RADIUS 135      /* [mm] */
#define MOTOR_GEAR_RATIO (18.0/1.0)


/*** Tuning ***/
#define MOTOR_MAX_SPEED     1200      /* [mm/sec]    */
#define MOTOR_MAX_ACC        650      /* [mm/sec^2]  */
#define MOTOR_MAX_QUICK_ACC  350      /* [mm/sec^2]  */


/*** Errors ***/
#define MOTOR_ERROR   (-1)
#define MOTOR_TIMEOUT (-2)


// Implemented subset of Epos_mode from epos.h
enum Motor_mode {
	Motor_mode_Velocity = 3,
	Motor_mode_Position = 1,
};


extern int motor_pdo_fd;
extern int motor_cfg_fd;

/*!
 * Opens the can connection,
 * and configures the two motors
 * \return 0 on success, MOTOR_ERROR (-1) on error
 */
int motor_init(void);


/*! Closes the CAN connection */
void motor_close(void);


/*!
 * Turns the motors on
 * \return 0 on success, MOTOR_ERROR (-1) on error
 */
int motor_enable(void);


/*!
 * Stops the motors as quickly as possible, and holds them there.
 * Run motor_enable() to re-enable the controller.
 */
int motor_halt(void);


/*!
 * Turns the motors off (no power).
 * \return 0 on success, MOTOR_ERROR (-1) on error
 */
int motor_disable(void);

/*!
 * Sets the opperation mode (Velocity or Position)
 * \return 0 on success, MOTOR_ERROR (-1) on error
 */
int motor_setmode(enum Motor_mode mode);


/*
 * For internal use in the driver.
 * Converts [mm/sec] to [rpm] and [mm/sec^2] to [rpm/sec]
 */
int motor_mmsec_to_rpm(int mm_per_sec);
int motor_rpm_to_mmsec(int rpm);
int motor_enc_to_mm(int enc);

#endif // LIBMOTOR_H
