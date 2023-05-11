#include <iostream>
#include <cstring>
#include <vector>
#include <string>

#define arrSiz 4096
using namespace std;

string matrixArr[arrSiz];
std::string result = "";
// sem_t smeaphore;
// pthread_mutex_t lock;
// int accesses;

// custom split method
int len(string str)
{
    int length = 0;
    for (int i = 0; str[i] != '\0'; i++)
        length++;
    return length;
}
void split(string str, char seperator)
{
    int currIndex = 0, i = 0;
    int startIndex = 0, endIndex = 0;
    while (i <= len(str))
    {
        if (str[i] == seperator || i == len(str))
        {
            endIndex = i;
            string subStr = "";
            subStr.append(str, startIndex, endIndex - startIndex);
            matrixArr[currIndex] = subStr;
            currIndex += 1;
            startIndex = endIndex + 1;
        }
        i++;
    }
}
