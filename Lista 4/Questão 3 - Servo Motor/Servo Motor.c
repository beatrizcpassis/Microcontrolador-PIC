#include <p18f4550.h>
#include <delays.h>
#include <adc.h>
#include <math.h>

//*************************************************NOMEA플O DAS PORTAS************************************************************
#define barramento PORTD
#define RS PORTCbits.RC5
#define WR PORTCbits.RC6
#define E PORTCbits.RC7
#define colunas PORTA
#define linhas PORTB
#define servo LATCbits.LATC0

//******************************************DECLARA플O DAS VARIAVEIS E FUN합ES****************************************************
int valor = 0;
int tempo = 0;
int teclado = 0;
int servo_motor = 72;
char texto_inicio[] = "1 - INICIAR";
char texto_angulo[] = "VALOR ANGULO: ";
int digitos[12] = {1,2,3,0xF0,5,6,7,8,9,0xF1,0xF3,0xF2};
int numero = 0;

int varredura();
void escolhe_angulo();
void servo_delay();
void envia_comando();
void envia_dado();
char string();

//**************************************************CONFIGURA합ES DO DISPLAY*****************************************************
void envia_comando(char dado)
{
	E=0;   
	RS=0;  
	WR=0;  
	barramento=dado;
	E=1;
	E=0;  
	Delay1KTCYx(20);
}

void envia_dado(char dado)
{
	E=0;
	RS=1;
	WR=0;
	barramento=dado;
	E=1;
	E=0;
	Delay1KTCYx(20); 
}
char string(char* dado) 
{
	int i;
	for(i=0; dado[i] != '\0'; i++) envia_dado(dado[i]);
}
void inicializa(void)
{
	OpenADC(0x00,0x00,ADC_0ANA);
	CloseADC();
	TRISA = 0x0F;
 	TRISB = 0xF0;
	TRISC=0;
	TRISD=0b00000000;
	envia_comando(0x38);
	envia_comando(0x38);
	envia_comando(0x06);
	envia_comando(0x0C);
	envia_comando(0x01);
}

int posicao_LCD(int linha, int coluna)
{
	envia_comando(0x80 +linha*0x40 + coluna);
}

//******************************************************CONFIGURA합ES PARA LEITURA DO ANGULO****************************************
void servo_delay(int x)
{
	int k;
	for(k = 0; k<x; k++) Delay1TCY();
}

int varredura()
{
	int i,j;
	for(i = 0; i<4; i++)
	{
		linhas = (1<<i);
		for(j=0; j<3; j++)
		{	
			if(colunas == 1<<j)
			{
				Delay10KTCYx(10);
				return digitos[(i*3) + j];
			}	 
			
		}	
	}	
}

void ciclo(char varredura)
{
	if(varredura != 4)
	{
		if(varredura == 0xF0) varredura = 4;
		if(varredura == 0xF3) varredura = 0;
		envia_comando(0xc0+numero);
		envia_dado('0' + varredura);
		Delay10KTCYx(1);
		numero++;
	}
	varredura = 0;
}

void escolhe_angulo(void)
{
	servo = 1;
	servo_delay(servo_motor);
	servo = 0;
	posicao_LCD(0,0);
	string(texto_inicio);
	posicao_LCD(1,0);
	teclado = varredura();
	switch(teclado)
	{
		case 1:
		{
			envia_comando(1);
			posicao_LCD(0,0);
			string(texto_angulo);
			while(1)
			{
				valor = varredura();
				Delay10KTCYx(20);
				if(valor == 0xF2) break;
				if(valor == 0 || valor == 4) continue;
				ciclo(valor);
				if(valor == 0xF0) valor = 4;
				if(valor == 0xF3) valor = 0;
				(numero == 0)?(tempo  = tempo + valor):(tempo  = tempo*pow(10,1) + valor);
			}
			servo_motor = 72 + 2*(tempo);
			tempo = 0;
			numero = 0;
			valor = 0;
			break;					
		}
	}	
}	
	
void main()
	{	
		inicializa();
		while (1)
			{
			escolhe_angulo();	
			}
	}
