#include <p18f4520.h>

#pragma config OSC = INTIO67

#define sinal  PORTAbits.RA5
#define saida  PORTC

int T;
int frequencia;

void delay_100us ()
{
	char i;
	for (i=0; i<11; i++){};
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
}

void mede_periodo()
{
	while (!sinal) { };
	while (sinal) { };

	T = 0;
	while (!sinal)
	{
		delay_100us();
		T++;
	}
	while (sinal)
	{
		delay_100us();
		T++;
	}
}

void calcula_freq(int t)
{	
	float periodo;
	periodo = t*0.0001;
	
	frequencia = 1/periodo;
}

char converte(int num)
{
	char BIN[] = {0b00000000, 0b00000001,0b00000010,0b00000011,0b00000100,0b00000101,0b00000110,
	0b00000111, 0b00001000,0b00001001,0b00010000,0b00010001,0b00010010, 0b00010011, 0b00010100,
	0b00010101, 0b00010110, 0b00010111, 0b00011000, 0b00011001, 0b00100000, 0b00100001, 0b00100010,
	0b00100011, 0b00100100, 0b00100101, 0b00100110, 0b00100111, 0b00101000, 0b00101001, 0b00110000,
	0b00110001, 0b00110010, 0b00110011, 0b00110100, 0b00110101, 0b00110110, 0b00110111, 0b00111000,
	0b00111001, 0b01000000, 0b01000001, 0b01000010, 0b01000011, 0b01000100, 0b01000101, 0b01000110,
	0b01000111, 0b01001000, 0b01001001, 0b01010000, 0b01010001, 0b01010010, 0b01010011, 0b01010100,
	0b01010101, 0b01010110, 0b01010111, 0b01011000, 0b01011001, 0b01100000, 0b01100001, 0b01100010,
	0b01100011, 0b01100100, 0b01100101, 0b01100110, 0b01100111, 0b01101000, 0b01101001, 0b01110000,
	0b01110001, 0b01110010, 0b01110011, 0b01110100, 0b01110101, 0b01110110, 0b01110111, 0b01111000,
	0b01111001, 0b10000000, 0b10000001, 0b10000010, 0b10000011, 0b10000100, 0b10000101, 0b10000110,
	0b10000111, 0b10001000, 0b10001001, 0b10010000, 0b10010001, 0b10010010, 0b10010011, 0b10010100,
	0b10010101, 0b10010110, 0b10010111, 0b10011000,0b10011000, 0b10011001};

   	if(num>100)
        		return 0b11101110; 
   	else
        		return BIN[num];
}

void conf()
{
	OSCCON = 0b1110010;
	TRISC = 0b00000000;
	TRISA = 0b00000000;
	ADCON1 = 0b00001111;
	Nop ();
	TRISAbits.RA5 = 1;
}

void main ()
{
	conf();

	while (1)
	{
		mede_periodo();
		calcula_freq (T);
		saida = converte(frequencia);
	}
}