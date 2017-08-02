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
	// ms ������ ī��Ʈ �ϵ��� ����
	delay_time += 300; // ��� �� �߰� ������ (��� ��������)
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

void move_forward() // ������ �Ѱ���
{
	printf("\nmotion started\n");
	Send_Command(0x04, 0xfb);
	DelayLoop(378);
	printf("motion end\n");
}

void move_forwardx2() // ������ ���� �ΰ���
{
	printf("\nmotion started\n");
	Send_Command(0x05, 0xfa);
	DelayLoop(785); // 785ms
	printf("motion end\n");
}
void move_back() // �ڷ� �Ѱ���
{
	printf("\nmotion started\n");
	Send_Command(0x06, 0xf9);
	DelayLoop(378);
	printf("motion end\n");
}

void turn_left()// �������� 30�� ȸ��
{
	printf("\nmotion started\n");
	Send_Command(0x08, 0xf7);
	DelayLoop(896);
	printf("motion end\n");
}
void turn_right() // ���������� 30�� ȸ��
{
	printf("\nmotion started\n");
	Send_Command(0x09, 0xf6);
	DelayLoop(896);
	printf("motion end\n");
}

void arm_attack_1() // ���
{
	Send_Command(0x0b, 0xf4);
	DelayLoop(976);
}
void arm_attack_2() // �δ�
{
	Send_Command(0x0c, 0xf3);
	DelayLoop(633);
}

void move_and_attack_1() // �����ϸ鼭 ���
{
	Send_Command(0x15, 0xea);
	DelayLoop(794);
}
void move_and_attack_2() // �����ϸ鼭 �δ�
{
	Send_Command(0x16, 0xe9);
	DelayLoop(720);
}