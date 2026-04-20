#include "Hypergraphe.h"

void Hypergraphe::addPage(const int pageId, const int blockId) {
    pages.insert(pageId);
    block_pages[blockId].push_back(pageId);
}

void Hypergraphe::addHyperarc(const int blockId, const int targetPageId) {
    bool pageLinksToItsBlock = false;

    for (const int p: block_pages[blockId]) {
        if (p == targetPageId) {
            pageLinksToItsBlock = true;
            break;
        }
    }

    if (!pageLinksToItsBlock) {
        block_out[blockId].insert(targetPageId);
        page_in[targetPageId].insert(blockId);
    }
}

int Hypergraphe::getIndegree(const int pageId) const {
    const auto result = page_in.find(pageId);

    if (result != page_in.end()) {
        return result->second.size();
    }

    return 0;
}
