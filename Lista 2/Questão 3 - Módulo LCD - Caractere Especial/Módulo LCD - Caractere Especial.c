#include <p18f4520.h>

#define barramento_dados PORTD
#define RS PORTCbits.RC0
#define RW PORTCbits.RC1
#define E PORTCbits.RC2

void envia_comando_lcd(char comando);
void envia_dado_lcd(char dado);
void mensagem_lcd(char *mensagem);
void inicializa_lcd(void);

char mensagem1[] = ("SPACE INVADERS");

unsigned char padrao[] = { 
	0b10001,
	0b10001,
	0b01110,
	0b10101,
	0b11111,
	0b10101,
	0b10101,
	0b10101
};

void delay_LCD()
{
	int i;
	for (i = 0; i < 400; i++) {}
}

void delay_inicializacao_LCD()
{
	int i;
	for (i = 0; i < 4000; i++) {}
}

void caractere_especial(unsigned char *padrao, const char local)
{
	int i = 0;
	envia_comando_lcd(0x40 + (local *8));
	for (i = 0; i < 8; i++)
		envia_dado_lcd(padrao[i]);
}

void envia_comando_lcd(char comando)
{
	E = 0;
	RS = 0;

	E = 1;
	barramento_dados = comando;
	E = 0;

	E = 1;
	comando = comando << 4;
	barramento_dados = comando;
	E = 0;

	delay_LCD();
}

void envia_dado_lcd(char dado)
{
	E = 0;
	RS = 1;

	E = 1;
	barramento_dados = dado;
	E = 0;

	E = 1;
	dado = dado << 4;
	barramento_dados = dado;
	E = 0;

	barramento_dados = dado;
	E = 0;

	delay_LCD();
}

void inicializa_lcd(void)
{
	delay_inicializacao_LCD();
	envia_comando_lcd(0b00000010);
	envia_comando_lcd(0b00101000);
	envia_comando_lcd(0b00001110);
	envia_comando_lcd(0b00000001);
}

void mensagem_lcd(char *mensagem)
{
	int i = 0;
	while (mensagem[i])
	{
		envia_dado_lcd(mensagem[i]);
		i++;
	}
}

void conf()
{
	TRISC = 0b11111000;
	TRISD = 0b00000000;
}

void main()
{
	conf();
	inicializa_lcd();
	caractere_especial(padrao, 1);

	while (1)
	{
		envia_comando_lcd(0x01);
		envia_dado_lcd(1);
		mensagem_lcd(mensagem1);
		envia_comando_lcd(0x106); 
		envia_dado_lcd(1);
		while (1);
	}
}