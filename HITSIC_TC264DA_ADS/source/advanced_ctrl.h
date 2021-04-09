/*
 * advanced.hpp
 *
 *  Created on: 2021年3月3日
 *      Author: 10798
 */

#ifndef SOURCE_ADVANCED_CTRL_H_
#define SOURCE_ADVANCED_CTRL_H_

#include "common.h"
#include "basic_ctrl.h"
#include "SmartCar_Pwm.h"
#include "SmartCar_Encoder.h"
#include "SmartCar_Systick.h"
#include "SmartCar_OLED.h"
#include "image.h"

typedef enum{
    car_straight,
    car_rightTurn,
    car_leftTurn,
    car_cross,//十字
    car_circle_in,//进环岛
    car_circle_out,
    car_ramp,//坡道
    car_branch,//三叉
    car_zebra,//斑马线
    car_unknown,
    car_stop,
    car_garage,//车库
}CAR_STATE;

extern int in_circle_flag;

extern int car_state;
extern int car_state_pre;
extern int car_direction;

extern int car_branch_direction;

extern float servo_angle_circle;

extern float servo_garage_left;
extern float servo_garage_right;

extern int car_time;
extern int circle_time;
extern int cross_time;
extern int branch_time;
extern int zebra_time;

void circle_in_ctrl(void);
void circle_out_ctrl(void);

void time_ctrl(void);//斑马线延时停车
void State_Update(void);//状态机更新
void Ctrl_Update();//选择目标速度
void my_start(void);//发车函数
void my_stop(void);//停车函数（手动停车）
void Garage_Enter(void);
void Garage_Quit(void);
void OutTract_Protect(void);

/*HISTORY相关*/
void History_Clean(void);
void History_Update(void);

extern int history_done[];


#endif /* SOURCE_ADVANCED_CTRL_H_ */
