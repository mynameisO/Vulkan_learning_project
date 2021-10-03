#include "MEApp.hpp"

int main(){
    ME::MEApp App{800,600,"ME Engine"};
    try{
        App.run();
    }catch(std::exception& e){
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}