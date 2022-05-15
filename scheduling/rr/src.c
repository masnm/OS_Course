/*
 * author : Ahmmed Sakib Noman
 * algo : Round Robin
 * Criteria : Time Quantum
 * Mode : Preemptive
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>

#include "tuple.c"
#include "process.c"
#include "queue.c"

int min_by_arival ( const void* a, const void* b )
{
	return ((process*)a)->arival_time - ((process*)b)->arival_time;
}

int min_by_id ( const void* a, const void* b )
{
	return ((process*)a)->id - ((process*)b)->id;
}

void get_process_into_queue ( queue* qu, process_arr* pa, int current_time )
{
	qsort ( pa->data, pa->size, sizeof(process), min_by_arival );
	for ( int i = 0 ; i < (int)pa->size ; ++i ) {
		if ( pa->data[i].burst_time > 0 &&
				pa->data[i].arival_time <= current_time &&
				!queue_available ( qu, pa->data[i].id ) )
			queue_push ( qu, pa->data[i].id );
	}
	qsort ( pa->data, pa->size, sizeof(process), min_by_id );
}

int min ( int a, int b ) { return ( a < b ? a : b ); }

int get_waiting_time ( process_arr* pa, int current_time )
{
	int min_time = INT_MAX;
	for ( int i = 0 ; i < (int)pa->size ; ++i )
		if ( pa->data[i].arival_time > current_time )
			min_time = min( min_time, pa->data[i].arival_time - current_time );
	assert ( min_time != INT_MAX );
	return min_time;
}

void calculate_sequence ( process_arr* pa )
{
	process_arr bkp = process_arr_get_copy ( pa );
	int current_time = 0;

	queue qu = queue_create ();
	get_process_into_queue ( &qu, pa, current_time );

	while ( !queue_empty ( &qu ) || process_arr_waiting( pa, current_time ) ) {
		if ( queue_empty ( &qu ) ) {
			int wait_time = get_waiting_time ( pa, current_time );
			current_time += wait_time;
			get_process_into_queue ( &qu, pa, current_time );
			continue;
		}
		int p_id, p_start, p_end;
		p_id = queue_front ( &qu );
		p_start = current_time;

		int mmin = min ( pa->time_quantum, pa->data[queue_front ( &qu )].burst_time );
		pa->data[queue_front ( &qu )].burst_time -= mmin;
		current_time += mmin;
		p_end = mmin;

		save_allocated_time ( pa, p_id, p_start, p_start + p_end );

		get_process_into_queue ( &qu, pa, current_time );

		int front = queue_front ( &qu );
		queue_pop ( &qu );
		if ( pa->data[front].burst_time > 0 )
			queue_push ( &qu, front );
	}

	queue_destroy ( &qu );
	process_arr_restore ( pa, &bkp );
	destroy_process_arr ( &bkp );
}

void print_gantt_chart ( process_arr pa )
{
	tuple_arr ta = tuple_arr_create ();
	for ( int i = 0 ; i < (int)pa.size ; ++i ) {
		for ( int j = 0 ; j < (int)pa.data[i].p_size ; ++j ) {
			tuple tpl = {
				.id = pa.data[i].id,
				.start = pa.data[i].pairs[j].start,
				.end = pa.data[i].pairs[j].end
			};
			tuple_arr_push ( &ta, tpl );
		}
	}
	tuple_arr_sort ( &ta );
	tuple_arr_print ( ta );
	tuple_arr_destroy ( &ta );
}

void calculate_completion_times ( process_arr* pa )
{
	for ( int i = 0 ; i < (int)pa->size ; ++i ) {
		pa->data[i].completion_time =
			pa->data[i].pairs[pa->data[i].p_size-1].end;
	}
}

void calculate_turnaround_times ( process_arr* pa )
{
	for ( int i = 0 ; i < (int)pa->size ; ++i ) {
		pa->data[i].turnaround_time =
			pa->data[i].completion_time - pa->data[i].arival_time;
	}
}

void calculate_waiting_times ( process_arr* pa )
{
	for ( int i = 0 ; i < (int)pa->size ; ++i ) {
		pa->data[i].waiting_time =
			pa->data[i].turnaround_time - pa->data[i].burst_time;
	}
}

void calculate_responce_times ( process_arr* pa )
{
	for ( int i = 0 ; i < (int)pa->size ; ++i ) {
		pa->data[i].responce_time =
			pa->data[i].pairs[0].start - pa->data[i].arival_time;
	}
}

int main ()
{
	process_arr process_array = process_arr_create ();
	process_arr_get_input ( &process_array );

	calculate_sequence ( &process_array );

	calculate_completion_times ( &process_array );
	calculate_turnaround_times ( &process_array );
	calculate_waiting_times ( &process_array );
	calculate_responce_times ( &process_array );

	print_process_arr ( process_array );
	print_gantt_chart ( process_array );

	destroy_process_arr ( &process_array );

	return 0;
}

