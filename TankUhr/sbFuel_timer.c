/*
 * sbFuel.c – Version mit Timer0 für Blinkfunktion
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#ifndef F_CPU
# define F_CPU 1000000UL
#endif

// -------------------------------------------------------------
// Globale Variablen
// -------------------------------------------------------------
volatile uint8_t blink_enabled = 0;   // 1 = PB0 blinkt, 0 = aus

// -------------------------------------------------------------
// Timer0: 100ms Toggle ISR
// -------------------------------------------------------------
ISR(TIMER0_COMPA_vect)
{
    if (blink_enabled)
        PORTB ^= (1 << PB0);  // PB0 toggeln
}

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
        while (ADCSRA & (1 << ADSC));
        sum += ADCW;
    }

    return sum / samples;
}

// LEDs auf PORTD setzen: 0–8 aktive LEDs
void set_level(uint8_t leds)
{
    uint8_t mask = 0;

    for(uint8_t i = 0; i < leds; i++)
        mask |= (1 << i);

    PORTD = mask;
}

// -------------------------------------------------------------
// Main
// -------------------------------------------------------------
int main(void)
{
    // I/O
    DDRD = 0xFF;
    DDRB = 0xFF;
    PORTD = 0x00;
    PORTB = 0x00;

    // ----- ADC Setup -----
    ADMUX  = (1 << MUX0) | (1 << REFS0); // ADC1, AVCC
    ADCSRA = (1 << ADEN) | (1 << ADPS2); // Prescaler 16

    // Dummy Read
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    (void)ADCW;

    // ----- Timer0 Setup (CTC, 100 ms) -----
    TCCR0A = (1 << WGM01);      // CTC Mode
    TCCR0B = (1 << CS01) | (1 << CS00); // Prescaler 64
    OCR0A  = 156;               // 100 ms bei 1 MHz
    TIMSK0 = (1 << OCIE0A);     // Compare Match Interrupt

    sei(); // Interrupts einschalten

    // ---------------------------------------------------------
    // Hauptloop
    // ---------------------------------------------------------
    while (1)
    {
        uint16_t result = adc_read_avg(4);

        // Untere Warnstufe → Blinken aktivieren
        if (result < 500)
        {
            blink_enabled = 1;
            PORTB |= (1 << PB1);    // PB1 dauerhaft On (Warnung)
            set_level(0);
        }
        else
        {
            blink_enabled = 0;      // Blinken aus
            PORTB &= ~(1 << PB0);   // sicher PB0 aus
            PORTB |= (1 << PB0) | (1 << PB1);
        }

        // LED-Balken
        if      (result < 500) set_level(0);
        else if (result < 577) set_level(0);
        else if (result < 633) set_level(1);
        else if (result < 688) set_level(2);
        else if (result < 744) set_level(3);
        else if (result < 800) set_level(4);
        else if (result < 856) set_level(5);
        else if (result < 912) set_level(6);
        else if (result < 968) set_level(7);
        else                   set_level(8);
    }
}
