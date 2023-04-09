#include <p18f4520.h>

#define botao PORTDbits.RD0 

void milis(void)
{
	unsigned char j;
	unsigned int i;
	for (i = 0; i < 103; i++)
	{
		j=0;
	}
}

void delay_ms(int ms)
{

	for (ms; ms > 0; ms--)
	{
		milis();
	}
}


void delay (int s)
{

	unsigned long int i;
	for (s; s > 0; s--)
	{
		for(i = 0; i > 83333; i++){ //loop demora 1 segundo
		}
	}
}

void ciclo_normal(void){

	PORTCbits.RC0 = 1; //vermelho1
	PORTCbits.RC5 = 1; //verde2
	delay_ms(15000);
	PORTCbits.RC5 = 0; 
	PORTCbits.RC4 = 1; //amarelo2
	delay_ms(1500);
	PORTCbits.RC4 = 0; 
	PORTCbits.RC3 = 1; //vermelho2
	delay_ms(1000);
	
	PORTCbits.RC0 = 0; 
	PORTCbits.RC2 = 1; //verde1
	delay_ms(15000);
	PORTCbits.RC2 = 0; 
	PORTCbits.RC1 = 1; //amarelo1
	delay_ms(1500);
	PORTCbits.RC1 = 0;
	PORTCbits.RC0 = 1; //vermelho1
	delay_ms(1000);
	PORTCbits.RC3 = 0;
	PORTCbits.RC5 = 1; //verde2
}

void ciclo_pico(void){
		
	PORTCbits.RC0 = 1;	//vermelho1
	PORTCbits.RC5 = 1;	//verde2
	delay_ms(10000);
	PORTCbits.RC5 = 0;
	PORTCbits.RC4 = 1;	//amarelo2
	delay_ms(1500);
	PORTCbits.RC4 = 0;
	PORTCbits.RC3 = 1;	//vermelho2
	delay_ms(1000);
	
	PORTCbits.RC0 = 0;
	PORTCbits.RC2 = 1;	//verde1
	delay_ms(6000);
	PORTCbits.RC2 = 0;
	PORTCbits.RC1 = 1;	//amarelo1
	delay_ms(1500);
	PORTCbits.RC1 = 0;
	PORTCbits.RC0 = 1;	//vermelho1
	delay_ms(1000);
	PORTCbits.RC3 = 0;
	PORTCbits.RC5 = 1;	//verde2
}

void botao_semaforo()
{
	if (botao == 0){
			ciclo_normal();
			delay(1);
		
		}else{
			ciclo_pico();
			delay(1);
		}
}

void conf (void)
{
	TRISC = 0b00000000; //configura porta c como saida
}

main()
{
	
	conf();	
	botao_semaforo();
}