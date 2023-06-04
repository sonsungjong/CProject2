#include <cstdio>

import airline_ticket;

int main0108()
{
	AirlineTicket myTicket;
	myTicket.setPassengerName("Sherman T. Socketwrench");
	myTicket.setNumberOfMiles(700);
	double cost { myTicket.calculatePriceInDollars() };
	printf("This ticket will cost %lf\n", cost);

	return 0;
}