#include "netAnalyzer.h"

constexpr int ANALYZE_NUMBER = 10;
constexpr int PROXY_MIN_NUMBER = 2;

auto sortByAvgSpeed = [](const Node& n1, const Node& n2) -> bool
{
    return n1.GetAvgTransmitSpeed() < n2.GetAvgTransmitSpeed();
};

auto sortBySessionsCount = [](const Node& n1, const Node& n2) -> bool
{
    return n1.SessionsCount < n2.SessionsCount;
};

template<int length>
std::ostream& operator<<(std::ostream& os, const std::array<int, length>& arr)
{
    for (auto i = 0; i < length-1; ++i)
    {
        os << arr[i] << ".";
    }
    os << *(arr.end()-1) << " ";
    return os;
}

auto operator==(const Node::IP_To_Protocol& pair1, const Node::IP_To_Protocol& pair2) -> bool
{
    return pair1.first == pair2.first && pair1.second == pair2.second;
}

void NetAnalyzer::Analyze(std::string_view filename)
{
    std::ifstream file(filename);

    std::string line;
    long long totalSize = 0;
    double totalTime = 0;
    double maxSpeed = 0.0;
    bool isUDPMaxSpeed = false;

    std::cout << "Please, wait - analyzing... " << std::endl;

    while (getline(file, line))
    {
        try
        {
            auto v = split(line, '\n');
            auto nodeLine = split(v.at(0), ';');

            auto ipPortFromLine = split(nodeLine.at(0), ':');
            auto ip = split(ipPortFromLine.at(0), '.');
            auto ipFrom = GetAddress<IP_ADDRESS_LENGTH>(ip);
            auto portFrom = ipPortFromLine[1];

            auto macLine = split(nodeLine.at(1), ';');
            auto mac = split(macLine.at(0), ':');
            auto macFrom = GetAddress<MAC_ADDRESS_LENGTH>(mac);

            auto ipPortToLine = split(nodeLine.at(2), ':');
            ip = split(ipPortToLine.at(0), '.');
            auto ipTo = GetAddress<IP_ADDRESS_LENGTH>(ip);
            auto portTo = ipPortToLine[1];

            macLine = split(nodeLine.at(3), ';');
            mac = split(macLine.at(0), ':');
            auto macTo = GetAddress<MAC_ADDRESS_LENGTH>(mac);

            auto isUDP = false;
            std::istringstream(split(nodeLine.at(4), ';')[0]) >> std::boolalpha >> isUDP;
            auto dataSize = stoi(split(nodeLine.at(5), ';')[0]);

            totalSize += dataSize;

            double time = 0.0;
            std::istringstream(split(nodeLine.at(6), ';')[0]) >> time;
            totalTime += time;

            auto ipToProtocol = std::make_pair(ipTo, isUDP);
            AddTransmittingNode(ipFrom, macFrom, dataSize, time, ipToProtocol);

            ipToProtocol = std::make_pair(ipFrom, isUDP);
            AddReceivingNode(ipTo, macTo, ipToProtocol);

            auto speed = dataSize / time;
            if (speed > maxSpeed)
            {
                maxSpeed = speed;
                isUDPMaxSpeed = isUDP;
            }
        }
        catch (const std::exception& ex)
        {
            //std::cerr << ex.what() << std::endl;
        }
    }

    std::cout << std::endl <<  "Q1: " << nodes.size() << " unique nodes in the net" << std::endl;
    std::cout << std::endl << "Q2: " << totalSize / totalTime << " byte/s average speed" << std::endl;
    if (isUDPMaxSpeed)
        std::cout << std::endl << "Q3: yes,  UDP has maximum peak data rate" << std::endl;
    else
        std::cout << std::endl << "Q3: no, UDP has not maximum peak data rate" << std::endl;


    std::vector<Node> vNodes(nodes.begin(), nodes.end());

    std::sort(vNodes.begin(), vNodes.end(), sortByAvgSpeed);
    std::cout << std::endl << "Q4: nodes with the highest average data transfer rate: " << std::endl;
    for (auto i=vNodes.size()-1; i> vNodes.size() - 1 - ANALYZE_NUMBER; i--)
    {
        std::cout << vNodes[i].GetIpAddr() << "avgSpeed = " << vNodes[i].GetAvgTransmitSpeed() << " bytes/s" << std::endl;
    }

    std::sort(vNodes.begin(), vNodes.end(), sortBySessionsCount);
    std::cout << std::endl << "Q5: most active subnets: " << std::endl;
    for (auto i = vNodes.size() - 1; i > vNodes.size() - 1 - ANALYZE_NUMBER; i--)
    {
        std::cout << vNodes[i].GetIpAddr() << "sessionsCount = " << vNodes[i].SessionsCount << std::endl;
    }

    std::cout << std::endl << "Q6: proxy nodes: " << std::endl;
    bool exists = false;
    for (const auto& node : nodes)
    {
        if (node.IsProxy)
        {
            std::cout << node.GetIpAddr() << std::endl;
            exists = true;
        }
    }
    if (!exists)
        std::cout << "The are no proxy nodes in the net." << std::endl;

    std::cout << std::endl << "Analyzing finished successfully." << std::endl;
}

void NetAnalyzer::AddTransmittingNode(const std::array<int, IP_ADDRESS_LENGTH>& ipAddr_, const std::array<int, MAC_ADDRESS_LENGTH>& macAddr_, int firstDataTransferredSize_, double firstDataTransferTime_, const Node::IP_To_Protocol & trasmittedTo_)
{
    auto result = nodes.emplace(ipAddr_, macAddr_, firstDataTransferredSize_, firstDataTransferTime_, trasmittedTo_);
    if (!result.second)
    {
        result.first->TotalDataTrasferredSize += firstDataTransferredSize_;
        result.first->TotalDataTransferredTime += firstDataTransferTime_;
        result.first->SessionsCount++;

        result.first->TransmittedTo.push_back(trasmittedTo_);
        if (!result.first->IsProxy)
        {
            auto receivedFrom = std::find(result.first->ReceivedFrom.begin(), result.first->ReceivedFrom.end(), trasmittedTo_);
            if (receivedFrom != result.first->ReceivedFrom.end())
            {
                result.first->ProxyCount++;
                if (result.first->ProxyCount == PROXY_MIN_NUMBER)
                {
                    result.first->IsProxy = true;
                }
            }
        }
    }
}

void NetAnalyzer::AddReceivingNode(const std::array<int, IP_ADDRESS_LENGTH>& ipAddr_, const std::array<int, MAC_ADDRESS_LENGTH>& macAddr_, const Node::IP_To_Protocol & receivedFrom_)
{
    auto result = nodes.emplace(ipAddr_, macAddr_, receivedFrom_);
    if (!result.second)
    {
        result.first->ReceivedFrom.push_back(receivedFrom_);
        if (!result.first->IsProxy)
        {
            auto transmittedTo = std::find(result.first->TransmittedTo.begin(), result.first->TransmittedTo.end(), receivedFrom_);
            if (transmittedTo != result.first->TransmittedTo.end())
            {
                result.first->ProxyCount++;
                if (result.first->ProxyCount == PROXY_MIN_NUMBER)
                {
                    result.first->IsProxy = true;
                }
            }
        }
    }
}

template<int length>
std::array<int, length> NetAnalyzer::GetAddress(const std::vector<std::string>& addr) const
{
    std::array<int, length> arr{};
    for (auto i = 0; i < addr.size(); i++)
    {
        if (!addr.at(i).empty())
            arr[i] = length > IP_ADDRESS_LENGTH ? stoi(addr.at(i), 0, 16) : stoi(addr.at(i));
    }
    return arr;
}

std::vector<std::string> NetAnalyzer::split(const std::string& str, char d)
{
    std::vector<std::string> r;

    size_t start = 0;
    auto stop = str.find_first_of(d);
    while (stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}
