/*
 * imu.cpp
 *
 * 不滤波直接读数积分后使用
 *
 *  Created on: 2021年2月14日
 *      Author: 10798
 */
#include "imu.hpp"



/**
 * @brief   IMU传感器原始数据
 * @ {
 */
float imu_accl[3] = {0.0f, 0.0f, 0.0f};
float imu_gyro[3] = {0.0f, 0.0f, 0.0f};
/**
 * @ }
 */
float imu_gyro_z = 0;
float imu_angle_z = 0;
float imu_gyro_drift = 0;

void imu_init(void)
{
    /** 初始化IMU */
       if (true != imu_6050.Detect())
       {
           PRINTF("IMU Detection Fail\n");
           while(1);
       }
       if (0U != imu_6050.Init())
       {
           PRINTF("IMU Initialization Fail\n");
           while(1);
       }
       if (0U != imu_6050.SelfTest()) ///> 自检时保持静止，否则会直接失败
       {
           PRINTF("IMU Self Test Fail\n");
           while(1);
       }
}

void imu_update(void)
{
    imu_6050.ReadSensorBlocking();///<读参
    imu_6050.Convert(imu_accl, imu_accl+1, imu_accl+2, imu_gyro, imu_gyro+1, imu_gyro+2);
    imu_gyro_z = imu_gyro[2]-imu_gyro_drift;
    imu_angle_z += imu_gyro_z * 0.005;
}

void imu_clear(void)
{
    imu_angle_z = 0;
}

