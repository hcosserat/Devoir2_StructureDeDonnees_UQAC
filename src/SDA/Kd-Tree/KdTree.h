#pragma once

#include <cstddef>
#include <initializer_list>
#include <iosfwd>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace KDTree {

struct Point {
	std::vector<double> values;

	Point() = default;
	explicit Point(std::vector<double> coords) : values(std::move(coords)) {}
	Point(std::initializer_list<double> coords) : values(coords) {}

	std::size_t dimension() const {
		return values.size();
	}

	double operator[](std::size_t idx) const {
		return values[idx];
	}

	double& operator[](std::size_t idx) {
		return values[idx];
	}

	bool operator==(const Point& other) const {
		return values == other.values;
	}
};

class KdTree {
public:
	explicit KdTree(std::size_t dimensions) : dimensions_(dimensions) {
		if (dimensions_ == 0) {
			throw std::invalid_argument("KdTree dimensions must be greater than 0");
		}
	}
	~KdTree() = default;

	KdTree(const KdTree&) = delete;
	KdTree& operator=(const KdTree&) = delete;
	KdTree(KdTree&& other) noexcept
		: root_(std::move(other.root_)), count_(other.count_), dimensions_(other.dimensions_) {
		other.count_ = 0;
	}
	KdTree& operator=(KdTree&& other) noexcept {
		if (this != &other) {
			root_ = std::move(other.root_);
			count_ = other.count_;
			dimensions_ = other.dimensions_;
			other.count_ = 0;
		}
		return *this;
	}

	void clear();
	bool empty() const;
	std::size_t size() const;
	std::size_t dimensions() const;

	void insert(const Point& point);
	bool remove(const Point& point);
	bool contains(const Point& point) const;

	std::vector<Point> rangeQuery(const Point& minCorner, const Point& maxCorner) const;
	Point nearestNeighbor(const Point& target) const;
	void print(std::ostream& out) const;

private:
	struct Node {
		Point point;
		std::unique_ptr<Node> left;
		std::unique_ptr<Node> right;

		explicit Node(const Point& p) : point(p) {}
	};

	std::unique_ptr<Node> root_;
	std::size_t count_{0};
	std::size_t dimensions_;

	void insertRecursive(std::unique_ptr<Node>& node, const Point& point, int depth);
	bool containsRecursive(const Node* node, const Point& point, int depth) const;
	bool removeRecursive(std::unique_ptr<Node>& node, const Point& point, int depth);
	const Node* findMinRecursive(const Node* node, int targetAxis, int depth) const;
	const Node* findMaxRecursive(const Node* node, int targetAxis, int depth) const;
	void printRecursive(const Node* node, std::ostream& out, const std::string& prefix, bool isLeft, int depth) const;
	void rangeQueryRecursive(const Node* node,
							 const Point& minCorner,
							 const Point& maxCorner,
							 int depth,
							 std::vector<Point>& out) const;
	void nearestNeighborRecursive(const Node* node,
								  const Point& target,
								  int depth,
								  const Node*& best,
								  double& bestDistSquared) const;
	void validatePointDimension(const Point& point) const;

	int axisAtDepth(int depth) const;
	static double squaredDistance(const Point& a, const Point& b);
	static bool isInsideRange(const Point& p, const Point& minCorner, const Point& maxCorner);
};

} // namespace KDTree

