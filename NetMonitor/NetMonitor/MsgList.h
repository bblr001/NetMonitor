#include <list>
#include <string>
#include <windows.h>
#include <memory>

//#include <boost\thread\mutex.hpp>
using namespace std;

//template <class T>
class MsgList
{
public:
	MsgList();
	~MsgList();
	static shared_ptr<MsgList> GetInstance()
	{
		if(m_pInstance == 0)
			m_pInstance = make_shared<MsgList>();			
		return m_pInstance;
	}
	void Popfront(string& val);
	void Pushback(string val);
	int  Size();
	void Clear();
private:
	list<string> m_list;
	CRITICAL_SECTION	m_csLock;
	static shared_ptr<MsgList> m_pInstance;
};
