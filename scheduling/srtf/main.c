/*
 * Author : Ahmmed Sakib Noman
 * Shortest Remaining Time First
 * Criteria : Arival Time
 * Mode : Preemptive
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

typedef struct {
	int first, second;
} pair ;

typedef struct {
	int id, arival_time, burst_time, completion_time,
		turn_around_time, waiting_time, responce_time;
	size_t pair_size;
	pair* pairs;
} process ;

typedef struct {
	size_t size;
	process* processes;
} process_arr ;

typedef struct {
	size_t size;
	int* data;
} int_arr ;

int min_by_arival ( const void* a, const void* b )
{
	if ( ((process*)a)->arival_time == ((process*)b)->arival_time )
		return ((process*)a)->burst_time - ((process*)b)->burst_time; 
	else
		return ((process*)a)->arival_time - ((process*)b)->arival_time;
}

int min_by_id ( const void* a, const void* b )
{
	return ((process*)a)->id - ((process*)b)->id;
}

int min_by_burst ( const void* a, const void* b )
{
	if ( ((process*)a)->burst_time == ((process*)b)->burst_time )
		return ((process*)a)->arival_time - ((process*)b)->arival_time; 
	else
		return ((process*)a)->burst_time - ((process*)b)->burst_time;
}

process_arr get_process_array ()
{
	process_arr pa;
	printf ( "Enter the number of processes : " );
	scanf ( "%zd", &pa.size );
	pa.processes = (process*) malloc ( sizeof(process) * pa.size );
	for ( int i = 0 ; i < (int)pa.size ; ++i ) {
		printf ( "Enter process %d arival and burst time : ", i+1 );
		scanf ( "%d%d", &pa.processes[i].arival_time, &pa.processes[i].burst_time );
		pa.processes[i].id = i + 1;
		pa.processes[i].completion_time = 0;
		pa.processes[i].turn_around_time = 0;
		pa.processes[i].waiting_time = 0;
		pa.processes[i].responce_time = 0;
		pa.processes[i].pair_size = 0;
		pa.processes[i].pairs = NULL;
	}
	return pa;
}

void calculate_completion_time ( process_arr pa )
{
	for ( int i = 0 ; i < (int)pa.size ; ++i ) {
		pa.processes[i].completion_time = pa.processes[i].pairs[pa.processes[i].pair_size-1].second;
	}
}

void calculate_turn_around_time ( process_arr pa )
{
	for ( int i = 0 ; i < (int)pa.size ; ++i ) {
		pa.processes[i].turn_around_time = pa.processes[i].completion_time - pa.processes[i].arival_time;
	}
}

void calculate_waiting_time ( process_arr pa )
{
	for ( int i = 0 ; i < (int)pa.size ; ++i ) {
		pa.processes[i].waiting_time = pa.processes[i].turn_around_time - pa.processes[i].burst_time;
	}
}

void calcuate_responce_time ( process_arr pa )
{
	for ( int i = 0 ; i < (int)pa.size ; ++i ) {
		pa.processes[i].responce_time = pa.processes[i].pairs[0].first - pa.processes[i].arival_time;
		// pa.processes[i].responce_time = pa.processes[i].completion_time - pa.processes[i].burst_time - pa.processes[i].arival_time;
	}
}

float calculate_average_waiting_time ( process_arr pa )
{
	int total = 0;
	for ( int i = 0 ; i < (int)pa.size ; ++i ) {
		total += pa.processes[i].waiting_time;
	}
	return (float)total / (float)pa.size;
}

float calculate_average_responce_time ( process_arr pa )
{
	int total = 0;
	for ( int i = 0 ; i < (int)pa.size ; ++i ) {
		total += pa.processes[i].responce_time;
	}
	return (float)total / (float)pa.size;
}

void print_process_arr ( process_arr pa )
{
	printf ( "\nid\tarival\tburst\tcomplet\ttat\twt\tresponce\n" );
	for ( int i = 0 ; i < (int)pa.size ; ++i ) {
		printf ( "%d\t%d\t%d\t%d\t%d\t%d\t%d\t", pa.processes[i].id, pa.processes[i].arival_time,
				pa.processes[i].burst_time, pa.processes[i].completion_time,
				pa.processes[i].turn_around_time, pa.processes[i].waiting_time,
				pa.processes[i].responce_time );
		printf ( "[" );
		for ( int j = 0 ; j < (int)pa.processes[i].pair_size ; ++j ) {
			if ( j != 0 ) printf ( ", " );
			printf ( "{ %d, %d }", pa.processes[i].pairs[j].first, pa.processes[i].pairs[j].second );
		}
		printf ( "]\n" );
	}
}

void sequence_add ( int_arr* ia, int id )
{
	ia->size += 1;
	ia->data = (int*) realloc ( ia->data, sizeof(int) * ia->size );
	ia->data[ia->size-1] = id;
}

void remove_begeining ( process_arr* pa )
{
	if ( pa->processes[0].burst_time <= 0 ) {
		for ( int i = 0 ; i < (int)pa->size - 1 ; ++i )
			pa->processes[i] = pa->processes[i+1];
		pa->size -= 1;
	}
}

void update_process_times ( int* ar, int n, process_arr* pa )
{
	int start = 0, end = 0, id = 0;
	while ( start < n ) {
		id = ar[start];
		end = start + 1;
		while ( end < n && ar[end] == id )
			++end;
		end -= 1;
		if ( id == -1 ) {
			start = end + 1;
			continue;
		}
		pa->processes[id-1].pair_size += 1;
		pa->processes[id-1].pairs = (pair*)
			realloc ( pa->processes[id-1].pairs,
			sizeof(pair) * pa->processes[id-1].pair_size );
		pa->processes[id-1].pairs[pa->processes[id-1].pair_size-1].first = start;
		pa->processes[id-1].pairs[pa->processes[id-1].pair_size-1].second = end + 1;
		start = end + 1;
	}
}

void calculate_sequence ( process_arr* pa )
{
	int_arr ret;
	ret.size = 0;
	ret.data = (int*) malloc ( sizeof(int) * ret.size );
	process_arr npa;
	npa.size = pa->size;
	npa.processes = (process*) malloc ( sizeof(process) * npa.size );

	for ( int i = 0 ; i < (int)npa.size ; ++i )
		npa.processes[i] = pa->processes[i];

	int current_time = 0;
	while ( pa->size != 0 ) {
		if ( pa->processes[0].arival_time <= current_time ) {
			sequence_add ( &ret, pa->processes[0].id );
			pa->processes[0].burst_time -= 1;
		} else
			sequence_add ( &ret, -1 );
		remove_begeining ( pa );
		++current_time;
		int mx_ind = 0;
		while ( (mx_ind < pa->size) &&
				(pa->processes[mx_ind].arival_time <= current_time) )
			++mx_ind;
		qsort ( pa->processes, mx_ind, sizeof(process), min_by_burst );
	}
	for ( int i = 0 ; i < (int)npa.size ; ++i )
		pa->processes[i] = npa.processes[i];
	free ( npa.processes );
	pa->size = npa.size;
	qsort ( pa->processes, pa->size, sizeof(process), min_by_id );
	update_process_times ( ret.data, ret.size, &(*pa) );
	free ( ret.data );
}

int main ()
{
	process_arr process_array = get_process_array();

	qsort ( process_array.processes, process_array.size, sizeof(process), min_by_arival );

	calculate_sequence ( &process_array );

	calculate_completion_time ( process_array );
	calculate_turn_around_time ( process_array );
	calculate_waiting_time ( process_array );
	calcuate_responce_time ( process_array );

	float average_waiting_time = calculate_average_waiting_time ( process_array );
	float average_responce_time = calculate_average_responce_time ( process_array );

	qsort ( process_array.processes, process_array.size, sizeof(process), min_by_id );

	print_process_arr ( process_array );
	printf ( "Average waiting time : %.2f\n", average_waiting_time );
	printf ( "Average responce time : %.2f\n", average_responce_time );

	for ( int i = 0 ; i < (int)process_array.size ; ++i ) {
		free ( process_array.processes[i].pairs );
	}
	free ( process_array.processes );

	return 0;
}

// TODO: find a way to print grant chart
