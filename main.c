#include <stdint.h>

void reset_handler(void);
int main(void);
void SystemClock_Config(void);

extern uint32_t _etext;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

#define RCC_CR        (*((volatile uint32_t*)0x40021000))
#define RCC_CFGR      (*((volatile uint32_t*)0x40021004))
#define RCC_AHBENR    (*((volatile uint32_t*)0x40021014))

#define GPIOC_MODER   (*((volatile uint32_t*)0x48000800))
#define GPIOC_OTYPER  (*((volatile uint32_t*)0x48000804))
#define GPIOC_OSPEEDR (*((volatile uint32_t*)0x48000808))
#define GPIOC_PUPDR   (*((volatile uint32_t*)0x4800080C))
#define GPIOC_ODR     (*((volatile uint32_t*)0x48000814))

#define GPIOA_MODER   (*((volatile uint32_t*)0x48000000))
#define GPIOA_OTYPER  (*((volatile uint32_t*)0x48000004))
#define GPIOA_OSPEEDR (*((volatile uint32_t*)0x48000008))
#define GPIOA_PUPDR   (*((volatile uint32_t*)0x4800000C))
#define GPIOA_IDR     (*((volatile uint32_t*)0x48000010))



__attribute__((section(".vector")))
const uint32_t vector_table[]={
    0x20004000,
    (uint32_t)&reset_handler,
    [2 ... 38] = 0
};

static void delay(void) {
    for(volatile uint32_t i=0x000ff000;i>0;i--) {
        __asm("nop");
    }
}

void reset_handler(void) {

    uint8_t *src=(uint8_t*)&_etext;
    uint8_t *dest=(uint8_t*)&_sdata;
    // Initialize the .data section
    // Copy initialized data from Flash (ROM) to SRAM (RAM)
    while (dest<(uint8_t*)&_edata) {
        *dest++ = *src++;
    }
    // Initialize the .bss section
    // Zero out the uninitialized data in SRAM
    dest=(uint8_t*)&_sbss;
    while (dest<(uint8_t*)&_ebss) {
        *dest++ = 0;
    }
    // Configure system clock (HSI as the clock source)    
    SystemClock_Config();
    // Call the main application
    main();
    // If main() returns, trap the CPU in an infinite loop
    // This prevents unintended execution    
    while(1);
}

void SystemClock_Config(void) {
    RCC_CR|= (1<<0);
    // Select HSI as the system clock (SW[1:0] bits in RCC_CFGR)
    RCC_CFGR &=~(3<<0);
    RCC_CFGR|=(1<<0);// Set SW to 01 (HSI(High Speed Internal) (8MHz) selected as system clock)
}

int main(void) {
    
    RCC_AHBENR |= (1<<19);//enable gpioc clock(PC6)
    RCC_AHBENR |= (1<<17);//Enable GPIOA clock(PA1)
    
    //config PC6 as output
    GPIOC_MODER &= ~(3<<12);
    GPIOC_MODER |= (1<<12);//01 is general purpose output mode

    //no need to set OTYPER, OSPEEDR because their default value is 0 i.e push-pull and low speed respectively and is enough for our use case
   
   //to prevent PA1 from floating we can set PUPDR register as pull down
   GPIOA_PUPDR &= ~(3<<2);
   GPIOA_PUPDR |= (2<<2);//SET PA1 to pull down mode (10)

   //configure PA1 as input
   GPIOA_MODER &= ~(3<<2);//input mode 00
  
   while(1){
   		//read pin status of the pin PA1(GPIOA Input data register)
   		uint8_t pinStatus=(GPIOA_IDR & 0x02);
	   	if(pinStatus){
			GPIOC_ODR |= (1<<6);
		}else{
			GPIOC_ODR &= ~(1<<6);
		}

	} 
   

    return 0;
}


 

