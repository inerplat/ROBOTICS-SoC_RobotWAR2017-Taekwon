#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include "robot_motion.h"
#include "robot_protocol.h"
#include "uart_api.h"
#include <termios.h>
static struct termios inittio, newtio;

void init_console(void)
{
    tcgetattr(0, &inittio);
    newtio = inittio;
    newtio.c_lflag &= ~ICANON;
    newtio.c_lflag &= ~ECHO;
    newtio.c_lflag &= ~ISIG;
    newtio.c_cc[VMIN] = 1;
    newtio.c_cc[VTIME] = 0;

    cfsetispeed(&newtio, B115200);

    tcsetattr(0, TCSANOW, &newtio);
}

void Order_to_Robot(int motion_number)
{
  int ret;

  init_console();

  ret = uart_open();
  if (ret < 0) return EXIT_FAILURE;

  uart_config(UART1, 57600, 8, UART_PARNONE, 1);

  switch(motion_number)
  {
	  case 1: printf("test_motion()"); test_motion();				break;
	  case 2: printf("default_motion()"); default_motion();			break;
	  case 3: printf("move_forward()"); move_forward();				break;
	  case 4: printf("move_forwardx2()"); move_forwardx2();			break;
	  case 5: printf("move_back()"); move_back();					break;
	  case 6: printf("turn_left()"); turn_left();					break;
	  case 7: printf("turn_right()"); turn_right();					break;
	  case 8: printf("arm_attack_1()"); arm_attack_1();				break;
	  case 9: printf("arm_attack_2()"); arm_attack_2();				break;
	  case 10: printf("move_and_attack_1()"); move_and_attack_1();	break;
	  case 11: printf("move_and_attack_2()"); move_and_attack_2();	break;
  }
	uart_close();
  return;
}
