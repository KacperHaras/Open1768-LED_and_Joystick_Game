# Open1768 LED and Joystick Game

This project features a game designed for the Open1768 board with an ARM Cortex-M3 processor. It incorporates LED indicators, joystick input, LCD graphical output, and UART communication.

## Features

- **LED Display**: Use of onboard LEDs for status indication.
- **Joystick Control**: Maneuvering through the game using the joystick.
- **LCD Output**: Gameplay visuals on an ILI9325 LCD.
- **UART Serial Communication**: Transmits game state and debug messages.

## Gameplay

Players navigate a character through a maze on the LCD, collecting gold. The grid is a 32x24 field with different elements represented by numeric values. The game concludes when all gold is collected and player get into exit.

## Instructions

- **Movement**: The joystick directs the character's movement.
- **Objective**: Collect all gold pieces on the grid.
- **Avoidance**: Stay clear of walls and obstacles.
- **End Condition**: Collect all gold or encounter an obstacle.

## Hardware Requirements

- **Open1768 Board**: Set up with power, display, and input devices.
- **LCD Display**: Connect to an ILI9325-compatible LCD.
- **Joystick**: Integrated on the board for navigation.


## Software Setup

1. **Compilation**: Use an ARM Cortex-M3 compatible compiler to build the code.
2. **Flashing**: Upload the compiled binary to the Open1768 board.
3. **Monitoring**: Open a serial connection to observe UART communication.

## Technical Implementation

- **Timing**: Utilizes the SysTick handler for millisecond timing.
- **Game Board Representation**: A 2D array tracks the grid state.
- **Rendering**: Functions draw the character and update the display.
- **Communication**: UART functions send out game status messages.

## Contribution Guidelines

To contribute to this project, you can fork the repository and propose changes through pull requests. For significant modifications, please open an issue to discuss the proposed changes.

---