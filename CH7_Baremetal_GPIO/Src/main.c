#include <stdint.h>
#include <stdbool.h>

#define RCC_BASE		0x40023800UL
#define	GPIOA_BASE		0x40020000UL
#define	GPIOB_BASE		0x40020400UL
#define	GPIOC_BASE		0x40020800UL
#define	GPIOD_BASE		0x40020C00UL

#define REG(addr)		(*(volatile uint32_t *)(addr))

#define RCC_AHB1ENR		REG(RCC_BASE + 0x30)

typedef struct {
	volatile uint32_t MODER;
	volatile uint32_t OTYPER;
	volatile uint32_t OSPEEDR;
	volatile uint32_t PUPDR;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t LCKR;
	volatile uint32_t AFRL;
	volatile uint32_t AFRH;
} gpio_regs_t;

#define GPIOA		((gpio_regs_t *)GPIOA_BASE)
#define GPIOB		((gpio_regs_t *)GPIOB_BASE)
#define GPIOC		((gpio_regs_t *)GPIOC_BASE)
#define GPIOD		((gpio_regs_t *)GPIOD_BASE)

typedef enum {
	GPIO_MODE_IN,
	GPIO_MODE_OUT,
	GPIO_MODE_AF,
	GPIO_MODE_ANALOG
} gpio_mode_t;

typedef enum {
	GPIO_PULL_NONE,
	GPIO_PULL_UP,
	GPIO_PU__DOWN
} gpio_pull_t;


void delay(volatile uint32_t n)
{
	while(n--);
}

void gpio_set_mode(gpio_regs_t *port, uint16_t pin, gpio_mode_t mode)
{
	uint32_t v = port->MODER;
	v &= ~(3 << (pin*2));
	v |= ((uint32_t)mode << (pin * 2));
	port->MODER = v;
}

void gpio_set_pull(gpio_regs_t *port, uint16_t pin, gpio_pull_t pull)
{
	uint32_t v = port->PUPDR;
	v &= ~(3 << (pin * 2));
	v |= ((uint32_t)pull << (pin *2));
	port->PUPDR = v;
}

void gpio_write(gpio_regs_t *port, uint16_t pin, bool level)
{
	port->BSRR = level ? (1 << pin) : (1 << (pin + 16));
}

bool gpio_read(const gpio_regs_t *port, uint16_t pin)
{
	return (port->IDR >> pin) & 1;
}

void gpio_toggle(gpio_regs_t *port, uint16_t pin)
{
	uint32_t mask = 1 << pin;
	uint32_t odr = port->ODR;
	port->BSRR = ((odr & mask) << 16) | (~odr & mask);
}

int main(void)
{
	// AHB1 RCC GPIOA Clock Enable
//	*((volatile uint32_t *)(0x40023800 + 0x30)) |= (1 << 0);  // 0x00000001


	// GPIO PA5
	// MODER : Output mode(01)
//	*((volatile uint32_t *)(0x40020000 + 0x00)) |= (1 << 10);  // 0x00000400
//	*((volatile uint32_t *)(0x40020000 + 0x00)) &= ~(1 << 11); // 0x00000400


	// OTYPER : push-pull(0)
//	*((volatile uint32_t *)(0x40020000 + 0x04)) &= ~(1 << 5); // 0x00000000


	// OPSEEDER : Medium speed(01)
//	*((volatile uint32_t *)(0x40020000 + 0x08)) |= (1 << 10); // 0x00000400
//	*((volatile uint32_t *)(0x40020000 + 0x08)) &= ~(1 << 11); // 0x00000400


	// PUPDR : Pull-up(01)
//	*((volatile uint32_t *)(0x40020000 + 0x0C)) |= (1 << 10); // 0x00000400
//	*((volatile uint32_t *)(0x40020000 + 0x0C)) &= ~(1 << 11); // 0x00000400

	// AHB1 RCC GPIOA Clock Enable
	RCC_AHB1ENR |= (1 << 0); // 0x00000001

	// GPIO PA5 Setting
//	GPIOA_MODER |= (1 << 10);  // 0x00000400
//	GPIOA_MODER &= ~(1 << 11); // 0x00000400
//	GPIOA_OTYPER  &= ~(1 << 5); // 0x00000000
//	GPIOA_OSPEEDR |= (1 << 10);  // 0x00000400
//	GPIOA_OSPEEDR |= ~(1 << 11); // 0x00000400
//	GPIOA_PUPDR |= (1 << 10); // 0x00000400
//	GPIOA_PUPDR  &= ~(1 << 11); // 0x00000400

	// AHB1 RCC GPIOA Clock Enable
	RCC_AHB1ENR |= (1 << 2); // 0x00000001

	// GPIO PA5 Setting
//	GPIOA_MODER |= (1 << 10);  // 0x00000400
//	GPIOA_MODER &= ~(1 << 11); // 0x00000400
//	GPIOA_OSPEEDR |= (1 << 10);  // 0x00000400
//	GPIOA_OSPEEDR |= ~(1 << 11); // 0x00000400

	gpio_set_mode(GPIOA, 5, GPIO_MODE_OUT);
	gpio_set_mode(GPIOC, 13, GPIO_MODE_IN);
	gpio_set_pull(GPIOC, 13, GPIO_PULL_UP);

	while(1)
	{
//		*((volatile uint32_t*)(0x40020000 + 0x14)) |= (1 << 5); // ODR 5bit set (0x00000020)
//		GPIOA_ODR |= (1 << 5); // ODR 5bit set (0x00000020)
//		delay(1000000);
//		*((volatile uint32_t*)(0x40020000 + 0x14)) &= ~(1 << 5); // ODR 5bit reset (0x00000020)
//		GPIOA_ODR &= ~(1 << 5); // ODR 5bit reset (0x00000020)
//		delay(1000000);

		if(gpio_read(GPIOC, 13)==0){
			gpio_write(GPIOA, 5, 1);
		}
		else {
			gpio_write(GPIOA, 5, 0);
		}
	}


	return 0;
}
