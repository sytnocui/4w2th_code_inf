/*
 * advanced.cpp
 *
 *  Created on: 2021年3月3日
 *      Author: 10798
 */

#include "advanced_ctrl.hpp"


int car_state = stop;
int car_state_pre = stop;
int car_direction = 0;
int car_branch_direction = 0;

float servo_garage_left= SERVO_MID ;
float servo_garage_right= SERVO_MID ;

int history_done[stop+1] = {0};
int history_todo[stop+1] = {0};

void my_start(void)
{
    Delay_ms(STM0, 1000);
    startline_time = 0;//停车计时
    Garage_Quit();
}

void Ctrl_Update()
{
    /*直接跳出*/
    if(stop==car_state)
    {
        speed_dream = 0;
        return;
    }
    if(garage == car_state)
    {
        speed_dream = speed_dream_turn;
        return;
    }

    /*正式处理*/
    if(rightTurn==car_state || leftTurn==car_state)
    {
        speed_dream = speed_dream_turn;
    }
    else if(straight==car_state)
    {
        speed_dream = speed_dream_str;
    }
    else if(cross == car_state)
    {
        speed_dream = speed_dream_turn;
    }
    else if(zebra == car_state)
    {
        Garage_Enter();
    }
}

void State_Update(void)
{
    /*特殊状态直接跳出*/
    if(stop == car_state || garage == car_state)
    {
        return;
    }
    /*赛道保护*/
    OutTract_Protect();

    if(cross_flag == true)
    {
        car_state=cross;
        cross_flag = false;
    }
    else if(startline_flag == true
//            && start_line_x <= stop_line
//            && start_line_x >=50
            && startline_time>=10)//斑马线
    {
        car_state=zebra;
        startline_flag = false;
    }
    else if(k_mid > k_str && k_mid<2.5)
    {
        car_state=rightTurn;
    }
    else if(k_mid <-k_str && k_mid>-2.5)
    {
        car_state=leftTurn;
    }
    else if(k_mid >-k_str && k_mid < k_str)
    {
        car_state=straight;
    }
}

void startline_ctrl(void)
{
    startline_time++;
}

void Garage_Enter(void)
{
    car_state = garage;
//    Beep_Open();
    imu_clear();
    while(imu_angle_z < 90 && imu_angle_z>-90);
//    Beep_Close();
    car_state = stop;
}

void Garage_Quit(void)
{
    car_state = garage;
//    Beep_Open();
    imu_clear();
    while(imu_angle_z < 80 && imu_angle_z>-80);
//    Beep_Close();
    car_state = straight;
}

void OutTract_Protect(void)//出赛道保护
{
    if(protect_sw == 1)
    {
        if(mid_line[img_stop] == MISS)
        {
            car_state = stop;
        }
    }
}

/*--------------------------history相关函数-------------------------------*/
uint8 History_Check(int type)
{
    if(history_done[type] < history_todo[type])
    {
        return true;
    }
    else if(history_done[type] == history_todo[type])
    {
        return false;
    }
    else
    {
        return false;
        //添加错误函数
    }
}

void History_Update(int type)
{
    if(car_state != car_state_pre)
    {
        history_done[type]++;
        car_state_pre = car_state;
    }
}




