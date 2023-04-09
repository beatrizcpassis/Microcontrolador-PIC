#include<p18f4520.h> 

#define botao_minutos PORTAbits.RA0
#define botao_horas PORTAbits.RA1

unsigned char dh,uh,dm,um,ds,us;

void delay_ms (int);
void milis (void);
void delay (int);
void mostra_relogio (void);
void atualiza_relogio (void);
void ajuste_relogio(void);

void delay_ms (int ms)
{
	for(ms;ms>0;ms--)
	{
		milis();
	}
}

void milis (void)
{
	unsigned char j;
	unsigned int	 i;
	for(i=0;i<103;i++) {j=0;}; //loop de 1ms
}

void delay (int s)
{
	unsigned long int i;
	for(s;s>0;s--)
	{
		for(i=0;i<83333;i++){;}; //loop de 1 seg 
	}
}

void inicializa_relogio(void)
{
	dh =0;
	uh =0;
	dm =0;
	um =0;
	ds =0;
	us =0;
}

void mostra_relogio(void)
{
	unsigned char aux,aux1,aux2;
	aux = ds;
	aux = aux <<4;
	aux = aux | us;
	
	aux1 = dm;
	aux1 = aux1 <<4;
	aux1 = aux1 | um;

	aux2 = dh;
	aux2 = aux2 <<4;
	aux2 = aux2 | uh;

	PORTD = aux2;
	PORTB = aux1;
	PORTC = aux;
}

void atualiza_relogio(void)
{
	us ++;
	if(us==10)
	{
		us=0;
		ds++;
	}
	if(ds==6 && us==0)
	{
		us=0;
		ds=0;
		um++;
	}
	if(um==10)
	{
		um=0;
		dm++;
	}
	if(dm==6 && um==0)
	{
		um=0;
		dm=0;
		uh++;
	}
	if(uh==10)
	{
		uh=0;
		dh++;
	}
	if(dh==2 && uh==4)
	{
		inicializa_relogio();
	}
}

void ajuste_relogio(void)
{
	if(botao_minutos)
	{
		while(botao_minutos){}	//Espera soltar o botão
		um++;
		if(um==10)
		{
			um=0;
			dm++;
		}
		if(dm==6 && um==0)
		{
			um=0;
			dm=0;
			uh++;
		}
		if(uh==10)
		{
			uh=0;
			dh++;
		}
		if(dh==2 && uh==4)
		{
			inicializa_relogio();
		}
	}
}

void conf()
{
	TRISC = 0b00000000; //CONFIGURA PORTA C COMO SAÍDA
	TRISB = 0b00000000; //PORTA B COMO SAÍDA
	TRISD = 0b00000000; //PORTA D COMO SÁIDA
}

main ()
{
	conf();
	inicializa_relogio();

	while(1)
	{
		ajuste_relogio();
		mostra_relogio();
		
		delay(1);
		atualiza_relogio();	
		
	}
}