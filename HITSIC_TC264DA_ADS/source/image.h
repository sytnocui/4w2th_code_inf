/*
 * image.hpp
 *
 *  Created on: 2021年3月3日
 *      Author: 10798
 */

#ifndef SOURCE_IMAGE_H_
#define SOURCE_IMAGE_H_

#include "common.h"

#include "SmartCar_MT9V034.h"

#define EPS 1e-9

/*ͼ���С�궨��*/
#define MISS 255
#define CAMERA_H  120                            //ͼƬ�߶�
#define CAMERA_W  188                            //ͼƬ���
#define FAR_LINE 1//ͼ�����ϱ߽�
#define NEAR_LINE 113//ͼ�����±߽�
#define LEFT_SIDE 0//ͼ������߽�
#define RIGHT_SIDE 187//ͼ�����ұ߽�
#define MISS 255
#define white_num_MAX 10//ÿ��������������
#define cut_line 27
#define num_all (120*188)



#define CHANGED_H 120
#define CHANGED_W 188
/**/
/////////////////////////////
/*��ɫ�궨��*/
#define black 0
#define white 1
#define blue  2
#define green 3
#define red   4
#define gray  5
#define purple 6
///////////////////////////
/*�������ͺ궨��*/
#define trident_flag 1
#define near_cross_flag 2
#define far_cross_flag 3
#define mid_cross_flag 4
#define straight_flag 5
#define near_circle_flag 6
#define oblique_cross_flag 7
#define zebra_flag 8

/////////////////////////////////////
#define MID_MAX 200
#define gap 5
typedef struct
{
    int trident;
    int cross;
    int straight;
    int circle;
    int zebra;
}MOOD;

typedef enum
{
    img_trident,
    img_near_cross,
    img_far_cross,
    img_mid_cross,
    img_straight,
    img_near_circle,
    img_oblique_cross,
    img_zebra,
    img_enter_circle,
}IMG_STATE;




/// /////////////////////////////

extern uint8 IMG[CAMERA_H][CAMERA_W];
extern uint8 image_Buffer_0[CAMERA_H][CAMERA_W];
extern uint8 img_original[CHANGED_H][CHANGED_W];
extern uint8* fullBuffer;ַ
extern int threshold_original;
extern uint8 last_mid_line[CAMERA_H];
extern const uint8 left_side[CHANGED_H];
extern const uint8 right_side[CHANGED_H];
extern const uint8 i_fix[CHANGED_H];
extern const uint8 j_fix[CHANGED_H][CHANGED_W];
//extern uint8 threshold;

extern boolean cross_flag;
extern uint8 mid_line[CAMERA_H];
extern uint8 startline_flag;
extern int prospect;
extern int near_prospect;
extern int far_prospect;

extern float img_angle;
extern int threshold;


extern int circle_enter_flag;

extern int img_state;
extern int img_state_pre;

void head_clear(void);
void THRE(int threshold);
int find_f(int node);
void search_white_range();
void find_all_connect();
void find_road();
uint8 find_continue(uint8 i_start, uint8 j_start);
void ordinary_two_line(void);
void image_main();
void get_mid_line(void);
IMG_STATE define_road_type(void);
void my_memset(uint8* ptr, uint8 num, uint8 size);
void my_init(void);
void two_line(void);
void find_x_right_left(void);
uint8 define_my_way(uint8 line);
//void vague_OSTU(void);
float define_PROSPECT(uint8 PROSPECT);
/*���������жϺ���*/
uint8 trident(void);
//uint8 cross(void);
uint8 near_circle(void);
uint8 enter_circle(void);
uint8 mid_cross(void);
uint8 tell_far_cross(void);
uint8 tell_near_cross(void);
uint8 tell_mid_cross(void);
uint8 tell_oblique_cross(void);
uint8 tell_zebra(void);

/*��֪���ͺ�Ĵ�����*/
void straight_oridinary_two_line(void);
void trident_ordinary_two_line(void);
void find_my_road(void);
uint8 judge_sign(float k1, float k2);
uint8 define_straight_line(uint8* line, uint8 start_line, uint8 end_line, uint8 dirction);
void filter_two_line(void);
void if_connect(void);
void if_smooth(void);
void find_mid(void);
void mid_cross_oridinary_two_line(void);
void near_cross_oridinary_two_line(void);
void far_cross_oridinary_two_line(void);
void oblique_cross_ordinary_two_line(void);
void zebra_ordinary_two_line(void);
void circle_ordinary_two_line(void);


//void Cross_out();
void Start_line(void);
void Distortion(void);
int thre1();
void map(void);
float regression(uint8 startline, uint8 endline, int type);
double process_curvity(uint8 x1, uint8 x2, uint8 y1, uint8 y2, uint8 x3, uint8 y3);
/*int find_point(uint8 start_point, uint8 end_point);
float process_curvity(uint8 x1, uint8 x2, uint8 y1, uint8 y2, uint8 x3, uint8 y3);
void LeastSquareCalc_Curvent(int startline, int endline, int type);
void regression(uint8 startline, uint8 endline, int type);

uint8 Maxium(uint8 m, uint8 n);
void my_memset(uint8* ptr, uint8 num, uint8 size);*/

#endif /* SOURCE_IMAGE_H_ */
