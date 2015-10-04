#include <iostream>
#include <fstream>
#include <thread>
#include <future>
#include <vector>
#include <string>
#include <cmath>
#include <atomic>

#include "utils.h"


std::atomic<bool> finding;

void loopStr(std::string& str){
  for(int i = str.size()-1; i>=0; i--){
    str[i]++;
    if(str[i]%123 == 0){
      str[i] = 'a';
    }else{
      break;
    }
  }
}

const static unsigned int PRINT=1000000;

std::string loop(std::string begin,
                 std::string end,
                 std::string target){
  //  std::cout << begin << ", " << end << "\n";
  unsigned int l = 0;
  std::string crack;
  //std::cout << begin << ", " << end << "\n";
  for(crack = begin; crack != end;){
    if(finding) return "";
    // if((l%PRINT)==0)std::cout << crack << "\n";
    if(encode(crack) == target) return crack;
    //loop crack string
    loopStr(crack);
    ++l;
  }
  return "";
}

void step(std::string& stepper, int step){
  for(int i= 0; i<stepper.size(); ++i){
    stepper[i]+=step;
  }
  if(((int)stepper[0]) > 122){
    stepper = "zzzzzzz";
  }
}

void target(std::string str){
  std::string enc = encode(str);
  std::cout << str << ", ";
  for(int i=0; i<enc.size(); ++i){
    std::cout << (unsigned int)enc[i] << " ";
  }
  std::cout << "\n";
}

const int NBTHREAD=2;

int main(int argc, char * argv[])
{
  std::fstream f;
  std::string ba = "aaaaaaa";
  std::string a = (argv[1]==0)?"":argv[1];
  if(a.size()!=7){
    std::cout << "Mauvaise taille de mdp.\n";
    return 1;
  }
  target(a);
  std::string c = encode(a);
  f.open("password", std::ios::in);
  std::string res;

  std::cout << "Parallel : \n";
  {
    finding = false;
    std::promise<std::string> p;
    auto f = p.get_future();
    std::vector<std::future<std::string>> vt;
    std::string last;
    int index = 0;
    {
      int base = floor(26.0/NBTHREAD);
      Timer t;
      for(int i=0; i< NBTHREAD; ++i){
        std::string bba = ba;
        step(ba, base);
        std::string eea = ba;
        last = ba;
        if(i+1 >= NBTHREAD)
          eea = "zzzzzzzz";
        vt.push_back(std::async(std::launch::async, loop, bba, eea, c));
      }
      int i = 0;
      do{
        auto status = vt[i].wait_for(std::chrono::milliseconds(100));
        if (status == std::future_status::ready){
          index = i;
          finding = true;
          break;
        }
        i = (i+1) % NBTHREAD;
      } while (true);
    }
    std::cout << "Trouve ";
    target(vt[index].get());
  }
  std::cout << "Sequentiel : \n";
  {
    finding = false;
    Timer t;
    res = loop("aaaaaaa", "zzzzzzz", c);
  }
  std::cout << "Trouve ";
  target(res);

  std::cout << "Done\n";
  return 0;
}

