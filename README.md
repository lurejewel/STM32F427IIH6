# STM32 Projects 

**Board**: DJI RoboMaster Development Board Type A\
**MCU**: STM32F427IIH6\
**Initialization Tool**:STM32CubeMX\
**Development Tool**: Keil V5

**Author**: Jin Wei\
**Affiliation**: Fudan Univ.

- - -

## BLINK
**LED light goes on and off alternately.**

LED: E11, red; 0 - on, 1 - off\
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
Button: B2, white; 0 - release, 1 - press\
LED on duration: 0.5 sec\

#### Learning Point:
- Comprehension of ``if`` statement
- Basic functions of ``HAL_ReadPin(GPIOx, GPIO_PIN_x)``
- Use **User Label** for better reusability using STM32CubeMX
- Use "*Go To Definition of*" to switch to the definition statement of the function or variable