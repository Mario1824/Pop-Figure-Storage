#include <iostream>
#include <vector>
#include <string>;
#include <fstream>

using namespace std;

class popFigure
{
private:	
	//private attributes of pop figures
	string popName;
	int popNum;
public:
	//default cosntructor
	popFigure(string p, int num)
	{
		popName = p;
		popNum = num;
	}
	//Setters
	void setPopName(const string& p)
	{
		popName = p;
	}
	void setPopNumber(const int& num)
	{
		popNum = num;
	}

	//Getters
	string getPopName()
	{
		return popName;
	}
	int getPopNum()
	{
		return popNum;
	}

	string getInfo() const {
		return popName + to_string(popNum);
	}
};

class popFigureCollection {
private:
	vector<popFigure> PopFigures;
public:
	//functions of application
	void menu();
	void addPop();
	void deletePop();
	void findPop();

};

void popFigureCollection::menu()
{
	int userChoice = 0;
	while (userChoice < 1 || userChoice > 4)
	{

		cout << "---------------------------------" << endl;
		cout << "Choose an option below: " << endl;
		cout << "1: Administrator" << endl;
		cout << "2: Add a pop figure" << endl;
		cout << "3: Delete a pop figure" << endl;
		cout << "4: Find a pop figure" << endl;
		cin >> userChoice;
		switch (userChoice) {
		case 1:
			cout << "Admin" << endl;
			break;
		case 2:
			popFigureCollection::addPop();
			break;
		case 3:
			popFigureCollection::deletePop();
			break;
		case 4:
			popFigureCollection::findPop();
			break;
		default:
			cout << "Incorrect option" << endl;
			break;
		}
	}
}

void popFigureCollection::addPop()
{
	string popName;
	int popNum;
	cout << "Adding pop figure" << endl;
	cout << "What is the name of the pop?: ";
	cin >> popName;
	cout << "\nWhat is the pop number?: ";
	cin >> popNum;

	popFigure newPop(popName, popNum);
	PopFigures.push_back(newPop);

	for (size_t i = 0; i < PopFigures.size(); i++)
	{
		cout << PopFigures[i].getInfo() << " " << endl;
	}
	return menu();
}

void popFigureCollection::deletePop()
{
	cout << "Deleting a pop figure function" << endl;
}

void popFigureCollection::findPop()
{
	cout << "Finding a pop figure function" << endl;
}



int main()
{
	popFigureCollection s;
	s.menu();
	return 0;
}