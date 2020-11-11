#include <iostream>
#include <string>
#include <limits>
using namespace std;

#include "utils.h"

struct Player
{
	unsigned points = 0;
	unsigned holes[6] = { 4,4,4,4,4,4 };
	bool is_bot = false;
};


void validate_input(int& action, const string& message, int lim1, int lim2) 
{
	while (cin.fail() || action < lim1 || lim2 < action)
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << message;
		cin >> action;
	}
	cin.ignore();
}

bool validate_hole(Player player, int hole)
{

	if (hole < 0 || hole > 5 || cin.fail()) {
		cout << "Invalid input, please choose a valid letter from your side of the board:\n";
		return false;
	}
	if (player.holes[hole] == 0) {
		cout << "Invalid input, please choose a hole with seeds in it:\n";
		return false;
	}
	return true;
}

void board_display()
{
	/*for (size_t i = HOLE_NUMBER-1; i > ; i++)
	{

	}*/

}

unsigned ask_player_hole(unsigned player, bool bot_is_playing, Player players[2])
{
	unsigned hole = 0;
	char letter;

	if (player == 0)
	{
		if (bot_is_playing) {
			cout << "Please pick a letter from your side of the board\n";
		}
		else {
			cout << "South player, pick a letter from your side of the board\n";
		}
		cin >> letter;
		hole = (unsigned)letter - 'A';
		while (!validate_hole(players[player], hole))
		{
			cin >> letter;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			hole = (unsigned)letter - 'A';
		}
	}

	else {
		if (!bot_is_playing) {
			cout << "North player, pick a letter from your side of the board\n";
			cin >> letter;
			hole = (unsigned)letter - 'a';
			while (!validate_hole(players[player], hole))
			{
				cin >> letter;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				hole = (unsigned)letter - 'a';

			}
		}
	}
	return hole;
}

void distribute_seeds(Player player, unsigned hole, unsigned& seeds)
{
	for (size_t i = hole; i < HOLE_NUMBER; i++)
	{
		player.holes[i]++;
		seeds--;
		if (seeds == 0) {
			break;
		}
	}
}

unsigned apply_points(Player player, unsigned& seeds)
{
	unsigned points = 0, hole = 0;
	bool last_hole_has_seeds = false;

	if (seeds == 0) {
		return points;
	}
	for (size_t i = 0; i < HOLE_NUMBER; i++)
	{
		player.holes[i]++;
		seeds--;
		hole = i;
		if (seeds == 0) {
			break;
		}
	}
	hole--;

	for (size_t i = HOLE_NUMBER - 1; i > hole; i++)
	{
		if (player.holes[i] != 0) {
			last_hole_has_seeds = true;
			break;
		}
	}
	if (seeds == 0) {
		while (hole >= 0)
		{
			if (player.holes[hole] == 2 || player.holes[hole] == 3) {
				if (hole == 0 && !last_hole_has_seeds) {
					break;
				}
				points += player.holes[hole];
				player.holes[hole] = 0;
			}
			else {
				break;
			}
			hole--;
		}
	}

	/*if (seeds == 0) {
		while (player.holes[hole] == 3 || player.holes[hole] == 2)
		{
			if (opponent_can_play(player)) {
				points += player.holes[hole];
				player.holes[hole] = 0;
			}
			hole--;
			if (hole < 0) {
				break;
			}
		}
	}*/
	return points;
}

void player_turn(unsigned player, Player players[2], unsigned hole)
{
	unsigned seeds = players[player].holes[hole];  // sow seeds
	players[player].holes[hole] = 0;
	unsigned opposing_player = 1 - player;
	unsigned points = 0;

	while (seeds)
	{
		distribute_seeds(players[player], hole, seeds);
		points += apply_points(players[opposing_player], seeds);
	}

	players[player].points += points;
	cout << "You have gained " << points << " this turn\n";

}

bool game_not_ended(int &winner, Player players[2])
{
	if (players[0].points == POINTS_TO_TIE && players[1].points == POINTS_TO_TIE) {
		winner = -1;
		return false;
	}
	else if(players[0].points >= POINTS_TO_WIN) {
		winner = 0;
		return false;
	}
	else if(players[1].points >= POINTS_TO_WIN) {
		winner = 1;
		return false;
	}
	return true;
}

void announce_winner(int winner, bool bot_is_playing)
{
	switch (winner)
	{
		case -1:
			cout << "It was a draw!\n";
			break;
		case 0:
			if (bot_is_playing) {
				cout << "You won!\n";
			}
			else {
				cout << "The South player won!\n";
			}
			break;
		case 1:
			if (bot_is_playing) {
				cout << "The Bot won!\n";
			}
			else {
				cout << "The North player won!\n";
			}
			break;
	}
}


bool opponent_can_play(Player player)
{
	bool can_play = false;

	for (size_t i = 0; i < HOLE_NUMBER; i++)
	{
		can_play |= player.holes[i] == 1 || player.holes[i] > 3;
	}
	return can_play;
}



void oware()
{
	Player player_south, player_north;
	Player players[2] = { player_south, player_north };
	int bot_is_playing, winner;
	bool in_game = true;
	cout << "Welcome to Oware developed in C++\n";
	cout << "Do you want to play versus a human player ou against a Bot.\n [0] For human\n[1] For bot\n";
	cin >> bot_is_playing;
	validate_input(bot_is_playing, "Invalid input, please choose again if you want to play against a human or a bot\n", 0, 1);
	if (bot_is_playing) {
		players[1].is_bot = true;
	}

	while (in_game) 
	{
		for (unsigned player = 0; player <= 1; player++) 
		{
			unsigned hole = ask_player_hole(player, bot_is_playing, players);
			player_turn(player, players, hole);
			in_game = game_not_ended(winner, players);
			if (!in_game) {
				break;
			}
		}
	}
	announce_winner(winner, bot_is_playing);
	cout << "I hope you have enjoyed playing Oware in the console!\n";
}
int main()
{
	oware();
	//Player player = { 0, {2,2,2,2,2,2}, false };
	//cout << opponent_can_play(player);
	
	return 0;
}

