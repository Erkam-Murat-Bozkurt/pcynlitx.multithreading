
#ifndef SYNCHRONIZER_HPP
#define SYNCHRONIZER_HPP

#include <iostream>
#include <thread>
#include <vector>

namespace pcynlitx {

  template<typename T>     
  class synchronizer
  {
     public:
      synchronizer(){};
      synchronizer(T * ptr){

        this->objPtr=ptr;
      };

      T * objPtr;

      template<typename A>
      A function(A (T::* fPtr) (void));

      template<typename B, typename... args>
      B function(B (T::* fPtr) (args... thParams), args... thParams);

      template<typename B, typename... args>
      B function(B (* func_Ptr) (args... thParams), args... thParams);

      void join(int thrNum);


      void lock();
      void unlock();
      void barrier_wait();
      void wait(int Number);
      void switch_wait(int Number);
      void start_serial(int start_number, int end_number, int thread_number);
      void end_serial(int start_number, int end_number, int thread_number);
      void wait(int Number, int Rescuer_Thread);
      void wait_until_exit(int Number, int Rescuer_Thread);
      void wait(std::string Function_Name, int Rescuer_Thread_Number);
      void wait(std::string Function_Name);
      void Exit();
      void rescue(int Number);
      void rescue(int Number, int Rescuer_Thread_Number);
      void rescue(std::string Function_Name, int Rescuer_Thread_Number);
      void Receive_Thread_ID(std::string Function_Name, int Thread_Number);
      int Get_Thread_Number();
      int Get_Operational_Thread_Number() const;
      bool Get_Thread_Block_Status(int Thread_Number) const;
      void yield();

     protected:
      std::vector<std::thread> threadPool;
   };


   template<typename T> template<typename A>
   A synchronizer<T>::function(A (T::* fPtr) (void)){

     this->threadPool.push_back(std::thread(fPtr,objPtr));
   }
   
   template<typename T> template<typename B, typename... args>
   B synchronizer<T>::function(B (T::* fPtr) (args... thParams), args... thParams){

      this->threadPool.push_back(std::thread(fPtr,objPtr,thParams...));
   };

   template<typename T> template<typename B, typename... args>
   B synchronizer<T>::function(B (* func_Ptr) (args... thParams), args... thParams){

      this->threadPool.push_back(std::thread(func_Ptr,objPtr,thParams...));
   };

   template<typename T>
   void synchronizer<T>::join(int thrNum){

         this->threadPool.at(thrNum).join();
   }

};




#endif  /* SYNCHRONIZER_HPP */