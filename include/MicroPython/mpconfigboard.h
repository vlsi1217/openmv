#define STM32F4DISC

#define MICROPY_HW_BOARD_NAME       "F4DISC"

#define MICROPY_HW_HAS_SWITCH       (0)
#define MICROPY_HW_HAS_SDCARD       (0)
#define MICROPY_HW_HAS_MMA7660      (0)
#define MICROPY_HW_HAS_LIS3DSH      (0)
#define MICROPY_HW_HAS_LCD          (0)
#define MICROPY_HW_HAS_WLAN         (0)
#define MICROPY_HW_ENABLE_RNG       (0)
#define MICROPY_HW_ENABLE_RTC       (0)
#define MICROPY_HW_ENABLE_TIMER     (0)
#define MICROPY_HW_ENABLE_SERVO     (0)
#define MICROPY_HW_ENABLE_AUDIO     (0)

#define USRSW_PORT          (GPIOA)
#define USRSW_PIN           (GPIO_Pin_0)
#define USRSW_PUPD          (GPIO_PuPd_NOPULL)
#define USRSW_EXTI_PIN      (EXTI_PinSource0)
#define USRSW_EXTI_PORT     (EXTI_PortSourceGPIOA)
#define USRSW_EXTI_LINE     (EXTI_Line0)
#define USRSW_EXTI_IRQN     (EXTI0_IRQn)
#define USRSW_EXTI_EDGE     (EXTI_Trigger_Falling)

/* LED */
#define PYB_LED1_PORT   (GPIOD)
#define PYB_LED1_PIN    (GPIO_Pin_14)

#define PYB_LED2_PORT   (GPIOD)
#define PYB_LED2_PIN    (GPIO_Pin_12)

#define PYB_LED3_PORT   (GPIOD)
#define PYB_LED3_PIN    (GPIO_Pin_15)

#define PYB_LED4_PORT   (GPIOD)
#define PYB_LED4_PIN    (GPIO_Pin_13)

#define PYB_OTYPE       (GPIO_OType_PP)

#define PYB_LED_ON(port, pin)  (port->BSRRL = pin)
#define PYB_LED_OFF(port, pin) (port->BSRRH = pin)

