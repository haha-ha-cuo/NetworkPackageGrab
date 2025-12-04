#pragma once

#include "Packet.h"


class PacketManager
{
public:
	PacketManager() = default;
	~PacketManager() = default;
	void AddPacket(unique_ptr<Packet> packet);
	void ParseAll();
	void displayAll() const;
	void displaySummaries() const;
	const vector<unique_ptr<Packet>>& GetPackets() const {
		return packets;
	}
	size_t GetPacketCount() const {
		return packets.size();
	}

private:
	vector<unique_ptr<Packet>> packets;

};

