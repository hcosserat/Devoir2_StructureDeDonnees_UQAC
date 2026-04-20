#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

class Hypergraphe {
public:
    // id de chaque page
    std::unordered_set<int> pages;

    // associe un id de bloc aux pages du bloc
    std::unordered_map<int, std::vector<int> > block_pages;

    // associe l'id d'une page avec les blocs qui pointent vers la page
    std::unordered_map<int, std::unordered_set<int> > page_in;

    // associe l'id d'un bloc avec les pages qu'il référence
    std::unordered_map<int, std::unordered_set<int> > block_out;

    void addPage(int pageId, int blockId);

    void addHyperarc(int blockId, int targetPageId);

    int getIndegree(int pageId) const;
};
