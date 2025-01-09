



 #ifndef SYNCHRONIZER_MPI_HPP
 #define SYNCHRONIZER_MPI_HPP

 #include "thread_data_holder.hpp"
 #include "thread_locker.hpp"
 #include <thread>
 #include <mutex>
 #include <iostream>
 #include <string>
 #include <cstdlib>
 #include <chrono>
 #include <condition_variable>
 #include <algorithm>
 #include <channel.hpp>
 #include <synchronizer.hpp>


 namespace pcynlitx{

   template<typename T=int>
   class synchronizer_mpi
   {
    
   public:
    synchronizer_mpi(){

    }

    virtual ~synchronizer_mpi(){

    }

    void Receive_Messenger(channel<T> * ptr){

         this->msg = ptr;
    }


    void Receive_Synchronizer(synchronizer * ptr){

        this->syn = ptr;
    }

    // THE FUNCTIONS FOR INITIALIZATION  -----------------------------

    void Connect(std::string Function_Name){

          this->syn->Connect(Function_Name);
    }

    void connection_wait(){

         this->syn->connection_wait();
    }

    void Receive_Thread_ID(int Thread_Number, std::thread::id id_num){

         this->syn->Receive_Thread_ID(Thread_Number,id_num);
    }


    void Receive_Main_Thread_Id(std::thread::id main_id){

         this->syn->Receive_Main_Thread_Id(main_id);
    }

    void Receive_Operational_Thread_Number(int * threadNum){

         this->syn->Receive_Operational_Thread_Number(threadNum);
    }

    void Exit(int thrNum){

         this->syn->Exit(thrNum);
    }

    void push(T item){

         this->msg->push(item);
    }

    T pop(){

       return this->msg->pop();
    }

    void operator << (T item){

        this->push(item);
    }

    void operator >> (T & out)
    {
        out = this->pop();
    }



    // ---------------------------------------------------------------



    // THREAD CONTROL FUNCTIONS --------------------------------------

    void lock(){

         this->syn->lock();
    }

    void unlock() {

         this->syn->unlock();
    }

    void barrier_wait(){ // Barrier for each thread

         this->syn->barrier_wait();
    }   
    
    void stop(int Number){

         this->syn->stop(Number);
    }

    void run(int Number){

         this->syn->run(Number);
    }

    void start_serial(){

         this->syn->start_serial();
    }
    
    void end_serial(){

         this->syn->end_serial();
    }
    
    void stop(int Number, int Rescuer_Thread){

         this->syn->stop(Number,Rescuer_Thread);
    }

    void run(int Number, int Rescuer_Thread_Number){

         this->syn->run(Number,Rescuer_Thread_Number);
    }

    void stop(std::string Function_Name, int Rescuer_Thread_Number){

         this->syn->stop(Function_Name,Rescuer_Thread_Number);
    }

    void run(std::string Function_Name, int Rescuer_Thread_Number){

         this->syn->run(Function_Name,Rescuer_Thread_Number);
    }

    void stop(std::string Function_Name){

         this->syn->stop(Function_Name);
    }
    
    // Barrier for the threads executing particular function 
    
    void function_switch(std::string function_1, std::string function_2){

         this->syn->function_switch(function_1,function_2);
    }

    void reset_function_switch(std::string function_1, std::string function_2){

         this->syn->reset_function_switch(function_1,function_2);
    }

    int  Get_Thread_Number(){

         return this->syn->Get_Thread_Number();
    }

    int  Get_Operational_Thread_Number() const {

         return this->syn->Get_Operational_Thread_Number();
    }

    int  GetTotalThreadNumber() const{

         return this->syn->GetTotalThreadNumber();
    }

    bool Get_Thread_Block_Status(int Thread_Number){

         return this->syn->Get_Thread_Block_Status(Thread_Number);
    }

    void yield() {

         this->syn->yield();
    }
    
    std::string GetFunctionName(int thread_num){

         return this->syn->GetFunctionName(thread_num);
    }


    // ---------------------------------------------------------------

    synchronizer * syn;
    channel<T> * msg;

    
   private: 



   };

 };

 #endif /* SYNCHRONIZER_MPI_HPP */