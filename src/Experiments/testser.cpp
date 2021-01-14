#include <cereal/archives/binary.hpp>
#include <sstream>
#include <cereal/types/string.hpp>
#include <cereal/types/common.hpp>
#include <fstream>

struct test_ep
{
	int id;
	std::string title;
	//char* test_charar;

	template<class Archive>
	void serialize(Archive & ar)
	{
		ar(id,title);
	}

};



void test()
{


	//std::stringstream ss;
	std::ofstream ss("out.text", std::ios::binary);

	cereal::BinaryOutputArchive oarchive(ss);

	oarchive(test_ep{ 0,"title1" });
	oarchive(test_ep{ 1,"title2" });
	//oarchive(test_ep{ 2,"title3" });
	
	ss.flush();
	ss.close();

	std::ifstream is("out.text", std::ios::binary);
	std::streambuf* pbuf = is.rdbuf();
	std::streamsize size = pbuf->pubseekoff(0, is.end);
	pbuf->pubseekoff(0, is.beg);
	
	
	cereal::BinaryInputArchive iarchive(is);

	test_ep res1, res2, res3;
	iarchive(res1);
	iarchive(res2);
	iarchive(res3);


	fprintf(stdout, "test ep title: %s", res1.title);

}