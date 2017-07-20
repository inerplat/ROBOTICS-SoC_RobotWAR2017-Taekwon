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
void MCU_process(U16* buf, unsigned char image[], unsigned char gray[], int histcnt)
{
  int i, j, cnt=0, k, l;
  static int hiscnt=0;
  int gaus3x3[3][3]={{1,2,1},{2,4,2},{1,2,1}};
  unsigned char histogram[256];
  int gray_cnt=0;
  int sum;
  int tmpR, tmpG, tmpB;
  int sumR, sumG, sumB;
  double alpha, beta;
  int hist[256];
  int Tmax;
  double VarMax;
  double darkAvg = 0, brightAvg = 0;
  int T = 5, darkCnt = 0, brightCnt = 0;

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
      hist[gray[gray_cnt]]++;
      ++gray_cnt;
      cnt+=3;
    }
  }

  if(!((histcnt)%10))
  {
  sum = 0;
  for (i = 0; i < 256; i++)
  {
    sum += histogram[i];
    hist[i] = (int)((double)sum * 0.1181 + 0.5); // 누적합*((최대밝기/전체영상사이즈)+0.5[반올림])
  }
  gray_cnt=0;
  for (i = 0; i < 120; i++)
  {
    for (j = 0; j < 180; j++)
    {
      gray[gray_cnt] = hist[gray[gray_cnt]];
      gray_cnt++;
    }
  }
}


  VarMax = -1;

  gray_cnt=0;
  for (T = 120; T < 135; T++)
  {
  	darkCnt = brightCnt = darkAvg = brightAvg = 0;
  	for (j = 0; j < 120; j+=2)
  	{
  		for (i = 0; i < 180; i+=2)
  		{
  			if (gray[gray_cnt] < T)
  			{
  				darkCnt++;
  				darkAvg += gray[j*180+i];
  			}
  			else
  			{
  				brightCnt++;
  				brightAvg += gray[j*180+i];
  			}
        ++gray_cnt;
  		}
  	}
  	darkAvg = darkAvg / (double)(!darkCnt ? 1 : darkCnt);
  	brightAvg = brightAvg / (double)(!brightCnt ? 1 : brightCnt);
  	alpha = ((double)brightCnt / (double)(60 * 90)) * (double)100;
  	beta = ((double)darkCnt / (double)(60 * 90)) * (double)100;
  	if (VarMax < alpha*beta*(darkAvg - brightAvg)*(darkAvg - brightAvg))
  	{
  		Tmax = T;
  		VarMax = alpha*beta*(darkAvg - brightAvg)*(darkAvg - brightAvg);
  	}
  }
  gray_cnt=0;
  for(i=0;i<120;i++)
  {
    for(j=0;j<180;j++)
    {
      if(gray[gray_cnt]<Tmax) gray[gray_cnt]=0;
      else gray[gray_cnt]=255;
      ++gray_cnt;
    }
  }

/*
  cnt=0;
  gray_cnt=0;
  for(i=0;i<120;i++)
  {
    for(j=0;j<180;j++)
    {
      if(!image[cnt+RED] && !image[cnt+GREEN] && !image[cnt+BLUE])
      { gray[gray_cnt]=255;}
      else gray[gray_cnt]=0;
      ++gray_cnt;
      cnt+=3;
    }
  }

  gray_cnt=0;
  for(i=3;i<117;i++)
  {
    for(j=3;j<177;j++)
    {
      if(gray[i*180+j])
      {
        for(k=1;k<=1;k++)
        {
          if(gray[(i-k)*180+j]<254 || gray[(i+k)*180+j]<254)
          {
            gray[i*180+j]=254;
            break;
          }
        }
      }
    }
  }
*/
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
