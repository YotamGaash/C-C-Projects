//RecommendationSystemLoader.h

#ifndef RECOMMENDATIONSYSTEMLOADER_H
#define RECOMMENDATIONSYSTEMLOADER_H
#include "RecommendationSystem.h"
#include <memory>
class RecommendationSystemLoader {

 private:

 public:
//  RecommendationSystemLoader () = delete;
  /**
   * loads movies_ by the given format for movies_ with their feature's score
   * @param movies_file_path a path to the file of the movies_
   * @return smart pointer to a RecommendationSystem which was created with
   * those movies_
   */
   //changed to static unique ptr
  static std::unique_ptr<RecommendationSystem> create_rs_from_movies
	  (const std::string &movies_file_path) noexcept (false);
};

#endif //RECOMMENDATIONSYSTEMLOADER_H
