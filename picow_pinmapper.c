#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "hardware/divider.h"
#include "pico/cyw43_arch.h"

// UART defines
// By default the stdout UART is `uart0`, so we will use the second one
#define UART_ID uart1
#define BAUD_RATE 115200

// Use pins 4 and 5 for UART1
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define UART_TX_PIN 4
#define UART_RX_PIN 5
#define PIN_COUNT 29

int main()
{
    stdio_init_all();

    // Initialise the Wi-Fi chip
    // if (cyw43_arch_init())
    // {
    //     printf("Wi-Fi init failed\n");
    //     return -1;
    // }

    // Set up our UART
    uart_init(UART_ID, BAUD_RATE);
    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    // For more examples of UART use see https://github.com/raspberrypi/pico-examples/tree/master/uart

    // GPIO initialisation.

    // Example to turn on the Pico W LED
    // cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    stdio_init_all();

    while (true)
    {
        sleep_ms(400);
        
        int connections[PIN_COUNT][PIN_COUNT] = {0};

        for (int i = 0; i < PIN_COUNT; i++)
        {
            gpio_init(i);
            gpio_set_dir(i, GPIO_OUT);
            gpio_put(i, 1);
            gpio_set_pulls(i, 1, 0);
            int currentpin = i;
            // printf("Main pin: %i \n", i);

            for (int j = 0; j < PIN_COUNT; j++)
            {
                if (j != currentpin)
                {
                    gpio_init(j);
                    gpio_set_dir(j, GPIO_IN);
                    gpio_put(j, 0);
                    gpio_set_pulls(j, 0, 1);
                    
                }
            }

            for (int j = 0; j < PIN_COUNT; j++)
            {
                if (j != currentpin)
                {
                    bool output = gpio_get(j);
                    connections[currentpin][j] = output ? 1 : 0;
                }
            }
        }
        int total=0;
        // Print the matrix and calculate total pin count for each row
        printf("Pin Connections:\n");
        for (int i = 0; i < PIN_COUNT; i++)
        {
            int total_pins_connected = 0;
            for (int j = 0; j < PIN_COUNT; j++)
            {
                printf("%d ", connections[i][j]);
                if (connections[i][j] == 1)
                {
                    total_pins_connected++;
                    total++;
                }
            }
            printf("Total pins connected for pin %d: %d\n", i, total_pins_connected);
        }
        printf("Total: %d",total);
        printf("\n");
    }
}