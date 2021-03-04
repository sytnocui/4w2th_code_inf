#include "menu.hpp"
#include "string.h"

//int cursor_y = 1;//���y����
int objnum =0;//����
/*����˵����ݣ����ʱ�뱣֤ͬһ�ļ����¶��������*/
OBJ obj[]={ {"state",my_variType,&car_state,NULL,"[MAIN]",0,0,ReadOnly_Flag},//ʵʱ��ʾ:����Ƕȣ����ת��
            {"time",my_variType,&startline_time,NULL,"[MAIN]",0,0,ReadOnly_Flag},//TODO:readonlyдһ��
            {"[SETVO]",my_menuType,NULL,NULL,"[MAIN]",0,0,Ordinary_Flag},   /*�������*/
            {"[MOTOR]",my_menuType,NULL,NULL,"[MAIN]",0,0,Ordinary_Flag},   /*�������*/
            {"[IMAGE]",my_menuType,NULL,NULL,"[MAIN]",0,0,Ordinary_Flag},   /*ͼ�����*/

            {"s_kpStr",my_varfType,NULL,&servo_kp,"[SETVO]",0,0,Ordinary_Flag},
            {"s_kdStr",my_varfType,NULL,&servo_kd,"[SETVO]",0,0,Ordinary_Flag},

            {"speed_str",my_varfType,NULL,&speed_dream_str,"[MOTOR]",0,0,Ordinary_Flag},
            {"speed_turn",my_varfType,NULL,&speed_dream_turn,"[MOTOR]",0,0,Ordinary_Flag},
            {"m_ki",my_varfType,NULL,&motor_ki,"[MOTOR]",0,0,Ordinary_Flag},
            {"m_kp",my_varfType,NULL,&motor_kp,"[MOTOR]",0,0,Ordinary_Flag},

            {"true",my_varfType,NULL,&threshold,"[IMAGE]",0,0,Ordinary_Flag},//TODO:����ֵ�ĳ�����
            {"forward",my_variType,&prospect,NULL,"[IMAGE]",0,0,Ordinary_Flag},
            {"img_stop",my_variType,&img_stop,NULL,"[IMAGE]",0,0,Ordinary_Flag},//������ͣ���ж�
            {"k_str",my_varfType,NULL,&k_str,"[IMAGE]",0,0,Ordinary_Flag},//ֱ�����б���ж�
            {0,0,0,0,0,0,0,0,0,0}};//����⵽����Ϊ0��ͣ


OBJ *pobj=&obj[0];

POS cursor_pos={0,"[MAIN]"};

KEY_STATE key_state=key_gnd;


void Menu_Welcome(void)
{
    Menu_Calculate_Objnum();//�����ܹ�������
    Menu_Calculate_num();//�������������
    Menu_Calculate_ny(0);//����������������  0-��ʼ����1-�Ϲ���-1-�¹�

    SmartCar_OLED_Fill(0x00);

    Menu_ShowFolder();//��ӡ�����ļ���
    Menu_ShowObject();//��ӡ�˵�
    Menu_ShowCursor();//��ӡ���
    Menu_ShowState();//��ӡ״̬��

    Menu_KeyCheck_Cursor();
    KEY_Clear();
}
void Menu_ShowObject(void)//��ӡ�˵�
{
    int i=0;
    for(i=0;obj[i].name[0]!=0;i++)
    {
        if(strcmp(cursor_pos.folder,obj[i].folder)==0)
        {
            if(obj[i].ny>=1 && obj[i].ny<8)
            {
                SmartCar_OLED_P6x8Str(10,obj[i].ny,obj[i].name);
                if(obj[i].type == my_variType)
                {
                    SmartCar_OLED_Printf6x8(80,obj[i].ny,"%3d",*(obj[i].pa));
                }
                else if(obj[i].type == my_varfType)
                {
                    SmartCar_OLED_Printf6x8(80,obj[i].ny,"%7.3f",*(obj[i].pb));
                }
                else if(obj[i].type == my_menuType)
                {
                    ;
                }
                else
                {
                    Menu_Tip_Delay("ShowObject");
                }
            }
        }
    }
}
void Menu_ShowFolder(void)//��ӡ�����ļ���
{
    SmartCar_OLED_P6x8Str(0,0,pobj->folder);//��ӡ�����ļ�����
}
void Menu_ShowCursor(void)//��ӡ���
{
    int i;
    for(i=1;i<8;i++) //����֮ǰ�Ĺ��λ��
    {
        SmartCar_OLED_P6x8Str(0,i," ");
    }
    SmartCar_OLED_P6x8Str(0,pobj->ny,">");
}
void Menu_ShowState(void)//��ӡ״̬��
{
    SmartCar_OLED_Printf6x8(90,0,"%2d/%2d",pobj->num,objnum);
}
void Menu_ShowUnder(int index)//��ӡ����ʱ������
{
    int under_pos=0;
    under_pos=50+(2-index)*6;//����λ��
    SmartCar_OLED_P6x8Str(20,6,"          ");
    SmartCar_OLED_P6x8Str(under_pos,6,"^");
}

void Menu_In(void)//����
{
    if(pobj->type == my_variType)
    {
        cursor_pos.mode=1;//�����ֵ����
        Menu_IntDataAdjust();
    }
    else if(pobj->type == my_varfType)
    {
        cursor_pos.mode=1;//�����ֵ����
        Menu_FloatDataAdjust();
    }
    else if(pobj->type == my_menuType)
    {
        strcpy(cursor_pos.folder,pobj->name);
        int i;
        for(i=0;obj[i].name[0]!=0;i++)
        {
            if(strcmp(obj[i].folder,cursor_pos.folder)==0)
            {
                pobj=&obj[i];//pobjָ���ڴ��ļ����ڵ�һ��
                return;
            }
        }
    }
    else
    {
        Menu_Tip_Delay("menu_in");
    }
}
void Menu_Out(void)//�˳�
{
    if(strcmp(cursor_pos.folder,"[MAIN]")!=0)
    {
        strcpy(cursor_pos.folder,"[MAIN]");
        pobj=&obj[0];
    }
    else if(strcmp(cursor_pos.folder,"[MAIN]") == 0)
    {
        ;//ʲô������
    }
}

void Menu_CursorMove(int flag)
{
    if(flag==1 && pobj->num>1)//��ֹԽ��
    {
        pobj--;
    }
    else if(flag==-1 && pobj->num<objnum)//��ֹԽ��
    {
        pobj++;
    }
    else
    {
        //Menu_Warning();//Խ�羯�棬����û��
    }
    if(pobj->ny<1)//������߽��˾͹���Ļ
    {
        Menu_Calculate_ny(1);
        SmartCar_OLED_Fill(0x00);
        Menu_ShowObject();//��ӡ�˵�
    }
    else if(pobj->ny>7)
    {
        Menu_Calculate_ny(-1);
        SmartCar_OLED_Fill(0x00);
        Menu_ShowObject();//��ӡ�˵�

    }
    else if(pobj->ny>=1 && pobj->ny<=7)
    {
        ;//ʲô������
    }
    else
    {
        Menu_Tip_Delay("Menu_CursorMove");
    }
    Menu_ShowCursor();//��ӡ���
    Menu_ShowState();//����״̬��
}
void Menu_CursorEnter(int flag)
{
    if(flag==1)
    {
        Menu_In();
    }
    if(flag==-1)
    {
        Menu_Out();
    }
}

void Menu_KeyCheck_Cursor(void)//��������
{
    while(1)
    {
       if(key_gnd != key_state)
       {
           Delay_ms(STM0,KEY_REMOVE_JITTER);
           if(key_up == key_state)
           {
               KEY_Clear();//��հ�������
               Menu_CursorMove(1);
           }
           else if(key_down == key_state)
           {
               KEY_Clear();//��հ�������
               Menu_CursorMove(-1);
           }
           else if(key_left == key_state)
           {
               KEY_Clear();//��հ�������
               Menu_CursorEnter(-1);
               return;
           }
           else if(key_right == key_state)
           {
               KEY_Clear();//��հ�������
               Menu_CursorEnter(1);
               return;
           }
           else if(key_enter == key_state)
           {
               KEY_Clear();//��հ�������
               Menu_CursorEnter(1);
               return;
           }
       }
    }
}

void Menu_IntDataAdjust(void)//������ֵ�ܺ���
{
    if(ReadOnly_Flag == pobj->pptflag)
    {
        Menu_Tip_Delay("Read Only");
        return;
    }
    int temp=0;
    int index=0;
    int figure=0;
    int flag=0;
    temp = *(pobj->pa);

    SmartCar_OLED_Fill(0x00);
    SmartCar_OLED_Printf6x8(0,0,"---%s---",pobj->name);
    SmartCar_OLED_P6x8Str(0,1,"integer adjust:");
    SmartCar_OLED_Printf6x8(20,3,"past:%3d",*(pobj->pa));
    SmartCar_OLED_Printf6x8(20,4,"save:%3d",*(pobj->pa));
    SmartCar_OLED_Printf6x8(20,5,"now :%3d",*(pobj->pa));
    Menu_ShowUnder(index);
    while(1)
    {
        flag=0;
        figure=0;
        if(key_gnd != key_state)
        {
            Delay_ms(STM0,KEY_REMOVE_JITTER);
           if(key_up == key_state)
           {
               KEY_Clear();//��հ�������
               figure=1;
               flag=1;
           }
           else if(key_down == key_state)
           {
               KEY_Clear();//��հ�������
               figure=-1;
               flag=1;
           }
           else if(key_left == key_state)
           {
               KEY_Clear();//��հ�������
               index++;
               if(index>2)
               {
                   index=2;
               }
               flag=2;
           }
           else if(key_right == key_state)
           {
               KEY_Clear();//��հ�������
               index--;
               if(index<0)
               {
                   index=0;
               }
               flag=2;
           }
           else if(key_enter == key_state)
           {
               KEY_Clear();//��հ�������
               if(*(pobj->pa) == temp)
               {
                   SmartCar_OLED_P6x8Str(40,7,"Adjusted OK!");
                   Delay_ms(STM0,500);
                   return;
               }
               else if(*(pobj->pa) != temp)
               {
                   *(pobj->pa) = temp;
                   SmartCar_OLED_P6x8Str(20,4,"             ");
                   SmartCar_OLED_Printf6x8(20,4,"save:%3d",*(pobj->pa));
               }
           }
        }

        /*������������Ľ����ֵ*/
        if(flag==1)
        {
            temp=temp+figure*pow(10,index);
            SmartCar_OLED_Printf6x8(20,5,"now :%3d",temp);
        }
        else if(flag==2)
        {
            Menu_ShowUnder(index);
        }
        else if(flag==0)
        {
            ;//ʲô������
        }
        else
        {
            Menu_Tip_Delay("Menu_IntDataAdjust");
        }
    }
}

void Menu_FloatDataAdjust(void)//��������ֵ�ܺ���
{
    if(ReadOnly_Flag == pobj->pptflag)
    {
        Menu_Tip_Delay("Read Only");
        return;
    }

    float temp=0;
    int index=0;
    int figure=0;
    int flag=0;
    temp = *(pobj->pb);

    SmartCar_OLED_Fill(0x00);//����
    SmartCar_OLED_Printf6x8(0,0,"---%s---",pobj->name);
    SmartCar_OLED_P6x8Str(0,1,"integer adjust:");
    SmartCar_OLED_Printf6x8(20,3,"past:%7.3f",*(pobj->pb));
    SmartCar_OLED_Printf6x8(20,4,"save:%7.3f",*(pobj->pb));
    SmartCar_OLED_Printf6x8(20,5,"now :%7.3f",*(pobj->pb));
    Menu_ShowUnder(index);
    while(1)
    {
        flag=0;
        figure=0;
        if(key_gnd != key_state)
        {
            Delay_ms(STM0,KEY_REMOVE_JITTER);
           if(key_state == key_up)
           {
               KEY_Clear();//��հ�������
               figure=1;
               flag=1;
           }
           else if(key_state == key_down)
           {
               KEY_Clear();//��հ�������
               figure=-1;
               flag=1;
           }
           else if(key_state == key_left)
           {
               KEY_Clear();//��հ�������
               index++;
               if(index>2)
               {
                   index=2;
               }
               flag=2;
           }
           else if(key_state == key_right)
           {
               KEY_Clear();//��հ�������
               index--;
               if(index<-3)
               {
                   index=-3;
               }
               flag=2;
           }
           else if(key_state == key_enter)
           {
               KEY_Clear();//��հ�������
               if(*(pobj->pb)==temp)
               {
                   SmartCar_OLED_P6x8Str(40,7,"Adjusted OK!");
                   Delay_ms(STM0,500);
                   return;
               }
               else if(*(pobj->pb) != temp)
               {
                   *(pobj->pb) = temp;
                   SmartCar_OLED_P6x8Str(20,4,"             ");
                   SmartCar_OLED_Printf6x8(20,4,"save:%7.3f",*(pobj->pb));
               }
           }
        }

        /*������������Ľ����ֵ*/
        if(flag==1)
        {
            temp=temp+figure*pow(10,index);
            SmartCar_OLED_Printf6x8(20,5,"now :%7.3f",temp);
        }
        else if(flag==2)
        {
            int index1=0;
            if(index<0)
            {
                index1=index-1;
            }
            else
            {
                index1=index;
            }
            Menu_ShowUnder(index1);
        }
        else if(flag==0)
        {
            ;//ʲô������
        }
        else
        {
            Menu_Tip_Delay("FloatDataAdjust");
        }
    }
}

void Menu_Calculate_Objnum(void)//�����ܹ�������
{
    int i;
    objnum=0;
    for(i=0;obj[i].name[0]!=0;i++)
    {
        if(strcmp(obj[i].folder,cursor_pos.folder)==0)
        {
            objnum++;
        }
    }
}
void Menu_Calculate_num(void)//�������������
{
    int i=0;
    int p=1;
    obj[0].num=p;
    p++;
    for(i=1;obj[i].name[0]!=0;i++)
    {
        if(strcmp(obj[i].folder,obj[i-1].folder)==0)
        {
            obj[i].num=p;
            p++;
        }
        else
        {
            p=1;
            obj[i].num=p;
            p++;
        }
    }
}
void Menu_Calculate_ny(int flag)//����������������   0-��ʼ����1-�Ϲ���-1-�¹�
{
    int i;
    for(i=0;obj[i].name[0]!=0;i++)
    {
        if(flag==0)//��ʼ��
        {
            obj[i].ny=obj[i].num;
        }
        else if(flag==1)//�Ϲ���������ϣ�
        {
            obj[i].ny++;
        }
        else if(flag==-1)//�¹���������£�
        {
            obj[i].ny--;
        }
    }
}

void Menu_Tip(char* str)//���Ͻ�����
{
    SmartCar_OLED_P6x8Str(0,0,str);
}
//TODO:������ʾ��Ϣ������
void Menu_Tip_Delay(char* str)//���Ͻ�����
{
    SmartCar_OLED_P6x8Str(0,0,str);
    Delay_ms(STM0,1000);
}


void KEY_Clear(void)
{
    key_state = key_gnd;
}


void f91mgj(void)//�������ֻ�Ǹ������ĺ���ָ��
{
    /*��һ����Զ���Ҵ��*/;
}
