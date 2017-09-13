/************************************************************************
  Title     : Robot Body Protocol Source File
  File name : robot_protocol.c

  Author    : adc inc. (oxyang@adc.co.kr)
  History
		+ v0.0  2007/2/14
		+ v1.0  2008/8/6
************************************************************************/
#include <stdio.h>
#include <string.h>
#include "robot_protocol.h"
#include "uart_api.h"
//////////////////////////////////////////////////// Protocol Test

void DelayLoop(int delay_time)
{
	//AMAZON-II = 166MHz
	// ms 단위로 카운트 하도록 제작
	delay_time += 300; // 모션 간 추가 딜레이 (모션 씹힘방지)
	delay_time *= 1660;

	while(delay_time)
		delay_time--;
}

void Send_Command(unsigned char Ldata, unsigned char Ldata1)
{
	unsigned char Command_Buffer[6] = {0,};

	Command_Buffer[0] = START_CODE;	// Start Byte -> 0xff
	Command_Buffer[1] = START_CODE1; // Start Byte1 -> 0x55
    Command_Buffer[2] = Ldata;
	Command_Buffer[3] = Ldata1;
	Command_Buffer[4] = Hdata;  // 0x00
	Command_Buffer[5] = Hdata1; // 0xff

	uart1_buffer_write(Command_Buffer, 6);
}

#define ERROR	0
#define OK	1
void test_motion()
{
	printf("\nmotion started\n");
	Send_Command(0x02, 0xfd);
	DelayLoop(504); // 504ms
	printf("motion end\n");
}

void default_motion()
{
	printf("\nmotion started\n");
	Send_Command(0x02, 0xfd);
	DelayLoop(504); // 504ms
	printf("motion end\n");
}

void move_forward() // 앞으로 한걸음
{
	printf("\nmotion started\n");
	Send_Command(0x04, 0xfb);
	DelayLoop(277);
	printf("motion end\n");
}

void move_forwardx2() // 앞으로 연속 두걸음
{
	printf("\nmotion started\n");
	Send_Command(0x05, 0xfa);
	DelayLoop(576); // 785ms
	printf("motion end\n");
}
void move_back() // 뒤로 한걸음
{
	printf("\nmotion started\n");
	Send_Command(0x06, 0xf9);
	DelayLoop(277);
	printf("motion end\n");
}

void turn_left()// 왼쪽으로 30도 회전
{
	printf("\nmotion started\n");
	Send_Command(0x08, 0xf7);
	DelayLoop(448);
	printf("motion end\n");
}
void turn_right() // 오른쪽으로 30도 회전
{
	printf("\nmotion started\n");
	Send_Command(0x09, 0xf6);
	DelayLoop(448);
	printf("motion end\n");
}

void arm_attack_1() // 찌르기
{
	Send_Command(0x0b, 0xf4);
	DelayLoop(640);
	DelayLoop(815);
}
void arm_attack_2() // 싸닥
{
	Send_Command(0x0c, 0xf3);
	DelayLoop(689);
	DelayLoop(815);
}
void arm_attack_3() // 대각선 앞 위로
{
	Send_Command(0x0d, 0xf2);
	DelayLoop(728);
	DelayLoop(815);
}
void arm_attack_4() // 뚝배기
{
	Send_Command(0x0e, 0xf1);
	DelayLoop(2028);
	DelayLoop(815);
}
void back_step_kick_1() // 뒤로 회전 후 뒷발 뻗기
{
	Send_Command(0x0f, 0xf0);
	DelayLoop(476);
	DelayLoop(1053);
}

void back_step_kick_2() // 뒤로 회전 후 뒷발 뻗고 2연속 킥
{
	Send_Command(0x11, 0xee);
	DelayLoop(498);
	DelayLoop(1190);
	DelayLoop(744);
}

void move_and_attack_1() // 전진하면서 싸닥
{
	Send_Command(0x15, 0xea);
	DelayLoop(1110);
}
void move_and_attack_2() // 전진하면서 찌르기
{
	Send_Command(0x16, 0xe9);
	DelayLoop(720);
}
void move_and_attack_3()
{
	Send_Command(0x17, 0xe8);
	DelayLoop(1247);
}
void move_and_attack_4()
{
	Send_Command(0x18, 0xe7);
	DelayLoop(792);
}

void detail_turn_left()	// 왼쪽으로 약간 회전
{
	Send_Command(0x1f, 0xe0);
	DelayLoop(166);
}
void detail_turn_right() // 오른쪽으로 약간 회전
{
	Send_Command(0x20, 0xdf);
	DelayLoop(166);
}

void attack_combo_1() // 앞으로 이동하면서 찌르기 후 싸닥 한뒤 뒤로 이동
{
	Send_Command(0x25, 0xda);
	DelayLoop(804);
	DelayLoop(892);
	DelayLoop(681);
}
void attack_combo_2() // 앞으로 이동하면서 싸닥 후 머리를 향해 찌르기 공격 후 뒤로 이동
{
	Send_Command(0x29, 0xd6);
	DelayLoop(618);
	DelayLoop(774);
	DelayLoop(331);
}
