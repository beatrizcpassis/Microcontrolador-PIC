#include <p18f4520.h>

//*********************************************NOMEANDO AS PORTAS DO TECLADO*****************************************
#define coluna1 PORTBbits.RB0
#define coluna2 PORTBbits.RB1
#define coluna3 PORTBbits.RB2

#define linhaA PORTBbits.RB3
#define linhaB PORTBbits.RB4
#define linhaC PORTBbits.RB5
#define linhaD PORTBbits.RB6

//******************************************NOMEANDO AS PORTAS DO LCD************************************************
#define RS PORTCbits.RC0
#define E PORTCbits.RC1

//**********************************************CHAMADO DE FUNCOES***************************************************
int digitos = 0;

void delay_milisegundo();
void envia_comando();
void mostra_dado();
void config_PIC();
void config_registradores();
void config_LCD();
void varredura_teclado();
void reseta_LCD();
void interrupcao_alta();

//*******************************************FUNÇÃO DELAY DE X MILIS*************************************************

void delay_milisegundo(unsigned long int milisegundos)
{
	 int i;
	
	for(milisegundos; milisegundos >0; milisegundos--)
	{
		for(i=0; i<109; i++) {};
	}
}

//*********************************************CONFIGURACOES DO LCD****************************************************

void envia_comando(int Comando)
{
	RS = 0; // Colocando para enviar comando
	PORTD = Comando; // Colocando o comando na porta D
	E = 1;
	E = 0;
	delay_milisegundo(3);
}

void mostra_dado(char dado)
{

	RS = 1; //Enviando dado
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

//**********************************************CONFIGURAÇOES PIC********************************************************

void config_PIC(void)
{
	TRISB = 0b00000111;   //RB0 a RB2 como entrada e RB4 a RB7 como saida
	linhaA = 0;
	linhaB = 0;
	linhaC = 0;
	linhaD = 0;
	TRISC = 0;           //Configura PORTC como saida
	TRISD = 0;           //Configura PORTD como saida
	ADCON1 = 0b00001111; //Configura todas as portas do PIC para digital
}

void config_registradores(void)
{

	INTCONbits.GIE = 1;      //Habilita as interrupções de alta prioridade
	INTCONbits.PEIE = 1;     //Habilita as interrupções de baixa prioridade
	INTCONbits.INT0IE = 1;   //Habilita as interrupções externas no INT0
	INTCON3bits.INT1IE = 1;  //Habilita as interrupções externas no INT1
	INTCON3bits.INT2IE = 1;  //Habilita as interrupções externas no INT2
	INTCON3bits.INT1IP = 1;  //Setando as interrupções vindas de INT1 como alta prioridade
	INTCON3bits.INT2IP = 1;  //Setando as interrupções vindas de INT2 como alta prioridade
	INTCON2bits.INTEDG0 = 1; //As interrupções no INT0 ocorrem na subida
	INTCON2bits.INTEDG1 = 1; //As interrupções no INT1 ocorrem na subida
	INTCON2bits.INTEDG2 = 1; //As interrupções no INT2 ocorrem na subida
}

//**************************************************VARREDURA DO TECLADO***************************************************

void varredura_teclado(void)
{
	linhaA = 1;
	delay_milisegundo(2);
	linhaA = 0;
	
	linhaB = 1;
	delay_milisegundo(2);
	linhaB = 0;
	
	linhaC = 1;
	delay_milisegundo(2);
	linhaC = 0;
	
	linhaD = 1;
	delay_milisegundo(2);
	linhaD = 0;
}

//**************************************************RESETA LCD***************************************************************

void reseta_LCD(void)
{
	if(digitos == 16)
	{
		envia_comando(0b11000000);
		envia_comando(0b00000000);
	}
	if(digitos == 33)
	{
		//Limpa display
		envia_comando(0b00000000);
		envia_comando(0b00010000);
		digitos = 0;
	}
}

//******************************************INTERRUPÇÃO DE ALTA PRIORIDADE****************************************************

#pragma code alta_prioridade = 0x08

void interrupcao_de_alta_prioridade(void)
{
  _asm goto interrupcao_alta _endasm
}

#pragma code
#pragma interrupt interrupcao_alta //Configurando para interrupções de alta prioridade

void interrupcao_alta(void)
{
	if(INTCONbits.INT0IF) //Se if retorna 1, então a interrupção ocorreu em INT0
	{
		if(linhaA) 
		{
			while(coluna1){};
			mostra_dado('1'); //Mostra 1 no LCD
			digitos++;
		}
		if(linhaB)
		{
			while(coluna1){}
			mostra_dado('4'); //Mostra 4 no LCD
			digitos++;
		}
		if(linhaC)
		{
			while(coluna1){}
			mostra_dado('7'); //Mostra 7 no LCD
			digitos++;
		}
		if(linhaD)
		{
			while(coluna1){}
			mostra_dado('*'); //Mostra * no LCD
			digitos++;
		}
		INTCONbits.INT0IF = 0;
	}
	
	if(INTCON3bits.INT1IF) //Se if retornar 1, então a interrupção ocorreu em INT1
	{	
		if(linhaA) 
		{
			while(coluna2){}
			mostra_dado('2'); //Mostra 2 no LCD
			digitos++;
		}
		if(linhaB)
		{
			while(coluna2){}
			mostra_dado('5'); //Mostra 5 no LCD
			digitos++;
		}
		if(linhaC)
		{
			while(coluna2){}
			mostra_dado('8'); //Mostra 8 no LCD
			digitos++;
		}
		if(linhaD)
		{
			while(coluna2){}
			mostra_dado('0'); //Mostra 0 no LCD
			digitos++;
		}
		INTCON3bits.INT1IF = 0;
	}

	if(INTCON3bits.INT2IF) //Se if retornar 1, então a interrupção ocorreu em INT2
	{	
		if(linhaA) 
		{
			while(coluna3){}
			mostra_dado('3'); //Mostrar o 3 no LCD
			digitos++;
		}		
		if(linhaB)
		{
			while(coluna3){}
			mostra_dado('6'); //Mostrar o 6 no LCD
			digitos++;
		}
		if(linhaC)
		{
			while(coluna3){}
			mostra_dado('9'); //Mostrar o 9 no LCD
			digitos++;
		}
		if(linhaD)
		{
			while(coluna3){}
			mostra_dado('#'); //Mostrar o # no LCD
			digitos++;
		}
		INTCON3bits.INT2IF = 0;	
	}
}

void main()
{
    config_PIC();
    config_registradores();
    config_LCD();
    while(1)
     {
       varredura_teclado();
       reseta_LCD();
     }
}