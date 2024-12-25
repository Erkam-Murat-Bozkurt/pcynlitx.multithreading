
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <pcynlitx>

using namespace pcynlitx;


/*
class Test{
public:
    Test(){  }

    void RunThread();

    //void Print(synchronizer & syn);

    void Print(synchronizer & syn,  int reputation);

    void SPrint(pcynlitx::synchronizer & syn, int reputation, std::string str);

};

*/


/*
void Test::Print(synchronizer & syn){

     std::cout << "\n Hello World";
}

*/

/*

void Test::Print(synchronizer & syn, int reputation){

     syn.Connect("Print");


     //syn.wait("Print");

     syn.function_switch("Print","SPrint");


     syn.start_serial();

     syn.lock();

     std::cout << "\n";

     std::cout << "\n thread -  :"    << syn.Get_Thread_Number();

     std::cout << "\n Function -: \"Print\"";

     std::cout << "\n";

     syn.unlock();

     syn.end_serial();




     /*
     syn.lock();

     std::cout << "\n\n";

     syn.unlock();

     
     //syn.start_serial();

     syn.lock();

     std::cout << "\n";

     std::cout << "\n thread   -:"    << syn.Get_Thread_Number();

     std::cout << "\n Function -: \"Print\"";

     std::cout << "\n";

     syn.unlock();


     //syn.end_serial();


     //syn.function_switch("Print","SPrint");


}

*/


/*

void Test::SPrint(synchronizer & syn, int reputation, std::string str){

     syn.Connect("SPrint");

     syn.stop(3,2);
     syn.stop(2,1);
     syn.stop(1,0);



     syn.lock();

     std::cout << "\n Before function barrier";

     std::cout << "\n Caller Thread Number   :" << syn.Get_Thread_Number();

     std::cout << "\n Function Name          :" << syn.GetFunctionName(syn.Get_Thread_Number());

     std::cout << "\n Operational Thread Num :" << syn.Get_Operational_Thread_Number();

     std::cout << "\n";

     syn.unlock();



     syn.run(3,2);
     syn.run(2,1);
     syn.run(1,0);

     

     /*
     syn.start_serial();

     syn.lock();

     std::cout << "\n";

     std::cout << "\n thread -:" << syn.Get_Thread_Number();

     std::cout << "\n Function -: \"SPrint\"";

     std::cout << "\n";

     syn.unlock();


     syn.end_serial();

   

     syn.lock();

     std::cout << "\n\n";

     syn.unlock();

     //syn.wait("SPrint");

     syn.function_switch("Print","SPrint");

  */

     /*
     //syn.start_serial();

     //syn.lock();

     syn.lock();

     std::cout << "\n";

     std::cout << "\n thread -:" << syn.Get_Thread_Number();

     std::cout << "\n Function -: \"SPrint\"";

     std::cout << "\n";

     syn.unlock();


     //syn.end_serial();


     //syn.function_switch("Print","SPrint");


}

*/

/*

void Test::RunThread(){

     pcynlitx::threads<Test> th(this,4);

     std::string str = "Hello ..";

     //void (Test::* fptr) (synchronizer &, int reputation) = &(Test::Print);

     //void (Test::* sptr) (synchronizer &, int reputation, std::string str) = &(Test::SPrint);


     //server.function(fptr,0,syn,2);

     //server.function(fptr,1,syn,1);



     th.activate(Test::SPrint,0,2,str);

     th.activate(Test::SPrint,1,2,str);

     th.activate(Test::SPrint,2,2,str);

     th.activate(Test::SPrint,3,2,str);



     th.join(0);

     th.join(1);

     th.join(2);

     th.join(3);
}

*/


void SPrint(synchronizer & syn, int reputation, std::string str){

     syn.Connect("SPrint");

     syn.stop(3,2);
     syn.stop(2,1);
     syn.stop(1,0);



     syn.lock();

     std::cout << "\n Before function barrier";

     std::cout << "\n Caller Thread Number   :" << syn.Get_Thread_Number();

     std::cout << "\n Function Name          :" << syn.GetFunctionName(syn.Get_Thread_Number());

     std::cout << "\n Operational Thread Num :" << syn.Get_Operational_Thread_Number();

     std::cout << "\n";

     syn.unlock();



     syn.run(3,2);
     syn.run(2,1);
     syn.run(1,0);

     

     /*
     syn.start_serial();

     syn.lock();

     std::cout << "\n";

     std::cout << "\n thread -:" << syn.Get_Thread_Number();

     std::cout << "\n Function -: \"SPrint\"";

     std::cout << "\n";

     syn.unlock();


     syn.end_serial();

   

     syn.lock();

     std::cout << "\n\n";

     syn.unlock();

     //syn.wait("SPrint");

     syn.function_switch("Print","SPrint");

  */

     /*
     //syn.start_serial();

     //syn.lock();

     syn.lock();

     std::cout << "\n";

     std::cout << "\n thread -:" << syn.Get_Thread_Number();

     std::cout << "\n Function -: \"SPrint\"";

     std::cout << "\n";

     syn.unlock();


     //syn.end_serial();


     //syn.function_switch("Print","SPrint");

     */

}

int main(){

    //Test sample;

    //sample.RunThread();

     


     threads th(4);

     std::string str = "Hello ..";

     //void (Test::* fptr) (synchronizer &, int reputation) = &(Test::Print);

     //void (Test::* sptr) (synchronizer &, int reputation, std::string str) = &(Test::SPrint);


     //server.function(fptr,0,syn,2);

     //server.function(fptr,1,syn,1);

     for(int i=0;i<4;i++){

          th.activate(SPrint,i,1,str);
     }


     for(int i=0;i<4;i++){

          th.join(i);
     }

     /*

     th.join(0);

     th.join(1);

     th.join(2);

     th.join(3);

     */


    return 0;
}

