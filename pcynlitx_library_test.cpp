
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

    void Print(synchronizer * syn,  int reputation);

    void SPrint(synchronizer * syn, int reputation, std::string str);

};


void Test::Print(synchronizer * syn){

     std::cout << "\n Hello World";
}

void Test::Print(synchronizer * syn, int reputation){

     syn->Connect("Print");

     syn->lock();

     std::cout << "\n";

     std::cout << "\n thread num:"    << syn->Get_Thread_Number();

     std::cout << "\n";

     syn->unlock();


     /*

     for(int i=0;i<reputation;i++){

         std::cout << "\n Hello World";     
     }

     */

     //syn->rescue(0,1);

}

void Test::SPrint(synchronizer * syn, int reputation, std::string str){

     syn->Connect("SPrint");


     //syn->lock();

     /*

     std::cout << "\n";

     std::cout << "\n Before wait function";

     std::cout << "\n thread num:" << syn->Get_Thread_Number();

     std::cout << "\n Function Name:" << syn->GetFunctionName(syn->Get_Thread_Number());

     std::cout << "\n";

     syn->unlock();

     */

     //syn->wait(2,3);

     syn->lock();

     std::cout << "\n";

     std::cout << "\n thread num:" << syn->Get_Thread_Number();

     std::cout << "\n";

     syn->unlock();


     /*

     for(int i=0;i<reputation;i++){

         std::cout << str;     
     }

     */



     //syn->rescue(2,3);

     //syn->rescue(1,2);

}


void Test::RunThread(){

     thread_server<Test> server(this,4);


     std::string str = "Hello ..";

     void (Test::* fptr) (synchronizer *, int reputation) = &(Test::Print);

     void (Test::* sptr) (synchronizer *,  int reputation, std::string str) = &(Test::SPrint);


     server.function(fptr,0,&server.syn,2);

     server.function(fptr,1,&server.syn,1);

     server.function(sptr,2,&server.syn,2,str);

     server.function(sptr,3,&server.syn,3,str);


     server.join(0);

     server.join(1);

     server.join(2);

     server.join(3);

}

int main(){

    Test sample;

    sample.RunThread();

    return 0;
}

