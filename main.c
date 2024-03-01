#include "Board_LED.h" // ::Board Support:LED
#include "Board_Joystick.h"
#include "LPC17xx.h" // Device header
#include "Driver_USART.h"
#include "PIN_LPC17xx.h"
#include "LCD_ILI9325.h"
#include "Open1768_LCD.h"
//All needed libraries and components are included

#include <stdio.h>

// SysTick counter
uint32_t msTicks = 0;

// Delay function
void delay(uint32_t x){
    msTicks =0;
    while(x/2>msTicks);

}

// SysTick Handler
void SysTick_Handler(void){
    msTicks++;
}

// Function to send data to UART
void send(char* str){
    while( *str!= '\0'){
        while((LPC_UART0->LSR&32)==0){
        }
        LPC_UART0->THR = *str;
        str++;
    }
    LPC_UART0->THR = '\r';
    LPC_UART0->THR = '\n';
}

int game_tab[32][24] = {	
    {4,4,1,1,1,0,0,0,1,1,0,1,0,0,0,0,1,0,0,0,1,3,3,3}, 
 	{4,4,0,0,0,0,1,0,0,1,0,0,0,1,1,0,1,0,1,1,0,1,3,3}, 
 	{1,0,1,1,1,0,1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,1,0},
 	{1,0,1,0,0,0,1,0,1,1,1,1,1,0,1,0,1,1,0,1,1,0,0,0},
 	{1,0,1,1,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,1},
 	{1,0,0,0,0,1,1,1,1,0,1,0,0,1,1,0,1,0,1,1,1,0,1,0},
 	{3,0,1,0,1,0,1,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,1},
 	{1,1,1,0,1,0,1,0,0,1,0,1,0,1,0,1,1,1,1,0,1,0,0,0},
 	{0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,1,0,1,0},
 	{0,1,1,1,0,0,1,0,1,0,1,1,0,1,1,0,0,1,0,1,1,0,1,0},
 	{0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0},
 	{3,3,1,0,1,1,0,1,1,1,0,0,1,1,1,0,0,0,1,1,0,1,0,1},
 	{3,1,0,0,1,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,0,1,0,0},
 	{1,0,0,1,1,0,1,0,1,0,1,1,0,1,1,0,0,1,0,0,0,1,1,0},
 	{0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,0,1,1,0,0},
 	{0,1,1,1,0,1,1,0,0,1,1,0,1,0,1,0,1,0,0,0,0,0,0,1},
 	{0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,1,0,0,1,0}, 
 	{1,1,1,0,1,0,1,1,1,0,1,1,1,1,1,0,1,0,0,0,0,1,3,0}, 
 	{0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0},
 	{0,1,0,0,1,0,1,0,0,0,1,0,1,0,0,1,1,1,0,0,0,0,1,0},
 	{0,0,1,0,0,0,0,1,1,1,1,0,1,1,0,0,0,0,0,0,1,1,1,0},
 	{1,0,1,1,1,0,0,1,0,0,0,0,0,0,1,0,1,0,0,1,0,0,0,0},
 	{0,0,0,0,1,0,1,1,0,1,1,1,1,0,1,0,1,0,1,0,0,1,1,1},
 	{0,1,1,0,1,0,1,0,0,0,0,3,1,0,1,0,1,0,0,0,1,0,0,3},
 	{0,0,0,0,0,0,0,0,1,1,0,1,0,0,1,0,0,1,0,1,0,0,1,0},
 	{1,0,1,1,0,1,1,1,0,0,0,1,1,0,0,1,0,1,0,1,0,1,1,0},
 	{0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,1,0,0},
 	{0,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,1,0,0,1,0,1},
 	{1,0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0},
 	{0,0,1,1,1,0,1,0,1,0,1,1,1,1,0,1,0,1,0,1,0,1,1,0},
 	{0,1,1,0,0,0,0,0,1,0,1,0,0,0,0,1,0,1,1,1,0,1,0,0},
	{0,0,0,0,1,0,1,0,0,0,1,0,1,1,1,0,0,1,0,0,0,0,0,1}
};
						
// Starting position of the main character and the gold
int man_x = 15;
int man_y = 11;
int gold = 0;

// Main character body to draw
int body[10][10] = {
    {2,2,2,2,2,2,2,0,0,0},
    {2,2,2,2,2,2,2,2,0,0},
    {0,2,2,2,2,2,2,0,2,0},
    {2,2,2,2,2,2,2,0,0,2},
    {0,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2},
    {0,2,2,2,2,2,2,0,0,2},
    {2,2,2,2,2,2,2,0,2,0},
    {0,2,2,2,2,2,2,2,0,0},
    {2,2,2,2,2,2,2,0,0,0}
};

// Function to drawing the main character
void draw_the_man(int mx, int my){
    for (int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            if(body[i][j] ==0){
                lcdWriteReg(ADRX_RAM,my*10 + j);
                lcdWriteReg(ADRY_RAM,mx*10 + i );
                lcdWriteReg(DATA_RAM, 0xEEEE);
            }
            else{
                lcdWriteReg(ADRX_RAM,my*10 + j );
                lcdWriteReg(ADRY_RAM,mx*10+ i );
                lcdWriteReg(DATA_RAM, LCDCyan);
            }
        }
    }
}

// Function to update the game board
void update_game(){
    for(int register x = 0; x < LCD_MAX_Y ; x++){
        for(int register y = 0; y < LCD_MAX_X ; y++){
            int color_indx = game_tab[(int)x/10][(int)y/10];

            if(color_indx == 0){
                lcdWriteReg(ADRX_RAM, y);
                lcdWriteReg(ADRY_RAM, x);
                lcdWriteReg(DATA_RAM, 0xEEEE);
            }
            else if (color_indx == 1){
                lcdWriteReg(ADRX_RAM, y);
                lcdWriteReg(ADRY_RAM, x);
                lcdWriteReg(DATA_RAM, LCDBlack);
            }
            else if (color_indx == 2){
                draw_the_man(man_x, man_y); 
            }
            else if (color_indx == 3){
                gold +=1;
                lcdWriteReg(ADRX_RAM, y);
                lcdWriteReg(ADRY_RAM, x);
                lcdWriteReg(DATA_RAM, LCDYellow);
            }
            else if(color_indx == 4){
                lcdWriteReg(ADRX_RAM, y);
                lcdWriteReg(ADRY_RAM, x);
                lcdWriteReg(DATA_RAM, LCDRed);
            }
            else if(color_indx==5){
                lcdWriteReg(ADRX_RAM, y);
                lcdWriteReg(ADRY_RAM, x);
                lcdWriteReg(DATA_RAM, LCDGreen);
            }
        

        }
    }
    gold = gold/100;
}

// End screen ( Game over )
int end_screen[32][24] = 
						{	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 
							{0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0},
							{0,0,0,1,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0},
							{0,0,0,1,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,1,0,0,0},
							{0,0,0,1,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,1,0,0,0},
							{0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,0,0,1,0,0,0,0},
							{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,0,0},
							{0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0},
							{0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0},
							{0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0},
							{0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,0,0},
							{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
							{0,0,0,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0,0,0}, 
							{0,0,0,1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0}, 
							{0,0,0,1,0,0,0,1,0,0,1,0,0,0,0,0,0,1,1,0,0,0,0,0},
							{0,0,0,1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0},
							{0,0,0,1,0,0,0,0,0,0,1,0,0,1,1,1,1,1,1,1,1,0,0,0},
							{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
							{0,0,0,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0,0,0},
							{0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0},
							{0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0},
							{0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0},
							{0,0,0,1,1,1,1,0,1,1,0,0,0,1,0,0,0,0,0,0,1,0,0,0},
							{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
								
};

// Function to display the end screen
void finish_game(){
	for (int i =0;i<32;i++){
	for (int j =0;j<24;j++){

	game_tab[i][j]=end_screen[i][j];
	}
	}
	
	update_game();
}

// Function to update the move afer using Joystick
void update_move(int new_man_x,int new_man_y){
    if (game_tab[new_man_x][new_man_y]==3){
        gold--;

        if(gold<1){
            game_tab[0][1] = 5;
            game_tab[0][0] = 5;
            game_tab[1][0] = 5;
            game_tab[1][1] = 5;

            for(int x = 0;x<20;x++){
                for(int y = 0;y<20;y++){
                    lcdWriteReg(ADRX_RAM, y);
                    lcdWriteReg(ADRY_RAM, x);
                    lcdWriteReg(DATA_RAM, LCDGreen);
                }
            }
        }
        
    }
    if (game_tab[new_man_x][new_man_y]==5){
        finish_game();
        while(1);

    }

    for(int x = man_x*10;x<(man_x+1)*10;x++){
        for(int y = man_y*10;y<(man_y+1)*10;y++){
            lcdWriteReg(ADRX_RAM, y);
            lcdWriteReg(ADRY_RAM, x);
            lcdWriteReg(DATA_RAM, 0xEEEE);
        }
    }
    draw_the_man(new_man_x, new_man_y);
}

// Function to check if the move is possible
bool check_if_move_possible(int new_man_x,int new_man_y){
    if(new_man_x>31 || new_man_y>23 || new_man_x<0 || new_man_y<0){
        return false;
    }
    if (game_tab[new_man_x][new_man_y]==1 || game_tab[new_man_x][new_man_y]==4 ){
        return false;
    }
    return true;
}




//configuration UART, LCD display and Joystick components
//Initialazing game board on LCD and necessary variables
//Main loop of the game with joystick control
int main(){
    PIN_Configure(0,2,1,0,0);
    PIN_Configure(0,3,1,2,0);
    LPC_UART0->LCR = 0x3 | (1 << 7);
    LPC_UART0->DLL = 27;
    LPC_UART0->DLM = 0;
    LPC_UART0->LCR = 0X3;
    SysTick_Config(SystemCoreClock/10);

    lcdConfiguration();
    init_ILI9325();

    update_game();
    Joystick_Initialize();

    char cmd;
    game_tab[man_x][man_y] = 2;

    while(1){
        int state = Joystick_GetState();

        if(state & JOYSTICK_UP){
            if (check_if_move_possible(man_x,man_y+1)){
                game_tab[man_x][man_y] = 0;
                update_move(man_x,man_y+1);
                man_y +=1;
                game_tab[man_x][man_y] = 2;
            }
        send("up");
        }
        else if(state & JOYSTICK_DOWN){
            if (check_if_move_possible(man_x,man_y-1)){
                game_tab[man_x][man_y] = 0;
                update_move(man_x,man_y-1);
                man_y -=1;
                game_tab[man_x][man_y] = 2;
            }
            send("down");
        }
        else if(state & JOYSTICK_RIGHT){
            if (check_if_move_possible(man_x+1,man_y)){
                game_tab[man_x][man_y] = 0;
                update_move(man_x+1,man_y);
                man_x +=1;
                game_tab[man_x][man_y] = 2;
            }
            send("right");
        }
        else if(state & JOYSTICK_LEFT){
            if (check_if_move_possible(man_x-1,man_y)){

                game_tab[man_x][man_y] = 0;
                update_move(man_x-1,man_y);
                man_x -=1;
                game_tab[man_x][man_y] = 2;
            }
                send("left");
        }
        else if(state & JOYSTICK_CENTER){
            send("center");
        }
        else {
            send("else");
        }

        char txt[20];
        sprintf(txt, "gold = %d", gold);
        send(txt);
        delay(5);
    }

    return 0;
}