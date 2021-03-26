/*
 * advanced.cpp
 *
 *  Created on: 2021��3��3��
 *      Author: 10798
 */

#include <advanced_ctrl.h>


int car_state = stop;
int car_state_pre = stop;
int car_direction = 0;
int car_branch_direction = 0;

uint32 servo_garage_left= SERVO_MID ;
uint32 servo_garage_right= SERVO_MID ;

int history_done[stop+1] = {0};
int history_todo[stop+1] = {0};

void my_start(void)
{
    SmartCar_OLED_Printf6x8(0, 2,"ready");
    Delay_ms(STM0, 1000);
    startline_time = 0;//ͣ����ʱ
//    Garage_Quit();
    car_state = straight;
}

void my_stop(void)
{
    SmartCar_OLED_Printf6x8(0, 2,"stop ");
    Delay_ms(STM0,1000);//����
//    Garage_Enter();
    car_state = stop;
}

void Ctrl_Update(void)
{
    /*ֱ������*/
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

    /*��ʽ����*/
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
//        car_state = stop;
    }
}

void State_Update(void)
{
    /*����״ֱ̬������*/
    if(stop == car_state || garage == car_state)
    {
        return;
    }
    /*��������*/
    OutTract_Protect();

    if(cross_flag == TRUE)
    {
        car_state=cross;
        cross_flag = FALSE;
    }
    else if(startline_flag == TRUE
//            && start_line_x <= stop_line
//            && start_line_x >=50
            && startline_time>=10)//������
    {
        car_state=zebra;
        startline_flag = FALSE;
    }
//    else if(k_mid > k_str && k_mid<2.5)
//    {
//        car_state=rightTurn;
//    }
//    else if(k_mid <-k_str && k_mid>-2.5)
//    {
//        car_state=leftTurn;
//    }
    //todo:�������ת���״̬��
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
//    beep_Open();
    imu_clear();
    speed_dream = speed_dream_turn;//����״̬��������while������������Ŀ���ٶ�
    while(imu_angle_z < 90 && imu_angle_z>-90);
//    beep_Close();
    car_state = stop;
}

void Garage_Quit(void)
{
    car_state = garage;
//    beep_open();
    imu_clear();
    speed_dream = speed_dream_turn;
    while(imu_angle_z < 80 && imu_angle_z>-80);
//    beep_close();
    car_state = straight;
}

void OutTract_Protect(void)//����������
{
//    beep_open();
    if(protect_sw == 1)
    {
        if(mid_line[img_stop] == MISS)
        {
            car_state = stop;
        }
    }
}

/*--------------------------history��غ���-------------------------------*/
boolean History_Check(int type)
{
    if(history_done[type] < history_todo[type])
    {
        return TRUE;
    }
    else if(history_done[type] == history_todo[type])
    {
        return FALSE;
    }
    else
    {
        return FALSE;
        //��Ӵ�����
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




