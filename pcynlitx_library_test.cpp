
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <thread_server.hpp>
#include "synchronizer.hpp"

using namespace pcynlitx;

class Test{
public:
    Test(){  }

    void RunThread();

    void Print(synchronizer * syn);

    void Print(synchronizer * syn, int thread_num, int reputation);

    void SPrint(synchronizer * syn, int thread_num,  int reputation, std::string str);

};


void Test::Print(synchronizer * syn){

     std::cout << "\n Hello World";
}

void Test::Print(synchronizer * syn, int thread_num, int reputation){

     syn->Connect(thread_num,"Print");

     syn->lock();

     std::cout << "\n thread num:" << syn->Get_Thread_Number();

     std::cout << "\n Function Name:" << syn->GetFunctionName(syn->Get_Thread_Number());

     syn->unlock();


     for(int i=0;i<reputation;i++){

         std::cout << "\n Hello World";     
     }
}

void Test::SPrint(synchronizer * syn, int thread_num, int reputation, std::string str){

     syn->Connect(thread_num,"SPrint");

     syn->lock();

     std::cout << "\n";

     std::cout << "\n thread num:" << syn->Get_Thread_Number();

     std::cout << "\n Function Name:" << syn->GetFunctionName(syn->Get_Thread_Number());

     std::cout << "\n";

     syn->unlock();


     for(int i=0;i<reputation;i++){

         std::cout << str;     
     }
}


void Test::RunThread(){

     thread_server<Test> server(this);

     synchronizer syn(3);

     std::string str = "Hello ..";

     void (Test::* fptr) (synchronizer *, int thread_num, int reputation) = &(Test::Print);

     void (Test::* sptr) (synchronizer *, int thread_num, int reputation, std::string str) = &(Test::SPrint);


     server.function(fptr,&syn,0,2);

     server.function(fptr,&syn,1,2);

     server.function(sptr,&syn,2,2,str);

     server.join(0);

     server.join(1);

     server.join(2);
}

int main(){

    Test sample;

    sample.RunThread();

    return 0;
}

