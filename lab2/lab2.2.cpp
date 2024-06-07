#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <map>
#include <sstream>
#include <set>
#include <unordered_map>

int alphabetSize;
int stateCount;
int initialState;
std::list<int> finalStates;
std::vector<std::vector<char>> transition_functions;
std::set<std::string> results;
std::unordered_map<int, std::vector<std::vector<int>>> state_to_transitions;

bool read(const std::string& filename)
{
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open())
    {
        std::cerr << "Unable to open file." << std::endl;
        return false;
    }
    
    std::getline(file, line);
    alphabetSize = std::stoi(line);
    
    std::getline(file, line);
    stateCount = std::stoi(line);
    
    std::getline(file, line);
    initialState = std::stoi(line);
    std::cout << "Initial state: " << initialState << std::endl;
    
    std::getline(file, line);
    std::istringstream finalStatesStream(line);
    int state;
    while (finalStatesStream >> state)
    {
        finalStates.push_back(state);
    }

    std::cout << "Final states: ";
    for (int state : finalStates)
    {
        std::cout << state << " ";
    }
    std::cout << std::endl;
    
    while (std::getline(file, line))
    {
        std::istringstream transitionStream(line);
        int fromState, toState;
        char symbol;
        transitionStream >> fromState >> symbol >> toState;
        transition_functions.push_back({(char)(fromState + '0'), symbol, (char)(toState + '0')});
        state_to_transitions[fromState].push_back({fromState, symbol, toState});
    }

    std::cout << "Transition functions: " << std::endl;
    for (const std::vector<char>& transition : transition_functions)
    {
        std::cout << transition[0] << " " << transition[1] << " " << transition[2] << std::endl;
    }

    return true;
}

bool isFinalState(int state)
{
    for (int endState : finalStates)
    {
        if (state == endState)
        {
            return true;
        }
    }
    return false;
}

bool is_unique(const std::string& input)
{
    return results.find(input) == results.end();
}

void find_cycles(int state, std::string path, std::unordered_map<int, std::string>& cycle_map, std::unordered_map<int, bool>& visited)
{
    if (visited[state])
    {
        cycle_map[state] = path;
        return;
    }

    visited[state] = true;

    for (const auto& transition : state_to_transitions[state])
    {
        int toState = transition[2];
        char symbol = transition[1];

        find_cycles(toState, path + symbol, cycle_map, visited);
    }

    visited[state] = false;
}

void find_unique_words(int currentState, std::string currentWord, std::unordered_map<int, int>& state_counts, std::unordered_map<int, std::string>& state_paths, const std::unordered_map<int, std::string>& cycle_map)
{
    state_counts[currentState]++;
    state_paths[currentState] = currentWord;

    for (const auto& transition : state_to_transitions[currentState])
    {
        int fromState = transition[0];
        char symbol = transition[1];
        int toState = transition[2];

        std::string newWord = currentWord + symbol;

        if (state_counts[toState] > 0)
        {
            std::string cycle = newWord.substr(state_paths[toState].length());
            newWord = state_paths[toState] + "(" + cycle.substr(1) + ")";
        }

        if (cycle_map.find(toState) != cycle_map.end() && cycle_map.at(toState).length() > 1)
        {
            std::string cycle = cycle_map.at(toState);
            newWord += "(" + cycle.substr(1) + ")";
        }

        if (isFinalState(toState) && is_unique(newWord))
        {
            results.insert(newWord);
        }

        if (state_counts[toState] == 0) 
        {
            find_unique_words(toState, newWord, state_counts, state_paths, cycle_map);
        }
    }

    state_counts[currentState]--;
    state_paths[currentState] = "";
}

int main()
{
    if (!read("Testinput2.txt"))
    {
        return 1;
    }

    std::unordered_map<int, std::string> cycle_map;
    std::unordered_map<int, bool> visited;
    find_cycles(initialState, "", cycle_map, visited);

    std::cout << "Results: " << std::endl;
    std::unordered_map<int, int> state_counts;
    std::unordered_map<int, std::string> state_paths;
    find_unique_words(initialState, "", state_counts, state_paths, cycle_map);

    for (const std::string& result : results)
    {
        std::cout << result << std::endl;
    }

    std::cout << "Exited successfully" << std::endl;

    return 0;
}