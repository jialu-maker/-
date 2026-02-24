#include<iostream>
#include<cstddef>
#include<cstring>
using namespace std;
struct FreeNode {
	FreeNode* next;
};
class memoryPool {
private:
	size_t block_size;
	size_t block_count;
	FreeNode* free_list;
	char* pool_buffer;
public:
	memoryPool(size_t block_size, size_t block_count)
		:block_size(block_size), block_count(block_count),
		free_list(nullptr), pool_buffer(nullptr) {
		if (block_size < sizeof(FreeNode))
		{
			block_size = sizeof(FreeNode);
		}
		pool_buffer = new char[block_size * block_count];
		initFreeList();
	}
	~memoryPool() {
		delete[]pool_buffer;
		pool_buffer = nullptr;
		free_list = nullptr;
	}
	memoryPool operator=(const memoryPool&) = delete;
	memoryPool(const memoryPool&) = delete;
	void* allocate() {
		if (free_list == nullptr)
		{
			cout << "内存池已满" << endl;
			return nullptr;

		}
		FreeNode* node = free_list;
		free_list = free_list->next;
		return node;
	}
	void deallocate(void* ptr)
	{
		if (ptr == nullptr) return;
		if (ptr < pool_buffer || ptr >= pool_buffer + block_size * block_count)
		{
			cout << "释放地址无效，非内存池内存" << endl;
			return;
		}
		FreeNode* node = static_cast<FreeNode*>(ptr);
		node->next = free_list;
		free_list = node;
	}
private:
	void initFreeList()
	{
		free_list = nullptr;
		for (size_t i = 0; i < block_count; i++)
		{
			char* addr = pool_buffer + i * block_size;
			FreeNode* node = reinterpret_cast<FreeNode*>(addr);
			node->next = free_list;
			free_list = node;
		}
		cout << "内存块大小初始化完成：块大小=" << block_size << " 字节，总块数=" << block_count << endl;
	};
};
	class Testobj {
	public:
		int a;
		double b;
		Testobj(int a_ = 0, double b_ = 0) : a(a_), b(b_) {
			cout << "Testobj构造：" << this << endl;
		}
		~Testobj() {
			cout << "Testobj析构：" << this << endl;
		}
	};
	int main()
	{
		memoryPool pool(sizeof(Testobj), 5);
		Testobj* p1 = new(pool.allocate())Testobj(10, 3.14);
		Testobj* p2 = new(pool.allocate())Testobj(20, 6.28);
		Testobj* p3 = new(pool.allocate())Testobj(30, 9.42);
		cout << "p1:" << p1 << "a=" << p1->a << "b=" << p1->b << endl;
		cout << "p2:" << p2 << "a=" << p2->a << "b=" << p2->b << endl;
		cout << "p3:" << p3 << "a=" << p3->a << "b=" << p3->b << endl;
		p2->~Testobj();
		pool.deallocate(p2);
		Testobj* p4 = new(pool.allocate())Testobj(40, 12.56);
		cout << "p4(复用p2内存):" << p4 << "a=" << p4->a << "b=" << p4->b << endl;
		Testobj* p5 = new(pool.allocate())Testobj(50, 15.7);
		Testobj* p6 = new(pool.allocate())Testobj(60, 18.84);
		p1->~Testobj(); pool.deallocate(p1);
		p4->~Testobj(); pool.deallocate(p4);
		p3->~Testobj(); pool.deallocate(p3);
		p5->~Testobj(); pool.deallocate(p5);
		cout << "测试结束，内存池析构时自动释放总缓冲区" << endl;
		return 0;
	}