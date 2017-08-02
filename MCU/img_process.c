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
  int gray_cnt=0;
  int sum;
  double alpha, beta;
  int first[180]={0}, last[180]={0}, density[180]={0};
  int nodapBlack[180], LeftMax[180],RightMax[180];
  int hist[256];
  int Tmax;
  double VarMax;
  int maxi=0;
int max;
  cnt=gray_cnt=0;
  memset(hist,0,256*sizeof(unsigned char));
  for(i=0;i<120;i++)
  {
    for(j=0;j<180;j++)
    {
      image[cnt+RED]=(buf[gray_cnt]&0xf800)>>8;
      image[cnt+GREEN]=(buf[gray_cnt]&0x07e0)>>3;
      image[cnt+BLUE]=(buf[gray_cnt]&0x001f)<<3;
      gray[gray_cnt]=(image[cnt+RED]+image[cnt+GREEN]+image[cnt+BLUE])/3;
      if(gray[gray_cnt]>128) gray[gray_cnt]=255;
      else
      {
        gray[gray_cnt]=0;
        if(first[j]==0) first[j]=i;
        last[j]=i;
      }
      hist[gray[gray_cnt]]++;
      ++gray_cnt;
      cnt+=3;
    }
  }
  for(i=0;i<180;i++)
  {
    for(j=115;j>=0;j--)
    {
      if(gray[j*180+i]==0)
      {
        nodapBlack[i]=j;
        buf[j*180+i]=2016;
        break;
      }
    }
  }
  max=0;

LeftMax[0] = nodapBlack[0];
  for(i = 1;i<180;i++)
  {
    if(nodapBlack[i] < LeftMax[i - 1])
      LeftMax[i] = nodapBlack[i];
    else
      LeftMax[i] = LeftMax[i - 1];
  }
RightMax[179] = nodapBlack[179];
for(i = 178;i>=0;i--)
{
  if(nodapBlack[i] < RightMax[i + 1])
    RightMax[i] = nodapBlack[i];
  else
    RightMax[i] = RightMax[i + 1];

    if(LeftMax[i] > RightMax[i])
    {
      if(max < nodapBlack[i] - LeftMax[i])
      {
        max = nodapBlack[i] - LeftMax[i];
        maxi = i;
      }
    }
    else
    {
      if(max < nodapBlack[i] - RightMax[i])
      {
        max = nodapBlack[i] - RightMax[i];
        maxi = i;
      }
    }
}

for(j = 0;j<120;j++)
buf[j*180+maxi]=0xf800;


   /*
   cnt=0;
   gray_cnt=0;
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
   cnt=0;
   for(j=0;j<120;j++)
   {
     buf[j*180+maxi]=63488;
     if(gray[j*180+maxi]==0) ++cnt;
   }
/*
cnt=0;
gray_cnt=0;
for(i=0;i<120;i++)
{
  for(j=0;j<180;j++)
  {
    buf[gray_cnt]=((gray[gray_cnt]>>3)<<11)+((gray[gray_cnt]>>2)<<5)+(gray[gray_cnt]>>3);
    ++gray_cnt;
    cnt+=3;
  }
*/
return 1;
  //int R,G,B;
/*
  cnt=0;
  for(i=0;i<120;i++)
  {
    for(j=0;j<180;j++)
    {

      image[cnt+RED]=(buf[gray_cnt]&0xf800)>>8;
      image[cnt+GREEN]=(buf[gray_cnt]&0x07e0)>>3;
      image[cnt+BLUE]=(buf[gray_cnt]&0x001f)<<3;
      gray[gray_cnt]=(image[cnt+RED]+image[cnt+GREEN]+image[cnt+BLUE])/3;
      ++gray_cnt;
      cnt+=3;
    }
  }

  for(i=1;i<119;i++)
  {
    for(j=1;j<179;j++)
    {
      tmpR=tmpG=tmpB=0;
      for(k=-1;k<=1;k++)
      {
        for(l=-1;l<=1;l++)
        {
          tmpR+=image[((i+k)*180+(j+l))*3+RED]*gaus3x3[k+1][l+1];
          tmpG+=image[((i+k)*180+(j+l))*3+GREEN]*gaus3x3[k+1][l+1];
          tmpB+=image[((i+k)*180+(j+l))*3+BLUE]*gaus3x3[k+1][l+1];
        }
      }
      image[(i*180+j)*3+RED]=tmpR>>4;
      image[(i*180+j)*3+GREEN]=tmpG>>4;
      image[(i*180+j)*3+BLUE]=tmpB>>4;
    }
  }


  cnt=0;
  gray_cnt=0;
  for(i=0;i<120;i++)
  {
    for(j=0;j<180;j++)
    {
      buf[gray_cnt]=((image[cnt+RED]>>3)<<11)+((image[cnt+GREEN]>>2)<<5)+(image[cnt+BLUE]>>3);
      ++gray_cnt;
      cnt+=3;
    }
  }
*/
}
