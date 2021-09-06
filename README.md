# STM32 Projects 

**Board**: DJI RoboMaster Development Board Type A\
**MCU**: STM32F427IIH6\
**Initialization Tool**:STM32CubeMX\
**Development Tool**: Keil V5

**Author**: Jin Wei\
**Affiliation**: Fudan Univ.

- - -
- - -

## BLINK
**LED light goes on and off alternately.**

LED: PE11, red; 0 - on, 1 - off\
LED on duration: 0.8 sec\
LED off duration: 0.2 sec\
Cycle: 1.0 sec

#### Learning Point:
- Set specific pin as **GPIO_Output** & generate initialized code using STM32CubeMX
- Set specific pin as **Reset_State** to release the pin and then re-generate the code using STM32CubeMX
- Comprehension of ``GPIO_SET``, ``GPIO_RESET``, ``GPIOx``, ``GPIO_PIN_x``
- Basic functions of ``HAL_Delay(ms)``, ``HAL_GPIO_WritePin(GPIOx, GPIO_PIN_x, GPIO_PIN_SET/ESET)``.
- Comprehension of ``while(1)`` in "main.c"

## BUTTON BLINK
**LED light turns on while pressing the button and maintains on shortly after that.**

LED: same as "BLINK"\
Button: PB2, white; 0 - release, 1 - press\
LED on duration: 0.5 sec\

#### Learning Point:
- Comprehension of ``if`` statement
- Basic functions of ``HAL_ReadPin(GPIOx, GPIO_PIN_x)``
- Use **User Label** for better reusability using STM32CubeMX
- Use "*Go To Definition of*" to switch to the definition statement of the function or variable

## ADC
**Read analog data and display the corresponding digital data on PC thorugh USART.**

Analog Input: force sensor (HYLY-019) with a weighing transmitter (HYBS-017)
Input Port: PB0 (ADC1_IN8)
Input Data Range: 0 ~ ±5.0V (uint16_t; push - positive, pull - negative)
ADC: board embedded ADC (0 ~ 3.3 V; need extra voltage division circuit)
Output Port: PG9 - RX, PG14 - TX (USART6)

#### Learning Point:
- Use data struct **array** to split a 16-bit **short int** into two 8-bit **char**
- Enable **HSE** using STM32CubeMX
- Set specific pin as **ADC** or **USART** port with appropriate configuration and generate initialized code using STM32CubeMX
- Basic analog reading functions including ``HAL_ADC_ConfigChannel(&hadcx, &sConfig)``, ``HAL_ADC_Start(&hadcx)``, ``HAL_ADC_PollForConversion(&hadcx, ms)``, ``HAL_ADC_Stop(&hadcx)``
- Basic serial port output function of ``HAL_UART_Transmit(&huartx, &char, n, 0xFFFF)``