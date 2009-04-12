#include <cstdio>
#include <cstddef>
#include <cstring>
#include <queue>

using std::queue;

const size_t ALPHA_SIZE = 256;
const size_t LMAX = 1024;

struct Node {
    Node * link[ ALPHA_SIZE ];
    Node * piLink;

    Node * parent;
    unsigned char c;
    
    bool stop;
    
    Node(Node * _parent, char _c) {
      memset(this, 0, sizeof(Node));
      parent = _parent;
      c = _c;
    }
};

class AhoMatcher {
  Node root;

public:
  
  AhoMatcher() : root(NULL, (char)0) { }
  
  void addPattern(char * pattern) {
    Node * node = & root;
    
    for(size_t i = 0; pattern[i]; ++i) {
      const unsigned char c = pattern[i];
      if( NULL == node->link[ c ] )
        node->link[ c ] = new Node(node, c);
      node = node->link[ c ];
    }
    
    node->stop = true;
  }
  
  void build() {
    queue<Node *> q;
    
    root.piLink = &root;
    for(size_t i = 0; i < ALPHA_SIZE; ++i) {
      if( root.link[i] )
        q.push(root.link[i]);
      else
        root.link[i] = &root;
    }
    
    while( ! q.empty() ) {
      Node * n = q.front(); 
      
      const unsigned char c = n->c;
      Node * k = n->parent;
      
      if( &root != k )
        k = k->piLink->link[ c ];
      
      n->piLink = k;
      
      for(size_t i = 0; i < ALPHA_SIZE; ++i) {
        if( n->link[i] )
          q.push(n->link[i]);
        else
          n->link[i] = k->link[i];
      }
      
      q.pop();
    }
  }
  
  bool match(const char * text, size_t & p, size_t & l) {
    const Node * n = & root;
    size_t i;
    
    for(i = 0; ! n->stop && text[i]; ++i)
      n = n->link[ (unsigned char) text[i] ];
    
    if( !n->stop )
      return false;
      
    p = i;
    l = 0;
    for(; n; n = n->parent) {
      --p;
      ++l;
    }
    
    return true;
  }
  
};


int main(size_t argc, char ** argv) {  
  AhoMatcher ahoMatcher;
  
  for(size_t i = 1; i < argc; ++i)
    ahoMatcher.addPattern(argv[i]);
    
  ahoMatcher.build();

  size_t lineCount = 0;
  char text[ LMAX - 1 ];
  size_t p, l;
  
  while( fgets(text, LMAX - 1, stdin) ) {
    ++lineCount;
    if( ahoMatcher.match(text, p, l) )
      fprintf(stdout, "%u: %s", lineCount, text);
  }

  return 0;
}
