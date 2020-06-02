
#include "all.h"
#include "math.h"
#include "iic.h"


static signed char gyro_orientation[9] = {-1, 0, 0,
                                           0,-1, 0,
                                           0, 0, 1};
float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;
float Pitch,Roll,Yaw;
unsigned long sensor_timestamp;
short gyro[3], accel[3], sensors;    //�����ǣ����ٶ�
unsigned char more;
long quat[4];                        //��Ԫ������

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�����ĸ������Խ�dmp��


//addr������slave_address
//reg ����������Ҫд�����ݵ��׵�ַ
//len ��д�����ݵĳ���
//data����Ҫд���һ������	  
u8 I2C_Write_Buffer(u8 addr, u8 reg, u8 len, u8 * data)
{
    int i;
    I2C_Start();
    I2C_Send_Byte(addr << 1 | 0);//7λ�����ӵ�ַ+��дλ
    if (I2C_Wait_Ack()) 
	{
        I2C_Stop();
        return 0;
    }
    I2C_Send_Byte(reg);
    I2C_Wait_Ack();
    for (i = 0; i < len; i++) 
	{
        I2C_Send_Byte(*data);
        if (I2C_Wait_Ack()) 
		{
            I2C_Stop();
            return 0;
        }
		data++;
    }
    I2C_Stop();
    return 1;
}



//addr������slave_address
//reg ����������Ҫ�������ݵ��׵�ַ
//len ���������ݵĳ���
//buf ����Ҫ���������ݴ洢λ��
u8 I2C_Read_Buffer(u8 addr, u8 reg, u8 len, u8* buf)
{
    I2C_Start();
    I2C_Send_Byte(addr << 1 | 0);
    if (I2C_Wait_Ack())
	{
        I2C_Stop();
        return 0;
    }
    I2C_Send_Byte(reg);
    I2C_Wait_Ack();

    I2C_Start();
    I2C_Send_Byte(addr << 1 | 1);
    I2C_Wait_Ack();
    while (len)
	{
        *buf = I2C_Read_Byte();
        if (len == 1)
            I2C_NAck();
        else
            I2C_Ack();
        buf++;
        len--;
    }
    I2C_Stop();
    return 1;
}

//����ֵ 0�����ɹ�
//		-1����ʧ��
int I2C_Read(u8 addr, u8 reg, u8 len, u8 *buf)
{
	if(I2C_Read_Buffer(addr,reg,len,buf))
		return 0;
	else
		return -1;
}
//����ֵ 0��д�ɹ�
//		-1��дʧ��
int I2C_Write(u8 addr, u8 reg, u8 len, u8* data)
{
	if(I2C_Write_Buffer(addr,reg,len,data))
		return 0;
	else
		return -1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void MPU6050_Init(void)
{
	int result=0;
	I2C_Init_IO();
	result=mpu_init();
	if(!result)
	{	 		 
		if(!mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL))		//mpu_set_sensor
		if(!mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL))	//mpu_configure_fifo
		if(!mpu_set_sample_rate(DEFAULT_MPU_HZ))	   	  		//mpu_set_sample_rate
		if(!dmp_load_motion_driver_firmware())   	  			//dmp_load_motion_driver_firmvare
		if(!dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation))) 	  //dmp_set_orientation
		if(!dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
		    DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
		    DMP_FEATURE_GYRO_CAL))		   	 					 //dmp_enable_feature
		if(!dmp_set_fifo_rate(DEFAULT_MPU_HZ))   	 			 //dmp_set_fifo_rate
		run_self_test();		//??
		if(!mpu_set_dmp_state(1));
	}
}


void MPU6050_Pose(void)
{
	dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors,&more);	 
	if(sensors & INV_WXYZ_QUAT )
	{
		q0 = quat[0] / q30;
		q1 = quat[1] / q30;
		q2 = quat[2] / q30;
		q3 = quat[3] / q30;

		Pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3;	// pitch
		Roll  = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3;	// roll
		Yaw   = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;	//yaw
	}
}
//�߼���ʱ��1�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��1
void Timer1_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);

	TIM_DeInit(TIM1); 

	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ClearFlag(TIM1, TIM_FLAG_Update); 
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);

	TIM_Cmd(TIM1,ENABLE);

}

void TIM1_UP_IRQHandler(void)
{
	static u8 count=0;
	if(TIM_GetITStatus(TIM1,TIM_IT_Update) !=RESET )
	{
		MPU6050_Pose();
		//printf("%f,%f,%f\r\n",Pitch,Roll,Yaw);//���ŷ����
		count++;
		if(count==40)
		{
		  count=0;
			Read_Encoder();
			//printf("%d,%d\r\n",Encoder_Left,Encoder_Right);//�������ֵ
		}
		
		//Control();
		
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
	}
}

