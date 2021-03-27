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

extern int car_state;
extern int car_state_pre;
extern int car_direction;

extern float servo_angle_circle;

extern uint32 servo_garage_left;
extern uint32 servo_garage_right;

//时间相关
extern int car_time;
extern int circle_time;

void startline_ctrl(void);//斑马线延时停车
void State_Update(void);//状态机更新
void Ctrl_Update(void);//选择目标速度
void my_start(void);//发车函数
void my_stop(void);//停车函数
void Garage_Enter(void);
void Garage_Quit(void);
void OutTract_Protect(void);

/*HISTORY相关*/
boolean History_check(int type);
void History_Update(int type);

extern int history_done[];
extern int history_todo[];


#endif /* SOURCE_ADVANCED_CTRL_H_ */
