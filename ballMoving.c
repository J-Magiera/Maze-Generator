#include "ballMoving.h"

typedef struct {
  /* Information about what has to be displayed */
  GUI_AUTODEV_INFO AutoDevInfo;
  /* My params */
	int x;
	int y;
	
} PARAM;


static void _Draw(void * p) {
	  PARAM * pParam = (PARAM *)p;
  /* Fixed background */
  if (pParam->AutoDevInfo.DrawFixed) {
		GUI_ClearRect(0,0,320,240);
			GUI_SetColor(GUI_BLUE);
			GUI_DrawLine(0,100,180,100);	
			GUI_FillRect(300,0,320,40);
	}	
	GUI_SetColor(GUI_RED);
	GUI_FillCircle(pParam->x, pParam->y ,10);
	  /* Fixed foreground */
  /*if (pParam->AutoDevInfo.DrawFixed) {
			GUI_SetColor(GUI_BLUE);
			GUI_DrawLine(10,100,180,100);	
			GUI_FillRect(40,40,80,80);
	}	*/
}
static void ball(void){
	
	PARAM Param;            /* Parameters for drawing routine */
	GUI_AUTODEV AutoDev;    /* Object for banding memory device */
	
  GUI_Clear();
	int win = 0;
	int kx,ky = 0; 
	Param.x = 18;
	Param.y = 10;
	GUI_MEMDEV_CreateAuto(&AutoDev);
	while(!win)
	{ 
		GUI_MEMDEV_DrawAuto(&AutoDev, &Param.AutoDevInfo, &_Draw, &Param);
		GUI_Delay(4);
		
		if(Param.x == 320){kx = 1;}
		else if (0 == Param.x){kx = 0;}
		if (Param.y == 0 || Param.y == 240 || (Param.y == 100 && (Param.x<180))){ ky = !ky;}
		if(0 == kx){Param.x++;}
		else{Param.x--;}
		if(0 == ky){Param.y++;}
		else{Param.y--;}
		if(Param.x > 300 && Param.y < 40){win = 1;}
	}
	GUI_Clear();
	GUI_DispStringHCenterAt("WIN!!!!",160,120);
	GUI_Delay(1400);
}

void GUI_StartWindow1(void){
	GUI_Init();
	while(1){
		ball();
	}	
}
