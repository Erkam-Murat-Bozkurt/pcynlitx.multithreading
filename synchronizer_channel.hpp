


/*

Copyright [2025] [Erkam Murat Bozkurt]

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*/


 #ifndef SYNCHRONIZER_CHANNEL_HPP
 #define SYNCHRONIZER_CHANNEL_HPP

 #include "thread_data_holder.hpp"
 #include "thread_mutex.hpp"
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
   class synchronizer_channel
   {
    
   public:
    synchronizer_channel(){

    }

    virtual ~synchronizer_channel(){

    }

    void Receive_Messenger(channel<T> * ptr){

         this->msg = ptr;
    }


    void Receive_Synchronizer(synchronizer * ptr){

        this->syn = ptr;

        this->msg->Receive_Synchronizer(ptr);
    }

    // THE FUNCTIONS FOR INITIALIZATION  -----------------------------

    void connect(std::string Function_Name){

          this->syn->connect(Function_Name);
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

    int  number(){

         return this->syn->number();
    }

    int  operational_thread_number() const {

         return this->syn->operational_thread_number();
    }

    int  thread_pool_size() const{

         return this->syn->thread_pool_size();
    }

    bool thread_block_status(int Thread_Number){

         return this->syn->thread_block_status(Thread_Number);
    }

    void yield() {

         this->syn->yield();
    }
    
    std::string function_name(){

         return this->syn->function_name();
    }


    void send_message(int i, int j){

         if(this->number() == i){

            this->msg->set_producer(i);

            this->msg->set_consumer(j);
         }
         //this->run(i,j);
    }


    /*
    void receive_message(int i, int j){

         this->stop(i,j);
    }
    */

    // ---------------------------------------------------------------

    synchronizer * syn;
    channel<T> * msg;

    
   private: 



   };

 };

 #endif /* SYNCHRONIZER_CHANNEL_HPP */