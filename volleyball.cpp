#include <iostream>
#include <vector>
#include <cmath> 

using namespace std;

struct Player{
	int index;
	int playerStats;
	char position;
	char secondaryPosition;
	string name;
};
Player *team;
std::vector<Player*> rotation;
int globalStatAve;

bool validatePair(Player *p1, Player *p2){
	//Validates if two players are not the same position and can stand next to each other
	if(p1->position != p2->position){
		return true;
	}else{
		if( (p1->secondaryPosition) && (p1->secondaryPosition != p2->position) ){
			return true;
		} else if( (p2->secondaryPosition) && (p1->position != p2->secondaryPosition)){
			return true;
		}
	}
	return false;
}
int margin = 15;
int validateTrio(Player *p1, Player *p2, Player *p3){
	//Validates if three players can play together and what their statistic average would be (so long as they are over a given margin)
	int passingAverage = (p1->playerStats-globalStatAve)+(p2->playerStats-globalStatAve)+(p3->playerStats-globalStatAve) + margin;
	if((p1->position != p2->position) && (p2->position != p3->position) && (p1->position != p3->position) ){
		return passingAverage;
	}
	if(p1->secondaryPosition){
		Player *p4 = new Player;
		p4->position = p1->secondaryPosition;
		bool holder = validateTrio(p4, p2, p3);
		delete p4;
		if(holder) return passingAverage;
	}
	if(p2->secondaryPosition){
		Player *p4 = new Player;
		p4->position = p2->secondaryPosition;
		bool holder = validateTrio(p1, p4, p3);
		delete p4;
		if(holder) return passingAverage;
	}
	if(p3->secondaryPosition){
		//cout << "P3S    ";
		Player *p4 = new Player;
		p4->position = p3->secondaryPosition;
		bool holder = validateTrio(p1, p2, p4);
		delete p4;
		if(holder) return passingAverage;
	}
	return 0;
}
int final =0; 
void printRotation(vector<Player*> thisRotation){
	for(int i=0; i<thisRotation.size(); i++){
		cout << thisRotation[i]->name << ": "<< thisRotation[i]->position << " " << thisRotation[i]->secondaryPosition << "      ";
	}
	cout << endl;
	final++;
}
void permuteRotation(bool *inRotation, int numPlayers, int teamStat){
	//Find all possible permutations of a given volleyball team
	if(rotation.size() == numPlayers){
		int trioAve1 = validateTrio(rotation[rotation.size()-1], rotation[0], rotation[1]);
		int trioAve2 = validateTrio(rotation[rotation.size()-2], rotation[rotation.size()-1], rotation[0]);
		if((trioAve1 > 0) && (trioAve2 > 0)){
			
			teamStat += (trioAve1 + trioAve2);
			printRotation(rotation);
			cout << "Stat average : " << (teamStat/numPlayers) << endl;;
			teamStat -= (trioAve1 + trioAve2);
		}
	}else{
		for(int i=0; i<numPlayers; i++){
			if(!inRotation[i]){
				rotation.push_back(&team[i]);
				inRotation[i] = true;
				if(rotation.size() == 2){
					if(validatePair(rotation[rotation.size()-2], rotation[rotation.size()-1]) == true){
						permuteRotation(inRotation, numPlayers, teamStat);	
					}
				}else if(rotation.size() > 2){
					int trioAve = validateTrio(rotation[rotation.size()-3], rotation[rotation.size()-2], rotation[rotation.size()-1]);
					if(trioAve > 0){
						///cout << "Trio ave " << trioAve << endl;
						teamStat += trioAve;
						permuteRotation(inRotation, numPlayers, teamStat);	
						teamStat -= trioAve;
					}
				}
				rotation.pop_back();
				inRotation[i] = false;
			}

		}
		return;
	}

}
void printTeam(Player *thisteam){
	for(int i=0; i<7; i++){
		cout  << "Player"<< thisteam[i].index << " "<< thisteam[i].position << " " << thisteam[i].secondaryPosition << "  " << thisteam[i].playerStats <<  endl;
	}
}
int main(int argc, char **argv){
	string S;
	int playerNum;
	bool stats = false;

	cin >> S;
	if(S == "Stats"){
		cout << "Correct" << endl;
		stats = true;
		cin >> S;
	}
	cin >> playerNum;
	
	team = new Player[7];
	char a; int c, playerStat, statAverage = 0;
	for(int i=0; i<7; i++){
		cin >> S >> c >> a;
		team[i].index = i;
		team[i].position = a; 
		team[i].name = S;
		if(c == 2){
			cin >> a;
			team[i].secondaryPosition = a;
		}
		if(stats){
			cin >> playerStat;
			team[i].playerStats = playerStat;
			statAverage += playerStat;
		}
		team[i].index = i;
	}
	globalStatAve = (statAverage/playerNum);
	cout << "\nPlayer average : " << globalStatAve << endl; 

	bool *takenPlayers = new bool[7];
	rotation.push_back(&team[0]);
	takenPlayers[0] = true;

	// cout << "\n\nTesting value : "<<takenPlayers[0] << endl;
	printTeam(team);
	// cout << "Testing : \n" << validatePair(&team[0], &team[1]) << endl; 
	if(stats){
		permuteRotation(takenPlayers, 7, 0);
	}else{
		permuteRotation(takenPlayers, 7, 0);
	}
	cout << final <<  endl;
}