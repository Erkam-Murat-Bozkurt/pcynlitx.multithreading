
#ifndef THREADS_HPP
#define THREADS_HPP

#include <iostream>
#include <thread>
#include <vector>
#include <synchronizer.hpp>
#include <chrono> 
#include <windows.h>


namespace pcynlitx {

  template<typename T>     
  class threads
  {
     public:
      threads(int thr_num): syn(thr_num){

         this->syn.Receive_Main_Thread_Id(std::this_thread::get_id());

         this->connection_counter = 0;
      };

      threads(T * ptr, int thr_num) : syn(thr_num){

        this->objPtr=ptr;

        this->syn.Receive_Main_Thread_Id(std::this_thread::get_id());

        this->connection_counter = 0;
      };

      T * objPtr;

      template<typename B, typename... args>
      B run(B (T::* fPtr)  (synchronizer * syn, args... thParams),  int thread_num, args... thParams);

      template<typename B, typename... args>
      B run(B (* func_Ptr) (synchronizer * syn, args... thParams),  int thread_num, args... thParams);

      void join(int thrNum);

      synchronizer syn;

     protected:
      std::vector<std::thread *> threadPool;

      int connection_counter;

   };


   template<typename T> template<typename B, typename... args>
   B threads<T>::run(B (T::* func_Ptr) (synchronizer * syn, args... thParams), 
   
      int thread_num, args... thParams)
   {
      std::thread * th = new std::thread(func_Ptr,objPtr,&this->syn,thParams...);

      std::thread::id th_id = th->get_id();

      this->syn.Receive_Thread_ID(thread_num,th_id);

      this->connection_counter++;

      this->threadPool.push_back(th);

      if(this->connection_counter>=this->syn.GetTotalThreadNumber()){

         this->syn.connect_condition = true;

         while(!syn.connection_status){
            
               Sleep(0.1);                        
         };

         this->syn.connection_wait();
      }
   };



   template<typename T> template<typename B, typename... args>
   B threads<T>::run(B (* func_Ptr) (synchronizer * syn, args... thParams),  
   
      int thread_num, args... thParams){

      std::thread * th = new std::thread(func_Ptr,objPtr,&this->syn,thParams...);

      

      this->threadPool.push_back(th);
   };

   template<typename T>
   void threads<T>::join(int thrNum){

         this->threadPool.at(thrNum)->join();
   }
};




#endif  /* THREADS_HPP */