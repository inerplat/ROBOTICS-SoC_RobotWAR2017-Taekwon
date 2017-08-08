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
#define TEST_MOTION 2
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

int MCU_process(U16* buf, unsigned char gray[], int *attack_count)
{
	int i, j, cnt = 0, k, l;
	int gray_cnt = 0;
	int sum;
	int first[180] = { 0 }, last[180] = { 0 }, density[180] = { 0 };
	int nodapBlack[180], LeftMax[180], RightMax[180];
	unsigned char maxnodap = 120;
	unsigned char distance;
	unsigned char rgb_max, rgb_min;
	unsigned char r, g, b, R, G, B;
	int hist[256];
	int maxi = 0, maxi_r = 0, maxi_l = 0;
	int blackline = 0;
	int camera_cover = 0;
	int ggray;
	int max;
	int black;
	double S, V;
	int black_cnt;
	memset(nodapBlack, 0, 180 * 4);
	for (i = 0; i < 60; i++)
	{
		for (j = 0; j < 180; j++)
		{
			buf[i * 180 + j] = 0xf81f;
		}
	}

	for (i = 60; i < 120; i++)
	{
		for (j = 0; j < 180; j++)
		{
			ggray = i * 180 + j;
			R = (buf[ggray] & 0xf800) >> 8;
			G = (buf[ggray] & 0x07e0) >> 3;
			B = (buf[ggray] & 0x001f) << 3;
			camera_cover += ((R + G + B) / 3 > 98 )? 0 : 1;
			rgb_max = MAX3(R, G, B);
			rgb_min = MIN3(R, G, B);
			V = rgb_max;
			if (V == 0) S = 0;
			else
			{
				S = 255 * (rgb_max - rgb_min) / (double)V;
			}

			if (S < 55 && V <= 160)
			{
				buf[ggray] = 0;
			}
			else buf[ggray] = 0xffff;
		}
	}

	maxi=0;
	for(i=10;i<170;i++)
	{
	for(j=61;j<120;j++)
	{
	if(buf[j*180+i]==0) ++nodapBlack[i];
	}
	if(nodapBlack[maxi]<nodapBlack[i]) maxi=i;
	}

/*
	for (i = 5; i < 180; i++)
	{
		black_cnt = 0;
    nodapBlack[i] = 59;
		for (j = 115; j >= 60; j--)
		{
			if (buf[j * 180 + i] == 0)
			{
				black_cnt++;
				if (black_cnt > 3)
				{
					nodapBlack[i] = j;
					if (j < maxnodap) maxnodap = j;
					buf[j * 180 + i] = 2016;
					break;
				}
			}
			else
				black_cnt = 0;
		}
	}
	max = 0;
  printf("camera_cover : %d\n", camera_cover);

	LeftMax[4] = nodapBlack[4];
	for (i = 5; i < 175; i++)
	{
		if (nodapBlack[i] < LeftMax[i - 1])
			LeftMax[i] = nodapBlack[i];
		else
			LeftMax[i] = LeftMax[i - 1];
	}
	RightMax[175] = nodapBlack[175];
	for (i = 174; i >= 5; i--)
	{
		if (nodapBlack[i] < RightMax[i + 1])
			RightMax[i] = nodapBlack[i];
		else
			RightMax[i] = RightMax[i + 1];

		if (LeftMax[i] > RightMax[i])
		{
			if (max < nodapBlack[i] - LeftMax[i])
			{
				max = nodapBlack[i] - LeftMax[i];
				maxi = i;
			}
		}
		else
		{
			if (max < nodapBlack[i] - RightMax[i])
			{
				max = nodapBlack[i] - RightMax[i];
				maxi = i;
			}
		}
	}

	for (maxi_l = maxi; maxi_l >= 5 && nodapBlack[maxi] - nodapBlack[maxi_l] < 5; maxi_l--);
	for (maxi_r = maxi; maxi_r < 175 && nodapBlack[maxi] - nodapBlack[maxi_r] < 5; maxi_r++);

	maxi = (maxi_r + maxi_l) / 2;
*/
	blackline = 0;
	for (j = 60; j < 120; j++)
	{
		if (buf[j * 180 + maxi] == 0) blackline++;
	}
	cnt = 0;
	for (j = 60; j < 120; j++)
	{
		buf[j * 180 + maxi] = 63488;
		if (gray[j * 180 + maxi] == 0) ++cnt;
	}

	distance = nodapBlack[maxi];
	///////////////////////////////////////////////////
	if (camera_cover >= 7000)
	{
    DelayLoop(1000);
    *attack_count=0;
		printf("my card is black\n");
    Order_to_Robot(8);
    Order_to_Robot(8);
    Order_to_Robot(8);

    Order_to_Robot(5);
    Order_to_Robot(5);
    Order_to_Robot(5);
    Order_to_Robot(5);

/*  	turn_left();
		turn_left();
		turn_left();
		move_forwardx2();
		move_forwardx2();
		move_forwardx2();
		move_forwardx2();*/
		return 2;
	}
	else if(blackline > 10)
	{
		if (nodapBlack[maxi] < 80)
		{
			if (maxi < 40)
			{
				return DETAIL_TURN_LEFT;
			}
			else if (maxi > 140)
			{
				return DETAIL_TURN_RIGHT;
			}
			else if (distance < 75) //거리가 매우 멀면 2걸음 전진
			{
				return MOVE_FOWARD2;
			}
			else //거리가 멀면 전진
			{
				return MOVE_FOWARD;
			}
		}
		else if(distance <= 110)//거리가 매우 가까우면 콤보공격
		{
			++(*attack_count);
			if (*attack_count % 2) return ATTACK_COMBO1;
			else ATTACK_COMBO2;
		}
    else if(distance > 110 && (*attack_count==13))
    {
      *attack_count=0;
      return BACK_STEP_KICK2;
    }
	}
	else //못찾으면 빠르게회전
	{
		return TURN_LEFT;
	}
}
