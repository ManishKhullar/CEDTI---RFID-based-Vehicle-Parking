/*
 * RFID1.c
 *
 * Created: 29-06-2012 20:41:07
 *  Author: Pranav
 */ 

#include <avr/io.h>
/* Controller with LCD and Motor driver*/


#define FOSC 4000000   //crysatl frequency
#define BAUD 9600  //baud rate
#define MYUBRR FOSC/16/BAUD-1
//portd7-en, portd6-rw, portd5rs
#define LCD_EN PORTD 
#define LCD_RS PORTD
#define LCD_RW PORTD

void Delay_ms(unsigned int ms)	
{
	unsigned int delay_cont,delay_cont1;

	for(delay_cont = 0; delay_cont < ms; delay_cont ++)
	{
		
		for(delay_cont1 = 0; delay_cont1 < 227; delay_cont1 ++)
		{
		}			
		
	}	
}

void WritePulse() 
{
	LCD_EN = 1;
	LCD_EN = 0;
	LCD_RW=0;

}
void LCDCmd(unsigned char commd)
{
	LCD_RW=0;
	LCD_RS = 0;
	LCD_EN = 0;
	PORTC = (commd );
	WritePulse();
}

void LCDData(unsigned char character)
{
	LCD_RW=0;
	LCD_RS = 1;
	LCD_EN = 0;
	PORTC = (character );
	WritePulse();
}
void lcd_command(unsigned char command)
{
	unsigned char i;
	
	LCDCmd(command );
	for(i = 0; i<100; i++);
}

void lcd_data(unsigned char data)
{
	unsigned char i;
	
	LCDData(data);
	for(i = 0; i<100; i++);
}

void goto_xy(int line , int pos)
{
	if(line == 1)
		lcd_command(0x80+(pos-1));
	if(line == 2)
		lcd_command(0xc0+(pos-1));
}


void lcd_string(int line, int pos, const char *c)
{
	goto_xy(line,pos);
	
	while(*c != '\0')
	{
		lcd_data(*c);
		c++;
	}
}

void init_lcd()
{


	LCD_RS = 0;
	LCD_EN = 0;

	lcd_command(0x30);		
	lcd_command(0x30);
	lcd_command(0x30);
	lcd_command(0x0C);		//display on
	lcd_command(0x28);		// 4 bit and 5x7 dots
	lcd_command(0x01);		// clear display
	lcd_command(0x06);		// auto increment
	lcd_command(0x0c);		// display on
	lcd_command(0x80);		// address
}


void init_hw()
{
PORTC=0xff;
DDRC=0XFF;
PORTA=0xff;
DDRA=0XFF;
PORTB=0xff;
DDRB=0XFF;
PORTD=0x0e;
}

void cleardisplay()
{
	lcd_string(1,1,"                ");
	lcd_string(2,1,"                ");

}


void USART_Init(unsigned int ubrr)
{
UBRRH = (unsigned char) (ubrr>>8);
UBRRL= (unsigned char) ubrr;
UCSRB=(1<<RXEN)|(1<<TXEN);
UCSRC=(1<<URSEL)|(1<<USBS)|(UCSZ0);
}

unsigned char USART_Receive(void)
{
while(!(UCSRA & (1<<RXC)));
return UDR;
}

 void Gate(void)
{
PORTA=0X01;
PORTA=0X00;
Delay_ms(1500);
Delay_ms(500);
PORTA=0X00;
PORTA=0X01;
}

int main (void)
{
int i;
char buf[20], str1[]="4200A72CB079", str2[]="4B0098F01A39";
void init_hw();
USART_Init(MYUBRR);
init_lcd();
lcd_string(1,1,"RFID Based");
lcd_string(2,1,"Car Parking");
Delay_ms(500);
while(1)
{
if(RXC==1)	//bitset when receive complet
	{
	for(i=0;(i<13);i++)
	buf[i]=USART_Receive();

	}
if(strcmp(buf,str1)||strcmp(buf,str2))
	{
	lcd_string(1,1,"Car IN");
	PORTB=0X01;//LED
	Gate();
	PORTB=0X00;//LED
	}
else
	{
	PORTB=0x02;
	Delay_ms(500);
	PORTB=0x00;
	}

}
}