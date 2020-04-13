

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/dac.h>

void msdelay(const int);

static void gpio_setup(void)
{

	rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_180MHZ]);		//PLL multiplier for increasing the system clock/oscillator (?) speed
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOC);
	
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO5);

	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13); //Pin rs
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO10);  //Pin rw
	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO14); //Pin en

	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO3);  //Pin 0
	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO2);  //Pin 1

	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO0);  //Pin 2
	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO1);  //Pin 3
	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO0);  //Pin 4
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO4);  //Pin 5

	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13); //Pin 6
	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO7);  //Pin 7

}

#define enable_on 	gpio_set(GPIOB, GPIO14)
#define enable_off 	gpio_clear(GPIOB, GPIO14)
#define rs_on 		gpio_set(GPIOB, GPIO13)
#define rs_off 		gpio_clear(GPIOB, GPIO13)
#define rw_on 		gpio_set(GPIOA, GPIO10)
#define rw_off 		gpio_clear(GPIOA, GPIO10)


void msdelay(int x)
{
	int j;
	const int z=30000*x;	//technically 45000

	for (j = 0; j < z ; j++)	
		__asm__("nop");
	
}

static void send_data(int val[], int rs)
{
	if(!rs)
		rs_off;
	else
		rs_on;

	if(val[0])
	gpio_set(GPIOC, GPIO3);
	else
	gpio_clear(GPIOC, GPIO3);


	if(val[1])
	gpio_set(GPIOC, GPIO2);
	else
	gpio_clear(GPIOC, GPIO2);


	if(val[2])
	gpio_set(GPIOC, GPIO0);
	else
	gpio_clear(GPIOC, GPIO0);


	if(val[3])
	gpio_set(GPIOC, GPIO1);
	else
	gpio_clear(GPIOC, GPIO1);


	if(val[4])
	gpio_set(GPIOB, GPIO0);
	else
	gpio_clear(GPIOB, GPIO0);


	if(val[5])
	gpio_set(GPIOA, GPIO4);
	else
	gpio_clear(GPIOA, GPIO4);


	if(val[6])
	gpio_set(GPIOC, GPIO13);
	else
	gpio_clear(GPIOC, GPIO13);


	if(val[7])
	gpio_set(GPIOB, GPIO7);
	else
	gpio_clear(GPIOB, GPIO7);


	enable_on;
	msdelay(20);
	enable_off;
	msdelay(20);

	
}

#if 0
void led_test()
{

	gpio_set(GPIOC, GPIO3);

	gpio_set(GPIOC, GPIO2);

	gpio_set(GPIOC, GPIO0);

	gpio_set(GPIOC, GPIO1);

	gpio_set(GPIOB, GPIO0);

	gpio_set(GPIOA, GPIO4);

	gpio_set(GPIOC, GPIO13);

	gpio_set(GPIOB, GPIO7);
	msdelay(5000); 
}

void led_test2()
{

	enable_on;
	rs_on;
	rw_on;

}
#endif



void lcd_init()
{
	int arr2[8]={0,0,1,1,0,0,0,0};
	int arr3[8]={0,0,1,1,1,0,0,0};


	msdelay(100); 

	send_data(arr2, 0);
	msdelay(5);  
	send_data(arr2, 0);
	msdelay(1);  
	send_data(arr2, 0);
	msdelay(1);
	send_data(arr3, 0);
	msdelay(1);
}

void display_init()
{

	int arr1[8]={0,0,0,0,1,0,0,0};
	int arr2[8]={0,0,0,0,0,0,0,1};
	int arr3[8]={0,0,0,0,0,1,1,0};
	int arr4[8]={0,0,0,0,1,1,0,0};



	send_data (arr1, 0); 
	msdelay(1);
	send_data (arr2, 0); 
	msdelay(3);
	send_data (arr3, 0);
	msdelay(1);
	send_data (arr4, 0);
	msdelay(1);

}







int main(void)
{
	unsigned int i;
	int arrt1[8]={1,1,0,0,0,0,1,0};
	int arrt2[8]={1,0,0,0,0,0,1,0};
	int arrt3[8]={0,0,1,1,1,0,1,0};
	int arr3[8]={0,0,1,1,1,0,0,0};
	
	gpio_setup();

	//led_test2();
	//led_test();

	lcd_init();
	display_init();
	send_data(arr3, 0);
	msdelay(1);
	send_data(arrt1, 1);
	msdelay(1);
	send_data(arrt2, 1);
	msdelay(1);
	send_data(arrt3, 1);
	msdelay(1);


	

	//Nucleo F446RE oscillator freq = 8 Mhz?? or 16? 1 nop instructoin takes 1 intruction cycle or 4 oscillator cycles. 8/4 = 2 Mhz, which is used in the for loop. (or 500 ns)
	//https://www.microchip.com/forums/m942878.aspx
	//Invalid if using PLL - in PLL case use 180/4 = 22.5 Mhz?? Confirm calculations. 22.5 Mhz results in 0.5s led on and 0.5s led off.
	//Google fcy, fosc for stm32: https://electronics.stackexchange.com/questions/103535/what-does-fcy-f-cy-stand-for
	//https://www.microchip.com/forums/m985863.aspx

	while(1)
	{

	gpio_set(GPIOA, GPIO5);
	msdelay(500);
	//for (i = 0; i < 22500000; i++)	
		//__asm__("nop"); 

	gpio_clear(GPIOA, GPIO5);
	msdelay(500);

	//for (i = 0; i < 22500000; i++)
		//__asm__("nop"); 
	}

	return 0;
}
