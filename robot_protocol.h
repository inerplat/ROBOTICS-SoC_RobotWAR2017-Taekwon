/* ------------------------------------------------------------------------ */
/*                  KAIST BATTLE ROBOT CONTROL PROGRAM						*/
/*                   														*/
/*          Coding by YKH      												*/
/*          file : robot_protocol.h    										*/
/* ------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------ */
/*                  KAIST BATTLE ROBOT CONTROL PROGRAM            			*/
/*                   														*/
/*          Modified by Goon-Ho Choi, HANOOL ROBOTICS 2003.07.16     		*/
/* ------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------ */
#ifndef __ROBOT_PROTOCOL_H__
#define __ROBOT_PROTOCOL_H__

#define Header0	0xff
#define Header1 0xff
#define Packet	0x09
#define pID	0xfd
#define CMD	0x16
#define DATA1	0x00
///////////////////////////////////////////////////////////////////////////////
void DelayLoop(int delay_time);
void Send_Command(unsigned char CS1, unsigned char CS2, unsigned char DATA0);
unsigned char Receive_Ack(void);
void Motion(unsigned char DATA0);
void init_robot(void);
void F_walk(void);
void B_walk(void);
void Turn_left(void);
void Turn_right(void);
void box(void);
////////////////////////////////////////////////////////////////////////////////

#endif // __ROBOT_PROTOCOL_H__
