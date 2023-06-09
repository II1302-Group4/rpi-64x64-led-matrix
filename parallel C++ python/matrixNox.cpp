#include <stdio.h>
#include <iostream>
#include <cstdlib> 
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include<algorithm>
#include <semaphore.h>
#define arrSiz 4096
using namespace std;


string matrixArr[arrSiz];
std::string result = "";
sem_t smeaphore;
pthread_mutex_t lock;
//int accesses;

//custom split method
int len(string str)
{  
    int length = 0;  
    for (int i = 0; str[i] != '\0'; i++)  
      length++;  
    return length;     
}  
void split (string str, char seperator)  
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

//method to get data from matrix and split it into an array
void* get_matrix() {
   FILE *read_matrix;
   int chars_read;
   int tot_reads;
   char buffer[BUFSIZ];
   //memset(buffer,'\0', sizeof(buffer));
   read_matrix = popen("python read_matrix.py", "r");
   
   //pthread_mutex_lock(&lock);
   //sem_wait(&smeaphore);
   //accesses++;
   while (tot_reads < 6)
   {
      chars_read = fread(buffer, sizeof(char), BUFSIZ, read_matrix);
      if (chars_read > 0)
      {
         result.append(buffer);
         tot_reads ++;
      }
   }
   int first = result.find("[");
   int last = result.find("]");
   result = result.substr(first+1,last-34);
   result.erase(remove(result.begin(), result.end(), '\''), result.end());
   split(result, ','); //split taking way too much time :/
   //pthread_mutex_unlock(&lock);
   //sem_post(&smeaphore);
   
   return nullptr;   
}

void* check_update(void*) {
   FILE* pipe = popen("python get_update.py", "r");

    char buffer[3];
    string result = "";
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fileno(pipe), &fds);
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 100000;

    while (true) {
      cout<<"start of thread1\n"; 
        while (true){
          int ret = select(fileno(pipe) + 1, &fds, NULL, NULL, &tv);
          if (ret > 0){
            if(fgets(buffer, 3, pipe) != NULL) {
            result += buffer;
            cout<<result<<"\n";
            result="";
            memset(buffer,'\0', sizeof(3));
          }
          sleep(1);
          }
        
        }
    }
    cout << result << endl;
    pclose(pipe);
    cout<<"end of thread1\n";
    return nullptr;
}

int main() {
  get_matrix();
  pthread_t thread1;
  pthread_create(&thread1,nullptr, check_update, nullptr);
  printf("after thread1\n");

  
  for (int i = 0; i < arrSiz; i++)  
  {  
    cout << "Pixel nr" << i+1 << " :" << matrixArr[i]<<"\n";  
  }
   
    
  pthread_join(thread1, nullptr);
  cout << "the end\n";

  return 0;
}
