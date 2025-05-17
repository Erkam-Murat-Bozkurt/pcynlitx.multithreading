
#include "function_blocker.hpp"

pcynlitx::function_blocker::function_blocker()
{

}

pcynlitx::function_blocker::~function_blocker()
{


}

void pcynlitx::function_blocker::Receive_Data_Holder(pcynlitx::thread_data_holder * ptr){

     this->data_holder_pointer = ptr;
}




void pcynlitx::function_blocker::stop(std::string Function_Name){

     this->Locker.lock();

     pcynlitx::Function_Names_Data * fdata = this->data_holder_pointer->Find_Function_Data_From_Name(Function_Name);

     this->Locker.unlock();

     // --------------------------------------------------------------------------------------------------

     std::unique_lock<std::mutex> Function_lock(fdata->Function_Barrier_Mutex);

     int Thread_Number = this->data_holder_pointer->Get_Thread_Number();

     if(this->data_holder_pointer->Get_Function_Name(Thread_Number) == Function_Name){

        int Function_Member_Number = this->data_holder_pointer->Get_Function_Member_Number(Function_Name);

        this->data_holder_pointer->Increase_Function_Wait_Enter_Counter(Function_Name);

        if(this->data_holder_pointer->Get_Function_Wait_Enter_Counter(Function_Name) < Function_Member_Number){

           this->data_holder_pointer->Stop_Thread(&Function_lock,Thread_Number);

           Function_lock.unlock();
        }
        else{
               this->data_holder_pointer->Set_Function_Wait_Enter_Counter(Function_Name,0);

               this->data_holder_pointer->Rescue_Function_Members(Function_Name);

               Function_lock.unlock(); 
         }
     }
     else{

           Function_lock.unlock();

     }
}





void pcynlitx::function_blocker::stop(std::string Function_Name, int Rescuer_Thread){

     this->Locker.lock();

     pcynlitx::Function_Names_Data * fdata = this->data_holder_pointer->Find_Function_Data_From_Name(Function_Name);

     this->Locker.unlock();


     // --------------------------------------------------------------------------------------------------

     std::unique_lock<std::mutex> Function_lock(fdata->Function_Mutex);

     int Thread_Number = this->data_holder_pointer->Get_Thread_Number();


     if(((Thread_Number == Rescuer_Thread ) || (this->data_holder_pointer->Get_Function_Name(Thread_Number) == Function_Name))){

        if(Thread_Number == Rescuer_Thread){

           int status = 0;

           this->data_holder_pointer->Set_Block_Function_Wait_Status(Function_Name,status);

           this->data_holder_pointer->Set_Function_Rescue_Permission(Function_Name,true);

         }

         this->data_holder_pointer->Increase_Function_Wait_Enter_Counter(Function_Name);

         int Member_Number = this->data_holder_pointer->Get_Function_Member_Number(Function_Name);

         int Enter_Counter = this->data_holder_pointer->Get_Function_Wait_Enter_Counter(Function_Name);

         bool Rescue_Permission = this->data_holder_pointer->Get_Function_Rescue_Permission(Function_Name);



         if(((Enter_Counter == (Member_Number+1)) && Rescue_Permission )){

            this->data_holder_pointer->Set_Function_Wait_Enter_Counter(Function_Name,0);

            if(this->data_holder_pointer->Get_Thread_Number() != Rescuer_Thread){

               this->data_holder_pointer->Activate_Thread(Rescuer_Thread);
            }

            this->data_holder_pointer->Rescue_Function_Members(Function_Name);

            Function_lock.unlock();
         }
         else{
 
               int status = 1;

               this->data_holder_pointer->Set_Block_Function_Wait_Status(Function_Name,status);

               this->data_holder_pointer->Stop_Thread(&Function_lock,Thread_Number);   // Thread Stop Point

               Function_lock.unlock();
         }
      }
      else{

          Function_lock.unlock();

      }
}




void pcynlitx::function_blocker::run(std::string Function_Name, int Rescuer_Thread){

     this->Locker.lock();

     pcynlitx::Function_Names_Data * fdata = this->data_holder_pointer->Find_Function_Data_From_Name(Function_Name);

     this->Locker.unlock();

     std::unique_lock<std::mutex> Function_lock(fdata->Function_Mutex);

     if(this->data_holder_pointer->Get_Thread_Number() == Rescuer_Thread){

        Function_lock.unlock();

        this->stop(Function_Name,Rescuer_Thread);

        this->data_holder_pointer->Set_Rescue_Permission(Rescuer_Thread,false);
     }
     else{
            Function_lock.unlock();
     }
}






