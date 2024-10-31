#ifndef CONFIG__H_
#define CONFIG__H_

#define LED_DISPLAY_ACTIVATED 0
#define UART_DISPLAY_ACTIVATED 0


#define LED_DISPLAY_PIN 27
#define LED_DISPLAY_LENGTH 14                     //14
#define LED_DISPLAY_HEIGHT 41                    //41
#define LED_DISPLAY_COUNT_LED LED_DISPLAY_LENGTH* LED_DISPLAY_HEIGHT  //574

#define LED_DISPLAY_OFFSET_BETWEEN_LINES 6
#define LED_DISPLAY_LINE_1 1
#define LED_DISPLAY_LINE_2 LED_DISPLAY_LINE_1+LED_DISPLAY_OFFSET_BETWEEN_LINES
#define LED_DISPLAY_LINE_3 LED_DISPLAY_LINE_2+LED_DISPLAY_OFFSET_BETWEEN_LINES
#define LED_DISPLAY_LINE_4 LED_DISPLAY_LINE_3+LED_DISPLAY_OFFSET_BETWEEN_LINES
#define LED_DISPLAY_LINE_5 LED_DISPLAY_LINE_4+LED_DISPLAY_OFFSET_BETWEEN_LINES
#define LED_DISPLAY_LINE_6 LED_DISPLAY_LINE_5+LED_DISPLAY_OFFSET_BETWEEN_LINES
#define LED_DISPLAY_LINE_7 LED_DISPLAY_LINE_6+LED_DISPLAY_OFFSET_BETWEEN_LINES

















































#endif /* CONFIG__H_ */