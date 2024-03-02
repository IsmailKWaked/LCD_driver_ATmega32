/*
 * LCD.c
 *
 * Created: 2/03/2024 6:11:24 PM
 *  Author: Ismail
 */ 

#include "BIT_MATH.h"
#include "STD_TYPES.h"
#include "DIO_interface.h"
#include "LCD_config.h"
#include "LCD_interface.h"
#include <util/delay.h>

void LCD_Init()
{
	_delay_ms(200);
	#if BIT_MODE_8
	
	DIO_SetPortDirection(LCD_Data,DIO_OUTPUT);
	DIO_SetPinDirection(LCD_Ctrl,E,DIO_OUTPUT);
	DIO_SetPinDirection(LCD_Ctrl,W,DIO_OUTPUT);
	DIO_SetPinDirection(LCD_Ctrl,RS,DIO_OUTPUT);
	
	LCD_SendCommand(Eight_Bit_Mode);
	LCD_SendCommand(CURSOR_OFF);
	LCD_SendCommand(CLEAR_COMMAND);
	_delay_ms(10);
	
	#elif BIT_MODE_4
	DIO_SetPortDirection(LCD_Data,0b00010111);
	DIO_SetPinDirection(LCD_Ctrl,E,DIO_OUTPUT);
	DIO_SetPinDirection(LCD_Ctrl,W,DIO_OUTPUT);
	DIO_SetPinDirection(LCD_Ctrl,RS,DIO_OUTPUT);
	
	LCD_SendCommand(0x02);
	_delay_ms(10);
	
	LCD_SendCommand(Four_Bit_Mode);
	LCD_SendCommand(CURSOR_OFF);
	LCD_SendCommand(CLEAR_COMMAND);
	_delay_ms(10);	
	LCD_SendCommand(0x06);
	_delay_ms(1);
	#endif
}

void LCD_SendCommand(u8 data)
{
	#if BIT_MODE_8
	
	DIO_SetPinValue(LCD_Ctrl,RS,DIO_LOW);
	DIO_SetPinValue(LCD_Ctrl,W,DIO_LOW);
	_delay_ms(1);
	DIO_SetPinValue(LCD_Ctrl,E,DIO_HIGH);
	_delay_ms(1);
	DIO_SetPortValue(LCD_Data,data);
	_delay_ms(1);
	DIO_SetPinValue(LCD_Ctrl,E,DIO_LOW);
	_delay_ms(1);
	
	#elif BIT_MODE_4
	
	DIO_SetPinValue(LCD_Ctrl,W,DIO_LOW);
	_delay_ms(1);
	
	DIO_SetPinValue(LCD_Data,D7,GET_BIT(data,7));
	DIO_SetPinValue(LCD_Data,D6,GET_BIT(data,6));
	DIO_SetPinValue(LCD_Data,D5,GET_BIT(data,5));
	DIO_SetPinValue(LCD_Data,D4,GET_BIT(data,4));
	
	
	DIO_SetPinValue(LCD_Ctrl,RS,DIO_LOW);
	
	DIO_SetPinValue(LCD_Ctrl,E,DIO_HIGH);
	_delay_ms(2);
	DIO_SetPinValue(LCD_Ctrl,E,DIO_LOW);
	_delay_ms(2);
	
	DIO_SetPinValue(LCD_Data,D3,GET_BIT(data,3));
	DIO_SetPinValue(LCD_Data,D2,GET_BIT(data,2));
	DIO_SetPinValue(LCD_Data,D1,GET_BIT(data,1));
	DIO_SetPinValue(LCD_Data,D0,GET_BIT(data,0));
	
	DIO_SetPinValue(LCD_Ctrl,RS,DIO_LOW);
	
	DIO_SetPinValue(LCD_Ctrl,E,DIO_HIGH);
	_delay_ms(2);
	DIO_SetPinValue(LCD_Ctrl,E,DIO_LOW);
	_delay_ms(2);
	
	#endif
}

void LCD_SendCharacter(u8 data)
{
	#if BIT_MODE_8
	
	DIO_SetPinValue(LCD_Ctrl,RS,DIO_HIGH);
	DIO_SetPinValue(LCD_Ctrl,W,DIO_LOW);
	_delay_ms(1);
	DIO_SetPinValue(LCD_Ctrl,E,DIO_HIGH);
	_delay_ms(1);
	DIO_SetPortValue(LCD_Data,data);
	_delay_ms(1);
	DIO_SetPinValue(LCD_Ctrl,E,DIO_LOW);
	_delay_ms(1);
	
	#elif BIT_MODE_4
	
	DIO_SetPinValue(LCD_Ctrl,W,DIO_LOW);
	_delay_ms(1);
	
	DIO_SetPinValue(LCD_Data,D7,GET_BIT(data,7));
	DIO_SetPinValue(LCD_Data,D6,GET_BIT(data,6));
	DIO_SetPinValue(LCD_Data,D5,GET_BIT(data,5));
	DIO_SetPinValue(LCD_Data,D4,GET_BIT(data,4));
	
	DIO_SetPinValue(LCD_Ctrl,RS,DIO_HIGH);
	
	DIO_SetPinValue(LCD_Ctrl,E,DIO_HIGH);
	_delay_ms(2);
	DIO_SetPinValue(LCD_Ctrl,E,DIO_LOW);
	_delay_ms(2);
	
	
	DIO_SetPinValue(LCD_Data,D3,GET_BIT(data,3));
	DIO_SetPinValue(LCD_Data,D2,GET_BIT(data,2));
	DIO_SetPinValue(LCD_Data,D1,GET_BIT(data,1));
	DIO_SetPinValue(LCD_Data,D0,GET_BIT(data,0));
	
	DIO_SetPinValue(LCD_Ctrl,RS,DIO_HIGH);
	
	DIO_SetPinValue(LCD_Ctrl,E,DIO_HIGH);
	_delay_ms(2);
	DIO_SetPinValue(LCD_Ctrl,E,DIO_LOW);
	_delay_ms(2);
	
	#endif
}

void LCD_SendString(u8 *str)
{
	while(*str != '\0')
	{
		LCD_SendCharacter(*str);
		str++;
	}
}

void LCD_GoToRowCol(u8 row, u8 col)
{
	u8 address=0;
	switch (row)
	{
		case 0:
			address = col;
			break;
		case 1:
			address = col + 0x40;
			break;
	}
	LCD_SendCommand(address | SET_CURSOR_LOCATION);
}

void LCD_SendStringRowCol(u8 row, u8 col, u8 *str)
{
	LCD_GoToRowCol(row,col);
	LCD_SendString(str);
}



void LCD_ClearScreen()
{
	LCD_SendCommand(CLEAR_COMMAND);
}

void LCD_writeNumber(u32 Num)
{
	u32 Ress=1;
	
	while (Num!=0)
	{
		Ress=Ress*10+Num%10;
		Num/=10;
	}
	do 
	{
		LCD_SendCharacter((Ress%10)+ '0');
		Ress/=10;
	} while (Ress!=1);
}

void LCD_Send_Special_Char(u8 specialChar,u8 index)
{
	
}