#ifndef NET_ANALYZER_H_
#define NET_ANALYZER_H_

#include "node.h"

#include <iostream>
#include <fstream> 
#include <sstream>
#include <string>
#include <filesystem>

class NetAnalyzer
{
public:

    NetAnalyzer() = default;

    void Analyze(const std::string& filename);

    template<int length>
    std::array<int, length> GetAddress(const std::vector<std::string>& addr) const;

    void AddTransmittingNode(const std::array<int, IP_ADDRESS_LENGTH>& ipAddr_, const std::array<int, MAC_ADDRESS_LENGTH>& macAddr_, int firstDataTransferredSize_,
        double firstDataTransferTime_, const Node::IP_To_Protocol& trasmittedTo_);

    void AddReceivingNode(const std::array<int, IP_ADDRESS_LENGTH>& ipAddr_, const std::array<int, MAC_ADDRESS_LENGTH>& macAddr_, const Node::IP_To_Protocol& receivedFrom_);

private:
    std::unordered_set<Node, Node::HashFunction> nodes;

    std::vector<std::string> split(const std::string& str, char d);
};
#endif
