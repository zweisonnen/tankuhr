/*
 * sbFuel.c – optimierte Version
 */

#include <avr/io.h>
#include <util/delay.h>

#ifndef F_CPU
# warning "F_CPU nicht definiert, setze auf 1 MHz"
# define F_CPU 1000000UL
#endif

// -------------------------------------------------------------
// Hilfsfunktionen
// -------------------------------------------------------------

// ADC 10-bit Mittelwert aus n Messungen
uint16_t adc_read_avg(uint8_t samples)
{
    uint16_t sum = 0;

    for(uint8_t i = 0; i < samples; i++)
    {
        ADCSRA |= (1 << ADSC);
        while (ADCSRA & (1 << ADSC)) ;   // warten
        sum += ADCW;
    }

    return sum / samples;
}

// PORTD LEDs setzen: 0 = alle aus, 1..8 = Anzahl LED an
void set_level(uint8_t leds)
{
    uint8_t mask = 0;

    for(uint8_t i = 0; i < leds; i++)
        mask |= (1 << i);

    PORTD = mask;
}

// Blinken eines Pins
void blink_pb0(uint8_t times)
{
    for(uint8_t i = 0; i < times; i++)
    {
        PORTB ^= (1 << PB0);
        _delay_ms(60);
    }
}

// -------------------------------------------------------------
// Main
// -------------------------------------------------------------

int main(void)
{
    DDRD = 0xFF;   // 8 LEDs
    DDRB = 0xFF;   // PB0, PB1
    PORTD = 0x00;
    PORTB = 0x00;

    // ----- ADC EINMALIG konfigurieren -----
    ADMUX = (1 << MUX0) | (1 << REFS0);        // ADC1, AVCC
    ADCSRA = (1 << ADEN) | (1 << ADPS2);       // Prescaler 16

    // Dummy read
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    (void)ADCW;

    // ---------------------------------------------------------
    // Hauptloop
    // ---------------------------------------------------------

    while (1)
    {
        uint16_t result = adc_read_avg(4);

        // Pegel entscheiden
        if (result < 500)
        {
            set_level(0);
            PORTB |= (1 << PB0);
            blink_pb0(3);
        }
        else if (result < 577) { set_level(0);  PORTB |= (1 << PB0) | (1 << PB1); }
        else if (result < 633) { set_level(1);  PORTB |= (1 << PB0) | (1 << PB1); }
        else if (result < 688) { set_level(2);  PORTB |= (1 << PB0) | (1 << PB1); }
        else if (result < 744) { set_level(3);  PORTB |= (1 << PB0) | (1 << PB1); }
        else if (result < 800) { set_level(4);  PORTB |= (1 << PB0) | (1 << PB1); }
        else if (result < 856) { set_level(5);  PORTB |= (1 << PB0) | (1 << PB1); }
        else if (result < 912) { set_level(6);  PORTB |= (1 << PB0) | (1 << PB1); }
        else if (result < 968) { set_level(7);  PORTB |= (1 << PB0) | (1 << PB1); }
        else {
            set_level(8);  // alle LEDs an
            PORTB |= (1 << PB0) | (1 << PB1);
        }
    }
}
