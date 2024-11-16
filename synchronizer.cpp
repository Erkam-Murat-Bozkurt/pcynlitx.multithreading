

#include "synchronizer.hpp"


pcynlitx::synchronizer::synchronizer(int thrNum){

   this->Total_Thread_Number = thrNum;

   this->data_holder.Receive_Total_Thread_Number(thrNum);

   this->Thread_Function_Number = 0;

   this->Operational_Thread_Number = 0;

   this->Function_enter_counter_with_rescuer_thread = 0;

   this->Function_enter_counter = 0;

   this->waiting_thread_number_in_barrier = 0;

   this->Thread_On_Point_Wait = -1;

};


pcynlitx::synchronizer::~synchronizer(){ };

void pcynlitx::synchronizer::lock(){

     this->Outside_Locker.lock();
}


void pcynlitx::synchronizer::unlock(){

     this->Outside_Locker.unlock();

}



void pcynlitx::synchronizer::Connect(int thread_num, std::string Function_Name){

     this->Receive_Thread_ID(Function_Name,thread_num);

     this->barrier_wait();
};


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

/*

void pcynlitx::synchronizer::Determine_Project_Specific_Data(int thread_num, std::string Thread_Function_Name){

     this->data_holder.Determine_Project_Specific_Data(thread_num,Thread_Function_Name);
}
*/

void pcynlitx::synchronizer::wait(int Number, int Rescuer_Thread){

     std::unique_lock<std::mutex> Function_lock(this->mtx_two_parameter_wait);

     int Thread_Number = this->data_holder.Get_Thread_Number();

     bool rescue_condition = this->data_holder.Get_Rescue_Permission(Rescuer_Thread);

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
            else {

                    this->data_holder.Stop_Thread(&Function_lock,Number);
            }
        }
     }
     else{

          Function_lock.unlock();
     };

};



void pcynlitx::synchronizer::rescue(int Number, int Rescuer_Thread){

     std::unique_lock<std::mutex> Function_lock(this->mtx_two_parameter_wait);

     if(this->data_holder.Get_Thread_Number() == Rescuer_Thread){

        this->data_holder.Set_Rescue_Permission(Rescuer_Thread,true);

        Function_lock.unlock();

        this->wait(Number,Rescuer_Thread);

        this->data_holder.Set_Rescue_Permission(Rescuer_Thread,false); 
     }
     else{

           Function_lock.unlock();
     }
}





void pcynlitx::synchronizer::wait(int Number){

     std::unique_lock<std::mutex> thread_lock(this->Thread_Mutex[Number]);

     this->data_holder.Stop_Thread(&thread_lock,Number);

     thread_lock.unlock();
}



void pcynlitx::synchronizer::rescue(int Number){

     this->data_holder.Activate_Thread(Number);
};

void pcynlitx::synchronizer::start_serial(int start_number, int end_number, int thread_number){

     if(thread_number > start_number){

        this->wait(thread_number,thread_number-1);
     }
};



void pcynlitx::synchronizer::end_serial(int start_number, int end_number, int thread_number){

     if(thread_number < (end_number -1)){

        this->rescue(thread_number+1,thread_number);
     }

     if(thread_number == start_number){

        this->wait(start_number,end_number-1);
     }

     if(thread_number == (end_number-1)){

        this->rescue(start_number,end_number-1);
     }
};



void pcynlitx::synchronizer::switch_wait(int Number){

     std::unique_lock<std::mutex> thread_lock(this->mtx_switch_wait);

     if(this->data_holder.Get_Thread_Number() == Number){

        if(this->Thread_On_Point_Wait!= -1){

           this->data_holder.Activate_Thread(this->Thread_On_Point_Wait);
        }

        this->Thread_On_Point_Wait = Number;

        if(!this->data_holder.Get_Dead_Lock_Risk()){

            this->data_holder.Stop_Thread(&thread_lock,this->Thread_On_Point_Wait);
        }

        thread_lock.unlock();

     }
     else{

           thread_lock.unlock();
     };
};


void pcynlitx::synchronizer::wait(std::string Function_Name){

     this->Inside_Locker.lock();

     int Function_Number = 0;

     //this->data_holder.Get_Thread_Function_Name_Number(Function_Name,&Function_Number);

     this->Inside_Locker.unlock();

     std::unique_lock<std::mutex> Function_lock(this->Function_Mutex[Function_Number]);

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


void pcynlitx::synchronizer::wait(std::string Function_Name, int Rescuer_Thread){

     this->Inside_Locker.lock();

     int Function_Number = 0;

     //this->data_holder.Get_Thread_Function_Name_Number(Function_Name,&Function_Number);

     this->Inside_Locker.unlock();

     std::unique_lock<std::mutex> Function_lock(this->Two_Pr_Function_Mutex[Function_Number]);

     int Thread_Number = this->data_holder.Get_Thread_Number();

     if(((Thread_Number == Rescuer_Thread ) || (this->data_holder.Get_Function_Name(Thread_Number) == Function_Name))){

        if(Thread_Number == Rescuer_Thread){

           int status = 0;

           this->data_holder.Set_Block_Function_Wait_Status(Function_Name,status);

           this->data_holder.Set_Function_Rescue_Permission(Function_Name,true);
         }

         this->data_holder.Increase_Two_Prm_Function_Wait_Enter_Counter(Function_Name);

         int Member_Number = this->data_holder.Get_Function_Member_Number(Function_Name);

         int Enter_Counter = this->data_holder.Get_Two_Prm_Function_Wait_Enter_Counter(Function_Name);

         bool Rescue_Permission = this->data_holder.Get_Function_Rescue_Permission(Function_Name);

         if(((Enter_Counter == (Member_Number+1)) && Rescue_Permission )){

            this->data_holder.Set_Two_Prm_Function_Wait_Enter_Counter(Function_Name,0);

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


void pcynlitx::synchronizer::rescue(std::string Function_Name, int Rescuer_Thread){

     this->Inside_Locker.lock();

     int Function_Number = 0;

     //this->data_holder.Get_Thread_Function_Name_Number(Function_Name,&Function_Number);

     this->Inside_Locker.unlock();

     std::unique_lock<std::mutex> Function_lock(this->Two_Pr_Function_Mutex[Function_Number]);

     if(this->data_holder.Get_Thread_Number() == Rescuer_Thread){

        Function_lock.unlock();

        this->wait(Function_Name,Rescuer_Thread);

        this->data_holder.Set_Rescue_Permission(Rescuer_Thread,false);
     }
     else{
             Function_lock.unlock();
     }
}


void pcynlitx::synchronizer::Exit(){

     this->Inside_Locker.lock();

     this->data_holder.Exit();

     this->Check_Is_There_Waiting_Until_Exit();

     this->Inside_Locker.unlock();

};


void pcynlitx::synchronizer::wait_until_exit(int Number, int Rescuer_Thread) {

     this->data_holder.Wait_Thread_Termination(Number,Rescuer_Thread);

     this->wait(Number,Rescuer_Thread);
};


 void pcynlitx::synchronizer::Check_Is_There_Waiting_Until_Exit() {

     int Thread_Number = this->data_holder.Get_Thread_Number();

     if(this->data_holder.Get_Thread_Waits_This_Thread_Termination(Thread_Number) != -1){

        this->rescue(this->data_holder.Get_Thread_Waits_This_Thread_Termination(Thread_Number),Thread_Number);
    }
};


void pcynlitx::synchronizer::Receive_Thread_ID(std::string Function_Name, int Thread_Number){

     this->Inside_Locker.lock();

     this->data_holder.Receive_Thread_ID(Function_Name,Thread_Number);

     this->Inside_Locker.unlock();
};


int pcynlitx::synchronizer::Get_Thread_Number(){

    return this->data_holder.Get_Thread_Number();
};


bool pcynlitx::synchronizer::Get_Thread_Block_Status(int Thread_Number) const {

     return this->data_holder.Get_Thread_Block_Status(Thread_Number);
};



int  pcynlitx::synchronizer::Get_Operational_Thread_Number() const{

     return this->data_holder.Get_Operational_Thread_Number();
};


void pcynlitx::synchronizer::function_switch(std::string function_1, std::string function_2){

     int thr_num_func_1 = this->data_holder.GetFirstThreadExecutingFunction(function_1);

     int thr_num_func_2 = this->data_holder.GetFirstThreadExecutingFunction(function_2);

     int func_2_block_status = this->data_holder.Get_Block_Function_Wait_Status(function_2);

     if(func_2_block_status > 0){

        this->rescue(function_2,thr_num_func_1);
     };

     this->wait(function_1,thr_num_func_2);
};


void pcynlitx::synchronizer::reset_function_switch(std::string function_1, std::string function_2){;

     int thr_num_func_1 = this->data_holder.GetFirstThreadExecutingFunction(function_1);

     int thr_num_func_2 = this->data_holder.GetFirstThreadExecutingFunction(function_2);

     int func_1_block_status = this->data_holder.Get_Block_Function_Wait_Status(function_1);

     int func_2_block_status = this->data_holder.Get_Block_Function_Wait_Status(function_2);

     if(func_1_block_status > 0){

        this->rescue(function_1,thr_num_func_2);
     };

     if(func_2_block_status > 0){

        this->rescue(function_2,thr_num_func_1);
     };
};


void pcynlitx::synchronizer::yield(){

     std::this_thread::yield;
};
