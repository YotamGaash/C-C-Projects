//
// Created by Yotam Gaash on 31/01/2024.
//

#include "markov_chain.h"
#include "linked_list.h"

// Free function for MarkovNodeFrequency
void free_frequency_list(MarkovNodeFrequency **frequency_list,
						 int size)
{
	for (int i = 0; i < size; i++)
	{
		if (frequency_list[i])
		{
			if (frequency_list[i]->markov_node)
			{
				free(frequency_list[i]->markov_node);
			}
	free(frequency_list[i]);
		}
	}
	if(*frequency_list != NULL){
		free(frequency_list);
	}
}

// Make function for MarkovNodeFrequency
MarkovNodeFrequency *
make_markov_node_frequency(MarkovNode *markov_node, int frequency)
{
	MarkovNodeFrequency *new_frequency = (MarkovNodeFrequency *) malloc(
			sizeof(MarkovNodeFrequency));
	if (!new_frequency)
	{
		// Allocation failure
		fprintf(stderr, ALLOCATION_ERROR_MASSAGE);
		return NULL;
	}
	new_frequency->markov_node = markov_node;
	new_frequency->frequency = frequency;

	return new_frequency;
}

// Free function for MarkovNode
void free_markov_node(MarkovNode *markov_node)
{

	free_frequency_list(markov_node->frequency_list,
						markov_node->frequency_list_size);
	free(markov_node);
}

// Make function for MarkovNode
MarkovNode *make_markov_node(char data[MAX_WORD_LENGTH])
{
	if (!data)
	{
		return NULL; //does not create a node if the data ptr is invalid
	}
	MarkovNode *new_node = (MarkovNode *) malloc(sizeof(MarkovNode));
	if (!new_node)
	{
		// Allocation failure
		fprintf(stderr, ALLOCATION_ERROR_MASSAGE);
		return NULL;
	}
	if (!strcpy(new_node->data, data))
	{ //error copying the data
		free_markov_node(new_node);
		fprintf(stderr, ALLOCATION_ERROR_MASSAGE);
		return NULL;
	}
	new_node->end_with_dot = new_node->data[strlen(data) - 1] == DOT;
	new_node->frequency_list = NULL;
	new_node->frequency_list_size = 0;
	return new_node;
}

//make markov chain structure
MarkovChain *make_markov_chain()
{
	MarkovChain *new_markov_chain = (MarkovChain *)
			malloc(sizeof(MarkovChain));
	if (!new_markov_chain)
	{
		//Allocation Failure
		return NULL;
	}
	new_markov_chain->database = (LinkedList *)
			malloc(sizeof(LinkedList));
	if (!new_markov_chain->database)
	{
		fprintf(stderr, ALLOCATION_ERROR_MASSAGE);
		free(new_markov_chain);
		return NULL;
	}
	new_markov_chain->database->size = 0;
	new_markov_chain->database->first = NULL;
	new_markov_chain->database->last = NULL;

	return new_markov_chain;
}

// create new Node structure
Node *make_new_node(char data[MAX_WORD_LENGTH])
{
	MarkovNode *new_markov_node = make_markov_node(data);
	if (!new_markov_node)
	{
		fprintf(stdout, ALLOCATION_ERROR_MASSAGE);
		return NULL;
	}
	Node *new_node = (Node *) malloc(sizeof(Node));
	if (!new_node)
	{
		fprintf(stdout, ALLOCATION_ERROR_MASSAGE);
		free_markov_node(new_markov_node);
	}
	new_node->data = new_markov_node;
	new_node->next = NULL;
	return new_node;
}

void free_node(Node *node)
{
	if (!node)
	{
		return;
	}
	if (node->data)
	{
		free(node->data);
	}
	free(node);
}

void free_linked_list(LinkedList *linked_list)
{
	if (!linked_list)
	{
		return;
	}
	Node *node_ptr = linked_list->first;
	while (node_ptr)
	{
		Node *next_ptr = node_ptr->next;
		free_node(node_ptr);
		node_ptr = next_ptr;
	}
}

Node *add_to_database(MarkovChain *markov_chain, char *data_ptr)
{
	//checking if the node is already in the database
	Node *node = get_node_from_database(markov_chain, data_ptr);
	if (node)
	{
		return node;
	}
	//increasing database size by one
	markov_chain->database = realloc(markov_chain->database,
									 ((1 + markov_chain->database->size) *
									  sizeof(LinkedList)));
//	if(!new_database)
	if(!markov_chain->database)
	{ // realloc fail
		fprintf(stdout, ALLOCATION_ERROR_MASSAGE);
		free(node);
		return NULL;
	}
//	markov_chain->database = new_database;
	MarkovNode *markov_node = make_markov_node(data_ptr);
	if (!markov_node)
	{
		free(node);
		return NULL;
	}
	if (add(markov_chain->database, markov_node))
	{
//		free_linked_list(new_database);
		free(markov_node); //failed to add the node to the end of the linked list
		return NULL;
	}
	return markov_chain->database->last;
}

Node *get_node_from_database(MarkovChain *markov_chain, char *data_ptr)
{
	Node *node_ptr = markov_chain->database->first;
	while (node_ptr)
	{
		if (!strcmp(node_ptr->data->data, data_ptr))
		{
			return node_ptr;
		}
		node_ptr = node_ptr->next;
	}
	return NULL; //node data not in the database
}

//int get_total_frequency_number(MarkovChain markov_chain){
//	int total_frequency = 0;
//	Node *node_ptr = markov_chain.database->first;
//	while(node_ptr)
//	{
//		total_frequency += node_ptr->data->
//	}
//}

bool add_node_to_frequencies_list(MarkovNode *first_node, MarkovNode
*second_node)
{
	if (first_node->end_with_dot)
	{
		return true; //words with dot have empty freq list
	}
	if (!first_node->frequency_list_size)
	{ //empty freq list
		first_node->frequency_list = (MarkovNodeFrequency *)
				malloc(sizeof(MarkovNodeFrequency));
		if (!first_node->frequency_list)
		{
			fprintf(stdout, ALLOCATION_ERROR_MASSAGE);
			return false; //malloc fail
		}
		first_node->frequency_list[0]->markov_node = second_node;
		first_node->frequency_list[0]->frequency = 1;
		first_node->frequency_list_size = 1;
		return true;
	}
	//checking if the word is already in the freq list
	for (int i = 0; i < first_node->frequency_list_size; i++)
	{
		if (!strcmp(first_node->frequency_list[i]->markov_node->data,
					second_node->data))
		{
			first_node->frequency_list[i]->frequency++;
			return true;
		}
	}
	MarkovNodeFrequency **new_frequency_list = realloc
			(first_node->frequency_list, (first_node->frequency_list_size + 1)
										 * sizeof(MarkovNodeFrequency));
	if (!new_frequency_list)
	{
		fprintf(stdout, ALLOCATION_ERROR_MASSAGE);
		return false; //realloc fail
	}
	first_node->frequency_list = new_frequency_list;
	first_node->frequency_list[first_node->frequency_list_size
	]->markov_node = second_node;
	first_node->frequency_list[first_node->frequency_list_size
	]->frequency = 1;
	first_node->frequency_list_size++;
	return true;

}

MarkovNode *get_first_random_node(MarkovChain *markov_chain)
{

}

MarkovNode *get_next_random_node(MarkovNode *state_struct_ptr)
{

}

void generate_random_sequence(MarkovChain *markov_chain, MarkovNode *
first_node, int max_length)
{

}


void free_markov_chain(MarkovChain *ptr_chain)
{
	if (!ptr_chain)
	{
		return;
	}
	if (!ptr_chain->database)
	{
		free(ptr_chain);
		return;
	}
	free_linked_list(ptr_chain->database);
	free(ptr_chain);
}

int get_rand_num(int max_num)
{
	return rand() % max_num;
}

