
STM32的4x4矩阵键盘，不用外部中断，可以检测多个按键同时按下
#ifndef __COMMON_H 
#define __COMMON_H 
#include "stm32f10x.h" 

/* 4*4矩阵键盘 */ 
void keyboard_init(void); 
void update_key(void); 
extern unsigned char key[4][4];

#endif  
/**************矩阵键盘.c文件*****************************/
#include "common.h"

struct io_port 
{                                            
GPIO_TypeDef *GPIO_x;                 
unsigned short GPIO_pin;
}; 
static struct io_port key_output[4] = {
{GPIOD, GPIO_Pin_0}, {GPIOD, GPIO_Pin_1},
{GPIOD, GPIO_Pin_2}, {GPIOD, GPIO_Pin_3}
};
static struct io_port key_input[4] = {
{GPIOD, GPIO_Pin_4}, {GPIOD, GPIO_Pin_5},
{GPIOD, GPIO_Pin_6}, {GPIOD, GPIO_Pin_7}
};
unsigned char key[4][4];
void keyboard_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
unsigned char i;

/* 键盘行扫描输出线 输出高电平 */
/* PA0 PA1 PA2 PA3 输出*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
/* 键盘列扫描输入线 键被按时输入高电平 放开输入低电平 */
/* PA4 PA5 PA6 PA7 输入*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);

for(i = 0; i < 4; i++)
{
  GPIO_SetBits(key_output[i].GPIO_x, key_output[i].GPIO_pin);
}
}
void update_key(void)
{
unsigned char i, j;
for(i = 0; i < 4; i++)             //i是输出口，依次置低电平
{
  GPIO_ResetBits(key_output[i].GPIO_x, key_output[i].GPIO_pin);   
  for(j = 0; j < 4; j++)            //j是输入口，当键按下时导通被置为低电平  
  {
   if(GPIO_ReadInputDataBit(key_input[j].GPIO_x, key_input[j].GPIO_pin) == 0)
   {
    key[i][j] = 1; 
   }else{
    key[i][j] = 0;
   }
  }
  GPIO_SetBits(key_output[i].GPIO_x, key_output[i].GPIO_pin);
}
}


