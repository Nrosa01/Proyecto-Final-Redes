#include "GameServer.h"
#include "NetworkMessage.h"

GameServer::GameServer(const char *address, const char *port) : socket(address, port), clientManager(&socket)
{
    socket.bind();
}

void GameServer::run()
{
    while (true)
    {
        Socket *clientSocket;
        char *msg = socket.recv(clientSocket);

        if (msg == nullptr)
        {
            std::cerr << "Recv: Error al recibir mensaje\n";
            continue;
        }

        MessageType msgType = getType(msg);

        switch (msgType)
        {
        case MessageType::LOGIN:
            addClient(clientSocket, msg);
            break;
        case MessageType::LOGOUT:
            removeClient(clientSocket);
            break;
        case MessageType::LEVEL_END:
            std::cout << "Recv: Mensaje de tipo LEVEL_END\n";
            break;
        case MessageType::BALL_HIT:
            std::cout << "Recv: Mensaje de tipo BALL_HIT\n";
            break;
        case MessageType::ERROR_MESSAGE:
            std::cout << "Recv: Mensaje de tipo ERROR_MESSAGE\n";
            break;
        default:
            std::cerr << "Recv: Mensaje de tipo desconocido\n";
            break;
        }
    }
}

MessageType GameServer::getType(char *data)
{
    if (data == nullptr || strlen(data) < sizeof(uint8_t))
        return ERROR_MESSAGE;

    uint8_t type;
    memcpy(&type, data, sizeof(uint8_t));

    return (MessageType)type;
}

bool GameServer::isConnected(Socket *socket)
{
    return clients.count(socket->getHashId()) > 0;
}

void GameServer::addClient(Socket *clientSocket, char *msg)
{
    if (isConnected(clientSocket))
    {
        std::cout << "Recv: Cliente ya conectado\n";
        return;
    }

    LoginMessage login;
    login.from_bin(msg);
    clients.insert(std::pair<uint32_t, Socket *>(clientSocket->getHashId(), clientSocket));

    std::cout << "Recv: Mensaje de tipo LOGIN " << login.loginCode << "\n";
    std::cout << "Recv: Clientes conectados: " << clients.size() << "\n";

    bool clientJoined = clientManager.addPlayer(clientSocket, login.loginCode);
    if (clientJoined)
    {
        bool fullRoom = clientManager.isRoomFull(login.loginCode);

        if (fullRoom)
            std::cout << "Player joined to Room, now is full\n";
        else
            std::cout << "Player joined to Room, waiting for other player\n";
    }
    std::cout << "---------- Log end -----------\n";
}

void GameServer::removeClient(Socket *clientSocket)
{
    std::cout << "Recv: Mensaje de tipo LOGOUT\n";
    std::cout << "Recv: Clientes conectados: " << clients.size() << "\n";

    clients.erase(clientSocket->getHashId());
    clientManager.removePlayer(clientSocket);

    std::cout << "---------- Log end -----------\n";
}