#ifndef VEL_H
#define VEL_H
#include <inttypes.h>

/** \file vel.c Profile velocity mode */

/**
 * \param vel [mm/sec]
 * \return success/error
 */
int vel_set_speed_left(int32_t vel);


/**
 * \param vel [mm/sec]
 * \return success/error
 */
int vel_set_speed_right(int32_t vel);

/**
 * Alternative to vel_set_speed_right/left
 * \param v_s Robot movement [mm/sec]
 * \param v_d Robot rotation [milli-rad/sec]
 * \return success/error
 */
int vel_set_speed(int32_t movement, int32_t rotation);

int vel_halt(void);


/**
 * Tries to read feedback and blocks untill data arrives.
 * \param timeout as in poll, typical in millisec, 0 is no timeout and -1
 * \param pos_* [mm]
 * \param vel_* [mm/sec]
 * \return 0 on success, -1 (MOTOR_ERROR) or -2 (MOTOR_TIMEOUT) on failure
 */
int vel_read(int32_t* pos_left, int32_t* vel_left, int32_t* pos_right,
             int32_t* vel_right, int timeout);

#endif
