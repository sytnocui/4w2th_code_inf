/*
 * basic_ctrl.hpp
 *
 *  Created on: 2021��3��3��
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

/*���PID����*/
extern float motor_kp ;    //�ٶȻ�p
extern float motor_ki ;    //�ٶȻ�i
extern float motor_kd ;    //�ٶȻ�d
extern float motor_err;
extern float motor_err_pre;
extern float motor_output;

/*���PID����*/
extern float servo_kp ;    //�ڻ�p
extern float servo_ki;     //�ڻ�i
extern float servo_kd ;    //�ڻ�d
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

void ctrl_init(void);//���Ƴ�ʼ��
void var_init(void);//��ֵ��ʼ��
void motor_ctrl(void);//�������

void servo_ctrl(void);//������ռ�ձ�
void servo_get_err(void);
void servo_pid_calculate(void);//���PID����
void motor_pid_calculate(void);//���pid����

void Encoder_Update(void);//���±�������ֵ
void Encoder_Distance_Clear(void);

void imu_clear(void);
void imu_update(void);



#endif /* SOURCE_BASIC_CTRL_H_ */
