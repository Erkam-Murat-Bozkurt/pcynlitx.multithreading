
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
    
    void function_1(synchronizer_channel<std::string> & syn);

    void function_2(synchronizer_channel<std::string> & syn);

};



void Test::function_1(synchronizer_channel<std::string> & syn){

     
    syn.start_serial(); 

 
    /* This function (start_serial) serialize the

    threads having the numbers varies between 0 to 3 */


    std::cout << "\n thread_id:" << syn.number();

    std::cout << "\n CRITICAL SECTION -1";


    syn.end_serial(); // This function (end_serial) ends serial execution


    

    syn.function_switch("function_1","function_2");

    syn.reset_function_switch("function_1","function_2");
}



void Test::function_2(synchronizer_channel<std::string> & syn){



     syn.function_switch("function_2","function_1");




     syn.start_serial();

     std::cout << "\n thread_id:" << syn.number();

     std::cout << "\n CRITICAL SECTION -2";

     syn.end_serial();



     //std::cout << "\n The end of MPrint";

     syn.reset_function_switch("function_1","function_2");

}

void Test::RunThread(){

     int thNum = 8;

     channel<std::string> ch;

     threads<Test,std::string> th(this,8,&ch);

     std::string str = "Hello ..";


     for(int i=0;i<4;i++){

        th.create(Test::function_1,i,"function_1");
     }

     for(int i=4;i<8;i++){

        th.create(Test::function_2,i,"function_2");
     }


     for(int i=0;i<8;i++){

         th.join(i);
     }


     ch.Clear_Channel_Stack();


     std::cout << "\n the end of the program";
     std::cout << "\n\n";
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

    for(int i=0;i<1500;i++){

        sample.RunThread();
    }



    /*

    //channel<std::string> ch;

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

*/

    std::cout << "\n the end of the program";


    return 0;
}

