
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <pcynlitx>

using namespace pcynlitx;



class Test
{
public:
    Test(){  }

    void RunThread();
    
    void SPrint(synchronizer_channel<std::string> & syn, int reputation, std::string str);

    void MPrint(synchronizer_channel<std::string> & syn, int reputation, std::string str);

};



void Test::SPrint(synchronizer_channel<std::string> & syn, 

     int reputation, std::string str){

     syn.lock();
     
     std::cout << "\n Thread -" << syn.number() << " created form " << syn.function_name();
     
     syn.unlock();



     if(syn.number() == 0){

          std::string s = "Hello from thread-1";

          syn.push(s);
     }

     syn.run(7,0);
}



void Test::MPrint(synchronizer_channel<std::string> & syn, 

     int reputation, std::string str){

     syn.lock();

     std::cout << "\n Thread -" << syn.number() << " created form " << syn.function_name() ;
     
     syn.unlock();


     syn.stop(7,0);


     syn.lock();

     if(syn.number() == 7){

        std::string s = syn.pop();

        std::cout << "\n The message comming from the thread 1";
        std::cout << "\n ";
        std::cout << s;
     }

     syn.unlock();

}

void Test::RunThread(){

     int thNum = 8;

     channel<std::string> ch;

     ch.set_producer(0);

     ch.set_consumer(7);

     threads<Test,std::string> th(this,8,&ch);

     std::string str = "Hello ..";


     for(int i=0;i<4;i++){

        th.create(Test::SPrint,i,"SPrint",2,str);
     }

     for(int i=4;i<8;i++){

        th.create(Test::MPrint,i,"MPrint",2,str);
     }


     for(int i=0;i<8;i++){

         th.join(i);
     }

     //std::cout << "\n the end of the program";
}


/*

void SPrint(synchronizer & syn, 

     int reputation, std::string str){

     syn.lock();
     
     std::cout << "\n Thread -" << syn.number()
     
     << " created form " << syn.function_name();
     
     syn.unlock();


}



void MPrint(synchronizer & syn, 

     int reputation, std::string str){

     syn.lock();

     std::cout << "\n Thread -" << syn.number()
     
     << " created form " << syn.function_name() ;
     
     syn.unlock();



}

*/


int main(){  

    Test sample;

    sample.RunThread();


    /*

    std::string str = "";

    for(int i=0;i<4;i++){

        th.create(SPrint,i,"SPrint",2,str);
    }

    for(int i=4;i<8;i++){

        th.create(MPrint,i,"MPrint",2,str);
    }


    for(int i=0;i<8;i++){

         th.join(i);
    }

    */

    std::cout << "\n the end of the program";


    return 0;
}

