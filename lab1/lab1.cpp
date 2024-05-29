#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <list>
#include <set>

std::list<std::string> result;
std::set<std::string> uniqueWords;

bool isConsonant(char l)
{
    l = tolower(l);
    return l == 'b' || l == 'c' || l == 'd' || l == 'f' || l == 'g' || l == 'h' || l == 'j'
        || l == 'k' || l == 'l' || l == 'm' || l == 'n' || l == 'p' || l == 'q' || l == 'r'
        || l == 's' || l == 't' || l == 'v' || l == 'w' || l == 'x' || l == 'z';
}

int countConsonants(const std::string& word)
{
    int count = 0;
    for (char l : word)
    {
        if (isConsonant(l))
        {
            count++;
        }
    }
    return count;
}

int main()
{
    std::ifstream file("Testinput.txt");
    std::istreambuf_iterator<char> iter(file);
    std::istreambuf_iterator<char> endIter;

    std::string word;
    while (iter != endIter)
    {
        if (isalpha(*iter))
        {
            while (iter != endIter && isalpha(*iter))
            {
                word += *iter;
                ++iter;
            }
            if (uniqueWords.find(word) == uniqueWords.end())
            {
                result.push_back(word);
                uniqueWords.insert(word);
            }
            word.clear();
        }
        else
        {
            ++iter;
        }
    }

    result.sort([](const std::string& a, const std::string& b) {
        return countConsonants(a) < countConsonants(b);
    });

    for (const std::string& word : result)
    {
        std::cout << word << " ";
    }

    return 0;
}
