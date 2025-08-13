#include <util/delay.h>
#include <avr/power.h>
#include <avr/interrupt.h>

#include "lcd.h"

#define NUMBER_DIGITS 2

#define TIME_CALIBRATION -4


volatile uint16_t time_counter = 0;
volatile uint8_t seconds = 0;
volatile uint8_t minutes = 0;
volatile uint8_t hours = 0;

void timer1_init() {
    TCCR1B |= (1 << WGM12);                // Modo CTC
    OCR1A = 64000 + TIME_CALIBRATION;
    TCCR1B |= (1 << CS11);   // Prescaler 8
    TIMSK1 |= (1 << OCIE1A); // Habilitar interrupción por OCR1A
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

ISR(TIMER1_COMPA_vect) {
    time_counter += 64;

    if(time_counter >= 1000){
        time_counter -= 1000;
        seconds++;  // Incrementar cada segundo

      if(seconds == 60){
        seconds = 0;
        minutes++;
      }

      if(minutes == 60){
        minutes = 0;
        hours++;
      }

      if(hours == 24){
        hours = 0;
      }

      lcd_gotoxy(0,3);
      print_number(hours);
      lcd_putc(':');
      print_number(minutes);
      lcd_putc(':');
      print_number(seconds);
      lcd_gotoxy(0,5);
      lcd_puts("||||||||"); // Caracter modificado en la librería para hacer un subrayado
    }
}

int main(void) {
  clock_prescale_set(clock_div_1);

  PORTD = 0b00000111;

  timer1_init();     // Inicializar Timer1
  sei();

  lcd_init(LCD_DISP_ON);    // init lcd and turn on

  lcd_puts("Mi bonito reloj");  // put string from RAM to display (TEXTMODE) or buffer (GRAPHICMODE)
  lcd_charMode(DOUBLESIZE);


    while (1) {

      if(bit_is_clear(PIND, PD0)){
        seconds = 0;
        time_counter = 0;
        TCNT1 = 0;
        _delay_ms(200);
      }
      else if(bit_is_clear(PIND, PD1)){
        minutes++;
        _delay_ms(200);
      }
      else if(bit_is_clear(PIND, PD2)){
        hours++;
        _delay_ms(200);
      }

    }
}
