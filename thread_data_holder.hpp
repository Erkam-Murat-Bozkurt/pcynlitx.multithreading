
 #ifndef THREAD_DATA_HOLDER_HPP
 #define THREAD_DATA_HOLDER_HPP

 #include "thread_mutex.hpp"
 #include <thread>
 #include <mutex>
 #include <iostream>
 #include <string>
 #include <cstdlib>
 #include <chrono>
 #include <condition_variable>
 #include <vector>
 #include <map>
 #include <unordered_map>
 #include <iterator>
 #include <utility>                     // std::pair, std::make_pair
 #include <stdexcept>                   // std::out_of_range


 namespace pcynlitx{

    struct Thread_Data
    {
        std::thread::id Thread_ID_Number;
        std::string Thread_Function_Name;
        int  Thread_Number;
        int  wait_enter_counter;
        bool Thread_Operational_Status;
        bool Rescue_Permission;
        bool Thread_Block_Status;
        std::condition_variable Condition_Variable;
        std::mutex threadMutex;
    };

    struct Function_Names_Data
    {
        int  Function_Number;
        int  Thread_Number;
        bool Rescue_Permission;
        int  Enter_Counter;
        int  function_block_wait_status;
        int  Function_Enter_Counter;
        int  Function_Barrier_Enter_Counter;
        std::string Thread_Function_Name;
        std::condition_variable Condition_Variable;
        std::mutex Function_Mutex;
        std::mutex Function_Barrier_Mutex;
    };

    struct Function_Member_Data
    {
        std::string name;
        int member_number;
        bool function_switch_block_status;
        std::vector<int> threadNumbers;
    };

    class thread_data_holder
    {
    public:
        thread_data_holder();
        virtual ~thread_data_holder();
        void Receive_Total_Thread_Number(int num);
        void Receive_Thread_ID(int Thread_Number, std::thread::id id_num);
        void Increase_Wait_Enter_Counter(int Thread_Number);
        void Increase_Barrier_Function_Wait_Enter_Counter(std::string Function_Name);
        void Increase_Function_Wait_Enter_Counter(std::string Function_Name);
        void Rescue_Function_Members(std::string Function_Name);
        int  Get_Function_Barrier_Wait_Enter_Counter(std::string Function_Name);
        int  Get_Function_Wait_Enter_Counter(std::string Function_Name);
        void Set_Rescue_Permission(int Thread_Number, bool permission);
        void Set_Thread_Block_Status(int Thread_Number, bool status);
        void Set_Wait_Enter_Counter(int Thread_Number, int Number);
        void Set_Function_Wait_Enter_Counter(std::string Function_Name, int Number);
        void Set_Function_Rescue_Permission(std::string Function_Name, bool permission);
        void Set_Function_Name(int Thread_Number, std::string Function_Name);
        bool Get_Function_Rescue_Permission(std::string Function_Name);
        int  Get_Wait_Enter_Counter(int Thread_Number);
        bool Get_Rescue_Permission(int Thread_Number);
        bool Get_Thread_Block_Status(int Thread_Number);
        int  Get_Thread_Number();
        int  Get_Function_Member_Number(std::string Function_Name);
        std::string Get_Function_Name(int Thread_Number);
        int  Get_Operational_Thread_Number() const;
        bool Get_Thread_Operational_Status(int Thread_Number);
        void Stop_Thread(std::unique_lock<std::mutex> * mtx, int thread_number);
        void Activate_Thread(int thread_number);
        void Set_Block_Function_Wait_Status(std::string function, int status);
        int  Get_Block_Function_Wait_Status(std::string function);
        bool Is_Exist_On_FunctionStack(std::string path);
        bool Is_Exist_On_ThreadStack(std::thread::id id);
        int  Find_thread_number_from_id(std::thread::id id);
        void Add_Function_Data(std::string Function_Name, int thread_num);
        void Set_Function_Name_To_Thread_Data(int threadNum, std::string fname);
        Function_Names_Data * Find_Function_Data_From_Name(std::string name);
        Thread_Data * Find_Thread_Data_From_Number(int threadNum);
        void Decrease_Function_Member_Counter(int thrNum);
        void Receive_Operational_Thread_Number(int * thrNum);
        void Exit(int thrNum);
        pcynlitx::Function_Member_Data * Find_Function_Member_Data_From_Name(std::string name);
        void clear_memory();
   private:
        bool Is_Function_Member_Data_Exist_On_Map(std::string function_name);
        void Add_Thread_Data(int Thread_Number, std::thread::id id_num);
        thread_mutex Inside_Locker;
        int Total_Thread_Number;
        int * operational_thread_number;

        std::vector<pcynlitx::Function_Member_Data *> Function_Mem_Data;        
        std::vector<pcynlitx::Thread_Data *> Thread_Data_List;
        std::vector<pcynlitx::Function_Names_Data *> Function_Names_Data_List;

        std::unordered_map<std::string, pcynlitx::Function_Names_Data *> function_data_map;
        std::unordered_map<int, Thread_Data *> thread_data_map;
        std::unordered_map<std::string, pcynlitx::Function_Member_Data *> function_member_data_map;
        std::unordered_map<std::thread::id,int> thread_id_list;

        template<typename T>     
        void Clear_Vector_Data(std::vector<T *> & vec){

             vec.shrink_to_fit();

             for(size_t i=0;i<vec.size();i++){

                 delete vec.at(i);
             }

             vec.clear();
        }

        bool memory_clear_status;
   };
 };

 #endif /* THREAD_DATA_HOLDER_HPP */