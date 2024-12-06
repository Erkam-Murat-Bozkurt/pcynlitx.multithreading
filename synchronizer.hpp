

 #ifndef SYNCHRONIZER_HPP
 #define SYNCHRONIZER_HPP

 #include "thread_data_holder.hpp"
 #include "thread_locker.hpp"
 #include <thread>
 #include <mutex>
 #include <iostream>
 #include <string>
 #include <cstdlib>
 #include <chrono>
 #include <condition_variable>



 namespace pcynlitx{

   class synchronizer
   {
   public:
    synchronizer(int thrNum);
    virtual ~synchronizer();
    void Connect(std::string Function_Name);
    void lock();
    void unlock();
    void barrier_wait();
    void connection_wait();
    void wait(int Number);
    void switch_wait(int Number);
    void start_serial(int start_number, int end_number, int thread_number);
    void end_serial(int start_number, int end_number, int thread_number);
    void wait(int Number, int Rescuer_Thread);
    void wait_until_exit(int Number, int Rescuer_Thread);
    void wait(std::string Function_Name, int Rescuer_Thread_Number);
    void wait(std::string Function_Name);
    void Exit(int thrNum);
    void rescue(int Number);
    void rescue(int Number, int Rescuer_Thread_Number);
    void rescue(std::string Function_Name, int Rescuer_Thread_Number);
    void Receive_Thread_ID(int Thread_Number, std::thread::id id_num);
    void Receive_Main_Thread_Id(std::thread::id main_id);
    void Receive_Operational_Thread_Number(int * threadNum);
    int  Get_Thread_Number();
    std::string GetFunctionName(int thread_num);
    int  Get_Operational_Thread_Number() const;
    bool Get_Thread_Block_Status(int Thread_Number) const;
    void function_switch(std::string function_1, std::string function_2);
    void reset_function_switch(std::string function_1, std::string function_2);
    std::thread::id main_thread_id;
    bool connect_condition;
    bool connection_status;
    int GetTotalThreadNumber() const;
    void yield();
   private:
    void Check_Is_There_Waiting_Until_Exit();
    std::condition_variable cv;
    std::mutex mtx_barrier_wait;
    std::mutex mtx_switch_wait;
    std::mutex mtx_two_parameter_wait;
    thread_locker Outside_Locker;
    thread_data_holder data_holder;
    thread_locker Inside_Locker;
    int Total_Thread_Number;
    int Connection_Wait_Counter;
    int Thread_Function_Number;
    int Thread_On_Point_Wait;
    int waiting_thread_number_in_barrier;
    int Function_enter_counter_with_rescuer_thread;
    int Function_enter_counter;
    int * operational_thread_number;
    std::mutex Function_Mutex[3];
    std::mutex Two_Pr_Function_Mutex[3];
    std::mutex Thread_Mutex[4];
   };
 };

 #endif /* SYNCHRONIZER_HPP */