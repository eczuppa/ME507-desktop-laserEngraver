/** @file laser.cpp
 *      This file contains the code which sets the laser PWM signal. 
 * 
 *  @author  Niko Banks
 *  @date    7-02-2022 File Created
 *  @date    7-02-2022 Last Updated
 * 
*/

#include "libraries&constants.h"

void initialize_laser(void)
{
    //There's a great video on youtube (https://www.youtube.com/watch?v=txnViYePocg) explaining where a lot of this code comes from, and the use of 
    //STM32CubeMX to generate code makes it much simpler than trying to read through forums. 

    //Laser pin is PB0, which defaults to using timer 1 for its PWM signal. We need to change that to timer 8 so timer 1 can be used for one of the encoders.

    // TIM_HandleTypeDef htim8;

    // TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    // TIM_MasterConfigTypeDef sMasterConfig = {0};
    // TIM_OC_InitTypeDef sConfigOC = {0};

    // TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

    // //Set Timer 8 Parameters and initialize timer
    // htim8.Instance = TIM8;
    // htim8.Init.Prescaler = 0;
    // htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
    // htim8.Init.Period = 255;
    // htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    // htim8.Init.RepetitionCounter = 0;
    // htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    // HAL_TIM_Base_Init(&htim8);

    // //Set Clock Source
    // sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    // HAL_TIM_ConfigClockSource(&htim8,&sClockSourceConfig);

    // //Initialize PWM
    // HAL_TIM_PWM_Init(&htim8);

    // //Trigger Output (TRGO) Parameters (not sure this is necessary, but putting it in anyway)
    // sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    // sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
    // sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    // HAL_TIMEx_MasterConfigSynchronization(&htim8,&sMasterConfig);


    // //Set output parameters
    // sConfigOC.OCMode = TIM_OCMODE_PWM1;
    // sConfigOC.Pulse = 127;                               //This is the duty cycle, in a number out of the total htim8.Init.Period. So setting this to the same value as htim8.Init.Period will create a duty cycle of 100%.
    // sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    // sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    // sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    // sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    // sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    // HAL_TIM_PWM_ConfigChannel(&htim8,&sConfigOC,TIM_CHANNEL_2);

    // //Not sure what this is, but the Nucleo adds this section:
    // sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
    // sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
    // sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
    // sBreakDeadTimeConfig.DeadTime = 0;
    // sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
    // sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
    // sBreakDeadTimeConfig.BreakFilter = 0;
    // sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
    // sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
    // sBreakDeadTimeConfig.Break2Filter = 0;
    // sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
    // HAL_TIMEx_ConfigBreakDeadTime(&htim8,&sBreakDeadTimeConfig);

    // //Initialize Pin
    // GPIO_InitTypeDef GPIO_InitStruct = {0};
    // __HAL_RCC_GPIOB_CLK_ENABLE();

    // GPIO_InitStruct.Pin = GPIO_PIN_0;
    // GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    // GPIO_InitStruct.Pull = GPIO_NOPULL;
    // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    // GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;
    // HAL_GPIO_Init(GPIOB,&GPIO_InitStruct);


    // //Run initialization functions
    // // HAL_TIM_PWM_MspInit(&htim8);

    // //Start Timer
    // HAL_TIM_PWM_Start(&htim8,LL_TIM_CHANNEL_CH2N);

    // Reset of all peripherals, initializes the flash interface and the systick.
    // HAL_Init();

    // Serial << "Checkpoint" << endl;

    // // Configure the system clock
    // SystemClock_Config();

    


    // TIM_HandleTypeDef htim3;

    // TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    // TIM_MasterConfigTypeDef sMasterConfig = {0};
    // TIM_OC_InitTypeDef sConfigOC = {0};

    // //Set Timer 8 Parameters and initialize timer
    // htim3.Instance = TIM3;
    // htim3.Init.Prescaler = 0;
    // htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    // htim3.Init.Period = 255;
    // htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    // htim3.Init.RepetitionCounter = 0;
    // htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    // if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
    // {
    //     Error_Handler();
    //     Serial << "Error 1" << endl;
    // }

    // //Set Clock Source
    // sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    // if (HAL_TIM_ConfigClockSource(&htim3,&sClockSourceConfig) != HAL_OK)
    // {
    //     Error_Handler();
    //     Serial << "Error 2" << endl;
    // }

    // //Initialize PWM
    // if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
    // {
    //     Error_Handler();
    //     Serial << "Error 3" << endl;
    // }

    // //Trigger Output (TRGO) Parameters (not sure this is necessary, but putting it in anyway)
    // sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    // sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
    // sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    // if (HAL_TIMEx_MasterConfigSynchronization(&htim3,&sMasterConfig) != HAL_OK)
    // {
    //     Error_Handler();
    //     Serial << "Error 4" << endl;
    // }


    // //Set output parameters
    // sConfigOC.OCMode = TIM_OCMODE_PWM1;
    // sConfigOC.Pulse = 127;                               //This is the duty cycle, in a number out of the total htim3.Init.Period. So setting this to the same value as htim3.Init.Period will create a duty cycle of 100%.
    // sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    // sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    // sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    // sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    // sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    // if (HAL_TIM_PWM_ConfigChannel(&htim3,&sConfigOC,TIM_CHANNEL_3) != HAL_OK)
    // {
    //     Error_Handler();
    //     Serial << "Error 5" << endl;
    // }

    // //Initialize Pin
    // GPIO_InitTypeDef GPIO_InitStruct = {0};
    // __HAL_RCC_GPIOB_CLK_ENABLE();

    // GPIO_InitStruct.Pin = GPIO_PIN_0;
    // GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    // GPIO_InitStruct.Pull = GPIO_NOPULL;
    // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    // GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
    // HAL_GPIO_Init(GPIOB,&GPIO_InitStruct);


    // //Run initialization functions
    // HAL_TIM_PWM_MspInit(&htim3);

    // //Start Timer
    // HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);



    // Start the Uart
    // UART_HandleTypeDef huart2;

    // huart2.Instance = USART2;
    // huart2.Init.BaudRate = 115200;
    // huart2.Init.WordLength = UART_WORDLENGTH_8B;
    // huart2.Init.StopBits = UART_STOPBITS_1;
    // huart2.Init.Parity = UART_PARITY_NONE;
    // huart2.Init.Mode = UART_MODE_TX_RX;
    // huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    // huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    // huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    // huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    // if (HAL_UART_Init(&huart2) != HAL_OK)
    // {
    //     Error_Handler();
    //     Serial << "Error 0" << endl;
    // }

}

void set_laser_PWM(uint8_t laser_PWM_signal)
{
    //Check to make sure laser signal is within range, correct it if not
    if(laser_PWM_signal>100)
    {
        laser_PWM_signal = 100;
    }
    else if(laser_PWM_signal<0)
    {
        laser_PWM_signal = 0;
    }
    //Convert laser PWM input to 0-255 scale for analogWrite()
    laser_PWM_signal = (float)laser_PWM_signal*255/100;
    
    //Set laser PWM
    // analogWrite(L_PWM,laser_PWM_signal);
    analogWrite(LASER_ADC_2,laser_PWM_signal);

}


void laser_test_task(void* p_params)
{
    // Account for the length of time it takes to run the task in the task timing requirement
    TickType_t xLastWakeTime = xTaskGetTickCount();

    //Initialize laser % signal
    uint8_t value = 0;

    //Correct the laser timer issue
    initialize_laser();

    for(;;)
    {
        //Increment once every task period
        value +=25;

        //If we're past 100, recycle value
        if(value>100)
        {
            value = 0;
        }

        set_laser_PWM(value);
        // Delay until reset        
        vTaskDelayUntil(&xLastWakeTime, 1000);
    }
}
