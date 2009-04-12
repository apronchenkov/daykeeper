#include <cstddef>
#include <cstdio>
#include <cstring>
#include <cassert>

#include <string>
#include <iostream>
#include <iomanip>

using std::cin;
using std::cout;
using std::string;
using std::endl;


const size_t INF = -1;

static size_t _id;

struct st_node_t {
	st_node_t * child[32];
	st_node_t * parent;
	st_node_t * suffix_link;

	size_t start;
	size_t stop;
	size_t id;

	st_node_t() : id(_id++), suffix_link(NULL) { 
		memset(child, 0, sizeof(child));
	}

	st_node_t(st_node_t * _parent, size_t _start, size_t _stop) : id(_id++), suffix_link(NULL), parent(_parent), start(_start), stop(_stop) {
		memset(child, 0, sizeof(child));
	}
};

class st_t {
public:	
	char * str;

	st_node_t root;

	void build_tree() {
		size_t count = 0;
		st_node_t ** slink = NULL; 
		st_node_t * node = & root;
		
		for(size_t i = 0; str[i]; ++i) {

			for(bool step_over = false; !step_over; ) {

				while( node->start + count > node->stop ) {
					count -= node->stop - node->start;
					node = node->child[str[i - count]];
				}

				if( node->start + count < node->stop ) {

					if( str[i] == str[node->start + count] ) { 
						++count;
						step_over = true;
					
					} else {
						st_node_t * current = new st_node_t(node->parent, node->start, node->start + count);
						current->child[ str[node->start + count] ] = node;
						current->parent->child[ str[node->start] ] = current;
						
						node->parent = current;
						node->start = node->start + count;
						
						st_node_t * child = new st_node_t(current, i, INF);
						current->child[ str[i] ] = child;
						
						if( slink )
							*slink = current;
						slink = &current->suffix_link;
						
						node = current->parent;

						if( &root == node ) {
							if( count )
								--count;
							else
								step_over = true;
						} else {
							node = node->suffix_link;
							count += node->stop - node->start;
						}
					}

				} else {
					if( slink )
						*slink = node;
					slink = NULL;

					if( node->child[ str[i] ] ) {
						node = node->child[ str[i] ];
						step_over = true;
						count = 1;

					} else {
						st_node_t * current = new st_node_t(node, i, INF);
						node->child[ str[i] ] = current;

						if( &root == node ) {
							if( count )
								--count;
							else
								step_over = true;
						} else {
							node = node->suffix_link;
							count = node->stop - node->start;
						}
					}
				}
			}
		}
	}
	
public:

	st_t(const char * _str) : root(NULL, 0, 0) {
		str = new char[ strlen(_str) ];
		for(size_t i = 0; _str[i]; ++i)
			str[i] = _str[i] - '@';

		build_tree();
	}


	void print(st_node_t * node = NULL) {
		if( !node ) node = &root;

		cout << "node " << node->id << endl;

		for(size_t i = 0; i < 32; ++i)
			if( node->child[i] ) {
				cout << "  -> node " << node->child[i]->id << " : ";
				for(size_t j = node->child[i]->start; str[j] && j < node->child[i]->stop; ++j)
					cout << str[j];
				cout << "(" << node->child[i]->start << ", ";
				if( INF == node->child[i]->stop )
					cout << "inf)\n";
				else
					cout << node->child[i]->stop << ")\n";
			}

		for(size_t i = 0; i < 256; ++i)
			if( node->child[i] )
				print(node->child[i]);
	}

};

char str[256*1024];
size_t N;

size_t OFFSET = 0;
size_t SIZE = 0;
size_t offset;

size_t DFS(st_node_t * node, size_t depth = 0) {
	size_t flag = 0;
	
	for(size_t i = 0; i < 32; ++i)
		if( node->child[i] ) {
			size_t new_depth = depth;
			if( INF != node->child[i]->stop )
				new_depth += node->child[i]->stop - node->child[i]->start;
			else
				new_depth += 2*N + 1 - node->child[i]->start;
			flag |= DFS(node->child[i], new_depth);
		}

	if( 0 == flag ) {
		offset = depth;
		return (depth <= N + 1) ? 1 : 2;
	}

	if( 3 == flag && depth > SIZE ) {
		SIZE = depth;
		OFFSET = offset;
	}
	
	return flag;
}

int main() {
	scanf("%u", &N);
	scanf("%s%s", str, str + N + 1);
	str[N] = '[';
	str[N + 1 + N] = ']';

	st_t tree(str);
//	tree.print();

	DFS(&tree.root);

	for(size_t i = 0; i < SIZE; ++i)
		printf("%c", str[2*N + 1 - OFFSET + i]);
	printf("\n");

	return 0;
}
