/*
 * imu.hpp
 *
 *  Created on: 2021年3月3日
 *      Author: 10798
 */

#ifndef SOURCE_IMU_HPP_
#define SOURCE_IMU_HPP_

#include "everything.hpp"

extern float imu_accl[3];
extern float imu_gyro[3];

extern float imu_gyro_z;
extern float imu_angle_z;
extern float imu_gyro_drift;

void imu_init(void);//初始化陀螺仪
void imu_update(void);
void imu_clear(void);



#endif /* SOURCE_IMU_HPP_ */
