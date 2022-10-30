#include<iostream>
#include<thread>

void sayHello(){
    for(int i = 0; i < 100000000; i++){}

    std::cout << "hello programmer\n";

}

void sayHello2(){
    std::cout << "hello programmer\n";
}

std::thread th1(sayHello);
std::thread th2(sayHello);
std::thread th3(sayHello2);


int main(){

    {
        std::cin.get();
        th1.join();
        std::cout << "main: line of code after th1.join()" << std::endl;
        th2.join();
    }

    {
        std::cout << "main: line of code after scope of 2 thread" << std::endl;
        th3.join();
    }

    return 0;
}
