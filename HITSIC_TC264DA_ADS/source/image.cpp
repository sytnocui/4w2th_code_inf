/*
 * image.cpp
 *
 *  Created on: 2021��3��3��
 *      Author: 10798
 */

#include "image.hpp"
#include "math.h"

int f[10 * CAMERA_H];//������ͨ����ͨ��

//ÿ������������
typedef struct {
    uint8   left;//��߽�
    uint8   right;//�ұ߽�
    int   connect_num;//��ͨ��ǣ��ţ�
}range;

//ÿ�е����а�����
typedef struct {
    uint8   num;//ÿ�а�������
    range   area[white_num_MAX];//���и���������
}all_range;

//����������ÿ������������
typedef struct {
    uint8   left;//��߽�
    uint8   right;//�ұ߽�
    uint8   width;//���
}road_range;

//ÿ������������ÿ��������
typedef struct {
    uint8   white_num;
    road_range   connected[white_num_MAX];
}road;
typedef struct edge_point {
    int x;
    int y;
} Point;
all_range white_range[CAMERA_H];//���а�����
road my_road[CAMERA_H];//����
uint8 IMG[CAMERA_H][CAMERA_W];//��ֵ����ͼ������
uint8 left_line[CAMERA_H], right_line[CAMERA_H];//���������ұ߽�
uint8 mid_line[CAMERA_H];
int all_connect_num = 0;//���а�������
uint8 top_road;//������ߴ���������
float threshold = 220;//��ֵ
uint8 j_continue[CAMERA_H];//��һ����ͨ·��


uint8 Left_begin = 0;
uint8 Right_begin = 0;
uint8 Left_end = 0;
uint8 Right_end = 0;

uint8 Left_widge = 0;
uint8 Right_widge = 0;
uint8 Left_flag = 0;
uint8 Right_flag = 0;
int start_line_x = 119;

int flagtext = 0;

int flag = 0;
float k_str=0;

bool startline_flag = false;
bool cross_flag = false;

float k_mid = 0;//����б��

int start_line_line = 0;
/*��menuǨ�ƹ�����*/
int prospect = 40; //ǰհ
int img_stop = 100;

int protect_sw = 1;

double process_curvity(uint8 x1, uint8 x2, uint8 y1, uint8 y2, uint8 x3, uint8 y3)
{
    double k;
    int S_of_ABC = ((x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1)) / 2;
    //����ķ��ű�ʾ����
    uint8 q1 = (uint8)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    double AB = sqrt(q1);
    q1 = (uint8)((x3 - x2) * (x3 - x2) + (y3 - y2) * (y3 - y2));
    double BC = sqrt(q1);
    q1 = (uint8)((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1));
    double AC = sqrt(q1);
    if (AB * BC * AC == 0)
    {
        k = 0;
    }
    else
        k = 4.0 * (float)S_of_ABC / (AB * BC * AC);
    return k;
}
////////////////////////////////////////////
//���ܣ���ֵ��
//���룺�Ҷ�ͼƬ
//�������ֵ��ͼƬ
//��ע��
///////////////////////////////////////////
void THRE()
{
    uint8* map;
    uint8* my_map;
    map = fullBuffer;
    for (int i = 0; i < 120; i++)
    {
        my_map = &IMG[i][0];
        for (int j = 0; j < 188; j++)
        {
            if ((*map) > threshold)
                (*my_map) = 255;
            else (*my_map) = 0;
            map++;
            my_map++;
        }
    }
}

////////////////////////////////////////////
//���ܣ�������峵ͷ
//���룺
//�����
//��ע��Ҫ�����Լ���ͷ�Ĵ�С�����޸�
///////////////////////////////////////////
void head_clear(void)
{
//    uint8* my_map;
//    for (int i = 119; i >= 84; i--)
//    {
//        my_map = &IMG[i][0];
//        for (int j = 40; j <= 135; j++)
//        {
//            *(my_map+j) = 255;
//        }
//    }
    uint8* my_map;
        for (int i = 119; i >= 75; i--)
        {
            my_map = &IMG[i][0];
            for (int j = 45+0.6*(119-i); j <= 130-0.6*(77-i); j++)
            {
                *(my_map+j) = 255;
            }
        }
}

////////////////////////////////////////////
//���ܣ����Ҹ��ڵ�
//���룺�ڵ���
//�������������
//��ע����·��ѹ��
///////////////////////////////////////////
int find_f(int node)//���ص��Ǹ��ڵ�
{
    if (f[node] == node)return node;//�ҵ���������ȣ�return
    f[node] = find_f(f[node]);//����Ѱ���Լ��ĸ��ڵ�
    return f[node];
}

////////////////////////////////////////////
//���ܣ���ȡ������ ����ȫ�������ӱ��
//���룺IMG[120][188]
//�����white_range[120]
//��ע��ָ������
///////////////////////////////////////////
void search_white_range()
{
    uint8 i, j;
    int istart = NEAR_LINE;//������ʼ��
    int iend = FAR_LINE;//������ֹ��
    int tnum = 0;//��ǰ�а�����
    all_connect_num = 0;//������ų�ʼ��
    uint8* map = NULL;
    for (i = istart; i >= iend; i--)
    {
        map = &IMG[i][LEFT_SIDE];//ָ�����߼ӿ�����ٶ�
        tnum = 0;
        for (j = LEFT_SIDE; j <= RIGHT_SIDE; j++, map++)
        {
            if ((*map))//��������߽�
            {
                tnum++;
                if (tnum >= white_num_MAX)break;
                range* now_white = &white_range[i].area[tnum];
                now_white->left = j;

                //��ʼ���һ��һ�����ص�����������ұ߽�
                map++;
                j++;

                while ((*map) && j <= RIGHT_SIDE)
                {
                    map++;
                    j++;
                }
                now_white->right = j - 1;
                now_white->connect_num = ++all_connect_num;//��������һ��������������
            }
        }
        white_range[i].num = tnum;
    }
}

////////////////////////////////////////////
//���ܣ�Ѱ�Ұ�������ͨ�ԣ���ȫ����ͨ�����ӵĽڵ���ˢ����������ȵĽڵ���
//���룺
//�����
//��ע��
///////////////////////////////////////////
void find_all_connect()
{
    //f�����ʼ��
    for (int i = 1; i <= all_connect_num; i++)
        f[i] = i;

    //uΪup dΪdown ��Ϊ��ǰ������������е��������к���������
    //u_num�������а�����
    //u_left�������е�ǰ������߽�
    //u_right�������е�ǰ�����ұ߽�
    //i_u����ǰ�������������ǵ�ǰ���У������У������еĵ�i_u��
    int u_num, i_u, u_left, u_right;
    int d_num, i_d, d_left, d_right;
    all_range* u_white = NULL;
    all_range* d_white = NULL;
    for (int i = NEAR_LINE; i > FAR_LINE; i--)//��Ϊÿ����ÿ���бȽ� ����ѭ����FAR_LINE+1
    {
        u_num = white_range[i - 1].num;
        d_num = white_range[i].num;
        u_white = &white_range[i - 1];
        d_white = &white_range[i];
        i_u = 1; i_d = 1;

        //ѭ������ǰ�л������а��������ľ�Ϊֹ
        while (i_u <= u_num && i_d <= d_num)
        {
            //�����ȱ��棬�����������д�������ҷ���Ч�ʵ�
            u_left = u_white->area[i_u].left;
            u_right = u_white->area[i_u].right;
            d_left = d_white->area[i_d].left;
            d_right = d_white->area[i_d].right;

            if (u_left <= d_right && u_right >= d_left)//�������������ͨ
                f[find_f(u_white->area[i_u].connect_num)] = find_f(d_white->area[i_d].connect_num);//���ڵ�������

            //��ǰ�㷨��������һ�����֪��Ϊɶ������
            if (d_right > u_right)i_u++;
            if (d_right < u_right)i_d++;
            if (d_right == u_right) { i_u++; i_d++; }
        }
    }
}

////////////////////////////////////////////
//���ܣ�Ѱ������
//���룺
//�����
//��ע��
///////////////////////////////////////////
void find_road()
{
    uint8 istart = NEAR_LINE;
    uint8 iend = FAR_LINE;
    top_road = NEAR_LINE;//������ߴ������������ȳ�ʼ����Ϊ��ʹ�
    int road_f = -1;//����������ͨ�򸸽ڵ��ţ��ȳ�ʼ��Ϊ-1�����ж��Ƿ��ҵ�����
    int while_range_num = 0, roud_while_range_num = 0;
    all_range* twhite_range = NULL;
    road* tmy_road = NULL;
    //Ѱ������������ͨ��
    // Ѱ�������ĵİ�����
    for (int i = 1; i <= white_range[istart].num; i++)
        if (white_range[istart].area[i].left <= CAMERA_W / 2
            && white_range[istart].area[i].right >= CAMERA_W / 2 && (white_range[istart].area[i].right - white_range[istart].area[i].left) >= 90)
            road_f = find_f(white_range[istart].area[i].connect_num);

    if (road_f == -1)//������û���м䣬��113��ѡһ�������Ϊ���������
    {
        int widthmax = 0, jselect = 1;
        for (int i = 1; i <= white_range[istart].num; i++)
            if (white_range[istart].area[i].right - white_range[istart].area[i].left > widthmax)
            {
                widthmax = white_range[istart].area[i].right - white_range[istart].area[i].left;
                jselect = i;
            }
        road_f = find_f(white_range[istart].area[jselect].connect_num);
    }

    //���������Ѿ��õ�������������ͨ�򸸽ڵ��ţ������������и��ڵ�����road_f�����а������ӽ��������������
    for (int i = istart; i >= iend; i--)
    {
        //�������棬����֮��д�������ҵ�Ч
        twhite_range = &white_range[i];
        tmy_road = &my_road[i];
        while_range_num = twhite_range->num;
        tmy_road->white_num = 0;
        roud_while_range_num = 0;
        for (int j = 1; j <= while_range_num; j++)
        {
            if (find_f(twhite_range->area[j].connect_num) == road_f)
            {
                top_road = i;
                tmy_road->white_num++; roud_while_range_num++;
                tmy_road->connected[roud_while_range_num].left = twhite_range->area[j].left;
                tmy_road->connected[roud_while_range_num].right = twhite_range->area[j].right;
                tmy_road->connected[roud_while_range_num].width = twhite_range->area[j].right - twhite_range->area[j].left;

            }
        }
    }
}

////////////////////////////////////////////
//���ܣ�����������һ�а����ӱ��
//���룺i_start��ʼ��  j_start�������
//������������
//��ע����Ϊ��һ���뱾�������ص����ֶԶ�İ���Ϊѡ������
///////////////////////////////////////////
uint8 find_continue(uint8 i_start, uint8 j_start)
{
    uint8 j_return;
    uint8 j;
    uint8 width_max = 0;
    uint8 width_new = 0;
    uint8 left = 0;
    uint8 right = 0;
    uint8 dright, dleft, uright, uleft;
    j_return = MISS;//���û�ҵ������255
    if (j_start > my_road[i_start].white_num)
        return MISS;
    //ѡһ���ص�����
    for (j = 1; j <= my_road[i_start - 1].white_num; j++)
    {
        dleft = my_road[i_start].connected[j_start].left;
        dright = my_road[i_start].connected[j_start].right;
        uleft = my_road[i_start - 1].connected[j].left;
        uright = my_road[i_start - 1].connected[j].right;
        if (//����
            dleft < uright
            &&
            dright > uleft
            )
        {
            //�����ص���С
            if (dleft < uleft) left = uleft;
            else left = dleft;

            if (dright > uright) right = uright;
            else right = dright;

            width_new = right - left + 1;

            if (width_new > width_max)
            {
                width_max = width_new;
                j_return = j;
            }
        }

    }
    return j_return;
}

////////////////////////////////////////////
//���ܣ�ͨ�þ���˫��
//���룺
//�����
//��ע��
///////////////////////////////////////////
void ordinary_two_line(void)
{
    uint8 i;
    uint8 j;

    uint8 a, b,c;

    uint8 i_start;
    uint8 i_end;
    uint8 j_start = MISS;
    int width_max;
    i_start = NEAR_LINE;
    i_end = FAR_LINE;
    width_max = 0;

    /*Ѱ�ҵ�һ�����*/
    for (j = 1; j <= my_road[i_start].white_num; j++)
    {
        if (my_road[i_start].connected[j].width > width_max)
        {
            width_max = my_road[i_start].connected[j].width;
            j_start = j;
        }
    }
    j_continue[i_start] = j_start;

    /*Ѱ���ص����*/
    for (i = i_start; i > i_end; i--)
    {
        if (j_continue[i] > my_road[i].white_num)
        {
            j_continue[i - 1] = MISS;
        }
        else
        {
            j_continue[i - 1] = find_continue(i, j_continue[i]);
        }

    }
    my_memset(left_line, MISS, CAMERA_H);
    my_memset(right_line, MISS, CAMERA_H);

    Left_flag = 0;
    Right_flag = 0;
    flag = 0;
    flagtext = 0;
    start_line_x = 0;

    /*��ʮ��*/
    /*if ((my_road[i_start - 5].connected[j_continue[i_start - 5]].width > 150) && (my_road[i_start - 8].connected[j_continue[i_start - 8]].width > 150))
    {
        Cross_out();
    }*/

    double k0;

    /*��ѭ��*/
    for (i = i_start; i > i_end; i--)
    {
        /*ʮ�����ñ���*/
        double k_left = (my_road[i].connected[j_continue[i]].left - my_road[i - 2].connected[j_continue[i - 2]].left) / 2.0;
        double k_left_pre = (my_road[i - 2].connected[j_continue[i - 2]].left - my_road[i - 4].connected[j_continue[i - 4]].left) / 2.0;
        double k_right = (my_road[i].connected[j_continue[i]].right - my_road[i - 2].connected[j_continue[i - 2]].right) / 2.0;
        double k_right_pre = (my_road[i - 2].connected[j_continue[i - 2]].right - my_road[i - 4].connected[j_continue[i - 4]].right) / 2.0;

        if (fabs(k_left_pre) - fabs(k_left) > 2.0) {
            Left_flag = 1;
            Left_begin = i;
        }
        if (fabs(k_right_pre) - fabs(k_right) > 2.0) {
            Right_flag = 1;
            Right_begin = i;
        }

        /*���ұ߽�*/
        if ((j_continue[i] <= my_road[i].white_num))
        {
            left_line[i] = my_road[i].connected[j_continue[i]].left;
            right_line[i] = my_road[i].connected[j_continue[i]].right;
        }
        else
        {
            left_line[i] = MISS;
            right_line[i] = MISS;
        }

        /*�жϸ��ӵ���*/
        if (i >= 30)
        {
            /*������*/
            b = my_road[i - 20].connected[j_continue[i - 20]].left;
            c = my_road[i - 30].connected[j_continue[i - 30]].left;
            a = my_road[i].connected[j_continue[i]].left;
            k0 = process_curvity(i, i - 20, a, b, i - 30, c);

            /*ʶ�������*/
            if (my_road[i - 5].connected[j_continue[i - 5]].width < 20 && my_road[i - 8].connected[j_continue[i - 8]].width < 10 && i>40 && fabs(k0) < 0.1)
            {
                start_line_x = i;

                Start_line();
                startline_flag = true;//�����߱�־λ
                break;
            }
            /*ʶ��ʮ��*/
            else if (Right_flag == 1 && Left_flag == 1 && i > 45 && i < 105)
            {
                Cross();
                cross_flag= true;//ʮ�ֱ�־λ
                break;
            }
            /*if (Right_flag == 1 && Left_flag == 1 && i >= 105) {
                Cross_out();
                break;
            }*/
        }
    }
}

void Cross(void) {
    Point Left_END, Right_END;//��˵㣬�Ҷ˵�
    int mid_pre = (my_road[Left_begin].connected[j_continue[Left_begin]].left + my_road[Right_begin].connected[j_continue[Right_begin]].right) / 2;//Ԥ������
    int count = 0;

    Left_end = 0;
    Right_end = 0;
    Left_END.x = 0;
    Left_END.y = 0;
    Right_END.x = 0;
    Right_END.y = 0;

    int t=0;
    float k4;
    if (Left_begin >= 30)
    {
        for (int i = Left_begin - 30; i > 2; i--)
        {
            if(Left_begin - NEAR_LINE!=0)
            {
                k4 = -((left_line[Left_begin] + right_line[Right_begin]) - (left_line[NEAR_LINE] + right_line[NEAR_LINE])) * 2.0 / (Left_begin - NEAR_LINE)
                    / 4.0;
                t = mid_pre - k4 * (i - Left_begin + 30);    //�е��ʼֵ
            }
            else{
                t=mid_pre;
            }


            for (int j = t; j > my_road[Left_begin].connected[j_continue[Left_begin]].left - 5; j--)
            {    //Ѱ����˵�
                if (IMG[i][j] == black)
                {
                    Left_end = 1;
                    Left_END.x = i;
                    Left_END.y = j;
                    break;
                }
            }
            for (int j = t;  j < my_road[Right_begin].connected[j_continue[Right_begin]].right + 5; j++)
            {    //Ѱ���Ҷ˵�
                if (IMG[i][j] == black)
                {
                    Right_end = 1;
                    Right_END.x = i;
                    Right_END.y = j;
                    break;
                }
            }
            if (Right_end == 1 && Left_end == 1)
            {
                count++;
            }

            if (count == 3)
            {
                for (int i = Left_END.y; i < Right_END.y; i++)
                {
                    IMG[Right_END.x][i] = red;
                }
                break;
            }
        }
    }
    float k_left = (float) (Left_END.y - my_road[Left_begin].connected[j_continue[Left_begin]].left) / (Left_END.x - Left_begin);/*��������������*/
    for (int i = Left_begin; i > Left_END.x; i--)
    {
        left_line[i] = (i - Left_begin) * k_left + my_road[Left_begin].connected[j_continue[Left_begin]].left;
    }
    float k_right = (float) (Right_END.y - my_road[Right_begin].connected[j_continue[Right_begin]].right) / (Right_END.x - Right_begin);
    for (int i = Right_begin; i > Right_END.x; i--)
    {
        right_line[i] = (i - Right_begin) * k_right + my_road[Right_begin].connected[j_continue[Right_begin]].right;
    }
    for (int i = Left_END.x; i > 2; i--)
    {
        int flag_left = 0;
        for (int j = Left_END.y + 5; j > Left_END.y - 5; j--)
        {

            if (IMG[i][j] == black && IMG[i][j + 1] == white)
            {
                left_line[i] = j;
                flag_left = 1;
                break;
            }

        }
        if (flag_left == 0)
        {
            left_line[i] = MISS;
        }
        int flag_right = 0;
        for (int j = Right_END.y - 5; j < Right_END.y + 5; j++)
        {

            if (IMG[i][j] == black && IMG[i][j - 1] == white)
            {
                right_line[i] = j;
                flag_right = 1;

                break;
            }
        }
        if (flag_right == 0) right_line[i] = MISS;
    }

}
void Start_line(void)
{
//    Beep_Open();
    uint8* my_map;
    my_map = &IMG[start_line_x - 5][0];
    start_line_line = start_line_x;
    for (int j = 187; j > 0; j--)
    {
        *(my_map + j) = green;
    }

    for (int i = start_line_x - 30; i > 30; i--)
    {
        if ((j_continue[i] <= my_road[i].white_num))
        {
            left_line[i] = my_road[i].connected[j_continue[i]].left;
            right_line[i] = my_road[i].connected[j_continue[i]].right;
        }
        else
        {
            left_line[i] = MISS;
            right_line[i] = MISS;
        }
    }
    float k_left = (float)(my_road[start_line_x].connected[j_continue[start_line_x]].left - my_road[start_line_x - 30].connected[j_continue[start_line_x - 30]].left) / (30.0);
        for (int i = start_line_x; i > start_line_x - 30; i--) {
            left_line[i] = (i - start_line_x) * k_left + my_road[start_line_x].connected[j_continue[start_line_x]].left;
        }
        float k_right = (float)(my_road[start_line_x].connected[j_continue[start_line_x]].right - my_road[start_line_x - 30].connected[j_continue[start_line_x - 30]].right) / (30.0);
    for (int i = start_line_x; i > start_line_x - 30; i--) {
        right_line[i] = (i - start_line_x) * k_right + my_road[start_line_x].connected[j_continue[start_line_x]].right;
    }
}
////////////////////////////////////////////
//���ܣ������ʼ��
//���룺uint8* ptr �����׵�ַ, uint8 num��ʼ����ֵ, uint8 size�����С
//�����
//��ע����Ϊk66������Ϊmemset��������ȫ�������޷�ʹ�ã������Ҫ�Լ�дһ��my_memset
///////////////////////////////////////////
void my_memset(uint8* ptr, uint8 num, uint8 size)
{
    uint8* p = ptr;
    uint8 my_num = num;
    uint8 Size = size;
    for (int i = 0; i < Size; i++, p++)
    {
        *p = my_num;
    }
}
////////////////////////////////////////////
//���ܣ����ߺϳ�
//���룺���ұ߽�
//���������
//��ע��
///////////////////////////////////////////
void get_mid_line(void)
{
    my_memset(mid_line, MISS, CAMERA_H);
    for(int i = NEAR_LINE;i >= FAR_LINE;i--)
        if (left_line[i] != MISS)
        {
            mid_line[i] = (left_line[i] + right_line[i]) / 2;
        }
        else
        {
            mid_line[i] = MISS;
        }


}

void k_mid_calculate()
{
    k_mid = (mid_line[prospect] - 94) / 20;//mid_line[prospect+20]
}

////////////////////////////////////////////
//���ܣ�ͼ����������
//���룺
//�����
//��ע��
///////////////////////////////////////////
void image_main()
{
    image_flag_clear();
    THRE();
//    head_clear();
    search_white_range();
    find_all_connect();
    find_road();
    /*���˴�Ϊֹ�������Ѿ��õ������������Ľṹ������my_road[CAMERA_H]*/
    ordinary_two_line();
    get_mid_line();
    /*б�ʼ���*/
    k_mid_calculate();

    /*ʹ��������Ϊ��ɫ*/
    for (int i = NEAR_LINE; i >= FAR_LINE; i--)
    {
        if (mid_line[i] != MISS)
                    IMG[i][mid_line[i]] = 0;
    }

    /*ʹ��Ļ����Ϊ��ɫ*/
    for(int i = 0;i<113;i++)
    {
        IMG[i][94] = 0;
    }
    /*ǰհΪ��ɫ*/
    for(int i =0;i<187;i++)
    {
        IMG[prospect][i] = 0;
    }
}

void image_flag_clear(void)
{
    cross_flag = false;
    startline_flag = false;
}


