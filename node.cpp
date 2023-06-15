#include "node.h"

std::array<int, IP_ADDRESS_LENGTH> Node::GetIpAddr() const
{
    return ipAddr;
}

double Node::GetAvgTransmitSpeed() const
{
    if (TotalDataTransferredTime > 0)
        return TotalDataTrasferredSize / TotalDataTransferredTime;
    else
        return 0.0;
}

bool Node::operator==(const Node& otherNode) const
{
    return this->GetIpAddr() == otherNode.GetIpAddr();
}

size_t Node::HashFunction::operator()(const Node & node) const
{
    auto ipAddr = node.GetIpAddr();
    size_t ipHash0 = std::hash<int>()(ipAddr[0]);
    size_t ipHash1 = std::hash<int>()(ipAddr[1]);
    size_t ipHash2 = std::hash<int>()(ipAddr[2]);
    size_t ipHash3 = std::hash<int>()(ipAddr[3]);
    return ipHash0 ^ ipHash1 ^ ipHash2 ^ ipHash3;
}
