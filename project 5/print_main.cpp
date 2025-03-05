//
//print_main.cpp
//

#include "RecommendationSystemLoader.h"
#include "UsersLoader.h"
//#include <function>
#include <functional>


void test(){
	features_map fm;
	sp_movie m1 = std::make_shared<Movie>("A",1999);
	sp_movie m2 = std::make_shared<Movie>("B",1999);
	sp_movie m3 = std::make_shared<Movie>("C",2000);
	fm[m1] = {1,2,3};
	fm[m2] = {1,2,3};
	fm[m3] = {1,2,3};
//	for (const auto& pair: fm)
//	{
//		std::cout << *pair.first;
//	}

	RecommendationSystem rs;
	rs.add_movie("a",10,{1});
	rs.add_movie("a",2,{1,2});
	rs.add_movie("b",1,{1.0000, 2.4});
	std::cout << rs;
}

//int main()
//{
//
////	test();
//	auto rs = RecommendationSystemLoader::create_rs_from_movies
//			("RecommendationSystemLoader_input.txt");
////	rs->add_movie("a", 122, {0, 0, 0,1});
//////	rs->add_movie("a", 122, {1, 2, 3,44});
////	rs->add_movie("b", 100, {-10001,0, 0,0});
//
////	std::cout << "rs created successfully" << std::endl;
////	std::cout << *rs << std::endl;
//    auto users = UsersLoader::create_users
//			("UsersLoader_input.txt",std::move(rs));
//
//    users[2].add_movie_to_rs("test",1000, {1,1,1,1},10);
//    sp_movie m = users[0].get_recommendation_by_content();
//    double score = users[2].get_prediction_score_for_movie("Twilight",2008,2);
////    std::cout << *m << " is reccomanded to: " << users[0];
//    std::cout << "user nicole score for test movie is: " << score;
////    sp_movie n = users[2].get_recommendation_by_cf(2);
////    std::cout << *n;
////    std::cout << users[0] << std::endl;
//
//}
