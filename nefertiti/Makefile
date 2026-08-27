APPLICATION = nefertiti

BOARD = stm32f4discovery
SRC += main.c
SRC += trust.c
SRC += crypto.c
SRC += asconopt.c
#SRC += ascon_table.c
CFLAGS += -DASCON_OPTIMIZED


USEMODULE += xtimer
USEMODULE += stdio_uart

STDIO_UART_DEV = USART2
BAUDRATE = 115200

include $(RIOTBASE)/Makefile.include

