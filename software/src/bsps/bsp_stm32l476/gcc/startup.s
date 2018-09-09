/*
Copyright(c) 2017 Cedric Jimenez

This file is part of Open-Vario.

Open-Vario is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Open-Vario is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Open-Vario.  If not, see <http://www.gnu.org/licenses/>.
*/

    .syntax unified
    .cpu cortex-m4
    .thumb
    
    .section .startup

    .global _start

    .extern pll_init
    .extern c_init
    .extern cpp_init
    .extern main


    .extern _TOP_OF_STACKS_
 


_start:
_vectors:
    /* Cortex M4 internal interrupts */
    .include "cm4_internal_interrupts.s.inc"

    /* External Interrupts */
    .long     WWDG_IRQHandler                   /* Window WatchDog */                                       
    .long     PVD_PVM_IRQHandler                /* PVD through EXTI Line detection */                       
    .long     TAMP_STAMP_IRQHandler             /* Tamper and TimeStamps through the EXTI line */            
    .long     RTC_WKUP_IRQHandler               /* RTC Wakeup through the EXTI line */                       
    .long     FLASH_IRQHandler                  /* FLASH */                                          
    .long     RCC_IRQHandler                    /* RCC */                                            
    .long     EXTI0_IRQHandler                  /* EXTI Line0 */                                            
    .long     EXTI1_IRQHandler                  /* EXTI Line1 */                                            
    .long     EXTI2_IRQHandler                  /* EXTI Line2 */                                            
    .long     EXTI3_IRQHandler                  /* EXTI Line3 */                                            
    .long     EXTI4_IRQHandler                  /* EXTI Line4 */                                            
    .long     DMA1_Channel1_IRQHandler          /* DMA1 Stream 1 */                                  
    .long     DMA1_Channel2_IRQHandler          /* DMA1 Stream 2 */                                  
    .long     DMA1_Channel3_IRQHandler          /* DMA1 Stream 3 */                                  
    .long     DMA1_Channel4_IRQHandler          /* DMA1 Stream 4 */                                  
    .long     DMA1_Channel5_IRQHandler          /* DMA1 Stream 5 */                                  
    .long     DMA1_Channel6_IRQHandler          /* DMA1 Stream 6 */                                  
    .long     DMA1_Channel7_IRQHandler          /* DMA1 Stream 7 */                                  
    .long     ADC1_2_IRQHandler                 /* ADC1, ADC2 */                            
    .long     CAN1_TX_IRQHandler                /* CAN1 TX */                                               
    .long     CAN1_RX0_IRQHandler               /* CAN1 RX0 */                                              
    .long     CAN1_RX1_IRQHandler               /* CAN1 RX1 */                                              
    .long     CAN1_SCE_IRQHandler               /* CAN1 SCE */                                              
    .long     EXTI9_5_IRQHandler                /* External Line[9:5]s */                                   
    .long     TIM1_BRK_TIM15_IRQHandler         /* TIM1 Break and TIM15 */                  
    .long     TIM1_UP_TIM16_IRQHandler          /* TIM1 Update and TIM16 */                
    .long     TIM1_TRG_COM_TIM17_IRQHandler     /* TIM1 Trigger and Commutation and TIM17 */
    .long     TIM1_CC_IRQHandler                /* TIM1 Capture Compare */                                  
    .long     TIM2_IRQHandler 		  	        /* TIM2 was TIM2_IRQHandler */
    .long     TIM3_IRQHandler                   /* TIM3 */                                           
    .long     TIM4_IRQHandler                   /* TIM4 */                                           
    .long     I2C1_EV_IRQHandler                /* I2C1 Event */                                            
    .long     I2C1_ER_IRQHandler                /* I2C1 Error */                                            
    .long     I2C2_EV_IRQHandler                /* I2C2 Event */                                            
    .long     I2C2_ER_IRQHandler                /* I2C2 Error */                                              
    .long     SPI1_IRQHandler                   /* SPI1 */                                           
    .long     SPI2_IRQHandler                   /* SPI2 */                                           
    .long     USART1_IRQHandler                 /* USART1 */                                         
    .long     USART2_IRQHandler                 /* USART2 */                                         
    .long     USART3_IRQHandler           	    /* USART3 */
    .long     EXTI15_10_IRQHandler              /* External Line[15:10]s */                                 
    .long     RTC_Alarm_IRQHandler              /* RTC Alarm (A and B) through EXTI Line */                 
    .long     DFSDM1_FLT3_IRQHandler            /*  */                        
    .long     TIM8_BRK_IRQHandler               /* TIM8 Break */                 
    .long     TIM8_UP_IRQHandler                /* TIM8 Update */                
    .long     TIM8_TRG_COM_IRQHandler           /* TIM8 Trigger and Commutation */
    .long     TIM8_CC_IRQHandler                /* TIM8 Capture Compare */                                   
    .long     ADC3_IRQHandler                   /* ADC3 */                                           
    .long     FMC_IRQHandler                    /* FSMC */                                           
    .long     SDMMC1_IRQHandler                 /* SDIMMC1 */                                           
    .long     TIM5_IRQHandler                   /* TIM5 */                                           
    .long     SPI3_IRQHandler                   /* SPI3 */                                          
    .long     UART4_IRQHandler                  /* UART4 */                                          
    .long     UART5_IRQHandler                  /* UART5 */                                          
    .long     TIM6_DAC_IRQHandler               /* TIM6 and DAC1&2 underrun errors */                  
    .long     TIM7_IRQHandler                   /* TIM7 */                  
    .long     DMA2_Channel1_IRQHandler          /* DMA2 Stream 1 */                                  
    .long     DMA2_Channel2_IRQHandler          /* DMA2 Stream 2 */                                  
    .long     DMA2_Channel3_IRQHandler          /* DMA2 Stream 3 */                                  
    .long     DMA2_Channel4_IRQHandler          /* DMA2 Stream 4 */                                  
    .long     DMA2_Channel5_IRQHandler          /* DMA2 Stream 5 */                                  
    .long     DFSDM1_FLT0_IRQHandler            /*  */                                                       
    .long     DFSDM1_FLT1_IRQHandler            /*  */                        
    .long     DFSDM1_FLT2_IRQHandler            /*  */                        
    .long     COMP_IRQHandler                   /* COMP */                                              
    .long     LPTIM1_IRQHandler                 /* LPTIM1 */                                              
    .long     LPTIM2_IRQHandler                 /* LPTIM2 */                                              
    .long     OTG_FS_IRQHandler                 /* USB OTG FS */                                     
    .long     DMA2_Channel6_IRQHandler          /* DMA2 Stream 6 */                                  
    .long     DMA2_Channel7_IRQHandler          /* DMA2 Stream 7 */                                  
    .long     LPUART1_IRQHandler                /* LPUART1 */                                  
    .long     QUADSPI_IRQHandler                /* QUADSPI */                                          
    .long     I2C3_EV_IRQHandler                /* I2C3 event */                                            
    .long     I2C3_ER_IRQHandler                /* I2C3 error */                                            
    .long     SAI1_IRQHandler                   /* SAI1 */                     
    .long     SAI2_IRQHandler                   /* SAI2 */                      
    .long     SWPMI1_IRQHandler                 /* SWPMI1 */                       
    .long     TSC_IRQHandler                    /* TSC */                                     
    .long     LCD_IRQHandler                    /* LCD */                                     
    .long     0                                 /* Reserved */                                     
    .long     RNG_IRQHandler                    /* RNG */
    .long     FPU_IRQHandler                    /* FPU */


.thumb_func
.type Reset_Handler, %function
Reset_Handler:

    /* PLL init */
    bl pll_init

    /* C vars init */
    bl c_init

    /* C++ init */
    bl cpp_init

    /* Branch to main */
    bl main

    /* To prevent exiting from main */
    b  .
  
.thumb_func
.type Default_Handler, %function
Default_Handler:
	b  .
	


    .weak	NMI_Handler
	.thumb_set NMI_Handler,Default_Handler

    .weak	HardFault_Handler
	.thumb_set HardFault_Handler,Default_Handler

    .weak	MemManage_Handler
	.thumb_set MemManage_Handler,Default_Handler

    .weak	BusFault_Handler
	.thumb_set BusFault_Handler,Default_Handler

	.weak	UsageFault_Handler
	.thumb_set UsageFault_Handler,Default_Handler

	.weak	SVC_Handler
	.thumb_set SVC_Handler,Default_Handler

	.weak	DebugMon_Handler
	.thumb_set DebugMon_Handler,Default_Handler

	.weak	PendSV_Handler
	.thumb_set PendSV_Handler,Default_Handler

	.weak	SysTick_Handler
	.thumb_set SysTick_Handler,Default_Handler

	.weak	WWDG_IRQHandler
	.thumb_set WWDG_IRQHandler,Default_Handler

	.weak	PVD_PVM_IRQHandler
	.thumb_set PVD_PVM_IRQHandler,Default_Handler

	.weak	TAMP_STAMP_IRQHandler
	.thumb_set TAMP_STAMP_IRQHandler,Default_Handler

	.weak	RTC_WKUP_IRQHandler
	.thumb_set RTC_WKUP_IRQHandler,Default_Handler

	.weak	FLASH_IRQHandler
	.thumb_set FLASH_IRQHandler,Default_Handler

	.weak	RCC_IRQHandler
	.thumb_set RCC_IRQHandler,Default_Handler

	.weak	EXTI0_IRQHandler
	.thumb_set EXTI0_IRQHandler,Default_Handler

	.weak	EXTI1_IRQHandler
	.thumb_set EXTI1_IRQHandler,Default_Handler

	.weak	EXTI2_IRQHandler
	.thumb_set EXTI2_IRQHandler,Default_Handler

	.weak	EXTI3_IRQHandler
	.thumb_set EXTI3_IRQHandler,Default_Handler

	.weak	EXTI4_IRQHandler
	.thumb_set EXTI4_IRQHandler,Default_Handler

	.weak	DMA1_Channel1_IRQHandler
	.thumb_set DMA1_Channel1_IRQHandler,Default_Handler

	.weak	DMA1_Channel2_IRQHandler
	.thumb_set DMA1_Channel2_IRQHandler,Default_Handler

	.weak	DMA1_Channel3_IRQHandler
	.thumb_set DMA1_Channel3_IRQHandler,Default_Handler

	.weak	DMA1_Channel4_IRQHandler
	.thumb_set DMA1_Channel4_IRQHandler,Default_Handler

	.weak	DMA1_Channel5_IRQHandler
	.thumb_set DMA1_Channel5_IRQHandler,Default_Handler

	.weak	DMA1_Channel6_IRQHandler
	.thumb_set DMA1_Channel6_IRQHandler,Default_Handler

	.weak	DMA1_Channel7_IRQHandler
	.thumb_set DMA1_Channel7_IRQHandler,Default_Handler

	.weak	ADC1_2_IRQHandler
	.thumb_set ADC1_2_IRQHandler,Default_Handler

	.weak	CAN1_TX_IRQHandler
	.thumb_set CAN1_TX_IRQHandler,Default_Handler

	.weak	CAN1_RX0_IRQHandler
	.thumb_set CAN1_RX0_IRQHandler,Default_Handler

	.weak	CAN1_RX1_IRQHandler
	.thumb_set CAN1_RX1_IRQHandler,Default_Handler

	.weak	CAN1_SCE_IRQHandler
	.thumb_set CAN1_SCE_IRQHandler,Default_Handler

	.weak	EXTI9_5_IRQHandler
	.thumb_set EXTI9_5_IRQHandler,Default_Handler

	.weak	TIM1_BRK_TIM15_IRQHandler
	.thumb_set TIM1_BRK_TIM15_IRQHandler,Default_Handler

	.weak	TIM1_UP_TIM16_IRQHandler
	.thumb_set TIM1_UP_TIM16_IRQHandler,Default_Handler

	.weak	TIM1_TRG_COM_TIM17_IRQHandler
	.thumb_set TIM1_TRG_COM_TIM17_IRQHandler,Default_Handler

	.weak	TIM1_CC_IRQHandler
	.thumb_set TIM1_CC_IRQHandler,Default_Handler

	.weak	TIM2_IRQHandler
	.thumb_set TIM2_IRQHandler,Default_Handler

	.weak	TIM3_IRQHandler
	.thumb_set TIM3_IRQHandler,Default_Handler

	.weak	TIM4_IRQHandler
	.thumb_set TIM4_IRQHandler,Default_Handler

	.weak	I2C1_EV_IRQHandler
	.thumb_set I2C1_EV_IRQHandler,Default_Handler

	.weak	I2C1_ER_IRQHandler
	.thumb_set I2C1_ER_IRQHandler,Default_Handler

	.weak	I2C2_EV_IRQHandler
	.thumb_set I2C2_EV_IRQHandler,Default_Handler

	.weak	I2C2_ER_IRQHandler
	.thumb_set I2C2_ER_IRQHandler,Default_Handler

	.weak	SPI1_IRQHandler
	.thumb_set SPI1_IRQHandler,Default_Handler

	.weak	SPI2_IRQHandler
	.thumb_set SPI2_IRQHandler,Default_Handler

	.weak	USART1_IRQHandler
	.thumb_set USART1_IRQHandler,Default_Handler

	.weak	USART2_IRQHandler
	.thumb_set USART2_IRQHandler,Default_Handler

	.weak	USART3_IRQHandler
	.thumb_set USART3_IRQHandler,Default_Handler

	.weak	EXTI15_10_IRQHandler
	.thumb_set EXTI15_10_IRQHandler,Default_Handler

	.weak	RTC_Alarm_IRQHandler
	.thumb_set RTC_Alarm_IRQHandler,Default_Handler

	.weak	DFSDM1_FLT3_IRQHandler
	.thumb_set DFSDM1_FLT3_IRQHandler,Default_Handler

	.weak	TIM8_BRK_IRQHandler
	.thumb_set TIM8_BRK_IRQHandler,Default_Handler

	.weak	TIM8_UP_IRQHandler
	.thumb_set TIM8_UP_IRQHandler,Default_Handler

	.weak	TIM8_TRG_COM_IRQHandler
	.thumb_set TIM8_TRG_COM_IRQHandler,Default_Handler

	.weak	TIM8_CC_IRQHandler
	.thumb_set TIM8_CC_IRQHandler,Default_Handler

	.weak	ADC3_IRQHandler
	.thumb_set ADC3_IRQHandler,Default_Handler

	.weak	FMC_IRQHandler
	.thumb_set FMC_IRQHandler,Default_Handler

	.weak	SDMMC1_IRQHandler
	.thumb_set SDMMC1_IRQHandler,Default_Handler

	.weak	TIM5_IRQHandler
	.thumb_set TIM5_IRQHandler,Default_Handler

	.weak	SPI3_IRQHandler
	.thumb_set SPI3_IRQHandler,Default_Handler

	.weak	UART4_IRQHandler
	.thumb_set UART4_IRQHandler,Default_Handler

	.weak	UART5_IRQHandler
	.thumb_set UART5_IRQHandler,Default_Handler

	.weak	TIM6_DAC_IRQHandler
	.thumb_set TIM6_DAC_IRQHandler,Default_Handler

	.weak	TIM7_IRQHandler
	.thumb_set TIM7_IRQHandler,Default_Handler

	.weak	DMA2_Channel1_IRQHandler
	.thumb_set DMA2_Channel1_IRQHandler,Default_Handler

	.weak	DMA2_Channel2_IRQHandler
	.thumb_set DMA2_Channel2_IRQHandler,Default_Handler

	.weak	DMA2_Channel3_IRQHandler
	.thumb_set DMA2_Channel3_IRQHandler,Default_Handler

	.weak	DMA2_Channel4_IRQHandler
	.thumb_set DMA2_Channel4_IRQHandler,Default_Handler

	.weak	DMA2_Channel5_IRQHandler
	.thumb_set DMA2_Channel5_IRQHandler,Default_Handler

	.weak	DFSDM1_FLT0_IRQHandler
	.thumb_set DFSDM1_FLT0_IRQHandler,Default_Handler	
	
	.weak	DFSDM1_FLT1_IRQHandler
	.thumb_set DFSDM1_FLT1_IRQHandler,Default_Handler	
	
	.weak	DFSDM1_FLT2_IRQHandler
	.thumb_set DFSDM1_FLT2_IRQHandler,Default_Handler	
	
	.weak	COMP_IRQHandler
	.thumb_set COMP_IRQHandler,Default_Handler
	
	.weak	LPTIM1_IRQHandler
	.thumb_set LPTIM1_IRQHandler,Default_Handler
	
	.weak	LPTIM2_IRQHandler
	.thumb_set LPTIM2_IRQHandler,Default_Handler	
	
	.weak	OTG_FS_IRQHandler
	.thumb_set OTG_FS_IRQHandler,Default_Handler	
	
	.weak	DMA2_Channel6_IRQHandler
	.thumb_set DMA2_Channel6_IRQHandler,Default_Handler	
	
	.weak	DMA2_Channel7_IRQHandler
	.thumb_set DMA2_Channel7_IRQHandler,Default_Handler	
	
	.weak	LPUART1_IRQHandler
	.thumb_set LPUART1_IRQHandler,Default_Handler	
	
	.weak	QUADSPI_IRQHandler
	.thumb_set QUADSPI_IRQHandler,Default_Handler	
	
	.weak	I2C3_EV_IRQHandler
	.thumb_set I2C3_EV_IRQHandler,Default_Handler	
	
	.weak	I2C3_ER_IRQHandler
	.thumb_set I2C3_ER_IRQHandler,Default_Handler	
	
	.weak	SAI1_IRQHandler
	.thumb_set SAI1_IRQHandler,Default_Handler
	
	.weak	SAI2_IRQHandler
	.thumb_set SAI2_IRQHandler,Default_Handler
	
	.weak	SWPMI1_IRQHandler
	.thumb_set SWPMI1_IRQHandler,Default_Handler
	
	.weak	TSC_IRQHandler
	.thumb_set TSC_IRQHandler,Default_Handler
	
	.weak	LCD_IRQHandler
	.thumb_set LCD_IRQHandler,Default_Handler
	
	.weak	RNG_IRQHandler
	.thumb_set RNG_IRQHandler,Default_Handler
	
	.weak	FPU_IRQHandler
	.thumb_set FPU_IRQHandler,Default_Handler    
    
    
    
    .end
