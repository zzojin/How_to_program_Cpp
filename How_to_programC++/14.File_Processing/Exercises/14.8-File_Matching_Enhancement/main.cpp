#include <iostream>
#include <fstream>
#include <cstdlib>

void handleMultipleTransactions(std::istream &input, int account, double *balance );

int main()
{
	std::ifstream inOldMaster( "oldmast.dat", std::ios::in );
	if ( !inOldMaster ) {
		std::cerr << "Failed to open file oldmast.dat" << std::endl;
		std::exit( EXIT_FAILURE );
	}

	std::ifstream inTransaction( "trans.dat", std::ios::in );
	if ( !inTransaction ) {
		std::cerr << "Failed to open file trans.dat" << std::endl;
		std::exit( EXIT_FAILURE );
	}

	std::ofstream outNewMaster( "newmast.dat", std::ios::out );
	if ( !outNewMaster ) {
		std::cerr << "Failed to open file newmast.dat" << std::endl;
		std::exit( EXIT_FAILURE );
	}

	int accountNum;
	double dollarAmount;

	// transaction data
	while ( inTransaction >> accountNum >> dollarAmount )
	{
		int accountNumber;
		std::string firstName;
		std::string lastName;
		double currentBalance;

		// old master data
		while ( inOldMaster >> accountNumber >> firstName >> lastName >> currentBalance )
		{
			if ( accountNumber < accountNum ) {
				outNewMaster << accountNumber << '\t' << firstName << " " << lastName << '\t' << currentBalance << std::endl;
				continue;
			}

			// assumed ascending order for each account
			if ( accountNumber == accountNum ) {
				handleMultipleTransactions( inTransaction, accountNumber, &currentBalance );
				currentBalance += dollarAmount;
			}
			else if ( accountNumber > accountNum ) { // no transaction match in master data
				std::cerr << "Unmatched transaction record for account number " << accountNum << std::endl;
				handleMultipleTransactions( inTransaction, accountNum, &dollarAmount );
				outNewMaster << accountNum << '\t' << " " << "\t\t" << dollarAmount << std::endl;
			}

			outNewMaster << accountNumber << '\t' << firstName << " " << lastName << '\t' << currentBalance << std::endl;
			break;
		}

		if ( accountNumber < accountNum ) {
			std::cerr << "Unmatched transaction record for account number " << accountNum << std::endl;
			handleMultipleTransactions( inTransaction, accountNum, &dollarAmount );
			outNewMaster << accountNum << '\t' << " " << "\t\t" << dollarAmount << std::endl;
		}
	}

	inOldMaster.close();
	inTransaction.close();
	outNewMaster.close();

	return 0;
}

void handleMultipleTransactions(std::istream &input, int accountNumber, double *balance )
{
	int count(0);
	int account;
	double amount;
	int pos = input.tellg();

	while ( input >> account >> amount ) 
	{
		if ( account == accountNumber ) 
		{
			// update balance
			*balance += amount;
			++count;
		}
		else {
			break;
		}
		pos = input.tellg();
	}

	input.seekg( pos );
}
