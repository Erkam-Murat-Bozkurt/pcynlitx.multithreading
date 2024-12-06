
#ifndef THREADS_HPP
#define THREADS_HPP

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <synchronizer.hpp>
#include <chrono> 
#include <windows.h>


namespace pcynlitx {

  template<typename T>     
  class threads
  {
     public:

      threads(T * ptr, int thr_num) : syn(thr_num){

        this->objPtr=ptr;

        this->syn.Receive_Main_Thread_Id(std::this_thread::get_id());

        this->connection_counter = 0;

        this->total_thread_number = thr_num; 

        this->operational_thread_number = thr_num;

        this->syn.Receive_Operational_Thread_Number(&this->operational_thread_number);
      };

      virtual ~threads(){

           size_t thread_num = this->threadPool.size();

           for(size_t i=0;i<thread_num;i++){

               delete this->threadPool.at(i);
           }
      }

      T * objPtr;

      template<typename B, typename... args>
      B run(B (T::* fPtr)  (synchronizer & syn, args... thParams),  int thread_num, args... thParams){

        std::thread * th = new std::thread(fPtr,this->objPtr,std::ref(this->syn),thParams...);

        std::thread::id th_id = th->get_id();

        this->syn.Receive_Thread_ID(thread_num,th_id);

        this->connection_counter++;

        this->threadPool.push_back(th);

        this->threadPool.shrink_to_fit();

        if(this->connection_counter>=this->syn.GetTotalThreadNumber()){

           this->syn.connect_condition = true;

           while(!syn.connection_status){
            
                 Sleep(0.1);                        
           };

           this->syn.connection_wait();
         }  
      }

      void join(int thrNum){

           this->threadPool.at(thrNum)->join();

           this->syn.Exit(thrNum);

           //std::unique_lock<std::mutex> exit_lock(this->mtx);

           //exit_lock.lock();

           this->operational_thread_number--;

           //exit_lock.unlock();
      }

      synchronizer syn;

     protected:

      std::vector<std::thread *> threadPool;

      int connection_counter;

      int total_thread_number;

      int operational_thread_number;

      std::mutex mtx;

   };

   template<>     
   class threads <void>
   {
      public:
      threads(int thr_num): syn(thr_num){

         this->syn.Receive_Main_Thread_Id(std::this_thread::get_id());

         this->connection_counter = 0;

         this->total_thread_number = thr_num; 

         this->operational_thread_number = thr_num;

         this->syn.Receive_Operational_Thread_Number(&this->operational_thread_number);

      };
      
      virtual ~threads(){

           size_t thread_num = this->threadPool.size();

           for(size_t i=0;i<thread_num;i++){

               delete this->threadPool.at(i);
           }
      }

      template<typename B, typename... args>
      B run(B (* func_Ptr) (synchronizer & syn, args... thParams),  int thread_num, args... thParams){

        std::thread * th = new std::thread(func_Ptr,std::ref(this->syn),thParams...);
      
        std::thread::id th_id = th->get_id();

        this->syn.Receive_Thread_ID(thread_num,th_id);

        this->connection_counter++;

        this->threadPool.push_back(th);

        if(this->connection_counter>=this->syn.GetTotalThreadNumber()){

           this->syn.connect_condition = true;

           while(!this->syn.connection_status){
            
                 Sleep(0.1);                        
           };

           this->syn.connection_wait();
        }
      }

      void join(int thrNum){

         this->threadPool.at(thrNum)->join();

         this->operational_thread_number--;
      }

      synchronizer syn;

     protected:

      std::vector<std::thread *> threadPool;

      int connection_counter;

      int total_thread_number;

      int operational_thread_number;

   };
};


#endif  /* THREADS_HPP */