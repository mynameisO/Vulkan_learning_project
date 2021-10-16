#include "App.hpp"

int main(){
    ME::MEApp app{"Welcome to hell"};

    try{
        app.run();
    }catch(std::exception& e){
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}