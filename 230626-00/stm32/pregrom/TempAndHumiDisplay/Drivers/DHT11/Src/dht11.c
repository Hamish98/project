#include "stm32f4xx_hal.h"

#define DHT11_GPIO_PORT     GPIOA
#define DHT11_GPIO_PIN      GPIO_PIN_0

void DHT11_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    // Enable GPIO clock
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // Configure GPIO pin as output
    GPIO_InitStruct.Pin = DHT11_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStruct);

    // Set the pin to high
    HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_SET);
}

void DHT11_Delay(uint32_t us)
{
    // Implement a delay function here (microseconds)
    // You can use HAL_Delay() if you want to delay in milliseconds
}

uint8_t DHT11_ReadByte(void)
{
    uint8_t data = 0;

    // Read 8 bits
    for (int i = 0; i < 8; i++)
    {
        // Wait for the pin to go low
        while (!HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN));

        // Delay for ~50us
        DHT11_Delay(50);

        // Check if the pin is high
        if (HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN))
        {
            // Set the corresponding bit in the data variable
            data |= (1 << (7 - i));
        }

        // Wait for the pin to go high
        while (HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN));
    }

    return data;
}

void DHT11_ReadData(uint8_t* humidity, uint8_t* temperature)
{
    uint8_t data[5];
    uint8_t checksum;

    // Initialize the data array
    for (int i = 0; i < 5; i++)
    {
        data[i] = 0;
    }

    // Set the pin to low
    HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_RESET);

    // Delay for ~18ms
    DHT11_Delay(18000);

    // Set the pin to high
    HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_SET);

    // Wait for the pin to go low
    while (HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN));

    // Wait for the pin to go high
    while (!HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN));

    // Read 40 bits (5 bytes)
    for (int i = 0; i < 5; i++)
    {
        data[i] = DHT11_ReadByte();
    }

    // Calculate checksum
    checksum = data[0] + data[1] + data[2] + data[3];

    // Check if the checksum is correct
    if (checksum == data[4])
    {
        *humidity = data[0];
        *temperature = data[2];
    }
}


