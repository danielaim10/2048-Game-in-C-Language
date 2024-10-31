# 2048-Game-in-C-Language
# Game Project

**Implementation Time:** 7 days (approximately 5-6 hours per day)

This game is designed to function and display uniformly in a terminal with the dimensions 115x40.

## Requirements

### Requirement 1: Main Menu
The menu is displayed at the start of the game, enclosed in a box that is approximately centered. You can navigate the menu using the up and down arrow keys. To select an option, press the ENTER key. Once the game starts, you can press the Q key to return to the menu. The option to RESUME allows you to continue the game from where you left off. If you try to press RESUME without an active game, a message stating "NO PREVIOUS GAME EXISTS!" will be displayed. The QUIT option will exit the terminal.

### Requirement 2: New Game
When the NEW GAME option is selected, a new game begins. The game board will be displayed within a white box (a more aesthetically pleasing design was not implemented). Along with the game board, two random numbers from the set {2, 4} will be generated. The numbers on the board are initialized to 0, but they will not be visible if they hold this value. On the right side of the screen, a control panel displays the score (highlighted in yellow), the commands available for gameplay, and the current time (in magenta), formatted as year-month-day hour:minute:second.

### Requirement 3: Gameplay Mechanics
The game meets all the requirements, allowing players to combine numbers in the direction indicated by the arrow key presses.

### Requirement 5: Game Over Conditions
Once no moves are left on the game board, the program will display the message "Game over!" on the left side of the board, indicating that the game has ended. In a fortunate scenario, if any element on the board reaches the value of 2048, the message "You won!" will be displayed. To start a new game, press the Q key, which will take you back to the menu, from where you can create a new game.

### Bonus Features
- Colored borders for the game board
- Colored score display
- Colored time display
- Highlighted win and loss messages

## Acknowledgments
Thank you for your attention! I hope you enjoy playing the game. It has been a rewarding experience to develop, and I look forward to any feedback you may have.
