#ifndef POS_H
#define POS_H

#include <inttypes.h>


void pos_straight(int32_t length);
void pos_rotate_rad(double rad);
void pos_rotate_grad(double grad);
int ppos_read(int32_t* pos_left, int32_t* vel_left, int32_t* pos_right, int32_t* vel_right);

#endif
