


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
            
            this->thread_memory_clear_status = false;
        }


        virtual ~thread_creator(){

             this->clear_thread_pool();
        };


        void clear_thread_pool(){

             size_t thread_num = this->threadPool.size();

             if(!this->thread_memory_clear_status){

                 this->thread_memory_clear_status = true;

                 for(size_t i=0;i<thread_num;i++){

                     delete this->threadPool.at(i);
                 }

                 this->threadPool.shrink_to_fit();
             }
        }



        void receive_synchronizer(synchronizer * syn){

             this->syn_ptr = syn;

             this->syn_ptr->Receive_Main_Thread_Id(std::this_thread::get_id());

             this->syn_ptr->Receive_Operational_Thread_Number(&this->operational_thread_number);
        }


        void receive_synchronizer_mpi(synchronizer_channel<M> * syn_mpi){

             this->syn_mpi_ptr = syn_mpi;
        }



        /*  THE THREAD CREATOR INCLUDING MESSAGE PASSING CHANNEL
            FOR MEMBER FUNCTION INVOKATOIN */

        template<typename B, typename... args>
        void create(B (T::* fPtr)  (synchronizer_channel<M> & syn, args... thParams), 
      
          int thread_num, std::string function_name, args... thParams){


          this->thread_memory_clear_status = false;

          std::thread * th  = new std::thread([function_name,obj = this->objPtr,
            
                              this, s_ptr = this->syn_mpi_ptr,thread_num,fPtr,thParams...](){

                              std::thread::id th_id = std::this_thread::get_id();

                              s_ptr->Receive_Thread_ID(thread_num,th_id);

                              s_ptr->connect(function_name);


                              std::invoke(fPtr,obj,std::ref(*s_ptr),thParams...);
             
          });


          this->threadPool.push_back(th);

          this->threadPool.shrink_to_fit();
        }




        // THE THREAD CREATOR INCLUDING MESSAGE PASSING CHANNEL FOR GLOBAL FUNCTION

        template<typename B, typename... args>
        void create(B (* func_Ptr) (synchronizer_channel<M> & syn, args... thParams),  
      
          int thread_num, std::string function_name, args... thParams){

          this->thread_memory_clear_status = false;

          std::thread * th  = new std::thread([function_name,
            
                              this, s_ptr = this->syn_mpi_ptr,thread_num,func_Ptr,thParams...](){

                              std::thread::id th_id = std::this_thread::get_id();

                              s_ptr->Receive_Thread_ID(thread_num,th_id);

                              s_ptr->connect(function_name);


                              std::invoke(func_Ptr,std::ref(*s_ptr),thParams...);

                              
             
          });


          this->threadPool.push_back(th);

          this->threadPool.shrink_to_fit();
       }




       // THE THREAD CREATOR FOR MEMBER FUNCTION INVOKATION

       template<typename B, typename... args>
       void create(B (T::* fPtr)  (synchronizer & syn, args... thParams), 
      
          int thread_num, std::string function_name, args... thParams){

          this->thread_memory_clear_status = false;

          std::thread * th  = new std::thread([function_name,obj = this->objPtr,
            
                              this, s_ptr = this->syn_ptr,thread_num,fPtr,thParams...](){

                              std::thread::id th_id = std::this_thread::get_id();

                              s_ptr->Receive_Thread_ID(thread_num,th_id);

                              s_ptr->connect(function_name);


                              std::invoke(fPtr,obj,std::ref(*s_ptr),thParams...);
             
          });


          this->threadPool.push_back(th);

          this->threadPool.shrink_to_fit();
       }






       // THE THREAD CREATOR FOR GLOBAL FUNCTION INVOKATION

       template<typename B, typename... args>
       void create(B (* func_Ptr) (synchronizer & syn, args... thParams),  
      
          int thread_num, std::string function_name, args... thParams){

          this->thread_memory_clear_status = false;

          std::thread * th  = new std::thread([function_name,
            
                              this, s_ptr = this->syn_ptr,thread_num,func_Ptr,thParams...](){

                              std::thread::id th_id = std::this_thread::get_id();

                              s_ptr->Receive_Thread_ID(thread_num,th_id);

                              s_ptr->connect(function_name);


                              std::invoke(func_Ptr,std::ref(*s_ptr),thParams...);
             
          });


          this->threadPool.push_back(th);

          this->threadPool.shrink_to_fit();

        }

        
        void join(int thrNum){

           this->threadPool.at(thrNum)->join();

           this->syn_ptr->Exit(thrNum);    
                      
           if(this->operational_thread_number ==0){

              this->clear_thread_pool();
           }
        }

        T * objPtr;

        channel<M> * ch;

        synchronizer_channel<M> * syn_mpi_ptr;

        synchronizer * syn_ptr;

        int  connection_counter;
        int  total_thread_number;
        int  operational_thread_number;
        bool thread_memory_clear_status;

        std::vector<std::thread *> threadPool;

    };
}


#endif  /* THREAD_CREATOR_HPP */