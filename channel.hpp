
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


 #ifndef CHANNEL_HPP
 #define CHANNEL_HPP


 #include <mutex>
 #include <iostream>
 #include <string>
 #include <cstdlib>
 #include <chrono>
 #include <condition_variable>
 #include <algorithm>
 #include <queue>
 #include <synchronizer.hpp>
 #include <vector>
 #include <map>

 namespace pcynlitx
 {
   template <typename T>
   class channel {
   
     public:

     channel(){

        this->producer_set_condition = false;

        this->consumer_set_condition = false;
     }

     virtual ~channel(){

         this->Reset_Channel();
     }

     void Clear_Channel_Stack(){

         if(!this->producer_threads.empty()){

             this->producer_threads.clear();
         }

         if(!this->consumer_threads.empty()){
            
             this->consumer_threads.clear();
         }

         for(int i=0;this->m_queue.size()>0;i++){

             this->m_queue.pop();
         }
     }

     void Reset_Channel(){

          this->producer_set_condition = false;

          this->consumer_set_condition = false;

          this->Clear_Channel_Stack();
     }

     void Receive_Synchronizer(synchronizer * syn){

          this->syn_ptr = syn;
     }

     void set_producer(int thrNum){

         this->producer_set_condition = true;

         std::string prod = "procuder_thread";

         this->producer_threads.push_back(thrNum);

         this->producer_threads.shrink_to_fit();
     }

     void set_consumer(int thrNum){

          this->consumer_set_condition = true;

          std::string cons = "consumer_thread";

          this->consumer_threads.push_back(thrNum);

          this->consumer_threads.shrink_to_fit();
     }


     // Pushes an element to the queue
     void push(T item)
     {

        // Acquire lock
        std::unique_lock<std::mutex> lock(m_mutex);

        if(this->producer_set_condition){

           int thr_num = this->syn_ptr->number();

           if(this->Is_Exist_On_Producer_Stack(thr_num)){

              // Add item
              m_queue.push(item);

              // Notify one thread that
              // is waiting
               m_cond.notify_one();
            }
            else{

                 std::cout << "\n Unauthorized acess to the pipe";

                 exit(EXIT_FAILURE);
            }
        }
        else{

              
              // Add item
              this->m_queue.push(item);

              // Notify one thread that
              // is waiting
               m_cond.notify_one();
        }
     }

     // Pops an element off the queue
     T pop()
     {
        // acquire lock
        std::unique_lock<std::mutex> lock(m_mutex);

        // wait until queue is not empty
        m_cond.wait(lock, [this]() { return !m_queue.empty(); });

        if(this->consumer_set_condition){

           // retrieve item

           int thr_num = this->syn_ptr->number();

           if(this->Is_Exist_On_Consumer_Stack(thr_num)){

              T item = m_queue.front();
              m_queue.pop();

              // return item
              return item;
            }
            else{

                 std::cout << "\n Unauthorized acess to the pipe";

                 exit(EXIT_FAILURE);
            }
        }
        else{

              T item = m_queue.front();
              m_queue.pop();

              // return item
              return item;
        }
     
     }

    private:

     bool Is_Exist_On_Producer_Stack(int thrNum){
     
         bool is_exist = false;

         for(size_t i=0;i<this->producer_threads.size();i++){

             if(thrNum == this->producer_threads.at(i)){

                 is_exist = true;

                 break;
             }
         }

         return is_exist;
     }


     bool Is_Exist_On_Consumer_Stack(int thrNum){
     
          bool is_exist = false;

          for(size_t i=0;i<this->consumer_threads.size();i++){

             if(thrNum == this->consumer_threads.at(i)){

                 is_exist = true;

                 break;
             }
          }

          return is_exist;
     }



     // Underlying queue
     std::queue<T> m_queue;

     // mutex for thread synchronization
     std::mutex m_mutex;

     // Condition variable for signaling
     std::condition_variable m_cond;

     bool producer_set_condition;

     bool consumer_set_condition;

     std::vector<int> producer_threads;

     std::vector<int> consumer_threads;

     synchronizer * syn_ptr;

 };

   
 };

 #endif /* CHANNEL_HPP */