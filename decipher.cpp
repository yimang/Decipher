#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <vector>

using namespace std; 

bool isEnglish(string dict)
{
  stringstream ss(dict);
  ifstream inf;
  inf.open("/home/ssdavis/40/p7/words");
  stringstream buff;
  buff << inf.rdbuf();
  string word;

  while(ss >> word)
  { 
    buff.seekg(0, buff.beg);
    
    while(buff >> dict) if (word == dict) break;
    
    if (word == dict) continue;
    return false;
  }//for all words
  
  return true;
}//isEnglish check if valid words

void shift(string &message, int amount)
{
  unsigned int i;

  // shift each char in message by amount
  for(i = 0; i < message.size(); i++)
  {
    if(isupper(message.at(i))) 
    {
      if(message.at(i) + amount <= 'Z' && isalpha(message.at(i)))
        message.at(i) += amount;
      else // shift loops
        if(isalpha(message.at(i)))
        message.at(i) = 'A' + amount - ('Z' - message.at(i) + 1) ;
    } // uppercase shift
    else // lowercase shift
      if(islower(message.at(i)))
    {
      if(message.at(i) + amount <= 'z' && isalpha(message.at(i)))
        message.at(i) += amount;
      else // shift loops
        if(isalpha(message.at(i)))
        message.at(i) = 'a' + amount - ('z' - message.at(i) + 1) ;
    } // lowercase shift
  } // shift letters by amount
} // shift()

void extract(string &message, vector<int> &frequencies, ifstream &inf)
{
  int num;
  stringstream ss;
  
  // extract encrypted text
  while(getline(inf, message))
    ss << message << endl;
  message = ss.str();
  inf.close();
  
  // extract character frequencies
  inf.open("/home/ssdavis/40/p7/frequencies");
  
  while(inf >> num)
    frequencies.push_back(num);
  inf.close();
  num = 0;
} // extract()

void determineLikes(string &message, vector<int> &mFrequencies, 
                    const vector<int> &frequencies, vector<int> &like)
{
  unsigned int i, j;
  int sum;
  char letter;
  
  // loop to find all combinations
  for(i = 0; i < 26; i++)
  {
    // shift letters
    shift(message, 1);
   
    // count the frequencies
    for(j = 0; j < 26; j++)
        mFrequencies.at(j) = 0;
    
    letter = 'A';
    
    for(j = 0; j < 26; j++)
        mFrequencies.at(j) += count(message.begin(), message.end(), letter++);

    letter = 'a';
    
    for(j = 0; j < 26; j++)
        mFrequencies.at(j) += count(message.begin(), message.end(), letter++);
    
    // determine likelihoods of different combinations
    for(j = 0; j < 26; j++)
      sum += (frequencies.at(j) * mFrequencies.at(j));
    
    like.at(i) = sum;
    sum = 0;
  } // find combinations
} // determineLikes()

void print(string &message, const vector<int> &like)
{
  unsigned int i, j, k;
  vector<int> likeCopy(26);
  string mCopy(message);
  likeCopy = like;
  sort(likeCopy.begin(), likeCopy.end());
  
  for(i = 0; i < 26; i++)
  {
    for(j = 0; j < 26; j++)
      if(likeCopy.at(25 - i) == like.at(j))
      {
        shift(message, j + 1);
        mCopy = message;
        
        for(k = 0; k < mCopy.size(); k++)
        {
          if(!isalpha(mCopy.at(k))) mCopy.at(k) = ' ';
          
          if(isupper(mCopy.at(k))) 
            mCopy.at(k) = tolower(mCopy.at(k));
        }// convert to lowercase for english checking
        
        if(isEnglish(mCopy))
        {
          cout << "Deciphered message version #" << i + 1 << endl;
          cout << message << endl;
        }// check if english
        
        shift(message, 25 - j);
        break;
      } // index found
  } // print out top 6 likelihoods
} // print()

int main(int argc, char *argv[])
{
  ifstream inf;
  vector<int> like(26);
  vector<int> frequencies;
  vector<int> mFrequencies(26, 0);
  string message;

  inf.open(argv[1]);
  extract(message, frequencies, inf);
  determineLikes(message, mFrequencies, frequencies, like);
  print(message, like);

  return 0;
} // main()