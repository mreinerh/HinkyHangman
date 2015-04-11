#include "hinky.h"
#include <algorithm>
#include <map>
#include <limits>


//#define FILE_NAME

void playGame(void);
//I chose vector as my data structure
void wordList(vector<string>& current, char c, string& largestkey);
char *filename;
int main(int argc, char* argv[])
{
  
  filename = argv[1];
  //Checks the arguments: one executable, one file name
  if(argc != 2){
    cout << "Usage: [executable hinky] [filename]\n";
    return 0;
  }
  //plays game
	playGame();
	return 0;
}
//These are the game mechanics
void playGame(void)
{
	vector< vector<string> > dict = readDictionary();
	char playagain;
	srand(time(0));
	do
	{
		unsigned int letters,guesses;
		char letter;
		string blanked;
		string guessed;
		string word;
		string largestkey;
		char see;
		//This is so the tester can see how many possible outcomes there are in the input file
		cout << "Do you want to see how many possibilities remain? [Testing Mode] Enter yes or no: ";
		cin >> see;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		see = tolower(see);
		while(see != 'n' && see !='y')
		{
			cout << "Please enter yes or no: ";
			cin >> see;
			see = tolower(see);
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		cout << "How many letters should the word have: ";
		cin >> letters;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		while(letters < 1 || letters > 29)
		{
			cout << "Invalid size: Enter a new size: ";
			cin >> letters;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}	
		
		vector<string> inner = dict.at(letters - 1);
		//Tests whether or not the size of the word exists
		while(inner.size() < 1)
		{
			cout << "There are no words of this size. Enter a new size: ";
			cin >> letters;	
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			inner = dict.at(letters - 1);
		}
		cout << "How many guesses: ";
		cin >> guesses;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		while(guesses < 1)
		{
			cout << "Invalid number of guesses. Enter a new number: ";
			cin >> guesses;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		for(unsigned int i= 0; i < letters; i++)
		{
			blanked += '-';
		}
		cout << endl << endl;
		while(guesses > 0)
		{	
			cout << blanked << endl;
			unsigned int didsomething = 0;
			cout << "Guesses remaining: " << guesses << endl;
			cout << "You have already guessed: " << guessed << endl;
			cout << "Guess a letter: ";
			cin >> letter;
			cin.clear();
			//Got this from stack overflow...idk what this does not gonna lie but am citing the source
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			while(!isalpha(letter) || guessed.find(letter) != string::npos)
			{
				cout << "Enter guess: ";
				cin >> letter;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
			if(tolower(see) == 'y')
			{
				cout << "Words remaining " << inner.size() << endl;
			}
			wordList(inner,letter,largestkey);
			for(unsigned int i = 0; i < letters; i++)
			{
				if(largestkey[i] == letter)
				{
					blanked[i] = letter;
					didsomething = 1;
				}
			}
			//COME BACK TO THIS LINE
			if(!didsomething)
			{	
				cout << "Sorry, there are no " << letter << "'s" << endl;
				guesses--;
			}
			if(guessed.find(letter) == string::npos)
			{
				guessed += letter;
			}
			if(blanked.find("-") == string::npos)
			{
				cout << "You win!" << endl;
				cout << "The word was " << blanked << endl;
				break;
			}
			cout << endl;
			sort(guessed.begin(),guessed.end());
			if(guesses == 0)
			{
				cout << "You lose!" << endl;
				cout << "The word was ";
				word = inner.at(rand() % inner.size());
				cout << word << endl;
			}
		}
		cout << endl << endl;
		cout << "Do you want to play again? Enter yes or no: ";
		cin >> playagain;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		playagain = tolower(playagain);
		while(playagain != 'n' && playagain != 'y')
		{
		  //User has option to play again
			cout << "Please enter yes or no: ";
			cin >> playagain;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
	while(tolower(playagain) != 'n');
	return;

}
//This opens the file and processes the error if anything goes wrong
vector< vector<string> >readDictionary(void)
{
  
	vector< vector<string> > outer;
	ifstream dict;
	string word;
	for(int i = 0; i < 29; i++)
	{
		vector<string> inner;
		outer.push_back(inner);
	}
	//CHANGED THIS LINE
	dict.open(filename);
	if(!dict.is_open())
	{
		cout << "Error opening the dictionary. Exiting." << endl;
		exit(1);
	}
	while(getline(dict,word))
	{		
		if(word.size() > 0)
		{
			vector<string>& inner = outer.at(word.size() - 1);
			inner.push_back(word);
		}
	}
	return outer;
}
//creates a word list of all the words in the file
void wordList(vector<string>& current, char d, string& largestkey)
{
	map<string, vector<string> > outer;
	unsigned int largest = 0;
	string key;
	vector<string> max;
	/*The madness begins...this iterates through the list choosing a word that has all of the chosen letters in the correct
	 position. When the program starts and the user decides the number of letters, all of those words are technically the "chosen" word.
	 When the user selects a letter and there is another word with the same length in the same "family" of words, the word switches. 
	 This goes on until there are no more words contained in the file that the program can switch to. 
	 */
	for(unsigned int i = 0; i < current.size(); i++)
	{
		string key;
		string temp = current.at(i);
		for(unsigned int j = 0; j < temp.size(); j++)
		{
			key += '-';
		}
		for(unsigned int j = 0; j < temp.size(); j++)
		{
			if(temp[j] == d)
			{
				key[j] = d;
			}
		}
		if(outer.find(key) == outer.end())
		{
			vector<string> tempv;
			outer.insert(pair<string, vector<string> >(key,tempv));
		}
		vector<string>& tempv = outer.at(key);
		tempv.push_back(temp);
	}
	//This for loop switch a string with a temp variable (used to change words)
	for(map<string, vector<string> >::iterator iterator = outer.begin(); iterator != outer.end(); iterator++)
	{
		if(iterator->second.size() > largest)
		{
			largest = iterator->second.size();
			max = iterator->second;
			key = iterator->first;
		}
	}
	largestkey = key;
	current = max;
}
