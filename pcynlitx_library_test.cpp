
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <threads.hpp>
#include <pthreads.hpp>
#include "synchronizer.hpp"

//using namespace pcynlitx;

/*
class Test{
public:
    Test(){  }

    void RunThread();

    //void Print(synchronizer & syn);

    //void Print(synchronizer & syn,  int reputation);

    void SPrint(pcynlitx::synchronizer & syn, int reputation, std::string str);

};
*/

/*
void Test::Print(synchronizer & syn){

     std::cout << "\n Hello World";
}

void Test::Print(synchronizer & syn, int reputation){

     syn.Connect("Print");

     syn.lock();

     std::cout << "\n";

     std::cout << "\n thread num:"    << syn.Get_Thread_Number();

     std::cout << "\n";

     syn.unlock();

}

*/

void SPrint(pcynlitx::synchronizer & syn, int reputation, std::string str){

     syn.Connect("SPrint");

     syn.lock();

     std::cout << "\n";

     std::cout << "\n thread num:" << syn.Get_Thread_Number();

     std::cout << "\n";

     syn.unlock();


}
/*
void Test::RunThread(){

     pcynlitx::threads<Test> th(this,4);

     std::string str = "Hello ..";

     //void (Test::* fptr) (synchronizer &, int reputation) = &(Test::Print);

     //void (Test::* sptr) (synchronizer &, int reputation, std::string str) = &(Test::SPrint);


     //server.function(fptr,0,syn,2);

     //server.function(fptr,1,syn,1);

     th.run(Test::SPrint,0,2,str);

     th.run(Test::SPrint,1,3,str);

     th.run(Test::SPrint,2,2,str);

     th.run(Test::SPrint,3,3,str);


     th.join(0);

     th.join(1);

     th.join(2);

     th.join(3);

}
*/


int main(){

    //Test sample;

    //sample.RunThread();

    

     pcynlitx::threads<void> th(4);

     std::string str = "Hello ..";

     //void (Test::* fptr) (synchronizer &, int reputation) = &(Test::Print);

     //void (Test::* sptr) (synchronizer &, int reputation, std::string str) = &(Test::SPrint);


     //server.function(fptr,0,syn,2);

     //server.function(fptr,1,syn,1);

     th.run(SPrint,0,2,str);

     th.run(SPrint,1,3,str);

     th.run(SPrint,2,2,str);

     th.run(SPrint,3,3,str);


     th.join(0);

     th.join(1);

     th.join(2);

     th.join(3);


    return 0;
}

