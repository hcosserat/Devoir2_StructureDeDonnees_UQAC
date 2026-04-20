#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "../SDA/Hypergraphe/Hypergraphe.h"

std::vector<std::pair<int, int> > indegree(const Hypergraphe &h, const int count) {
    std::vector<std::pair<int, int> > indegrees;

    indegrees.reserve(h.pages.size());

    for (int p: h.pages) {
        indegrees.emplace_back(p, h.getIndegree(p));
    }

    std::ranges::sort(indegrees, [](const std::pair<int, int> &a, const std::pair<int, int> &b) {
        return a.second > b.second;
    });

    indegrees.resize(count);

    return indegrees;
}

std::vector<std::pair<int, double> > pageRank(const Hypergraphe &h, const int iterations) {
    // Tout le monde commence avec un score de 1/N, puis on met a jour iterativement avec la formule
    //
    // PR_{t+1}(p) = (1 - d)/N + d * somme, pour chaque q pointant vers p, de PR_t(q) / deg_sortant(q)
    //

    const double damping = 0.85;
    const double pageCount = static_cast<double>(h.pages.size());
    const double teleportation = (1.0 - damping) / pageCount;

    std::unordered_map<int, double> score;
    score.reserve(h.pages.size());

    const double initialScore = 1.0 / pageCount;
    for (int pageId: h.pages) {
        score[pageId] = initialScore;
    }

    for (int step = 0; step < iterations; ++step) {
        std::unordered_map<int, double> nextScore;
        nextScore.reserve(h.pages.size());

        for (int targetPageId: h.pages) {
            double incomingContribution = 0.0;

            const auto incomingBlocksIt = h.page_in.find(targetPageId);
            if (incomingBlocksIt != h.page_in.end()) {
                for (int sourceBlockId: incomingBlocksIt->second) {
                    double sourceBlockScore = 0.0;

                    const auto blockPagesIt = h.block_pages.find(sourceBlockId);
                    if (blockPagesIt != h.block_pages.end()) {
                        for (int sourcePageId: blockPagesIt->second) {
                            sourceBlockScore += score[sourcePageId];
                        }
                    }

                    const auto outgoingTargetsIt = h.block_out.find(sourceBlockId);
                    const std::size_t outgoingTargetCount =
                            (outgoingTargetsIt == h.block_out.end()) ? 0 : outgoingTargetsIt->second.size();

                    if (outgoingTargetCount > 0) {
                        incomingContribution += sourceBlockScore / static_cast<double>(outgoingTargetCount);
                    }
                }
            }

            nextScore[targetPageId] = teleportation + damping * incomingContribution;
        }

        score = std::move(nextScore);
    }

    std::vector<std::pair<int, double> > ranking(score.begin(), score.end());
    std::ranges::sort(ranking, [](const auto &a, const auto &b) {
        return a.second > b.second;
    });

    return ranking;
}

void computeRankings(const std::string &nodesPath, const std::string &edgesPath, bool useDomainGranularity) {
    Hypergraphe h;
    std::unordered_map<int, int> nodeToBlock;
    std::unordered_map<int, std::string> idToUrl;
    std::unordered_map<std::string, int> domainToBlockId;
    int nextBlockId = 0;

    std::cout << "Chargement des noeuds" << std::endl;

    std::ifstream fnodes(nodesPath);
    std::string line;
    std::getline(fnodes, line);
    std::getline(fnodes, line);

    while (std::getline(fnodes, line)) {
        std::stringstream ss(line);
        int id, outdeg;
        std::string url;

        // parsing de la ligne
        if (ss >> id >> outdeg >> url) {
            idToUrl[id] = url;
            int blockId;

            if (useDomainGranularity) {
                size_t start = url.find("://");
                if (start == std::string::npos) {
                    start = 0;
                } else {
                    start += 3;
                }
                const size_t end = url.find('/', start);
                std::string domain = url.substr(start, end - start);
                if (!domainToBlockId.contains(domain)) {
                    domainToBlockId[domain] = ++nextBlockId;
                }
                blockId = domainToBlockId[domain];
            } else {
                blockId = id;
            }

            nodeToBlock[id] = blockId;
            h.addPage(id, blockId);
        }
    }

    std::cout << "Chargement des arêtes" << std::endl;

    std::ifstream fedges(edgesPath);
    std::getline(fedges, line);
    std::getline(fedges, line);

    while (std::getline(fedges, line)) {
        std::stringstream ss(line);
        int from, to;

        // parsing de la ligne
        if (ss >> from >> to) {
            if (from == to) continue;

            if (nodeToBlock.contains(from) && nodeToBlock.contains(to)) {
                int blockId = nodeToBlock[from];
                h.addHyperarc(blockId, to);
            }
        }
    }

    std::cout << "Indegree :\n Calcul en cours..." << std::endl;

    auto iRank = indegree(h, 10);

    for (int i = 0; i < iRank.size(); ++i) {
        std::cout << "  (" << i + 1 << ") " << idToUrl[iRank[i].first] << " Score : " << iRank[i].second
                << std::endl;
    }

    std::cout << "PageRank :\n  Calcul en cours..." << std::endl;

    auto pRank = pageRank(h, 10);

    for (int i = 0; i < 10 && i < pRank.size(); ++i) {
        std::cout << "  (" << i + 1 << ") " << idToUrl[pRank[i].first] << " Score : " << pRank[i].second <<
                std::endl;
    }
}

int main() {
    const std::string edgesPath = "data/ex2/eu-2005/eu-2005.edges.txt";
    const std::string nodesPath = "data/ex2/eu-2005/eu-2005.nodes.txt";

    // const std::string edgesPath = "data/ex2/in-2004/in-2004.edges.txt";
    // const std::string nodesPath = "data/ex2/in-2004/in-2004.nodes.txt";

    std::cout << "Classement avec regroupement par page" << std::endl;
    computeRankings(nodesPath, edgesPath, false);

    std::cout << "\nClassement avec regroupement par domaine" << std::endl;
    computeRankings(nodesPath, edgesPath, true);

    return 0;
}
