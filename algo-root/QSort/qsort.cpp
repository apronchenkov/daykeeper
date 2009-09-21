void qsort(int * const begin, int * const end) {
  const size_t size = end - begin;
  if( size < 2 )
    return;

  const int x = begin[rand() % size];

  int * i = begin, * j = end;

  while( i < j ) {
    while( *i < x ) { ++i; }
    do { --j; } while( *j > x );

    if( *i < *j )
      swap(*i, *j);

    ++i;
  }

  if( begin < j )
    qsort(begin, j);

  if( i < end )
    qsort(i, end);
}