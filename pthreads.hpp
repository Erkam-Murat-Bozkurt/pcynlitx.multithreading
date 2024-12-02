


#ifndef PTHREADS_HPP
#define PTHREADS_HPP

#include <iostream>
#include <thread>
#include <vector>
#include <synchronizer.hpp>
#include <chrono> 
#include <windows.h>


namespace pcynlitx {

  class pthreads
  {
     public:
      pthreads(int thr_num): syn(thr_num){

         this->syn.Receive_Main_Thread_Id(std::this_thread::get_id());

         this->connection_counter = 0;
      };

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
      }

      synchronizer syn;

     protected:

      std::vector<std::thread *> threadPool;

      int connection_counter;
   };
};



#endif  /* PTHREADS_HPP */