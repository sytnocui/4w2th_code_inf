/*���ļ������������ƣ������������������ǣ���������pid�������غ���*/

#include <basic_ctrl.hpp>

/*���PID����*/
float motor_kp = 0.0;    //�ٶȻ�p
float motor_ki = 0.0;    //�ٶȻ�i
float motor_kd = 0.0;    //�ٶȻ�d
float motor_err = 0;
float motor_err_pre = 0;
float motor_output = 0;

/*�����ٶ�*/
float speed_dream = 0;
float speed_dream_str = 0;
float speed_dream_turn = 0;

/*�����ٶ�*/
float speed_actual = 0;

/*����������ֵ*/
int enco_get = 0;
float enco_distance = 0;

/*���PID����*/
float servo_kp = 0.0;      //λ�û�p
float servo_ki = 0.0;      //λ�û�i
float servo_kd = 0.0;      //λ�û�d
float servo_err = 0;
float servo_err_pre = 0;
float servo_pid_output = 0;
float servo_output = SERVO_MID;


int stop_line = 100;

/*���ݣ���������*/
int img_var=0;
int startline_time =0;

void var_init(void)
{
    servo_kp = 0.25;
    servo_kd = 0.5;

    motor_kp = 20;
    motor_ki = 10;

    speed_dream = 1;
    speed_dream_str = 1;
    speed_dream_turn = 1;

    servo_garage_left = 6.6;
    servo_garage_right = 8.5;

    car_state = stop;

    imu_gyro_drift = 0.618;
}

void ctrl_init(void)
{
    pitMgr_t::insert(5U, 0U, motor_ctrl, pitMgr_t::enable);
    pitMgr_t::insert(20U, 1U, servo_ctrl, pitMgr_t::enable);
    pitMgr_t::insert(1000U, 7U, startline_ctrl, pitMgr_t::enable); //������
    //    pitMgr_t::insert(20U, 3U, ADC_Upgrade, pitMgr_t::enable);

    /*��ʼ����ֵ*/
    var_init();
}


void motor_ctrl(void)
{
    Encoder_Update();//���±���������ֵ������ʵ���ٶ�
    motor_pid_calculate();//���µ���ٶ�

    /*���״̬�жϣ��Ƿ����� �� ����ת*/
    if(motor_output >= 0)//������ת
    {
        SCFTM_PWM_ChangeHiRes(motor_FTM,forward_chnl,20000U,motor_output);
        SCFTM_PWM_ChangeHiRes(motor_FTM,backward_chnl,20000U,0.0);
    }
    else if(motor_output < 0)//���ַ�ת
    {
        SCFTM_PWM_ChangeHiRes(motor_FTM,forward_chnl,20000U,0.0);
        SCFTM_PWM_ChangeHiRes(motor_FTM,backward_chnl,20000U,(-motor_output));
    }
}

//TODO:̽���ܲ��ܴӶ�ʱ���Ƶ��ص�������
void servo_ctrl(void)
{
    /*����״̬������Ҫ���㣬��ֵ֮��ֱ������*/
    if(stop == car_state)//ͣ��
    {
        SCFTM_PWM_ChangeHiRes(servo_FTM,servo_chnl,50U,SERVO_MID);
        return;
    }
    if(garage == car_state)//�����
    {
        if(car_direction)
        {
            SCFTM_PWM_ChangeHiRes(servo_FTM,servo_chnl,50U,servo_garage_left);
        }
        else
        {
            SCFTM_PWM_ChangeHiRes(servo_FTM,servo_chnl,50U,servo_garage_right);
        }
        return;
    }

    /*��ʽ����*/
    servo_pid_calculate();
    SCFTM_PWM_ChangeHiRes(servo_FTM,servo_chnl,50U,servo_output);
}

void servo_pid_calculate(void)
{
    img_var = mid_line[prospect];//ǰհ������λ��
    servo_err = (float)(img_var-94)/10;
    servo_pid_output = servo_kp * servo_err + servo_kd * (servo_err - servo_err_pre);
    servo_output = SERVO_MID + servo_pid_output;
    servo_err_pre = servo_err;

    /*�޷�*/
    if(servo_output > SERVO_RIGHT)
        servo_output = SERVO_RIGHT;
    else if(servo_output < SERVO_LEFT)
        servo_output = SERVO_LEFT;
}

void motor_pid_calculate(void)
{
    motor_err_pre = motor_err;
    motor_err = speed_dream - speed_actual;      //�����ٶ�ƫ��
    motor_output += motor_ki*motor_err + motor_kp* (motor_err-motor_err_pre);    //�������

    /*����޷�*/
    if(motor_output >= 99)
        motor_output = 99;
    else if(motor_output <= -99)
        motor_output = -99;
}

void Encoder_Update(void)
{
    enco_get = -SmartCar_Encoder_Get(GPT12_T2);
    enco_distance += enco_get * 0.000167;       //*0.6 / 3592.4
    speed_actual = enco_get * 0.0334;   //*0.6 / 3592.4 / 0.005   *0.6 / 3592.4Ϊ�����������ı�����/0.005Ϊdt��
    SmartCar_Encoder_Clear(GPT12_T2);
    //TODO:
}

void Encoder_Distance_Clear(void)
{
    enco_distance = 0;
}

