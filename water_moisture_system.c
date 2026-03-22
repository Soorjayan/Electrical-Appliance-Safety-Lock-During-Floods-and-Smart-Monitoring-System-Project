#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

// UART
void UART_init(unsigned int ubrr)
{
    UBRR0H = (ubrr >> 8);
    UBRR0L = ubrr;
    UCSR0B = (1<<TXEN0);
    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
}

void UART_sendChar(char data)
{
    while (!(UCSR0A & (1<<UDRE0)));
    UDR0 = data;
}

void UART_sendString(char *str)
{
    while(*str)
    {
        UART_sendChar(*str++);
    }
}

// ADC
void ADC_init()
{
    ADMUX = (1<<REFS0);
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

uint16_t ADC_read(uint8_t ch)
{
    ADMUX = (ADMUX & 0xF0) | ch;
    ADCSRA |= (1<<ADSC);
    while (ADCSRA & (1<<ADSC));
    return ADC;
}

int main(void)
{
    DDRD |= (1<<PD2)|(1<<PD3)|(1<<PD4)|(1<<PD5)|(1<<PD6)|(1<<PD7);

    ADC_init();
    UART_init(103); // 9600 baud

    char buffer[30];
    uint16_t water, moisture;

    while(1)
    {
        water = ADC_read(0);
        moisture = ADC_read(1);

        // Send data to ESP32
        sprintf(buffer, "W:%d,M:%d\n", water, moisture);
        UART_sendString(buffer);

        // Water LEDs
        PORTD &= ~((1<<PD2)|(1<<PD3)|(1<<PD4)|(1<<PD5));

        if(water >=100 && water <=600)
            PORTD |= (1<<PD2);
        else if(water >=601 && water <=625)
            PORTD |= (1<<PD3);
        else if(water >=626)
            PORTD |= (1<<PD4)|(1<<PD5);

        // Moisture alert LED
        if(moisture < 400)
            PORTD |= (1<<PD7);
        else
            PORTD &= ~(1<<PD7);

        _delay_ms(500);
    }
}