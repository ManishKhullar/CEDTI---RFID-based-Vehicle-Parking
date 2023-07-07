/* Controller with LCD and Motor driver*/
#include<avr/io.h>
//#include
#define FOSC 4000000   //crysatl frequency
#define BAUD 9600  //baud rate
#define MYUBRR FOSC/16/BAUD-1


void Delay_ms(unsigned int ms)	
{
	unsigned int delay_cont,delay_cont1;

	//asm("clrwdt");
	for(delay_cont = 0; delay_cont < ms; delay_cont ++)
	{
		
		for(delay_cont1 = 0; delay_cont1 < 227; delay_cont1 ++)
		{
		}			
		 //asm("nop"); 
	}	
}



void init_hw()
{
PORTC= 0xff;
PORTA= 0xff;
PORTB= 0xff;
PORTD= 0x0e;
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



void main (void)
{
char buf[20], str1[]="4200A72CB079",str2[]="4B0098F01A39";
int i;
void init_hw();
USART_Init(MYUBRR);
//init_lcd();
//lcd_string(1,1,"RFID Based");
//lcd_string(2,1,"Car Parking");
Delay_ms(500);
while(1)
{
if(RXC==1)//bitset when receive complet
	{
	for(i=0;(i<13);i++)
	buf[i]=USART_Receive();

	}
if(strcmp(buf,str1))
{
//lcd_string(1,1,"Car A");
PORTB=0X01;
Delay_ms(500);
Delay_ms(500);
PORTB=0X00;
}

else if(strcmp(buf,str1))
{
//lcd_string(1,1,"Car B");
PORTB=0X01;

Delay_ms(500);
Delay_ms(500);
PORTB=0X00;
PORTA=0X00;
}
else
{
PORTA=0X01;
PORTB=0X02;
Delay_ms(1500);
PORTA=0X00;
PORTB=0X00;
}
}

}

