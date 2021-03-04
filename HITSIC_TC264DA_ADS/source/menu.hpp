/*
 * menu.hpp
 *
 *  Created on: 2021年3月3日
 *      Author: 10798
 */

#ifndef SOURCE_MENU_HPP_
#define SOURCE_MENU_HPP_

#define KEY_REMOVE_JITTER 90 //按键消抖毫秒数

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
}DATA_PPTFLAG; //property 属性

typedef struct object{
    char name[20];//名称
    DATA_TYPE type;//数据存储模式
    int* pa;//整数地址
    float* pb;//浮点数地址

    char folder[20];//所在文件夹
    int num;//序号，指针指时为了避免越界，需要知道指的是第几个
    int ny;//纵坐标-打印使用

    DATA_PPTFLAG pptflag;//属性标志位

}OBJ;//每一项的数据


typedef struct position{
    int mode;//模式，1-光标选项，2-光标调数
    char folder[20];//光标所在文件夹
}POS;//光标所在位置



void Menu_Welcome(void);

void Menu_ShowFolder(void);//打印所在文件夹
void Menu_ShowObject(void);//打印菜单
void Menu_ShowCursor(void);//打印光标
void Menu_ShowState(void);//打印状态栏
void Menu_ShowUnder(int index);//打印底标

void Menu_In(void);//进入
void Menu_Out(void);//退出

void Menu_CursorMove(int flag);//移动光标位置
void Menu_CursorEnter(int flag);//进入光标所指对象

void Menu_IntDataAdjust(void);//整数改值函数
void Menu_FloatDataAdjust(void);//浮点数改值函数

void Menu_Calculate_Objnum(void);//计算总共的项数
void Menu_Calculate_num(void);//计算各个项的序号
void Menu_Calculate_ny(int flag);//计算各个项的纵坐标  0-初始化，1-上滚，-1-下滚

void Menu_KeyCheck_Cursor(void);//五项按键检测

void Menu_Tip(char* str);//打印提示信息函数
void Menu_Tip_Delay(char* str);//打印提示信息+延时

void KEY_Check_Right(void);
void KEY_Check_Enter(void);
void KEY_Check_Up(void);
void KEY_Check_Left(void);
void KEY_Check_Down(void);

void KEY_Clear(void);

void f91mgj(void);//凑数用的



#endif /* SOURCE_MENU_HPP_ */
