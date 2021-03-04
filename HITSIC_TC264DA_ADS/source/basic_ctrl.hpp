/*
 * basic_ctrl.hpp
 *
 *  Created on: 2021��3��3��
 *      Author: 10798
 */

#ifndef SOURCE_BASIC_CTRL_HPP_
#define SOURCE_BASIC_CTRL_HPP_

#include "everything.hpp"

#define motor_FTM FTM0       //���
#define servo_FTM FTM3      //���

#define forward_chnl        kFTM_Chnl_1       //������ǰ
#define backward_chnl       kFTM_Chnl_0      //�������
#define servo_chnl          kFTM_Chnl_7     //���

#define SERVO_RIGHT     8.9
#define SERVO_LEFT      6.3
#define SERVO_MID       7.49

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



void ctrl_init(void);//���Ƴ�ʼ��
void var_init(void);//��ֵ��ʼ��
void motor_ctrl(void);//�������

void servo_ctrl(void);//������ռ�ձ�
void servo_pid_calculate(void);//���PID����
void motor_pid_calculate(void);//���pid����

void Encoder_Update(void);//���±�������ֵ
void Encoder_Distance_Clear(void);



#endif /* SOURCE_BASIC_CTRL_HPP_ */
