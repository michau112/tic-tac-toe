#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>


struct Player
{
    std::string symbol;
    int wins = 0;
    int draws = 0;
    int losses = 0;
    int points = 0;

    void addWin()
    {
        wins++;
        points += 3;
    }

    void addDraw()
    {
        draws++;
        points += 1;
    }

    void addLoss()
    {
        losses++;
    }
};


std::unordered_map<int, Player> readPlayers(const std::string& filename)
 {
    std::unordered_map<int, Player> players;
    std::ifstream infile(filename);
    std::string line;

    while (std::getline(infile, line))
    {
        std::stringstream ss(line);
        int index;
        std::string symbol;
        ss >> index >> symbol;
        players[index] = Player{symbol};
    }

    return players;
}

void readMatches(const std::string& filename, std::unordered_map<int, Player>& players)
{
    std::ifstream infile(filename);
    std::string line;
    std::unordered_map<std::string, int> symbol_to_index;

    for (auto it = players.begin(); it != players.end(); ++it)
    {
        symbol_to_index[it->second.symbol] = it->first;
    }

    while (std::getline(infile, line))
        {
        if (line.empty()) continue;

        std::string line1, line2, line3;
        line1 = line;
        std::getline(infile, line2);
        std::getline(infile, line3);


        std::unordered_map<char, int> count;
        for (char ch : line1 + line2 + line3)
        {
            if (ch != '.') count[ch]++;
        }


        char winner = '.';
        for (auto it = count.begin(); it != count.end(); ++it)
        {
            if (it->second == 3)
            {
                winner = it->first;
                break;
            }
        }

        if (winner == '.')
        {

            for (auto it = symbol_to_index.begin(); it != symbol_to_index.end(); ++it)
            {
                const std::string& symbol = it->first;
                int index = it->second;

                if (line1.find(symbol) != std::string::npos ||
                    line2.find(symbol) != std::string::npos ||
                    line3.find(symbol) != std::string::npos)
                {
                    players[index].addDraw();
                }
            }
        }
                else
        {
            int winner_index = symbol_to_index[std::string(1, winner)];
            players[winner_index].addWin();
            for (auto it = symbol_to_index.begin(); it != symbol_to_index.end(); ++it) {
                const std::string& symbol = it->first;
                int index = it->second;

                if (symbol != std::string(1, winner) &&
                    (line1.find(symbol) != std::string::npos ||
                     line2.find(symbol) != std::string::npos ||
                     line3.find(symbol) != std::string::npos))

                {
                    players[index].addLoss();
                }
            }
        }
    }
}


void showTable(const std::unordered_map<int, Player>& players)
{
    std::vector<std::pair<int, Player>> sorted_players;
    for (const auto& pair : players)
    {
        if (pair.first != 0)
        {
            sorted_players.emplace_back(pair);
        }
    }

    std::sort(sorted_players.begin(), sorted_players.end(), [](const std::pair<int, Player>& a, const std::pair<int, Player>& b)
    {
        return a.second.points > b.second.points;
    });

    std::cout << "Indeks\tSymbol\tPunkty\tWygrane\tRemisy\tPrzegrane\n";
    for (const auto& pair : sorted_players)
    {
        std::cout << pair.first << "\t" << pair.second.symbol << "\t" << pair.second.points << "\t"
        << pair.second.wins << "\t" << pair.second.draws << "\t" << pair.second.losses << "\n";
    }
}


void showResults(const std::string& filename)
{
    std::ifstream infile(filename);
    std::string line;
    while (std::getline(infile, line))
    {
        std::cout << line << "\n";
    }
}


void showPlayerStats(const std::unordered_map<int, Player>& players)
{
    int index;
    std::cout << "Podaj numer indeksu gracza: ";
    std::cin >> index;

    auto it = players.find(index);
    if (it != players.end()) {
        const Player& player = it->second;
        std::cout << "Statystyki gracza " << index << " (" << player.symbol << "):\n";
        std::cout << "Punkty: " << player.points << "\n";
        std::cout << "Zwyciestwa: " << player.wins << "\n";
        std::cout << "Remisy: " << player.draws << "\n";
        std::cout << "Porazki: " << player.losses << "\n";
    }
    else
    {
        std::cout << "Gracz o podanym indeksie nie istnieje.\n";
    }
}

int main()
{
    std::unordered_map<int, Player> players = readPlayers("players.txt");
    readMatches("tictactoe.txt", players);

    int choice;
    while (true)
    {
        std::cout << "\n1. Zobacz tabele\n";
        std::cout << "2. Zobacz wyniki\n";
        std::cout << "3. Zobacz statystyki gracza\n";
        std::cout << "4. Wyjscie\n";
        std::cout << "Wybierz opcje: ";
        std::cin >> choice;

        switch (choice)
        {
            case 1:
                showTable(players);
                break;
            case 2:
                showResults("tictactoe.txt");
                break;
            case 3:
                showPlayerStats(players);
                break;
            case 4:
                std::cout << "Koniec programu.\n";
                return 0;
            default:
                std::cout << "Niepoprawna opcja. Sprobuj ponownie.\n";
                break;
        }
    }

    return 0;
}
