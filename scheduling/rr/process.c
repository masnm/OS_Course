typedef struct {
	int start, end;
} pair ;

typedef struct {
	int id, arival_time, burst_time, completion_time,
		turnaround_time, waiting_time, responce_time;
	size_t p_size;
	pair* pairs;
} process ;

typedef struct {
	size_t size, time_quantum;
	process* data;
} process_arr ;

process_arr process_arr_create ()
{
	process_arr pa = {
		.size = 0,
		.time_quantum = 0,
		.data = NULL
	};
	return pa;
}

void process_arr_get_input ( process_arr* pa )
{
	printf ( "Enter number of processes : " );
	scanf ( "%zd", &pa->size );
	printf ( "Enter time quantum limit : " );
	scanf ( "%zd", &pa->time_quantum );
	pa->data = (process*) malloc ( sizeof(process) * pa->size );
	for ( int i = 0 ; i < (int)pa->size ; ++i ) {
		pa->data[i].id = i;
		pa->data[i].p_size = 0;
		pa->data[i].pairs = NULL;
		printf ( "Enter arival and burst time of process %d : ", i + 1 );
		scanf ( "%d%d", &pa->data[i].arival_time, &pa->data[i].burst_time );
		pa->data[i].completion_time = 0;
		pa->data[i].turnaround_time = 0;
		pa->data[i].waiting_time = 0;
		pa->data[i].responce_time = 0;
	}
}

process_arr process_arr_get_copy ( process_arr* pa )
{
	process_arr cpy = {
		.size = pa->size,
		.time_quantum = pa->time_quantum,
		.data = NULL
	};
	cpy.data = (process*) malloc ( sizeof(process) * cpy.size );
	for ( int i = 0 ; i < (int)cpy.size ; ++i )
		cpy.data[i] = pa->data[i];
	return cpy;
}

void process_arr_restore ( process_arr* pa, process_arr* cpy )
{
	pa->size = cpy->size;
	pa->time_quantum = cpy->time_quantum;
	for ( int i = 0 ; i < (int)pa->size ; ++i ) {
		pair* prs = pa->data[i].pairs;
		size_t psz = pa->data[i].p_size;
		pa->data[i] = cpy->data[i];
		pa->data[i].pairs = prs;
		pa->data[i].p_size = psz;
	}
}

bool process_arr_waiting ( process_arr* pa, int current_time )
{
	for ( int i = 0 ; i < (int)pa->size ; ++i )
		if ( pa->data[i].arival_time > current_time )
			return true;
	return false;
}

void save_allocated_time ( process_arr* pa, int p_id, int p_start, int p_end )
{
	assert ( p_id > -1 && p_id < (int)pa->size );
	process* p = pa->data + p_id;
	p->p_size += 1;
	p->pairs = (pair*) realloc ( p->pairs, sizeof(pair) * p->p_size );
	p->pairs[p->p_size - 1].start = p_start;
	p->pairs[p->p_size - 1].end = p_end;
}

void print_process_arr ( process_arr pa )
{
	printf ( "Time Quantum is : %zd\n", pa.time_quantum );
	printf ( "id\tarival\tburst\tcomplet\tTurnAro\tWait\tRespo\n" );
	for ( int i = 0 ; i < (int)pa.size ; ++i ) {
		printf ( "%d\t%d\t%d\t%d\t%d\t%d\t%d\n", pa.data[i].id + 1,
				pa.data[i].arival_time, pa.data[i].burst_time,
				pa.data[i].completion_time, pa.data[i].turnaround_time,
				pa.data[i].waiting_time, pa.data[i].responce_time );
		// printf ( "[" );
		// for ( int j = 0 ; j < (int)pa.data[i].p_size ; ++j ) {
		// 	if ( j != 0 ) printf ( ", " );
		// 	printf ( "{ %d, %d }", pa.data[i].pairs[j].start,
		// 			pa.data[i].pairs[j].end );
		// }
		// printf ( "]\n" );
	}
}

void destroy_process_arr ( process_arr* pa )
{
	for ( int i = 0 ; i < (int)pa->size ; ++i ) {
		free ( pa->data[i].pairs );
		pa->data[i].p_size = 0;
	}
	free ( pa->data );
	pa->size = 0;
}
