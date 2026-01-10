/*

 * sbFuel.c

 *

 * Created: 04.01.2015

 *  Author: ziolar

 */

#include <avr/io.h>

#include <util/delay.h>

#ifndef F_CPU

/* prevent compiler error by supplying a default */

# warning "F_CPU ist nicht definiert, setze auf 1Mhz"

# define F_CPU    1000000UL

#endif

int main(void)

{

	DDRD = 0xFF;
	PORTD = 0x00;
	DDRB = 0xFF;
	PORTB = 0x00;

	while (1)

	{

		uint8_t i;

		uint16_t result;

		int i2 = 4;

		// Kanal des Multiplexers waehlen

		// AVCC Referenzspannung verwenden

		ADMUX = (1 << MUX0) | (0 << REFS1) | (1 << REFS0);

		ADCSRA = (1 << ADEN) | (1 << ADPS2) | (0 << ADPS1); // Frequenzvorteiler

		// setzen auf 16 (1) und ADC aktivieren (1)

		/* nach Aktivieren des ADC wird ein "Dummy-Readout" empfohlen, man liest

		 also einen Wert und verwirft diesen, um den ADC "warmlaufen zu lassen" */
		ADCSRA |= (1 << ADSC);              // eine ADC-Wandlung

		while (ADCSRA & (1 << ADSC))

		{

			;     // auf Abschluss der Konvertierung warten

		}

		result = ADCW;  // ADCW muss einmal gelesen werden,

		// sonst wird Ergebnis der nächsten Wandlung

		// nicht übernommen.

		/* Eigentliche Messung - Mittelwert aus 4 aufeinanderfolgenden Wandlungen */

		result = 0;

		for (i = 0; i < i2; i++)

		{

			ADCSRA |= (1 << ADSC);          // eine Wandlung "single conversion"

			while (ADCSRA & (1 << ADSC))

			{

				;   // auf Abschluss der Konvertierung warten

			}

			result += ADCW;            // Wandlungsergebnisse aufaddieren

		}

		ADCSRA &= ~(1 << ADEN);             // ADC deaktivieren (2)

		result /= i2;            // Summe durch vier teilen = arithm. Mittelwert

		if (result < 500)
		{
			PORTB |= (1 << PB0);
			PORTB &= ~((1 << PB1));
			PORTD &= ~((1 < PD7) | (1 << PD6) | (1 << PD5) | (1 << PD4) | (1 << PD3) | (1 << PD2) | (1 << PD1) | (1 << PD0));

			//PORTD = 0xff;
			//PORTB = 0x03;

			for (int zaehler = 0; zaehler < 3; zaehler++) {
				PORTB ^= (1<<PB0);
				_delay_ms(50);
			}

		}


		else if (result >= 500 && result < 577) {
			PORTB |= (1 << PB0) | (1 << PB1);
			PORTD &= ~((1 < PD7) | (1 << PD6) | (1 << PD5) | (1 << PD4) | (1 << PD3) | (1 << PD2) | (1 << PD1) | (1 << PD0));

			//PORTD = 0xff;
			//PORTB = 0x01;

			for(int zaehler = 0; zaehler < 3; zaehler++){
				PORTB ^= (1<<PB0) | (1<<PB1);
			}
		}

		else if (result >= 577 && result < 633) {
			PORTB |= (1 << PB0) | (1 << PB1);
			PORTD |= (1 << PD7);
			PORTD &= ~((1 << PD6) | (1 << PD5) | (1 << PD4) | (1 << PD3) | (1 << PD2) | (1 << PD1) | (1 << PD0));

			//PORTD = 0xff;
			//PORTB = 0x00;
		}

		else if (result >= 633 && result < 688) {
			PORTB |= (1 << PB0) | (1 << PB1);
			PORTD |= (1 << PD7) | (1 << PD6);
			PORTD &= ~((1 << PD5) | (1 << PD4) | (1 << PD3) | (1 << PD2) | (1 << PD1) | (1 << PD0));
			//PORTD = 0x7f;
			//PORTB = 0x00;
		}

		else if (result >= 688 && result < 744) {
			PORTB |= (1 << PB0) | (1 << PB1);
			PORTD |= (1 << PD7) | (1 << PD6) | (1 << PD5);
			PORTD &= ~((1 << PD4) | (1 << PD3) | (1 << PD2) | (1 << PD1) | (1 << PD0));
			//PORTD = 0x3f;
			//PORTB = 0x00;
		}

		else if (result >= 744 && result < 800) {
			PORTB |= (1 << PB0) | (1 << PB1);
			PORTD |= (1 << PD7) | (1 << PD6) | (1 << PD5) | (1 << PD4);
			PORTD &= ~((1 << PD3) | (1 << PD2) | (1 << PD1) | (1 << PD0));
			//PORTD = 0x1f;
			//PORTB = 0x00;
		} else if (result >= 800 && result < 856) {
			PORTB |= (1 << PB0) | (1 << PB1);
			PORTD |= (1 << PD7) | (1 << PD6) | (1 << PD5) | (1 << PD4) | (1 << PD3);
			PORTD &= ~((1 << PD2) | (1 << PD1) | (1 << PD0));
			//PORTD = 0x0f;
			//PORTB = 0x00;
		} else if (result >= 856 && result < 912) {
			PORTB |= (1 << PB0) | (1 << PB1);
			PORTD |= (1 << PD7) | (1 << PD6) | (1 << PD5) | (1 << PD4) | (1 << PD3) | (1 << PD2);
			PORTD &= ~((1 << PD1) | (1 << PD0));
		} else if (result >= 912 && result < 968) {
			PORTB |= (1 << PB0) | (1 << PB1);
			PORTD |= (1 << PD7) | (1 << PD6) | (1 << PD5) | (1 << PD4)	| (1 << PD3) | (1 << PD2) | (1 << PD1);
			PORTD &= ~((1 << PD0));
		} else if (result >= 968) {
			// Blinkphase, erst aus, was auch muss
			PORTB |= (1 << PB0) | (1 << PB1);
			PORTD |= (1 << PD7) | (1 << PD6) | (1 << PD5) | (1 << PD4) | (1 << PD3) | (1 << PD2) | (1 << PD1) | (1 << PD0);
		}
	}
	return 0;
}
