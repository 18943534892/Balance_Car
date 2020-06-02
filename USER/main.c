/*****************************************************************
@ ����STM32 & MPU6050_DMP����ƽ��С��
*****************************************************************/
#include "all.h"

/*
void LCD1602_Allot()
{
	Pitch*=100;
	Roll*=100;
	Yaw*=100;
  if(Pitch<0)
	{
	  Pitch=0-Pitch;
		hang1[0]='-';
	}
	else hang1[0]=' ';
	
	if(Roll<0)
	{
	  Roll=0-Roll;
		hang1[9]='-';
	}
	else hang1[9]=' ';
	
	//if(Yaw<0)
	//{
	//  Yaw=0-Yaw;
	//	hang2[0]='-';
	//}
	//else hang2[0]=' ';
	
	hang1[1]='0'+(u16)(Pitch)/10000;
	hang1[2]='0'+(u16)(Pitch)%10000/1000;
	hang1[3]='0'+(u16)(Pitch)%1000/100;
	hang1[4]='.';
	hang1[5]='0'+(u16)(Pitch)%100/10;
	hang1[6]='0'+(u16)(Pitch)%10;
	
	hang1[7]=' ';
	hang1[8]=' ';
	
	hang1[10]='0'+(u16)(Roll)/10000;
	hang1[11]='0'+(u16)(Roll)%10000/1000;
	hang1[12]='0'+(u16)(Roll)%1000/100;
	hang1[13]='.';
	hang1[14]='0'+(u16)(Roll)%100/10;
	hang1[15]='0'+(u16)(Roll)%10;
	
	hang2[0]=' ';
	hang2[1]=' ';
	hang2[2]='0'+pwm_L%100000/10000;
	hang2[3]='0'+pwm_L%10000/1000;
	hang2[4]='0'+pwm_L%1000/100;
	hang2[5]='0'+pwm_L%100/10;
	hang2[6]='0'+pwm_L%10;
	
	hang2[7]=' ';
	hang2[8]=' ';
	hang2[9]='0'+pwm_R%100000/10000;
	hang2[10]='0'+pwm_R%10000/1000;
	hang2[11]='0'+pwm_R%1000/100;
	hang2[12]='0'+pwm_R%100/10;
	hang2[13]='0'+pwm_R%10;
	hang2[14]=' ';
	hang2[15]=' ';
	
	LCD1602_Display();	
}
*/
 int main(void)
 {
	 u8 Control_Flag=1;
	 delay_init();  //��ʼ����ʱ
	 USART1_Init(115200,0);
	 USART3_Init(115200);
//	 LCD1602_Init();
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //��ʼ��NVIC
	 MPU6050_Init();  //��ʼ��������
	 Timer1_Init(499,71);
	 TIM2_PWM_Init(7199,0);    //����Ƶ������Ϊ72000000/(7199+1)=10kHZ 
	 TIM3_Mode_Config();
	 TIM4_Mode_Config();
	 Adc_Init();
	 
	OLED_Init();				//��ʼ��OLED
  OLED_ShowString(0,0,"ALIENTEK",24);  
	OLED_ShowString(0,24, "0.96' OLED TEST",16);  
 	OLED_ShowString(0,40,"ATOM 2014/5/4",12);  
 	OLED_ShowString(0,52,"ASCII:",12);  
 	OLED_ShowString(64,52,"CODE:",12);  
	OLED_Refresh_Gram();//������ʾ��OLED	
	while(1)
	{ 
		//printf("%f,%f,%f\r\n",Pitch,Roll,Yaw);//���ŷ����
    //printf("%d,%d,%d\r\n",gyro[0],gyro[1],gyro[2]);
		//printf("%d,%d,%d\r\n",accel[0],accel[1],accel[2]);
		//gyro[3], accel[3]   in mpu6050.c �Ǽ��ٶȼ�����������
		
		
		if((Roll<-26)||(Roll>30))Control_Flag=0;
		if(Control_Flag)Control();
		else Close_Motor();

		delay_ms(1);
  }
}
 



