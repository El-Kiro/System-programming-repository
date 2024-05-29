#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <map>

int alpabetSize;
int stateCount;
int initialState;
std::list<int> finalStates;
std::vector<std::vector<char>> transition_functions;
std::list<std::string> results;
std::list<std::vector<int>> transition_functions_history;

bool read(std::string text)
{
    std::ifstream file("Testinput2.txt");
    std::istreambuf_iterator<char> iter(file);
    std::istreambuf_iterator<char> endIter;

    int lineCounter = 1;

    while (lineCounter < 5 and iter != endIter)
    {
        if (*iter == '\n')
        {
            ++lineCounter;
        }
        else if (lineCounter == 1)
        {
            alpabetSize = *iter - '0';
        }
        else if (lineCounter == 2)
        {
            stateCount = *iter - '0';
        }
        else if (lineCounter == 3)
        {
            initialState = *iter - '0';
            std::cout << "Initial state: " << initialState << std::endl;
        }
        else if (lineCounter == 4 and *iter != ' ')
        {
            finalStates.push_back(*iter - '0');
        }
        ++iter;
    }

    std::cout << "Final states: ";
    for (int state : finalStates)
    {
        std::cout << state << " ";
    }
    std::cout << std::endl;

    std::vector<char> transFunc;
    while (iter != endIter)
    {
        if (*iter == '\n')
        {
            transition_functions.push_back(transFunc);
            transFunc.clear();
        }
        else if (*iter != ' ')
        {
            transFunc.push_back(*iter);
        }
        ++iter;
    }

    std::cout << "Transition functions: " << std::endl;
    for (std::vector<char> transition : transition_functions)
    {
        std::cout << transition[0] << " " << transition[1] << " " << transition[2] << " " << std::endl;
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

bool isUsedTransitionFunc(int transition, std::list<int> banned_transition_functions)
{
    for (int forbidden_transition : banned_transition_functions)
    {
        if (transition == forbidden_transition)
        {
            return true;
        }
    }
    return false;
}

bool is_unique(std::string input)
{
    for (std::string word : results)
    {
        if (input == word)
        {
            return 0;
        }
    };
    return 1;
};

void find_unique_word()
{
    std::string result = "";
    std::map<int, std::list<int>> banned_transition_functions;
    bool searching = true;
    int currentState = initialState;
    std::vector<int> states;
    int last_transition_func = -1;

    states.push_back(currentState);

    while (searching == true)
    {
        bool transition_func_found;

        for (int i = 0; i < transition_functions.size(); i++)
        {
            transition_func_found = false;

            if ((transition_functions[i][0] - '0' == currentState) && !isUsedTransitionFunc(i, banned_transition_functions[result.length()]))
            {
                result += transition_functions[i][1];
                currentState = transition_functions[i][2] - '0';
                searching = true;
                transition_func_found = true;
                states.push_back(i + 1);
                last_transition_func = i;

                if (isFinalState(currentState) && is_unique(result))
                {
                    banned_transition_functions.clear();
                    results.push_back(result);
                    states.clear();
                    transition_func_found = true;
                    searching = false;
                    break;
                }
            }
        }

        if (!transition_func_found)
        {
            if (result.length() == 0)
            {
                break;
            }
            result.pop_back();
            banned_transition_functions[result.length()].push_back(last_transition_func);
            states.pop_back();
            currentState = states.back();
        }

        for (auto it = banned_transition_functions.begin(); it != banned_transition_functions.end();)
        {
            if (it->first < result.length())
            {
                it = banned_transition_functions.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

int main()
{
    read("Testinput3.txt");

    std::cout << "Results: " << std::endl;
    std::string last_entry = "";
    while (true)
    {
        find_unique_word();

        if (results.back() != last_entry)
        {
            std::cout << results.back() << std::endl;
            last_entry = results.back();
        }
        else
        {
            break;
        }
    }

    std::cout << "Exited succesfully" << std::endl;

    return 0;
}