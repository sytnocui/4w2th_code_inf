/*
 * menu.hpp
 *
 *  Created on: 2021��3��3��
 *      Author: 10798
 */

#ifndef SOURCE_MENU_HPP_
#define SOURCE_MENU_HPP_

#define KEY_REMOVE_JITTER 90 //��������������

#include "everything.hpp"


typedef enum {
    key_gnd,
    key_right,
    key_enter,
    key_up,
    key_left,
    key_down,
}KEY_STATE;

typedef enum {
  my_variType,
  my_varfType,
  my_menuType
}DATA_TYPE;

typedef enum{
    Ordinary_Flag,
    ReadOnly_Flag,
}DATA_PPTFLAG; //property ����

typedef struct object{
    char name[20];//����
    DATA_TYPE type;//���ݴ洢ģʽ
    int* pa;//������ַ
    float* pb;//��������ַ

    char folder[20];//�����ļ���
    int num;//��ţ�ָ��ָʱΪ�˱���Խ�磬��Ҫ֪��ָ���ǵڼ���
    int ny;//������-��ӡʹ��

    DATA_PPTFLAG pptflag;//���Ա�־λ

}OBJ;//ÿһ�������


typedef struct position{
    int mode;//ģʽ��1-���ѡ�2-������
    char folder[20];//��������ļ���
}POS;//�������λ��



void Menu_Welcome(void);

void Menu_ShowFolder(void);//��ӡ�����ļ���
void Menu_ShowObject(void);//��ӡ�˵�
void Menu_ShowCursor(void);//��ӡ���
void Menu_ShowState(void);//��ӡ״̬��
void Menu_ShowUnder(int index);//��ӡ�ױ�

void Menu_In(void);//����
void Menu_Out(void);//�˳�

void Menu_CursorMove(int flag);//�ƶ����λ��
void Menu_CursorEnter(int flag);//��������ָ����

void Menu_IntDataAdjust(void);//������ֵ����
void Menu_FloatDataAdjust(void);//��������ֵ����

void Menu_Calculate_Objnum(void);//�����ܹ�������
void Menu_Calculate_num(void);//�������������
void Menu_Calculate_ny(int flag);//����������������  0-��ʼ����1-�Ϲ���-1-�¹�

void Menu_KeyCheck_Cursor(void);//��������

void Menu_Tip(char* str);//��ӡ��ʾ��Ϣ����
void Menu_Tip_Delay(char* str);//��ӡ��ʾ��Ϣ+��ʱ

void KEY_Check_Right(void);
void KEY_Check_Enter(void);
void KEY_Check_Up(void);
void KEY_Check_Left(void);
void KEY_Check_Down(void);

void KEY_Clear(void);

void f91mgj(void);//�����õ�



#endif /* SOURCE_MENU_HPP_ */
