
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <synchronizer.hpp>

class Test{
public:
    Test(){ }

    void RunThread();

    void Print();

    void Print(int reputation);

    void Print(int reputation,std::string str);
};


void Test::Print(){

     std::cout << "\n Hello World";
}

void Test::Print(int reputation){

     for(int i=0;i<reputation;i++){

         std::cout << "\n Hello World";     
     }
}

void Test::Print(int reputation, std::string str){

     for(int i=0;i<reputation;i++){

         std::cout << str;     
     }
}


void Test::RunThread(){

     pcynlitx::synchronizer<Test> syn(this);

     std::string str = "Hello ..";

     syn.function<void>(&Test::Print,2,str);

     syn.join(0);
}

int main(){

    Test sample;

    sample.RunThread();


    return 0;
}

