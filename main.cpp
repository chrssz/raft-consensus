#include "src/raftnode/raftnode.hpp" 
SOCKET connectTo(const std::string PORT){
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
        WSACleanup();
        closesocket(ConnectSocket);
        return INVALID_SOCKET;
    }

    return ConnectSocket;
}

int main(){
    initWinSock();
    u_int const CLUSTER_SIZE = 4;
    std::string const PORTS[] = {"80", "81", "82", "83"};
    
    std::vector<RaftNode> cluster = {};

    for(int i = 0; i < CLUSTER_SIZE; ++i) {
        cluster.emplace_back(RaftNode());
        cluster[i].start(PORTS[i]);
    }

    //Connect the netwrok
    for(int i = 0; i < CLUSTER_SIZE; ++i){
        
        for(int j = i + 1; j < CLUSTER_SIZE; ++j){
            SOCKET connectSock = connectTo(cluster[j].getPort());
            if(connectSock != INVALID_SOCKET){
                cluster[i].addConnection(connectSock);

            }
        }
    }
    
    WSACleanup();
}