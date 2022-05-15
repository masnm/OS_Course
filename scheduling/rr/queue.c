typedef struct {
	size_t size;
	int* data;
} queue ;

queue queue_create ()
{
	queue qu = {
		.size = 0,
		.data = NULL
	};
	return qu;
}

bool queue_empty ( queue* qu )
{
	return ( qu->size == 0 );
}

void queue_push ( queue* qu, int val )
{
	qu -> size += 1;
	qu->data = (int*) realloc ( qu->data, sizeof(int) * qu->size );
	qu->data[qu->size - 1] = val;
}

int queue_front ( queue* qu )
{
	return qu->data[0];
}

int queue_pop ( queue* qu )
{
	int ret = qu->data[0];
	qu->size -= 1;
	for ( int i = 0 ; i < (int)qu->size ; ++i )
		qu->data[i] = qu->data[i + 1];
	qu->data = (int*) realloc ( qu->data, sizeof(int) * qu->size );
	return ret;
}

bool queue_available ( queue* qu, int val )
{
	for ( int i = 0 ; i < (int)qu->size ; ++i )
		if ( qu->data[i] == val )
			return true;
	return false;
}

void queue_destroy ( queue* qu )
{
	qu->size = 0;
	free ( qu->data );
	qu->data = NULL;
}
