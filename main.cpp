#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <chrono>
#include <thread>

using namespace std;

/* Main Created by Cole Lamers
* My thoughts on this program were to create something in a simple fashion similar to data feeds like twitter all in cli
*/

class Observer {
public:
	virtual void update(string team, int touchdowns, int yards, float winlossRatio) = 0;
};

class Subject {
public:
	virtual void registerObserver(Observer* o) = 0;
	virtual void unregisterObserver(Observer* o) = 0;
	virtual void notifyObservers() = 0;
};


class FootBallData :public Subject {
private:
	int touchdowns;
	int yards;
	float winlossRatio;
	string team;
	vector<string> teamsList;
	vector<Observer*> observerList;

	void notifyObservers() {
		for (int i = 0; i < observerList.size(); i++) {
			if (observerList.at(i) != nullptr) {
				observerList.at(i)->update(team, touchdowns, yards, winlossRatio);
			}
		}
	}

	int getLatesttouchdowns() {
		int value = rand() % 10;
		return (value > yards) ? yards : value;
	}

	int getLatestyards() {
		return (rand() % 85);
	}

	float getLatestWinAverage() {
		return floorf(((float)rand() / (RAND_MAX)) * 1000) / 1000;
	}

	string getTeam() {
		int team = (rand() % 5);
		return teamsList.at(team);
	}

public:
	FootBallData() {
		srand(time(0));  // Initialize random number generator.
		vector<string> teams({ "Packers", "Vikings", "Lions", "Bears", "Panthers" }); //adds teams to list
		teamsList = teams;
	}

	void registerObserver(Observer* o) {
		observerList.push_back(o);
	}

	void unregisterObserver(Observer* o) {
		for (int i = 0; i < observerList.size(); i++) {
			if (observerList.at(i) == o) {
				observerList.erase(observerList.begin() + i);
			}
		}
	}

	// This method is used update data and displays
	void dataChanged() {

		//get latest data
		yards = getLatestyards();
		touchdowns = getLatesttouchdowns();
		winlossRatio = getLatestWinAverage();
		team = getTeam();

		notifyObservers();
	}

};

class CalcScoreDisplay : public Observer {
private:
	int new_touchdowns;
	string new_team;

public:
	void update(string team, int touchdowns, int yards, float winlossRatio) {
		new_team = team;
		new_touchdowns = touchdowns;
		display();
	}

	void display() {
		std::cout << "\nTeam: " << new_team;
		std::cout << "\nCurrent Touchdowns: ";
		std::cout << new_touchdowns << "\n";
	}
};

class CurrentScoreDisplay : public Observer {
private:
	int touchdowns, yards;
	float winlossRatio;
	string team;

public:
	void update(string new_team, int new_touchdowns, int new_yards, float new_winlossRatio) {
		team = new_team;
		touchdowns = new_touchdowns;
		yards = new_yards;
		winlossRatio = new_winlossRatio;
		display();
	}

	void display() {
		std::cout << "\nTeam: " << team;
		std::cout << "\nCurrent Score Display:\n";
		std::cout << "Touchdowns: " << touchdowns;
		std::cout << "\nAverage yards: " << yards;
		std::cout << "\nWin Chance Next Game: " << winlossRatio << "\n";
	}
};



int main() {
	// create objects for display
	CalcScoreDisplay calcScoreDisplay = CalcScoreDisplay();
	CurrentScoreDisplay currentScoreDisplay = CurrentScoreDisplay();

	FootBallData footBallData = FootBallData();

	int choice = 2;
	int decision = 2;
	string refresh = "";

	std::cout << "Welcome to the NFL Data Feed." << endl;

	while (true)
	{
		std::cout << "\nWhat would you like to do?" << endl;
		std::cout << "Type \"subscribe\", \"unsubscribe\", \"feed\", or \"exit\"." << endl;
		std::cin >> refresh;
		if (refresh == "subscribe")
		{
			std::cout << "Which one would you like to subscribe to: " << endl;
			std::cout << "1 - Scoring Feed" << endl;
			std::cout << "2 - Gambling Feed" << endl;
			std::cout << "3 - Both" << endl;
			std::cout << "4 - Exit" << endl;
			std::cin >> decision;
			switch (decision)
			{
			case 1:
			{
				footBallData.registerObserver(&calcScoreDisplay);
				break;
			}
			case 2:
			{
				footBallData.registerObserver(&currentScoreDisplay);
				break;
			}
			case 3:
			{
				footBallData.registerObserver(&calcScoreDisplay);
				footBallData.registerObserver(&currentScoreDisplay);
				break;
			}
			case 4:
			{
				break;
			}
			}
			refresh = "";//resets decision
		}
		else if (refresh == "unsubscribe")
		{
			std::cout << "Which one would you like to unsubscribe to: " << endl;
			std::cout << "1 - Scoring Feed" << endl;
			std::cout << "2 - Gambling Feed" << endl;
			std::cout << "3 - Both" << endl;
			std::cout << "4 - Exit" << endl;
			std::cin >> decision;
			switch (decision)
			{
			case 1:
			{
				footBallData.unregisterObserver(&calcScoreDisplay);
				break;
			}
			case 2:
			{
				footBallData.unregisterObserver(&currentScoreDisplay);
				break;
			}
			case 3:
			{
				footBallData.unregisterObserver(&calcScoreDisplay);
				footBallData.unregisterObserver(&currentScoreDisplay);
				break;
			}
			case 4:
			{
				break;
			}
			}
			refresh = "";//resets refresh decision
		}
		else if (refresh == "feed")
		{
			cout << "\nFetching feed..." << endl;
			int count = 0;
			do
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(1500));
				footBallData.dataChanged();
				count++;
			} while (count < 5);
		}
		else
		{
			cout << "Now leaving the NFL Data Feed. Goodbye." << endl;
			break;
		}
	}

}
