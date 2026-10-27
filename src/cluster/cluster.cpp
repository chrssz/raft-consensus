#include "cluster.hpp"

Cluster::Cluster(int size) : CLUSTER_SIZE(size){};

SOCKET Cluster::connectTo(const std::string PORT){
    SOCKET ConnectSocket;
    ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(ConnectSocket == INVALID_SOCKET){
        std::cout << "Error creating a connect socket" << std::endl;
        return INVALID_SOCKET;
    }
    
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(std::stoi(PORT));
    
    int result = connect(ConnectSocket, (SOCKADDR*)& addr, sizeof(addr));
    if(result == SOCKET_ERROR){
        std::cout << "Connect to port : " << PORT << " Failed " << std::endl;
        closesocket(ConnectSocket);
        return INVALID_SOCKET;
    }

    return ConnectSocket;
}
void Cluster::start(){
    initWinSock();
    
    std::string const PORTS[] = {"80", "81", "82", "83"};
    
    std::vector<RaftNode> raftNodes;
    std::vector<std::jthread> threads;
    raftNodes.reserve(CLUSTER_SIZE);
    threads.reserve(CLUSTER_SIZE);
    
    for(int i = 0; i < CLUSTER_SIZE; ++i) {
        raftNodes.emplace_back(RaftNode(i));
    }
    
    //Start the nodes.
    for(int i = 0; i < CLUSTER_SIZE; i ++){
         threads.emplace_back(
            std::jthread([&raftNodes, &PORTS, i](){
                raftNodes[i].start(PORTS[i]);
            })
        );
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    //Connect the netwrok
    for(int i = 0; i < CLUSTER_SIZE; ++i){
        
        for(int j = i + 1; j < CLUSTER_SIZE; ++j){
            SOCKET connectSock = connectTo(raftNodes[j].getPort());
            if(connectSock != INVALID_SOCKET){
                raftNodes[i].addConnection(connectSock);
            }
        }
    }
    
    //Wait for all threads to complete
    for(auto& t : threads){
        t.join();
    }

    WSACleanup();
}
Cluster::~Cluster(){

    WSACleanup();
};