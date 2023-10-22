#pragma once
#include<vector>
#include<queue>
using std::vector;

//Huffman����ѹ������
//1.ͳ��Դ�ļ���ÿ���ֽڳ��ֵĴ���
//2.�ֽ�Ƶ�� ������Huffman��
//3.��ȡÿ���ֽڵ�Huffman����
//4.�õõ��ı����Դ�ļ��е�ÿ���ֽڽ��и�д 

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
		// 1.�����е�Ȩֵ����ֻ�и��ڵ�Ķ�����ɭ��
		// ɭ���ж�����Ӧ��ʹ�ö�(���ȼ�����)������

		// С�� -- ���ȼ�����Ĭ���Ǵ��
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

			// ��left��right��Ϊһ���½ڵ����������
			Node* parent = new Node(left->_weight+right->_weight);
			parent->_left = left;
			parent->_right = right;

			left->_parent = right->_parent = parent;
			// ���parent���뵽������ɭ����
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