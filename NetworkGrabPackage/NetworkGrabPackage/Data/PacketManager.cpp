#include "PacketManager.hpp"

void PacketManager::AddPacket(std::unique_ptr<Packet> packet) {
    packets.push_back(std::move(packet));
}

void PacketManager::ParseAll() {
    for (auto& packet : packets) {
        try {
            packet->Parse();
        }
        catch (const std::exception& e) {
            std::cerr << "Error parsing packet: " << e.what() << std::endl;
        }
    }
}

void PacketManager::displayAll() const {
    for (const auto& packet : packets) {
        packet->display();
        std::cout << std::endl;
    }
}

void PacketManager::displaySummaries() const {
    for (const auto& packet : packets) {
        std::cout << packet->getSummary() << std::endl;
    }
}
