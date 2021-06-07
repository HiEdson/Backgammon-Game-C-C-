#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <ctime>
#include <map>
#include <sstream>
#include <cstring>
using namespace std;

struct Player
{
    bool first = false;
    char name;
};

string mainStatus[3][12] = {{"5Y", "0", "0", "0", "3X", "0", "5X", "0", "0", "0", "0", "2Y"},
                            {"0", "0", "0", "0", "FX", "D1", "D2",
                             "FY", "0", "0", "0", "0"},
                            {"5X", "0", "0", "0", "3Y", "0", "5Y", "0", "0", "0", "0", "2X"}};
string newStr[3][12] = {};

void welcomeSms();
void startPlayer(int *);                             //decides heither player X or Y is going to start
void firstPlayer(string mainStatus[3][12], int, Player &, Player &);           //give some privileges to the first player
void play(string mainStatus[3][12], Player, Player); //start to play the game
void log(char, int, int);
void log(int, int);
void initialState(string mainStatus[][12]);
void initialStateConsole(string mainStatus[][12]); // just to test
void dice_in_grapth(string mainStatus[][12], int, int);
string to_string(char);
int char_to_int(char);
string int_to_char(int);
char string_to_char(string, int);
void move_x(string mainStatus[3][12], int, char[]);
void move_y(string mainStatus[3][12], int, char[]);
void decrement(int, int, string a[3][12]);
char get_first_letter(char[]);
int get_number(char[]);
int letter_to_number(char);
string get_latter_from_grapth(string);
int fromFileX = 0;
int fromFileY = 0;
void fromFile(int *, int *);
void toFile(int, int);
bool checkFlag(string mainStatus[][12], char, int, int);
void pick_flakes(string mainStatus[][12], int, int *, int *);
void DiceInFile(string mainStatus[3][12], int, int);
void DiceFromFile(string mainStatus[3][12], int *, int *);

//void actual_dices(string mainStatus[][12], int, int);
int main(void)
{


    initialStateConsole(mainStatus);
    welcomeSms();

    return 0;
}

//ELIMINAR INITIALSTATE CONSOLE ORGANIZAR O GRAFICO AO IMPRIMIR

void welcomeSms()
{
    initialState(mainStatus);
    printf("WELCOME TO OUR GAME, ENTER A NUMBER\n");
    printf("1==> NEW GAME\n2==> CONTINUE GAME\n3==> QUIT\n");
    int a;
    if (scanf("%d", &a) == 1)
    {
        switch (a)
        {
        case 1:
        {
            initialState(mainStatus);
            Player X, Y;
            int start_player = 0;
            startPlayer(&start_player);

            //set privilegies to the first player
            firstPlayer(mainStatus, start_player, X, Y);
            play(mainStatus, X, Y);
            initialState(mainStatus);
        }
        break;

        case 2:
        {
        	 
        	
        	DiceFromFile(newStr, &fromFileX, &fromFileY);
        	initialState(newStr);
        	int comp = char_to_int(string_to_char(newStr[1][5],0));
	
            if(comp == 0)
            {
                printf("NO PREVIOUS GAME RECORDED!\n");
                break;
            }
            else
            {
            	
                
                //initialStateConsole(newStr);
                Player X, Y;
                int start_player = 0;
                startPlayer(&start_player);

                //set privilegies to the first player
                firstPlayer(mainStatus, start_player, X, Y);
                play(newStr, X, Y);
                initialState(newStr);
            }
        }
        break;
        case 3:
            printf("BYE BYE!!\n");
            break;
        }
    }
}

void DiceInFile(string mainStatus[3][12], int a, int b)
{

    //using sequential file
    FILE *continueGame;
    if ((continueGame = fopen("continueGame.dat", "w")) == NULL)
    {
        printf("LOG.txt file not found \n");
    }
    else
    {
        fprintf(continueGame, "%d %d ", a, b);

        for (int i = 0; i < 3; i++)
        {
            for (int t = 0; t < 12; t++)
            {
                string a = mainStatus[i][t];
                fprintf(continueGame, "%s ", a.c_str());
            }
        }

        fclose(continueGame);
    }
}
void DiceFromFile(string newStr[3][12], int *a, int *b)
{
    FILE *continueGame;
    if ((continueGame = fopen("continueGame.dat", "r")) == NULL)
    {
        printf("LOG.txt file not found \n");
    }
    else
    {
        int xa, xb;
        fscanf(continueGame, "%d %d ", &xa, &xb);
        for (int i = 0; i < 3; i++)
        {
            for (int t = 0; t < 12; t++)
            {
                string a = mainStatus[i][t];
                fscanf(continueGame, "%s ", newStr[i][t].c_str());
            }
        }

        fclose(continueGame);
    }
}

//defining every function here
string int_to_char(int a)
{
    char sx[2];
    sprintf(sx, "%d", a);
    return sx;
}

int char_to_int(char a)
{

    string b = to_string(a);
    stringstream to_int(b);
    int x = 0;
    to_int >> x;
    return x;
}

//check if the slot we want to move belongs to the actual user;
string get_latter_from_grapth(string a)
{
    string x = a;
    char oneOnly = x.at(1);
    string s;
    s.push_back(oneOnly);
    return s;
}

char get_first_letter(char slot[])
{
    //get the first letter of the slot
    return slot[0];
}

int get_number(char slot[])
{
    // return the number after the letter eg. A1 ==> 1
    return slot[1] - 48;
}

int letter_to_number(char a)
{
    return a - 0x41;
}

string to_string(char x)
{
    char oneOnly = x;
    string s;
    s.push_back(oneOnly);
    return s;
}

char string_to_char(string a, int position)
{

    int n = a.length();
    char char_array[n + 1];

    strcpy(char_array, a.c_str());
    return char_array[position];
}

//choose the player that is going to start the game
void startPlayer(int *start_player)
{

    int a, b;
    printf("X PLAYER THROW THE DICE (any number between 1-6)\n");
    scanf("%d", &a);

    printf("Y PLAYER THROW THE DICE (any number between 1-6)\n");
    scanf("%d", &b);
    log(a, b);
    //******************I HAVE TO WRITE IT IN THE FILE log.dat

    srand(time(0));
    int dice1 = (rand() % 6 + 1);
    int dice2 = (rand() % 6 + 1);

    if (dice1 > dice2)
    {
        printf("X PLAYER IS GOIND TO START THE GAME!\n");
        *start_player = 1; //means its X
    }
    else
    {
        printf("Y PLAYER IS GOIND TO START THE GAME!\n");
        *start_player = 2; //means its Y
    }
}

void firstPlayer(string a[3][12], int start_player, Player &x, Player &y)
{
	
    x.name = 'X';
    y.name = 'Y';
    string comp = a[1][8];
	
    if(string_to_char(comp,0)=='X'){
    	x.first = true;
	}else if (string_to_char(comp,0)=='Y'){
		y.first = true;
	}
	else{
		if (start_player == 1)
    	{
        	x.first = true;
    	}
    	else
    	{
        	y.first = true;
    	}
	}
}


bool same_user(string a[3][12], int entered_slot_int, int slot_position, Player p)
{

    bool permit = false;

    if (entered_slot_int == 1)
    {
        string t;
        t.push_back(p.name);

        if (get_latter_from_grapth(a[0][slot_position]) == t)
        {
            permit = true;
        }
    }
    else if (entered_slot_int == 5)
    {
        string t;
        t.push_back(p.name);
        if (get_latter_from_grapth(a[2][slot_position]) == t)
        {
            permit = true;
        }
    }
    else
    {
        printf("YOU ARE ONLY ALLOWED TO MOVE VALID SLOTS\n");
    }

    return permit;
}

bool moviment_allowed(string a[3][12], int dice1, int dice2, char slot1[], char slot2[], Player p1)
{

    bool go = false;

    char entered_slot1_char = get_first_letter(slot1); //get the first latter
    char entered_slot2_char = get_first_letter(slot2);
    int entered_slot1_int = get_number(slot1); // Will help us to get the row E´1' means the first row
    int entered_slot2_int = get_number(slot2);

    int firstSlotPosition = letter_to_number(entered_slot1_char); // return the equivalent number. if A ==> 0
    int secondSlotPosition = letter_to_number(entered_slot2_char);

    bool permitSlot1 = same_user(a, entered_slot1_int, firstSlotPosition, p1);
    bool permitSlot2 = same_user(a, entered_slot2_int, secondSlotPosition, p1);

    if (permitSlot1 && permitSlot2)
    {
        go = true;
    }

    return go;
}



void log(int a, int b)
{
    FILE *log;
    if ((log = fopen("log.dat", "a")) == NULL)
    {
        printf("LOG.dat file not found \n");
    }
    else
    {
        fprintf(log, "%d\n%d\n\n", a, b);
        fclose(log);
    }
}

void log(char c, int a, int b)
{
    FILE *log;
    if ((log = fopen("log.dat", "a")) == NULL)
    {
        printf("LOG.dat file not found \n");
    }
    else
    {
        fprintf(log, "%c %d %d\n", c, a, b);
        fclose(log);
    }
}




void play(string str[3][12], Player player1, Player player2)
{

    if (player1.first == true)
    {
        while (1 == 1)
        {
            int a, b;
            char slot1[2], slot2[2];
            printf("%c PLAYER THROW DICE1 and DICE2: NUMBER BETWEEN 1-6 (0 to finish or save for future play)\n", player1.name);
            scanf("%d%d", &a, &b);

            while ((a != 0 && a > 6) || (b != 0 && b > 6))
            {
                printf("INVALID NUMBERS, ENTER NUMBER BETWEEN 1-6\n");
                scanf("%d%d", &a, &b);
            }
            if (a == 0 || b == 0)
            {
                printf("PRESS\n1==>END THE GAME\n2==>SAVE FOR FUTURE PLAY!\n");
                int g;
                scanf("%d", &g);
                switch (g)
                {
                case 1:
                {
                    printf("GAME ENDED BY THE USER!\n");
                    //clean the grapth
                    string array[3][12] = {};
                    for (int i = 0; i < 3; i++)
                    {
                        for (int t = 0; t < 12; t++)
                        {
                            array[i][t] = "0";
                        }
                    }
                    DiceInFile(array, 0, 0);
                }
                break;
                case 2:
                {
                    DiceInFile(str, fromFileX, fromFileY);
                    printf("GAME WAS SAVED SUCCESSFULLY, BYE BYE!\n");
                }
                break;
                }
                break;
            }
            else
            {

                if (checkFlag(str, 'X', fromFileX, fromFileY))
                {
                    pick_flakes(str, 0, &fromFileX, &fromFileY);
                }
                else
                {
                    //write the information in the file log.txt
                    log(player1.name, a, b);
                    dice_in_grapth(str, a, b);
                    initialState(str);

                    //CHECK IF IS A SUPER PLAYER
                    if (a == b)
                    {
                        printf("\n DICE1= %d \n DICE2= %d\n", a, b);
                        printf("ENTER THE SLOT THAT WILL BE MOVED TWICE: CAPITAL LETTERS A1, B..\n");
                        scanf("%s", slot1);
                        if (slot1[0] >= 'a' && slot1[0] <= 'l')
                        {
                            slot1[0] = slot1[0] - 32;
                        }
                        bool result = moviment_allowed(mainStatus, a, b, slot1, slot1, player1);
                        while (result != 1)
                        {
                            printf("THE ENTERED SLOT ARE NOT ALLOWED TO MOVED!\n");
                            printf("ENTER VALID SLOT\n");
                            scanf("%s", slot1);
                            result = moviment_allowed(mainStatus, a, b, slot1, slot1, player1);
                        }
                        initialState(str);
                        move_x(str, a, slot1); //move the slot one twice
                        move_x(str, a, slot1); //
                        str[1][8] = "Y";
                        initialState(str);
                    }
                    else
                    {
                        printf("\n DICE1= %d \n DICE2= %d\n", a, b);
                        printf("ENTER THE DESIRED SLOTS: CAPITAL LETTERS A1, B...\n");
                        scanf("%s%s", slot1, slot2);

                        bool result = moviment_allowed(mainStatus, a, b, slot1, slot2, player1);
                        while (result != 1)
                        {
                            printf("THE ENTERED SLOTS ARE NOT ALLOWED TO MOVED!\n");
                            printf("ENTER VALID SLOTS\n");
                            scanf("%s%s", slot1, slot2);
                            result = moviment_allowed(mainStatus, a, b, slot1, slot2, player1);
                        }
                        initialState(str);
                        move_x(str, a, slot1); //move the slot1
                        move_x(str, b, slot2); // move slot2
                        str[1][8] = "Y";
                        initialState(str);
                    }
                }
            }

            //next player

            printf("%c PLAYER THROW DICE1 and DICE2: NUMBER BETWEEN 1-6 (0 to finish or save the for future play)\n", player2.name);
            scanf("%d%d", &a, &b);

            while ((a != 0 && a > 6) || (b != 0 && b > 6))
            {
                printf("INVALID NUMBERS, ENTER NUMBER BETWEEN 1-6\n");
                scanf("%d%d", &a, &b);
            }
            if (a == 0 || b == 0)
            {
                printf("PRESS\n1==>END THE GAME\n2==>SAVE FOR FUTURE PLAY!\n");
                int g;
                scanf("%d", &g);
                switch (g)
                {
                case 1:
                {
                    printf("GAME ENDED BY THE USER!\n");
                    //clean the grapth
                    string array[3][12] = {};
                    for (int i = 0; i < 3; i++)
                    {
                        for (int t = 0; t < 12; t++)
                        {
                            array[i][t] = "0";
                        }
                    }
                    DiceInFile(array, 0, 0);
                }
                break;
                case 2:
                {
                    DiceInFile(str, fromFileX, fromFileY);
                    printf("GAME WAS SAVED SUCCESSFULLY, BYE BYE!\n");
                }
                break;
                }
                break;
            }
            else
            {
                if (checkFlag(str, 'Y', fromFileX, fromFileY))
                {
                    pick_flakes(str, 0, &fromFileX, &fromFileY);
                }
                else
                {
                    log(player2.name, a, b);
                    dice_in_grapth(str, a, b);
                    initialState(str);

                    //CHECK IF IS A SUPER PLAYER
                    if (a == b)
                    {
                        printf("\n DICE1= %d \n DICE2= %d\n", a, b);
                        printf("ENTER THE SLOT THAT WILL BE MOVED TWICE: CAPITAL LETTERS A1, B...\n");
                        scanf("%s", slot1);
                        bool result = moviment_allowed(mainStatus, a, b, slot1, slot1, player2);
                        while (result != 1)
                        {
                            printf("THE ENTERED SLOT ARE NOT ALLOWED TO MOVED!\n");
                            printf("ENTER VALID SLOT\n");
                            scanf("%s", slot1);
                            result = moviment_allowed(mainStatus, a, b, slot1, slot1, player2);
                        }
                        initialState(str);
                        move_y(str, a, slot1); //move the slot one twice
                        move_y(str, a, slot1); //
                        str[1][8] = "X";
                        initialState(str);
                    }
                    else
                    {
                        printf("DICE1= %d \n DICE2= %d\n", a, b);
                        printf("ENTER THE DESIRED SLOTS: CAPITAL LETTERS A1, B...\n");
                        scanf("%s%s", slot1, slot2);
                        bool result = moviment_allowed(mainStatus, a, b, slot1, slot2, player2);
                        while (result != 1)
                        {
                            printf("THE ENTERED SLOTS ARE NOT ALLOWED TO MOVED!\n ");
                            printf("ENTER VALID SLOTS\n");
                            scanf("%s%s", slot1, slot2);
                            result = moviment_allowed(mainStatus, a, b, slot1, slot2, player2);
                        }
                        initialState(str);
                        move_y(str, a, slot1); //move the slot1
                        move_y(str, b, slot2); // move slot2
                        str[1][8] = "X";
                        initialState(str);
                    }
                }
            }
        }
    }

    else if (player2.first == true)
    {
        while (1 == 1)
        {
            int a, b;
            char slot1[2], slot2[2];

            printf("%c PLAYER THROW DICE1 and DICE2: NUMBER BETWEEN 1-6  (0 to finish or save for future play)\n", player2.name);
            scanf("%d%d", &a, &b);

            while ((a != 0 && a > 6) || (b != 0 && b > 6))
            {
                printf("INVALID NUMBERS, ENTER NUMBER BETWEEN 1-6\n");
                scanf("%d%d", &a, &b);
            }
            if (a == 0 || b == 0)
            {
                printf("PRESS\n1==>END THE GAME\n2==>SAVE FOR FUTURE PLAY!\n");
                int g;
                scanf("%d", &g);
                switch (g)
                {
                case 1:
                {
                    printf("GAME ENDED BY THE USER!\n");

                    string array[3][12] = {};
                    for (int i = 0; i < 3; i++)
                    {
                        for (int t = 0; t < 12; t++)
                        {
                            array[i][t] = "0";
                        }
                    }
                    DiceInFile(array, 0, 0);
                }
                break;
                case 2:
                {
                    DiceInFile(str, fromFileX, fromFileY);
                    printf("GAME WAS SAVED SUCCESSFULLY, BYE BYE!\n");
                }
                break;
                }
                break;
            }
            else
            {
                if (checkFlag(str, 'Y', fromFileX, fromFileY))
                {
                    pick_flakes(str, 0, &fromFileX, &fromFileY);
                }
                else
                {
                    log(player2.name, a, b);
                    dice_in_grapth(str, a, b);
                    initialState(str);

                    //CHECK IF IS A SUPER PLAYER
                    if (a == b)
                    {
                        printf("\n DICE1= %d \n DICE2= %d\n", a, b);
                        printf("ENTER THE SLOT THAT WILL BE MOVED TWICE: CAPITAL LETTERS A1, B...\n");
                        scanf("%s", slot1);
                        bool result = moviment_allowed(mainStatus, a, b, slot1, slot1, player2);
                        while (result != 1)
                        {
                            printf("THE ENTERED SLOT ARE NOT ALLOWED TO MOVED!\n");
                            printf("ENTER VALID SLOT\n");
                            scanf("%s", slot1);
                            result = moviment_allowed(mainStatus, a, b, slot1, slot1, player2);
                        }
                        initialState(str);
                        move_y(str, a, slot1); //move the slot one twice
                        move_y(str, a, slot1); //
                        str[1][8] = "X";
                        initialState(str);
                    }
                    else
                    {
                        printf("DICE1= %d \n DICE2= %d\n", a, b);
                        printf("ENTER THE DESIRED SLOTS: CAPITAL LETTERS A1, B...\n");
                        scanf("%s%s", slot1, slot2);
                        bool result = moviment_allowed(mainStatus, a, b, slot1, slot2, player2);
                        while (result != 1)
                        {
                            printf("THE ENTERED SLOTS ARE NOT ALLOWED TO MOVED!\n ");
                            printf("ENTER VALID SLOTS\n");
                            scanf("%s%s", slot1, slot2);
                            result = moviment_allowed(mainStatus, a, b, slot1, slot2, player2);
                        }
                        initialState(str);
                        move_y(str, a, slot1); //move the slot1
                        move_y(str, b, slot2); // move slot2
                        str[1][8] = "X";
                        initialState(str);
                    }
                }
            }

            //for the next user

            printf("%c PLAYER THROW DICE1 and DICE2: NUMBER BETWEEN 1-6  (0 to finish or save for future play)\n", player1.name);
            scanf("%d%d", &a, &b);
            while ((a != 0 && a > 6) || (b != 0 && b > 6))
            {
                printf("INVALID NUMBERS, ENTER NUMBER BETWEEN 1-6\n");
                scanf("%d%d", &a, &b);
            }
            if (a == 0 || b == 0)
            {
                printf("PRESS\n1==>END THE GAME\n2==>SAVE FOR FUTURE PLAY!\n");
                int g;
                scanf("%d", &g);
                switch (g)
                {
                case 1:
                {
                    printf("GAME ENDED BY THE USER!\n");
                    //clean the grapth
                    string array[3][12] = {};
                    for (int i = 0; i < 3; i++)
                    {
                        for (int t = 0; t < 12; t++)
                        {
                            array[i][t] = "0";
                        }
                    }
                    DiceInFile(array, 0, 0);
                }
                break;
                case 2:
                {
                    DiceInFile(str, fromFileX, fromFileY);
                    printf("GAME WAS SAVED SUCCESSFULLY, BYE BYE!\n");
                }
                break;
                }
                break;
            }
            else
            {
                if (checkFlag(str, 'X', fromFileX, fromFileY))
                {
                    pick_flakes(str, 0, &fromFileX, &fromFileY);
                }
                else
                {
                    log(player1.name, a, b);
                    dice_in_grapth(str, a, b);
                    initialState(str);

                    //CHECK IF IS A SUPER PLAYER
                    if (a == b)
                    {
                        printf("\n DICE1= %d \n DICE2= %d\n", a, b);
                        printf("ENTER THE SLOT THAT WILL BE MOVED TWICE: CAPITAL LETTERS A1, B..\n");
                        scanf("%s", slot1);
                        if (slot1[0] >= 'a' && slot1[0] <= 'l')
                        {
                            slot1[0] = slot1[0] - 32;
                        }
                        bool result = moviment_allowed(mainStatus, a, b, slot1, slot1, player1);
                        while (result != 1)
                        {
                            printf("THE ENTERED SLOT ARE NOT ALLOWED TO MOVED!\n");
                            printf("ENTER VALID SLOT\n");
                            scanf("%s", slot1);
                            result = moviment_allowed(mainStatus, a, b, slot1, slot1, player1);
                        }
                        initialState(str);
                        move_x(str, a, slot1); //move the slot one twice
                        move_x(str, a, slot1); //
                        str[1][8] = "Y";
                        initialState(str);
                    }
                    else
                    {
                        printf("\n DICE1= %d \n DICE2= %d\n", a, b);
                        printf("ENTER THE DESIRED SLOTS: CAPITAL LETTERS A1, B...\n");
                        scanf("%s%s", slot1, slot2);

                        bool result = moviment_allowed(mainStatus, a, b, slot1, slot2, player1);
                        while (result != 1)
                        {
                            printf("THE ENTERED SLOTS ARE NOT ALLOWED TO MOVED!\n");
                            printf("ENTER VALID SLOTS\n");
                            scanf("%s%s", slot1, slot2);
                            result = moviment_allowed(mainStatus, a, b, slot1, slot2, player1);
                        }
                        initialState(str);
                        move_x(str, a, slot1); //move the slot1
                        move_x(str, b, slot2); // move slot2
                        str[1][8] = "Y";
                        initialState(str);
                    }
                }
            }
        }
    }
    else
    {
        printf("Something is wrong, conditions not satisfying \n");
    }
}


//add the dice values to the grapth
void dice_in_grapth(string a[3][12], int dice1, int dice2)
{

    string sdice1 = to_string(dice1);
    string sdice2 = to_string(dice2);

    a[1][5] = sdice1 + " ";
    a[1][6] = sdice2 + " ";
}

//add the broken dices for x and for y
void broken_dices(string a[3][12], char c)
{

    string broken = "";
    if (c == 'X')
    {
        broken = a[1][4];
    }
    else
    {
        broken = a[1][7];
    }
    stringstream to_int(broken);
    int x = 0;
    to_int >> x;
    x += 1;

    char sx[2];
    itoa(x, sx, 10);

    if (c == 'X')
    {
        a[1][4] = sx;
    }
    else
    {
        a[1][7] = sx;
    }
}

void move_x(string mainStatus[3][12], int dice, char slot[])
{

    int res = char_to_int(slot[1]);
    if (res == 1)
    {
        int col = letter_to_number(slot[0]);
        int row = 0;
        int next_col = dice + col;
        string value1 = mainStatus[0][next_col];
        if (value1 == "0")
        {

            mainStatus[0][next_col] = (int_to_char(1) + "X");

            //decrement
            string actualValue = mainStatus[0][col];

            char decrement = actualValue.at(0);

            int decrementInt = char_to_int(decrement);

            decrementInt = decrementInt - 1;
            mainStatus[0][col] = (int_to_char(decrementInt) + "X");
            string after = mainStatus[0][col];

            printf("SLOT MOVED SUCCESSFULLY\n");
        }
        else if (get_latter_from_grapth(mainStatus[0][next_col]) == "X")
        {
            string nextValue = mainStatus[0][next_col];
            char onlyTheNumber = nextValue.at(0);
            int onlyNumberInt = char_to_int(onlyTheNumber);

            if (onlyNumberInt < 6)
            {
                onlyNumberInt = onlyNumberInt + 1;
                mainStatus[0][next_col] = (int_to_char(onlyNumberInt) + "X");
                string place = mainStatus[0][next_col];

                //decrement
                string actualValue = mainStatus[0][col];
                char decrement = actualValue.at(0);
                int decrementInt = char_to_int(decrement);
                decrementInt = decrementInt - 1;
                mainStatus[0][col] = (int_to_char(decrementInt) + "X");
                string after = mainStatus[0][col];
            }
            else
            {
                printf("IMPOSSIBLE TO MOVE THIS SLOT, WE ALREADY HAVE 5 ELEMENTS IN THAT POSITION\n");
            }
        }
        else if (get_latter_from_grapth(mainStatus[0][next_col]) != "X")
        {
            string nextPos = mainStatus[0][next_col];
            char nextNumber = nextPos.at(0);
            int nextInt = char_to_int(nextNumber);
            if (nextInt == 1)
            {
                broken_dices(mainStatus, 'Y');
                mainStatus[0][next_col] = (int_to_char(1) + "X");

                //decrement
                string actualValue = mainStatus[0][col];
                char decrement = actualValue.at(0);
                int decrementInt = char_to_int(decrement);
                decrementInt = decrementInt - 1;
                mainStatus[0][col] = (int_to_char(decrementInt) + "X");
                string after = mainStatus[0][col];
            }
            else
            {
                printf("IMPOSSIBLE TO PLAY IN THIS SLOT");
            }
        }
    }
    else
    {
        //means the user entered something from line 5
        int col = letter_to_number(slot[0]);
        int reminder;
        int next_col;
        if (col > dice && col != dice)
        {
            next_col = col - dice;
        }
        else if (col == dice)
        {
            next_col = 0;
        }
        else
        {
            reminder = dice - col;
            next_col = reminder;
        }
        if (reminder == 0)
        {
            string value1 = mainStatus[2][next_col];
            if (value1 == "0")
            {

                mainStatus[2][next_col] = (int_to_char(1) + "X");
                //decrement
                string actualValue = mainStatus[2][col];
                char decrement = actualValue.at(0);
                int decrementInt = char_to_int(decrement);
                decrementInt = decrementInt - 1;
                mainStatus[2][col] = (int_to_char(decrementInt) + "X");
                string after = mainStatus[2][col];
                printf("SLOT MOVED SUCCESSFULLY\n");
            }
            else if (get_latter_from_grapth(mainStatus[2][next_col]) == "X")
            {
                string nextValue = mainStatus[2][next_col];
                char onlyTheNumber = nextValue.at(0);
                int onlyNumberInt = char_to_int(onlyTheNumber);
                if (onlyNumberInt < 6)
                {
                    onlyNumberInt = onlyNumberInt + 1;
                    mainStatus[2][next_col] = (int_to_char(onlyNumberInt) + "X");
                    string place = mainStatus[2][next_col];
                    //decrement
                    string actualValue = mainStatus[2][col];

                    char decrement = actualValue.at(0);

                    int decrementInt = char_to_int(decrement);

                    decrementInt = decrementInt - 1;
                    mainStatus[2][col] = (int_to_char(decrementInt) + "X");
                    string after = mainStatus[2][col];

                    printf("SLOT MOVED SUCCESSFULLY\n");
                }
                else
                {
                    printf("IMPOSSIBLE TO MOVE THIS SLOT, WE ALREADY HAVE 5 ELEMENTS IN THAT POSITION\n");
                }
            }
            else if (get_latter_from_grapth(mainStatus[2][next_col]) != "X")
            {
                string nextPos = mainStatus[2][next_col];
                char nextNumber = nextPos.at(0);
                int nextInt = char_to_int(nextNumber);

                if (nextInt == 1)
                {
                    broken_dices(mainStatus, 'Y');
                    mainStatus[2][next_col] = (int_to_char(1) + "X");

                    //decrement
                    string actualValue = mainStatus[2][col];
                    char decrement = actualValue.at(0);

                    int decrementInt = char_to_int(decrement);

                    decrementInt = decrementInt - 1;
                    mainStatus[2][col] = (int_to_char(decrementInt) + "X");
                    string after = mainStatus[2][col];
                }
                else
                {
                    printf("IMPOSSIBLE TO PLAY IN THIS SLOT");
                }
            }
        }
        else
        {
            string value2 = mainStatus[0][next_col - 1];
            if (value2 == "0")
            {

                mainStatus[0][next_col - 1] = (int_to_char(1) + "X");

                //decrement
                string actualValue = mainStatus[2][col];

                char decrement = actualValue.at(0);

                int decrementInt = char_to_int(decrement);

                decrementInt = decrementInt - 1;
                mainStatus[2][col] = (int_to_char(decrementInt) + "X");
                string after = mainStatus[2][col];

                printf("SLOT MOVED SUCCESSFULLY\n");
            }
            else if (get_latter_from_grapth(mainStatus[0][next_col - 1]) == "X")
            {
                string nextValue = mainStatus[0][next_col - 1];
                char onlyTheNumber = nextValue.at(0);

                int onlyNumberInt = char_to_int(onlyTheNumber);

                if (onlyNumberInt < 6)
                {
                    onlyNumberInt = onlyNumberInt + 1;
                    mainStatus[0][next_col - 1] = (int_to_char(onlyNumberInt) + "X");
                    string place = mainStatus[0][next_col - 1];

                    //decrement
                    string actualValue = mainStatus[2][col];

                    char decrement = actualValue.at(0);

                    int decrementInt = char_to_int(decrement);

                    decrementInt = decrementInt - 1;
                    mainStatus[2][col] = (int_to_char(decrementInt) + "X");
                    string after = mainStatus[2][col];

                    printf("SLOT MOVED SUCCESSFULLY\n");
                }
                else
                {
                    printf("IMPOSSIBLE TO MOVE THIS SLOT, WE ALREADY HAVE 5 ELEMENTS IN THAT POSITION\n");
                }
            }
            else if (get_latter_from_grapth(mainStatus[0][next_col - 1]) != "X")
            {
                string nextPos = mainStatus[0][next_col - 1];
                char nextNumber = nextPos.at(0);
                int nextInt = char_to_int(nextNumber);

                if (nextInt == 1)
                {
                    broken_dices(mainStatus, 'Y');
                    mainStatus[0][next_col - 1] = (int_to_char(1) + "X");

                    //decrement
                    string actualValue = mainStatus[2][col];

                    char decrement = actualValue.at(0);

                    int decrementInt = char_to_int(decrement);

                    decrementInt = decrementInt - 1;
                    mainStatus[2][col] = (int_to_char(decrementInt) + "X");
                    string after = mainStatus[2][col];

                    printf("SLOT MOVED SUCCESSFULLY\n");
                }
                else
                {
                    printf("IMPOSSIBLE TO PLAY IN THIS SLOT");
                }
            }
        }
    }
}

void move_y(string mainStatus[3][12], int dice, char slot[])
{

    //for y player
    int row;

    int res = char_to_int(slot[1]);

    if (res == 1)
    {
        row = 0;
        int col = letter_to_number(slot[0]);
        int reminder;
        int next_col;
        if (col > dice && col != dice)
        {
            next_col = col - dice;
        }
        else if (col == dice)
        {
            next_col = 0;
        }
        else
        {
            reminder = dice - col;
            next_col = reminder;
        }

        if (reminder == 0)
        {
            string value1 = mainStatus[0][next_col];
            if (value1 == "0")
            {

                mainStatus[0][next_col] = (int_to_char(1) + "Y");

                //decrement
                string actualValue = mainStatus[0][col];

                char decrement = actualValue.at(0);

                int decrementInt = char_to_int(decrement);

                decrementInt = decrementInt - 1;
                mainStatus[0][col] = (int_to_char(decrementInt) + "Y");

                printf("SLOT MOVED SUCCESSFULLY\n");
            }
            else if (get_latter_from_grapth(mainStatus[0][next_col]) == "Y")
            {
                string nextValue = mainStatus[0][next_col];
                char onlyTheNumber = nextValue.at(0);

                int onlyNumberInt = char_to_int(onlyTheNumber);

                if (onlyNumberInt < 6)
                {
                    onlyNumberInt = onlyNumberInt + 1;
                    mainStatus[0][next_col] = (int_to_char(onlyNumberInt) + "Y");

                    //decrement
                    string actualValue = mainStatus[0][col];

                    char decrement = actualValue.at(0);

                    int decrementInt = char_to_int(decrement);

                    decrementInt = decrementInt - 1;
                    mainStatus[0][col] = (int_to_char(decrementInt) + "Y");
                    string after = mainStatus[0][col];

                    printf("SLOT MOVED SUCCESSFULLY\n");
                }
                else
                {
                    printf("IMPOSSIBLE TO MOVE THIS SLOT, WE ALREADY HAVE 5 ELEMENTS IN THAT POSITION\n");
                }
            }
            else if (get_latter_from_grapth(mainStatus[0][next_col]) != "Y")
            {
                string nextPos = mainStatus[0][next_col];
                char nextNumber = nextPos.at(0);
                int nextInt = char_to_int(nextNumber);

                if (nextInt == 1)
                {
                    broken_dices(mainStatus, 'X');
                    mainStatus[0][next_col] = (int_to_char(1) + "Y");

                    //decrement
                    string actualValue = mainStatus[0][col];

                    char decrement = actualValue.at(0);

                    int decrementInt = char_to_int(decrement);

                    decrementInt = decrementInt - 1;
                    mainStatus[0][col] = (int_to_char(decrementInt) + "Y");

                    printf("SLOT MOVED SUCCESSFULLY\n");
                }
                else
                {
                    printf("IMPOSSIBLE TO PLAY IN THIS SLOT");
                }
            }
        }
        else
        {

            string value2 = mainStatus[2][next_col - 1];
            if (value2 == "0")
            {

                mainStatus[2][next_col - 1] = (int_to_char(1) + "Y");
                //decrement
                string actualValue = mainStatus[0][col];

                char decrement = actualValue.at(0);

                int decrementInt = char_to_int(decrement);

                decrementInt = decrementInt - 1;
                mainStatus[0][col] = (int_to_char(decrementInt) + "Y");
                string after = mainStatus[0][col];

                printf("SLOT MOVED SUCCESSFULLY\n");
            }
            else if (get_latter_from_grapth(mainStatus[2][next_col - 1]) == "Y")
            {
                string nextValue = mainStatus[2][next_col - 1];
                char onlyTheNumber = nextValue.at(0);

                int onlyNumberInt = char_to_int(onlyTheNumber);
                if (onlyNumberInt < 6)
                {
                    onlyNumberInt = onlyNumberInt + 1;
                    mainStatus[2][next_col - 1] = (int_to_char(onlyNumberInt) + "Y");
                    string place = mainStatus[2][next_col - 1];

                    //decrement
                    string actualValue = mainStatus[0][col];

                    char decrement = actualValue.at(0);

                    int decrementInt = char_to_int(decrement);

                    decrementInt = decrementInt - 1;
                    mainStatus[0][col] = (int_to_char(decrementInt) + "Y");

                    printf("SLOT MOVED SUCCESSFULLY\n");
                }
                else
                {
                    printf("IMPOSSIBLE TO MOVE THIS SLOT, WE ALREADY HAVE 5 ELEMENTS IN THAT POSITION\n");
                }
            }
            else if (get_latter_from_grapth(mainStatus[2][next_col - 1]) != "Y")
            {
                string nextPos = mainStatus[2][next_col - 1];
                char nextNumber = nextPos.at(0);
                int nextInt = char_to_int(nextNumber);
                //printf("The X value is %d", nextInt);
                if (nextInt == 1)
                {
                    broken_dices(mainStatus, 'X');
                    mainStatus[2][next_col - 1] = (int_to_char(1) + "Y");

                    //decrement
                    string actualValue = mainStatus[0][col];
                    char decrement = actualValue.at(0);

                    int decrementInt = char_to_int(decrement);

                    decrementInt = decrementInt - 1;
                    mainStatus[0][col] = (int_to_char(decrementInt) + "Y");

                    printf("SLOT MOVED SUCCESSFULLY\n");
                }
                else
                {
                    printf("IMPOSSIBLE TO PLAY IN THIS SLOT");
                }
            }
        }
    }

    //end of 1
    else
    {

        int col = letter_to_number(slot[0]);
        row = 2;
        int next_col = dice + col;
        string value1 = mainStatus[2][next_col];
        if (value1 == "0")
        {

            mainStatus[2][next_col] = (int_to_char(1) + "Y");

            //decrement
            string actualValue = mainStatus[2][col];

            char decrement = actualValue.at(0);

            int decrementInt = char_to_int(decrement);

            decrementInt = decrementInt - 1;
            mainStatus[2][col] = (int_to_char(decrementInt) + "Y");

            printf("SLOT MOVED SUCCESSFULLY\n");
        }
        else if (get_latter_from_grapth(mainStatus[2][next_col]) == "Y")
        {
            string nextValue = mainStatus[2][next_col];
            char onlyTheNumber = nextValue.at(0);

            int onlyNumberInt = char_to_int(onlyTheNumber);

            if (onlyNumberInt < 6)
            {
                onlyNumberInt = onlyNumberInt + 1;
                mainStatus[2][next_col] = (int_to_char(onlyNumberInt) + "Y");
                string place = mainStatus[2][next_col];
                //decrement
                string actualValue = mainStatus[2][col];
                char decrement = actualValue.at(0);

                int decrementInt = char_to_int(decrement);
                decrementInt = decrementInt - 1;
                mainStatus[2][col] = (int_to_char(decrementInt) + "Y");
                printf("SLOT MOVED SUCCESSFULLY\n");
            }
            else
            {
                printf("IMPOSSIBLE TO MOVE THIS SLOT, WE ALREADY HAVE 5 ELEMENTS IN THAT POSITION\n");
            }
        }
        else if (get_latter_from_grapth(mainStatus[2][next_col]) != "Y")
        {
            string nextPos = mainStatus[2][next_col];
            char nextNumber = nextPos.at(0);
            int nextInt = char_to_int(nextNumber);

            if (nextInt == 1)
            {
                broken_dices(mainStatus, 'X');
                mainStatus[2][next_col] = (int_to_char(1) + "Y");

                //decrement
                string actualValue = mainStatus[2][col];

                char decrement = actualValue.at(0);

                int decrementInt = char_to_int(decrement);

                decrementInt = decrementInt - 1;
                mainStatus[2][col] = (int_to_char(decrementInt) + "Y");

                printf("SLOT MOVED SUCCESSFULLY\n");
            }
            else
            {
                printf("IMPOSSIBLE TO PLAY IN THIS SLOT");
            }
        }
    }
}

bool checkFlag(string mainStatus[][12], char c, int fromFileX, int fromFileY)
{
    if (c == 'X')
    {
        int total = 0;
        for (int i = 6; i < 12; i++)
        {
            string actualValue = mainStatus[0][i];
            if (actualValue == "0")
            {
                continue;
            }
            char variable = actualValue.at(1);
            if (variable == c)
            {
                char number = actualValue.at(0);
                int numberInt = char_to_int(number);
                total = total + numberInt;
            }
        }
        total = total + fromFileX;
        if (total == 15)
        {
            return true;
        }
    }

    else
    {
        int total = 0;
        for (int i = 6; i < 12; i++)
        {
            string actualValue = mainStatus[2][i];
            if (actualValue == "0")
            {
                continue;
            }
            char variable = actualValue.at(1);
            if (variable == c)
            {
                char number = actualValue.at(0);
                int numberInt = char_to_int(number);
                total = total + numberInt;
            }
        }
        total = total + fromFileY;
        if (total == 15)
        {
            return true;
        }
    }

    return false;
}

void pick_flakes(string mainStatus[][12], int row, int *fromFileX, int *fromFileY)
{
    char var;
    if (row == 0)
    {
        var = 'X';
    }
    else
    {
        var = 'Y';
    }

    if (var == 'X')
    {
        int count = 11;

        if ((mainStatus[row][count] != "0" || mainStatus[row][count] != "X" || mainStatus[row][count] != "0X") && count > 5)
        {
            string value = mainStatus[row][count];
            char valueChar = value.at(0);
            int valueInt = char_to_int(valueChar);
            valueInt = valueInt - 1;
            *fromFileX = *fromFileX + 1;

            mainStatus[row][count] = (int_to_char(valueInt) + "X");
        }
        else if ((mainStatus[row][count] == "0" || mainStatus[row][count] == "X" || mainStatus[row][count] == "0X") && count > 5)
        {
            count = count - 1;
            string value = mainStatus[row][count];
            char valueChar = value.at(0);
            int valueInt = char_to_int(valueChar);
            valueInt = valueInt - 1;
            *fromFileX = *fromFileX + 1;
            mainStatus[row][count] = (int_to_char(valueInt) + "X");
        }
    }
    else
    {
        int count = 11;
        if ((mainStatus[row][count] != "0" || mainStatus[row][count] != "Y" || mainStatus[row][count] != "0Y") && count > 5)
        {
            string value = mainStatus[row][count];
            char valueChar = value.at(0);
            int valueInt = char_to_int(valueChar);
            valueInt = valueInt - 1;
            *fromFileY = *fromFileY + 1;
            mainStatus[row][count] = (int_to_char(valueInt) + "Y");
        }
        else if ((mainStatus[row][count] == "0" || mainStatus[row][count] == "Y" || mainStatus[row][count] == "0Y") && count > 5)
        {
            count = count - 1;
            string value = mainStatus[row][count];
            char valueChar = value.at(0);
            int valueInt = char_to_int(valueChar);
            valueInt = valueInt - 1;
            *fromFileY = *fromFileY + 1;
            mainStatus[row][count] = (int_to_char(valueInt) + "Y");
        }
    }
}

void fromFile(int *fromFileX, int *fromFileY)
{
    FILE *continueGame;
    if ((continueGame = fopen("continueGame.dat", "r")) == NULL)
    {
        printf("maalesef");
    }

    fscanf(continueGame, "%d%d", &fromFileX, &fromFileY);

    fclose(continueGame);
}

void toFile(int a, int b)
{
    FILE *continueGame;
    if ((continueGame = fopen("continueGame.dat", "w")) == NULL)
    {
        printf("maalesef");
    }
    else
    {
        fprintf(continueGame, "%d\n%d", a, b);
        fclose(continueGame);
    }
}

//print the initial state
void initialState(string a[3][12])
{
    FILE *Table;
    if ((Table = fopen("Table.dat", "w")) == NULL)
    {
        printf("LOG.txt file not found \n");
    }
    else
    {

        fprintf(Table, "   A        B          C        D        E        F   ||   G        H        I        J        K        L        \n");
        fprintf(Table, "_________________________________________________________________________________________________________ \n");
        for (int i = 0; i < 5; i++)
        { 
        	string ed =" ";
            if (i == 0)
            {
                for (int j = 0; j < 12; j++)
                {
                    if (j != 6)
                    {
                        fprintf(Table, "|   %s   ", a[i][j].c_str());
                    }
                    else
                    {
                        fprintf(Table, "||  %s   ", a[i][j].c_str());
                    }
                }
                fprintf(Table, "  %d", i + 1);
                fprintf(Table, "\n_________________________________________________________________________________________________________ \n");
            }
            else if (i == 2)
            {
                for (int j = 0; j < 12; j++)
                {
                    if (j != 6)
                    {
                        fprintf(Table, "|   %s   ", a[i - 1][j].c_str());
                    }
                    else
                    {
                        fprintf(Table, "||  %s   ", a[i - 1][j].c_str());
                    }
                }
                fprintf(Table, "  %d", i + 1);
                fprintf(Table, "\n_________________________________________________________________________________________________________ \n");
            }
            else if (i == 4)
            {
                for (int j = 0; j < 12; j++)
                {
                    if (j != 6)
                    {
                        fprintf(Table, "|   %s   ", a[i - 2][j].c_str());
                    }
                    else
                    {
                        fprintf(Table, "||  %s   ", a[i - 2][j].c_str());
                    }
                }
                fprintf(Table, "  %d", i + 1);
                fprintf(Table, "\n_________________________________________________________________________________________________________ \n");
            }
            else
            {
                for (int j = 0; j < 12; j++)
                {
                    if (j != 6)
                    {
                        fprintf(Table, "|   0%s   ",ed.c_str());
                    }
                    else
                    {
                        fprintf(Table, "||   0%s   ",ed.c_str());
                    }
                }
                fprintf(Table, "  %d", i + 1);
                fprintf(Table, "\n_________________________________________________________________________________________________________ \n");
            }
        }
        printf("\n");

        //fprintf( log, "%c %d %d\n", c, a, b );
        fclose(Table);
    }
}


void initialStateConsole(string a[3][12])
{

    printf("   A        B          C        D        E        F   ||   G        H        I        J        K        L        \n");
    printf("_________________________________________________________________________________________________________ \n");
    for (int i = 0; i < 5; i++)
    {
    	string ed =" ";
        if (i == 0)
        {
            for (int j = 0; j < 12; j++)
            {
                if (j != 6)
                {
                    printf("|   %s   ",a[i][j].c_str());
                }
                else
                {
                    printf("||   %s   ", a[i][j].c_str());
                }
            }
            printf("     %d", i + 1);
            printf("\n_________________________________________________________________________________________________________ \n");
        }
        else if (i == 2)
        {
            for (int j = 0; j < 12; j++)
            {
                if (j != 6)
                {
                    printf("|   %s   ", a[i - 1][j].c_str());
                }
                else
                {
                    printf("||   %s   ", a[i - 1][j].c_str());
                }
            }
            printf("%d", i + 1);
            printf("\n_________________________________________________________________________________________________________ \n");
        }
        else if (i == 4)
        {
            for (int j = 0; j < 12; j++)
            {
                if (j != 6)
                {
                    printf("|   %s   ",a[i - 2][j].c_str());
                }
                else
                {
                    printf("||    %s   ",a[i - 2][j].c_str());
                }
            }
            printf("  %d", i + 1);
            printf("\n_________________________________________________________________________________________________________ \n");
        }
        else
        {
            for (int j = 0; j < 12; j++)
            {
                if (j != 6)
                {
                    printf("|   0%s   ",ed.c_str());
                }
                else
                {
                    printf("||   0%s  ",ed.c_str());
                }
            }
            printf("  %d", i + 1);
            printf("\n_________________________________________________________________________________________________________ \n");
        }
    }
    printf("\n");
}
