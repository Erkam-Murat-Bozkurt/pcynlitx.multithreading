
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <thread_server.hpp>
#include "synchronizer.hpp"

using namespace pcynlitx;

class Test{
public:
    Test(){ 
    }

    void RunThread();

    void Print(synchronizer * syn);

    void Print(synchronizer * syn,int reputation);

    void Print(synchronizer * syn,int reputation,std::string str);

};


void Test::Print(synchronizer * syn){

     std::cout << "\n Hello World";
}

void Test::Print(synchronizer * syn, int reputation){

     for(int i=0;i<reputation;i++){

         std::cout << "\n Hello World";     
     }
}

void Test::Print(synchronizer * syn, int reputation, std::string str){

     for(int i=0;i<reputation;i++){

         std::cout << str;     
     }
}


void Test::RunThread(){

     thread_server<Test> server(this);

     synchronizer syn;

     std::string str = "Hello ..";

     void (Test::* fptr) (synchronizer *, int reputation) = &(Test::Print);

     server.function(fptr,&syn,2);

     server.join(0);
}

int main(){

    Test sample;

    sample.RunThread();

    return 0;
}

