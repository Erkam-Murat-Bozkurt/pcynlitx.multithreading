
#ifndef THREAD_SERVER_HPP
#define THREAD_SERVER_HPP

#include <iostream>
#include <thread>
#include <vector>

namespace pcynlitx {

  template<typename T>     
  class thread_server
  {
     public:
      thread_server(){};
      thread_server(T * ptr){

        this->objPtr=ptr;
      };

      T * objPtr;

      template<typename B, typename... args>
      B function(B (T::* fPtr)  (args... thParams), args... thParams);

      template<typename B, typename... args>
      B function(B (* func_Ptr) (args... thParams), args... thParams);

      void join(int thrNum);

     protected:
      std::vector<std::thread> threadPool;
   };

   template<typename T> template<typename B, typename... args>
   B thread_server<T>::function(B (T::* func_Ptr) (args... thParams), args... thParams){

      this->threadPool.push_back(std::thread(func_Ptr,objPtr,thParams...));
   };

   template<typename T> template<typename B, typename... args>
   B thread_server<T>::function(B (* func_Ptr) (args... thParams), args... thParams){

      this->threadPool.push_back(std::thread(func_Ptr,objPtr,thParams...));
   };

   template<typename T>
   void thread_server<T>::join(int thrNum){

         this->threadPool.at(thrNum).join();
   }

};




#endif  /* THREAD_SERVER_HPP */