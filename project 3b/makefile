CC = gcc
CFLAGS = -Wall -Wextra -Wvla -std=c99


all: tweets snake

tweets: tweets_generator

snake: ladders_and_snakes

tweets_generator: tweets_generator.c markov_chain.c linked_list.c
	$(CC) $(CFLAGS) tweets_generator.c markov_chain.c linked_list.c -o tweets_generator

ladders_and_snakes: snakes_and_ladders.c markov_chain.c linked_list.c
	$(CC) $(CFLAGS) snakes_and_ladders.c markov_chain.c linked_list.c -o snakes_and_ladders

clean:
	rm -f tweets_generator snakes_and_ladders