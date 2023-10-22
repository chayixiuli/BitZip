#pragma once
#include<vector>
#include<queue>
using std::vector;

//Huffman编码压缩过程
//1.统计源文件中每个字节出现的次数
//2.字节频次 来创建Huffman树
//3.获取每个字节的Huffman编码
//4.用得到的编码对源文件中的每个字节进行改写 

template<class W>
struct HuffmanTreeNode {
	HuffmanTreeNode<W>* _left;
	HuffmanTreeNode<W>* _right;
	HuffmanTreeNode<W>* _parent;
	W _weight;
	HuffmanTreeNode(const W& weight = W()) 
		:_left(nullptr)
		,_right(nullptr)
		,_parent(nullptr)
		,_weight(weight)
	{}
};


template<class W>
class HuffmanTree
{
	typedef HuffmanTreeNode<W> Node;
	class Compare {
	public:
		bool operator()(const Node* left, const Node* right)const {
			return left->_weight > right->_weight;
		}
	};
public:
	HuffmanTree()
		:_root(nullptr)
	{}

	HuffmanTree(const std::vector<W>& vw,const W& valid = W()) {
		// 1.用所有的权值构造只有根节点的二叉树森林
		// 森林中二叉树应该使用堆(优先级队列)来保存

		// 小堆 -- 优先级队列默认是大堆
		std::priority_queue<Node*, vector<Node*>, Compare> q;
		for (auto& e : vw) {
			if (valid != e)
				q.push(new Node(e));
		}

		while (q.size() > 1) {
			Node* left = q.top();
			q.pop();

			Node* right = q.top();
			q.pop();

			// 将left和right作为一个新节点的左右子树
			Node* parent = new Node(left->_weight+right->_weight);
			parent->_left = left;
			parent->_right = right;

			left->_parent = right->_parent = parent;
			// 最后将parent放入到二叉树森林中
			q.push(parent);

		}
		_root = q.top();
	}
	~HuffmanTree()
	{
		Destory(_root);
	}

	Node* GetRoot() {
		return _root;
	}

private:
	void Destory(Node*& root) {
		if (root) {
			Destory(root->_left);
			Destory(root->_right);
			delete root;
			root = nullptr;
		}
	}

	Node* _root;

};
//void TestHuffmanTree() {
//
//	std::vector<int> v{ 7,5,3,1 };
//	HuffmanTree<int> ht(v);
//}