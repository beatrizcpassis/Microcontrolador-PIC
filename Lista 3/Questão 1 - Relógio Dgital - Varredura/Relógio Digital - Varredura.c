#include <p18f4520.h>

#pragma config WDT = OFF // PIC não reinicia

int segundos = 0, minutos = 0, horas = 0, controle_relogio = 0, ajuste_relogio = 0;

char mostra_display[10] = 
{
0b10111111,
0b10000110,
0b11011011,
0b11001111,
0b11100110,
0b11101101,
0b11111101,
0b10000111,
0b11111111,
0b11101111
};

char habilita_display[7] =
{
0b11111111,
0b11011111,
0b11101111,
0b11110111,
0b11111011,
0b11111101,
0b11111110
}; 
 
void delay_milis();
void config_PIC();
void config_registradores();
void mostrar_relogio();
void atualizar_relogio();
void inrerrupcao_alta();


void delay_milis(unsigned long int milisegundos)
{
	unsigned long int i;
	for(milisegundos; milisegundos >0; milisegundos--)
	{
		for(i=0; i<79; i++) {};
	}
}

void config_PIC(void)
{
	TRISC = 0b10000000; // R7 como pino de entrada
	TRISD = 0; 
	TRISBbits.RB0 = 1; 
	TRISBbits.RB1 = 1; 
	TRISBbits.RB2 = 1; 
	ADCON1 = 0x0F; // Configura as portas do PIC para digital
}

void config_registradores(void)//Habilita niveis de prioridade e interrupções
{

	RCONbits.IPEN = 1; // Habilita os níveis de prioridade
	INTCONbits.GIE = 1; // Habilita as interrupções de alta prioridade
	INTCONbits.PEIE = 1; // Habilita as interrupções de baixa prioridade
	INTCONbits.INT0IE = 1; // Habilita as interrupções externas no INT0
	INTCON3bits.INT1IE = 1; // Habilita as interrupções externas no INT1
	INTCON3bits.INT2IE = 1; // Habilita as interrupções externas no INT2
	INTCON3bits.INT1IP = 1; // Setando as interrupções vindas de INT1 como alta prioridade
	INTCON3bits.INT2IP = 1; // Setando as interrupções vindas de INT2 como alta prioridade
	INTCON2bits.INTEDG0 = 1; // As interrupções no INT0 ocorrem na subida
	INTCON2bits.INTEDG1 = 1; // As interrupções no INT1 ocorrem na subida
	INTCON2bits.INTEDG2 = 1; // As interrupções no INT2 ocorrem na subida

	// Timer 0 (configurações)
	T0CONbits.TMR0ON = 1; // Ligando timer 0
	T0CONbits.T08BIT = 0; // Timer 0 configurado para 16 bits
	T0CONbits.T0CS = 0; // Fonte do timer 0 é o clock interno do PIC
	T0CONbits.PSA = 0; // Habilitando Prescaler
	T0CONbits.T0PS2 = 1;
	T0CONbits.T0PS1 = 1;    // 1:128 Prescaler
	T0CONbits.T0PS0 = 0;
	TMR0H = 0xC2; // Parte alta de onde começa a contar no Timer 0
	TMR0L = 0xF7; // Parte baixa de onde começa a contar no Timer 0 (Timer 0 conta 1s)
	// Parte alta junto com a parte baixa formam o número de onde o timer começa a contar
	
	// Timer 0 (Habilitando)
	
	INTCONbits.TMR0IE = 1; // Habilitando as interrupções vindas do Timer 0
	INTCON2bits.TMR0IP = 1; // Setando as interrupções vindas de Timer 0 como alta prioridade
	
	// Timer 1 (configurações)
	
	T1CONbits.TMR1ON = 1; // Ligando timer 1
	T1CONbits.T1RUN = 0; // Clock vem de uma fonte diferente do oscilador do timer 1
	T1CONbits.T1OSCEN = 0; // Oscilador do timer 1 desligado
	T1CONbits.RD16 = 1; // Timer 1 configurado para 16 bits
	T1CONbits.TMR1CS = 0; // Fonte do timer 1 é o clock interno
	TMR1H = 0xD8; // Parte alta de onde começa a contar no Timer 1
	TMR1L = 0xF0; // Parte baixa de onde começa a contar no Timer 1 (Timer 1 conta 5 ms)
	// Parte alta junto com a parte baixa formam o número de onde o timer começa a contar
	
	// Timer 1 (Habilitando)
	
	PIE1bits.TMR1IE = 1; // Habilitando as interrupções vindas do Timer 1
	IPR1bits.TMR1IP = 1; // Setando as interrupções vindas de Timer 1 como alta prioridade
	
	// Timer 3 (configurações)
	
	T3CONbits.TMR3ON = 1; // Ligando timer 3
	T3CONbits.T3CKPS1 = 1; // 
	T3CONbits.T3CKPS0 = 1; //  1:8 Prescaler
	T3CONbits.RD16 = 1; // Timer 3 configurado para 16 bits
	T3CONbits.TMR3CS = 0; // Fonte do timer 1 é o clock interno
	TMR3H = 0x3C; // Parte alta de onde começa a contar no Timer 3
	TMR3L = 0xB0; // Parte baixa de onde começa a contar no Timer 3  (Timer 3 conta 200 ms)
	// Parte alta junto com a parte baixa formam o número de onde o timer começa a contar
	
	// Timer 3 (Habilitando)

	PIE2bits.TMR3IE = 1; // Habilitando as interrupções vindas do Timer 3
	IPR2bits.TMR3IP = 1; // Setando as interrupções vindas de Timer 3 como alta prioridade

}

// Função que mostra as horas, minutos e segundos no display
void mostrar_relogio(int controle_relogio)
{

	if(controle_relogio == 0) // Mostrar unidade do Display
	{	
		PORTD = habilita_display[0]; // Primeiro se é desligado todos os displays
		PORTC = mostra_display[segundos%10]; // Depois se é mostrado a unidade dos segundos
		PORTD = habilita_display[1]; // E por último se é abilitado apenas o display da unidade dos segundos
		delay_milis(1);
	}
	
	if(controle_relogio == 1) // Mostrar dezena do Display
	{	
		PORTD = habilita_display[0]; // Primeiro se é desligado todos os displays
		PORTC = mostra_display[(segundos/10) %10]; // Depois se é mostrado a dezena dos segundos
		PORTD = habilita_display[2]; // E por último se é abilitado apenas o display da dezena dos segundos
		delay_milis(1);
	}

	if(controle_relogio == 2) // Mostrar centena do Display
	{
		PORTD = habilita_display[0]; // Primeiro se é desligado todos os displays
		PORTC = mostra_display[minutos%10]; // Depois se é mostrado a unidade dos minutos
		PORTD = habilita_display[3]; // E por último se é abilitado apenas o display da unidade dos minutos
		delay_milis(1);
	}

	if(controle_relogio == 3) // Mostrar unidade de milhar do Display
	{		
		PORTD = habilita_display[0]; // Primeiro se é desligado todos os displays
		PORTC = mostra_display[(minutos/10) %10]; // Depois se é mostrado a dezena dos minutos
		PORTD = habilita_display[4]; // E por último se é abilitado apenas o display da dezena dos minutos
		delay_milis(1);
	}
	
	if(controle_relogio == 4) // Mostrar dezena de milhar do Display
	{
		PORTD = habilita_display[0]; // Primeiro se é desligado todos os displays
		PORTC = mostra_display[horas%10]; // Depois se é mostrado a unidade das horas
		PORTD = habilita_display[5]; // E por último se é abilitado apenas o display da unidade das horas
		delay_milis(1);
	}
	
	if(controle_relogio == 5) // Mostrar centena de milhar do Display
	{	
		PORTD = habilita_display[0]; // Primeiro se é desligado todos os displays
		PORTC = mostra_display[(horas/10) %10]; // Depois se é mostrado a dezena das horas
		PORTD = habilita_display[6]; // E por último se é abilitado apenas o display da dezena das horas
		delay_milis(1);
	}	
}

// Função que conta as horas, minutos e segundos
void atualizar_relogio(void)
{
	segundos++;

	if(segundos == 60)
	{
		segundos = 0;
		minutos++;
	
		if(minutos == 60)
		{
			minutos = 0;
			horas++;

			if(horas == 24)
			{
				horas = 0;
			}
		}
	}
}

// Alocando no endereço 0x08 o que deve ser feito na ocorrência de uma interrupção de alta prioridade

#pragma code alta_prioridade = 0x08

void interrupcao_de_alta_prioridade(void)
{
  _asm goto inrerrupcao_alta _endasm
}

#pragma code

#pragma interrupt inrerrupcao_alta // Configurando a resposta do PIC para interrupções de alta prioridade

// Função que checará onde a interrupção de alta prioridade ocorreu
void inrerrupcao_alta(void)
{

	if(INTCONbits.INT0IF) // Se esse if retornar 1, então o botão de ajuste foi pressionado
	{
		INTCON2bits.INTEDG0 = ~INTCON2bits.INTEDG0; // A interrupção ocorre na beira oposta para a mundança do botão sempre ser detectada
		INTCONbits.INT0IF = 0;
		TMR3H = 0x3C; 
		TMR3L = 0xB0; 
		ajuste_relogio = 0; // Se começará a contar os 3 segundos do ajuste	
	}
	
	if(INTCON3bits.INT1IF) // Se esse if retornar 1, então o botão de aumentar 1 hora foi pressionado
	{
	
		if(TRISCbits.RC7 == 0) // Se RC7 está configurado como saída, então o modo ajuste está ligado
		{
			TMR3H = 0x3C; 
			TMR3L = 0xB0; 
			ajuste_relogio = 0; // O tempo no timer 3 começa a contar os 3 segundos do 0, pois o botão das horas foi pressionado 
			horas++;
			if(horas == 24)
				{
					horas = 0;
				}	
		}
		
		INTCON3bits.INT1IF = 0;	
	}
	
	if(INTCON3bits.INT2IF) // Se esse if retornar 1, então o botão de aumentar 1 minuto foi pressionado
	{
	
		if(TRISCbits.RC7 == 0) // Se RC7 está configurado como saída, então o modo ajuste está ligado
		{
			TMR3H = 0x3C; 
			TMR3L = 0xB0; 
			ajuste_relogio = 0; // O tempo no timer 3 começa a contar os 3 segundos do 0, pois o botão dos minutos foi pressionado 
			minutos++;
			if(minutos == 60)
				{
					minutos = 0;
					horas++;
		
					if(horas == 24)
					{
						horas = 0;
					}
				}
		}
		
		INTCON3bits.INT2IF = 0;
	}
	
	if(INTCONbits.TMR0IF) // A cada 1 segundo, o relógio é atualizado
	{
		TMR0H = 0xC2; 
		TMR0L = 0xF7; 
		atualizar_relogio();
		INTCONbits.TMR0IF = 0; 
	}
	
	if(PIR1bits.TMR1IF) // A cada 5 milisegundos, se é mostrado as horas, minutos e segundos no Display
	{
	
		TMR1H = 0xD8; 
		TMR1L = 0xF0;
		mostrar_relogio(controle_relogio);
		controle_relogio++; // A variável controle será responsável por controlar qual digito será mostrado a cada interrupção do timer 1
		if(controle_relogio == 6)
			{
				controle_relogio = 0; // Mostrou-se o último dígito do display
			}
		PIR1bits.TMR1IF = 0; 
	
	}
	
	if(PIR2bits.TMR3IF) // Timer que conta 200 ms 
	{
		TMR3H = 0x3C; // Parte alta de onde começa a contar no Timer 1
		TMR3L = 0xB0; // Parte baixa de onde começa a contar no Timer 1
		ajuste_relogio++;
		if( ajuste_relogio == 15) // Quando Ajuste chega a 15, 3 segundos se passaram 
			{
			if(PORTBbits.RB0 == 1) // Se após esses 3 segundos, o botão de ajuste continuar sendo 1, 
								   //o relógio entra em modo de ajuste e o timer volta a contar os 3 segundos novamente
				{
					TRISCbits.RC7 = 0;
					ajuste_relogio = 0; 
				}
			else // Se não, ele apenas volta a contar os 3 segundos novamente
				{
					TRISCbits.RC7 = 1;
					ajuste_relogio = 0;
				}
			}
		
		PIR2bits.TMR3IF = 0;	
	}
}


void main(void)
{
config_PIC();
config_registradores();

while(1)
{}
}