#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>

#define SLAVE_ADDR 0x08
#define BUFFER_SIZE 256

/* Globals */
volatile unsigned int data_len = 0;
volatile unsigned int bytes_received = 0;
char json_buffer[BUFFER_SIZE];
volatile unsigned char receiving_length = 1; /* first expect length MSB */
unsigned char length_bytes[2];
volatile unsigned char length_byte_index = 0;
volatile unsigned char data_ready = 0;

void i2c_set_address()
{
    TWAR = (SLAVE_ADDR << 1); /* Set own slave address */
}

// Reset TWCR
void i2c_reset_twcr()
{
    TWCR = (1 << TWEN) |  /* Enable TWI */
           (1 << TWEA) |  /* Enable ACK generation */
           (1 << TWINT) | /* Clear interrupt flag */
           (1 << TWIE);   /* Enable TWI interrupt */
}

/* Initialize TWI (I2C) as slave */
void i2c_slave_init(void)
{
    i2c_set_address();
    i2c_reset_twcr();
}

/* TWI interrupt service routine to handle receiving bytes */
ISR(TWI_vect)
{
    unsigned char status;

    status = TWSR & 0xF8;

    switch (status)
    {
    case 0x60: /* Own SLA+W received, ACK returned */
    case 0x68: /* Arbitration lost, own SLA+W received */
        length_byte_index = 0;
        data_len = 0;
        bytes_received = 0;
        receiving_length = 1;
        data_ready = 0;
        i2c_reset_twcr(); /* ACK next */
        break;

    case 0x80: /* Data received, ACK returned */
        if (receiving_length)
        {
            length_bytes[length_byte_index++] = TWDR;
            if (length_byte_index == 2)
            {
                data_len = ((unsigned int)length_bytes[0] << 8) | length_bytes[1];
                if (data_len > (BUFFER_SIZE - 1))
                {
                    data_len = BUFFER_SIZE - 1;
                }
                receiving_length = 0;
            }
        }
        else
        {
            if (bytes_received < data_len)
            {
                json_buffer[bytes_received++] = TWDR;
            }
        }
        i2c_reset_twcr(); /* ACK next */
        break;

    case 0xA0: /* STOP or repeated START received */
        /* End of reception */
        if ((bytes_received == data_len) && (data_len > 0))
        {
            json_buffer[bytes_received] = '\0'; /* Null terminate string */
            data_ready = 1;
        }
        i2c_reset_twcr(); /* Ready for next reception */
        break;

    default:
        /* For other statuses, just ACK to continue */
        i2c_reset_twcr();
        break;
    }
}

int main(void)
{
    /* Setup PB5 as output (Arduino onboard LED pin) */
    DDRB |= (1 << PB5);

    i2c_slave_init();

    /* Enable global interrupts */
    sei();

    while (1)
    {
        if (data_ready)
        {
            if (strstr(json_buffer, "\"led\":1") != 0)
            {
                PORTB |= (1 << PB5); /* Turn LED ON */
            }
            else if (strstr(json_buffer, "\"led\":0") != 0)
            {
                PORTB &= ~(1 << PB5); /* Turn LED OFF */
            }
            data_ready = 0;
        }
    }
    /* never reached */
    return 0;
}
