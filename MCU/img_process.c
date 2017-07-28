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
#define SIZE 180*120
#define RED 0
#define GREEN 1
#define BLUE 2

int MCU_process(U16* buf, unsigned char image[], unsigned char gray[], int histcnt)
{
  int i, j, cnt=0, k, l;
  static int hiscnt=0;
  int gray_cnt=0;
  int sum;
  double alpha, beta;
  int first[180]={0}, last[180]={0}, density[180]={0};
  int avg;
  int vari;
  int max;
  int maxi=15;
  cnt=gray_cnt=0;
  for(i=0;i<120;i++)
  {
    for(j=0;j<180;j++)
    {
      image[cnt+RED]=(buf[gray_cnt]&0xf800)>>8;
      image[cnt+GREEN]=(buf[gray_cnt]&0x07e0)>>3;
      image[cnt+BLUE]=(buf[gray_cnt]&0x001f)<<3;
      avg=(image[cnt+RED]+image[cnt+GREEN]+image[cnt+BLUE])/3;
      gray[gray_cnt]=(avg>128 ? 255 : 0);
      vari=(image[cnt+RED]-avg)*(image[cnt+RED]-avg)+(image[cnt+GREEN]-avg)*(image[cnt+GREEN]-avg)+(image[cnt+BLUE]-avg)*(image[cnt+BLUE]-avg);
      if(avg<65) buf[gray_cnt]=0;
      if(vari<155 && avg<145 && avg>65)
      {
        buf[gray_cnt]=2016;
    /*    if(avg<45)
        {
          buf[gray_cnt]=2017;
          if(buf[(i-1)*180+j] == 2016 || buf[(i+1)*180+j] == 2016 || buf[i*180+j+1] == 2016 || buf[i*180+j-1] == 2016) buf[gray_cnt]=2016;
        }*/
        if(first[j]==0) first[j]=i;
        last[j]=i;
      }

      ++gray_cnt;
      cnt+=3;
    }
  }
  for(i=0;i<180;i++)
  {
    density[i]=0;
    for(j=first[i];j<=last[i];j++)
    {
      if(buf[j*180+i]==2016) density[i]+=5;
      else if(!buf[j*180+i]) density[i]++;
    }
    if(density[i] >> 2 >(last[i]-first[i])*0.3)
    {
      if(last[maxi]-first[maxi]<last[i]-first[i]) maxi=i;

    }
  }
  cnt=0;
  for(j=first[maxi];j<last[maxi];j++)
  {
    buf[j*180+maxi]=63488;
    if(gray[j*180+maxi]==0) ++cnt;
  }

  if(cnt>=100) printf("nyan nyan punch\n");
  else if(cnt>=90) printf("assa kick\n");
  else if(cnt>=30) printf("go\n");
  else printf("ssibal\n");
  max=0;
  gray_cnt=0;
  cnt=0;
  /*
  for(i=0;i<120;i++)
  {
    for(j=0;j<180;j++)
    {
      buf[gray_cnt]=((gray[gray_cnt]>>3)<<11)+((gray[gray_cnt]>>2)<<5)+(gray[gray_cnt]>>3);
      ++gray_cnt;
      cnt+=3;
    }
  }
*/

}
