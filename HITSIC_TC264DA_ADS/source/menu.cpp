#include "old_menu.hpp"

//int cursor_y = 1;//光标y坐标
int objnum =0;//项数
/*定义菜单数据，添加时请保证同一文件夹下对象的相邻*/
OBJ obj[]={ {"state",my_variType,&car_state,NULL,"[MAIN]",0,0,ReadOnly_Flag},//实时显示:舵机角度，电机转速
            {"time",my_variType,&startline_time,NULL,"[MAIN]",0,0,ReadOnly_Flag},//TODO:readonly写一下
            {"[SETVO]",my_menuType,NULL,NULL,"[MAIN]",0,0,Ordinary_Flag},   /*舵机参数*/
            {"[MOTOR]",my_menuType,NULL,NULL,"[MAIN]",0,0,Ordinary_Flag},   /*电机参数*/
            {"[IMAGE]",my_menuType,NULL,NULL,"[MAIN]",0,0,Ordinary_Flag},   /*图像参数*/

            {"s_kpStr",my_varfType,NULL,&servo_kpStr,"[SETVO]",0,0,Ordinary_Flag},
            {"s_kdStr",my_varfType,NULL,&servo_kdStr,"[SETVO]",0,0,Ordinary_Flag},
            {"s_kpTurn",my_varfType,NULL,&servo_kpTurn,"[SETVO]",0,0,Ordinary_Flag},
            {"s_kdTurn",my_varfType,NULL,&servo_kdTurn,"[SETVO]",0,0,Ordinary_Flag},

            {"speed_str",my_varfType,NULL,&speed_dream_str,"[MOTOR]",0,0,Ordinary_Flag},
            {"speed_turn",my_varfType,NULL,&speed_dream_turn,"[MOTOR]",0,0,Ordinary_Flag},
            {"m_ki",my_varfType,NULL,&motor_speed_ki,"[MOTOR]",0,0,Ordinary_Flag},
            {"m_kp",my_varfType,NULL,&motor_speed_kp,"[MOTOR]",0,0,Ordinary_Flag},

            {"true",my_varfType,NULL,&threshold,"[IMAGE]",0,0,Ordinary_Flag},//TODO:把阈值改成整数
            {"forward",my_variType,&prospect,NULL,"[IMAGE]",0,0,Ordinary_Flag},
            {"img_stop",my_variType,&img_stop,NULL,"[IMAGE]",0,0,Ordinary_Flag},//斑马线停车判断
            {"k_str",my_varfType,NULL,&k_str,"[IMAGE]",0,0,Ordinary_Flag},//直道弯道斜率判断
            {0,0,0,0,0,0,0,0,0,0}};//最后检测到名字为0就停


OBJ *pobj=&obj[0];

POS cursor_pos={0,"[MAIN]"};

KEY_STATE key_state=key_gnd;


void Menu_Welcome(void)
{
    Menu_Calculate_Objnum();//计算总共的项数
    Menu_Calculate_num();//计算各个项的序号
    Menu_Calculate_ny(0);//计算各个项的纵坐标  0-初始化，1-上滚，-1-下滚

    DISP_SSD1306_Fill(0);

    Menu_ShowFolder();//打印所在文件夹
    Menu_ShowObject();//打印菜单
    Menu_ShowCursor();//打印光标
    Menu_ShowState();//打印状态栏

    Menu_KeyCheck_Cursor();
    KEY_Clear();
}
void Menu_ShowObject(void)//打印菜单
{
    int i=0;
    for(i=0;obj[i].name[0]!=0;i++)
    {
        if(strcmp(cursor_pos.folder,obj[i].folder)==0)
        {
            if(obj[i].ny>=1 && obj[i].ny<8)
            {
                DISP_SSD1306_Print_F6x8(10,obj[i].ny,obj[i].name);
                if(obj[i].type == my_variType)
                {
                    DISP_SSD1306_Printf_F6x8(80,obj[i].ny,"%3d",*(obj[i].pa));
                }
                else if(obj[i].type == my_varfType)
                {
                    DISP_SSD1306_Printf_F6x8(80,obj[i].ny,"%7.3f",*(obj[i].pb));
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
void Menu_ShowFolder(void)//打印所在文件夹
{
    DISP_SSD1306_Print_F6x8(0,0,pobj->folder);//打印所在文件夹名
}
void Menu_ShowCursor(void)//打印光标
{
    int i;
    for(i=1;i<8;i++) //清理之前的光标位置
    {
        DISP_SSD1306_Print_F6x8(0,i," ");
    }
    DISP_SSD1306_Print_F6x8(0,pobj->ny,">");
}
void Menu_ShowState(void)//打印状态栏
{
    DISP_SSD1306_Printf_F6x8(90,0,"%2d/%2d",pobj->num,objnum);
}
void Menu_ShowUnder(int index)//打印调数时底座标
{
    int under_pos=0;
    under_pos=50+(2-index)*6;//计算位置
    DISP_SSD1306_Print_F6x8(20,6,"          ");
    DISP_SSD1306_Print_F6x8(under_pos,6,"^");
}

void Menu_In(void)//进入
{
    if(pobj->type == my_variType)
    {
        cursor_pos.mode=1;//进入改值函数
        Menu_IntDataAdjust();
    }
    else if(pobj->type == my_varfType)
    {
        cursor_pos.mode=1;//进入改值函数
        Menu_FloatDataAdjust();
    }
    else if(pobj->type == my_menuType)
    {
        strncpy(cursor_pos.folder,pobj->name,10);
        int i;
        for(i=0;obj[i].name[0]!=0;i++)
        {
            if(strcmp(obj[i].folder,cursor_pos.folder)==0)
            {
                pobj=&obj[i];//pobj指向在此文件夹内的一项
                return;
            }
        }
    }
    else
    {
        Menu_Tip_Delay("menu_in");
    }
}
void Menu_Out(void)//退出
{
    if(strcmp(cursor_pos.folder,"[MAIN]")!=0)
    {
        strncpy(cursor_pos.folder,"[MAIN]",10);
        pobj=&obj[0];
    }
    else if(strcmp(cursor_pos.folder,"[MAIN]") == 0)
    {
        ;//什么都不做
    }
}

void Menu_CursorMove(int flag)
{
    if(flag==1 && pobj->num>1)//防止越界
    {
        pobj--;
    }
    else if(flag==-1 && pobj->num<objnum)//防止越界
    {
        pobj++;
    }
    else
    {
        //Menu_Warning();//越界警告，等于没有
    }
    if(pobj->ny<1)//如果到边界了就滚屏幕
    {
        Menu_Calculate_ny(1);
        DISP_SSD1306_Fill(0);
        Menu_ShowObject();//打印菜单
    }
    else if(pobj->ny>7)
    {
        Menu_Calculate_ny(-1);
        DISP_SSD1306_Fill(0);
        Menu_ShowObject();//打印菜单

    }
    else if(pobj->ny>=1 && pobj->ny<=7)
    {
        ;//什么都不做
    }
    else
    {
        Menu_Tip_Delay("Menu_CursorMove");
    }
    Menu_ShowCursor();//打印光标
    Menu_ShowState();//更新状态栏
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

void Menu_KeyCheck_Cursor(void)//五项按键检测
{
    while(1)
    {
       if(key_gnd != key_state)
       {
           SDK_DelayAtLeastUs(KEY_REMOVE_JITTER*1000,CLOCK_GetFreq(kCLOCK_CoreSysClk));
           if(key_up == key_state)
           {
               KEY_Clear();//清空按键检验
               Menu_CursorMove(1);
           }
           else if(key_down == key_state)
           {
               KEY_Clear();//清空按键检验
               Menu_CursorMove(-1);
           }
           else if(key_left == key_state)
           {
               KEY_Clear();//清空按键检验
               Menu_CursorEnter(-1);
               return;
           }
           else if(key_right == key_state)
           {
               KEY_Clear();//清空按键检验
               Menu_CursorEnter(1);
               return;
           }
           else if(key_enter == key_state)
           {
               KEY_Clear();//清空按键检验
               Menu_CursorEnter(1);
               return;
           }
       }
    }
}

void Menu_IntDataAdjust(void)//整数改值总函数
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

    DISP_SSD1306_Fill(0);
    DISP_SSD1306_Printf_F6x8(0,0,"---%s---",pobj->name);
    DISP_SSD1306_Print_F6x8(0,1,"integer adjust:");
    DISP_SSD1306_Printf_F6x8(20,3,"past:%3d",*(pobj->pa));
    DISP_SSD1306_Printf_F6x8(20,4,"save:%3d",*(pobj->pa));
    DISP_SSD1306_Printf_F6x8(20,5,"now :%3d",*(pobj->pa));
    Menu_ShowUnder(index);
    while(1)
    {
        flag=0;
        figure=0;
        if(key_gnd != key_state)
        {
            SDK_DelayAtLeastUs(KEY_REMOVE_JITTER*1000,CLOCK_GetFreq(kCLOCK_CoreSysClk));
           if(key_up == key_state)
           {
               KEY_Clear();//清空按键检验
               figure=1;
               flag=1;
           }
           else if(key_down == key_state)
           {
               KEY_Clear();//清空按键检验
               figure=-1;
               flag=1;
           }
           else if(key_left == key_state)
           {
               KEY_Clear();//清空按键检验
               index++;
               if(index>2)
               {
                   index=2;
               }
               flag=2;
           }
           else if(key_right == key_state)
           {
               KEY_Clear();//清空按键检验
               index--;
               if(index<0)
               {
                   index=0;
               }
               flag=2;
           }
           else if(key_enter == key_state)
           {
               KEY_Clear();//清空按键检验
               if(*(pobj->pa) == temp)
               {
                   DISP_SSD1306_Print_F6x8(40,7,"Adjusted OK!");
                   SDK_DelayAtLeastUs(500*1000,CLOCK_GetFreq(kCLOCK_CoreSysClk));
                   return;
               }
               else if(*(pobj->pa) != temp)
               {
                   *(pobj->pa) = temp;
                   DISP_SSD1306_Print_F6x8(20,4,"             ");
                   DISP_SSD1306_Printf_F6x8(20,4,"save:%3d",*(pobj->pa));
               }
           }
        }

        /*根据上述检验的结果改值*/
        if(flag==1)
        {
            temp=temp+figure*pow(10,index);
            DISP_SSD1306_Printf_F6x8(20,5,"now :%3d",temp);
        }
        else if(flag==2)
        {
            Menu_ShowUnder(index);
        }
        else if(flag==0)
        {
            ;//什么都不做
        }
        else
        {
            Menu_Tip_Delay("Menu_IntDataAdjust");
        }
    }
}

void Menu_FloatDataAdjust(void)//浮点数改值总函数
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

    DISP_SSD1306_Fill(0);
    DISP_SSD1306_Printf_F6x8(0,0,"---%s---",pobj->name);
    DISP_SSD1306_Print_F6x8(0,1,"integer adjust:");
    DISP_SSD1306_Printf_F6x8(20,3,"past:%7.3f",*(pobj->pb));
    DISP_SSD1306_Printf_F6x8(20,4,"save:%7.3f",*(pobj->pb));
    DISP_SSD1306_Printf_F6x8(20,5,"now :%7.3f",*(pobj->pb));
    Menu_ShowUnder(index);
    while(1)
    {
        flag=0;
        figure=0;
        if(key_gnd != key_state)
        {
            SDK_DelayAtLeastUs(KEY_REMOVE_JITTER*1000,CLOCK_GetFreq(kCLOCK_CoreSysClk));
           if(key_state == key_up)
           {
               KEY_Clear();//清空按键检验
               figure=1;
               flag=1;
           }
           else if(key_state == key_down)
           {
               KEY_Clear();//清空按键检验
               figure=-1;
               flag=1;
           }
           else if(key_state == key_left)
           {
               KEY_Clear();//清空按键检验
               index++;
               if(index>2)
               {
                   index=2;
               }
               flag=2;
           }
           else if(key_state == key_right)
           {
               KEY_Clear();//清空按键检验
               index--;
               if(index<-3)
               {
                   index=-3;
               }
               flag=2;
           }
           else if(key_state == key_enter)
           {
               KEY_Clear();//清空按键检验
               if(*(pobj->pb)==temp)
               {
                   DISP_SSD1306_Print_F6x8(40,7,"Adjusted OK!");
                   SDK_DelayAtLeastUs(500*1000,CLOCK_GetFreq(kCLOCK_CoreSysClk));
                   return;
               }
               else if(*(pobj->pb) != temp)
               {
                   *(pobj->pb) = temp;
                   DISP_SSD1306_Print_F6x8(20,4,"             ");
                   DISP_SSD1306_Printf_F6x8(20,4,"save:%7.3f",*(pobj->pb));
               }
           }
        }

        /*根据上述检验的结果改值*/
        if(flag==1)
        {
            temp=temp+figure*pow(10,index);
            DISP_SSD1306_Printf_F6x8(20,5,"now :%7.3f",temp);
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
            ;//什么都不做
        }
        else
        {
            Menu_Tip_Delay("FloatDataAdjust");
        }
    }
}

void Menu_Calculate_Objnum(void)//计算总共的项数
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
void Menu_Calculate_num(void)//计算各个项的序号
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
void Menu_Calculate_ny(int flag)//计算各个项的纵坐标   0-初始化，1-上滚，-1-下滚
{
    int i;
    for(i=0;obj[i].name[0]!=0;i++)
    {
        if(flag==0)//初始化
        {
            obj[i].ny=obj[i].num;
        }
        else if(flag==1)//上滚（光标向上）
        {
            obj[i].ny++;
        }
        else if(flag==-1)//下滚（光标向下）
        {
            obj[i].ny--;
        }
    }
}

void Menu_DataRead(void)//读取数据
{
    int flag,i=0;
    flag=0;

    uint8_t *buff = (uint8_t*)malloc(sizeof(uint8_t)*4096);
    float DATA[4][20]={0};
    uint8_t Error;
    Error = FLASH_SectorRead(0,buff);
    if(Error==0)
    {
        Menu_Tip_Delay("DataRead");
    }
    memcpy(DATA,&buff[0],4*20*sizeof(float));

    for(i=0;obj[i].name[0]!=0;i++)//把数据转移到obj里
    {
        if(obj[i].type == my_variType)
        {
            *(obj[i].pa) = DATA[flag][i];
        }
        else if(obj[i].type == my_varfType)
        {
            *(obj[i].pb) = DATA[flag][i];
        }
        else if(obj[i].type == my_menuType)
        {
            ;//不管
        }
    }
    free(buff);
}
void Menu_DataSave(void)//保存数据
{
    int flag,i=0;
    flag=0;

    uint8_t *buff = (uint8_t*)malloc(sizeof(uint8_t)*4096);
    float DATA[4][20];
    uint8_t Error;

    for(i=0;obj[i].name[0]!=0;i++)//把数据从obj里转移到DATA里
    {
        if(obj[i].type==0)
        {
            DATA[flag][i] = *(obj[i].pa);
        }
        else if(obj[i].type==1)
        {
            DATA[flag][i] = *(obj[i].pb);
        }
        else if(obj[i].type==2)
        {
            ;//不管
        }
    }
    memcpy(&buff[0],DATA,4*20*sizeof(float));
    Error = FLASH_SectorWrite(0,buff);

    if(Error==0)
    {
        Menu_Tip_Delay("DataSave");
    }
    free(buff);

    DISP_SSD1306_Print_F6x8(40,7,"Flash written!");
    SDK_DelayAtLeastUs(1000*1000,CLOCK_GetFreq(kCLOCK_CoreSysClk));

}
void Menu_Tip(char* str)//左上角提醒
{
    DISP_SSD1306_Print_F6x8(0,0,str);
}
//TODO:设置提示信息缓存区
void Menu_Tip_Delay(char* str)//左上角提醒
{
    DISP_SSD1306_Print_F6x8(0,0,str);
    SDK_DelayAtLeastUs(1000*1000,CLOCK_GetFreq(kCLOCK_CoreSysClk));
}

void KEY_Check_Right(void)
{
    key_state = key_right;
}
void KEY_Check_Enter(void)
{
    key_state = key_enter;
}
void KEY_Check_Up(void)
{
    key_state = key_up;
}
void KEY_Check_Left(void)
{
    key_state = key_left;
}
void KEY_Check_Down(void)
{
    key_state = key_down;
}
void KEY_Clear(void)
{
    key_state = key_gnd;
}

void DISP_SSD1306_Printf_F6x8(uint8_t x,uint8_t y,const char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  static char* buf = new char[64];
  vsnprintf(buf, 64, fmt, args);
  DISP_SSD1306_Print_F6x8(x, y, buf);
}

void DISP_SSD1306_Printf_F8x16(uint8_t x,uint8_t y,const char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  static char* buf = new char[64];
  vsnprintf(buf, 64, fmt, args);
  DISP_SSD1306_Print_F8x16(x, y, buf);
}

void Boma_init(void)
{
    PORT_SetPinInterruptConfig(PORTA, 9U, kPORT_InterruptFallingEdge);
    extInt_t::insert(PORTA, 9U, f91mgj);
    PORT_SetPinInterruptConfig(PORTA, 11U, kPORT_InterruptFallingEdge);
    extInt_t::insert(PORTA, 11U,f91mgj);
    PORT_SetPinInterruptConfig(PORTA, 13U, kPORT_InterruptFallingEdge);
    extInt_t::insert(PORTA, 13U,f91mgj);
    PORT_SetPinInterruptConfig(PORTA, 15U, kPORT_InterruptFallingEdge);
    extInt_t::insert(PORTA, 15U,f91mgj);
}
void KEY_init(void)
{
    PORT_SetPinInterruptConfig(PORTE, 9U, kPORT_InterruptFallingEdge);
    extInt_t::insert(PORTE, 9U,KEY_Check_Left);
    PORT_SetPinInterruptConfig(PORTE, 10U, kPORT_InterruptFallingEdge);
    extInt_t::insert(PORTE, 10U,KEY_Check_Enter);
    PORT_SetPinInterruptConfig(PORTE, 7U, kPORT_InterruptFallingEdge);
    extInt_t::insert(PORTE, 7U,KEY_Check_Down);
    PORT_SetPinInterruptConfig(PORTE, 8U, kPORT_InterruptFallingEdge);
    extInt_t::insert(PORTE, 8U,KEY_Check_Right);
    PORT_SetPinInterruptConfig(PORTE, 11U, kPORT_InterruptFallingEdge);
    extInt_t::insert(PORTE, 11U,KEY_Check_Up);
}


void f91mgj(void)//这个函数只是个凑数的函数指针
{
    /*孙一峰永远是我大哥*/;
}
