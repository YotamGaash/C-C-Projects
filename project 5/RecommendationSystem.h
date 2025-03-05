//
// RecommendationSystem.h
//

#ifndef RECOMMENDATIONSYSTEM_H
#define RECOMMENDATIONSYSTEM_H
#include <unordered_map>
#include <map>
#include <set>
#include <memory>
#include "Movie.h"

#define ERROR_NULL_ARGS "one or more arguments in NULL"
#define ERROR_FEATURES_MAP "Features map is not initialized"
#define ERROR_MOVIE_EXISTS "Movie already exists in the features map"
#define ERROR_ZERO_MAGNITUDE "Magnitude is zero in cosine similarity calculation"
#define ERROR_NO_FEATURES "No features for movies is rs"
#define MIN_SIMILARITY -1.0
typedef std::shared_ptr<Movie> sp_movie;
typedef std::vector<double> features_vector;
typedef std::map<sp_movie , features_vector , movie_cmp> features_map;
typedef bool rec_eq_func(const double&, const double&);
typedef std::unordered_map<sp_movie, double, hash_func,equal_func> rank_map;
typedef std::set<sp_movie, movie_cmp> movies_set;
class User;



class RecommendationSystem
{
public:

	//explicit RecommendationSystem()

	RecommendationSystem() = default;

    /**
     * adds a new movie to the system
     * @param name name of movie
     * @param year year it was made
     * @param features features for movie
     * @return shared pointer for movie in system
     */
	sp_movie add_movie(const std::string& name,int year,
                       const std::vector<double>& features);


    /**
     * a function that calculates the movie with highest score based on
     * movie features
     * @param ranks user ranking to use for algorithm
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_content(const User& user);

    /**
     * a function that calculates the movie with highest predicted
     * score based on ranking of other movies_
     * @param ranks user ranking to use for algorithm
     * @param k
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_cf(const User& user, int k);


    /**
     * Predict a user rating for a movie given argument using item
     * cf procedure with k most similar movies_.
     * @param user_rankings: ranking to use
     * @param movie: movie to predict
     * @param k:
     * @return score based on algorithm as described in pdf
     */
	double predict_movie_score(const User &user, const sp_movie &movie,
												  int k);

	/**
	 * gets a shared pointer to movie in system
	 * @param name name of movie
	 * @param year year movie was made
	 * @return shared pointer to movie in system
	 */
	sp_movie get_movie(const std::string &name, int year) const;


	friend std::ostream& operator<<(std::ostream& os, const
	RecommendationSystem& rs);
private:


	typedef std::map<sp_movie, std::vector<double>, movie_cmp> features_map;

	features_map features_map_;
//	int num_of_features_;
//	/**
//	 * returns the rs feature map.
//	 * @return
//	 */
//	features_map get_features_map(){
//		return features_map_;}
	/**
	 * get a users preference vector
	 * @param user the user for which to get the pref vector
	 * @return a vector of weights for the user features preferences
	 */
	std::vector<double> get_user_pref_vector(const User& user);


	/**
	 * get the avg movie ratings for a user.
	 * @param ranks the rank map of the user
	 * @return
	 */
	double get_avg_ratings(const rank_map& ranks);

	/**
	 * gets the cosine similarity between two vectors
	 * @param v1
	 * @param v2
	 * @return v1*v2 / (||v1|| * ||v2||)
	 */
	double cosine_similarity(const std::vector<double>& v1,const
	std::vector<double>& v2);
};


//	inline bool rec_comp_func(const double& a,const double& b){
//	return a > b;
//}


#endif //RECOMMENDATIONSYSTEM_H
