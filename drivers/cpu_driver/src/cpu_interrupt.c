
#include "host_stack.h"


void LIB_16_IRQHandler(void)
{
	lib_bluetooth_irq16();
}

void LIB_18_IRQHandler(void)
{
	lib_bluetooth_irq18();
}

void LIB_20_IRQHandler(void)
{
	lib_bluetooth_irq20();
}

void LIB_19_IRQHandler(void)
{
	lib_bluetooth_irq19();
}

void LIB_4_IRQHandler(void)
{
	lib_bluetooth_irq4();
}

void LIB_5_IRQHandler(void)
{
	lib_bluetooth_irq5();
}

void LIB_6_IRQHandler(void)
{
	lib_bluetooth_irq6();
}

void LIB_7_IRQHandler(void)
{
	lib_bluetooth_irq7();
}

void LIB_8_IRQHandler(void)
{
	lib_bluetooth_irq8();
}

void LIB_9_IRQHandler(void)
{
	lib_bluetooth_irq9();
}

void LIB_10_IRQHandler(void)
{
	lib_bluetooth_irq10();
}
uint8_t IRQ_FLAG;
void LIB_11_IRQHandler(void)
{
	IRQ_FLAG = 0;
	lib_bluetooth_irq11();
}

void LIB_13_IRQHandler(void)
{
	lib_bluetooth_irq13();
}
