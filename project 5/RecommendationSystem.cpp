//
// Created by Yotam Gaash on 20/03/2024.
//
#include "RecommendationSystem.h"
#include "User.h"
#include <unordered_map>


sp_movie RecommendationSystem::get_movie(const std::string &name, int year)
const {
    if (features_map_.empty()) {
        return nullptr;
    }
    for (const auto &it: features_map_) {
        if (it.first->get_name() == name && it.first->get_year() == year) {
            return it.first;
        }
    }
    return nullptr;
}

sp_movie RecommendationSystem::add_movie(const std::string &name, int year,
                                         const std::vector<double> &features) {
    if (name.empty() || !year || features.empty()) {
        throw std::invalid_argument(ERROR_NULL_ARGS);
    }

    sp_movie movie = std::make_shared<Movie>(name, year);
    features_map_[movie] = features;
    return movie;
}

double RecommendationSystem::predict_movie_score(const User &user,
                                                 const sp_movie &movie,
                                                 int k) {
    rank_map user_ranks = user.get_ranks();
    std::vector<double> user_pref_vector = get_user_pref_vector(user);

    double sim_sum = 0.0;
    double rank_sum = 0.0;

//comp for sp_movies by their ranks
    auto comp = [&]
            (const sp_movie &a, const sp_movie &b) {
        return cosine_similarity(features_map_[a],
                                 features_map_[movie]) >
               cosine_similarity(features_map_[b]
                                 , features_map_[movie]);
    };
    // Use a map with custom comparator to store similar movies sorted by rank
    std::map<sp_movie, double, decltype(comp)> similar_movies(comp);
    // Find k most similar movies to the given movie from the user's
    // ranked movies
    for (const auto &pair: user_ranks) {
        sp_movie ranked_movie = pair.first;
            double similarity =
                    cosine_similarity(features_map_[ranked_movie],
                                      features_map_[movie]);
            similar_movies[ranked_movie] = similarity;
    }
    int count = 0;
    for (const auto &pair: similar_movies) {
        if (count >= k) {
            break;
        }
        double similarity = pair.second;
        double rank = user_ranks[pair.first];
        sim_sum += similarity;
        rank_sum += similarity * rank;
        count++;
    }
    return rank_sum / sim_sum;
}
//    // Predict the score for the given movie based on similarity and
//    rank of similar movies
//    if (sim_sum != 0.0) {
//        return rank_sum / sim_sum;
//    } else {
//        // Return default score if no similar movies found
//        return 0; // Define DEFAULT_SCORE as appropriate
//    }
//}

//double RecommendationSystem::predict_movie_score(const User &user,
//                                                 const sp_movie &movie,
//                                                 int k) {
//    std::vector<double> user_pref_vector = get_user_pref_vector(user);
//    rank_map user_ranks = user.get_ranks();
//    std::map<sp_movie, double> rec_map;
//    int count = 0;
//    std::vector<double> movie_features = features_map_[movie];
//    double sim_sum = 0;
//    double rank_sum = 0;
//    for (const auto &pair: user_ranks) {
//        if (pair.second > 0) {
//            double movie_similarity = cosine_similarity(movie_features,
//                                                        features_map_[pair
//                                                                .first]);
//            rec_map[pair.first] = movie_similarity;
//        }
//    }
//    for (auto it = rec_map.begin(); it != rec_map.end()
//                                    && count < k; ++it,
//                 ++count) {
//        double movie_sim = it->second;
//        double movie_rank = user_ranks[it->first];
//        sim_sum += movie_sim;
//        rank_sum += movie_sim * movie_rank;
//    }
//    return rank_sum / sim_sum;
//}


std::ostream &operator<<(std::ostream &os, const RecommendationSystem &rs) {
    if (rs.features_map_.empty()) {
        return os;
    }
    for (const auto &pair: rs.features_map_) {
        os << *pair.first;
    }
    return os;
}

std::vector<double>
RecommendationSystem::get_user_pref_vector(const User &user) {
    size_t num_of_features = features_map_.begin()->second.size();
    if (num_of_features == 0) {
        throw std::runtime_error(ERROR_NO_FEATURES);
    }
    std::vector<double> user_pref_vector(num_of_features, 0.0);

    rank_map user_ranks = user.get_ranks();
    double user_avg = get_avg_ratings(user_ranks);
    for (const auto &pair: user_ranks) {
        double movie_rating = pair.second;
        if (movie_rating > 0) {
            // Calculate normalized rating
            double normalized_rating = movie_rating - user_avg;

            // Find features of the movie
            auto it = features_map_.find(pair.first);
            if (it != features_map_.end()) {
                const std::vector<double> &movie_features = it->second;

                // Update preference vector
                std::transform(user_pref_vector.begin(),
                               user_pref_vector.end(),
                               movie_features.begin(),
                               user_pref_vector.begin(),
                               [normalized_rating](double pref,
                                                   double feature) {
                                   return pref + normalized_rating * feature;
                               });
            } else {
                // Handle case where features of movie are not found
                throw std::runtime_error(
                        "Features of movie not found in features_map");
            }
        }
    }
//    for (const auto &pair: user_ranks) {
//        if (pair.second > 0) {
//            double normalized_rank = pair.second - user_avg;
//            std::vector<double> features = features_map_[pair.first];
//            for (size_t i = 0; i < num_of_features; i++) {
//                user_pref_vector[i] += normalized_rank * features[i];
//            }
//        }
//    }
    return user_pref_vector;
}

double RecommendationSystem::get_avg_ratings(const rank_map &ranks) {
    double avg_rating = 0.0;
    int num_rated_movies = 0;
    for (const auto &pair: ranks) {
        avg_rating += pair.second;
        num_rated_movies++;
    }
    if (num_rated_movies != 0) {
        avg_rating /= num_rated_movies;
    }
    return avg_rating;
}

double RecommendationSystem::cosine_similarity(const std::vector<double> &v1,
                                               const std::vector<double> &v2) {
    if (v1.empty() || v2.empty()) {
        throw std::invalid_argument(EMPTY_VECTOR_ERROR_MSG);
    }
    if (v1.size() != v2.size()) {
        throw std::invalid_argument(INVALLID_PARAMETERS_ERROR_MSG);
    }
    double dot_product = 0.0;
    double magnitude1 = 0.0;
    double magnitude2 = 0.0;

    for (std::size_t i = 0; i < v1.size(); ++i) {
        dot_product += v1[i] * v2[i];
        magnitude1 += v1[i] * v1[i];
        magnitude2 += v2[i] * v2[i];
    }
    magnitude1 = std::sqrt(magnitude1);
    magnitude2 = std::sqrt(magnitude2);
    if (magnitude1 == 0 || magnitude2 == 0) {
        throw std::runtime_error(ERROR_ZERO_MAGNITUDE);
    }
// Calculate cosine similarity
    double similarity = dot_product / (magnitude1 * magnitude2);
    return similarity;
}

sp_movie RecommendationSystem::recommend_by_content(const User &user) {
    std::vector<double> user_pref_vector = get_user_pref_vector(user);
    rank_map user_ranks = user.get_ranks();
    double best_similarity = MIN_SIMILARITY;
    sp_movie best_movie = nullptr;

    for (const auto &pair: features_map_) {
        sp_movie movie = pair.first;
        const std::vector<double> &movie_features = pair.second;
        // Check if the movie is not in user ranks
        if (user.get_ranks().find(movie) == user.get_ranks().end()) {
            // Calculate cosine similarity between movie features and user
            // preference vector
            double similarity = cosine_similarity(movie_features,
                                                  user_pref_vector);

            // Update best movie if similarity is higher
            if (similarity > best_similarity) {
                best_similarity = similarity;
                best_movie = movie;
            }
        }
    }
    return best_movie;
}

//    for (const auto &pair: user_ranks) {
//        if (pair.second == NA) {
//            std::vector<double> movie_features = features_map_[pair.first];
//            double similarity = cosine_similarity(movie_features,
//                                                  user_pref_vector);
//            if (similarity > best_movie_score) {
//                similarity = best_movie_score;
//                best_movie = pair.first;
//            }
//        }
//    }
//    return best_movie;
//}

sp_movie RecommendationSystem::recommend_by_cf(const User &user, int k) {
    rank_map user_ranks = user.get_ranks();
    std::map<sp_movie, double> predicted_scores;

    double highest_score = 0;
    sp_movie recommended_movie = nullptr;

    for (const auto &pair: features_map_) {
        sp_movie movie = pair.first;
        if (user_ranks.find(movie) == user_ranks.end()) {
            double score = predict_movie_score(user, movie, k);

            if (score > highest_score) {
                highest_score = score;
                recommended_movie = movie;
            }
        }
    }
    return recommended_movie;
}



