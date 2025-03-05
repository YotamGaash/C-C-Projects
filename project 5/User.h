//
// User.h
//

#ifndef USER_H
#define USER_H
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <stdexcept>
//#include "RecommendationSystem.h"
#include "Movie.h"

#define NA 0.0
#define MAX_RANK 10.0
#define MIN_RANK 1.0

#define INVALLID_PARAMETERS_ERROR_MSG "invalid parameters for the function"
#define EMPTY_VECTOR_ERROR_MSG "Input vector cannot be empty"
#define ZERO_LENGTH_ERROR_MSG "Input vector cannot have zero length"
#define INVALID_RANK_ERROR_MSG "ranking value must be between 1 to 10"

#define ERROR_MOVIE_NOT_FOUND "Movie not in recommendation system"

class RecommendationSystem;
typedef std::unordered_map<sp_movie, double, hash_func,equal_func> rank_map;
typedef std::shared_ptr<RecommendationSystem> sp_rs;
class User
{
public:

	typedef std::shared_ptr<Movie> sp_movie;
	/**
	 * Constructor for the class
	 */
	User(std::string  username,
		 rank_map  rankings,
		 sp_rs rs);
	/**
	 * a getter for the user's name
	 * @return the username
	 */
	std::string get_name() const;

	/**
	 * function for adding a movie to the DB
	 * @param name name of movie
     * @param year year it was made
	 * @param features a vector of the movie's features
	 * @param rate the user rate for this movie
	 */
	void add_movie_to_rs(const std::string &name, int year,
                         const std::vector<double> &features,
                         double rate);


    /**
     * a getter for the ranks map
     * @return
     */
    rank_map get_ranks() const;

	/**
	 * returns a recommendation according to the movie's content
	 * @return recommendation
	 */
	sp_movie get_recommendation_by_content() const;

	/**
	 * returns a recommendation according to the similarity recommendation
	 *	method
	 * @param k the number of the most similar movies_ to calculate by
	 * @return recommendation
	 */
	sp_movie get_recommendation_by_cf(int k) const;

	/**
	 * predicts the score for a given movie
	 * @param name the name of the movie
	 * @param year the year the movie was created
	 * @param k the parameter which represents the number of the most similar
	 * movies_ to predict the score by
	 * @return predicted score for the given movie
	 */
	double get_prediction_score_for_movie(const std::string& name,
										  int year, int k) const;

	/**
	 * output stream operator
	 * @param os the output stream
	 * @param user the user
	 * @return output stream
	 */
	friend std::ostream& operator<<(std::ostream& os, const User& user);

	private:
		std::string name_;
		rank_map ranks_;
		sp_rs rs_; // Pointer to RecommendationSystem
		double avg_ratings_{};
		std::vector<double> preference_vector_;
		friend RecommendationSystem;

//		/**
//		 * returns the normalized vector according to the Exercise definition
//		 * @param vec doubles vector
//		 * @return normalized vector
//		 */
//		std::vector<double> normalize(const std::vector<double>& vec);
//		/**
//		 * returns the overall preference vector of the user
//		 * @return
//		 */
//		std::vector<double>create_preference_vector();
//
//		/**
//		 * finds the cosine of the angle between two vectors calculated by
//		 * (vec1 * vec2) / (||vec1|| * ||vec2||)
//		 * @param vec1 vec1
//		 * @param vec2 vec2
//		 * @return cosine between two vectors
//		 */
//		double cosine_similarity(const std::vector<double> &vec1,
//							 const std::vector<double> &vec2) const;
//
//		/**
//		 * get the vector of the avg user preferences based on all his movie
//		 * rankings
//		 * @return
//		 */
//		std::vector<double> create_normalized_ranking();
//
//	double get_avg_ratings();
};



#endif //USER_H
