

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


#include "thread_blocker.hpp"

pcynlitx::thread_blocker::thread_blocker(){ };



pcynlitx::thread_blocker::~thread_blocker(){ };



void pcynlitx::thread_blocker::Receive_Data_Holder(thread_data_holder * ptr){

     this->data_holder_pointer = ptr;
}


void pcynlitx::thread_blocker::stop(int Number, int Rescuer_Thread)
{
     std::unique_lock<std::mutex> Function_lock(this->mtx);

     int  Thread_Number    = this->data_holder_pointer->Get_Thread_Number();

     bool rescue_condition = this->data_holder_pointer->Get_Rescue_Permission(Rescuer_Thread);
 

     // ---------------------------------------------------------------------------------------

     if(((Thread_Number == Number) || ((Thread_Number == Rescuer_Thread) && rescue_condition))){

        this->data_holder_pointer->Increase_Wait_Enter_Counter(Number);

        if(this->data_holder_pointer->Get_Wait_Enter_Counter(Number) > 1){

            if(Thread_Number == Number){

               this->data_holder_pointer->Set_Wait_Enter_Counter(Number,0);

               this->data_holder_pointer->Activate_Thread(Rescuer_Thread);

               Function_lock.unlock();
            }
            else{

               this->data_holder_pointer->Set_Wait_Enter_Counter(Number,0);

               this->data_holder_pointer->Activate_Thread(Number);

               Function_lock.unlock();
            }
        }
        else{

               //The mutex is unlocked inside data holder stop_thread function

               if(Thread_Number == Rescuer_Thread){

                  this->data_holder_pointer->Stop_Thread(&Function_lock,Rescuer_Thread);
               }
               else{
                   
                  this->data_holder_pointer->Stop_Thread(&Function_lock,Number);
               }
        }
     }
     else{

          Function_lock.unlock();
     };
};





void pcynlitx::thread_blocker::run(int Number, int Rescuer_Thread){

     std::unique_lock<std::mutex> Function_lock(this->mtx);

     if(this->data_holder_pointer->Get_Thread_Number() == Rescuer_Thread){

        this->data_holder_pointer->Set_Rescue_Permission(Rescuer_Thread,true);

        Function_lock.unlock();
        
        this->stop(Number,Rescuer_Thread);

        this->data_holder_pointer->Set_Rescue_Permission(Rescuer_Thread,false); 
     }
     else{

           Function_lock.unlock();
     }
}





void pcynlitx::thread_blocker::stop(int Number){

     this->Locker.lock();

     pcynlitx::Thread_Data * data = this->data_holder_pointer->Find_Thread_Data_From_Number(Number);

     this->Locker.unlock();

     std::unique_lock<std::mutex> thread_lock(data->threadMutex);

     this->data_holder_pointer->Stop_Thread(&thread_lock,Number);

}





void pcynlitx::thread_blocker::run(int Number){

     this->data_holder_pointer->Activate_Thread(Number);
};




