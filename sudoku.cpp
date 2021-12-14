#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
using namespace std;


string promptFilename();

void getGameData(string fileName, string gameData[]);

void interact(char command, string gameData[]);

void displayBoard(string gameData[]);

bool isValidCommand(char command);

void displayOptions();

void promptLocation(string gameData[], char command);

void addNumber(string location, string gameData[], int number);

bool isValidLocation(string location, string gameData[]);

void possibleValues(string location, string gameData[]);

void saveFile(string gameData[]);

int findRow(string gameData[], string location);

int findCol(string gameData[], string location);

bool isNumberValid(string gameData[], string location, int number);

/**********************************************************************
 * Gets gameData, fileName, and contains game loop
 ***********************************************************************/
int main(){
   bool keepPlaying = true;
   string gameData[9];
   string fileName = promptFilename();
   getGameData(fileName, gameData);
   displayOptions();
   displayBoard(gameData);

   // Game loop
   while (keepPlaying == true){
      char command;
      cout << endl;
      cout << "> ";
      cin >> command;

      interact(command, gameData);

      if (command == 'Q' || command == 'q'){
         keepPlaying = false;
      }
   }

   return 0;
}

/**********************************************************************
 * Gets file name from user
 ***********************************************************************/
string promptFilename(){
   string fileName;
   cout << "Where is your board located? ";
   cin >> fileName;
   return fileName;
}

/**********************************************************************
 * Gets game data from file
 ***********************************************************************/
void getGameData(string fileName, string gameData[]){
   ifstream fin(fileName);

   for (int i = 0; i < 9; i++){
      getline(fin, gameData[i]);
   }

   fin.close();
}

/**********************************************************************
 * Displays options to user
 ***********************************************************************/
void displayOptions(){
   cout << "Options:\n";
   cout << "   ?  Show these instructions\n";
   cout << "   D  Display the board\n";
   cout << "   E  Edit one square\n";
   cout << "   S  Show the possible values for a square\n";
   cout << "   Q  Save and Quit\n";
   cout << endl;
}

/**********************************************************************
 * Displays the game board to the user
 ***********************************************************************/
void displayBoard(string gameData[]){
   cout << "   A B C D E F G H I\n";
   int row = 1;
   string divider = "   -----+-----+-----";
   for (int i = 0; i < 9; i++){
      if (i == 3 || i == 6){
         cout << divider << endl;
      }

      cout << row << "  ";

      for (int j = 0; j < 17; j++){
         if (j == 5 || j == 11){
            cout << '|';
            continue;
         }

         if (j % 2 == 0){
            (gameData[i][j] == '0' ? cout << ' ' : cout << gameData[i][j]);
         }
         else{
            cout << ' ';
         }
      }
      cout << endl;
      row++;
   }
}

/**********************************************************************
 * Gets the location of the square to be edited
 ***********************************************************************/
void interact(char command, string gameData[])
{
   if (command == '?'){
      displayOptions();
   }
   else if (command == 'D' || command == 'd'){
      displayBoard(gameData);
   }
   else if (command == 'E' || command == 'e'){
      promptLocation(gameData, command);
   }
   else if (command == 'Q' || command == 'q'){
      saveFile(gameData);
   }
   else if (command == 'S' || command == 's'){
      promptLocation(gameData, command);
   }
   else{
      cout << "Unknown command\n";
   }
}

/**********************************************************************
 * Gets the location of the square to be edited
 ***********************************************************************/
void promptLocation(string gameData[], char command){
   string location;
   cout << "What are the coordinates of the square: ";
   cin >> location;

   bool isValid = isValidLocation(location, gameData);
   if (isValid == false){
      cout << "ERROR: Square '" << location << "' is filled\n";
      return;
   }

   if (command == 'E' || command == 'e'){
      int number;
      cout << "What is the value at '" << location << "': ";
      cin >> number;
      addNumber(location, gameData, number);
   }
   else if (command == 'S' || command == 's'){
      possibleValues(location, gameData);
   }
}

/**********************************************************************
 * Adds number to gameData
 ***********************************************************************/
void addNumber(string location, string gameData[], int number){
   int row = findRow(gameData, location);
   int col = findCol(gameData, location);

   bool isValid = isNumberValid(gameData, location, number);
   if (isValid == false){
      cout << "ERROR: Value '" << number << "' in square '" << location;
      cout << "' is invalid\n";
      return;
   }

   // Converts int to char
   char cNumber = '0' + number;

   // Adds number to gameData
   gameData[row][col] = cNumber;
}

/**********************************************************************
 * Saves the gameData to a file
 ***********************************************************************/
void saveFile(string gameData[]){
   string fileName;
   cout << "What file would you like to write your board to: ";
   cin >> fileName;

   ofstream fout;
   fout.open(fileName);

   for (int i = 0; i < 9; i++){
      fout << gameData[i];
      fout << endl;
   }

   fout.close();

   cout << "Board written successfully\n";
}

/**********************************************************************
 * Checks to see if the location is playable
 ***********************************************************************/
bool isValidLocation(string location, string gameData[]){
   bool isValid = true;

   int row = findRow(gameData, location);
   int col = findCol(gameData, location);

   char number = gameData[row][col];

   if (number != '0'){
      return false;
   }

   return isValid;
}

/**********************************************************************
 * Checks to see if the location is playable
 ***********************************************************************/
void possibleValues(string location, string gameData[]){
   int row = findRow(gameData, location);
   int col = findCol(gameData, location);
   bool validNumbers[9] = {true, true, true, true, true, true, true,
      true, true};

   // Searches row
   for (int i = 0; i < 17; i += 2){
      int num = gameData[row][i] - '0';
      if (num > 0){
         validNumbers[num - 1] = false;
      }
   }

   // Searches col
   for (int i = 0; i < 9; i++){
      int num = gameData[i][col] - '0';
      if (num > 0){
         validNumbers[num - 1] = false;
      }
   }

   // Searches location area
   int groupRow;
   int groupCol;

   if (row <= 2){
      groupRow = 0;
   }
   else if (row <= 5){
      groupRow = 3;
   }
   else{
      groupRow = 6;
   }

   if (col <= 4){
      groupCol = 0;
   }
   else if (col <= 10){
      groupCol = 6;
   }
   else{
      groupCol = 12;
   }

   for (int i = groupRow; i < groupRow + 3; i++){
      for (int j = groupCol; j < groupCol + 5; j += 2){
         int num = gameData[i][j] - '0';
         if (num > 0){
            validNumbers[num - 1] = false;
         }
      }
   }

   // Displays valid numbers to user
   cout << "The possible values for '" << location << "' are: ";

   bool addedOne = false;
   for (int i = 0; i < 9; i++){
      if (addedOne == true && validNumbers[i] == true){
         cout << ", ";
      }
      if (validNumbers[i] == true){
         cout << i + 1;
         addedOne = true;
      }
   }
   cout << endl;
}

/**********************************************************************
 * Checks to see if the number can be placed in location
 ***********************************************************************/
bool isNumberValid(string gameData[], string location, int number){
   bool isValid = true;

   int row = findRow(gameData, location);
   int col = findCol(gameData, location);
   bool validNumbers[9] = {true, true, true, true, true, true, true,
      true, true};

   // Searches row
   for (int i = 0; i < 17; i += 2){
      int num = gameData[row][i] - '0';
      if (num > 0){
         validNumbers[num - 1] = false;
      }
   }

   // Searches col
   for (int i = 0; i < 9; i++){
      int num = gameData[i][col] - '0';
      if (num > 0){
         validNumbers[num - 1] = false;
      }
   }

   // Searches location area
   int groupRow;
   int groupCol;

   if (row <= 2){
      groupRow = 0;
   }
   else if (row <= 5){
      groupRow = 3;
   }
   else{
      groupRow = 6;
   }

   if (col <= 4){
      groupCol = 0;
   }
   else if (col <= 10){
      groupCol = 6;
   }
   else{
      groupCol = 12;
   }

   for (int i = groupRow; i < groupRow + 3; i++){
      for (int j = groupCol; j < groupCol + 5; j += 2){
         int num = gameData[i][j] - '0';
         if (num > 0){
            validNumbers[num - 1] = false;
         }
      }
   }

   // Checks to see if number was already used
   for (int i = 0; i < 9; i++){
      if (validNumbers[i] == false && i + 1 == number){
         isValid = false;
      }
   }

   return isValid;
}

/**********************************************************************
 * Finds the row
 ***********************************************************************/
int findRow(string gameData[], string location){
   int row;

   if (!isalpha(location[0])){
      if (location[0] == '1'){
         row = 0;
      }
      else if (location[0] == '2'){
         row = 1;
      }
      else if (location[0] == '3'){
         row = 2;
      }
      else if (location[0] == '4'){
         row = 3;
      }
      else if (location[0] == '5'){
         row = 4;
      }
      else if (location[0] == '6'){
         row = 5;
      }
      else if (location[0] == '7'){
         row = 6;
      }
      else if (location[0] == '8'){
         row = 7;
      }
      else if (location[0] == '9'){
         row = 8;
      }
   }
   else{
      if (location[1] == '1'){
         row = 0;
      }
      else if (location[1] == '2'){
         row = 1;
      }
      else if (location[1] == '3'){
         row = 2;
      }
      else if (location[1] == '4'){
         row = 3;
      }
      else if (location[1] == '5'){
         row = 4;
      }
      else if (location[1] == '6'){
         row = 5;
      }
      else if (location[1] == '7'){
         row = 6;
      }
      else if (location[1] == '8'){
         row = 7;
      }
      else if (location[1] == '9'){
         row = 8;
      }
   }

   return row;
}

/**********************************************************************
 * Finds the col
 ***********************************************************************/
int findCol(string gameData[], string location){
   int col;

   if (isalpha(location[0])){
      if (location[0] == 'A' || location[0] == 'a'){
         col = 0;
      }
      else if (location[0] == 'B' || location[0] == 'b'){
         col = 2;
      }
      else if (location[0] == 'C' || location[0] == 'c'){
         col = 4;
      }
      else if (location[0] == 'D' || location[0] == 'd'){
         col = 6;
      }
      else if (location[0] == 'E' || location[0] == 'e'){
         col = 8;
      }
      else if (location[0] == 'F' || location[0] == 'f'){
         col = 10;
      }
      else if (location[0] == 'G' || location[0] == 'g'){
         col = 12;
      }
      else if (location[0] == 'H' || location[0] == 'h'){
         col = 14;
      }
      else if (location[0] == 'I' || location[0] == 'i'){
         col = 16;
      }
   }
   else{
      if (location[1] == 'A' || location[1] == 'a'){
         col = 0;
      }
      else if (location[1] == 'B' || location[1] == 'b'){
         col = 2;
      }
      else if (location[1] == 'C' || location[1] == 'c'){
         col = 4;
      }
      else if (location[1] == 'D' || location[1] == 'd'){
         col = 6;
      }
      else if (location[1] == 'E' || location[1] == 'e'){
         col = 8;
      }
      else if (location[1] == 'F' || location[1] == 'f'){
         col = 10;
      }
      else if (location[1] == 'G' || location[1] == 'g'){
         col = 12;
      }
      else if (location[1] == 'H' || location[1] == 'h'){
         col = 14;
      }
      else if (location[1] == 'I' || location[1] == 'i'){
         col = 16;
      }
   }

   return col;
}
