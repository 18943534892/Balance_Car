#include "all.h"

#define Lcd1602_delay 2    //LCD1602��ʱʱ��

u8 hang1[16]="Made by:Talent  ";
u8 hang2[16]="QQ:1533923192 ~ ";

u8 LCD1602_5x8_1[8]={0x1c,0x14,0x1c,0x00,0x00,0x00,0x00,0x00};
u8 LCD1602_5x8_2[8]={0x10,0x10,0x10,0x00,0x00,0x00,0x00,0x00};
u8 LCD1602_5x8_3[8]={0x14,0x14,0x14,0x00,0x00,0x00,0x00,0x00};

void PB15_8_Write(u8 data)
{
 /* GPIO_SetBits(GPIOB,((data*256)|0xff)&0xffff);
  GPIO_ResetBits(GPIOB,(~(((data*256)|0xff)&0xffff)));*/
	
  D0=data&0x01;data>>=1;
	D1=data&0x01;data>>=1;
	D2=data&0x01;data>>=1;
	D3=data&0x01;data>>=1;
	D4=data&0x01;data>>=1;
	D5=data&0x01;data>>=1;
	D6=data&0x01;data>>=1;
	D7=data&0x01;
	
}

void LCD1602_Write_Reg(u8 data)  //д����
{
  RS=0;
	E=0;
  delay_us(100);
	E=1;
	PB15_8_Write(data);
  E=0;
}

void LCD1602_Write_Dat(u8 data)  //д����
{
  RS=1;
	E=0;
  delay_us(100);
	E=1;
	PB15_8_Write(data);
  E=0;
}

void LCD1602_Init()
{
	u8 i;
	/*************************GPIO��ʼ��**************/
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  //ʹ��RCC_APB2Periph_AFIOʱ��

	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);//��ȫ����SWD��JTAG 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);     //����JTAG

  GPIO_InitTypeDef gpio;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //ʹ��GPIOBʱ��
  gpio.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_12|GPIO_Pin_13|
	              GPIO_Pin_14|GPIO_Pin_15;
  gpio.GPIO_Mode=GPIO_Mode_Out_OD;
  gpio.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOB,&gpio);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ��GPIOAʱ��
  gpio.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15;
  gpio.GPIO_Mode=GPIO_Mode_Out_OD;
  gpio.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&gpio);
	/*************************************************/
	
	LCD1602_Write_Reg(0x01);       // д���ִ�������ȳ�ʼ��
	delay_ms(Lcd1602_delay);
	
	LCD1602_Write_Reg(0x02);
	delay_ms(Lcd1602_delay);
	
	LCD1602_Write_Reg(0x06);
	delay_ms(Lcd1602_delay);
	
	LCD1602_Write_Reg(0x0c);
	delay_ms(Lcd1602_delay);
	
	LCD1602_Write_Reg(0x14);
	delay_ms(Lcd1602_delay);
	
	LCD1602_Write_Reg(0x38);
	delay_ms(Lcd1602_delay);
	/*
	LCD1602_Write_Reg(0x40);             //���Զ���ͼ��д��CGRAM��    ��
	delay_ms(1);
	 for(i=0;i<8;i++) 
	{
	    LCD1602_Write_Dat(LCD1602_5x8_1[i]);
	}
	*/
	LCD1602_Write_Reg(0x40);             //���Զ���ͼ��д��CGRAM��    ��
	delay_ms(Lcd1602_delay);
	 for(i=0;i<8;i++) 
	{
	    LCD1602_Write_Dat(LCD1602_5x8_1[i]);
	}
	
	LCD1602_Write_Reg(0x48);             //���Զ���ͼ��д��CGRAM��    '
	delay_ms(Lcd1602_delay);
	 for(i=0;i<8;i++) 
	{
	    LCD1602_Write_Dat(LCD1602_5x8_2[i]);
	}
	
	LCD1602_Write_Reg(0x50);             //���Զ���ͼ��д��CGRAM��    "
	delay_ms(Lcd1602_delay);
	 for(i=0;i<8;i++) 
	{
	    LCD1602_Write_Dat(LCD1602_5x8_3[i]);
	}
	
}

void LCD1602_Display()
{
  u8 i;
	
	LCD1602_Write_Reg(0x80);	                       //��һ����ʾ
	 for(i=0;i<16;i++)
	 {
	   LCD1602_Write_Dat(hang1[i]);
	 }

  LCD1602_Write_Reg(0xc0);	                       //�ڶ�����ʾ
	 for(i=0;i<16;i++)
	 {
	   LCD1602_Write_Dat(hang2[i]);
	 }

}
