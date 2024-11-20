
 #ifndef THREAD_DATA_HOLDER_HPP
 #define THREAD_DATA_HOLDER_HPP

 #include "thread_locker.hpp"
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
        int  Thread_Number;
        int  wait_enter_counter;
        int  wait_termination;
        std::string Thread_Function_Name;
        bool Thread_Operational_Status;
        bool ref_wait_status;
        bool Rescue_Permission;
        bool Thread_Block_Status;
        int  wait_untill_exit_thread_number;
        std::condition_variable Condition_Variable;
    };

    struct Function_Names_Data
    {
        int  Function_Number;
        int  Member_Counter;
        bool Rescue_Permission;
        int  Enter_Counter;
        int  function_block_wait_status;
        int  Two_Prm_Function_Enter_Counter;
        std::string Thread_Function_Name;
        std::condition_variable Condition_Variable;
    };


    class thread_data_holder
    {
    public:
        thread_data_holder();
        virtual ~thread_data_holder();
        void Receive_Total_Thread_Number(int num);
        void Receive_Thread_ID(std::string Function_Name, int Thread_Number);
        void Increase_Wait_Enter_Counter(int Thread_Number);
        void Increase_Function_Wait_Enter_Counter(std::string Function_Name);
        void Increase_Two_Prm_Function_Wait_Enter_Counter(std::string Function_Name);
        void Rescue_Function_Members(std::string Function_Name);
        void Wait_Thread_Termination(int Number, int Rescuer_Thread);
        int  Get_Thread_Waits_This_Thread_Termination(int Number) const;
        int  Get_Function_Wait_Enter_Counter(std::string Function_Name);
        int  Get_Two_Prm_Function_Wait_Enter_Counter(std::string Function_Name);
        void Set_Rescue_Permission(int Thread_Number, bool permission);
        void Set_Thread_Block_Status(int Thread_Number, bool status);
        void Set_Wait_Enter_Counter(int Thread_Number, int Number);
        void Set_Function_Wait_Enter_Counter(std::string Function_Name, int Number);
        void Set_Two_Prm_Function_Wait_Enter_Counter(std::string Function_Name, int Number);
        void Set_Function_Rescue_Permission(std::string Function_Name, bool permission);
        void Set_Function_Name(int Thread_Number, std::string Function_Name);
        bool Get_Function_Rescue_Permission(std::string Function_Name);
        int  Get_Wait_Enter_Counter(int Thread_Number) const;
        bool Get_Rescue_Permission(int Thread_Number) const;
        bool Get_Thread_Block_Status(int Thread_Number) const;
        bool Get_Dead_Lock_Risk();
        int  Get_Thread_Number();
        int  Get_Function_Member_Number(std::string Function_Name);
        std::string Get_Function_Name(int Thread_Number) const;
        int  Get_Operational_Thread_Number() const;
        bool Get_Thread_Operational_Status(int Thread_Number) const;
        int  GetFirstThreadExecutingFunction(std::string Function_Name);
        void Stop_Thread(std::unique_lock<std::mutex> * mtx, int thread_number);
        void Activate_Thread(int thread_number);
        void Set_Block_Function_Wait_Status(std::string function, int status);
        int  Get_Block_Function_Wait_Status(std::string function);
        bool Is_Exist_On_FunctionStack(std::string path);
        Function_Names_Data * Find_Function_Data_From_Name(std::string name);
        void Exit();
   private:
        thread_locker Inside_Locker;
        thread_locker Thread_Exit_Locker;

        void Add_Function_Data(std::string Function_Name);
        void Add_Thread_Data(std::string Function_Name, int Thread_Number);

        int Total_Thread_Number;
        int Operational_Thread_Number;
        int Thread_Function_Number;
        int Caller_Thread_Number;
        int The_First_Thread_Execution_Function;
        bool Dead_Lock_Risk;
        std::vector<pcynlitx::Thread_Data *> Thread_Data_List;
        std::vector<pcynlitx::Function_Names_Data *> Function_Names_Data_List;
        std::vector<std::string> Function_Name_List;
        std::vector<std::condition_variable * > cond_list;
        std::unordered_map<std::string, pcynlitx::Function_Names_Data *> function_data_map;
        std::unordered_map<int, Thread_Data *> thread_data_map;
   };
 };

 #endif /* THREAD_DATA_HOLDER_HPP */