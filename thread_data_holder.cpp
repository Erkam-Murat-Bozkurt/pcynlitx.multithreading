

#include "thread_data_holder.hpp"

pcynlitx::thread_data_holder::thread_data_holder(){

     this->Caller_Thread_Number = -1;

     this->The_First_Thread_Execution_Function = 0;

     this->Total_Thread_Number = 0;

     this->Thread_Function_Number = 0;
};



pcynlitx::thread_data_holder::~thread_data_holder(){


};


void pcynlitx::thread_data_holder::Initialize_Thread_Data(){

      for(int i=0;i<this->Total_Thread_Number;i++){

          this->Thread_Data_List[i].Thread_Number = -1;

          this->Thread_Data_List[i].wait_untill_exit_thread_number = -1;

          this->Thread_Data_List[i].wait_enter_counter = 0;

          this->Thread_Data_List[i].Rescue_Permission = false;

          this->Thread_Data_List[i].Thread_Block_Status = false;

          this->Thread_Data_List[i].ref_wait_status = false;

          this->Thread_Data_List[i].wait_termination = -1;
     };
};



void pcynlitx::thread_data_holder::Receive_Thread_ID(std::string Function_Name, int Thread_Number){

     this->Inside_Locker.lock();

     std::thread::id this_id = std::this_thread::get_id();

     this->Thread_Data_List[Thread_Number].Thread_ID_Number = this_id;

     this->Thread_Data_List[Thread_Number].Thread_Number = Thread_Number;

     this->Thread_Data_List[Thread_Number].Thread_Function_Name = Function_Name;

     int Function_Name_Number = 0;

     this->Thread_Data_List[Thread_Number].Thread_Operational_Status = true;

     this->Get_Thread_Function_Name_Number(Function_Name,&Function_Name_Number);

     this->Function_Names_Data_List[Function_Name_Number].Member_Counter++;

     this->Inside_Locker.unlock();
};


int pcynlitx::thread_data_holder::Get_Thread_Number(){

    std::thread::id this_id = std::this_thread::get_id();

    for(int i=0;i<this->Total_Thread_Number;i++){

        if(this->Thread_Data_List[i].Thread_ID_Number == this_id ){

           this->Caller_Thread_Number = i;

           break;
        }
     };

     return this->Caller_Thread_Number;
};


void pcynlitx::thread_data_holder::Get_Thread_Function_Name_Number(std::string Function_Name, 

     int * Function_Name_Number){

     *Function_Name_Number = -1;

     for(int i=0;i<this->Thread_Function_Number;i++){

        if(this->Function_Names_Data_List[i].Thread_Function_Name == Function_Name){

           *Function_Name_Number = this->Function_Names_Data_List[i].Function_Number;

           break;
        }
     }

     if(*Function_Name_Number == -1){

        std::cout << "\n\n [Error] - The synchronizer object  can not determine the thread function names!";

        std::cout << "\n\n           Most probably, the synchronizer object recives a wrong name !";

        std::cout << "\n\n           Please check the initialization of the synchronizer object.";

        std::cout << "\n\n";

        exit(EXIT_FAILURE);
     }
};


void pcynlitx::thread_data_holder::Exit(){

     this->Thread_Exit_Locker.lock();

     this->Operational_Thread_Number--;

     int Thread_Number = this->Get_Thread_Number();

     this->Thread_Data_List[Thread_Number].Thread_Operational_Status = false;

     int Function_Name_Number = 0;

     std::string Function_Name = this->Get_Function_Name(Thread_Number);

     this->Get_Thread_Function_Name_Number(Function_Name,&Function_Name_Number);

     this->Function_Names_Data_List[Function_Name_Number].Member_Counter--;

     this->Thread_Exit_Locker.unlock();

};


bool pcynlitx::thread_data_holder::Get_Dead_Lock_Risk(){

     this->Dead_Lock_Risk = false;

     int blocked_thread_number = 0;

     for(int i=0;i<this->Operational_Thread_Number;i++){

         if(this->Get_Thread_Block_Status(i) == true){

           blocked_thread_number++;

         };
     };

     if((this->Operational_Thread_Number - blocked_thread_number) < 2){

         this->Dead_Lock_Risk = true;
     }

     return this->Dead_Lock_Risk;

};


int pcynlitx::thread_data_holder::Get_Operational_Thread_Number() const {

     return this->Operational_Thread_Number;

};


void pcynlitx::thread_data_holder::Increase_Wait_Enter_Counter(int Thread_Number){

     this->Thread_Data_List[Thread_Number].wait_enter_counter++;
};



int pcynlitx::thread_data_holder::Get_Wait_Enter_Counter(int Thread_Number) const {

    return this->Thread_Data_List[Thread_Number].wait_enter_counter;

};

void pcynlitx::thread_data_holder::Increase_Function_Wait_Enter_Counter(std::string Function_Name){

     int Function_Name_Number = 0;

     this->Get_Thread_Function_Name_Number(Function_Name,&Function_Name_Number);

     this->Function_Names_Data_List[Function_Name_Number].Enter_Counter++;

};

int pcynlitx::thread_data_holder::Get_Function_Wait_Enter_Counter(std::string Function_Name){

    int Function_Name_Number = 0;

    this->Get_Thread_Function_Name_Number(Function_Name,&Function_Name_Number);

    return this->Function_Names_Data_List[Function_Name_Number].Enter_Counter;

};


void pcynlitx::thread_data_holder::Increase_Two_Prm_Function_Wait_Enter_Counter(std::string Function_Name){

     int Function_Name_Number = 0;

     this->Get_Thread_Function_Name_Number(Function_Name,&Function_Name_Number);

     this->Function_Names_Data_List[Function_Name_Number].Two_Prm_Function_Enter_Counter++;  
};

int pcynlitx::thread_data_holder::Get_Two_Prm_Function_Wait_Enter_Counter(std::string Function_Name){

    int Function_Name_Number = 0;

    this->Get_Thread_Function_Name_Number(Function_Name,&Function_Name_Number);

    return this->Function_Names_Data_List[Function_Name_Number].Two_Prm_Function_Enter_Counter;

};


void pcynlitx::thread_data_holder::Set_Function_Wait_Enter_Counter(std::string Function_Name, int Number){

     int Function_Name_Number = 0;

     this->Get_Thread_Function_Name_Number(Function_Name,&Function_Name_Number);

     this->Function_Names_Data_List[Function_Name_Number].Enter_Counter = Number;
};


void pcynlitx::thread_data_holder::Set_Two_Prm_Function_Wait_Enter_Counter(std::string Function_Name, int Number){

     int Function_Name_Number = 0;

     this->Get_Thread_Function_Name_Number(Function_Name,&Function_Name_Number);

     this->Function_Names_Data_List[Function_Name_Number].Two_Prm_Function_Enter_Counter = Number;

};

void pcynlitx::thread_data_holder::Stop_Thread(std::unique_lock<std::mutex> * mtx, int thread_number){

     this->Set_Thread_Block_Status(thread_number,true);

     this->Thread_Data_List[thread_number].Condition_Variable.wait(*mtx);
};

void pcynlitx::thread_data_holder::Activate_Thread(int thread_number){

     this->Set_Thread_Block_Status(thread_number,false);

     this->Thread_Data_List[thread_number].Condition_Variable.notify_one();
};


void pcynlitx::thread_data_holder::Set_Function_Name(int Thread_Number, std::string Function_Name){

     this->Thread_Data_List[Thread_Number].Thread_Function_Name = Function_Name;
};

void pcynlitx::thread_data_holder::Set_Wait_Enter_Counter(int Thread_Number, int number){

     this->Thread_Data_List[Thread_Number].wait_enter_counter = number;
};


std::string pcynlitx::thread_data_holder::Get_Function_Name(int Thread_Number) const {

     return this->Thread_Data_List[Thread_Number].Thread_Function_Name;
};


int pcynlitx::thread_data_holder::Get_Function_Member_Number(std::string Function_Name){

    int Function_Name_Number = 0;

    this->Get_Thread_Function_Name_Number(Function_Name,&Function_Name_Number);

    return this->Function_Names_Data_List[Function_Name_Number].Member_Counter;

};



void pcynlitx::thread_data_holder::Set_Rescue_Permission(int Thread_Number, bool permission){

     this->Thread_Data_List[Thread_Number].Rescue_Permission = permission;
};


bool pcynlitx::thread_data_holder::Get_Rescue_Permission(int Thread_Number) const {

     return this->Thread_Data_List[Thread_Number].Rescue_Permission;

};

void pcynlitx::thread_data_holder::Set_Function_Rescue_Permission(std::string Function_Name,bool permission){

     int Function_Name_Number = 0;

     this->Get_Thread_Function_Name_Number(Function_Name,&Function_Name_Number);

     this->Function_Names_Data_List[Function_Name_Number].Rescue_Permission = permission;

}

bool pcynlitx::thread_data_holder::Get_Function_Rescue_Permission(std::string Function_Name){

     int Function_Name_Number = 0;

     this->Get_Thread_Function_Name_Number(Function_Name,&Function_Name_Number);

     return this->Function_Names_Data_List[Function_Name_Number].Rescue_Permission;
};


bool pcynlitx::thread_data_holder::Get_Thread_Operational_Status(int thread_number) const {

     return this->Thread_Data_List[thread_number].Thread_Operational_Status;
};


void pcynlitx::thread_data_holder::Wait_Thread_Termination(int Number, int Rescuer_Thread){

     this->Thread_Data_List[Rescuer_Thread].wait_untill_exit_thread_number = Number;
};

int pcynlitx::thread_data_holder::Get_Thread_Waits_This_Thread_Termination(int Number) const {

    return this->Thread_Data_List[Number].wait_untill_exit_thread_number;
}

void pcynlitx::thread_data_holder::Rescue_Function_Members(std::string Function_Name) {

     int Thread_Number = this->Get_Thread_Number();

     for(int i=0;i<this->Total_Thread_Number;i++){

         if(((this->Get_Function_Name(i) == Function_Name) && (i != Thread_Number))){

               if(this->Get_Thread_Operational_Status(i)){

                  this->Activate_Thread(i);
               }
         }
     }
};


void pcynlitx::thread_data_holder::Set_Thread_Block_Status(int Thread_Number, bool status) {
     
     this->Thread_Data_List[Thread_Number].Thread_Block_Status = status;
};


bool pcynlitx::thread_data_holder::Get_Thread_Block_Status(int Thread_Number) const{

     return this->Thread_Data_List[Thread_Number].Thread_Block_Status;

};

void pcynlitx::thread_data_holder::Set_Block_Function_Wait_Status(std::string Function_Name, int status){

     int Function_Name_Number = 0;

     this->Get_Thread_Function_Name_Number(Function_Name,&Function_Name_Number);

     this->Function_Names_Data_List[Function_Name_Number].function_block_wait_status = status;
};


int pcynlitx::thread_data_holder::Get_Block_Function_Wait_Status(std::string Function_Name) {

    int Function_Name_Number = 0;

    this->Get_Thread_Function_Name_Number(Function_Name,&Function_Name_Number);

    return this->Function_Names_Data_List[Function_Name_Number].function_block_wait_status;
};

int pcynlitx::thread_data_holder::GetFirstThreadExecutingFunction(std::string Function_Name) {

    this->The_First_Thread_Execution_Function = 0;

    for(int i=0;i<this->Total_Thread_Number;i++){

       if(this->Thread_Data_List[i].Thread_Function_Name == Function_Name){

          this->The_First_Thread_Execution_Function = i;
       }
     }

     return this->The_First_Thread_Execution_Function;
};

