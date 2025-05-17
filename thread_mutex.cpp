


/*

Copyright [2025] [Erkam Murat Bozkurt]

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*/

#include <thread_mutex.hpp>

pcynlitx::thread_mutex::thread_mutex(){


};


pcynlitx::thread_mutex::~thread_mutex(){

};

void pcynlitx::thread_mutex::lock(){

     this->lock_mutex.lock();
}


void pcynlitx::thread_mutex::unlock(){

     this->lock_mutex.unlock();
}

