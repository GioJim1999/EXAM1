//Giovanni Jimenez
//10-9-2023
//CMPR 121 - EXAM 1

#include<iostream>
#include<cstring>
#include<fstream>
#include<string>
#include"input.h"
#include<iomanip>
#include<Windows.h>
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
//Option A functions
void optionA();
void optionAOne(string& fileName);
void optionATwo(string fileName);

//Option B Functions
void optionB();
void createElementsArray(ChemistryElement* elements, int length);
bool checkArrayEmpty(ChemistryElement* elements, int length);
int searchAtomicNumber(ChemistryElement* elements, int length, int AtomicNumber);

//Functions that edit the Chemistry Element struct
void displayElement(ChemistryElement& e);
void editElementMenu();
ChemistryElement editElement(ChemistryElement& e);
double convertToCelcius(double kelvin);


//Periodic Table of elements display functions
void displayTableNumbers();
void displayTableSymbols();

//Constants
const int NUMBER_OF_ELEMENTS = 118;
const string ChemicalGroupBlock[] = { "unknown", "Nonmetal", "Noble Gas", "Alkali Metal", "Alkaline Earth Metal", "Transition Metal", "Post-transistion Metal", "Metalloid", "Halogen", "Lanthanide", "Actinide"};
const string StandardState[] = { "unknown", "Gas", "Liquid", "Solid"};
enum Colors { blue = 9, green = 10, cyan = 11, pink = 12, violet = 13, yellow = 14, white = 15, grey = 8, brown = 6, red = 4 };


int main()
{
	do
	{
		system("cls");
		displayTableSymbols();
		cout << "\n\tCMPR 121 - Exam 1 (Chapters 12 ... 1) by Giovanni Jimenez (10/9/2023)\n";
		cout << "\t___________________________________________________________________________\n";
		cout << "\n\tA. Advanced Binary File Operations \t(Chapter 12)";
		cout << "\n\tB. Dynamic Allocated Array \t\t(Chapter 9)";
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
		default:
			cout << "Invalid option try again...";
		}

	} while (true);


	system("pause");
	return 0;
}

//Code for Option A
void optionA()
{
	string fileName = "0";
	do
	{
		system("cls");
		displayTableSymbols();
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

//Code for Option A-2
//Precondition: fileName (String)
//Postcondition: Asks user for an element symbol searches the file to find the element 
//Then allows the user to either update or not update the element's data
//Writes it back to the file at the location the element was found.
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
		system("cls");
		displayElement(element);
		//TODO MAYBE THIS DOESNT BELONG HERE?
		element = editElement(element);
		file.seekp(sizeof(element) * counter, ios::beg);
		file.write(reinterpret_cast<char*>(&element), sizeof(element));
		file.clear();
	}
	else
	{
		cout << "\n\tCould not find element try again..\n";
		system("Pause");
	}

	file.close();
}

//Display edit Element Menu
void editElementMenu()
{
	cout << "\n\tEdit Element Properties Menu";
	cout << "\n\t------------------------------------------------";
	cout << "\n\t1. Atomic Number";
	cout << "\n\t2. Symbol";
	cout << "\n\t3. Name";
	cout << "\n\t4. Atomic Mass";
	cout << "\n\t5. Chemical Group Block";
	cout << "\n\t6. Standard State";
	cout << "\n\t7. Melting Point";
	cout << "\n\t8. Boiling Point";
	cout << "\n\t9. Discovered Year";
	cout << "\n\t10. Discovered By";
	cout << "\n\t------------------------------------------------";
	cout << "\n\t-1. Return without committing the updates";
	cout << "\n\t0. Commit the updates and return";
	cout << "\n\t------------------------------------------------\n";
}

//Precondition: e (ChemistryElement)
//Postcondition: Creates a temporary Element to store the updated version of the element the user passed in, if they press 0 the 
//updated Chemistry Element is returned so that it can be updated, if -1 it returns the old NOT updated Chemistry Element to be 
//written to the file hence not actually overwritting anything.
ChemistryElement editElement(ChemistryElement& e)
{
	ChemistryElement temp = e;
	string userInput = "";

	do
	{
		editElementMenu();
		switch (inputInteger("\n\tOption: ",-1,10))
		{
		case 0:
		{
			cout << "\n\tElement has been updated!\n";
			system("pause");
			return temp;
		}
		case -1:
		{
			cout << "\n\tElement has NOT been updated!\n";
			system("pause");
			return e;
		}
		case 1:
			temp.atomicNumber = inputInteger("\n\tEnter the new Atomic Number: ", 1, 118);
			break;
		case 2:
		{
			userInput = inputString("\n\tEnter the new Symbol: ", false);
			if (userInput.length() > 2)
			{
				cout << "\n\tNew Symbol cannot be larger than 2 characters...\n";
				system("pause");
				break;
			}
			else
			{
				strcpy_s(temp.symbol, userInput.c_str());
				break;
			}
		}
		case 3:
		{
			userInput = inputString("\n\tEnter the new Name: ", false);
			if (userInput.length() > 24)
			{
				cout << "\n\tNew Name cannot be longer than 24 characters...\n";
				system("pause");
				break;
			}
			else
			{
				strcpy_s(temp.name, userInput.c_str());
				break;
			}
		}
		case 4:
			temp.atomicMass = inputDouble("\n\tEnter the new Atomic Mass: ", true);
			break;
		case 5:
			temp.chemicalGroupBlock = inputInteger("\n\tEnter the Chemical Group Block (0 - Unknown, 1 - Nonmetal, 2 - Noble Gas, 3 - Alkali Metal, 4 - Alkaline Earth Metal, 5 - Transition Metal, 6 - Post-Transition Metal, 7 - Metalloid, 8 - Halogen, 10 - Lanthanide, or 11 - Actinide): ", 0, 11);
			break;
		case 6:
			temp.standardState = inputInteger("\n\tEnter the Standard State (0 - Unknown, 1 - Gas, 2 - Liquid, 3 - Solid): ", 0,3);
			break;
		case 7: 
			temp.meltingPoint = inputDouble("\n\tEnter the new Melting Point (K): ", true);
			break;
		case 8: 
			temp.boilingPoint = inputDouble("\n\tEnter the new Boiling Point (K): ", true);
			break;
		case 9:
			temp.yearDiscovered = inputInteger("\n\tEnter the discovery year (0...3000): ", 0, 3000);
			break;
		case 10:
			userInput = inputString("\n\tEnter the name of the discoverer: ", true);
			if (userInput.length() > 99)
			{
				cout << "\n\tName of Discoverer cannot be longer than 99 characters...\n";
				break;
			}
			else 
			{
				strcpy_s(temp.Discoverers, userInput.c_str());
				break;
			}
		}
	} while (true);
}

//Code for Option A-1
//Precondition: fileName (String)
//Postcondition: Reads the file from fileName and outputs the data onto the screen.
void optionAOne(string& fileName)
{
	fileName = inputString("\n\tInput the file name: ", false);
	fstream file;

	file.open(fileName, ios::in | ios::binary);

	if (!file)
	{
		cout << "\n\tError opening file try again...\n";
		fileName = "0";
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

//Precondition: e (ChemistryElement)
//Postcondition: Displays the input Chemistry Element data with proper formatting
void displayElement(ChemistryElement& e)
{
	cout << "\n\t-----------------------------------------------------------------";
	cout << "\n\tAtomic Number" << setw(20) << ":" << e.atomicNumber;
	cout << "\n\tSymbol" << setw(27) << ":" << e.symbol;
	cout << "\n\tName" << setw(29) << ":" << e.name;
	cout << "\n\tAtomic Mass" << setw(22) << setprecision(5) << showpoint << fixed << ":" << e.atomicMass;
	cout << "\n\tChemical Group Block" << setw(13) << ":" << ChemicalGroupBlock[e.chemicalGroupBlock];
	cout << "\n\tStandard State" << setw(19) << ":" << StandardState[e.standardState];
	cout << "\n\tMelting Point" << setw(20) << setprecision(2) << showpoint << fixed << ":" << convertToCelcius(e.meltingPoint) << "\370C (" << e.meltingPoint << " K)";
	cout << "\n\tBoiling Point" << setw(20) << setprecision(2) << showpoint << fixed << ":" << convertToCelcius(e.boilingPoint) << "\370C (" << e.boilingPoint << " K)";
	cout << "\n\tYear Discovered" << setw(18) << ":" << e.yearDiscovered;
	cout << "\n\tDiscovered by"	<< setw(20) << ":" << e.Discoverers;
	cout << "\n\n";
}


//Code for option B
void optionB()
{
	ChemistryElement* elements = new ChemistryElement[NUMBER_OF_ELEMENTS];

	do
	{
		system("cls");
		displayTableNumbers();
		cout << "\n\tDynamic Allocated Array Menu";
		cout << "\n\t___________________________________________________________________________";
		cout << "\n\t1. Read, store ALL elements from binary file into the sorted array";
		cout << "\n\t2. Display ALL elements from the array";
		cout << "\n\t3. Search and update an element by Atomic Number from the array";
		cout << "\n\t4. Write ALL elements from the array into the binary file";
		cout << "\n\t___________________________________________________________________________";
		cout << "\n\t0. return to main menu";
		cout << "\n\t___________________________________________________________________________\n";

		switch (inputInteger("\n\tOptions: "))
		{
		case 0:
			delete[] elements;
			return;
		case 1:
			createElementsArray(elements, NUMBER_OF_ELEMENTS);
			break;
		case 2:
		{
			if (checkArrayEmpty(elements, NUMBER_OF_ELEMENTS))
			{
				cout << "\n\tYou have not done option 1 to store the array with elements please choose option 1 first...\n";
				system("pause");
			}
			else
			{
				for (int i = 0; i < NUMBER_OF_ELEMENTS; i++)
				{
					cout << "\tArray Element [" << to_string(i) << "]";
					displayElement(*(elements + i));
				}
				system("pause");
			}
			break;
		case 3:
			if (checkArrayEmpty(elements, NUMBER_OF_ELEMENTS))
			{
				cout << "\n\tYou have not done option 1 to store the array with elements please choose option 1 first...\n";
				system("pause");
			}
			else
			{
				int userAtomicNumber = inputInteger("\n\tEnter an Atomic Number you wish to search for:	");
				int elementIndex = searchAtomicNumber(elements, NUMBER_OF_ELEMENTS, userAtomicNumber);
				if (elementIndex != -1)
				{
					displayElement(*(elements + elementIndex));
					*(elements + elementIndex) = editElement(*(elements + elementIndex));
				}
				else
				{
					cout << "\n\tCould not find that element in the array try again...\n";
					system("pause");
				}
			}
			break;
		case 4:
			if (checkArrayEmpty(elements, NUMBER_OF_ELEMENTS))
			{
				cout << "\n\tYou have not done option 1 to store the array with elements please choose option 1 first...\n";
				system("pause");
			}
			else
			{
				string fileName = inputString("\n\tEnter the new file name ending in .dat or .bin: ", false);
				fstream file;
				file.open(fileName, ios::out | ios::binary);
				for (int i = 0; i < NUMBER_OF_ELEMENTS; i++)
				{
					file.write(reinterpret_cast<char*>(&*(elements + i)), sizeof(ChemistryElement));
				}
				cout << "\n\tFile has been created and written to...\n";
				system("pause");
				file.close();
			}
		}
		default:
			continue;
		}
	} while (true);

	system("pause");
}

//Precondition: elements (Array ChemistryElements), length (Integer), userAtomicNumber (Integer)
//Postcondition: returns the index at which the user's desired number was found or -1 if it wasn't found
int searchAtomicNumber(ChemistryElement* elements, int length, int userAtomicNumber)
{
	for (int i = 0; i < length; i++)
	{
		if ((elements + i)->atomicNumber == userAtomicNumber)
			return i;
	}
	return -1;
}

//Precondition: elements (Array ChemistryElements), length (integer)
//Postcondition: Returns true if the array is empty, false if its not empty
bool checkArrayEmpty(ChemistryElement* elements, int length)
{
	int numberOfEmtpyElements = 0;
	for (int i = 0; i < length; i++)
	{
		if ((elements + i)->atomicNumber == 0)
		{
			numberOfEmtpyElements++;
		}
		if (numberOfEmtpyElements == 118)
			return true;
	}
	return false;
}

//Precondition: elements (Array ChemistryElements), Length (const int)
//Postcondition: Fills the elements array with data from what ever file the user entered, or outputs that the file doesn't exist
void createElementsArray(ChemistryElement* elements, const int length)
{
	//get user input for file name
	string fileName = inputString("\n\tEnter the file name: ", false);
	
	//create file object using fileName
	fstream file;
	file.open(fileName, ios::in | ios::binary);

	//Check to see if file exists if not just return say file not here 
	if (!file)
	{
		cout << "\n\tFailed to open file...\n";
		system("pause");
		return;
	}

	int counter = 0;

	//read file and store every struct into the array
	while (!file.eof())
	{
		file.read(reinterpret_cast<char*>(&*(elements + counter)), sizeof(ChemistryElement));
		counter++;
	}

	file.close();

	//Sort the array by Atomic Number 
	for (int j = 0; j < length - 1; j++)
	{
		int iMin = j;
		for (int i = j + 1; i < length; i++)
		{
			if (elements[i].atomicNumber < elements[iMin].atomicNumber)
				iMin = i;
		}
		if (iMin != j)
			swap(elements[j], elements[iMin]);
	}

	cout << "\n\tSUCCESS: " << to_string(counter - 1) << "(structs) data from " << fileName << " have been stored into the dynamically allocated array and have been sorted by atomic number!\n";

	system("pause");
}

//Precondition: kelvin (Double)
//Postcondition: Returns the Celcius converison of the input Kelvin
double convertToCelcius(double kelvin)
{
	return kelvin - 273.15;
}


//CODE FOR TABLE OUTPUT AND COLOR CHANGES BELOW HERE--------------------------------------------------------------------------

//Displays the periodic table of elemnts by number
//All the if statements checks to see where the element is on the table
//and changes its color whether its a range of elements or a single element
void displayTableNumbers()
{
	cout << "\n\t";

	//used to set console color from the windows.h header file
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);


	//Outputs elements 1 - 4 with a huge space in between 1 and 2 and 4 and 5
	cout << "\n\t\t\t\tPERIODIC TABLE OF ELEMENTS BY ATOMIC NUMBER\n";
	int num = 1;
	cout << "\n\t";
	SetConsoleTextAttribute(h, yellow);
	cout << "[" << setw(3) << to_string(num++) << "]";
	cout << setw(81);
	SetConsoleTextAttribute(h, pink);
	cout << "[" << setw(3) << to_string(num++) << "]";
	cout << "\n\t";
	SetConsoleTextAttribute(h, cyan);
	cout << "[" << setw(3) << to_string(num++) << "]";
	SetConsoleTextAttribute(h, red);
	cout << "[" << setw(3) << to_string(num++) << "]";
	cout << setw(51);

	//Rows for numbers 5 to 10
	for (int i = 0; i < 6; i++)
	{
		if (num == 5)
			SetConsoleTextAttribute(h, blue);
		if (num > 5 && num < 9)
			SetConsoleTextAttribute(h, yellow);
		if (num == 9)
			SetConsoleTextAttribute(h, white);
		if (num == 10)
			SetConsoleTextAttribute(h, pink);
		cout << "[" << setw(3) << to_string(num++) << "]";
	}
	cout << "\n\t";

	//Outputs elements  11 and 12 and a huge space between 12 and 13
	SetConsoleTextAttribute(h, cyan);
	cout << "[" << setw(3) << to_string(num++) << "]";
	SetConsoleTextAttribute(h, red);
	cout << "[" << setw(3) << to_string(num++) << "]";
	cout << setw(51);

	//Rows for numbers 13 to 18
	for (int i = 0; i < 6; i++)
	{
		if (num == 13)
			SetConsoleTextAttribute(h, green);
		if (num == 14)
			SetConsoleTextAttribute(h, blue);
		if (num > 14 && num < 17)
			SetConsoleTextAttribute(h, yellow);
		if (num == 17)
			SetConsoleTextAttribute(h, white);
		if (num == 18)
			SetConsoleTextAttribute(h, pink);
		cout << "[" << setw(3) << to_string(num++) << "]";
	}
	cout << "\n\t";

	//Rows from 19 to 118 minus the Actinides and Lanthanides rows (58 - 103)
	for (int i = 0; i <= 3; i++)
	{
		for (int j = 0; j < 18; j++)
		{

			//Checks what element we are currently on and updates its color accordingly
			if (num == 19 || num == 37 || num == 55 || num == 87)
				SetConsoleTextAttribute(h, cyan);
			if (num == 20 || num == 38 || num == 56 || num == 88)
				SetConsoleTextAttribute(h, red);
			if ((num > 20 && num < 31) || (num > 38 && num < 48) || (num > 71 && num < 81) || (num > 103 && num < 109))
				SetConsoleTextAttribute(h, violet);
			if (num == 31 || num == 49 || num == 50)
				SetConsoleTextAttribute(h, green);
			if (num > 80 && num < 86)
				SetConsoleTextAttribute(h, green);
			if (num > 31 && num < 34 || (num == 51 || num == 52))
				SetConsoleTextAttribute(h, blue);
			if (num == 34)
				SetConsoleTextAttribute(h, yellow);
			if (num == 35 || num == 53)
				SetConsoleTextAttribute(h, white);
			if (num == 36 || num == 54 || num == 86)
				SetConsoleTextAttribute(h, pink);
			if (num > 108 && num < 1118)
				SetConsoleTextAttribute(h, grey);

			//Outputting the * that splits the table at element 57 and 88
			//and updates the current element number accordingly
			if (num == 57)
			{
				cout << setw(3) << "*" << setw(3);
				num = 72;
				continue;
			}
			if (num == 89)
			{
				cout << setw(4) << "**" << setw(2);
				num = 104;
				continue;
			}

			cout << "[" << setw(3) << to_string(num++) << "]";
		}
		cout << "\n\t";
	}

	//Row for Lanthanides and Actinides (58-103)
	cout << "\n";
	SetConsoleTextAttribute(h, blue);
	num = 57;
	for (int i = 0; i < 2; i++)
	{
		cout << setw(23);
		for (int j = 0; j < 15; j++)
		{
			//Outputting for * to show beginnging of table split
			if (num == 57)
				cout << "* ";
			if (num == 89)
				cout << "**";
			cout << "[" << setw(3) << to_string(num++) << "]";
		}
		num = 89;
		SetConsoleTextAttribute(h, brown);
		cout << "\n";
	}
	
	//All is done set color back to white
	SetConsoleTextAttribute(h, white);
}


//Displays the periodic table of elemnts by symbol
//All the if statements checks to see where the element is on the table
//and changes its color whether its a range of elements or a single element
void displayTableSymbols()
{
	//An array containing all the periodic table of element symbols
	const string elementSymbol[] = { "0", "H", "He", "Li", "Be", "B", "C", "N", "O",
	"F", "Ne", "Na", "Mg", "Al", "Si", "P", "S" , "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V", "Cr","Mn","Fe","Co", "Ni", "Cu","Zn",
	"Ga", "Ge", "As", "Se","Br", "Kr" , "Rb", "Sr" ,"Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn",
	"Sb", "Te", "I","Xe", "Cs","Ba","La", "Ce", "Pr" ,"Nd" , "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm" ,"Yb", "Lu", "Hf",
	"Ta", "W" ,"Re" ,"Os","Ir","Pt","Au","Hg", "Tl", "Pb","Bi","Po","At","Rn","Fr","Ra","Ac","Th","Pa", "U","Np","Pu","Am","Cm","Bk",
	"Cf","Es", "Fm","Md","No", "Lr","Rf","Db","Sg","Bh","Hs","Mt", "Ds","Rg","Cn","Nh","Fl","Mc","Lv","Ts","Og" };
	cout << "\n\t";


	//used to set console color from the windows.h header file
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	//sets the console color
	SetConsoleTextAttribute(h, white);

	//Outputs elemnts 1 - 4 with a huge space in between 1 and 2 and 4 and 5
	cout << "\n\t\t\t\tPERIODIC TABLE OF ELEMENTS BY SYMBOL\n";
	int num = 0;
	cout << "\n\t";
	SetConsoleTextAttribute(h, yellow);
	cout << "[" << setw(3) << elementSymbol[++num] << "]";
	num++;
	cout << setw(81);
	SetConsoleTextAttribute(h, pink);
	cout << "[" << setw(3) << elementSymbol[num++] << "]";
	SetConsoleTextAttribute(h, cyan);
	cout << "\n\t";
	cout << "[" << setw(3) << elementSymbol[num++] << "]";
	SetConsoleTextAttribute(h, red);
	cout << "[" << setw(3) << elementSymbol[num++] << "]";
	cout << setw(51);

	//Rows for numbers 5 to 10
	for (int i = 0; i < 6; i++)
	{
		if (num == 5)
			SetConsoleTextAttribute(h, blue);
		if (num > 5 && num < 9)
			SetConsoleTextAttribute(h, yellow);
		if (num == 9)
			SetConsoleTextAttribute(h, white);
		if (num == 10)
			SetConsoleTextAttribute(h, pink);
		cout << "[" << setw(3) << elementSymbol[num++] << "]";
	}
	
	//Outputs elements  11 and 12 and a huge space between 12 and 13
	cout << "\n\t";
	SetConsoleTextAttribute(h, cyan);
	cout << "[" << setw(3) << elementSymbol[num++] << "]";
	SetConsoleTextAttribute(h, red);
	cout << "[" << setw(3) << elementSymbol[num++] << "]";
	cout << setw(51);

	//Rows for numbers 13 to 18
	for (int i = 0; i < 6; i++)
	{
		if (num == 13)
			SetConsoleTextAttribute(h, green);
		if (num == 14)
			SetConsoleTextAttribute(h, blue);
		if (num > 14 && num < 17)
			SetConsoleTextAttribute(h, yellow);
		if (num == 17)
			SetConsoleTextAttribute(h, white);
		if (num == 18)
			SetConsoleTextAttribute(h, pink);
		cout << "[" << setw(3) << elementSymbol[num++] << "]";
	}
	cout << "\n\t";
	
	//Rows from 19 to 118 minus the Actinides and Lanthanides rows (58 - 103)
	for (int i = 0; i <= 3; i++)
	{
		for (int j = 0; j < 18; j++)
		{
			//Checks what element we are currently on and updates its color accordingly
			if (num == 19 || num == 37 || num == 55 || num == 87)
				SetConsoleTextAttribute(h, cyan);
			if (num == 20 || num == 38 || num == 56 || num == 88)
				SetConsoleTextAttribute(h, red);
			if ((num > 20 && num < 31) || (num > 38 && num < 48) || (num > 71 && num < 81) || (num > 103 && num < 109))
				SetConsoleTextAttribute(h, violet);
			if (num == 31 || num == 49 || num == 50)
				SetConsoleTextAttribute(h, green);
			if (num > 80 && num < 86)
				SetConsoleTextAttribute(h, green);
			if (num > 31 && num < 34 || (num == 51 || num == 52))
				SetConsoleTextAttribute(h, blue);
			if (num == 34)
				SetConsoleTextAttribute(h, yellow);
			if (num == 35 || num == 53)
				SetConsoleTextAttribute(h, white);
			if (num == 36 || num == 54 || num == 86)
				SetConsoleTextAttribute(h, pink);
			if (num > 108 && num < 1118)
				SetConsoleTextAttribute(h, grey);

			//Outputting the * that splits the table at element 57 and 88
			//and updates the current element number accordingly
			if (num == 57)
			{
				cout << setw(3) << "*" << setw(3);
				num = 72;
				continue;
			}
			if (num == 89)
			{
				cout << setw(4) << "**" << setw(2);
				num = 104;
				continue;
			}

			cout << "[" << setw(3) << elementSymbol[num++] << "]";
		}
		cout << "\n\t";
	}

	//Rows for Lanthanides and Actinides (58-103)
	cout << "\n";
	num = 57;
	SetConsoleTextAttribute(h, blue);
	for (int i = 0; i < 2; i++)
	{
		cout << setw(23);
		for (int j = 0; j < 15; j++)
		{
			//Outputting for * to show beginnging of table split
			if (num == 57)
				cout << "* ";
			if (num == 89)
				cout << "**";
			cout << "[" << setw(3) << elementSymbol[num++] << "]";
		}
		num = 89;
		SetConsoleTextAttribute(h, brown);
		cout << "\n";
	}

	//All is done we revert back to original console color
	SetConsoleTextAttribute(h, white);
}