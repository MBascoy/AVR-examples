# Configuración principal
MCU   = atmega168pa
F_CPU = 8000000UL
BAUD  = 9600UL

PROGRAMMER = usbasp

CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -DBAUD=$(BAUD) -Os -Wall
LDFLAGS = -mmcu=$(MCU)

SRC = watch.c lcd.c i2c.c font.c
OBJ = $(SRC:.c=.o)

TARGET = watch

# Reglas de compilación
all: $(TARGET).hex

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).elf: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# Flasheo al microcontrolador
flash: $(TARGET).hex
	$(AVRDUDE) -c $(PROGRAMMER) -p $(MCU) -U flash:w:$(TARGET).hex:i

# Limpieza
clean:
	rm -f *.o *.elf *.hex
