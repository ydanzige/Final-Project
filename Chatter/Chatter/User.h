#include <cpprest\asyncrt_utils.h>
#include <string>
#include <vector>

using namespace utility;
#define String utility::string_t 
#define Vector std::vector 

namespace ServerNS
{
	/**
	Contains user details and bun list of usernames 
	*/
	class User
	{
	public:
		User();
		User(String, String);
		~User();

		bool SetPassword(String);
		String GetUsername() const;
		Vector<String> GetBunUsers() const;

	private:
		String m_name;
		String m_pass;
		Vector<String> m_bunUserList;
	};
}

