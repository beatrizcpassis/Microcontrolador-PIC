#include <p18f4520.h>

#include <string.h>

#pragma config WDT = OFF

#define RS PORTCbits.RC0
#define RW PORTCbits.RC1
#define E PORTCbits.RC2

#define coluna_1 PORTBbits.RB0
#define coluna_2 PORTBbits.RB1
#define coluna_3 PORTBbits.RB2

#define linha_A PORTBbits.RB3
#define linha_B PORTBbits.RB4
#define linha_C PORTBbits.RB5
#define linha_D PORTBbits.RB6

#define led PORTBbits.RB7

char tecla;
int tamanho_senha = 3;
char senha_correta[3] = "786";
char senha_digitada[3];
int count = 0;

void delay_ms (int);
void milis (void);
void delay_s (int);
void envia_comando_lcd (char);
void envia_dado_lcd (char);
void inicializa_lcd (void);
void configura_pic (void);
char le_teclado (void);

void delay_ms (int ms)
{

	for (ms; ms > 0; ms--) 
	{
		milis();
	};
}

void milis (void) 
{
	unsigned char j;
	unsigned int i;
	for (i = 0; i < 103; i++) {j = 0;}
}

void delay_s (int s) 
{

	unsigned long int i;
	for (s; s > 0; s--) 
	{
		for (i = 0; i < 83333; i++) {};
	}	
}

void envia_comando_lcd (char dado) 
{
	RS = 0;
	RW = 0;
	E = 1;
	PORTD = dado;
	E = 0;
	delay_ms(3);
}

void envia_dado_lcd (char comando) 
{
	RS = 1;
	RW = 0;
	E = 1;
	PORTD = comando;
	E = 0;
	delay_ms(3);
}

void inicializa_lcd () 
{
	delay_ms(30); 
	envia_comando_lcd(0b00111000); 
	envia_comando_lcd(0b00001110);
	envia_comando_lcd(0x40);
	envia_comando_lcd(0b00000001);
}

void conf () 
{
	TRISC = 0b00000000; 
	TRISD = 0b00000000; 
	TRISB = 0b01111000; 
	ADCON1 = 0b00001111;
}

char le_teclado () 
{
	delay_ms(2);
	coluna_1 = 0;
	coluna_2 = 1;
	coluna_3 = 1;

	if (!linha_A) 
	{
		while (!linha_A) {};
		return '1';
	}

	if (!linha_B) 
	{
		while (!linha_B) {};
		return '4';
	}

	if (!linha_C) 
	{
		while (!linha_C) {};
		return '7';
	}

	if (!linha_D) 
	{
		while (!linha_D) {};
		return '*';
	}

	delay_ms(2);
	coluna_1 = 1;
	coluna_2 = 0;
	coluna_3 = 1;

	if (!linha_A) 
	{
		while (!linha_A) {};
		return '2';
	}

	if (!linha_B)
	{
		while (!linha_B) {};
		return '5';
	}

	if (!linha_C) 
	{
		while (!linha_C) {};
		return '8';
	}

	if (!linha_D) 
	{
		while (!linha_D) {};
		return '0';
	}

	delay_ms(2);
	coluna_1 = 1;
	coluna_2 = 1;
	coluna_3 = 0;

	if (!linha_A) 
	{
		while (!linha_A) {};
		return '3';
	}

	if (!linha_B) 
	{
		while (!linha_B) {};
		return '6';
	}

	if (!linha_C) 
	{
		while (!linha_C) {};
		return '9';
	}

	if (!linha_D) 
	{
		while (!linha_D) {};
		return '#';
	}
	return 0;
}

void senha() 
{
	envia_dado_lcd(tecla);
	
	senha_digitada[count++] = tecla;
	if (count == tamanho_senha) {
		if (strncmp(senha_digitada, senha_correta, tamanho_senha) == 0) {
			led = 1;
			delay_s(1);
			envia_comando_lcd(0b00000001);
			count = 0;
			led = 0;
		}
		else {
			led = 0;
			delay_s(1);
			envia_comando_lcd(0b00000001);
			count = 0;	
		}
	}	 
}

void main () 
{
	conf();
	inicializa_lcd();
	
	while (1) 
	{
		tecla = le_teclado();
		if (tecla) 
		{
			senha();
		}	
	}
}