///*
// * imu.cpp
// *
// * 不滤波直接读数积分后使用
// *
// *  Created on: 2021年2月14日
// *      Author: 10798
// */
//#include "imu.hpp"
//
//mpu_t mpu6050;
//
///**
// * @brief   IMU传感器原始数据
// * @ {
// */
//float imu_accl[3] = {0.0f, 0.0f, 0.0f};
//float imu_gyro[3] = {0.0f, 0.0f, 0.0f};
///**
// * @ }
// */
float imu_gyro_z = 0;
float imu_angle_z = 0;
float imu_gyro_drift = 0;
//
//void imu_init(void)
//{
//    /** 初始化IMU */
//    if (0U != SmartCar_MPU_Init2(mpu6050))
//    {
//        while(1);
//    }
//    SmartCar_MPU_SelfCheck(); ///> 自检时保持静止，否则会直接失败
//}
//
//void imu_update(void)
//{
//    SmartCar_MPU_Getgyro2(mpu6050);
//    imu_gyro_z = imu_gyro[2]-imu_gyro_drift;
//    imu_angle_z += imu_gyro_z * 0.005;
//}
//
//void imu_clear(void)
//{
//    imu_angle_z = 0;
//}
//
