
//Movie.cpp
#include "Movie.h"
#define HASH_START 17
#define RES_MULT 31

//movie cotr getting a movie name and year of release
Movie::Movie(const std::string& name, int year) : name_(name), year_(year) {}

//name getter
const std::string& Movie::get_name() const {
	return name_;
}

//year getter
int Movie::get_year() const {
	return year_;
}

// Operator< overload implementation
bool operator<(const Movie& lhs, const Movie& rhs) {
	return (lhs.year_ < rhs.year_) ||
    (lhs.year_ == rhs.year_ && lhs.name_ < rhs.name_);
}

// Operator<< overload implementation
std::ostream& operator<<(std::ostream& os, const Movie& movie) {
	os << "<" << movie.name_ << "> (" << movie.year_ << ")" << std::endl;
	return os;
}
/**
 * hash function used for a unordered_map (implemented for you)
 * @param movie shared pointer to movie
 * @return an integer for the hash map
 */
std::size_t sp_movie_hash(const sp_movie& movie){
    std::size_t res = HASH_START;
    res = res * RES_MULT + std::hash<std::string>()(movie->get_name());
    res = res * RES_MULT + std::hash<int>()(movie->get_year());
    return res;
}

/**
 * equal function used for an unordered_map (implemented for you)
 * @param m1
 * @param m2
 * @return true if the year and name are equal else false
 */
bool sp_movie_equal(const sp_movie& m1,const sp_movie& m2){
    return !(*m1 < *m2) && !(*m2 < *m1);
}
