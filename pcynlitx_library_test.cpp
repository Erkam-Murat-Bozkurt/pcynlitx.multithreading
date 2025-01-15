
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <pcynlitx>

using namespace pcynlitx;


class Test{
public:
    Test(){  }

    void RunThread();
    
    void SPrint(synchronizer_mpi<std::string> & syn, int reputation, std::string str);

};



void Test::SPrint(synchronizer_mpi<std::string> & syn, 

     int reputation, std::string str){

     syn.connect("SPrint");

     syn.stop(3,2);
     syn.stop(2,1);
     syn.stop(1,0);

     std::cout << "\n";

     std::cout << "\n Caller Thread Number   :" << syn.number();

     std::cout << "\n Function Name          :" << syn.GetFunctionName(syn.number());

     std::cout << "\n";


     if(syn.number() == 0){

          std::string s = "Hello";
          
          syn << s;
     }

     if(syn.number() == 3){

          std::string s;
          
          syn >> s;

          std::cout << "\n The message coming:" << s; 
     }

     syn.run(3,2);
     syn.run(2,1);
     syn.run(1,0);


}



void Test::RunThread(){

     channel<std::string> ch;

     threads<Test,std::string> th(this,4,&ch);

     std::string str = "Hello ..";


     for(int i=0;i<4;i++){

        th.activate(Test::SPrint,i,2,str);
     }

     for(int i=0;i<4;i++){

         th.join(i);
     }
}


/*

void SPrint(synchronizer_mpi<std::string> & syn, int reputation, std::string str){

     syn.connect("SPrint");

     syn.stop(3,2);
     syn.stop(2,1);
     syn.stop(1,0);



     std::cout << "\n Before function barrier";

     std::cout << "\n Caller Thread Number   :" << syn.number();

     std::cout << "\n Function Name          :" << syn.GetFunctionName(syn.number());

     std::cout << "\n Operational Thread Num :" << syn.Get_Operational_Thread_Number();

     std::cout << "\n";


     /*

     if(syn.number() == 0){

          std::string s = "Hello";
          
          syn << s;
     }

     if(syn.number() == 3){

          std::string s;
          
          syn >> s;

          std::cout << "\n The message coming:" << s; 
     }


     syn.run(3,2);
     syn.run(2,1);
     syn.run(1,0);

     

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

int main(){

    Test sample;

    sample.RunThread();

     
     /*

     //channel<std::string> ch;

     threads th(4);

     //th.syn.Receive_Messenger(&msg);

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

