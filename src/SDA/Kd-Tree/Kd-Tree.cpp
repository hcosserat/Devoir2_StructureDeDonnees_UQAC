#include "KdTree.h"

#include <ostream>
#include <limits>
#include <string>

namespace KDTree {

void KdTree::clear() {
	root_.reset();
	count_ = 0;
}

bool KdTree::empty() const {
	return count_ == 0;
}

std::size_t KdTree::size() const {
	return count_;
}

std::size_t KdTree::dimensions() const {
	return dimensions_;
}

void KdTree::insert(const Point& point) {
	validatePointDimension(point);
	insertRecursive(root_, point, 0);
}

bool KdTree::remove(const Point& point) {
	validatePointDimension(point);
	return removeRecursive(root_, point, 0);
}

bool KdTree::contains(const Point& point) const {
	validatePointDimension(point);
	return containsRecursive(root_.get(), point, 0);
}

std::vector<Point> KdTree::rangeQuery(const Point& minCorner, const Point& maxCorner) const {
	validatePointDimension(minCorner);
	validatePointDimension(maxCorner);

	for (std::size_t i = 0; i < dimensions_; ++i) {
		if (minCorner[i] > maxCorner[i]) {
			throw std::invalid_argument("rangeQuery requires minCorner[i] <= maxCorner[i] for all axes");
		}
	}

	std::vector<Point> result;
	rangeQueryRecursive(root_.get(), minCorner, maxCorner, 0, result);
	return result;
}

Point KdTree::nearestNeighbor(const Point& target) const {
	validatePointDimension(target);
	if (!root_) {
		throw std::runtime_error("nearestNeighbor called on an empty KdTree");
	}

	const Node* best = nullptr;
	double bestDistSquared = std::numeric_limits<double>::infinity();
	nearestNeighborRecursive(root_.get(), target, 0, best, bestDistSquared);
	return best->point;
}

void KdTree::print(std::ostream& out) const {
	if (!root_) {
		out << "[empty tree]\n";
		return;
	}

	out << "KD-Tree (" << dimensions_ << "D, " << count_ << " node(s))\n";
	printRecursive(root_.get(), out, "", false, 0);
}


// Recursive helper functions
void KdTree::insertRecursive(std::unique_ptr<Node>& node, const Point& point, int depth) {
	if (!node) {
		node = std::make_unique<Node>(point);
		++count_;
		return;
	}

	if (point == node->point) {
		return;
	}

	const int axis = axisAtDepth(depth);
	if (point[static_cast<std::size_t>(axis)] < node->point[static_cast<std::size_t>(axis)]) {
		insertRecursive(node->left, point, depth + 1);
	} else {
		insertRecursive(node->right, point, depth + 1);
	}
}

bool KdTree::containsRecursive(const Node* node, const Point& point, int depth) const {
	if (!node) {
		return false;
	}

	if (point == node->point) {
		return true;
	}

	const int axis = axisAtDepth(depth);
	if (point[static_cast<std::size_t>(axis)] < node->point[static_cast<std::size_t>(axis)]) {
		return containsRecursive(node->left.get(), point, depth + 1);
	}
	return containsRecursive(node->right.get(), point, depth + 1);
}

bool KdTree::removeRecursive(std::unique_ptr<Node>& node, const Point& point, int depth) {
	if (!node) {
		return false;
	}

	const int axis = axisAtDepth(depth);
	if (node->point == point) {
		if (node->right) {
			const Node* successor = findMinRecursive(node->right.get(), axis, depth + 1);
			node->point = successor->point;
			return removeRecursive(node->right, successor->point, depth + 1);
		}

		if (node->left) {
			const Node* predecessor = findMaxRecursive(node->left.get(), axis, depth + 1);
			node->point = predecessor->point;
			return removeRecursive(node->left, predecessor->point, depth + 1);
		}

		node.reset();
		--count_;
		return true;
	}

	if (point[static_cast<std::size_t>(axis)] < node->point[static_cast<std::size_t>(axis)]) {
		return removeRecursive(node->left, point, depth + 1);
	}
	return removeRecursive(node->right, point, depth + 1);
}

const KdTree::Node* KdTree::findMinRecursive(const Node* node, int targetAxis, int depth) const {
	if (!node) {
		return nullptr;
	}

	const int currentAxis = axisAtDepth(depth);
	const std::size_t axisIdx = static_cast<std::size_t>(targetAxis);

	if (currentAxis == targetAxis) {
		if (!node->left) {
			return node;
		}
		return findMinRecursive(node->left.get(), targetAxis, depth + 1);
	}

	const Node* best = node;
	const Node* leftBest = findMinRecursive(node->left.get(), targetAxis, depth + 1);
	const Node* rightBest = findMinRecursive(node->right.get(), targetAxis, depth + 1);

	if (leftBest && leftBest->point[axisIdx] < best->point[axisIdx]) {
		best = leftBest;
	}
	if (rightBest && rightBest->point[axisIdx] < best->point[axisIdx]) {
		best = rightBest;
	}

	return best;
}

const KdTree::Node* KdTree::findMaxRecursive(const Node* node, int targetAxis, int depth) const {
	if (!node) {
		return nullptr;
	}

	const int currentAxis = axisAtDepth(depth);
	const std::size_t axisIdx = static_cast<std::size_t>(targetAxis);

	if (currentAxis == targetAxis) {
		if (!node->right) {
			return node;
		}
		return findMaxRecursive(node->right.get(), targetAxis, depth + 1);
	}

	const Node* best = node;
	const Node* leftBest = findMaxRecursive(node->left.get(), targetAxis, depth + 1);
	const Node* rightBest = findMaxRecursive(node->right.get(), targetAxis, depth + 1);

	if (leftBest && leftBest->point[axisIdx] > best->point[axisIdx]) {
		best = leftBest;
	}
	if (rightBest && rightBest->point[axisIdx] > best->point[axisIdx]) {
		best = rightBest;
	}

	return best;
}

void KdTree::printRecursive(const Node* node, std::ostream& out, const std::string& prefix, bool isLeft, int depth) const {
	if (!node) {
		return;
	}

	if (node->right) {
		printRecursive(node->right.get(), out, prefix + (isLeft ? "    " : "|   "), false, depth + 1);
	}

	out << prefix;
	if (!prefix.empty()) {
		out << (isLeft ? "\\-- " : "/-- ");
	}
	out << '(';
	for (std::size_t i = 0; i < node->point.dimension(); ++i) {
		out << node->point[i];
		if (i + 1 < node->point.dimension()) {
			out << ", ";
		}
	}
	out << ") [axis " << axisAtDepth(depth) << "]\n";

	if (node->left) {
		printRecursive(node->left.get(), out, prefix + (isLeft ? "|   " : "    "), true, depth + 1);
	}
}

void KdTree::rangeQueryRecursive(const Node* node,
								 const Point& minCorner,
								 const Point& maxCorner,
								 int depth,
								 std::vector<Point>& out) const {
	if (!node) {
		return;
	}

	if (isInsideRange(node->point, minCorner, maxCorner)) {
		out.push_back(node->point);
	}

	const int axis = axisAtDepth(depth);
	const double splitValue = node->point[static_cast<std::size_t>(axis)];

	if (minCorner[static_cast<std::size_t>(axis)] <= splitValue) {
		rangeQueryRecursive(node->left.get(), minCorner, maxCorner, depth + 1, out);
	}
	if (maxCorner[static_cast<std::size_t>(axis)] >= splitValue) {
		rangeQueryRecursive(node->right.get(), minCorner, maxCorner, depth + 1, out);
	}
}

void KdTree::nearestNeighborRecursive(const Node* node,
									  const Point& target,
									  int depth,
									  const Node*& best,
									  double& bestDistSquared) const {
	if (!node) {
		return;
	}

	const double nodeDist = squaredDistance(node->point, target);
	if (!best || nodeDist < bestDistSquared) {
		best = node;
		bestDistSquared = nodeDist;
	}

	const int axis = axisAtDepth(depth);
	const std::size_t axisIdx = static_cast<std::size_t>(axis);
	const double delta = target[axisIdx] - node->point[axisIdx];

	const Node* nearChild = delta < 0.0 ? node->left.get() : node->right.get();
	const Node* farChild = delta < 0.0 ? node->right.get() : node->left.get();

	nearestNeighborRecursive(nearChild, target, depth + 1, best, bestDistSquared);
	if ((delta * delta) <= bestDistSquared) {
		nearestNeighborRecursive(farChild, target, depth + 1, best, bestDistSquared);
	}
}

void KdTree::validatePointDimension(const Point& point) const {
	if (point.dimension() != dimensions_) {
		throw std::invalid_argument("Point dimension does not match KdTree dimension");
	}
}

int KdTree::axisAtDepth(int depth) const {
	return depth % static_cast<int>(dimensions_);
}

double KdTree::squaredDistance(const Point& a, const Point& b) {
	double sum = 0.0;
	for (std::size_t i = 0; i < a.dimension(); ++i) {
		const double d = a[i] - b[i];
		sum += d * d;
	}
	return sum;
}

bool KdTree::isInsideRange(const Point& p, const Point& minCorner, const Point& maxCorner) {
	for (std::size_t i = 0; i < p.dimension(); ++i) {
		if (p[i] < minCorner[i] || p[i] > maxCorner[i]) {
			return false;
		}
	}
	return true;
}

} // namespace KDTree
