#include <msp.h>

//Must always include our OS header file
#include "fate.h"


//Functions that implement our 2 periodic tasks
//Return type and arguments must always be void
//For now, no local variables
//No calling other functions
//When execution is finished, must always call "Task_stop" with its name
//NOT return: that would probably crash our kernel

int T1 = 0;
int T2 = 0;
int T3 = 0;

void Task_1(void)
{
	P2OUT = 0x01;
    TA1CTL |= (uint16_t) BIT4;
	while(!(T1 == 1)){};
    TA1CTL &=(uint16_t) (~BIT4);
    T1 = 0;
	Task_stop((uint32_t)Task_1);
}

void Task_2(void)
{
	P2OUT = 0x04;
    TA2CTL |= (uint16_t) BIT4;
	while(!(T2==10)){};
    TA2CTL &=(uint16_t) (~BIT4);
    T2 = 0;
	Task_stop((uint32_t)Task_2);
}

void Task_3(void)
{
	P2OUT = 0x02;
    TA3CTL |= (uint16_t) BIT4;
	while(!(T3==3)){};
    TA3CTL &=(uint16_t) (~BIT4);
    T3 = 0;
	Task_stop((uint32_t)Task_3);
}

void TA1_N_IRQHandler(void){
    if(TA1CTL & BIT0){
        TA1CTL &=(uint16_t) (~BIT0);
        T1++;
    }
}

void TA2_N_IRQHandler(void){
    if(TA2CTL & BIT0){
        TA2CTL &=(uint16_t) (~BIT0);
        T2++;
    }
}

void TA3_N_IRQHandler(void){
    if(TA3CTL & BIT0){
        TA3CTL &=(uint16_t) (~BIT0);
        T3++;
    }
}

void configTimers(void)
{
    TA1CTL &= (uint16_t) 0;
    TA1CTL |= (uint16_t) BIT8;
    //TA1CTL |= (uint16_t) BIT6;
    TA1CTL |= (uint16_t) BIT1;
    TA1CCR0 = 0x7fff;

    TA2CTL &= (uint16_t) 0;
    TA2CTL |= (uint16_t) BIT8;
    //TA2CTL |= (uint16_t) BIT6;
    TA2CTL |= (uint16_t) BIT1;
    TA2CCR0 = 0x7fff;

    TA3CTL &= (uint16_t) 0;
    TA3CTL |= (uint16_t) BIT8;
    //TA3CTL |= (uint16_t) BIT6;
    TA3CTL |= (uint16_t) BIT1;
    TA3CCR0 = 0x7fff;

    NVIC_EnableIRQ(TA1_N_IRQn);
	NVIC_SetPriority(TA1_N_IRQn, 2);

    NVIC_EnableIRQ(TA2_N_IRQn);
	NVIC_SetPriority(TA2_N_IRQn, 2);

    NVIC_EnableIRQ(TA3_N_IRQn);
	NVIC_SetPriority(TA3_N_IRQn, 2);
}



int main()
{
	
	//LED P2.0.1.2 (RGB)
	P2SEL0 &= (uint8_t)(~(BIT0)|(BIT1)|(BIT2));
	P2SEL1 &= (uint8_t)(~((BIT0)|(BIT1)|(BIT2)));
	P2DIR |= (uint8_t)((BIT0)|(BIT1)|(BIT2));
	P2OUT &= (uint8_t)0;
	
    configTimers();
	//Initialize Task list, includes setting up idle task
	//Always the first function that must be called
	Task_list_init();
	
	//Initialize our periodic tasks, with periods 100 and 200 ticks (1 and 2s), respectively
	Task_add((uint32_t)Task_1,(uint32_t) 1500, (uint32_t) 1);
    Task_add((uint32_t)Task_3,(uint32_t) 1500, (uint32_t) 2);
	Task_add((uint32_t)Task_2,(uint32_t) 1500, (uint32_t) 3);

	
	//Task_event_add((uint32_t)LED_RGB_toggle, SWITCH_P1_4, (uint32_t) 1);
	
	//This will begin scheduling our tasks 
	Task_schedule();
	
	return 0;
}
