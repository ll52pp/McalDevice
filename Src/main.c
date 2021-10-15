#include "main.h"
#include "adc.h"
#include "gpioLED3.h"
#include "delay.h"
#include "i2c.h"
#include "usart.h"
#include "spi.h"
#include "iwdg.h"
#include "delay.h"
#include "tim2.h"
#include "tim3.h"
#include "tim4.h"
#include "tim5.h"
#include "tim8.h"
#include "tim16.h"
#include "key.h"
#include "app.h"
#include "tms3705.h"
#include <usr_mem_define.h>

void SystemClock_Config(void);
int main(void)
 {
  HAL_Init();	 
  SystemClock_Config();
	delay_init(40u);

/*clear ram*/	 
	ram_clear();
	 
/*����1*/
	MX_UART1_Init();
	 
/*LED�Ƴ�ʼ��PE2->��������*/
	LED3_Init();
	 
/*300 KHz*/
	MX_TIM8_Init(133u,80u);
	 
/*BEEP*/
	TIM3_PWM_Init(10u,5u);	

/*��Դ��ť��ʱ��*/
	My_TIM4_Init();
	 
/*����������*/
	Beep_start();

/*9K Hz*/
	TIM16_PWM_Init(9.0f);	 
	 
/*PWM-Bridge_EN*/
	ENABLE_Init(); 
	
/*12V,5V��Դʹ��*/
  V_12_5_EN(EN);
	
/*���״̬��ʾ,PG*/
 CHG_STATE();
	
/*������ʼ��*/
	KEY_Init();
	
/*LED״ָ̬ʾ*/
	LED_Indicate_Init();
	
/*��ɼ���*/
	LTC2943_Init();

/*NFC,�������,����*/	 
	PortRx_Init();					
	TIM5_Init();
	NFC_TXCT();

/*ADC�ɼ�*/
	MY_ADC_Init();

	Power_On_Off(NEN);
  while (1)						
  {				
#if EN
			if(*((uint8_t *)KEY_POWER_PRE_FLG) == 0x66u)		/*�����簴ť���²���ͨ�ųɹ�����Ὺ���������Ч��*/
			{
				chg_p_judge(5u,10u);	/*5�벻��磬10��ֹͣ*/
			}			

		if(Chg_Current() < 0u)	  		/*���������û���ڳ��*/
		{
			Chg_Self_Stau(NEN);		 	  /*���������û���ڳ��*/				
			key_all_scan();						/*����ɨ��*/							
			Key_Ble_Pres();						/*�������ذ�ť*/
			Key_Stimulate_On_Pres();	/*�̼�����ť*/
			Key_Stimulate_Off_Pres();	/*�̼��ذ�ť*/
			Key_Power_pres();					/*��簴ť*/	
		}
		else									
		{
			Chg_Self_Stau(EN);			/*����������ڳ��*/
			Chg_Volate();						/*����ص�ѹ*/
			Chg_Temp();							/*������¶�*/
			CHG_Collect();
		}
#endif
	}
}




























































































/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
