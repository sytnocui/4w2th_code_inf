/*
 * basic_ctrl.hpp
 *
 *  Created on: 2021年3月3日
 *      Author: 10798
 */

#ifndef SOURCE_BASIC_CTRL_HPP_
#define SOURCE_BASIC_CTRL_HPP_

#include "everything.hpp"

#define motor_FTM FTM0       //电机
#define servo_FTM FTM3      //舵机

#define forward_chnl        kFTM_Chnl_1       //后轮向前
#define backward_chnl       kFTM_Chnl_0      //后轮向后
#define servo_chnl          kFTM_Chnl_7     //舵机

#define SERVO_RIGHT     8.9
#define SERVO_LEFT      6.3
#define SERVO_MID       7.49

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



void ctrl_init(void);//控制初始化
void var_init(void);//数值初始化
void motor_ctrl(void);//电机控制

void servo_ctrl(void);//舵机输出占空比
void servo_pid_calculate(void);//舵机PID计算
void motor_pid_calculate(void);//电机pid计算

void Encoder_Update(void);//更新编码器的值
void Encoder_Distance_Clear(void);



#endif /* SOURCE_BASIC_CTRL_HPP_ */
