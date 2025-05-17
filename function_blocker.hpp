
 #ifndef FUNCTION_BLOCKER_HPP
 #define FUNCTION_BLOCKER_HPP

 #include "thread_data_holder.hpp"
 #include "thread_mutex.hpp"
 #include <thread>
 #include <mutex>
 #include <iostream>
 #include <string>
 #include <cstdlib>
 #include <chrono>
 #include <condition_variable>
 #include <algorithm>



 namespace pcynlitx{

   class function_blocker
   {
    
   public:

        function_blocker();

        virtual ~function_blocker();

        void Receive_Data_Holder(thread_data_holder * dh);

        void stop(std::string Function_Name, int Rescuer_Thread_Number);

        void run(std::string Function_Name, int Rescuer_Thread_Number);

        void stop(std::string Function_Name); 
    
        // Barrier for the threads executing particular function 

    private:

        pcynlitx::thread_data_holder * data_holder_pointer;

        pcynlitx::thread_mutex Locker;  

        std::vector<std::mutex *> Function_Mutex;

   };
 };

#endif /* FUNCTION_BLOCKER_HPP */