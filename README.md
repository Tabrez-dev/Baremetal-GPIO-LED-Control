# Baremetal GPIO LED Control

## Overview
This project focuses on controlling an on board LED on the STM32F072B-DISCO board using baremetal programming. It demonstrates basic GPIO control to toggle the LED on and off, showcasing fundamental concepts in embedded systems programming.

problem statement
![image](https://github.com/user-attachments/assets/96ae1275-6da9-4b95-93c8-8db8665c73f4)

I am using free I/O pin(PA1) of my board and on board LED (PC6).
## Learnings

- How to configure GPIO pins for output and how to use a GPIO pin to read data from the outside world.
- The importance of using internal pull-down resistors for input pins.
- How to ensure stable signals and prevent unwanted behaviors in embedded systems.

## Technical Details
I had learned how to configure low-level hardware in a baremetal environment, where the STM32F072B-DISCO board is directly controlled using memory-mapped registers.

1. **System Initialization**: 
   The project begins by initializing the `.data` section by copying initialized variables from flash to RAM and the `.bss` section by zeroing out uninitialized variables. This is performed in the `reset_handler` function before the main program execution.

2. **System Clock Configuration**: 
   The system clock is configured to use the HSI (High-Speed Internal) oscillator, which is set as the clock source for the system. This is done by modifying specific bits in the `RCC_CR` and `RCC_CFGR` registers to ensure that the microcontroller operates at 8 MHz.

3. **GPIO Pin Configuration**: 
   - The GPIO pins are configured for the intended functionality. For example, PC6 is set as an output pin to control the LED, and PA1 is configured as an input pin to read its status. The clock for GPIO ports A and C are enabled by setting the appropriate bits in the `RCC_AHBENR` register.
   - The output pin (PC6) is set in "General Purpose Output Mode" using the `GPIOC_MODER` register.
   - The input pin (PA1) is configured to use an internal pull-down resistor by setting the appropriate bits in the `GPIOA_PUPDR` register. This prevents the pin from floating and ensures it reads as low when no external input is connected.

![image](https://github.com/user-attachments/assets/4349a754-a9de-4075-bccb-39a2f16cea93)


   - PA1 was chosen becuase it had free I/O according to the Reference manual.
5. **Main Logic**: 
   In the `main` function, the status of PA1 is continuously monitored in a while loop. If the pin is high ,the on board LED on PC6 is turned on. If the pin is low, the LED is turned off. This demonstrates the basic GPIO input/output handling.

## Conclusion
This project serves as a foundation for learning GPIO control and the importance of proper pin configuration in embedded systems.
