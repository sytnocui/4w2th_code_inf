/*
 * basic_ctrl.hpp
 *
 *  Created on: 2021年3月3日
 *      Author: 10798
 */

#ifndef SOURCE_BASIC_CTRL_H_
#define SOURCE_BASIC_CTRL_H_

#include "common.h"
#include "SmartCar_Pwm.h"
#include "SmartCar_Encoder.h"
#include "SmartCar_MPU.h"
#include "advanced_ctrl.h"
#include "image.h"

#include "Cpu0_Main.h"

#define SERVO_RIGHT     890
#define SERVO_LEFT      700
#define SERVO_MID       795

/*电机PID参数*/
extern float motor_kp ;    //速度环p
extern float motor_ki ;    //速度环i
extern float motor_kd ;    //速度环d
extern float motor_err;
extern float motor_err_pre;
extern float motor_output;

/*舵机PID参数*/
extern float servo_kp ;    //内环p
extern float servo_ki;     //内环i
extern float servo_kd ;    //内环d
extern float servo_err ;
extern float servo_output ;
extern float servo_pid_output;

extern float speed_actual;
extern float speed_dream;
extern float speed_dream_str;
extern float speed_dream_turn;

extern int enco_get;
extern float enco_distance;

extern int img_var;

extern int stop_line;
extern int startline_time;

extern mpu_t my_mpu;
extern mpu_t* this_mpu;
extern float imu_dt;
extern float imu_angle_z;
extern float imu_angle_drift;
extern float imu_accl[3];
extern float imu_gyro[3];
extern float imu_gyro_z;

void ctrl_init(void);//控制初始化
void var_init(void);//数值初始化
void motor_ctrl(void);//电机控制

void servo_ctrl(void);//舵机输出占空比
void servo_get_err(void);
void servo_pid_calculate(void);//舵机PID计算
void motor_pid_calculate(void);//电机pid计算

void Encoder_Update(void);//更新编码器的值
void Encoder_Distance_Clear(void);

void imu_clear(void);
void imu_update(void);



#endif /* SOURCE_BASIC_CTRL_H_ */
