#ifndef _LED_H_
#define _LED_H_


#define RED      PBout(12)
#define GREEN    PBout(13)
#define YELLOW   PBout(14)


typedef struct
{

	_Bool Led_Status;

} LED_INFO;

#define LED_ON		1

#define LED_OFF	0

extern LED_INFO led_info;

void GPIOInit(void);
void Led_Init(void);

void Led_Set(_Bool status);


#endif
