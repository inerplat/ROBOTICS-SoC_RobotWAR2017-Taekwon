#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <math.h>
#include "amazon2_sdk.h"
#include "graphic_api.h"
#include "img_process.h"
#include "img_analysis.h"
#include "robot_motion.h"
#include "robot_protocol.h"
#include "uart_api.h"
#define AMAZON2_GRAPHIC_VERSION		"v0.5"
#define RED 0
#define GREEN 1
#define BLUE 2
int histcnt;
unsigned char image[120*180*3], gray[120*180];
int main(int argc, char **argv)
{
	int i,j,cnt,gray_cnt;
	if (open_graphic() < 0) {
		return -1;
	}

	SURFACE* bmpsurf = 0;
	U16* fpga_videodata = (U16*)malloc(180 * 120 * 2);
	direct_camera_display_off();
	clear_screen();
	while (1)
	{
		int motion;
		//printf("read fpga video data\n");
		read_fpga_video_data(fpga_videodata); //fpga로부터 처리된 영상데이터를 fpga_videodata에 받아옴

		//for(i=0;i<120*180;i++) fpga_videodata[i]=0;
		MCU_process(fpga_videodata, image, gray,histcnt++); // MCU를 이용한 영상처리

		draw_fpga_video_data_full(fpga_videodata);	//보드에 fpga영상 데이터를 출력
		flip();
	//	motion=MCU_analysis(fpga_videodata); // 영상에대한 분석작업
	//	Order_to_Robot(motion); // 가장 적절한 모션을 수행
/*
		if(direct_camera_display_stat() > 0) {
			printf("direct camera display on\n");
			printf("please direct camera diplay off\n");
			break;
		}
		printf("draw fpga video data\n");
*/
	}
	free(fpga_videodata);
	if (bmpsurf != 0)
		release_surface(bmpsurf);
	close_graphic();
	return 0;
}
