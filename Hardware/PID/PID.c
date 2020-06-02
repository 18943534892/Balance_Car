#include "all.h"

int pwm_L,pwm_R;
u8 Flag_Left=0,Flag_Right=0,Flag_Front=0,Flag_Back=0,Flag_Stop=1;

int balance(float Now_angle,short gyro)                //ƽ�⻷
{
	static float KP=121;    //  163
	static float KD=0.369;    //   0.325
	
	static float Set_Angle=2.5;     //���ýǶ�
	float Error;                    //���ƫ��
	int balance;                    //PWM����ֵ
	
	Error=Now_angle-Set_Angle;
	balance=KP*Error+KD*gyro;       //�������PD���Ƶĵ��PWM
	return balance;
}

int speed_control(int encoder_left,int encoder_right)  //�ٶȻ�
{
	static float Velocity,Encoder_Least,Encoder;
	static short Movement=0;
	static float Encoder_Integral=0;
	static float kp=4;                 //4
	static float ki;
	
	ki=kp/200.0;

	if(Flag_Front==1)
	{
	  Movement=150;
	}
	if(Flag_Back==1)
	{
	  Movement=-150;
	}
	if(Flag_Stop==1)
	{
	  Movement=0;
	}
	
	Movement=350/*350*/;        //���ԣ�ǰ��
	
	Encoder_Least =(Encoder_Left+Encoder_Right)-0;
	Encoder *= 0.8;
	Encoder += Encoder_Least*0.2;
	Encoder_Integral +=Encoder;
	Encoder_Integral=Encoder_Integral-Movement;
	Velocity=Encoder*kp+Encoder_Integral*ki;
	return Velocity;
}

int turn(int encoder_left,int encoder_right,float gyro)//ת��
{
	u16 adc_l,adc_r;
	
	//��ת�򻷿���
	float Turn_Amplitude=88/2,turn,Tkp=0.063,Bias;        //0.06
	
	if(Flag_Left==1) Turn_Amplitude=-1000;
	else if(Flag_Right==1) Turn_Amplitude=1000;
	else Turn_Amplitude=0;
	
	//Turn_Amplitude=350;        //���ԣ���ת
	
	adc_l=Get_Adc_Average(5,4);
	adc_r=Get_Adc_Average(4,4);
	Turn_Amplitude=(adc_r*1.10-adc_l)*2;
	
	Bias=gyro-0; //Ŀ����ٶ���0�������Ϊ����С������0���ٶȵ���ǰ����
	turn=Tkp*Bias;
	turn+=Turn_Amplitude;
	return turn;
}

void PWM_Set(short pwm_L,short pwm_R)                  //����pwm
{
	
	u16 Motor_Dead=120;                    //�������
	
  if(pwm_L<0)
	{
		pwm_L=0-pwm_L;
		if(pwm_L>4000)pwm_L=4000;
	  TIM_SetCompare1(TIM2,0);  //ͨ��1  ��
	  TIM_SetCompare2(TIM2,pwm_L+Motor_Dead);         //ͨ��2  ��
	}
	else 
	{
		if(pwm_L>4000)pwm_L=4000;
	  TIM_SetCompare1(TIM2,pwm_L+Motor_Dead);         //ͨ��1  ��
	  TIM_SetCompare2(TIM2,0);  //ͨ��2  ��
	}
	
	if(pwm_R<0)
	{
		pwm_R=0-pwm_R;
		if(pwm_R>4000)pwm_R=4000;
		TIM_SetCompare3(TIM2,pwm_R+Motor_Dead);  //ͨ��1  ��
	  TIM_SetCompare4(TIM2,0);         //ͨ��2  ��
	}
	else 
	{
		if(pwm_R>4000)pwm_R=4000;
	  TIM_SetCompare3(TIM2,0);         //ͨ��1  ��
	  TIM_SetCompare4(TIM2,pwm_R+Motor_Dead);  //ͨ��2  ��
	}
}

void Control(void)                                     //�ܿ���
{
	
	short pwm_balance,pwm_speed,pwm_turn;
	
	//printf("%d,%d\r\n",Encoder_Left,Encoder_Right);
	
	pwm_balance=balance(Roll,gyro[0]);
	pwm_speed=speed_control(Encoder_Left,Encoder_Right);
	pwm_turn=turn(Encoder_Left,Encoder_Right,gyro[2]);
	
	pwm_L=pwm_balance+pwm_speed+pwm_turn;
	pwm_R=pwm_balance+pwm_speed-pwm_turn;
	
	PWM_Set(pwm_L,pwm_R);

}

void Close_Motor(void)                                 //�رյ��
{
  TIM_SetCompare1(TIM2,0);
	TIM_SetCompare2(TIM2,0);
	TIM_SetCompare3(TIM2,0);
	TIM_SetCompare4(TIM2,0);
}
