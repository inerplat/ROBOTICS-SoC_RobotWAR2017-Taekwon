#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <math.h>
#include <memory.h>
#include "img_process.h"
#include "amazon2_sdk.h"
#include "robot_motion.h"
#include "robot_protocol.h"
#define SIZE 180*120
#define RED 0
#define GREEN 1
#define BLUE 2
#define black_score 2
#define white_score -2
#define other_score -1
#define DEFAULT_MOTION 2
#define MOVE_FOWARD 4
#define MOVE_FOWARD2 5
#define MOVE_BACK 6
#define TURN_LEFT 8
#define TURN_RIGHT 9
#define ARM_ATTACK1 11
#define ARM_ATTACK2 12
#define ARM_ATTACK2 13
#define BACK_STEP_KICK1 15
#define BACK_STEP_KICK2 17
#define MOVE_AND_ATTACK1 21
#define MOVE_AND_ATTACK2 22
#define MOVE_AND_ATTACK3 23
#define MOVE_AND_ATTACK4 24
#define DETAIL_TURN_LEFT 34
#define DETAIL_TURN_RIGHT 35
#define ATTACK_COMBO1 37
#define ATTACK_COMBO2 41
#define MIN3(x,y,z)		( (y) <= (z) ? ((x) <= (y) ? (x) : (y)) : ((x) <= (z) ? (x) : (z)) )
#define MAX3(x,y,z)		( (y) >= (z) ? ((x) >= (y) ? (x) : (y)) : ((x) >= (z) ? (x) : (z)) )
int MCU_process(U16* buf, int SUM[], unsigned char image[], unsigned char gray[], int *attack_count)
{
	int i, j, cnt = 0, k, l;
	int gray_cnt = 0;
	int sum;
	int MAX_SUM = 0;
	int max_j_s = 0, max_j_e = 0, max_i_s = 0, max_i_e = 0, j_s, j_e, i_s, i_e;
	double S,V;
	int rgb_max, rgb_min;
	int area;
	int x,y;
	cnt=33480;
	gray_cnt=10980;
	for (i = 62; i < 120; i++)
	{
		for (j = 0; j < 180; j++)
		{
			image[cnt + RED] = (buf[gray_cnt] & 0xf800) >> 8;
			image[cnt + GREEN] = (buf[gray_cnt] & 0x07e0) >> 3;
			image[cnt + BLUE] = (buf[gray_cnt] & 0x001f) << 3;

			rgb_max = MAX3(image[cnt + RED], image[cnt + GREEN], image[cnt + BLUE]);
			rgb_min = MIN3(image[cnt + RED], image[cnt + GREEN], image[cnt + BLUE]);
			V = rgb_max;
			if (V == 0) S = 0;
			else
			{
				S = 255 * (rgb_max - rgb_min) / (double)V;
			}

			if (S < 55 && V <= 160)
			{
				buf[gray_cnt] = 0;
				gray[gray_cnt] =0;
			}

			else if (image[cnt+RED] < 80 && image[cnt + GREEN] < 80 && image[cnt + BLUE] < 80)
			{
				buf[gray_cnt] = 0x0000;
				gray[gray_cnt] = 0;
			}
			else if (image[cnt + RED] > 190 && image[cnt + GREEN] > 190 && image[cnt + BLUE] > 190)
			{
				buf[gray_cnt] = 0xffff;
				gray[gray_cnt] = 255;
			}
			else
			{
				buf[gray_cnt] = 0x7bef;
				gray[gray_cnt] = 128;
			}

			cnt += 3;
			++gray_cnt;
		}
	}

	for (i = 2; i < 180 - 2; i += 2)
	{
		sum = 0;
		for (j = 62; j < 120 - 2; j += 2)
		{
			if (gray[j * 180 + i] == 0)sum += black_score;
			else if (gray[j * 180 + i] == 255)sum += white_score;
			else sum += other_score;

			SUM[j * 180 + i] = sum;
		}
	}

	for (j_s = 62; j_s < 115 - 2; j_s += 2)
	{
		for (j_e = j_s + 4; j_e < 120 - 2; j_e += 2)
		{
			for (i_s = 2; i_s < 180 - 2; i_s += 2)
			{
				sum = 0;
				for (i_e = i_s; i_e < 180 - 2; i_e += 2)
				{
					sum += SUM[j_e * 180 + i_e] - SUM[j_s * 180 + i_e];
					if (MAX_SUM < sum)
					{
						MAX_SUM = sum;
						max_j_s = j_s;
						max_j_e = j_e;
						max_i_s = i_s;
						max_i_e = i_e;
					}

					if (sum < 0)
						break;
				}
				i_s = i_e;
			}
		}
	}

	for (j = max_j_s; j <= max_j_e; j++)
	{
		buf[j * 180 + max_i_s] = 0xf800;
		buf[j * 180 + max_i_e] = 0xf800;
	}

	for (i = max_i_s; i <= max_i_e; i++)
	{
		buf[max_j_s * 180 + i] = 0xf800;
		buf[max_j_e * 180 + i] = 0xf800;
	}
	x=(max_i_s + max_i_e) / 2;
	y=(max_j_s + max_j_e) / 2;
	printf("x : %d, y : %d\n:", x, y);
	area=(max_i_e-max_i_s)*(max_j_e-max_j_s);
	printf("area : %d\n", area);

	if(x<50)
	{
		return DETAIL_TURN_LEFT;
	}
	else if (x > 140)
	{
		return DETAIL_TURN_RIGHT;
	}
	else
	{
		++(*attack_count);
			if (*attack_count % 2) return ATTACK_COMBO1;
			else ATTACK_COMBO2;
	}
}
