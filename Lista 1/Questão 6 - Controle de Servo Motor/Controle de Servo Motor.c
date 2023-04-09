#include<p18f4520.h>

#define saida PORTCbits.RC0
#define C1 PORTDbits.RD0
#define C2 PORTDbits.RD1
#define C3 PORTDbits.RD2
#define C4 PORTDbits.RD3
#define C5 PORTDbits.RD4

void delay_5()
{
    long int i;
	for(i=0;i<2;i++)
	{
    Nop();Nop();Nop();Nop();
	}
    
}
void delay_ms()
{
    long int i;
	for(i=0;i<33;i++){}
    Nop();
}

void delay_20ms()
{
    long int i;
	for(i=0;i<30*40;i++){}
}

void mostra_angulo()
{
    long int i;
    if(C1==0)//5
	{
        saida=1;
        delay_ms();
        delay_5();
               
        saida=0; 
        delay_20ms();
    }
    if(C2==0)//55
	{
        saida=1;
        delay_ms();
        for(i=0;i<8;i++)
		{
            delay_5();
            Nop();Nop();
            Nop();Nop();
            Nop();Nop();
            Nop();Nop();Nop();
            Nop();Nop();
        }
        
        saida=0; 
        delay_20ms();
    }
    if(C3==0)//90
	{
        saida=1;
        delay_ms();
        for(i=0;i<14;i++)
		{
            delay_5();Nop();Nop();
            Nop();           
            
        }
        
        saida=0; 
        delay_20ms();                
    }
    if(C4==0)//125
	{
        saida=1;
        delay_ms();
        for(i=0;i<20;i++)
		{
            delay_5();
        }
        
        saida=0; 
        delay_20ms();                
    }
    if(C5==0)//180
	{ 
        saida=1;
        delay_ms();
        for(i=0;i<30;i++)
		{
            delay_5();
        }
        
        saida=0; 
        delay_20ms();               
    }
}
void conf()
{
	TRISA=0b11111111;
	TRISB=0b11111111;
	TRISC=0b11111110;
	TRISD=0b11111111;
}
void main()
{
	conf();
	while(1)
	{
		mostra_angulo();
	}
}