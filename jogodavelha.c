#include <stdio.h>//Default
#include <stdlib.h>//Default
#include <string.h>//For treating strings
#include <locale.h>//For localized text
#include <time.h>//For time operations, used by delay function
#include <curses.h>//For advanced screen out

char *title = "Jogo da Velha";//Game title, as an string for easier use when printing on screen center

short int row,col;//Screen size

//Players names and scores
char player1[100];//Player1 name
short int player1Score = 0;//Player1 score
char player2[100];//Player2 name
short int player2Score = 0;//Player2 score

//Initialize game table
char game[3][3];//3 x 3 table

/* Auxiliar third party delay function - http://c-for-dummies.com/blog/?p=69 */
void delay(int milliseconds)
{
    long pause;
    clock_t now,then;

    pause = milliseconds*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause )
        now = clock();
}

//Print centered function
void printCentered(int row, int totalcols, char *string2print){
	move(row, 0);
	clrtoeol();
	mvprintw(row, (totalcols-strlen(string2print))/2,"%s\n", string2print);
}

//Splash with ASCII art
void splashScreen(){
	curs_set(0);
	mvprintw(3, 0, "      #                                      #     #                            ");
	mvprintw(4, 0, "      #  ####   ####   ####   #####    ##    #     # ###### #      #    #   ##  ");
	mvprintw(5, 0, "      # #    # #    # #    #  #    #  #  #   #     # #      #      #    #  #  # ");
	mvprintw(6, 0, "      # #    # #      #    #  #    # #    #  #     # #####  #      ###### #    #");
	mvprintw(7, 0, "#     # #    # #  ### #    #  #    # ######   #   #  #      #      #    # ######");
	mvprintw(8, 0, "#     # #    # #    # #    #  #    # #    #    # #   #      #      #    # #    #");
	mvprintw(9, 0, " #####   ####   ####   ####   #####  #    #     #    ###### ###### #    # #    #");
	refresh();
	delay(2000);
	curs_set(1);
}

//Initialize game table
void initializeGameTable(){
	game[0][0] = '1'; game[0][1] = '2'; game[0][2] = '3';
	game[1][0] = '4'; game[1][1] = '5'; game[1][2] = '6';
	game[2][0] = '7'; game[2][1] = '8'; game[2][2] = '9';
}

/* Function to show the header, including title and players names */
void refreshScores(){
	//Check if players names was informed
	if(strcmp(player1, "")){
		//Print a divisor line
		for(short int _clrscr_i = 0; _clrscr_i < col; _clrscr_i++){
			mvprintw(2,_clrscr_i,"-");
		}
		//Print the player names
		attron(COLOR_PAIR(2));//Set player 1 color
		mvprintw(3, 0, "P1: %s, com X - %d vitórias", player1, player1Score);//Show player1 data
		attron(COLOR_PAIR(3));//Set player 2 color
		if(!strcmp(player2, "")){
			mvprintw(3, col/2, "PC, com O - %d vitórias", player2Score);//Show PC data
		}else{
			mvprintw(3, col/2, "P2: %s, com O - %d vitórias", player2, player2Score);//Show player2 data
		}
		attron(COLOR_PAIR(1));//Set default color
		//Break a line
		printw("\n");
	}
}
void showHeader(){
	//Clear screen
	wclear(stdscr);
	//Declare i for counter
	short int _clrscr_i;
	//Print the first divisor line
	for(_clrscr_i = 0; _clrscr_i < col; _clrscr_i++){
		mvprintw(0,_clrscr_i,"-");
	}
	//Print the # above title
	for(_clrscr_i = 0; _clrscr_i < ((col-strlen(title))/2)-1; _clrscr_i++){
		mvprintw(1,_clrscr_i,"#");
	}
	//Print title in the center
	mvprintw(1, (col-strlen(title))/2,"%s\n", title);
	//Print the # after title
	for(_clrscr_i = _clrscr_i + strlen(title) + 2; _clrscr_i < col; _clrscr_i++){
		mvprintw(1, _clrscr_i, "#");
	}

	//Show scores
	refreshScores();

	//Print a divisor line
	for(_clrscr_i = 0;_clrscr_i < col; _clrscr_i++){
		printw("-");
	}
}

//Request names from the players
bool requestedNames = 0;
void requestPlayerNames(){
	if(!requestedNames){
		//While the player1 name is empty, ask for it
		while(!strcmp(player1,"")){
			printw("Digite o nome do primeiro jogador: \n");
			attron(COLOR_PAIR(2));
			getstr(player1);
			attron(COLOR_PAIR(1));
		}
		//While the player2 name is empty, ask for it
		//while(!strcmp(player2,"")){
			printw("Digite o nome do segundo jogador, ou deixe em branco e pressione enter para jogar contra o PC: \n");
			attron(COLOR_PAIR(3));
			getstr(player2);
			attron(COLOR_PAIR(1));
		//}
		requestedNames = 1;
	}
}

//Set color for use in refreshGame()
void setColorRefreshGame(char symbol){
	if(symbol == 'X'){
		attron(COLOR_PAIR(2));
	} else if(symbol == 'O'){
		attron(COLOR_PAIR(3));
	}else{
		attron(COLOR_PAIR(1));
	}
}

//Refresh the game table
void refreshGame(){
	//Here we'll check if it's from player 1, or 2, or none, and colour respectively, and then print X, O, or the numbers and table format
	setColorRefreshGame(game[0][0]);
	mvprintw(6, ((col-9)/2), "%c", game[0][0]);
	attron(COLOR_PAIR(1));
	mvprintw(6, ((col-9)/2)+1, " | ");
	setColorRefreshGame(game[0][1]);
	mvprintw(6, ((col-9)/2)+4, "%c", game[0][1]);
	attron(COLOR_PAIR(1));
	mvprintw(6, ((col-9)/2)+5, " | ");
	setColorRefreshGame(game[0][2]);
	mvprintw(6, ((col-9)/2)+8, "%c", game[0][2]);
	attron(COLOR_PAIR(1));
	mvprintw(7, (col-9)/2, "--+---+--\n");
	setColorRefreshGame(game[1][0]);
	mvprintw(8, ((col-9)/2), "%c", game[1][0]);
	attron(COLOR_PAIR(1));
	mvprintw(8, ((col-9)/2)+1, " | ");
	setColorRefreshGame(game[1][1]);
	mvprintw(8, ((col-9)/2)+4, "%c", game[1][1]);
	attron(COLOR_PAIR(1));
	mvprintw(8, ((col-9)/2)+5, " | ");
	setColorRefreshGame(game[1][2]);
	mvprintw(8, ((col-9)/2)+8, "%c", game[1][2]);
	attron(COLOR_PAIR(1));
	mvprintw(9, (col-9)/2, "--+---+--\n");
	setColorRefreshGame(game[2][0]);
	mvprintw(10, ((col-9)/2), "%c", game[2][0]);
	attron(COLOR_PAIR(1));
	mvprintw(10, ((col-9)/2)+1, " | ");
	setColorRefreshGame(game[2][1]);
	mvprintw(10, ((col-9)/2)+4, "%c", game[2][1]);
	attron(COLOR_PAIR(1));
	mvprintw(10, ((col-9)/2)+5, " | ");
	setColorRefreshGame(game[2][2]);
	mvprintw(10, ((col-9)/2)+8, "%c", game[2][2]);
	attron(COLOR_PAIR(1));

	//Break a line
	printw("\n");
}

//Check if someone win the game
int checkWin(){
	bool p1win = 0;
	bool p2win = 0;

	//Check X axis
	for(short int i=0; i<3; i++){
		if(game[i][0] == game[i][1] && game[i][1] == game[i][2] && game[i][2] == 'X')
			p1win = 1;
		if(game[i][0] == game[i][1] && game[i][1] == game[i][2] && game[i][2] == 'O')
			p2win = 1;
	}
	//Check Y axis
	for(short int i=0; i<3; i++){
		if(game[0][i] == game[1][i] && game[1][i] == game[2][i] && game[2][i] == 'X')
			p1win = 1;
		if(game[0][i] == game[1][i] && game[1][i] == game[2][i] && game[2][i] == 'O')
			p2win = 1;
	}
	//Check diagonal 1
	if(game[0][0] == game[1][1] && game[1][1] == game[2][2] && game[2][2] == 'X')
		p1win = 1;
	if(game[0][0] == game[1][1] && game[1][1] == game[2][2] && game[2][2] == 'O')
		p2win = 1;
	//Check diagonal 2
	if(game[0][2] == game[1][1] && game[1][1] == game[2][0] && game[2][0] == 'X')
		p1win = 1;
	if(game[0][2] == game[1][1] && game[1][1] == game[2][0] && game[2][0] == 'O')
		p2win = 1;

	//Check and return the winner, or 0 if the game is still running
	if(p1win){
		return 1;
	}else if(p2win){
		return 2;
	}else{
		return 0;
	}
}

//Request player match
short int actualPlayer = 1;
void requestMatch(){
	short int choosenNumberValid = 0;
	char choosenNumber;
	char playerSymbol;
	while(!choosenNumberValid){
		//Clear line 13
		move(13,0);
		clrtoeol();
		//Ask user for the number
		if(actualPlayer == 1){
			attron(COLOR_PAIR(2));//Set player 1 color
			mvprintw(13, 0, "%s", player1);//Print player name
			attron(COLOR_PAIR(1));//Set default color
			printw(", sua vez, escolha um número e pressione no teclado.");
			playerSymbol = 'X';
		}
		//Ask user for the number
		if(actualPlayer == 2 && !strcmp(player2, "")){
			attron(COLOR_PAIR(3));
			mvprintw(13, 0, "%s", player2);
			attron(COLOR_PAIR(1));
			printw(", sua vez, escolha um número e pressione no teclado.");
			playerSymbol = 'O';
		}

		//Get and validate the number if it's user, or use a little IA if it's PC
		if(actualPlayer == 1){
			choosenNumber = getch();
		}else{
			if(strcmp(player2, "")){
				choosenNumber = getch();
			}else{
				//Use A.I. to get a number to play
				//Check if there is a place that we can win the game, or the player can win
				if(((game[0][0] == 'X' && game[0][1] == 'X') || (game[0][0] == 'O' && game[0][1] == 'O')) && game[0][2] == '3'){
					choosenNumber = '3';
				}else if(((game[0][1] == 'X' && game[0][2] == 'X') || (game[0][1] == 'O' && game[0][2] == 'O')) && game[0][0] == '1'){
					choosenNumber = '1';
				}else if(((game[1][0] == 'X' && game[1][1] == 'X') || (game[1][0] == 'O' && game[1][1] == 'O')) && game[1][2] == '6'){
					choosenNumber = '6';
				}else if(((game[1][1] == 'X' && game[1][2] == 'X') || (game[1][1] == 'O' && game[1][2] == 'O')) && game[1][0] == '4'){
					choosenNumber = '4';
				}else if(((game[2][0] == 'X' && game[2][1] == 'X') || (game[2][0] == 'O' && game[2][1] == 'O')) && game[2][2] == '9'){
					choosenNumber = '9';
				}else if(((game[2][1] == 'X' && game[2][2] == 'X') || (game[2][1] == 'O' && game[2][2] == 'O')) && game[2][0] == '7'){
					choosenNumber = '7';
				}else if(((game[0][0] == 'X' && game[1][0] == 'X') || (game[0][0] == 'O' && game[1][0] == 'O')) && game[2][0] == '7'){
					choosenNumber = '7';
				}else if(((game[1][0] == 'X' && game[2][0] == 'X') || (game[1][0] == 'O' && game[2][0] == 'O')) && game[0][0] == '1'){
					choosenNumber = '1';
				}else if(((game[0][1] == 'X' && game[1][1] == 'X') || (game[0][1] == 'O' && game[1][1] == 'O')) && game[2][1] == '8'){
					choosenNumber = '8';
				}else if(((game[1][1] == 'X' && game[2][1] == 'X') || (game[1][1] == 'O' && game[2][1] == 'O')) && game[0][1] == '2'){
					choosenNumber = '2';
				}else if(((game[0][2] == 'X' && game[1][2] == 'X') || (game[0][2] == 'O' && game[1][2] == 'O')) && game[2][2] == '9'){
					choosenNumber = '9';
				}else if(((game[1][2] == 'X' && game[2][2] == 'X') || (game[1][2] == 'O' && game[2][2] == 'O')) && game[0][2] == '3'){
					choosenNumber = '3';
				}else if(((game[0][0] == 'X' && game[1][1] == 'X') || (game[0][0] == 'O' && game[1][1] == 'O')) && game[2][2] == '9'){
					choosenNumber = '9';
				}else if(((game[1][1] == 'X' && game[2][2] == 'X') || (game[1][1] == 'O' && game[2][2] == 'O')) && game[0][0] == '1'){
					choosenNumber = '1';
				}else if(((game[0][2] == 'X' && game[1][1] == 'X') || (game[0][2] == 'O' && game[1][1] == 'O')) && game[2][0] == '7'){
					choosenNumber = '7';
				}else if(((game[1][1] == 'X' && game[2][0] == 'X') || (game[1][1] == 'O' && game[2][0] == 'O')) && game[0][2] == '3'){
					choosenNumber = '3';
				}else{
					//Check if there is some blank space between two filled
					if(((game[0][0] == 'X' && game[0][2] == 'X') || (game[0][0] == 'O' && game[0][2] == 'O')) && game[0][1] == '2'){
						choosenNumber = '2';
					}else if(((game[1][0] == 'X' && game[1][2] == 'X') || (game[1][0] == 'O' && game[1][2] == 'O')) && game[1][1] == '5'){
						choosenNumber = '5';
					}else if(((game[2][0] == 'X' && game[2][2] == 'X') || (game[2][0] == 'O' && game[2][2] == 'O')) && game[2][1] == '8'){
						choosenNumber = '8';
					}else if(((game[0][0] == 'X' && game[2][0] == 'X') || (game[0][0] == 'O' && game[2][0] == 'O')) && game[1][0] == '4'){
						choosenNumber = '4';
					}else if(((game[0][1] == 'X' && game[2][1] == 'X') || (game[0][1] == 'O' && game[2][1] == 'O')) && game[1][1] == '5'){
						choosenNumber = '5';
					}else if(((game[0][2] == 'X' && game[2][2] == 'X') || (game[0][2] == 'O' && game[2][2] == 'O')) && game[1][2] == '6'){
						choosenNumber = '6';
					}else{
						//If there's no blank spaces on centers...
						//Select by priority
						if(game[1][1] == '5'){
							choosenNumber = '5';
						}else if(game[0][0] == '1'){
							choosenNumber = '1';
						}else if(game[2][2] == '9'){
							choosenNumber = '9';
						}else if(game[0][2] == '3'){
							choosenNumber = '3';
						}else if(game[2][0] == '7'){
							choosenNumber = '7';
						}else if(game[0][1] == '2'){
							choosenNumber = '2';
						}else if(game[1][0] == '4'){
							choosenNumber = '4';
						}else if(game[1][2] == '6'){
							choosenNumber = '6';
						}else if(game[2][1] == '8'){
							choosenNumber = '8';
						}
					}
				}
				mvprintw(0, 0, "PC: %c", choosenNumber);
				refresh();
			}
		}
		
		switch(choosenNumber){
			case '1':
				if(game[0][0] == choosenNumber)
					choosenNumberValid = 1;
			break;
			case '2':
				if(game[0][1] == choosenNumber)
					choosenNumberValid = 1;
			break;
			case '3':
				if(game[0][2] == choosenNumber)
					choosenNumberValid = 1;
			break;
			case '4':
				if(game[1][0] == choosenNumber)
					choosenNumberValid = 1;
			break;
			case '5':
				if(game[1][1] == choosenNumber)
					choosenNumberValid = 1;
			break;
			case '6':
				if(game[1][2] == choosenNumber)
					choosenNumberValid = 1;
			break;
			case '7':
				if(game[2][0] == choosenNumber)
					choosenNumberValid = 1;
			break;
			case '8':
				if(game[2][1] == choosenNumber)
					choosenNumberValid = 1;
			break;
			case '9':
				if(game[2][2] == choosenNumber)
					choosenNumberValid = 1;
			break;
			default:
				choosenNumberValid = 0;
			break;
		}
	}

	//Cast choosen number to int
	choosenNumberValid = choosenNumber - '0';

	//Fill the game table
	if(choosenNumberValid < 4){
		game[0][choosenNumberValid-1] = playerSymbol;
	}
	if(choosenNumberValid >= 4 && choosenNumberValid < 7){
		game[1][choosenNumberValid-4] = playerSymbol;
	}
	if(choosenNumberValid >= 7){
		game[2][choosenNumberValid-7] = playerSymbol;
	}

	//Go to next player
	if (actualPlayer == 1){
		actualPlayer = 2;
	}else{
		actualPlayer = 1;
	}
}

int main(){
	setlocale(LC_ALL, "ptb");//Set pt-br locale
	initscr();//Initialize cursor mode
	start_color();//Initialize color mode
	getmaxyx(stdscr,row,col);//Get screen dimensions
	attron(A_BOLD);

	//Configure color pairs
	init_pair(1, COLOR_WHITE, COLOR_BLACK);//Our default
	init_pair(2, COLOR_BLUE, COLOR_BLACK);//For Player 1
	init_pair(3, COLOR_RED, COLOR_BLACK);//For Player 2
	//Set default color pair
	wbkgd(stdscr, COLOR_PAIR(1));
	//Refresh screen to apply
	refresh();

	//Splash screen
	splashScreen();

	//wclear(stdscr);

	//Ready!

	char playAgain = 's';//Answer to the play again question on the end
	//Restart the game while play again answer is positive
	while(playAgain == 's'){
		initializeGameTable();//Initialize game table
		showHeader();//Show header with only title
		requestPlayerNames();//Request players for typing their names
		showHeader();//Show header with the title and players names
		refreshGame();//Show the game table
		//Print a divisor line after a blank line
		printw("\n");
		for(short int i = 0; i < col; i++){
			printw("-");
		}

		/* Here is the game, we'll request a match and refresh the game table while the game is not won or tied (tied is defined by number of matches) */
		//Matches counter
		int matchesCounter = 1;
		//Disable cursor
		curs_set(0);
		//While the game is not won and matches counter is under/equal 9
		while(checkWin() == 0 && matchesCounter <= 9){
			//Request match
			requestMatch();
			//Refresh the game table
			refreshGame();
			//Increase matches counter
			matchesCounter++;
		}

		//Clear line 13
		move(13,0);
		clrtoeol();
		//Check who won, or if the game tied
		if(checkWin() == 1){
			//Player 1 won
			//Increase score
			player1Score++;
			//Show congratulations on player color
			attron(COLOR_PAIR(2));
			mvprintw(13, 0, "Parabéns %s, você ganhou!\n", player1);
			attron(COLOR_PAIR(1));
		}
		if(checkWin() == 2){
			if(!strcmp(player2, "")){
				//PC won
				//Increase score
				player2Score++;
				//Show congratulations on player color
				attron(COLOR_PAIR(3));
				mvprintw(13, 0, "O PC ganhou!\n", player2);
				attron(COLOR_PAIR(1));
			}else{
				//Player2 won
				//Increase score
				player2Score++;
				//Show congratulations on player color
				attron(COLOR_PAIR(3));
				mvprintw(13, 0, "Parabéns %s, você ganhou!\n", player2);
				attron(COLOR_PAIR(1));
			}
		}
		if(checkWin() == 0){
			//Game tied
			mvprintw(13, 0, "Deu velha! O jogo terminou em empate.\n");
		}

		//Refresh scores
		refreshScores();

		//Reset play again var
		playAgain = ' ';
		//Enable cursor
		curs_set(1);
		//Validate play again answer
		while(!(playAgain == 's' | playAgain == 'n')){
			//Clear line 15, removing possible wrong answers from screen
			move(15, 0);
			clrtoeol();
			//Ask user
			printw("Desejam jogar novamente?[s/n] ");
			playAgain = getch();
		}
	}

	
	//Disable cursor
	curs_set(0);
	//Clear screen and show the header
	showHeader();
	//Print the thanks on center
	mvprintw(6, (col-strlen("Obrigado por jogar!"))/2,"Obrigado por jogar!\n");
	//Delay
	refresh(); delay(600);
	//Print the credits
	mvprintw(9, 0, "Desenvolvido por:\n");
	printw("Renan Galeno <renangaleno@gmail.com>\n");
	printw("Marcos Antuanny <caramarcos@hotmail.com>\n");
	printw("Victor Patrick <vp8_10@hotmail.com>\n");
	printw("\n");
	//Delay
	refresh(); delay(600);
	printw("Este trabalho está licenciado sob uma Licença Creative Commons Atribuição-CompartilhaIgual 4.0 Internacional.\nPara ver uma cópia desta licença, visite http://creativecommons.org/licenses/by-sa/4.0/.\n\n");
	//Delay
	refresh(); delay(1000);
	//Print the press anything to exit message
	printw("Pressione qualquer tecla para sair.");
	getch();


	//End curses mode
	endwin();

	//Return 0, as the program had no errors
	return(0);
}