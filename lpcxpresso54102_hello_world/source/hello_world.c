/*
 * The Clear BSD License
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "pin_mux.h"
#include <stdbool.h>
#include "fsl_utick.h"
#include "stdlib.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define FIELD_WIDTH 12
#define FIELD_HEIGHT 22

char field[FIELD_HEIGHT][FIELD_WIDTH];
char dispBuffer[FIELD_HEIGHT][FIELD_WIDTH]={0};

enum{
	MINO_TYPE_I,
	MINO_TYPE_O,
	MINO_TYPE_S,
	MINO_TYPE_Z,
	MINO_TYPE_J,
	MINO_TYPE_L,
	MINO_TYPE_T,
	MINO_TYPE_MAX
};

enum{
	MINO_ANGLE_0,
	MINO_ANGLE_90,
	MINO_ANGLE_180,
	MINO_ANGLE_270,
	MINO_ANGLE_MAX
};
#define MINO_WIDTH 4
#define MINO_HEIGHT 4

char minoShapes[MINO_TYPE_MAX][MINO_ANGLE_MAX][MINO_WIDTH][MINO_HEIGHT] = {
		{//MINO_TYPE_I,
			//MINO_ANGLE_0,
			{
					{0, 1, 0, 0},
					{0, 1, 0, 0},
					{0, 1, 0, 0},
					{0, 1, 0, 0},
			},
				//MINO_ANGLE_90,
			{
					{0, 0, 0, 0},
					{0, 0, 0, 0},
					{1, 1, 1, 1},
					{0, 0, 0, 0},
			},
				//MINO_ANGLE_180,
			{
					{0, 0, 1 ,0},
					{0, 0, 1, 0},
					{0, 0, 1, 0},
					{0, 0, 1, 0},
			},
				//MINO_ANGLE_270,
			{
					{0, 0 ,0 ,0},
					{1, 1 ,1 ,1},
					{0, 0 ,0 ,0},
					{0, 0 ,0 ,0},
			},
		},
		//MINO_TYPE_O,
		{
			//MINO_ANGLE_0,
			{
					{0, 0, 0 ,0},
					{0, 1, 1 ,0},
					{0, 1, 1 ,0},
					{0, 0, 0 ,0},
			},
			//MINO_ANGLE_90,
			{
					{0, 0, 0, 0},
					{0, 1, 1, 0},
					{0, 1, 1, 0},
					{0, 0, 0 ,0},
			},
			//MINO_ANGLE_180,
			{
					{0, 0, 0, 0},
					{0, 1, 1, 0},
					{0, 1, 1, 0},
					{0, 0, 0, 0},
			},
			//MINO_ANGLE_270,
			{
					{0, 0, 0, 0},
					{0, 1, 1, 0},
					{0, 1 ,1, 0},
					{0, 0, 0, 0},
			},
		},
		//MINO_TYPE_S,
		{
			//MINO_ANGLE_0,
			{
					{0, 0, 0, 0},
					{0, 1, 1, 0},
					{1, 1, 0, 0},
					{0, 0, 0, 0},
			},
			//MINO_ANGLE_90,
			{
					{0, 1, 0, 0},
					{0, 1, 1, 0},
					{0, 0, 1, 0},
					{0, 0, 0, 0},
			},
			//MINO_ANGLE_180,
			{
					{0, 0, 0 ,0},
					{0, 0, 1, 1},
					{0, 1, 1 ,0},
					{0, 0, 0 ,0},
			},
			//MINO_ANGLE_270,
			{
					{0, 0, 0, 0},
					{0, 1, 0, 0},
					{0, 1, 1, 0},
					{0, 0, 1, 0},
			},
		},
		//MINO_TYPE_Z,
		{
			//MINO_ANGLE_0,
			{
					{0, 0, 0, 0},
					{1, 1, 0, 0},
					{0, 1, 1, 0},
					{0, 0, 0, 0},
			},
			//MINO_ANGLE_90,
			{
					{0, 0, 0, 0},
					{0, 0, 1, 0},
					{0, 1, 1, 0},
					{0, 1, 0, 0},
			},
			//MINO_ANGLE_180,
			{
					{0, 0, 0, 0},
					{0, 1, 1, 0},
					{0, 0, 1, 1},
					{0, 0, 0, 0},
			},
			//MINO_ANGLE_270,
			{
					{0, 0, 1, 0},
					{0, 1, 1, 0},
					{0, 1, 0, 0},
					{0, 0, 0, 0},
			},
		},
		//MINO_TYPE_J,
		{
			//MINO_ANGLE_0,
			{
					{0, 0, 1, 0},
					{0, 0, 1, 0},
					{0, 1, 1, 0},
					{0, 0, 0, 0},
			},
			//MINO_ANGLE_90,
			{
					{0, 0, 0, 0},
					{1, 1, 1, 0},
					{0, 0, 1, 0},
					{0, 0, 0, 0},
			},
			//MINO_ANGLE_180,
			{
					{0, 0, 0, 0},
					{0, 1, 1, 0},
					{0, 1, 0, 0},
					{0, 1, 0, 0},
			},
			//MINO_ANGLE_270,
			{
					{0, 0, 0, 0},
					{0, 1, 0, 0},
					{0, 1, 1, 1},
					{0, 0, 0, 0},
			},
		},
		//MINO_TYPE_L,
		{
			//MINO_ANGLE_0,
			{
					{0, 1, 0, 0},
					{0, 1, 0, 0},
					{0, 1, 1, 0},
					{0, 0, 0, 0},
			},
			//MINO_ANGLE_90,
			{
					{0, 0, 0, 0},
				    {0, 0, 1, 0},
					{1, 1, 1, 0},
					{0, 0, 0, 0},
	        },
			//MINO_ANGLE_180,
			{
					{0, 0, 0, 0},
					{0, 1, 1, 0},
					{0, 0, 1, 0},
					{0, 0, 1, 0},
		    },
			//MINO_ANGLE_270,
			{
					{0, 0, 0, 0},
					{0, 1, 1, 1},
					{0, 1, 0, 0},
					{0, 0, 0, 0},
            },
		},
		//MINO_TYPE_T,
		{
			//MINO_ANGLE_0,
			{
					{0, 0, 0, 0},
					{1, 1, 1, 0},
					{0, 1, 0, 0},
					{0, 0, 0, 0},
			},
			//MINO_ANGLE_90,
			{
					{0, 0, 0, 0},
					{0, 1, 0, 0},
					{0, 1, 1, 0},
					{0, 1, 0, 0},
			},
			//MINO_ANGLE_180,
			{
					{0, 0, 0, 0},
					{0, 0, 1, 0},
					{0, 1, 1, 1},
					{0, 0, 0, 0},
			},
			//MINO_ANGLE_270,
			{
					{0, 0, 1, 0},
					{0, 1, 1, 0},
					{0, 0, 1, 0},
					{0, 0, 0, 0},
			},
		},

};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
uint32_t cnt=0;
bool utickIntFlag;
int minoX=5, minoY=1;
uint32_t minoType=0, minoAngle=0;
char ch;

void cb_utick(){
	UTICK_ClearStatusFlags(UTICK0);
	utickIntFlag=true;
}

void display(){
	utickIntFlag = false;
	/* Screen clear  */
	//PRINTF("\033[2J");   	//Escape Sequence Clear screen
	//system("cls");
	PRINTF("\033[1;1f");	//Cursor moves to (1,1)
	memcpy(dispBuffer, field, sizeof(field));
	for (int y =0; y< MINO_HEIGHT; y++)
		for(int x =0; x < MINO_WIDTH; x++)
			dispBuffer[minoY +y][minoX+x] |= minoShapes[minoType][minoAngle][y][x];

	/* Drawing */
	for (int y =0; y < FIELD_HEIGHT; y++){
		for(int x=0; x<FIELD_WIDTH; x++){
			//if (field[y][x]){
		    if (dispBuffer[y][x]){
				PRINTF("◽");
		    }else{
				PRINTF("  ");
		    }
		}
		PRINTF("\n\r");
	}
}

bool isHit(uint32_t _minoX, uint32_t _minoY,uint32_t _minoType, uint32_t _minoAngle){
	for(uint32_t y=0; y < MINO_HEIGHT; y++){
		for(uint32_t x=0; x <MINO_WIDTH; x++){
			if(minoShapes[_minoType][_minoAngle][y][x]&&field[_minoY +y][_minoX +x])
				return true;
		}
	}
	return false;
}

void resetMino(){
	minoX = 5, minoY =0;

	minoType = rand() % MINO_TYPE_MAX;
	minoAngle = rand() % MINO_ANGLE_MAX;
}

int main(void)
{
    char ch;
    memset(field, 0 , sizeof field);
    uint32_t seed;

    /* Init board hardware. */
    /* attach 12 MHz clock to USART0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitPins();
    BOARD_BootClockPLL96M(); /* Rev B device can only support max core frequency to 96Mhz.
                                Rev C device can support 100Mhz,use BOARD_BootClockPLL100M() to boot core to 100Mhz.
                                DEVICE_ID1 register in SYSCON shows the device version.
                                More details please refer to user manual and errata. */
    BOARD_InitDebugConsole();

    srand(cnt); //As time can not be obtained, seed is static.
    utickIntFlag = false;
    UTICK_Init(UTICK0);
    UTICK_SetTick(UTICK0, kUTICK_Repeat, 600000, cb_utick);
    resetMino();

    while(1){

    	if (USART0->STAT&USART_STAT_RXRDY_MASK){
    		switch(ch=USART0->RXDAT&USART_RXDAT_DATA_MASK){
    			case 'a':
    				if(!isHit(
    						minoX-1,	//uint32_t _minoX,
							minoY,		//uint32_t _minoY,
							minoType,	//uint32_t _minoType,
							minoAngle	//uint32_t _minoAngle)
							))
    					minoX--;
    				break;
				case 'd':
					if(!isHit(
							minoX+1,	//uint32_t _minoX,
							minoY,		//uint32_t _minoY,
							minoType,	//uint32_t _minoType,
							minoAngle	//uint32_t _minoAngle)
							))
						minoX++;
					break;
				case 's':
					if(!isHit(
							minoX,		//uint32_t _minoX,
							minoY+1,	//uint32_t _minoY,
							minoType,	//uint32_t _minoType,
							minoAngle	//uint32_t _minoAngle)
							)){
					minoY++; break;
					}
 				case 0x20:
 					if(!isHit(
							minoX,		//uint32_t _minoX,
							minoY,		//uint32_t _minoY,
							minoType,	//uint32_t _minoType,
							(minoAngle+1) % MINO_ANGLE_MAX//uint32_t _minoAngle)
							))
 					minoAngle = (minoAngle+1) % MINO_ANGLE_MAX;
 					break;
				default: break;
			}
			display();
    	}

    	if(utickIntFlag){
			for (int y=0; y< FIELD_HEIGHT; y++){
				field[y][0] = field[y][FIELD_WIDTH-1] = 1;
			}
			for(int x=0; x < FIELD_WIDTH; x++)
				field[FIELD_HEIGHT-1][x]=1;
			if(isHit(
				minoX,			//uint32_t _minoX,
				minoY+1,		//uint32_t _minoY,
				minoType,		//uint32_t _minoType,
				minoAngle		//uint32_t _minoAngle)
				)){
				for(uint32_t y=0; y < MINO_HEIGHT; y++){
					for(uint32_t x=0; x <MINO_WIDTH; x++){
							field[minoY + y][minoX + x] |= minoShapes[minoType][minoAngle][y][x];
					}
				}
				for (uint8_t y=0; y<FIELD_HEIGHT-1; y++){
					bool lineFill = true;
					for(uint8_t x=1; x<FIELD_WIDTH-1; x++){
						if(!field[y][x])
							lineFill=false;
					}
					if (lineFill){
						//for (uint8_t x=1; x<FIELD_WIDTH-1; x++)
						//	field[y][x]=0;
						for (uint8_t j = y; 0<j; j--)
							memcpy(field[j], field[j-1], FIELD_WIDTH);
					}
				}
				resetMino();

			}else{
				minoY++;
			}
			display();
    	}
    }

}
