

#include "synchronizer.hpp"

pcynlitx::synchronizer::synchronizer(int thrNum)
{
   this->Total_Thread_Number = thrNum;

   this->data_holder.Receive_Total_Thread_Number(thrNum);

   this->operational_thread_number = nullptr;

   this->waiting_thread_number_in_barrier = 0;

   this->connection_status = false;

   this->connect_condition = false;
};



pcynlitx::synchronizer::~synchronizer()
{


};

void pcynlitx::synchronizer::lock()
{
     this->Outside_Locker.lock();
}

void pcynlitx::synchronizer::unlock()
{
     this->Outside_Locker.unlock();

}

void pcynlitx::synchronizer::Connect(std::string Function_Name){

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
     this->operational_thread_number = thrNum;
     
     this->data_holder.Receive_Operational_Thread_Number(thrNum);
}    

void pcynlitx::synchronizer::Receive_Main_Thread_Id(std::thread::id main_id){

     this->main_thread_id = main_id;
}

void pcynlitx::synchronizer::barrier_wait(){

     std::unique_lock<std::mutex> barrier_wait_lock(this->mtx_barrier_wait);

     this->waiting_thread_number_in_barrier++;

     if(this->waiting_thread_number_in_barrier < this->Total_Thread_Number){

        this->cv.wait(barrier_wait_lock);

        barrier_wait_lock.unlock();

     }
     else{

           barrier_wait_lock.unlock();

           this->waiting_thread_number_in_barrier = 0;

           for(int i=0;i<(this->Total_Thread_Number)-1;i++){

               this->cv.notify_one();
           }
     }
};


void pcynlitx::synchronizer::connection_wait(){

     std::unique_lock<std::mutex> wait_lock(this->mtx_barrier_wait);

     if(std::this_thread::get_id() != this->main_thread_id){

        this->Connection_Wait_Counter++;

        if(this->Connection_Wait_Counter >= this->Total_Thread_Number){

          this->connection_status = true;
        }
     }

     wait_lock.unlock();



     wait_lock.lock();

     if(!this->connect_condition || !this->connection_status){

        if(std::this_thread::get_id() != this->main_thread_id){

           this->cv.wait(wait_lock);
        }

        wait_lock.unlock();
     }
     else{

           wait_lock.unlock();

           this->Connection_Wait_Counter = 0;

           this->connect_condition = false;

           for(int i=0;i<this->Total_Thread_Number;i++){

               this->cv.notify_one();
           }
     }     
};


void pcynlitx::synchronizer::stop(int Number, int Rescuer_Thread)
{
     std::unique_lock<std::mutex> Function_lock(this->mtx_two_parameter_wait);


     int Thread_Number = this->data_holder.Get_Thread_Number();

     bool rescue_condition = this->data_holder.Get_Rescue_Permission(Rescuer_Thread);

     // ---------------------------------------------------------------------------------------

     if(((Thread_Number == Number) || ((Thread_Number == Rescuer_Thread) && rescue_condition))){

        this->data_holder.Increase_Wait_Enter_Counter(Number);

        if(this->data_holder.Get_Wait_Enter_Counter(Number) > 1){

            if(Thread_Number == Number){

               this->data_holder.Set_Wait_Enter_Counter(Number,0);

               this->data_holder.Activate_Thread(Rescuer_Thread);

               Function_lock.unlock();
            }
            else{

               this->data_holder.Set_Wait_Enter_Counter(Number,0);

               this->data_holder.Activate_Thread(Number);

               Function_lock.unlock();
            }
        }
        else{
               if(Thread_Number == Rescuer_Thread){

                  this->data_holder.Stop_Thread(&Function_lock,Rescuer_Thread);
               }
               else{

                  this->data_holder.Stop_Thread(&Function_lock,Number);
               }
        }
     }
     else{

          Function_lock.unlock();
     };
};


void pcynlitx::synchronizer::run(int Number, int Rescuer_Thread){

     std::unique_lock<std::mutex> Function_lock(this->mtx_two_parameter_wait);

     if(this->data_holder.Get_Thread_Number() == Rescuer_Thread){

        this->data_holder.Set_Rescue_Permission(Rescuer_Thread,true);

        Function_lock.unlock();

        this->stop(Number,Rescuer_Thread);

        this->data_holder.Set_Rescue_Permission(Rescuer_Thread,false); 
     }
     else{

           Function_lock.unlock();
     }
}

void pcynlitx::synchronizer::stop(int Number){

     this->Inside_Locker.lock();

     pcynlitx::Thread_Data * data = this->data_holder.Find_Thread_Data_From_Number(Number);

     this->Inside_Locker.unlock();

     std::unique_lock<std::mutex> thread_lock(data->threadMutex);

     this->data_holder.Stop_Thread(&thread_lock,Number);

     thread_lock.unlock();
}

void pcynlitx::synchronizer::run(int Number){

     this->data_holder.Activate_Thread(Number);
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

     this->Inside_Locker.lock();

     pcynlitx::Function_Names_Data * fdata = this->data_holder.Find_Function_Data_From_Name(Function_Name);

     this->Inside_Locker.unlock();

     // --------------------------------------------------------------------------------------------------

     std::unique_lock<std::mutex> Function_lock(fdata->Function_Barrier_Mutex);

     int Thread_Number = this->data_holder.Get_Thread_Number();

     if(this->data_holder.Get_Function_Name(Thread_Number) == Function_Name){

        int Function_Member_Number = this->data_holder.Get_Function_Member_Number(Function_Name);

        this->data_holder.Increase_Function_Wait_Enter_Counter(Function_Name);

        if(this->data_holder.Get_Function_Wait_Enter_Counter(Function_Name) < Function_Member_Number){

           this->data_holder.Stop_Thread(&Function_lock,Thread_Number);

           Function_lock.unlock();
        }
        else{
               this->data_holder.Set_Function_Wait_Enter_Counter(Function_Name,0);

               this->data_holder.Rescue_Function_Members(Function_Name);

               Function_lock.unlock(); 
         }

     }
     else{

           Function_lock.unlock();

     }
}


void pcynlitx::synchronizer::stop(std::string Function_Name, int Rescuer_Thread){

     this->Inside_Locker.lock();

     pcynlitx::Function_Names_Data * fdata = this->data_holder.Find_Function_Data_From_Name(Function_Name);

     this->Inside_Locker.unlock();


     // --------------------------------------------------------------------------------------------------

     std::unique_lock<std::mutex> Function_lock(fdata->Function_Mutex);

     int Thread_Number = this->data_holder.Get_Thread_Number();


     if(((Thread_Number == Rescuer_Thread ) || (this->data_holder.Get_Function_Name(Thread_Number) == Function_Name))){

        if(Thread_Number == Rescuer_Thread){

           int status = 0;

           this->data_holder.Set_Block_Function_Wait_Status(Function_Name,status);

           this->data_holder.Set_Function_Rescue_Permission(Function_Name,true);

         }

         this->data_holder.Increase_Function_Wait_Enter_Counter(Function_Name);

         int Member_Number = this->data_holder.Get_Function_Member_Number(Function_Name);

         int Enter_Counter = this->data_holder.Get_Function_Wait_Enter_Counter(Function_Name);

         bool Rescue_Permission = this->data_holder.Get_Function_Rescue_Permission(Function_Name);



         if(((Enter_Counter == (Member_Number+1)) && Rescue_Permission )){

            this->data_holder.Set_Function_Wait_Enter_Counter(Function_Name,0);

            if(this->data_holder.Get_Thread_Number() != Rescuer_Thread){

               this->data_holder.Activate_Thread(Rescuer_Thread);
            }

            this->data_holder.Rescue_Function_Members(Function_Name);

            Function_lock.unlock();
         }
         else{
 
               int status = 1;

               this->data_holder.Set_Block_Function_Wait_Status(Function_Name,status);

               this->data_holder.Stop_Thread(&Function_lock,Thread_Number);   // Thread Stop Point

               Function_lock.unlock();
         }
      }
      else{

          Function_lock.unlock();

      }
}


void pcynlitx::synchronizer::run(std::string Function_Name, int Rescuer_Thread){

     this->Inside_Locker.lock();

     pcynlitx::Function_Names_Data * fdata = this->data_holder.Find_Function_Data_From_Name(Function_Name);

     this->Inside_Locker.unlock();

     std::unique_lock<std::mutex> Function_lock(fdata->Function_Mutex);

     if(this->data_holder.Get_Thread_Number() == Rescuer_Thread){

        Function_lock.unlock();

        this->stop(Function_Name,Rescuer_Thread);

        this->data_holder.Set_Rescue_Permission(Rescuer_Thread,false);
     }
     else{
            Function_lock.unlock();
     }
}


void pcynlitx::synchronizer::Exit(int thrNum){

     this->data_holder.Exit(thrNum);
};

void pcynlitx::synchronizer::Receive_Thread_ID(int Thread_Number, std::thread::id id_num){

     this->Inside_Locker.lock();

     this->data_holder.Receive_Thread_ID(Thread_Number,id_num);

     this->Inside_Locker.unlock();
};

int pcynlitx::synchronizer::Get_Thread_Number(){

    return this->data_holder.Get_Thread_Number();
};

std::string pcynlitx::synchronizer::GetFunctionName(int thread_num){

     return this->data_holder.Get_Function_Name(thread_num);
}

bool pcynlitx::synchronizer::Get_Thread_Block_Status(int Thread_Number) {

     return this->data_holder.Get_Thread_Block_Status(Thread_Number);
};


int  pcynlitx::synchronizer::Get_Operational_Thread_Number() const
{
     return *this->operational_thread_number;
};


void pcynlitx::synchronizer::function_switch(std::string function_1, std::string function_2){

     pcynlitx::Thread_Data * th_data = this->data_holder.Find_Thread_Data_From_Number(this->data_holder.Get_Thread_Number());

     this->stop(th_data->Thread_Function_Name);

     pcynlitx::Function_Member_Data * data_f1 =  this->data_holder.Find_Function_Member_Data_From_Name(function_1);

     pcynlitx::Function_Member_Data * data_f2 =  this->data_holder.Find_Function_Member_Data_From_Name(function_2);


     int thr_num_func_1 = data_f1->threadNumbers.at(0);

     int thr_num_func_2 = data_f2->threadNumbers.at(0);


     int func_2_block_status = this->data_holder.Get_Block_Function_Wait_Status(function_2);

     if(func_2_block_status > 0){

        this->run(function_2,thr_num_func_1);
     };

     this->stop(function_1,thr_num_func_2);
};


void pcynlitx::synchronizer::reset_function_switch(std::string function_1, std::string function_2){;


     pcynlitx::Function_Member_Data * data_f1 =  this->data_holder.Find_Function_Member_Data_From_Name(function_1);

     pcynlitx::Function_Member_Data * data_f2 =  this->data_holder.Find_Function_Member_Data_From_Name(function_2);


     int thr_num_func_1 = data_f1->threadNumbers.at(0);

     int thr_num_func_2 = data_f2->threadNumbers.at(0);



     int func_1_block_status = this->data_holder.Get_Block_Function_Wait_Status(function_1);

     int func_2_block_status = this->data_holder.Get_Block_Function_Wait_Status(function_2);

     if(func_1_block_status > 0){

        this->run(function_1,thr_num_func_2);
     };

     if(func_2_block_status > 0){

        this->run(function_2,thr_num_func_1);
     };
};

void pcynlitx::synchronizer::yield(){

     std::this_thread::yield;
};

int pcynlitx::synchronizer::GetTotalThreadNumber() const {

    return this->Total_Thread_Number;
}
