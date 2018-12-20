/* coinswap.cpp
Make moves to solve the puzzle where the objective is to swap the place of
the pieces on either side of the board.  X can only move to the right into
an empty square, or can jump to the right over an O into an empty square.
Conversely O can only move to the left into an empty square, or can jump
to the left over an X into an empty square.
*/
#include <iostream>
#include <string.h>
using namespace std;

// Global constants
const int BoardSize = 5;

//Declaring struct to keep game saved

struct Node {
	char newboard[BoardSize + 1] = "XX OO";
	int newmoveNumber;
	Node *pNext;
};

//--------------------------------------------------------------------------------
// Display name and program information
void displayIdentifyingInformation()
{
	printf("\n");
	printf("Author: Jakub          \n");
	printf("Program: #6, Coin Swap     \n");
	printf("TA: Minh, Mon 11-11:50 \n");
	printf("Nov 24, 2016               \n");
	printf("\n");
}//end displayIdentifyingInformation()


 //--------------------------------------------------------------------------------
 // Display instructions
void displayInstructions()
{
	cout << "Welcome to the coin swap puzzle.                                  \n"
		<< "Make moves to solve the puzzle where the objective is to swap the \n"
		<< "place of the pieces on either side of the board.  X can only move \n"
		<< "to the right into an empty square, or can jump to the right over  \n"
		<< "an O into an empty square. Conversely O can only move to the left \n"
		<< "into an empty square, or can jump to the left over an X into an   \n"
		<< "empty square.  \n"
		<< " \n"
		<< "For each move enter the source (1..5) and destination (1..5).     \n"
		<< "IMPORTANT** type in letter u to undo move! \n"
		<< "Enter x to exit the program. \n";
}//end displayInstructions()


 //--------------------------------------------------------------------------------
 // See if board pieces have finished being swapped.  This is the case when
 // the left-most two pieces are both 'O' and the right-most two pieces are 'X'
bool notDone(char board[])
{
	return board[0] != 'O' || board[1] != 'O' || board[3] != 'X' || board[4] != 'X';
}


//--------------------------------------------------------------------------------
void displayBoard(char board[], Node *pHead)
{//Displaying the board
	int count = 0;
	cout << endl;
	cout << "   1 2 3 4 5\n";
	cout << "   ";
	for (int i = 0; i<BoardSize; i++) {
		cout << board[i] << " ";
	}
	//Displaying the List
	cout << "   List: ";
	while (pHead != NULL) {
		if (count != 0) {
			cout << "->";
		}
		cout << pHead->newmoveNumber; //Printing out the moveNumber
		count++;					  //Count is used when to print out "->"
		pHead = pHead->pNext;		  //Advancing pointer to next Node
	}
	cout << endl;
}


//--------------------------------------------------------------------------------
void promptForAndGetMove(char board[], int &moveNumber, int &source, int &destination, Node *pTemp, Node *&pHead)
{
	char userInput;
	// Infinite loop to handle possible multiple undo of moves
	while (true) {
		cout << moveNumber << ". Enter source and destination: ";
		cin >> userInput;
		// See if user input of x was given to exit the program
		if (userInput == 'x') {
			cout << "\n";
			cout << "Exiting program...\n";
			exit(0);
		}
		if (userInput == 'u') {						//If user enters u do the following
			if (pHead->newmoveNumber == 1) {		// List is always at one at start. So display error if undo, when list is 1
				cout << "*** You cannot undo past the beginning of the game.  Please retry." << endl;
				displayBoard(board, pHead);
				continue;
			}
			// undo move
			pTemp = pHead;		//Get a temporary pointer to the current head of the list
			pHead = pHead->pNext;     //Advance the current head of the list
			strcpy(board, pTemp->newboard);  //Copy over the board
			moveNumber--;			//Subtract one from moveNumber
			pHead->newmoveNumber = moveNumber;  //Store the new MoveNumber
			delete pTemp;    //Delete the node pointed to by the temporary pointer

							 //Display List and board
			displayBoard(board, pHead);
			continue;   // Prompt to retry move
		}
		// Input is likely numeric and is not 'x' or 'u'.  Convert to a number.
		source = userInput - '0';

		// Also get destination portion of user input
		cin >> destination;

		// Adjust user entry to compensate for 0-based indexing, rather than 1-based
		source--;
		destination--;

		// break out of enclosing loop, as we don't need to undo a move
		break;

	}
}//end promptForAndGetMove()


 //--------------------------------------------------------------------------------
bool moveNotValid(char board[], int source, int destination)
{
	// Move validation code should go here...
	//Checking for: "The source and destination positions must be within the range 1..5"
	if ((source < 0 || source > 4) || (destination < 0) || (destination > 4)) {
		cout << " Cannot move out of bounds. ";
		return true;
	}
	//Destination square must be empty for both X and O
	if (board[destination] != ' ')
	{
		cout << " Must be empty space for move. ";
		return true;
	}
	//Checking for: 'X' pieces only move to the right by 1 move
	if ((board[source] == 'X')) {
		if (board[source - 1] == ' ' || board[source - 2] == ' ' || board[source - 3] == ' ' || board[source - 4] == ' '
			|| board[source + 3] == ' ' || board[source + 4] == ' ') {
			cout << "Piece X can only move to the right and can only move one space over. ";
			return true;
		}
	}
	//Checking for: 'O' pieces only move to the left by 1 move
	if ((board[source] == 'O')) {
		if (board[source + 1] == ' ' || board[source + 2] == ' ' || board[source + 3] == ' ' || board[source + 4] == ' ' ||
			board[source - 3] == ' ' || board[source - 4] == ' ') {
			cout << "Piece O can only move to the left and can only move one space over. ";
			return true;
		}
	}
	//Checking for X piece can only jump O pieces
	if ((board[source] == 'X') && board[source + 1] == 'X') {
		cout << "*** A jumped square must have an opponent. ";
		return true;
	}
	//Checking for O piece can only jump X pieces
	if ((board[source] == 'O') && board[source - 1] == 'O') {
		cout << "*** A jumped square must have an opponent. ";
		return true;
	}
	return false;
}

void createNewNode(Node *&pHead, Node *pTemp, char board[], int &moveNumber) {
	//Creating a new node
	pTemp = new Node;
	strcpy(pTemp->newboard, board);
	pTemp->newmoveNumber = moveNumber;
	pTemp->pNext = pHead;
	pHead = pTemp;
}

//--------------------------------------------------------------------------------
int main()
{
	//Declaring new varibales
	char board[BoardSize + 1] = "XX OO";  // extra character for the NULL
	int source, destination;
	int moveNumber = 1;

	//Creating two Nodes, setting one to NULL
	Node *pHead = NULL;
	Node *pTemp = NULL;

	// Display identifying information, the instructions, 
	displayIdentifyingInformation();
	displayInstructions();
	//Creating a new Node to store BEGINNING information, and displaying initial board & List
	createNewNode(pHead, pTemp, board, moveNumber);
	displayBoard(board, pHead);


	// Game play loop
	while (notDone(board)) {
		promptForAndGetMove(board, moveNumber, source, destination, pTemp, pHead);
		if (moveNotValid(board, source, destination)) {
			cout << "Invalid move, please retry. \n";
			displayBoard(board, pHead);
			continue;
		}
		//Incrementing move number and creating Node to store information
		moveNumber++;
		createNewNode(pHead, pTemp, board, moveNumber);
		// Make move.  Note that no move validation is being done.
		board[destination] = board[source];
		board[source] = ' ';

		//Displaying the board and List
		displayBoard(board, pHead);
	}

	cout << "Congratulations, you did it! \n"
		<< "\n"
		<< "Exiting program ...\n";
	return 0;         // return value to keep C++ happy
}//end main()
