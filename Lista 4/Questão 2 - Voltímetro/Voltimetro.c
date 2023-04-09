#include <p18f4520.h>

#pragma config WDT = OFF //Configura o PIC para n�o reiniciar 

//*********************************************NOMEA��O DAS PORTAS DO LCD***********************************************************

#define RS PORTCbits.RC0
#define E PORTCbits.RC1

//*********************************************DECLARA��O DE VARIAVEIS E FUN��ES*****************************************************
char controle;
int resultado, voltagem1, voltagem2, voltagem3;
 
void delay_milisegundo();
void envia_comando();
void config_PIC();
void config_LCD();
void config_registradores();
void mostra_voltagem();
void interrupcao_alta();
void mostra();

//***************************************************FUN��O QUE RETORNA X MILIS******************************************************

void delay_milisegundo(unsigned long int milisegundos)
{
	unsigned long int i;
	
	for(milisegundos; milisegundos >0; milisegundos--)
	{
		for(i=0; i<79; i++) {};
	}
}

//*******************************************************CONFIGURA��ES DO LCD*********************************************************

void envia_comando(int Comando)
{
	RS = 0;          //Enviando comando
	PORTD = Comando; // Colocando o comando na porta D
	E = 1;
	E = 0;
	delay_milisegundo(3);
}

void envia_dado(char dado)
{
	RS = 1; 
	PORTD = dado;         //4 bits mais significativos do dado
	E = 1;
	E = 0;
	delay_milisegundo(3);
	PORTD = dado << 4;    //4 bits menos significativos do dado
	E = 1;
	E = 0;
	delay_milisegundo(3);
}

void config_LCD(void)
{
	//Setando 
	delay_milisegundo(70);
	envia_comando(0b00100000);
	envia_comando(0b00100000);
	envia_comando(0b10000000);
	envia_comando(0b00000000);
	envia_comando(0b11000000);
	//Limpando display
	envia_comando(0b00000000);
	envia_comando(0b00010000);
}

//***********************************************************CONFIGURA��ES DO PIC******************************************************
void config_PIC(void)
{
	TRISC = 0; //PORTC como saida
	TRISD = 0; //PORTD como saida
	TRISA = 0b11111111;
}

void config_registradores(void)
{
	INTCONbits.GIE = 1;   //Habilita as interrup��es de alta prioridade
	INTCONbits.PEIE = 1;  //Habilita as interrup��es de baixa prioridade
	
	//Configurando para a convers�o AD
	ADCON0bits.CHS3 = 0;
	ADCON0bits.CHS2 = 0;  //canal AN0 para recebe a voltagem
	ADCON0bits.CHS1 = 0;
	ADCON0bits.CHS0 = 0;
	ADCON0bits.ADON = 1;  //Habilita conversor AD
	
	ADCON1 = 0b00001011;  //AN0,AN1,AN2 e AN3 configurados para leitura anal�gica
	
	ADCON2bits.ADFM = 1;  //10 bits mais a direita
	ADCON2bits.ACQT2 = 1;
	ADCON2bits.ACQT1 = 1; //Tempo de aquisi��o = 20 
	ADCON2bits.ACQT0 = 1;
	ADCON2bits.ADCS2 = 1;
	ADCON2bits.ADCS1 = 0; //clock do conversor � Fosc/4
	ADCON2bits.ADCS0 = 0;
	
	//Habilitando as interrup��es do conversor AD
	PIE1bits.ADIE = 1;   //Habilita as interrup��es A/D
	IPR1bits.ADIP = 1;   //As interrup��es A/D possuem prioridade alta
	
	delay_milisegundo(1);
	ADCON0bits.GO = 1;   //Inicia a convers�o
}

//*******************************************************MOSTRA VOLTAGEM NO LCD*********************************************************

void mostra_voltagem(int ordem)
{
	envia_dado('V');
	envia_dado(ordem + 48);
	envia_dado(':');
	envia_dado(voltagem1 + 48);
	envia_dado(',');
	envia_dado(voltagem2 + 48);
	envia_dado(voltagem3 + 48);
	envia_dado(' ');
	
	ADCON0bits.GO = 1;
}

//*****************************************************INTERRUP��O DE ALTA PRIORIDADE*****************************************************

#pragma code alta_prioridade = 0x08

void interrupcao_de_alta_prioridade(void)
{
  _asm goto interrupcao_alta _endasm
}

#pragma code
#pragma interrupt interrupcao_alta //Configurando para interrup��es de alta prioridade

void interrupcao_alta(void)
{
	if(PIR1bits.ADIF)
	{
		resultado = (((int)ADRESH << 8) | (ADRESL));
		voltagem1 = resultado * 0.0048828125;
		voltagem2 = (((resultado * 0.0048828125) * 10) - (voltagem1 * 10));
		voltagem3 =  ((((resultado * 0.0048828125) * 100) - (voltagem1 * 100)) - (voltagem2*10));
		PIR1bits.ADIF = 0;
		controle++;
		ADCON0 = (controle << 2) | 0b00000001;
		if(controle == 4)
		{
			controle = 0;
			ADCON0 = (controle << 2) | 0b00000001;
		}
	}
}

//********************************************************MOSTRA AS 4 ENTRADAS********************************************************
void mostra(void)
{
 	mostra_voltagem(1);
    mostra_voltagem(2);
    envia_comando(0b11000000);
    envia_comando(0b00000000);
    mostra_voltagem(3);
    mostra_voltagem(4);
    envia_comando(0b00000000);
    envia_comando(0b00100000);
}

void main()
{
    config_PIC();
    config_LCD();
    config_registradores();
    while(1)
     {
        mostra();
      }

}