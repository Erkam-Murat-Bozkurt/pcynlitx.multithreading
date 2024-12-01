
#ifndef THREAD_SERVER_HPP
#define THREAD_SERVER_HPP

#include <iostream>
#include <thread>
#include <vector>
#include <synchronizer.hpp>
#include <chrono> 
#include <windows.h>


namespace pcynlitx {

  template<typename T>     
  class thread_server
  {
     public:
      thread_server(int thr_num): syn(thr_num){

         this->syn.Receive_Main_Thread_Id(std::this_thread::get_id());

         this->connection_counter = 0;
      };

      thread_server(T * ptr, int thr_num) : syn(thr_num){

        this->objPtr=ptr;

        this->syn.Receive_Main_Thread_Id(std::this_thread::get_id());

        this->connection_counter = 0;
      };

      T * objPtr;

      template<typename B, typename... args>
      B function(B (T::* fPtr)  (synchronizer * syn, args... thParams),  int thread_num, args... thParams);

      template<typename B, typename... args>
      B function(B (* func_Ptr) (synchronizer * syn, args... thParams),  int thread_num, args... thParams);

      void join(int thrNum);

      synchronizer syn;

     protected:
      std::vector<std::thread *> threadPool;

      int connection_counter;

   };


   template<typename T> template<typename B, typename... args>
   B thread_server<T>::function(B (T::* func_Ptr) (synchronizer * syn, args... thParams), 
   
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
   B thread_server<T>::function(B (* func_Ptr) (synchronizer * syn, args... thParams),  
   
      int thread_num, args... thParams){

      std::thread * th = new std::thread(func_Ptr,objPtr,&this->syn,thParams...);

      

      this->threadPool.push_back(th);
   };

   template<typename T>
   void thread_server<T>::join(int thrNum){

         this->threadPool.at(thrNum)->join();
   }
};




#endif  /* THREAD_SERVER_HPP */