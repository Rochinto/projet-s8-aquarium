
#include "Graphics.hpp"
#include "Prompt.hpp"
#include "Config.hpp"

int main(int argc, char *argv[]){
    utils::setup();
    
    options opts;
    #ifndef ENSEIRB
    if(argc == 1){
        opts = get_options_config("affichage.cfg");
        cout << opts.address << opts.id << opts.port << opts.resources << opts.timeout << endl;
    }else 
    #endif
    if(argc == 2 && string(argv[1])=="debug"){
        opts.address = "debug";
        utils::setDebug();
        cout << "[DEBUG MODE ON]\n";
        utils::printlog("[DEBUG MODE ON]");
    }else if(argc < 3){
        cerr << "Bad arguments : " << argv[0] << " <host> <port>" << endl;
        return 1;
    }else{
        opts.address = argv[1];
        opts.port = atoi(argv[2]);
    }

    Aquarium aquarium("Aquarium de test",WIDTH,HEIGHT,FPS);
    Graphics graphics(aquarium);
    thread thread_graphics(&Graphics::loop, &graphics);

    
    SocketAPI api;
    if(!utils::debug()){
        api = SocketAPI(opts.address, opts.port, &aquarium);
    }else{
        api = SocketAPI(string("debug"), 0, &aquarium);
    }
    
    thread thread_socket = thread(&SocketAPI::loopRead, &api);
    Prompt parser(&api, &aquarium);
    api.sendCommand("hello");
    api.sendCommand("getFishesContinuously");
    //cout << "$ ";
    while(parser.prompt_user());
    cout << endl;

    graphics.stop();
    thread_graphics.join();
    if(!api.sendCommand("log out", "bye")) utils::error("Le serveur n'a pas répondu (bye) après envoi de : log out");
    api.close(&thread_socket);
    utils::close();

    exit(0);
    return 0;
}