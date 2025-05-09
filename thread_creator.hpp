

#ifndef THREAD_CREATOR_HPP
#define THREAD_CREATOR_HPP





#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <synchronizer_channel.hpp>
#include <synchronizer.hpp>
#include <channel.hpp>
#include <chrono> 
#include <windows.h>
#include <type_traits>
#include <typeinfo>
#include <functional>
#include <utility>

namespace pcynlitx {

    template <typename T, typename M>
    class thread_creator
    {
        public:
        thread_creator(int thr_num){
  
           this->initialize_members(thr_num);
        }

        thread_creator(int thr_num, channel<M> * ch){

           this->ch = ch;

           this->initialize_members(thr_num);
        }

        thread_creator(T * ptr, int thr_num){

           this->objPtr = ptr;

           this->initialize_members(thr_num);
        }


        thread_creator(T * ptr, int thr_num, channel<M> * ch){

           this->objPtr = ptr;

           this->ch = ch;

           this->initialize_members(thr_num);
        }


        void initialize_members(int thr_num){

            this->connection_counter = 0;

            this->total_thread_number = thr_num; 

            this->operational_thread_number = thr_num;  
        }


        virtual ~thread_creator(){

           size_t thread_num = this->threadPool.size();

           for(size_t i=0;i<thread_num;i++){

               delete this->threadPool.at(i);
           }
        };

        void receive_synchronizer(synchronizer * syn){

             this->syn_ptr = syn;

             this->syn_ptr->Receive_Main_Thread_Id(std::this_thread::get_id());

             this->syn_ptr->Receive_Operational_Thread_Number(&this->operational_thread_number);
        }


        void receive_synchronizer_mpi(synchronizer_channel<M> * syn_mpi){

             this->syn_mpi_ptr = syn_mpi;
        }


        template<typename B, typename... args>
        B create(B (T::* fPtr)  (synchronizer_channel<M> & syn, args... thParams), 
      
          int thread_num, args... thParams){

          std::thread * th = new std::thread(fPtr,this->objPtr,std::ref(*this->syn_mpi_ptr),thParams...);

          std::thread::id th_id = th->get_id();

          this->syn_mpi_ptr->Receive_Thread_ID(thread_num,th_id);

          this->connection_counter++;

          this->threadPool.push_back(th);

          this->threadPool.shrink_to_fit();

          if(this->connection_counter>=this->syn_ptr->thread_pool_size()){

             this->syn_ptr->connect_condition = true;

             while(!this->syn_ptr->connection_status){
            
                 Sleep(0.1);                        
             };

             this->syn_ptr->connection_wait();
          }  
        }


        template<typename B, typename... args>
        B create(B (* func_Ptr) (synchronizer_channel<M> & syn, args... thParams),  
      
          int thread_num, args... thParams){

          std::thread * th = new std::thread(func_Ptr,std::ref(*this->syn_mpi_ptr),thParams...);
      
          std::thread::id th_id = th->get_id();

          this->syn_mpi_ptr->Receive_Thread_ID(thread_num,th_id);

          this->connection_counter++;

          this->threadPool.push_back(th);

          if(this->connection_counter>=this->syn_ptr->thread_pool_size()){

            this->syn_ptr->connect_condition = true;

            while(!this->syn_ptr->connection_status){
            
                 Sleep(0.1);                        
            };

            this->syn_ptr->connection_wait();
         }
       }


       template<typename B, typename... args>
       B create(B (T::* fPtr)  (synchronizer & syn, args... thParams), 
      
          int thread_num, std::string func_name, args... thParams){


          /*
          std::unique_lock<std::mutex> lamda_lock(this->mtx);

          std::cout << "\n func_name:" << func_name;

          lamda_lock.unlock();


          std::string name = func_name;

          std::unique_lock<std::mutex> * lock_ptr = &lamda_lock;

          */

          T * obj = this->objPtr;

          synchronizer * s_ptr = this->syn_ptr;


          std::thread * th;

          //int * counter = &this->connection_counter;

          th = new std::thread([func_name,obj,this,s_ptr,thread_num,fPtr,thParams...](){

                   std::thread::id th_id = std::this_thread::get_id();

                   s_ptr->Receive_Thread_ID(thread_num,th_id);

                   s_ptr->connect(func_name);


                   //lock_ptr->lock();

                   //(*counter)++;

                   //lock_ptr->unlock();


                   /**/


                   /*
                   if(*counter>=s_ptr->thread_pool_size()){


                       s_ptr->connect_condition = true;

                       while(!s_ptr->connection_status){
            
                          Sleep(0.1);                        
                       };



                       s_ptr->connection_wait();
                   }
                     
                   */

                   std::invoke(fPtr,obj,std::ref(*s_ptr),thParams...);
             
               });


          this->threadPool.push_back(th);

          this->threadPool.shrink_to_fit();

          /*

          std::thread * th = new std::thread(fPtr,this->objPtr,std::ref(*this->syn_ptr),thParams...);

          std::thread::id th_id = th->get_id();

          this->syn_ptr->Receive_Thread_ID(thread_num,th_id);

          this->connection_counter++;

          this->threadPool.push_back(th);

          this->threadPool.shrink_to_fit();

          if(this->connection_counter>=this->syn_ptr->thread_pool_size()){

             this->syn_ptr->connect_condition = true;

             while(!this->syn_ptr->connection_status){
            
                 Sleep(0.1);                        
             };

             this->syn_ptr->connection_wait();
          }  
             */
       }


       template<typename B, typename... args>
       B create(B (* func_Ptr) (synchronizer & syn, args... thParams),  
      
          int thread_num, args... thParams){

          std::thread * th = new std::thread(func_Ptr,std::ref(*this->syn_ptr),thParams...);
      
          std::thread::id th_id = th->get_id();

          this->syn_ptr->Receive_Thread_ID(thread_num,th_id);

          this->connection_counter++;

          this->threadPool.push_back(th);

          if(this->connection_counter>=this->syn_ptr->thread_pool_size()){

             this->syn_ptr->connect_condition = true;

             while(!this->syn_ptr->connection_status){
            
                 Sleep(0.1);                        
             };

             this->syn_ptr->connection_wait();
          }
        }

        void join(int thrNum){

           this->threadPool.at(thrNum)->join();

           this->syn_ptr->Exit(thrNum);            
        }

        T * objPtr;

        channel<M> * ch;

        synchronizer_channel<M> * syn_mpi_ptr;

        synchronizer * syn_ptr;

        std::mutex mtx;

        int connection_counter;
        int total_thread_number;
        int operational_thread_number;

        std::vector<std::thread *> threadPool;

    };
}


#endif  /* THREAD_CREATOR_HPP */