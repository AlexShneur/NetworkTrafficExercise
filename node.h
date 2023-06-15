#ifndef NODE_H_
#define NODE_H_

#include <array>
#include <unordered_set>
#include <vector>
#include <cstddef>

constexpr int IP_ADDRESS_LENGTH = 4;
constexpr int MAC_ADDRESS_LENGTH = 6;

class Node
{
public:

    using IP_To_Protocol = std::pair<std::array<int, IP_ADDRESS_LENGTH>, bool>;     //bool - isUDP

    Node(const std::array<int, IP_ADDRESS_LENGTH>& ipAddr_, const std::array<int, MAC_ADDRESS_LENGTH>& macAddr_, int firstDataTransferredSize_,
        double firstDataTransferTime_, const IP_To_Protocol& trasmittedTo_) :
        ipAddr(ipAddr_), macAddr(macAddr_), TotalDataTrasferredSize(firstDataTransferredSize_), TotalDataTransferredTime(firstDataTransferTime_),
        SessionsCount(1), ProxyCount(0), IsProxy(false)
    {
        TransmittedTo.push_back(trasmittedTo_);
    }

    Node(const std::array<int, IP_ADDRESS_LENGTH>& ipAddr_, const std::array<int, MAC_ADDRESS_LENGTH>& macAddr_, const IP_To_Protocol& receivedFrom_) :
        ipAddr(ipAddr_), macAddr(macAddr_), SessionsCount(0), ProxyCount(0), IsProxy(false)
    {
        ReceivedFrom.push_back(receivedFrom_);
    }

    std::array<int, IP_ADDRESS_LENGTH> GetIpAddr() const;
    double GetAvgTransmitSpeed() const;

    bool operator==(const Node& otherNode) const;
    struct HashFunction
    {
        size_t operator()(const Node& node) const;
    };

    mutable long TotalDataTrasferredSize;
    mutable double TotalDataTransferredTime;
    mutable int SessionsCount;
    mutable std::vector<IP_To_Protocol> TransmittedTo;
    mutable std::vector<IP_To_Protocol> ReceivedFrom;
    mutable int ProxyCount;
    mutable bool IsProxy;

private:
    std::array<int, IP_ADDRESS_LENGTH> ipAddr;
    std::array<int, MAC_ADDRESS_LENGTH> macAddr;
};
#endif
