/*
Name: Daniel Yatali

*/
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <stdlib.h>
#include <ctime>
#include <windows.h>
#include <stdio.h>
#include <conio.h>

using namespace std;

//These values were preset to allow easy differenciation between visibility
//Compiler gives a warning sometimes.
struct Display_Cases{
	int num;
	int visibility = 0;
	string mask = "$$$$$$";
};
//Struct for cases
//Started of with these 2 arrays when I realised it would be better to declare it as a stuct and have a struct array it was too late.
struct Case{
	int cases_values[50];
	Display_Cases display_cases[50];
};
//Bonus question struct
struct Question{
	string question;
	string choices[4];
	string answer;
};


int read_file(string file_name, int cases[], int rounds[], int num_rounds);
void display_all_cases(Case cases, int n, int selector);
bool check_choice(int choice, Display_Cases display_cases[], int num_cases);
void shuffle(int cases[], int shuffled_cases[], int num_cases);
bool shuffle_check(int shuffled_cases[], int n, int length);
float deal(Case cases, float num_cases, float round_number, float nums_rounds);
int count_digits(int n);
int display_all(Case cases, int num_cases);
int return_first_digit(int n);
int bonus_event(Case cases, int num_cases,int shown_questions[], int count, Question questions[], int num_questions, int selector);
int read_questions(Question questions[]);
void diplay_question(Question questions);
bool contains_question(int shown_questions[], int count, int index);
bool deal_handler(Case cases,int num_cases, float monetary);



int main(){
	int i = 0, num_cases, num_rounds = 0, cases0[50], rounds[50], shown_questions[50], shown_questions_count = 0;
	int selector = 0;
	Case cases;
	Question questions[50];
	int question_rate;
	srand(time(NULL));
	
	//Reading number of questions for bonus(There is a total of 10 questions)
	int num_questions = read_questions(questions);

    
	
	string file_name = "Data", n;
	string response;
	cout << "\t\t\t\t" <<"||============================||\n";
	cout << "\t\t\t\t" <<"||D E A L  O R   N O  D E A L ||\n"; 
	cout << "\t\t\t\t" <<"||============================||\n\n";
	PlaySound("start.wav", NULL, SND_FILENAME);
	cout << "How many cases would you like the game to have: ";
	cin >> n;
	
	while( n != "16" && n != "18" && n != "20" && n != "22" && n != "24" && n != "26"){
		PlaySound("error.wav", NULL, SND_FILENAME);
		cout << "You entered an invalid number of cases. Try Again: ";
		cin >> n;
	}
	
	//String for file name is concatenate based on number of cases entered.
	file_name += n + ".txt";
	
	//the string entered is converted to a number to be used as number of cases
	stringstream int_n(n); 
	int_n >> num_cases;
	
	PlaySound("goodluck.wav", NULL, SND_FILENAME);
	
	//Reading case data from file
	num_rounds = read_file(file_name, cases0, rounds, num_cases);
	
	if(num_rounds == -1){
		cout << "Error Opening files! Aborting..";
		return 1;
	}
	
	//This functions shuffles the cases values and places them in the struct
	shuffle(cases0, cases.cases_values, num_cases);

    //Creating dummy indexes for display
	for(int i = 0; i < num_cases; i++){
		cases.display_cases[i].num = i +1;
	}
	
	int round_number = 0, first_choice, choice;
	
	//Generates Game Board
	display_all_cases(cases, num_cases, 0);

	cout << "\n\nPlease pick a Case from the above section: ";
	cin >> first_choice;
	
	//Invalid choice checker
	while(!check_choice(first_choice, cases.display_cases, num_cases)){
		PlaySound("error.wav", NULL, SND_FILENAME);
		cout << "Invalid Case Selection: ";
		cin >>first_choice;
	}
	
	//Changing the mask of the first choice so that is cannot be accessed by the player
	cases.display_cases[first_choice - 1].mask = "######";
	cout << "\n";
	system("pause");
	system("cls");
	
	//GAME LOOP
	for(round_number = 0; round_number < num_rounds; round_number++){
		
		for(int i = 0; i < rounds[round_number]; i++){
			
			question_rate = rand() % 5; //Generates a random number for the 1st bonus
			int question_rate2 = rand() % 8;//Generates a random number for the 2nd bonus
			
			//FINAL ROUND
			if(round_number + 1 == num_rounds){
				cout << "F I N A L  R O U N D\n\n";
				PlaySound("start.wav", NULL, SND_FILENAME);
				display_all_cases(cases, num_cases, 0);
				cout << "\nWill you keep your first selected case OR will you switch with the final case?\n";
				cout << "Answer: ";
				cin >> response;
				
				//Allows the player to either keep their first selected case or switch with the last case
				while(response != "keep" && response != "switch"){
					PlaySound("error.wav", NULL, SND_FILENAME);
					cout << "Error. Try again (keep or switch): ";
					cin >>response;
				}
				
				cout << "\n";
				system("pause");
				system("cls");
				
				//Displays all the cases and all the cases contents
				int final_choice = display_all(cases, num_cases);
				cout <<"\n";
				if(response == "keep"){
					cout << "Congratulation you won: $" << cases.cases_values[first_choice - 1] << "\n";
					if(cases.cases_values[first_choice - 1] <= 1000){
						PlaySound("ohno.wav", NULL, SND_FILENAME);
					}
					else{
						PlaySound("spongebob.wav", NULL, SND_FILENAME);
					}
	
				}
				else{
					cout << "Congratulation you won: $" << cases.cases_values[final_choice] << "\n";
					if(cases.cases_values[final_choice] <= 1000){
						PlaySound("ohno.wav", NULL, SND_FILENAME);
					}
					else{
						PlaySound("spongebob.wav", NULL, SND_FILENAME);
					}
				}
				
			}
			else{
				//BONUS QUESTION
				if(question_rate == 2 || question_rate2 == 1 ){
					
					//20% chance that you will get this bonus
					if(question_rate == 2 && shown_questions_count < num_questions){
						selector = 1; //The selector decides which bonus reward happens.
						
						//Generates bonus question
						choice = bonus_event(cases, num_cases, shown_questions, shown_questions_count, questions, num_questions, selector);
						if(choice != -1){
							int n = return_first_digit(cases.cases_values[choice-1]);
							cases.display_cases[choice-1].visibility = n;
					    }
					system("pause");
					system("cls");	
					}
					
					//12.5% chance of this bonus occuring
					else if(question_rate2 == 1 && shown_questions_count < num_questions){
						selector = 2; //The selector decides which bonus reward happens.
						
						//Generates bonus question
						choice = bonus_event(cases, num_cases, shown_questions, shown_questions_count, questions, num_questions, selector);
					    system("pause");
					    system("cls");
					}
					shown_questions_count++;
			   }
			   	//NORMAL ROUNDS
				cout << "R O U N D   N U M B E R: "<< round_number + 1 << "\n";
				cout << "\nPlease Choose "<< rounds[round_number] << " Cases: \n";
				
			    display_all_cases(cases, num_cases, 0);
				cout << "\n\nCase "<< i+1 << " : ";
				cin >> choice;
				
				while(!check_choice(choice, cases.display_cases, num_cases)){
					PlaySound("error.wav", NULL, SND_FILENAME);
					cout << "Invalid Case Selection: ";
					cin >>choice;
				}
				PlaySound("case_open.wav", NULL, SND_FILENAME);
				cout << "Value : $" << cases.cases_values[choice-1] << "\n\n"; 
				
				//If case value is more than $400, 000 it is considered a bad case because you have eliminated a huge sum of money from the game
				//The object should be to open the cases with little amounts of money.
				
				if(cases.cases_values[choice-1] >= 1000000){ //If 1 million or higher is eliminated it's the worst case scenario.
					PlaySound("ohno.wav", NULL, SND_FILENAME);
				}
				else if(cases.cases_values[choice-1] > 400000){
					PlaySound("bad_case.wav", NULL, SND_FILENAME);
				}
			
				//-1 visibility means, the case value will be shown
				//0 means value will not be shown
				
				cases.display_cases[choice - 1].visibility = -1;
				system("pause");
				system("cls");
				}
		}//end for loop
		//DEALS
		if(round_number + 1 < num_rounds){
			cout << "Deal or No Deal?\n\n";
			PlaySound("bank.wav", NULL, SND_FILENAME);
			
			//Calculates the offer
			float monetary = deal(cases, num_cases,round_number + 1, num_rounds);
			
			//This function generates the deal
			bool stop = deal_handler(cases,num_cases, monetary);
			if(stop){
				break;
			}	
		}
		system("pause");
		system("cls");
	}//end for loop
}
//This function is called at the end of each round except the last round to generate a deal.
bool deal_handler(Case cases,int num_cases, float monetary){
	string response;
	int n = 0;
	
	cout << "The Bank has Offered you: $" << monetary << "\n";
	cout << "Deal(y) OR No Deal(n): ";
	
    cin.ignore();//checks for a /n from previous "cin"
    
	getline(cin, response);
	
	while(response != "y" && response != "n"){
		PlaySound("error.wav", NULL, SND_FILENAME);
		cout << "\nDeal(y) OR No Deal(n): ";
		getline(cin, response);
		} 
		
	if(response == "y"){
		n = display_all(cases, num_cases);
		cout << "Congratulation you won: $" << monetary << "\n";
		PlaySound("spongebob.wav", NULL, SND_FILENAME);
		return true;
		}
		
	else{
		PlaySound("nothanks.wav", NULL, SND_FILENAME);
		cout << "\nI'll Get you a better offer next time!\n";
		return false;
		}
}

//This function checks if a question was already displayed to the player.
//It ensures that no duplicate bonus questions are shown.
bool contains_question(int shown_questions[], int count, int index){
	for(int i = 0; i < count; i++){
		if(shown_questions[i] == index){
			return true;
		}
	}
	shown_questions[count] = index;
	return false;
}

//This functions prints the bonus question to the screen
void diplay_question(Question questions){
	cout << "\nQ U E S T I O N\n\n";
	cout << questions.question << "\n";
	cout << questions.choices[0] << "\n";
	cout << questions.choices[1] << "\n";
	cout << questions.choices[2] << "\n";
	cout << questions.choices[3] << "\n";
}

//This function reads all question in the bonus question file
int read_questions(Question questions[]){
	ifstream input;
	string n;
	int i = 0;
	input.open("Bonus.txt");
	if(!input.is_open()){
		cout << "Error File not open!";
		return -1;
	}
	getline(input, n);
	while(n != "END"){
	    questions[i].question = n;
		getline(input, questions[i].choices[0]);
		getline(input, questions[i].choices[1]);
		getline(input, questions[i].choices[2]);
		getline(input, questions[i].choices[3]);
		getline(input, questions[i].answer);
		getline(input, n);
		getline(input, n);
		i++;
	}
	return i;
	
}

//This function generates the bonus event that happens randomly.
//There are 2 Bonus events.
//1st bonus 20% of occuring each round.
//If the player answers the question correctly the player can select a case and the first digit of that case's value is displayed
//This value remains visible throughout the game

//2nd bonus 12.5% of occuring
//If the player answers the question correctly the player can select a case.
//The gameboard is returned and the case's value is visible until the player presses enter
//After which the value will not be visible
//The player can use these bonuses to their advantage to win the 1 million dollar case.
int bonus_event(Case cases, int num_cases,int shown_questions[], int count, Question questions[], int num_questions, int selector){
	string response;
	srand(time(NULL));
	string message;
	int random_index;
	int choice;
	cin.ignore();
	
	random_index = rand() % num_questions;
	while(contains_question(shown_questions, count, random_index)){
		random_index = rand() % num_questions;
	}
	if(selector == 1){
        message = "If you answer this question correctly you will be allowed to view the first digit of a case's value!\nThis value will remain visible\n";
	}
	else{
		message = "If you answer this question correctly you will be allow to view a case's value!\n";	
	}

	cout << "B O N U S   E V E N T!!\n\n";
	PlaySound("wow.wav", NULL, SND_FILENAME);
	cout << message;
	diplay_question(questions[random_index]);
	cout << "\nAnswer: ";
	getline(cin, response);
	
	while(response != "a" && response != "b" && response != "c" && response != "d"){
		PlaySound("error.wav", NULL, SND_FILENAME);
		cout << "Try again: ";
		cin >> response;
	}
	if(response == questions[random_index].answer){
		cout << "\n";
		display_all_cases(cases, num_cases, 0);
		PlaySound("applause.wav", NULL, SND_FILENAME);
		cout << "Yay you got it correct now please select a case: ";
		cin >> choice;
	    while(!check_choice(choice, cases.display_cases, num_cases)){
	    	PlaySound("error.wav", NULL, SND_FILENAME);
			cout << "Invalid Case Selection: ";
			cin >>choice;
		}
		//This selector is use to decide what happens
		if(selector == 1){
			return choice;
		  }
		else{
			//Bonus 2, changes the cases visibility so it can be see
			//However it is reverted back to 0 on the click of enter.
			//One can technically say it was not changed.
			cases.display_cases[choice-1].visibility = -2;
			system("cls");
			cout << "Case Value for " <<choice<<" is open.";
			cout << "\nWhen enter is pressed this value will no longer be visible.\n";
		    display_all_cases(cases, num_cases, 1);
		    cout << "\nCase Value of "<< choice << ": $" << cases.cases_values[choice - 1] << "\n\n";
		    return 0;
		}
	}
	random_index = 1 + rand() % 2;
	//If player fails to anwser the question correctly
	cout << "\nFailure You don't deserve this bonus!\n";
	if(random_index == 1){
		PlaySound("failed.wav", NULL, SND_FILENAME);
	}
	else{
		PlaySound("mission_failed.wav", NULL, SND_FILENAME);
	}
	return -1;	
		
}

//Returns the first digit in a number.
int return_first_digit(int n){
	while (n >= 10){
	  n /= 10;
    }
    return n;
}

//Displays all cases and all case values to the board.
int display_all(Case cases, int num_cases){
	int n = 0;
	for(int i = 0; i < num_cases; i++){
		if(cases.display_cases[i].visibility != -1 && cases.display_cases[i].mask == "$$$$$$"){
			n = i;
		}
		cases.display_cases[i].visibility = -1;
		
	}
	display_all_cases(cases, num_cases, 0);
	return n;
	
}
//Calculates deal offer
float deal(Case cases, float num_cases, float round_number, float nums_rounds){
	float average = 0;
	int sum = 0;
	int counter = 0;
	for(int i = 0; i < num_cases; i++){
		if(cases.display_cases[i].visibility != -1){
			sum += cases.cases_values[i];
			counter++;
		}
	}

	average = sum / counter;

	average *= (round_number*1.0/nums_rounds*1.0);
	return average;
	
}
//Shuffles Cases
void shuffle(int cases[], int shuffled_cases[], int num_cases){
	srand(time(NULL));
	int n = 0;
	for(int i = 0; i < num_cases; i++){
		n = rand() % num_cases;
		while(!shuffle_check(shuffled_cases, cases[n], i )){
			n = rand() % num_cases;
		}
		shuffled_cases[i] = cases[n];
	}
}
//Checks if the case index is not already present in the shuffled cases array.
bool shuffle_check(int shuffled_cases[], int n, int length){
	for(int i = 0; i < length; i++){
		if(shuffled_cases[i] == n){
			return false;
		}
	}
	return true;
}
//Validates case choices
bool check_choice(int choice, Display_Cases display_cases[], int num_cases){
	if(choice < 1 || choice > num_cases){
		return false;
	}
	if(display_cases[choice-1].visibility != -1 && display_cases[choice-1].mask == "$$$$$$" ) 
		return true;
	return false;
}
//Counts the number of digits in a number
int count_digits(int n){
	int count = 0;
	while (n != 0) {
        n /= 10;     
        count++;
    }
    return count;
}

//Displays Game Board
void display_all_cases(Case cases, int n, int selector){
	int line = 0;
	cout<<"\n---------------------------------------------------------------------------------\n| ";
	for(int i = 0; i < n; i++){
		line++;
		//Check visibility of cases and prints accordingly
		//Visibility 0 and mask $$$$$$ prints: prints index and mask
		//Visibility -1 : prints index and value
		//Visibility -2: prints that value and case number alone
		//Those with neither 0 or -1 get a mask of 5 $s this leaves room for the first digit to be infront
		if(selector == 0){
			if(cases.display_cases[i].visibility != -1 && cases.display_cases[i].visibility != 0){
			cout << cases.display_cases[i].num << "\t";
			cout << cases.display_cases[i].visibility << "$$$$$\t| ";
			}
			else if(cases.display_cases[i].visibility == -1){
				cout << cases.display_cases[i].num << "\t";
				cout << cases.cases_values[i]  << "\t| ";
			}
			else if(cases.display_cases[i].visibility == 0){
				cout << cases.display_cases[i].num << "\t";
				cout << cases.display_cases[i].mask << "\t| ";	
			}	
		}
		else{
			if(cases.display_cases[i].visibility == -2){
				cout << cases.display_cases[i].num << "\t";
			    cout << cases.cases_values[i]  << "\t| ";
			}
			else{
				cout <<" \t";
				cout << " \t|";
			}
			
		}
	
		if(line == 5 && i + 1 < n){
			cout << "\n| ";
			line = 0;
		}
	}
	cout<<"\n---------------------------------------------------------------------------------\n";
}
//Reading file
int read_file(string file_name, int cases[], int rounds[], int num_rounds){
	ifstream input;
	input.open(file_name.c_str()); //This conversion was learnt in COMP 1603
	
	if(!input.is_open()){
		return -1;
	}
	int i = 0;
	while( i < num_rounds){
		input >> cases[i];
		i++;
	}
	input >> num_rounds;
	//cout << num_rounds;
	i = 0;
	while(!input.eof()){
		input >> rounds[i];
		i++;
	}
	
	return i -1;	
}
