
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <pcynlitx>

using namespace pcynlitx;


/*

class Test
{
public:
    Test(){  }

    void RunThread();
    
    void SPrint(synchronizer & syn, int reputation, std::string str);

    void MPrint(synchronizer & syn, int reputation, std::string str);

};



void Test::SPrint(synchronizer & syn, 

     int reputation, std::string str){

     syn.lock();
     
     std::cout << "\n Thread -" << syn.number() << " created form " << syn.function_name();
     
     syn.unlock();


}



void Test::MPrint(synchronizer & syn, 

     int reputation, std::string str){

     syn.lock();

     std::cout << "\n Thread -" << syn.number() << " created form " << syn.function_name() ;
     
     syn.unlock();




}

void Test::RunThread(){

     int thNum = 8;

     threads<Test> th(this,thNum);

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

     std::cout << "\n the end of the program";
}

*/

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




int main(){

    threads th(8);
    
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

    std::cout << "\n the end of the program";


    return 0;
}

