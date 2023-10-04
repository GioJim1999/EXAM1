//Giovanni Jimenez
//10-9-2023
//CMPR 121 - EXAM 1

#include<iostream>
#include<cstring>
#include<fstream>
#include<string>
#include"input.h"
using namespace std;


//Chemistry element sturcuture
struct ChemistryElement 
{
	int atomicNumber = 0;
	double atomicMass = 0.0;
	char symbol[3] = "NA";
	char name[25] = "Unknown";
	int standardState = 0;
	int chemicalGroupBlock = 0;
	double meltingPoint = 0.0; //degrees K
	double boilingPoint = 0.0; //degrees K
	int yearDiscovered = 0;
	char Discoverers[100] = "Unknown";
};


//Function prototypes
void optionA();
void optionAOne(string& fileName);
void optionATwo(string fileName);

void optionB();

void optionC();

void displayElement(ChemistryElement& e);

//Constants
const int NUMBER_OF_ELEMENTS = 118;


int main()
{


	do
	{
		system("cls");
		cout << "\n\tCMPR 121 - Exam 1 (Chapters 12 ... 1) by Giovanni Jimenez (10/9/2023)\n";
		cout << "\t___________________________________________________________________________\n";
		cout << "\n\tA. Advanced Binary File Operations \t(Chapter 12)";
		cout << "\n\tB. Dynamic Allocated Array \t\t(Chapter 9)";
		cout << "\n\tC. Vector (Array) \t\t\t(Chapter 7)";
		cout << "\n\t----------------------------------------------------------------------------\n";
		cout << "\tX. Exit";
		cout << "\n\t----------------------------------------------------------------------------\n";
		
		switch (toupper(inputChar("\tOption:")))
		{
		case 'X':
			exit(0);
			break;
		case 'A':
			optionA();
			break;
		case 'B':
			optionB();
			break;
		case 'C':
			optionC();
			break;
		default:
			cout << "Invalid option try again...";
		}

	} while (true);


	system("pause");
	return 0;
}

void optionA()
{
	string fileName = "0";
	do
	{
		system("cls");
		cout << "\n\tAdvanced Binary File Operations Menu";
		cout << "\n\t___________________________________________________________________________";
		cout << "\n\t1. Read and Display ALL elements from binary data file";
		cout << "\n\t2. Search and update an element by Symbol from binary file";
		cout << "\n\t___________________________________________________________________________";
		cout << "\n\t0. Return to main menu\n";

		switch (inputInteger("\n\tOption: "))
		{
		case 0:
			return;
		case 1:
			optionAOne(fileName);
			break;
		case 2:
		{
			if (fileName == "0")
			{
				cout << "\n\tNo file from option 1 selected!\n";
				system("pause");
				continue;
			}
			optionATwo(fileName);
			break;
		}
		default:
			cout << "Invalid option...";
		}
	} while (true);

	system("pause");
}

void optionATwo(string fileName)
{
	//create a string from the userinput lower it to use it to compare it to the element symbol later.
	string userInput = inputString("\n\tEnter an element symbol to search for: ", false);
	bool foundElement = false;
	int counter = 0;
	string elementSymbol = "";
	userInput = convertLower(userInput.c_str());

	//element struct
	ChemistryElement element;

	//file reading object and open
	fstream file;
	file.open(fileName, ios::in | ios::out | ios::binary);

	//read the first struct in the file
	file.read(reinterpret_cast<char*>(&element), sizeof(element));


	//loop through the file and find the element that matches the user input
	while (!file.eof())
	{
		//create a string and lower it based ont he current element's symbol from the file
		elementSymbol = convertLower(element.symbol);
		
		//compare the userinput to the element symbol found in the file
		//if they match we found the element the user was looking for and break out of the loop
		if (userInput == elementSymbol)
		{
			foundElement = true;
			file.clear();
			break;
		}
		//if they don't match read the next input from the file and start over
		file.read(reinterpret_cast<char*>(&element), sizeof(element));
		counter++;
	}

	//if this bool is true that means we found an element in the file from the user input now we can display it.
	//else tell the user we did not find an element with the symbol they entered
	if (foundElement)
	{
		displayElement(element);
		file.seekp(sizeof(element) * counter, ios::beg);
	}
	else
	{
		cout << "\n\tCould not find element try again..\n";
	}

	file.close();
	system("pause");
}

//TO DO: 
// 1.DISPLAY MENU
// 2.MAKE SWITCH TO EDIT THE ELEMENTS ELEMENTS
// 3.SAVE CHANGES OR NOT SAVE CHANGES DEPENDING ON USER INPUT
// 4.WRITE CHANGES TO FILE.
void editElementMenu(ChemistryElement& e)
{

}

void optionAOne(string& fileName)
{
	fileName = inputString("\n\tInput the file name: ", false);
	fstream file;

	file.open(fileName, ios::in | ios::binary);

	if (!file)
	{
		cout << "\n\tError opening file try again...\n";
		system("pause");
		return;
	}

	ChemistryElement element;
	file.read(reinterpret_cast<char*>(&element), sizeof(element));
	int counter = 1;

	while (!file.eof())
	{
		cout << "\n\tElement (struct) #" << counter << " - " << sizeof(element) << " Bytes";
		displayElement(element);
		file.read(reinterpret_cast<char*>(&element), sizeof(element));
		counter++;
	}
	file.close();

	cout << '\n';
	system("pause");
}

void displayElement(ChemistryElement& e)
{
	cout << "\n\t-----------------------------------------------------------------";
	cout << "\n\tAtomic Number \t\t\t:" << e.atomicNumber;
	cout << "\n\tSymbol \t\t\t\t:" << e.symbol;
	cout << "\n\tName \t\t\t\t:" << e.name;
	cout << "\n\tAtomic Mass \t\t\t:" << e.atomicMass;
	cout << "\n\tChemical Group Block \t\t:" << e.chemicalGroupBlock;
	cout << "\n\tStandard State \t\t\t:" << e.standardState;
	cout << "\n\tMelting Point \t\t\t:" << e.meltingPoint;
	cout << "\n\tBoiling Point \t\t\t:" << e.boilingPoint;
	cout << "\n\tYear Discovered \t\t:" << e.yearDiscovered;
	cout << "\n\tDiscovered by \t\t\t:" << e.Discoverers;
	cout << "\n\n";
}

void optionB()
{
	system("cls");
	cout << "\n\tDynamic Allocated Array Menu";
	cout << "\n\t___________________________________________________________________________";
	cout << "\n\t1. Read, store ALL elements from binary file into the sorted array";
	cout << "\n\t2. Display ALL elements from the array";
	cout << "\n\t3. Search and update an element by Atomic Number from the array";
	cout << "\n\t4. Write ALL elements from the array into the binary file";
	cout << "\n\t___________________________________________________________________________";
	cout << "\n\t0. return to main menu";
	cout << "\n\t___________________________________________________________________________\n";

	system("pause");
}

void optionC()
{
	system("cls");
	cout << "\n\tVector (array) Menu";
	cout << "\n\t___________________________________________________________________________";
	cout << "\n\t1. Read and store ALL elements from binary file into the unsorted vector (array)";
	cout << "\n\t2. Display ALL elements from the vector";
	cout << "\n\t3. Search and update an element by Name from the vector";
	cout << "\n\t4. Write ALL elements from the vector into the binary file";
	cout << "\n\t___________________________________________________________________________";
	cout << "\n\t0. Return to main menu";
	cout << "\n\t___________________________________________________________________________\n";

	system("pause");
}