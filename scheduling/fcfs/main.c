/*
 * Author : Ahmmed Sakib Noman
 * First Come First serve
 * Criteria : Arival time
 * Mode : Non-preemptive
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

typedef struct {
	int id, arival_time, burst_time, completion_time,
		turn_around_time, waiting_time, responce_time;
} process ;

typedef struct {
	size_t size;
	process* processes;
} process_arr ;

int min_by_arival ( const void* a, const void* b )
{
	return ((process*)a)->arival_time - ((process*)b)->arival_time;
}

int min_by_id ( const void* a, const void* b )
{
	return ((process*)a)->id - ((process*)b)->id;
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
	}
	return pa;
}

void calculate_completion_time ( process_arr pa )
{
	int total_time = 0;
	for ( int i = 0 ; i < (int)pa.size ; ++i ) {
		total_time += pa.processes[i].burst_time;
		pa.processes[i].completion_time += total_time;
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
		pa.processes[i].responce_time = pa.processes[i].completion_time - pa.processes[i].burst_time - pa.processes[i].arival_time;
	}
}

int calculate_average_waiting_time ( process_arr pa )
{
	int total = 0;
	for ( int i = 0 ; i < (int)pa.size ; ++i ) {
		total += pa.processes[i].waiting_time;
	}
	return total / (int)pa.size;
}

int calculate_average_responce_time ( process_arr pa )
{
	int total = 0;
	for ( int i = 0 ; i < (int)pa.size ; ++i ) {
		total += pa.processes[i].responce_time;
	}
	return total / (int)pa.size;
}

void print_process_arr ( process_arr pa )
{
	printf ( "\nid\tarival\tburst\tcomplet\ttat\twt\tresponce\n" );
	for ( int i = 0 ; i < (int)pa.size ; ++i ) {
		printf ( "%d\t%d\t%d\t%d\t%d\t%d\t%d\n", pa.processes[i].id, pa.processes[i].arival_time,
				pa.processes[i].burst_time, pa.processes[i].completion_time,
				pa.processes[i].turn_around_time, pa.processes[i].waiting_time,
				pa.processes[i].responce_time );
	}
}

int main ()
{
	process_arr process_array = get_process_array();

	qsort ( process_array.processes, process_array.size, sizeof(process), min_by_arival );

	calculate_completion_time ( process_array );
	calculate_turn_around_time ( process_array );
	calculate_waiting_time ( process_array );
	calcuate_responce_time ( process_array );

	int average_waiting_time = calculate_average_waiting_time ( process_array );
	int average_responce_time = calculate_average_responce_time ( process_array );

	qsort ( process_array.processes, process_array.size, sizeof(process), min_by_id );

	print_process_arr ( process_array );
	printf ( "Average waiting time : %d\n", average_waiting_time );
	printf ( "Average responce time : %d\n", average_responce_time );

	free ( process_array.processes );

	return 0;
}

// TODO: find a way to print grant chart
