/*
 * imu.hpp
 *
 *  Created on: 2021��3��3��
 *      Author: 10798
 */

#ifndef SOURCE_IMU_H_
#define SOURCE_IMU_H_

#include "common.h"

extern float imu_accl[3];
extern float imu_gyro[3];

extern float imu_gyro_z;
extern float imu_angle_z;
extern float imu_gyro_drift;

void imu_init(void);//��ʼ��������
void imu_update(void);
void imu_clear(void);



#endif /* SOURCE_IMU_H_ */
