
//user.cpp

// don't change those includes
#include "User.h"

#include <utility>
#include "RecommendationSystem.h"


#define NO_RANKED_MOVIES_ERROR_MSG "User hasn't ranked any movies_"

//constructor Implementation
User::User(std::string username, rank_map rankings, sp_rs rs)
		: name_(std::move(username)), ranks_(std::move(rankings)),
		rs_(std::move(rs))
{
//	avg_ratings_ = get_avg_ratings();
//	preference_vector_ = create_preference_vector();
}

// Getter for username implementation
std::string User::get_name() const
{
	return name_;
}

// Function for adding a movie to the DB implementation
void User::add_movie_to_rs(const std::string &name, int year,
						   const std::vector<double> &features, double rate)
{
	if(rate > MAX_RANK || rate < MIN_RANK){
		throw std::out_of_range(INVALID_RANK_ERROR_MSG);
	}
	sp_movie movie = rs_->add_movie(name, year, features);
	ranks_[movie] = rate;

}

// Getter for the ranks map implementation
rank_map User::get_ranks() const
{
	return ranks_;
}

// Returns a recommendation according to the movie's content implementation
sp_movie User::get_recommendation_by_content() const
{
	return rs_->recommend_by_content(*this);
}

// Returns a recommendation according to the similarity recommendation
// method implementation
sp_movie User::get_recommendation_by_cf(int k) const
{
	return rs_->recommend_by_cf(*this, k);
}

// Predicts the score for a given movie implementation
double User::get_prediction_score_for_movie(const std::string &name, int year,
											int k) const
{
	sp_movie movie = rs_->get_movie(name, year);
    if (movie == nullptr){
        throw std::runtime_error(ERROR_MOVIE_NOT_FOUND);
    }
	return rs_->predict_movie_score(*this, movie, k);
}

std::ostream &operator<<(std::ostream &os, const User &user)
{
	os << "name: " << user.get_name() << std::endl;
	os << *user.rs_ << std::endl;
	return os;
}

//{
//double User::get_avg_ratings()
//{
//	double avg_rating = 0.0;
//	int num_rated_movies = 0;
//	for(const auto& pair: ranks_){
//		if(pair.second > 0){
//			avg_rating+= pair.second;
//			++num_rated_movies;
//		}
//	}
//	if(num_rated_movies != 0){
//		avg_rating /= num_rated_movies;
//	}
//	return avg_rating;
//std::vector<double> User::create_normalized_ranking(){
////	double avg_rating = get_avg_ratings();
//	if(avg_ratings_== 0){
//		throw std::invalid_argument(NO_RANKED_MOVIES_ERROR_MSG);
//	}
//	std::vector<double> normalized_rankings;
//	for(const auto& pair: ranks_){
//		if(pair.second > 0){
//			normalized_rankings.push_back(pair.second - avg_ratings_);
//		}
//		else{
//			normalized_rankings.push_back(NA);
//		}
//	}
//	return normalized_rankings;
//double User::cosine_similarity(const std::vector<double>& vec1,
// const std::vector<double>& vec2) const {
//	// Calculate dot product
//	if (vec1.empty() || vec2.empty()) {
//		throw std::invalid_argument(EMPTY_VECTOR_ERROR_MSG);
//	}
//	if(vec1.size() != vec2.size()){
//		throw std::invalid_argument(INVALLID_PARAMETERS_ERROR_MSG);
//	}
//	double dot_product = 0.0;
//	for (std::size_t i = 0; i < vec1.size(); ++i) {
//		dot_product += vec1[i] * vec2[i];
//	}
//
//	// Calculate magnitudes
//	double magnitude1 = 0.0;
//	double magnitude2 = 0.0;
//	for (std::size_t i = 0; i < vec1.size(); ++i) {
//		magnitude1 += vec1[i] * vec1[i];
//		magnitude2 += vec2[i] * vec2[i];
//	}
//	magnitude1 = std::sqrt(magnitude1);
//	magnitude2 = std::sqrt(magnitude2);
//
//	// Calculate cosine similarity
//	double similarity = dot_product / (magnitude1 * magnitude2);
//	return similarity;
// Operator<< overload for output stream implementation

//}

//}



//}

//std::vector<double> User::normalize(const std::vector<double> &vec)
//	std::vector<double> normalized_vec;
//	for(double i : vec){
//		normalized_vec.push_back(i - avg_ratings_);
//	}
//	return normalized_vec;
//}
