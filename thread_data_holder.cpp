

#include "thread_data_holder.hpp"

pcynlitx::thread_data_holder::thread_data_holder(){

     this->Caller_Thread_Number = -1;

     this->The_First_Thread_Execution_Function = 0;

     this->Total_Thread_Number = 0;

     this->Thread_Function_Number = 0;
};



pcynlitx::thread_data_holder::~thread_data_holder(){


};


void pcynlitx::thread_data_holder::Receive_Thread_ID( int Thread_Number, std::thread::id id_num){

     this->Inside_Locker.lock();

     this->Add_Thread_Data(Thread_Number,id_num);

     /*

     if(this->Is_Exist_On_FunctionStack(Function_Name)){

        pcynlitx::Function_Names_Data * data_ptr = 
               
                this->Find_Function_Data_From_Name(Function_Name);

        data_ptr->Member_Counter++;
              
        data_ptr->Enter_Counter++;
     }
     else{

          this->Add_Function_Data(Function_Name);

     }
     
     */

     this->Inside_Locker.unlock();
};



void pcynlitx::thread_data_holder::Add_Function_Data(std::string Function_Name){
     
     // ADD A NEW FUNCTION DATA TO FUNCTION LIST VECTOR WITHOUT SETTING MEMBER VALUES

     /* The first value of each member is set to zero, and its actual value setted 

      after initialization */

     pcynlitx::Function_Names_Data * Function_Data = new pcynlitx::Function_Names_Data;

     Function_Data->Function_Number = this->Function_Names_Data_List.size();

     Function_Data->Member_Counter = 0;

     Function_Data->Rescue_Permission = false;

     Function_Data->Enter_Counter = 0;

     Function_Data->function_block_wait_status = false;

     Function_Data->Two_Prm_Function_Enter_Counter = 0;

     Function_Data->Thread_Function_Name = Function_Name;

     this->Function_Names_Data_List.push_back(Function_Data);

     this->function_data_map.insert(std::make_pair(Function_Name,
          
                                  this->Function_Names_Data_List.back()));

     this->Function_Names_Data_List.shrink_to_fit();
}



void pcynlitx::thread_data_holder::Add_Thread_Data(int thrNum, std::thread::id id_num){

     // ADD A NEW THREAD DATA TO FUNCTION LIST VECTOR WITHOUT SETTING MEMBER VALUES

     /* The first value of each member is set to zero, and its actual value setted 

      after initialization */

     pcynlitx::Thread_Data * Data = new pcynlitx::Thread_Data;

     Data->Thread_ID_Number = id_num;

     Data->Thread_Number = thrNum;

     //Data->Thread_Function_Name = Function_Name;

     Data->Thread_Operational_Status = true;

     Data->ref_wait_status = false;

     Data->Rescue_Permission = false;

     Data->Thread_Block_Status = false;

     Data->wait_untill_exit_thread_number = 0;

     
     //std::cout << "\n INSIDE  thread_data_holder::Add_Thread_Data";

     this->Thread_Data_List.push_back(Data);

     this->thread_data_map.insert(std::make_pair(thrNum,
          
                                  this->Thread_Data_List.back()));

     //std::cout << "\n The id number added to the thread data list:" << id_num;

     //std::cout << "\n The thrNum number added to the thread data list:" << thrNum;


     this->thread_id_list.insert(std::make_pair(id_num,thrNum));

     //std::cout << "\n this->thread_id_list.size():" << this->thread_id_list.size();


     this->Thread_Data_List.shrink_to_fit();

     //std::cout << "\n THE END OF   thread_data_holder::Add_Thread_Data";

}


bool pcynlitx::thread_data_holder::Is_Exist_On_FunctionStack(std::string function_name){
     
     bool is_exist = false;

     if(this->function_data_map.find(function_name)!=this->function_data_map.end()){

        is_exist = true;
     }

     return is_exist;
}


bool pcynlitx::thread_data_holder::Is_Exist_On_ThreadStack(std::thread::id id){
     
     bool is_exist = false;

     if(this->thread_id_list.find(id)!=this->thread_id_list.end()){

        is_exist = true;
     }

     return is_exist;
}


pcynlitx::Function_Names_Data * pcynlitx::thread_data_holder::Find_Function_Data_From_Name(std::string name)
{
    try {        

         return this->function_data_map.at(name);
    }
    catch (const std::out_of_range & oor) {
        
         std::cerr << "\n Out of Range error: " << oor.what() << '\n';

         std::cout << "\n Inside function name data stack,";

         std::cout << "\n Inside Find_Search_Data_From_Path,";

         std::cout << "\n the file located on " << name << " can not find!.\n";

         exit(EXIT_FAILURE);
    }     
}


int pcynlitx::thread_data_holder::Find_thread_number_from_id(std::thread::id id)
{
    try {        

         return this->thread_id_list.at(id);
    }
    catch (const std::out_of_range & oor) {
        
         std::cerr << "\n Out of Range error: " << oor.what() << '\n';

         std::cout << "\n thread number correspoinding " << id << " can not find!.\n";

         exit(EXIT_FAILURE);
    }     
}

void pcynlitx::thread_data_holder::Receive_Total_Thread_Number(int num){

     this->Total_Thread_Number = num;
}

int pcynlitx::thread_data_holder::Get_Thread_Number(){

    std::thread::id this_id = std::this_thread::get_id();

    //std::cout << "\n this_id:" << this_id;

    this->Caller_Thread_Number =  this->Find_thread_number_from_id(this_id);

    return this->Caller_Thread_Number;
};

/*
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

*/


void pcynlitx::thread_data_holder::Exit(){

     this->Thread_Exit_Locker.lock();

     this->Operational_Thread_Number--;

     int Thread_Number = this->Get_Thread_Number();

     this->Thread_Data_List[Thread_Number]->Thread_Operational_Status = false;

     int Function_Name_Number = 0;

     std::string Function_Name = this->Get_Function_Name(Thread_Number);

     this->Function_Names_Data_List[Function_Name_Number]->Member_Counter--;

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

     this->Thread_Data_List[Thread_Number]->wait_enter_counter++;
};



int pcynlitx::thread_data_holder::Get_Wait_Enter_Counter(int Thread_Number) const {

    return this->Thread_Data_List[Thread_Number]->wait_enter_counter;

};

void pcynlitx::thread_data_holder::Increase_Function_Wait_Enter_Counter(std::string Function_Name){

     int Function_Name_Number = 0;

     this->Function_Names_Data_List[Function_Name_Number]->Enter_Counter++;

};

int pcynlitx::thread_data_holder::Get_Function_Wait_Enter_Counter(std::string Function_Name){

    int Function_Name_Number = 0;

    return this->Function_Names_Data_List[Function_Name_Number]->Enter_Counter;

};


void pcynlitx::thread_data_holder::Increase_Two_Prm_Function_Wait_Enter_Counter(std::string Function_Name){

     int Function_Name_Number = 0;

     this->Function_Names_Data_List[Function_Name_Number]->Two_Prm_Function_Enter_Counter++;  
};

int pcynlitx::thread_data_holder::Get_Two_Prm_Function_Wait_Enter_Counter(std::string Function_Name){

    int Function_Name_Number = 0;

    return this->Function_Names_Data_List[Function_Name_Number]->Two_Prm_Function_Enter_Counter;

};


void pcynlitx::thread_data_holder::Set_Function_Wait_Enter_Counter(std::string Function_Name, int Number){

     int Function_Name_Number = 0;

     this->Function_Names_Data_List[Function_Name_Number]->Enter_Counter = Number;
};


void pcynlitx::thread_data_holder::Set_Two_Prm_Function_Wait_Enter_Counter(std::string Function_Name, int Number){

     int Function_Name_Number = 0;

     this->Function_Names_Data_List[Function_Name_Number]->Two_Prm_Function_Enter_Counter = Number;

};

void pcynlitx::thread_data_holder::Stop_Thread(std::unique_lock<std::mutex> * mtx, int thread_number){

     this->Set_Thread_Block_Status(thread_number,true);

     this->Thread_Data_List[thread_number]->Condition_Variable.wait(*mtx);
};

void pcynlitx::thread_data_holder::Activate_Thread(int thread_number){

     this->Set_Thread_Block_Status(thread_number,false);

     this->Thread_Data_List[thread_number]->Condition_Variable.notify_one();
};


void pcynlitx::thread_data_holder::Set_Function_Name(int Thread_Number, std::string Function_Name){

     this->Thread_Data_List[Thread_Number]->Thread_Function_Name = Function_Name;
};

void pcynlitx::thread_data_holder::Set_Wait_Enter_Counter(int Thread_Number, int number){

     this->Thread_Data_List[Thread_Number]->wait_enter_counter = number;
};


std::string pcynlitx::thread_data_holder::Get_Function_Name(int Thread_Number) const {

     return this->Thread_Data_List[Thread_Number]->Thread_Function_Name;
};


int pcynlitx::thread_data_holder::Get_Function_Member_Number(std::string Function_Name){

    int Function_Name_Number = 0;

    return this->Function_Names_Data_List[Function_Name_Number]->Member_Counter;

};



void pcynlitx::thread_data_holder::Set_Rescue_Permission(int Thread_Number, bool permission){

     this->Thread_Data_List[Thread_Number]->Rescue_Permission = permission;
};


bool pcynlitx::thread_data_holder::Get_Rescue_Permission(int Thread_Number) const {

     return this->Thread_Data_List[Thread_Number]->Rescue_Permission;

};

void pcynlitx::thread_data_holder::Set_Function_Rescue_Permission(std::string Function_Name,bool permission){

     int Function_Name_Number = 0;

     this->Function_Names_Data_List[Function_Name_Number]->Rescue_Permission = permission;

}

bool pcynlitx::thread_data_holder::Get_Function_Rescue_Permission(std::string Function_Name){

     int Function_Name_Number = 0;

     return this->Function_Names_Data_List[Function_Name_Number]->Rescue_Permission;
};


bool pcynlitx::thread_data_holder::Get_Thread_Operational_Status(int thread_number) const {

     return this->Thread_Data_List[thread_number]->Thread_Operational_Status;
};


void pcynlitx::thread_data_holder::Wait_Thread_Termination(int Number, int Rescuer_Thread){

     this->Thread_Data_List[Rescuer_Thread]->wait_untill_exit_thread_number = Number;
};

int pcynlitx::thread_data_holder::Get_Thread_Waits_This_Thread_Termination(int Number) const {

    return this->Thread_Data_List[Number]->wait_untill_exit_thread_number;
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
     
     this->Thread_Data_List[Thread_Number]->Thread_Block_Status = status;
};


bool pcynlitx::thread_data_holder::Get_Thread_Block_Status(int Thread_Number) const{

     return this->Thread_Data_List[Thread_Number]->Thread_Block_Status;

};

void pcynlitx::thread_data_holder::Set_Block_Function_Wait_Status(std::string Function_Name, int status){

     int Function_Name_Number = 0;

     this->Function_Names_Data_List[Function_Name_Number]->function_block_wait_status = status;
};


int pcynlitx::thread_data_holder::Get_Block_Function_Wait_Status(std::string Function_Name) {

    int Function_Name_Number = 0;

    return this->Function_Names_Data_List[Function_Name_Number]->function_block_wait_status;
};

int pcynlitx::thread_data_holder::GetFirstThreadExecutingFunction(std::string Function_Name) {

    this->The_First_Thread_Execution_Function = 0;

    for(int i=0;i<this->Total_Thread_Number;i++){

       if(this->Thread_Data_List[i]->Thread_Function_Name == Function_Name){

          this->The_First_Thread_Execution_Function = i;
       }
     }

     return this->The_First_Thread_Execution_Function;
};

