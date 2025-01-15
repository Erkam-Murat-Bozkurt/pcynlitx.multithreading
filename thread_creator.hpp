

#ifndef THREAD_CREATOR_HPP
#define THREAD_CREATOR_HPP





#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <synchronizer_mpi.hpp>
#include <synchronizer.hpp>
#include <channel.hpp>
#include <chrono> 
#include <windows.h>
#include <type_traits>
#include <typeinfo>


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


        void receive_synchronizer_mpi(synchronizer_mpi<M> * syn_mpi){

             this->syn_mpi_ptr = syn_mpi;
        }


        template<typename B, typename... args>
        B activate(B (T::* fPtr)  (synchronizer_mpi<M> & syn, args... thParams), 
      
          int thread_num, args... thParams){

          std::thread * th = new std::thread(fPtr,this->objPtr,std::ref(*this->syn_mpi_ptr),thParams...);

          std::thread::id th_id = th->get_id();

          this->syn_mpi_ptr->Receive_Thread_ID(thread_num,th_id);

          this->connection_counter++;

          this->threadPool.push_back(th);

          this->threadPool.shrink_to_fit();

          if(this->connection_counter>=this->syn_ptr->GetTotalThreadNumber()){

             this->syn_ptr->connect_condition = true;

             while(!this->syn_ptr->connection_status){
            
                 Sleep(0.1);                        
             };

             this->syn_ptr->connection_wait();
          }  
        }


        template<typename B, typename... args>
        B activate(B (* func_Ptr) (synchronizer_mpi<M> & syn, args... thParams),  
      
          int thread_num, args... thParams){

          std::thread * th = new std::thread(func_Ptr,std::ref(*this->syn_mpi_ptr),thParams...);
      
          std::thread::id th_id = th->get_id();

          this->syn_mpi_ptr->Receive_Thread_ID(thread_num,th_id);

          this->connection_counter++;

          this->threadPool.push_back(th);

          if(this->connection_counter>=this->syn_ptr->GetTotalThreadNumber()){

            this->syn_ptr->connect_condition = true;

            while(!this->syn_ptr->connection_status){
            
                 Sleep(0.1);                        
            };

            this->syn_ptr->connection_wait();
         }
       }


       template<typename B, typename... args>
       B activate(B (T::* fPtr)  (synchronizer & syn, args... thParams), 
      
          int thread_num, args... thParams){

          std::thread * th = new std::thread(fPtr,this->objPtr,std::ref(*this->syn_ptr),thParams...);

          std::thread::id th_id = th->get_id();

          this->syn_ptr->Receive_Thread_ID(thread_num,th_id);

          this->connection_counter++;

          this->threadPool.push_back(th);

          this->threadPool.shrink_to_fit();

          if(this->connection_counter>=this->syn_ptr->GetTotalThreadNumber()){

             this->syn_ptr->connect_condition = true;

             while(!this->syn_ptr->connection_status){
            
                 Sleep(0.1);                        
             };

             this->syn_ptr->connection_wait();
          }  
       }


       template<typename B, typename... args>
       B activate(B (* func_Ptr) (synchronizer & syn, args... thParams),  
      
          int thread_num, args... thParams){

          std::thread * th = new std::thread(func_Ptr,std::ref(*this->syn_ptr),thParams...);
      
          std::thread::id th_id = th->get_id();

          this->syn_ptr->Receive_Thread_ID(thread_num,th_id);

          this->connection_counter++;

          this->threadPool.push_back(th);

          if(this->connection_counter>=this->syn_ptr->GetTotalThreadNumber()){

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

        synchronizer_mpi<M> * syn_mpi_ptr;

        synchronizer * syn_ptr;

        int connection_counter;
        int total_thread_number;
        int operational_thread_number;

        std::vector<std::thread *> threadPool;

    };
}


#endif  /* THREAD_CREATOR_HPP */