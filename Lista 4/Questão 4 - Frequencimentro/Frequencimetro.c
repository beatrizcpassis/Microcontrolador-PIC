#include <p18f4520.h>

//******************************************************NOMEAÇÃO DAS PORTAS DO DISPLAY***************************************************
#define display PORTD                                   
#define p1 PORTCbits.RC4                             
#define p2 PORTCbits.RC5                             
#define p3 PORTCbits.RC6                             
#define p4 PORTCbits.RC7                             

//*****************************************************DECLARAÇÃO DAS VARIAVEIS E FUNÇOES************************************************
int captura, captura1,captura2, frequencia, i;
int digitos[4] = {1,2,3,4};

void interrupcao_alta();
void envia_7seg(int numero, int posicao);
void config_PIC();
void config_CCP();
void mostra_frequencia();

//******************************************************CONFIGURAÇÃO DO DISPLAY BCD*******************************************************
void envia_7seg(int numero, int posicao)
{
	display = 0b00000000;	
	if(posicao == 1)
	{
		p1 = 0;
		p2 = 1;
		p3 = 1;
		p4 = 1;
	}
	if(posicao == 2)
	{
		p1 = 1;
		p2 = 0;
		p3 = 1;
		p4 = 1;
	}
	if(posicao == 3)
	{
		p1 = 1;
		p2 = 1;
		p3 = 0;
		p4 = 1;
	}
	if(posicao == 4)
	{
		p1 = 1;
		p2 = 1;
		p3 = 1;
		p4 = 0;
	}
	if(numero == 1)display = 0b0110000;
	if(numero == 2)display = 0b1101101;
	if(numero == 3)display = 0b1111001;
	if(numero == 4)display = 0b0110011;
	if(numero == 5)display = 0b1011011;
	if(numero == 6)display = 0b1011111;
	if(numero == 7)display = 0b1110000;
	if(numero == 8)display = 0b1111111;
	if(numero == 9)display = 0b1111011;
	if(numero == 0)display = 0b1111110;
}

//*****************************************************CONFIGURAÇÕES DAS PORTAS E DO MODO DE CAPTURA*******************************************
void config_PIC(void)
{
	TRISCbits.RC2 = 1;	
	TRISD = 0b00000000; 
	TRISCbits.RC4 = 0;
	TRISCbits.RC5 = 0;
	TRISCbits.RC6 = 0;
	TRISCbits.RC7 = 0;
}

void config_CCP(void)
{
	CCP1CONbits.CCP1M3 = 0;	//captura a cada borda de descida do sinal
	CCP1CONbits.CCP1M2 = 1;			
	CCP1CONbits.CCP1M1 = 0;			
	CCP1CONbits.CCP1M0 = 0;	
	
	// configurando TMR1

	T1CON = 0b10010101;	// 16bits, prescaler 1:2, fosc/4, TMR1 ligado
	
	//configura interrupção CCP1
	
	IPR1bits.CCP1IP = 1; //alta priorida
	PIE1bits.CCP1IE = 1;		
	PIR1bits.CCP1IF = 0;	
	INTCONbits.GIE = 1;
	INTCONbits.PEIE = 1; //habilitador geral do CCP1	
}

//***********************************************************CONFIGURAÇÃO DA INTERRUPÇÃO*********************************************************
#pragma code interrupcao_de_alta_prioridade = 0x08

void interrupcao_de_alta_prioridade(void)
{
	_asm GOTO interrupcao_alta _endasm	// Desvia o programa para a função alta_prioridade.
}
#pragma code

#pragma interrupt interrupcao_alta

void interrupcao_alta()
{
	if (PIR1bits.CCP1IF)
	{
		TMR1H = 0;
		TMR1L = 7;			//desconta a execução do programa
		PIR1bits.CCP1IF = 0;
		captura1 = CCPR1H;
		captura2 = CCPR1L;	
	}		
}

//********************************************************CALCULO PARA MOSTRAR A FREQUENCIA NO DISPLAY*******************************************
void mostra_frequencia(void)
{
	captura = (captura1*256) + captura2; // junção das 2 partes da captura resultando no tempo em micro segundos
	frequencia = (1000000/captura); // calculando a frequencia
	digitos[0] = frequencia/1000;  //primeiro digito da frequencia
	digitos[1] = (frequencia - digitos[0]*1000)/100; //segundo digito da frequencia
	digitos[2] = (frequencia - digitos[0]*1000 - (digitos[1]*100))/10; //terceiro digito da frequencia
	digitos[3] = frequencia - digitos[0]*1000 - digitos[1]*100 - digitos[2]*10; //quarto digito da frequencia	
	for(i=0; i<4; i++)
	{                                              
		envia_7seg(digitos[i], (i+1));
	}
}

void main()
{
	config_PIC();
	config_CCP();

	while(1)
	{	
		mostra_frequencia();
	}
}
