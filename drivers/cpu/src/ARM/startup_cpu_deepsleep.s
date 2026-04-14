;/**************************************************************************//**
; * @file     startup_<Device>.s
; * @brief    CMSIS Cortex-M# Core Device Startup File for
; *           Device <Device>
; * @version  V3.10
; * @date     23. November 2012
; *
; * @note
; *
; ******************************************************************************/
;/* Copyright (c) 2012 ARM LIMITED
;
;   All rights reserved.
;   Redistribution and use in source and binary forms, with or without
;   modification, are permitted provided that the following conditions are met:
;   - Redistributions of source code must retain the above copyright
;     notice, this list of conditions and the following disclaimer.
;   - Redistributions in binary form must reproduce the above copyright
;     notice, this list of conditions and the following disclaimer in the
;     documentation and/or other materials provided with the distribution.
;   - Neither the name of ARM nor the names of its contributors may be used
;     to endorse or promote products derived from this software without
;     specific prior written permission.
;   *
;   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
;   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
;   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
;   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
;   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
;   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
;   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
;   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
;   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
;   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
;   POSSIBILITY OF SUCH DAMAGE.
;   ---------------------------------------------------------------------------*/
;/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000a00

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler         ; MPU Fault Handler
                DCD     BusFault_Handler          ; Bus Fault Handler
                DCD     UsageFault_Handler        ; Usage Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     DebugMon_Handler          ; Debug Monitor Handler
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
        		DCD     0
        		DCD     0
                DCD     UART0_IRQHandler
        		DCD     0
                DCD     SPI0_IRQHandler
        	    DCD		0
        		DCD     QDEC_IRQHandler
        		DCD     0
        		DCD		TIMER_IRQHandler
        		DCD		0
        		DCD		0
        		DCD		0
        		DCD     0
        		DCD     RTC_IRQHandler
                DCD     R24G_IRQHandler 
        		DCD     0
        		DCD     GPIO_INT0_IRQHandler
        		DCD		GPIO_INT1_IRQHandler
        		DCD		GPIO_INT2_IRQHandler
        		DCD		GPIO_INT3_IRQHandler
        		DCD		LIB_0_IRQHandler
        		DCD		LIB_1_IRQHandler
        		DCD		LIB_2_IRQHandler
        		DCD		LIB_3_IRQHandler
        		DCD		LIB_4_IRQHandler
        		DCD		LIB_5_IRQHandler
        		DCD		LIB_6_IRQHandler
        		DCD		LIB_7_IRQHandler
        		DCD		LIB_8_IRQHandler
        		DCD		LIB_9_IRQHandler
        		DCD		LIB_10_IRQHandler
        		DCD		LIB_11_IRQHandler
        		DCD		LIB_12_IRQHandler
        		DCD 	LIB_13_IRQHandler
        		DCD 	LIB_22_IRQHandler
				DCD     AUDIO_ADC_IRQHandler
        		DCD 	IRDA_IRQHandler
        		DCD 	LIB_14_IRQHandler
        		DCD 	LIB_15_IRQHandler
                DCD     LIB_16_IRQHandler
                DCD     LIB_17_IRQHandler
                DCD     LIB_18_IRQHandler
                DCD     LIB_19_IRQHandler
                DCD     LIB_20_IRQHandler
__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main
FLAG_NEED_RECOVER		EQU		0x234
ADDR_FLAG_NEED_RECOVER	EQU		0x20000008
ADDR_PC					EQU		0x20000004	
ADDR_SP					EQU		0x20000010					
ADDR_POWER_ON_FLAG		EQU		0x500080f4

CACHE_ADDR_BD				EQU		0x40005040
LIB_22_ADDR_DEVICE_PARA		EQU		0x40009014
LIB_22_ADDR_CACHE_INIT_CMD	EQU		0x40009038
LIB_22_ADDR_IO_CTRL			EQU		0x40009044

BK_CACHE_ADDR_BD				EQU		0x20000014
BK_LIB_22_ADDR_DEVICE_PARA		EQU		0x20000018
BK_LIB_22_ADDR_CACHE_INIT_CMD		EQU		0x2000001C
BK_LIB_22_ADDR_IO_CTRL			EQU		0x20000020
				
				LDR     R7,=ADDR_POWER_ON_FLAG
				LDR     R6,[R7]
				AND     R6,#0x08
				CBZ     R6,NEW_START
				
				LDR		R0,=ADDR_FLAG_NEED_RECOVER
				LDR		R1,=ADDR_SP
				LDR		R2,=ADDR_PC
				
				LDR		R3,[R0];GET FLAG
				CMP		R3,#FLAG_NEED_RECOVER
				ITTT	EQ
				
				LDREQ	R8,=CACHE_ADDR_BD
				LDREQ	R9,=BK_CACHE_ADDR_BD
				LDREQ	R9,[R9]
				STREQ	R9,[R8]
				LDREQ	R8,=LIB_22_ADDR_DEVICE_PARA
				LDREQ	R9,=BK_LIB_22_ADDR_DEVICE_PARA
				LDREQ	R9,[R9]
				STREQ	R9,[R8]
				LDREQ	R8,=LIB_22_ADDR_CACHE_INIT_CMD
				LDREQ	R9,=BK_LIB_22_ADDR_CACHE_INIT_CMD
				LDREQ	R9,[R9]
				STREQ	R9,[R8]
				LDREQ	R8,=LIB_22_ADDR_IO_CTRL
				LDREQ	R9,=BK_LIB_22_ADDR_IO_CTRL
				LDREQ	R9,[R9]
				STREQ	R9,[R8]
				
				LDREQ	SP,[R1]
				LDREQ	R4,[R2]
				BXEQ	R4
NEW_START				
				LDR     R0, =__Vectors
                LDR     R1, =0xE000ED08
                STR     R0, [R1]
                LDR     R0, [R0]
                MOV     SP, R0
				
				LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP

; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler         [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler          [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler        [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler          [WEAK]
                B       .
                ENDP
PendSV_Handler\
                PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler\
                PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC
; ToDo:  Add here the export definition for the device specific external interrupts handler
				EXPORT		UART0_IRQHandler			[WEAK]
				EXPORT		SPI0_IRQHandler				[WEAK]
				EXPORT		QDEC_IRQHandler				[WEAK]
				EXPORT		TIMER_IRQHandler			[WEAK]
				EXPORT		RTC_IRQHandler				[WEAK]
				EXPORT      	R24G_IRQHandler             [WEAK]
				EXPORT		RNG_IRQHandler				[WEAK]
				EXPORT		GPIO_INT0_IRQHandler			[WEAK]
				EXPORT		GPIO_INT1_IRQHandler			[WEAK]
				EXPORT		GPIO_INT2_IRQHandler			[WEAK]
				EXPORT		GPIO_INT3_IRQHandler			[WEAK]
				EXPORT		LIB_0_IRQHandler			[WEAK]
				EXPORT		LIB_1_IRQHandler		[WEAK]
				EXPORT		LIB_2_IRQHandler			[WEAK]
				EXPORT		LIB_3_IRQHandler		[WEAK]
				EXPORT		LIB_4_IRQHandler		[WEAK]
				EXPORT		LIB_5_IRQHandler		[WEAK]
				EXPORT		LIB_6_IRQHandler			[WEAK]
				EXPORT		LIB_7_IRQHandler		[WEAK]
				EXPORT		LIB_8_IRQHandler			[WEAK]
				EXPORT		LIB_9_IRQHandler		[WEAK]
				EXPORT		LIB_10_IRQHandler	[WEAK]
				EXPORT		LIB_11_IRQHandler	[WEAK]
				EXPORT		LIB_12_IRQHandler	[WEAK]
				EXPORT		LIB_13_IRQHandler	[WEAK]
				EXPORT		LIB_22_IRQHandler			[WEAK]
               			EXPORT      	AUDIO_ADC_IRQHandler   		[WEAK]
				EXPORT		IRDA_IRQHandler			[WEAK]
				EXPORT		LIB_14_IRQHandler	[WEAK]
				EXPORT		LIB_15_IRQHandler	[WEAK]	
                EXPORT      LIB_16_IRQHandler                      [WEAK]
                EXPORT      LIB_17_IRQHandler                   [WEAK]
                EXPORT      LIB_18_IRQHandler                        [WEAK]
                EXPORT      LIB_19_IRQHandler                  [WEAK]
                EXPORT      LIB_20_IRQHandler                     [WEAK]                  
; ToDo:  Add here the names for the device specific external interrupts handler
UART0_IRQHandler
SPI0_IRQHandler
QDEC_IRQHandler
TIMER_IRQHandler
RTC_IRQHandler
R24G_IRQHandler
RNG_IRQHandler
GPIO_INT0_IRQHandler
GPIO_INT1_IRQHandler
GPIO_INT2_IRQHandler
GPIO_INT3_IRQHandler
LIB_0_IRQHandler
LIB_1_IRQHandler
LIB_2_IRQHandler
LIB_3_IRQHandler
LIB_4_IRQHandler
LIB_5_IRQHandler
LIB_6_IRQHandler
LIB_7_IRQHandler
LIB_8_IRQHandler
LIB_9_IRQHandler
LIB_10_IRQHandler
LIB_11_IRQHandler
LIB_12_IRQHandler
LIB_13_IRQHandler
LIB_22_IRQHandler
AUDIO_ADC_IRQHandler
IRDA_IRQHandler
LIB_14_IRQHandler
LIB_15_IRQHandler
LIB_16_IRQHandler      
LIB_17_IRQHandler
LIB_18_IRQHandler     
LIB_19_IRQHandler
LIB_20_IRQHandler
                B       .
                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap

__user_initial_stackheap PROC
                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR
                ENDP

                ALIGN

                ENDIF


                END
