module airline_ticket;

AirlineTicket::AirlineTicket() : m_passengerName{ "Unknown Passenger" }, m_numberOfMiles{ 0 }, m_hasEliteSuperRewardsStatus{ false }
{
	/*
		m_passengerName = "Unknown Passenger";
		m_numberOfMiles = 0;
		m_hasEliteSuperRewardsStatus = false;
	*/
}

AirlineTicket::~AirlineTicket()
{

}

double AirlineTicket::calculatePriceInDollars()
{
	if (hasEliteSuperRewardsStatus()) {
		// 우수 등급
		return 0;
	}
	return getNumberOfMiles() * 0.1;
}

std::string AirlineTicket::getPassengerName() { 
	return m_passengerName;
}

void AirlineTicket::setPassengerName(std::string name) {
	m_passengerName = name;
}

int AirlineTicket::getNumberOfMiles() {
	return m_numberOfMiles;
}

void AirlineTicket::setNumberOfMiles(int miles) {
	m_numberOfMiles = miles;
}

bool AirlineTicket::hasEliteSuperRewardsStatus() {
	return m_hasEliteSuperRewardsStatus;
}

void AirlineTicket::setHasEliteSuperRewardsStatus(bool status) {
	m_hasEliteSuperRewardsStatus = status;
}