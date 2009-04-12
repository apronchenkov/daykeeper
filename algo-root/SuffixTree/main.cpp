#pragma comment(linker, "/STACK:16777216")

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <cassert>

const size_t INF = -1;

static size_t _id;

struct st_node_t {
	st_node_t * parent;
	st_node_t * suffix_link;

	size_t start;
	size_t stop;

	st_node_t ** child;
	char * child_name;
	size_t child_count;
	size_t child_size;

	st_node_t * get_child(char c) const {
		for(size_t i = 0; i < child_count; ++i)
			if( child_name[i] == c )
				return child[i];
		return NULL;
	}

	void set_child(char c, st_node_t * _child) {
		for(size_t i = 0; i < child_count; ++i)
			if( child_name[i] == c ) {
				child[i] = _child;
				return;
			}

		if( child_count == child_size )
			resize_child();

		child[child_count] = _child;
		child_name[child_count] = c;
		++child_count;
	}

	void resize_child() {
		if( 0 == child_size ) 
			child_size = 2;
		else
			child_size *= 2;

		st_node_t ** ch2 = new st_node_t*[child_size];
		char * ch_n2 = new char[child_size];

		memcpy(ch2, child, sizeof(st_node_t*)*child_count);
		memcpy(ch_n2, child_name, sizeof(char)*child_count);

		delete [] child;
		delete [] child_name;

		child = ch2;
		child_name = ch_n2;	
	}

	st_node_t(st_node_t * _parent, size_t _start, size_t _stop) {
		suffix_link = NULL;
		parent = _parent;
		start = _start;
		stop = _stop;
		
		child_size = 0;
		child_count = 0;
		child = NULL;
		child_name = NULL;
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
					//node = node->child[str[i - count]];
					node = node->get_child(str[i - count]);
				}

				if( node->start + count < node->stop ) {

					if( str[i] == str[node->start + count] ) { 
						++count;
						step_over = true;
					
					} else {
						st_node_t * current = new st_node_t(node->parent, node->start, node->start + count);
						//current->child[ str[node->start + count] ] = node;
						current->set_child(str[node->start + count], node);
						//current->parent->child[ str[node->start] ] = current;
						current->parent->set_child(str[node->start], current);
						
						node->parent = current;
						node->start = node->start + count;
						
						st_node_t * child = new st_node_t(current, i, INF);
						//current->child[ str[i] ] = child;
						current->set_child(str[i], child);
						
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

					//if( node->child[ str[i] ] ) {
					if( node->get_child(str[i]) ) {
						//node = node->child[ str[i] ];
						node = node->get_child(str[i]);
						step_over = true;
						count = 1;

					} else {
						st_node_t * current = new st_node_t(node, i, INF);
						//node->child[ str[i] ] = current;
						node->set_child(str[i], current);

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
		str = new char[ strlen(_str) + 1];
		strcpy(str, _str);

		build_tree();
	}
};

char str[256*1024];
size_t N;

size_t OFFSET = 0;
size_t SIZE = 0;
size_t offset;

size_t DFS(st_node_t * node, size_t depth = 0) {
	size_t flag = 0;
	
	for(size_t i = 0; i < node->child_count; ++i) {
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

	DFS(&tree.root);

	for(size_t i = 0; i < SIZE; ++i)
		printf("%c", str[2*N + 1 - OFFSET + i]);
	printf("\n");

	return 0;
}
