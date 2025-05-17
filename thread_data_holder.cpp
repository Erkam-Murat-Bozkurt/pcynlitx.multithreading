

#include "thread_data_holder.hpp"

pcynlitx::thread_data_holder::thread_data_holder(){

     this->Total_Thread_Number = 0;

     this->memory_clear_status = false;
};



pcynlitx::thread_data_holder::~thread_data_holder(){

     this->clear_memory();
};

void pcynlitx::thread_data_holder::clear_memory(){

     if(!this->memory_clear_status){

         this->memory_clear_status = true;
         
         if(!this->function_data_map.empty()){

             this->function_data_map.clear();
         }

         if(!this->thread_data_map.empty()){

            this->thread_data_map.clear();
         }

         if(!this->function_member_data_map.empty()){

            this->function_member_data_map.clear();
         }

         if(!this->thread_id_list.empty()){

            this->thread_id_list.clear();
         }

         this->Clear_Vector_Data(this->Function_Mem_Data);

         this->Clear_Vector_Data(this->Thread_Data_List);

         this->Clear_Vector_Data(this->Function_Names_Data_List);
     }
}

void pcynlitx::thread_data_holder::Receive_Thread_ID( int Thread_Number, std::thread::id id_num){

     this->Inside_Locker.lock();

     this->Add_Thread_Data(Thread_Number,id_num);

     this->Inside_Locker.unlock();
};


void pcynlitx::thread_data_holder::Receive_Operational_Thread_Number(int * thrNum){

     this->operational_thread_number = thrNum;
}


void pcynlitx::thread_data_holder::Add_Function_Data(std::string Function_Name, int thread_num)
{
     
     // ADD A NEW FUNCTION DATA TO FUNCTION LIST VECTOR WITHOUT SETTING MEMBER VALUES

     /* The first value of each member is set to zero, and its actual value setted 

      after initialization */

     if(!this->Is_Function_Member_Data_Exist_On_Map(Function_Name)){

        pcynlitx::Function_Member_Data * function_mem_data = new pcynlitx::Function_Member_Data;

        function_mem_data->name = Function_Name;

        function_mem_data->member_number=1;

        int thr_num = this->Get_Thread_Number();

        function_mem_data->threadNumbers.push_back(thr_num);

        this->Function_Mem_Data.push_back(function_mem_data);

        this->Function_Mem_Data.shrink_to_fit();

        this->function_member_data_map.insert(std::make_pair(Function_Name,
          
                                  this->Function_Mem_Data.back()));
     }
     else{
           
          pcynlitx::Function_Member_Data * function_mem_data 
          
          = this->Find_Function_Member_Data_From_Name(Function_Name);

          function_mem_data->member_number++;

          int thr_num = this->Get_Thread_Number();

          function_mem_data->threadNumbers.push_back(thr_num);
     }


     pcynlitx::Function_Names_Data * Function_Data = new pcynlitx::Function_Names_Data;

     Function_Data->Function_Number = this->Function_Names_Data_List.size();

     Function_Data->Rescue_Permission = false;

     Function_Data->Enter_Counter = 0;

     Function_Data->Thread_Number = thread_num;

     Function_Data->function_block_wait_status = false;

     Function_Data->Function_Enter_Counter = 0;

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

     Data->wait_enter_counter = 0;

     Data->Thread_Operational_Status = true;

     Data->Rescue_Permission = false;

     Data->Thread_Block_Status = false;

     this->Thread_Data_List.push_back(Data);

     this->thread_data_map.insert(std::make_pair(thrNum,
          
                                  this->Thread_Data_List.back()));

     this->thread_id_list.insert(std::make_pair(id_num,thrNum));

     this->Thread_Data_List.shrink_to_fit();
}


void pcynlitx::thread_data_holder::Set_Function_Name_To_Thread_Data(int threadNum, std::string fname){

     pcynlitx::Thread_Data * data = this->Find_Thread_Data_From_Number(threadNum);

     data->Thread_Function_Name = fname;
}


bool pcynlitx::thread_data_holder::Is_Exist_On_FunctionStack(std::string function_name){
     
     bool is_exist = false;

     if(this->function_data_map.find(function_name)!=this->function_data_map.end()){

        is_exist = true;
     }

     return is_exist;
}


bool pcynlitx::thread_data_holder::Is_Function_Member_Data_Exist_On_Map(std::string function_name){
     
     bool is_exist = false;

     if(this->function_member_data_map.find(function_name)!=this->function_member_data_map.end()){

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

         std::cout << "\n Inside Find_Function_Data_From_Name,";

         std::cout << "\n the file located on " << name << " can not find!.\n";

         exit(EXIT_FAILURE);
    }     
}


pcynlitx::Function_Member_Data * pcynlitx::thread_data_holder::Find_Function_Member_Data_From_Name(std::string name)
{
    try {        

         return this->function_member_data_map.at(name);
    }
    catch (const std::out_of_range & oor) {
        
         std::cerr << "\n Out of Range error: " << oor.what() << '\n';

         std::cout << "\n Inside function member data stack,";

         std::cout << "\n Inside Find_Function_Mmeber_Data_From_Name,";

         std::cout << "\n the data for " << name << " can not find!.\n";

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

pcynlitx::Thread_Data * pcynlitx::thread_data_holder::Find_Thread_Data_From_Number(int threadNum)
{
    try {        

         return this->thread_data_map.at(threadNum);
    }
    catch (const std::out_of_range & oor) {
        
         std::cerr << "\n Out of Range error: " << oor.what() << '\n';

         std::cout << "\n thread data correspoinding " << threadNum << " thread number can not find!.\n";

         exit(EXIT_FAILURE);
    }     
}


void pcynlitx::thread_data_holder::Receive_Total_Thread_Number(int num){

     this->Total_Thread_Number = num;
}

int pcynlitx::thread_data_holder::Get_Thread_Number(){

    std::thread::id this_id = std::this_thread::get_id();

    int caller_thread_number =  this->Find_thread_number_from_id(this_id);

    return caller_thread_number;
};


void pcynlitx::thread_data_holder::Exit(int thrNum){

     this->Inside_Locker.lock();

     pcynlitx::Thread_Data * data =  this->Find_Thread_Data_From_Number(thrNum);

     data->Thread_Operational_Status = false;

     this->Inside_Locker.unlock();


     this->Decrease_Function_Member_Counter(thrNum);

     
     this->Inside_Locker.lock();

     (*this->operational_thread_number)--;

     this->Inside_Locker.unlock();

};


int pcynlitx::thread_data_holder::Get_Operational_Thread_Number() const {

     return *this->operational_thread_number;
};


void pcynlitx::thread_data_holder::Increase_Wait_Enter_Counter(int Thread_Number){

     this->Inside_Locker.lock();

     pcynlitx::Thread_Data * data = this->Find_Thread_Data_From_Number(Thread_Number);

     data->wait_enter_counter++;

     this->Inside_Locker.unlock();
};



int pcynlitx::thread_data_holder::Get_Wait_Enter_Counter(int Thread_Number){

    pcynlitx::Thread_Data * data = this->Find_Thread_Data_From_Number(Thread_Number);

    return data->wait_enter_counter;
};

void pcynlitx::thread_data_holder::Increase_Barrier_Function_Wait_Enter_Counter(std::string Function_Name){

     this->Inside_Locker.lock();

     pcynlitx::Function_Names_Data * fdata = this->Find_Function_Data_From_Name(Function_Name);

     fdata->Function_Barrier_Enter_Counter++;

     this->Inside_Locker.unlock();
};

int pcynlitx::thread_data_holder::Get_Function_Barrier_Wait_Enter_Counter(std::string Function_Name){

    pcynlitx::Function_Names_Data * fdata = this->Find_Function_Data_From_Name(Function_Name);

    return fdata->Function_Barrier_Enter_Counter;

};


void pcynlitx::thread_data_holder::Increase_Function_Wait_Enter_Counter(std::string Function_Name){

     this->Inside_Locker.lock();

     pcynlitx::Function_Names_Data * fdata = this->Find_Function_Data_From_Name(Function_Name);

     fdata->Function_Enter_Counter++;  

     this->Inside_Locker.unlock();

};

int pcynlitx::thread_data_holder::Get_Function_Wait_Enter_Counter(std::string Function_Name){

    pcynlitx::Function_Names_Data * fdata = this->Find_Function_Data_From_Name(Function_Name);

    return fdata->Function_Enter_Counter;

};


void pcynlitx::thread_data_holder::Set_Function_Wait_Enter_Counter(std::string Function_Name, int Number){

     this->Inside_Locker.lock();

     pcynlitx::Function_Names_Data * fdata = this->Find_Function_Data_From_Name(Function_Name);

     fdata->Function_Enter_Counter = Number;

     this->Inside_Locker.unlock();
};

void pcynlitx::thread_data_holder::Stop_Thread(std::unique_lock<std::mutex> * mtx, int thread_number){

     pcynlitx::Thread_Data * data = this->Find_Thread_Data_From_Number(thread_number);

     data->Thread_Block_Status = true;

     data->Condition_Variable.wait(*mtx);
};

void pcynlitx::thread_data_holder::Activate_Thread(int thread_number){

     pcynlitx::Thread_Data * data = this->Find_Thread_Data_From_Number(thread_number);

     data->Thread_Block_Status = false;

     data->Condition_Variable.notify_one();
};


void pcynlitx::thread_data_holder::Set_Function_Name(int Thread_Number, std::string Function_Name){

     this->Inside_Locker.lock();

     pcynlitx::Thread_Data * data = this->Find_Thread_Data_From_Number(Thread_Number);

     data->Thread_Function_Name = Function_Name;

     this->Inside_Locker.unlock();
};

void pcynlitx::thread_data_holder::Set_Wait_Enter_Counter(int Thread_Number, int number){

     this->Inside_Locker.lock();

     pcynlitx::Thread_Data * data = this->Find_Thread_Data_From_Number(Thread_Number);

     data->wait_enter_counter = number;

     this->Inside_Locker.unlock();
};


std::string pcynlitx::thread_data_holder::Get_Function_Name(int Thread_Number){

     pcynlitx::Thread_Data * data = this->Find_Thread_Data_From_Number(Thread_Number);

     return data->Thread_Function_Name;
};


int pcynlitx::thread_data_holder::Get_Function_Member_Number(std::string Function_Name) {

    pcynlitx::Function_Member_Data * fdata = this->Find_Function_Member_Data_From_Name(Function_Name);

    return fdata->member_number;
};


void pcynlitx::thread_data_holder::Decrease_Function_Member_Counter(int thrNum){

     this->Inside_Locker.lock();

     pcynlitx::Thread_Data * data = this->Find_Thread_Data_From_Number(thrNum);

     pcynlitx::Function_Member_Data * fdata = this->Find_Function_Member_Data_From_Name(data->Thread_Function_Name);
     
     fdata->member_number--;

     this->Inside_Locker.unlock();
};



void pcynlitx::thread_data_holder::Set_Rescue_Permission(int Thread_Number, bool permission){

     this->Inside_Locker.lock();

     pcynlitx::Thread_Data * data = this->Find_Thread_Data_From_Number(Thread_Number);

     data->Rescue_Permission = permission;

     this->Inside_Locker.unlock();
};


bool pcynlitx::thread_data_holder::Get_Rescue_Permission(int Thread_Number){

     pcynlitx::Thread_Data * data = this->Find_Thread_Data_From_Number(Thread_Number);

     return data->Rescue_Permission;
};

void pcynlitx::thread_data_holder::Set_Function_Rescue_Permission(std::string Function_Name,bool permission){

     this->Inside_Locker.lock();

     pcynlitx::Function_Names_Data * fdata = this->Find_Function_Data_From_Name(Function_Name);

     fdata->Rescue_Permission = permission;

     this->Inside_Locker.unlock();
}

bool pcynlitx::thread_data_holder::Get_Function_Rescue_Permission(std::string Function_Name){

     pcynlitx::Function_Names_Data * fdata = this->Find_Function_Data_From_Name(Function_Name);

     return fdata->Rescue_Permission;
};


bool pcynlitx::thread_data_holder::Get_Thread_Operational_Status(int thread_number){

     pcynlitx::Thread_Data * data = this->Find_Thread_Data_From_Number(thread_number);

     return data->Thread_Operational_Status;
};


void pcynlitx::thread_data_holder::Rescue_Function_Members(std::string Function_Name) 
{
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
     
     this->Inside_Locker.lock();

     pcynlitx::Thread_Data * data = this->Find_Thread_Data_From_Number(Thread_Number);

     data->Thread_Block_Status = status;

     this->Inside_Locker.unlock();
};


bool pcynlitx::thread_data_holder::Get_Thread_Block_Status(int Thread_Number){

     pcynlitx::Thread_Data * data = this->Find_Thread_Data_From_Number(Thread_Number);

     return data->Thread_Block_Status;
};

void pcynlitx::thread_data_holder::Set_Block_Function_Wait_Status(std::string Function_Name, int status){

     this->Inside_Locker.lock();

     pcynlitx::Function_Names_Data * fdata = this->Find_Function_Data_From_Name(Function_Name);

     fdata->function_block_wait_status = status;

     this->Inside_Locker.unlock();
};

int pcynlitx::thread_data_holder::Get_Block_Function_Wait_Status(std::string Function_Name) {

    pcynlitx::Function_Names_Data * fdata = this->Find_Function_Data_From_Name(Function_Name);

    return fdata->function_block_wait_status;
};
