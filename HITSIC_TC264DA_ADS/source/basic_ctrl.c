/*���ļ������������ƣ������������������ǣ���������pid�������غ���*/

#include <basic_ctrl.h>

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

/*�����ǲ���*/
mpu_t my_mpu;
mpu_t* this_mpu = &my_mpu;
float imu_dt = 0;
float imu_angle_z = 0;
float imu_gyro_z = 0;
float imu_angle_drift = 0;
float imu_accl[3] = {0.0f, 0.0f, 0.0f};
float imu_gyro[3] = {0.0f, 0.0f, 0.0f};


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
    servo_kp = 3;
    servo_kd = 6;

    motor_kp = 1000;
    motor_ki = 500;

    speed_dream = 0;
    speed_dream_str = 1;
    speed_dream_turn = 1;

    servo_garage_left = 750;
    servo_garage_right = 850;

    motor_output = 0;

//    imu_angle_drift = 0.535;
    imu_dt = 0.05;

    car_state = car_stop;

    car_direction = 1;
}

void ctrl_init(void)
{
    /*��ʼ����ֵ*/
    var_init();
}

//TODO:float-->uint32,��ͨ��
void motor_ctrl(void)
{
//    Encoder_Update();//���±���������ֵ������ʵ���ٶ�
//    motor_pid_calculate();//���µ���ٶ�

    motor_output = 1000 * speed_dream;
    /*���״̬�жϣ��Ƿ����� �� ����ת*/
    if(motor_output >= 0)//������ת
    {
        SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_6_TOUT6_P02_6_OUT,(uint32)motor_output);
        SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_7_TOUT7_P02_7_OUT,0);
    }
    else if(motor_output < 0)//���ַ�ת
    {
        SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_6_TOUT6_P02_6_OUT,0);
        SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_7_TOUT7_P02_7_OUT,(uint32)(-motor_output));
    }
}

//TODO:̽���ܲ��ܴӶ�ʱ���Ƶ��ص�������
//TODO:float-->uint32,��ͨ��
void servo_ctrl(void)
{
    /*����״̬������Ҫ���㣬��ֵ֮��ֱ������*/
    if(car_stop == car_state)//ͣ��
    {
        SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM1_1_TOUT31_P33_9_OUT,(uint32)SERVO_MID);
        return;
    }
    if(car_garage == car_state)//�����
    {
        if(car_direction)
        {
            SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM1_1_TOUT31_P33_9_OUT,(uint32)servo_garage_left);
        }
        else
        {
            SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM1_1_TOUT31_P33_9_OUT,(uint32)servo_garage_right);
        }
        return;
    }

    /*��ʽ����*/
    servo_pid_calculate();
    SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM1_1_TOUT31_P33_9_OUT,(uint32)servo_output);
}

void servo_pid_calculate(void)
{
    img_var = mid_line[prospect];//ǰհ������λ��
    servo_err = (float)(img_var-94);
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
    if(motor_output >= 9900)
        motor_output = 9900;
    else if(motor_output <= -9900)
        motor_output = -9900;
}

void Encoder_Update(void)
{
    enco_get = SmartCar_Encoder_Get(GPT12_T5);
    enco_distance += enco_get * 0.000167;       //*0.6 / 3592.4
    speed_actual = enco_get * 0.0334;   //*0.6 / 3592.4 / 0.005   *0.6 / 3592.4Ϊ�����������ı�����/0.005Ϊdt��
    SmartCar_Encoder_Clear(GPT12_T5);
}

void Encoder_Distance_Clear(void)
{
    enco_distance = 0;
}

void imu_clear(void)
{
    imu_angle_z = 0;
}

void imu_update(void)
{
    SmartCar_MPU_Getgyro2(this_mpu);
//    imu_gyro[0] = this_mpu->mpu_rawdata.gyro_x;
//    imu_gyro[1] = this_mpu->mpu_rawdata.gyro_y;
    imu_gyro[2] = this_mpu->mpu_rawdata.gyro_z;
    imu_gyro_z = imu_gyro[2] - imu_angle_drift;
    imu_angle_z += imu_gyro_z * imu_dt;

}

