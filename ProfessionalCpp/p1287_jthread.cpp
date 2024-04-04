import "HeaderUnits.h";

/*
std::jthread : 소멸자에서 자동으로 조인, 협조적 취소 제공
std::stop_token : stop_requested() 를 주기적으로 호출해서 취소해야하는지 확인
std::stop_source : stop_requested() 로 멈춤 요청이 이미 들어왔는지 검사
get_stop_token()
get_stop_source()

*/

int main1287()
{
	std::jthread job([](stop_token token) {
		while (!token.stop_requested())
		{

		}
	});

	// 다른 스레드에서 job.request_stop(); 을 통해 취소요청을 할 수 있다


	return 0;
}