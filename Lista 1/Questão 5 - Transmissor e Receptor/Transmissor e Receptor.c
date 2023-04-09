#include <p18f4520.h>

#define TX PORTCbits.RC0 
#define RX PORTCbits.RC1
#define bit_inteiro 54 
#define meio_bit 54/2

int controle=0;

void delay (long int time)
{
	for(;time!=0;time--);
}

void transmite_dado(char caractere)
{
	int i=0;
	TX=0;
	delay(bit_inteiro);
	while(i!=8)
	{
		TX=caractere;
		caractere=caractere>>1;
		delay(bit_inteiro);
		i++;	
	}
	TX=1;
	delay(bit_inteiro);
}

char recebe_dado()
{
    char caractere_=0, b[8];
    int i=0,j=7;
    delay(meio_bit);
    while(i<=8)
	{
        if(i>0)
		{
            b[j]= RX;
            j--;
            delay(bit_inteiro);
        }	
        i++;
    } 
    i=0;
    while(i<=7)
	{
		if(i<7)
        caractere_=caractere_<<1;
        caractere_=caractere_+b[i];
        i++;
    }
    controle=1;
    return caractere_;
}

void transmissao()
{
	char Caractere;
    if(RX==0)
	{
    	Caractere=recebe_dado();
    }
    if(controle==1)
	{
        controle=0;
        transmite_dado(Caractere);
        transmite_dado(Caractere);
	}
}
void conf()
{
	TRISA=0b11111111;   
	TRISB=0b11111111;	
	TRISC=0b11111110;   
	TRISD=0b11111111;   
	TX=1;
}

void main(void)
{
	conf();
	while (1)
	{
        transmissao();
    }
}
