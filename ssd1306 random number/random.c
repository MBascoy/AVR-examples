#include <avr/io.h>
#include <avr/power.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd.h"

#define NUMBER_DIGITS 2

void timer1_init() {
    TCCR1B |= (1 << WGM12); // Modo CTC
    TCCR1B |= (1 << CS10); // Sin preescaler
    OCR1A = 65535;
}

int get_random(int a, int b){
    int numberDiff = b - a + 1;
    return (TCNT1 % numberDiff) + a; //Usamos el timer1 para obtener un valor y hacemos los calculos para que esté en el rango que se ha indicado
}

void print_number(int number){
  char number_string[NUMBER_DIGITS] = {0};
  int i = NUMBER_DIGITS;

  while(number > 0){
    number_string[i-1] = number%10;
    number = number / 10;
    i--;
  }

  for(i=0; i<NUMBER_DIGITS; i++){
    lcd_putc(48 + number_string[i]);
  }
}

ISR(INT0_vect) {

    for(int i=0; i < 10; i++){
        lcd_gotoxy(0,4);
        lcd_putc(48+i);
        lcd_putc(57-i);
        _delay_ms(50);
    }

    lcd_gotoxy(0,4);
    print_number(get_random(0,3));

    EIFR |= (1 << INTF0); //eliminamos posibles interrupciones generadas durante el ISR
}

int main(void) {
    clock_prescale_set(clock_div_1);

    DDRB = 0xFF; //Todos los puertos del registro B como salida
    PORTD |= (1 << PD2); // pull-up resistor en PD2

    EIMSK |= (1 << INT0); //Activamos INT0 para interrupciones
    EICRA |= (1 << ISC01); //Generamos interrupción cuando haya un cambio en el puerto
    sei(); //Activamos interrupciones globalmente

    timer1_init(); // Inicializar Timer1

    lcd_init(LCD_DISP_ON);    // init lcd and turn on
    lcd_puts("Random numbers");  // put string from RAM to display (TEXTMODE) or buffer (GRAPHICMODE)
    lcd_charMode(DOUBLESIZE);

    while (1) {
    }
}
