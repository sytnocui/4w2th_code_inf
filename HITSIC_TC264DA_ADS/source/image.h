/*
 * image.hpp
 *
 *  Created on: 2021年3月3日
 *      Author: 10798
 */

#ifndef SOURCE_IMAGE_H_
#define SOURCE_IMAGE_H_

#include "common.h"

#define MISS 255
#define CAMERA_H  120
#define CAMERA_W  188
#define FAR_LINE 1
#define NEAR_LINE 113
#define LEFT_SIDE 0
#define RIGHT_SIDE 187
#define MISS 255
#define white_num_MAX 10

/////////////////////////////
#define black 0
#define white 1
#define blue  2
#define green 3
#define red   4
#define gray  5
#define purple 6
///////////////////////////


extern uint8 IMG[CAMERA_H][CAMERA_W];
extern uint8 image_Buffer_0[CAMERA_H][CAMERA_W];
extern uint8* fullBuffer;
extern int start_line_line;
extern int start_line_x;

extern int prospect ;
extern int img_stop;

extern boolean startline_flag;
extern boolean cross_flag;

extern float k_str;
extern float k_mid;

extern int protect_sw;

double process_curvity(uint8 x1, uint8 x2, uint8 y1, uint8 y2, uint8 x3, uint8 y3);
void head_clear(void);
void THRE(void);
int find_f(int a);
void search_white_range();
void find_all_connect();
void find_road();
uint8 find_continue(uint8 i_start, uint8 j_start);
void ordinary_two_line(void);
void image_main();
void get_mid_line(void);
void Start_line(void);
void Cross(void);
void k_mid_calculate();//斜率计算
extern uint8 mid_line[CAMERA_H];
extern float threshold;
extern int all_connect_num;
extern uint8 top_road;

extern float k_str;

void my_memset(uint8* ptr, uint8 num, uint8 size);

void image_flag_clear(void);



#endif /* SOURCE_IMAGE_H_ */
