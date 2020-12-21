
#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

// Definiciones
//--------------------------------------------------------------------- 
#define M1A     PTED_PTED0
#define M1B     PTED_PTED1
#define M2A     PTDD_PTDD3
#define M2B     PTDD_PTDD2
#define EM1     PTCD_PTCD0
#define EM2     PTCD_PTCD1
#define EM3     PTCD_PTCD2
#define EM4     PTCD_PTCD3
#define EM5     PTCD_PTCD4
#define encoder1B     PTFD_PTFD0
#define encoder2B     PTED_PTED3
int direccionEnc1,direccionEnc2,timeEnc1,timeEnc2;
int Medida1,Medida2,Medida3,Medida4,Medida5;
int Luz1,Luz2,Luz3,Luz4,Luz5;
int HuecoIzq=0,HuecoDer=0;

int emisor=0;
int canal=0;
int on=0;
// Headers
//---------------------------------------------------------------------
void init_ics(void);
void conf_init(void);
void conf_adc(void);
void conf_puertos(void);
void config_irq(void);
void config_rti(void);
void Acciones(void);
void Giro();
void delay();
void init_TPM(int cicloUtil, int modulo);
void config_TPMM1(int newCicloUtil);
void config_TPMM2(int newCicloUtil2);

void main(void) {
	 EnableInterrupts;
  /* include your code here */
  conf_init();
  conf_puertos();  
  config_irq();
  init_TPM(350,1000);
  config_rti();
  conf_adc();
 
 for(;;) {
   // __RESET_WATCHDOG();	/* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}

// Funciones
void conf_init(void){
	SOPT = 0x02;
}

void conf_puertos(void){
	PTCDD=0x1F;
	PTDDD_PTDDD2=1;
	PTDDD_PTDDD3=1;
	PTEDD_PTEDD0=1;
	PTEDD_PTEDD1=1;
	PTEDD_PTEDD3=0;	
	PTFDD_PTFDD0=0;
	M1A=0;
	M1B=1;
	M2A=0;
	M2B=1;

}

void init_TPM(int cicloUtil, int modulo){
	//PWM motores
	TPM2SC =0x2A;
	TPM2MOD=modulo;//1000 (Modulo 1Khz)
	TPM2C0SC =0x08;
	TPM2C0V=cicloUtil;
	TPM2C1SC =0x08;
	TPM2C1V=cicloUtil;
	//TPM captura entrada
	/*TPM1SC=0x48;
	TPM1MOD=0x00;
	TPM1C0SC =0x44;//PTF1-ENCODER1A
	TPM1C3SC =0x44;//PTE2-ENCODER2A*/
}

void config_TPMM1(int newCicloUtil){
	TPM1C0V=newCicloUtil;
}
void config_TPMM2(int newCicloUtil){
	TPM1C1V=newCicloUtil;
}

void config_irq(void){
	IRQSC_IRQEDG=1;
	IRQSC_IRQPE=1;
	IRQSC_IRQACK=1;
	IRQSC_IRQMOD=0;
	IRQSC_IRQIE=1;
}

void config_rti(void){
	SRTISC_RTICLKS=0;
	SRTISC_RTIE=1;
	SRTISC_RTIACK =1;
	SRTISC_RTIS0=1;//8 ms
	SRTISC_RTIS1=0;
	SRTISC_RTIS2=0;
}

void conf_adc(void){
			ADC1SC1_ADCO = 1;					//Continuous conversion selected
			ADC1CFG_ADICLK = 0b11;				//Asynchronous ADC clock
			ADC1CFG_ADIV = 0b10;				//Input clock without division
			ADC1CFG_MODE = 0b10;				//8 bit conversion
			ADC1SC1_AIEN =1;					//Enable  conversion Complete interruption.
			ADC1CFG_ADLSMP = 0;					//Fast sampling time
			APCTL1_ADPC0 = 1;					//Enable channel 0 a ADC input
			APCTL1_ADPC1 = 1;
			APCTL1_ADPC2 = 1;	
			APCTL1_ADPC3 = 1;
			APCTL2_ADPC8 = 1;

						//Select channel 0

}

void Vecinos(void){
	if(Medida4-Luz4<200 && Medida5-Luz5<100){//Diagonales No pared
		HuecoIzq=1;
		HuecoDer=1;
	}else if(Medida4-Luz4<200){//Diagonales No Pared Derecha
		HuecoDer=1;
	}else if(Medida5-Luz5<200){//Diagonales No Pared Derecha
		HuecoIzq=1;
	}else{
		HuecoIzq=0;
		HuecoDer=0;
	}
}
// Funciones ///////////////////////////////////////////////////
void Giro(int direccion){
	if(direccion==0){
		delay(3000000);
		M1A=1;
	    M1B=0;
	    delay(4000000);
	    M1A=0;
	    M1B=1;
	    delay(5000000);
	}else if(direccion==1){
	    delay(3000000);
		M2A=1;
		M2B=0;
		delay(4000000);
		M2A=0;
		M2B=1;
		delay(5000000);
	}else if(direccion=2){
		M1A=1;
	    M1B=0;
	    M2A=0;
	    M2B=1;
	    delay(80000000);
	    M1A=0;
	    M1B=1;
	    M2A=0;
		M2B=1;
	    
		//M1B=1;
		//M2B=1;
	    /*M1A=0;
		M1B=1;
		delay(1000000);*/
		
	}
	
}

void delay(int retardo){
	int cont=0;
	while(cont<retardo){
		cont++;
	}
}

void Acciones(void){
	//Vecinos();
	if((Medida3-Luz3)>230 &&(Medida1-Luz1)>250 && (Medida5-Luz5)>300){//Callejon
		Giro(2);
		}
	if((Medida1-Luz1)<250 && (Medida5-Luz5)<300){//No encontro pared izquiera y derecha
		if((Medida4-Luz4)<(Medida2-Luz2+230)){
			Giro(0);
		}else{
			Giro(1);
		}
	}
	if((Medida1-Luz1)<250&& (Medida5-Luz5)>300){//No Pared  derecha si Izquierda 
		Giro(1);
	}
	if((Medida5-Luz5)<300 && (Medida1-Luz1)>250){//No Pared Izquierda si derecha
		Giro(0);
	}
	
}

// INTERRUPT BODIES ///////////////////////////////////////////////////                    
//---------------------------------------------------------------------  +

interrupt VectorNumber_Vtpm1ch0 void Captura_Enc1(){
	unsigned int mascaraCeros=0;
	TPM1CNT=0x00;
	mascaraCeros=TPM1SC;
	TPM1SC&=0x7F;//Limpiar bandera de overflow
	mascaraCeros=TPM1C0SC;
	TPM1C0SC&=0x7F;
	direccionEnc2=encoder2B;
	timeEnc2=TPM1C0V;
	
}

interrupt VectorNumber_Vtpm1ch3 void Captura_Enc2(){
	unsigned int mascaraCeros=0;
	TPM1CNT=0x00;
	mascaraCeros=TPM1SC;
	TPM1SC&=0x7F;//Limpiar bandera de overflow
	mascaraCeros=TPM1C0SC;
	TPM1C3SC&=0x7F;
	direccionEnc1=encoder1B;
	timeEnc1=TPM1C3V;
	
}

interrupt VectorNumber_Vtpm1ovf void Overflow_modulo(){
	unsigned int mascaraCeros=0;
	mascaraCeros=TPM1SC;
	TPM1SC&=0x7F;
	timeEnc1=0xFF;
	timeEnc2=0xFF;
}

interrupt VectorNumber_Vadc1 void ADC_rutina(){
Acciones();
	if(ADC1SC1_ADCH == 0b0000){
		 if(EM1==0){
			 Luz1=ADC1R;
		}else{
			Medida1=ADC1R;
			}
			ADC1SC1_ADCH = 0b0001;
		}else if(ADC1SC1_ADCH == 0b0001){
			if(EM2==0){
				Luz2=ADC1R;
			}else{
				Medida2=ADC1R;
							  }	                                                       
				ADC1SC1_ADCH = 0b0010;
		}else if(ADC1SC1_ADCH == 0b0010){
				if(EM3==0){
					Luz3=ADC1R;
					 }else{
						 Medida3=ADC1R;
						}   
			ADC1SC1_ADCH = 0b0011;					
		}else if(ADC1SC1_ADCH == 0b0011){
			if(EM4==0){
				Luz4=ADC1R;
				 }else{
			  Medida4=ADC1R;
					}   
		ADC1SC1_ADCH = 0b1000;				
		}else if(ADC1SC1_ADCH == 0b1000){
				if(EM5==0){
					Luz5=ADC1R;
					 }else{
					Medida5=ADC1R;
						}   
			ADC1SC1_ADCH = 0b0000;
					
				}
}

interrupt VectorNumber_Virq void IRQ_rutina(){
	on=1;
}

interrupt VectorNumber_Vrti void RTI_rutina(){
	
	if(EM1==0&&EM2==0&&EM3==0&&EM4==0&&EM5==0){
			EM1=1;
			ADC1SC1_ADCH = 0b0000;
		}else if(EM1==1){
			EM1=0;
			EM2=1;
			
		}else if(EM2==1){
			EM2=0;
			EM3=1;
			
		}else if(EM3==1){
			EM3=0;
			EM4=1;
		}else if(EM4==1){
			EM4=0;
			EM5=1;
		}else if(EM5==1){
			EM5=0;
			EM1=1;
		}		
		SRTISC_RTIACK =1;		
}
