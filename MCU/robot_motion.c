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

  uart_config(UART1, 115200, 8, UART_PARNONE, 1);

  switch(motion_number)
  {
    /*
      case 1:
      printf("F_Walk\n");
      F_walk();
      break;

      case 2:
      B_walk();
      break;

      case 3:
      Turn_left();
      break;

      case 4:
      Turn_right();
      break;
      */
  }
	uart_close();
  return;
}
