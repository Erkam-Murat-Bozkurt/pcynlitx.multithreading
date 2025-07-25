

 #ifndef SYNCHRONIZER_HPP
 #define SYNCHRONIZER_HPP

 #include "function_blocker.hpp"
 #include "thread_data_holder.hpp"
 #include "thread_mutex.hpp"
 #include "thread_blocker.hpp"
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
    void clear_memory();

    // THE FUNCTIONS FOR INITIALIZATION  -----------------------------

    void connect(std::string Function_Name);
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


    int  number();
    int  operational_thread_number() const;
    int  thread_pool_size() const;
    bool thread_block_status(int thr_num);

    void yield();
    
    std::string function_name();


    // ---------------------------------------------------------------


    
    std::thread::id main_thread_id;
    bool connect_condition;
    bool connection_status;

   private: 
    pcynlitx::thread_data_holder data_holder;
    pcynlitx::function_blocker fnblocker;
    pcynlitx::thread_blocker thrblocker;
    pcynlitx::thread_mutex Outside_Locker;
    pcynlitx::thread_mutex Inside_Locker;

    std::condition_variable cv;
    std::mutex mtx_barrier_wait;
    std::mutex mtx_two_parameter_wait;

    int total_thread_number;
    int Connection_Wait_Counter;
    int waiting_thread_number_in_barrier;
    int * operational_thr_num;

    
    std::vector<std::mutex *> Function_Mutex;

    bool clear_memory_status;

   };
 };

 #endif /* SYNCHRONIZER_HPP */