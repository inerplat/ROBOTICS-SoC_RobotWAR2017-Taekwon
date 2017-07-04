#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <math.h>
#include "img_analysis.h"

int MCU_analysis(U16* buf)
{
  int i;
  int cnt=0;
  for(i=0;i<180*120*2;i++) if(buf[i]==0) ++cnt;
  if(cnt>180*120*2-50) return 1;
}
