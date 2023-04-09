#include <p18f4520.h>
#include <stdlib.h>

#pragma config WDT = OFF 
#pragma config OSC = INTIO67

#define dados PORTD

#define RS PORTCbits.RC5
#define RW PORTCbits.RC6
#define E PORTCbits.RC7


char conta_timer=250, flag=0, estouro=0;
char cont;
unsigned char high, low;


float resultado;
float periodo,Frequencia;
unsigned char high, low;

void mede_periodo();
void calcula_freq (float freq);


void configura_pic();
void regist();
void interrupcao_alta_priori();

void delay_lcd();
void delay_inicializa_lcd();
void envia_comando_lcd(char comando);
void envia_dado_lcd(char dado);
void mensagem_lcd(char *mensagem);
void inicializa_lcd(void);

//**************************    LCD    *********************************


char nome[] = (" x Hz"); // como fazer?


void delay_lcd(){
	int i;
	for (i = 0; i < 350; i++) {}
}

void delay_inicializa_lcd(){
	int i;
	for (i = 0; i < 4000; i++) {}
}

void envia_comando_lcd(char comando){
	E = 0;
	RS = 0;

	E = 1;
	dados = comando;
	E = 0;

	E = 1;
	comando = comando << 4;
	dados = comando;
	E = 0;

	delay_lcd();
}

void envia_dado_lcd(char dado){
	E = 0;
	RS = 1;

	E = 1;
	dados = dado;
	E = 0;

	E = 1;
	dado = dado << 4;
	dados = dado;
	E = 0;

	dados = dado;
	E = 0;

	delay_lcd();
}

void inicializa_lcd(void){
	delay_inicializa_lcd();
	envia_comando_lcd(0b00000010);
	envia_comando_lcd(0b00101000);
	envia_comando_lcd(0b00001110);
	envia_comando_lcd(0b00000001);
}

void mensagem_lcd(char *mensagem){
	int i = 0;
	while (mensagem[i])
	{
		envia_dado_lcd(mensagem[i]);
		i++;
	}
}


//**************************     Frequencia     *******************************



void mede_periodo(){
	flag = 0;
    resultado = (high<<8)|low;              //periodos pre-estouro
    periodo = estouro*65536 + resultado;    //periodos totais
}

void calcula_freq(float freq){
	
    
	float T;
    float factor = 0.0001;
	T = freq * factor;
	
	Frequencia = 1/T;

}

//*************************     PIC     *******************************

void configura_pic(void){
    
   	TRISC = 0b11111000;
	TRISD = 0b00000000;
	inicializa_lcd();
 
    ADCON1 = 0x0F; // Configurando todas as portas do PIC para digital
}

void regist(void){
    
    RCONbits.IPEN = 1;         
    INTCONbits.GIE = 1;        
    INTCONbits.PEIE = 1;        
    INTCONbits.INT0IE = 1;      
    INTCON3bits.INT1IE = 1;     
    INTCON3bits.INT2IE = 1;     
    INTCON3bits.INT1IP = 1;    
    INTCON3bits.INT2IP = 1;     
    INTCON2bits.INTEDG0 = 1;   
    INTCON2bits.INTEDG1 = 1;    
    INTCON2bits.INTEDG2 = 1;    
    
// Timer 0
    T0CONbits.TMR0ON = 1;   // Timer0 on
    T0CONbits.T08BIT = 0;   // Configurado para 16 bits
    T0CONbits.T0CS = 0;     // Fonte é clock interno
    T0CONbits.PSA = 0;      // Habilita Prescaler
    T0CONbits.T0PS2 = 1;
    T0CONbits.T0PS1 = 1;    // Prescaler de 1:128 
    T0CONbits.T0PS0 = 0;
    
    TMR0H = 0xC2; 
    TMR0L = 0xF7; 

    INTCONbits.TMR0IE = 1;  // Interv de timer0 on
    INTCON2bits.TMR0IP = 1; // Interv de alta priori

// Timer 1

    T1CONbits.TMR1ON = 1;   // Ligando timer 1
    T1CONbits.T1RUN = 0;    // Clock do oscilador do timer 1
    T1CONbits.T1OSCEN = 0;  // Oscilador timer 1 desligado
    T1CONbits.RD16 = 1;     // Configurado para 16 bits
    T1CONbits.TMR1CS = 1;   // Fonte do timer 1 é o clock externo Pelo pino RC0
    T1CONbits.T1SYNC = 1;   // Não sincroniza fonte do timer 1 com clock interno
    
    TMR1H = 0xD8;
    TMR1L = 0xF0;

    PIE1bits.TMR1IE = 1;    // Interv de timer0 on
    IPR1bits.TMR1IP = 1;    // Interv de alta priori
    
}

//*************************     Interrupção     *******************************


#pragma code alta_prioridade = 0x08

void interrupcao_de_alta_prioridade(void)
{
  _asm goto interrupcao_alta_priori _endasm
}

#pragma code

#pragma interrupt interrupcao_alta_priori 

// Define qual prioridade acontece

void interrupcao_alta_priori(void){
     
    if(INTCONbits.TMR0IF){      // Timer0 de 1 seg
        
        TMR0H = 0xC2; 
        TMR0L = 0xF7; 
                            
        low = TMR1L;            // conta o periodo de RC0
        high = TMR1H;
        TMR1H = 0; 
        TMR1L = 0;
        flag =1;
        
        mede_periodo();         // calcula o periodo de RC0
        calcula_freq(periodo);
        
        ultoa(buffer,Frequencia, 10);
        
        INTCONbits.TMR0IF = 0; 
    }

   
    if(PIR1bits.TMR1IF){       
        
        estouro++;
        PIR1bits.TMR1IF = 0; 
    }
    
   
}
    

void main(){
    
    configura_pic();
    regist();
    
    while(1){
    }
}




