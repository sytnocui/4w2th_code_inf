/*
 * imu.hpp
 *
 *  Created on: 2021��3��3��
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

void imu_init(void);//��ʼ��������
void imu_update(void);
void imu_clear(void);



#endif /* SOURCE_IMU_HPP_ */
