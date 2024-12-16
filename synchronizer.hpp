

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
 #include <algorithm>



 namespace pcynlitx{

   class synchronizer
   {
    
   public:

    synchronizer(int thrNum);
    virtual ~synchronizer();

    // THE FUNCTIONS FOR INITIALIZATION  -----------------------------

    void Connect(std::string Function_Name);
    void connection_wait();
    void Receive_Thread_ID(int Thread_Number, std::thread::id id_num);
    void Receive_Main_Thread_Id(std::thread::id main_id);
    void Receive_Operational_Thread_Number(int * threadNum);
    void Exit(int thrNum);

    // ---------------------------------------------------------------



    // THREAD CONTROL FUNCTIONS --------------------------------------

    void lock();
    void unlock();

    void barrier_wait();   // Barrier for each thread
    
    void stop(int Number);
    void run(int Number);

    void start_serial();
    void end_serial();
    
    void stop(int Number, int Rescuer_Thread);
    void run(int Number, int Rescuer_Thread_Number);

    void stop(std::string Function_Name, int Rescuer_Thread_Number);
    void run(std::string Function_Name, int Rescuer_Thread_Number);

    void stop(std::string Function_Name); 
    
    // Barrier for the threads executing particular function 
    
    void function_switch(std::string function_1, std::string function_2);
    void reset_function_switch(std::string function_1, std::string function_2);


    int  Get_Thread_Number();
    int  Get_Operational_Thread_Number() const;
    int  GetTotalThreadNumber() const;
    bool Get_Thread_Block_Status(int Thread_Number);

    void yield();
    
    std::string GetFunctionName(int thread_num);


    // ---------------------------------------------------------------


    
    std::thread::id main_thread_id;
    bool connect_condition;
    bool connection_status;

   private: 

    pcynlitx::thread_data_holder data_holder;
    pcynlitx::thread_locker Outside_Locker;
    pcynlitx::thread_locker Inside_Locker;

    std::condition_variable cv;
    std::mutex mtx_barrier_wait;
    std::mutex mtx_two_parameter_wait;

    int Total_Thread_Number;
    int Connection_Wait_Counter;
    int waiting_thread_number_in_barrier;
    int * operational_thread_number;

    std::vector<std::mutex *> Function_Mutex;

   };
 };

 #endif /* SYNCHRONIZER_HPP */