

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

#include "synchronizer.hpp"

pcynlitx::synchronizer::synchronizer(int thrNum)
{
   this->total_thread_number = thrNum;

   this->data_holder.Receive_Total_Thread_Number(thrNum);

   this->fnblocker.Receive_Data_Holder(&this->data_holder);

   this->thrblocker.Receive_Data_Holder(&this->data_holder);

   this->operational_thr_num = nullptr;

   this->waiting_thread_number_in_barrier = 0;

   this->connection_status = false;

   this->connect_condition = false;

   this->clear_memory_status = false;
};



pcynlitx::synchronizer::~synchronizer(){


};

void pcynlitx::synchronizer::clear_memory(){

      if(!this->clear_memory_status){

          this->clear_memory_status = true;
          
          this->data_holder.clear_memory();
      }
      
}

void pcynlitx::synchronizer::Receive_Thread_ID(int Thread_Number, std::thread::id id_num){

     this->Inside_Locker.lock();

     this->data_holder.Receive_Thread_ID(Thread_Number,id_num);

     this->Inside_Locker.unlock();
};


void pcynlitx::synchronizer::lock()
{
     this->Outside_Locker.lock();
}

void pcynlitx::synchronizer::unlock()
{
     this->Outside_Locker.unlock();

}

void pcynlitx::synchronizer::connect(std::string Function_Name){

     this->connection_wait();



     this->Inside_Locker.lock();

     int thrd_num = this->data_holder.Get_Thread_Number();

     this->data_holder.Add_Function_Data(Function_Name,thrd_num);

     this->data_holder.Set_Function_Name_To_Thread_Data(thrd_num,Function_Name);
     
     this->Inside_Locker.unlock();




     this->barrier_wait();


     
     this->Inside_Locker.lock();

     pcynlitx::Function_Member_Data * fmem_data = 
     
          this->data_holder.Find_Function_Member_Data_From_Name(Function_Name);

     fmem_data->threadNumbers.shrink_to_fit();


     if(this->data_holder.Get_Thread_Number() == 0){

         std::sort(fmem_data->threadNumbers.begin(),fmem_data->threadNumbers.end(),
    
            [](int a, int b){ return a < b;});
     }

     this->Inside_Locker.unlock();



     this->barrier_wait();


};


void pcynlitx::synchronizer::Receive_Operational_Thread_Number(int * thrNum)
{    
     this->operational_thr_num = thrNum;
     
     this->data_holder.Receive_Operational_Thread_Number(thrNum);
}    

void pcynlitx::synchronizer::Receive_Main_Thread_Id(std::thread::id main_id){

     this->main_thread_id = main_id;
}




void pcynlitx::synchronizer::barrier_wait(){

     std::unique_lock<std::mutex> barrier_wait_lock(this->mtx_barrier_wait);

     this->waiting_thread_number_in_barrier++;

     if(this->waiting_thread_number_in_barrier < this->total_thread_number){

        this->cv.wait(barrier_wait_lock);

        barrier_wait_lock.unlock();

     }
     else{

           barrier_wait_lock.unlock();

           this->waiting_thread_number_in_barrier = 0;

           this->cv.notify_all();
     }
};

void pcynlitx::synchronizer::function_switch(std::string function_1, std::string function_2){

     pcynlitx::Function_Member_Data * fdata_1 = this->data_holder.Find_Function_Member_Data_From_Name(function_1);

     pcynlitx::Function_Member_Data * fdata_2 = this->data_holder.Find_Function_Member_Data_From_Name(function_2);
     
     if(fdata_2->function_switch_block_status){

        this->run(function_2,fdata_1->threadNumbers.at(0));

        
        this->lock();

        if(this->number() == fdata_1->threadNumbers.at(0)){

           fdata_2->function_switch_block_status = false;
        }

        this->unlock();
     }

     fdata_1->function_switch_block_status = true;

     this->stop(function_1,fdata_2->threadNumbers.at(0));
}


void pcynlitx::synchronizer::reset_function_switch(std::string function_1, std::string function_2){

     pcynlitx::Function_Member_Data * fdata_1 = this->data_holder.Find_Function_Member_Data_From_Name(function_1);

     pcynlitx::Function_Member_Data * fdata_2 = this->data_holder.Find_Function_Member_Data_From_Name(function_2);

     if(this->function_name()==function_2){

        if(fdata_1->function_switch_block_status){

           this->run(function_1,fdata_2->threadNumbers.at(0));

           
           this->lock();

           if(this->number() == fdata_2->threadNumbers.at(0)){

              fdata_1->function_switch_block_status = false;
           }

           this->unlock();
        }
     }

     if(this->function_name()==function_1){

        if(fdata_2->function_switch_block_status){

           this->run(function_2,fdata_1->threadNumbers.at(0));


           this->lock();

           if(this->number() == fdata_1->threadNumbers.at(0)){

               fdata_2->function_switch_block_status = false;
           }

           this->unlock();
        }
     }
}



void pcynlitx::synchronizer::connection_wait(){

     std::unique_lock<std::mutex> wait_lock(this->mtx_barrier_wait);

     wait_lock.unlock();


     wait_lock.lock();

     if( this->Connection_Wait_Counter < this->total_thread_number-1){

        if(std::this_thread::get_id() != this->main_thread_id){

          this->Connection_Wait_Counter++;

          this->cv.wait(wait_lock);
        }

        wait_lock.unlock();
     }
     else{

           wait_lock.unlock();

           this->cv.notify_all();
     }    


     wait_lock.lock();

     this->Connection_Wait_Counter = 0;

     wait_lock.unlock();
};


void pcynlitx::synchronizer::stop(int Number, int Rescuer_Thread)
{
     this->thrblocker.stop(Number,Rescuer_Thread);

};


void pcynlitx::synchronizer::run(int Number, int Rescuer_Thread)
{
     this->thrblocker.run(Number,Rescuer_Thread);
}




void pcynlitx::synchronizer::stop(int Number){

     this->thrblocker.stop(Number);
}



void pcynlitx::synchronizer::run(int Number){

     this->thrblocker.run(Number);     
};




void pcynlitx::synchronizer::start_serial(){

     this->Inside_Locker.lock();

     std::string Function_Name = this->data_holder.Get_Function_Name(this->data_holder.Get_Thread_Number());

     pcynlitx::Function_Member_Data * fdata = this->data_holder.Find_Function_Member_Data_From_Name(Function_Name);

     this->Inside_Locker.unlock();


     // --------------------------------------------------------------------------------------------------

     for(size_t i=fdata->threadNumbers.size()-1;i>0;i--){

         this->stop(i,i-1);
     } 
};



void pcynlitx::synchronizer::end_serial(){
     
     this->Inside_Locker.lock();

     std::string Function_Name = this->data_holder.Get_Function_Name(this->data_holder.Get_Thread_Number());

     pcynlitx::Function_Member_Data * fdata = this->data_holder.Find_Function_Member_Data_From_Name(Function_Name);

     this->Inside_Locker.unlock();


     // --------------------------------------------------------------------------------------------------

     for(size_t i=fdata->threadNumbers.size()-1;i>0;i--){

         this->run(i,i-1);
     }

};




void pcynlitx::synchronizer::stop(std::string Function_Name){

     this->fnblocker.stop(Function_Name);
}



void pcynlitx::synchronizer::stop(std::string Function_Name, int Rescuer_Thread){

     this->fnblocker.stop(Function_Name,Rescuer_Thread);
}



void pcynlitx::synchronizer::run(std::string Function_Name, int Rescuer_Thread){

     this->fnblocker.run(Function_Name,Rescuer_Thread);
}


void pcynlitx::synchronizer::yield(){

     std::this_thread::yield;
};



int pcynlitx::synchronizer::thread_pool_size() const {

    return this->total_thread_number;
}


int pcynlitx::synchronizer::number(){

    return this->data_holder.Get_Thread_Number();
};


std::string pcynlitx::synchronizer::function_name(){

     return this->data_holder.Get_Function_Name(this->number());
}


bool pcynlitx::synchronizer::thread_block_status(int Thread_Number) {

     return this->data_holder.Get_Thread_Block_Status(Thread_Number);
};


int  pcynlitx::synchronizer::operational_thread_number() const
{
     return *this->operational_thr_num;
};


void pcynlitx::synchronizer::Exit(int thrNum){

     this->data_holder.Exit(thrNum);
};
