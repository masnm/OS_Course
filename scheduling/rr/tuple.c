typedef struct {
	int id, start, end;
} tuple ;

typedef struct {
	size_t size;
	tuple* data;
} tuple_arr ;

tuple_arr tuple_arr_create ()
{
	tuple_arr ta = {
		.size = 0,
		.data = NULL
	};
	return ta;
}

void tuple_arr_push ( tuple_arr* tp, tuple val )
{
	tp->size += 1;
	tp->data = (tuple*) realloc ( tp->data, sizeof(tuple) * tp->size );
	tp->data[tp->size - 1] = val;
}

int tpl_min_by_time ( const void* a, const void* b )
{
	return ((tuple*)a)->start - ((tuple*)b)->start;
}

void tuple_arr_sort ( tuple_arr* tp )
{
	qsort ( tp->data, tp->size, sizeof(tuple), tpl_min_by_time );
}

void tuple_arr_print ( tuple_arr tp )
{
	printf ( "[" );
	for ( int i = 0 ; i < (int)tp.size ; ++i ) {
		if ( i != 0 ) printf ( "," );
		printf ( "{ %d, %d, %d }", tp.data[i].id + 1,
				tp.data[i].start, tp.data[i].end );
	}
	printf ( "]\n" );
}

void tuple_arr_destroy ( tuple_arr* tp )
{
	free ( tp->data );
	tp->size = 0;
	tp->data = NULL;
}
